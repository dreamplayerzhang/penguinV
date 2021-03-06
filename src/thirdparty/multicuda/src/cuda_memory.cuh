#pragma once

#include <cstdint>
#include <cuda_runtime.h>
#include <map>
#include <set>
#include <vector>
#include "multicuda_exception.h"

namespace multiCuda
{
    // Class for memory allocation on devices with CUDA support
    class MemoryAllocator
    {
    public:
        explicit MemoryAllocator( size_t availableSize )
            : _data         ( nullptr )
            , _size         ( 0 )
            , _availableSize( availableSize )
        {
            if( _availableSize == 0 )
                throw multiCudaException( "Available size cannot be 0" );
        }

        ~MemoryAllocator()
        {
            _free();
        }

        // this function allocates a chunk of memory on devices with CUDA support
        // we recommend to call this function only one time at the startup of an application
        // do not reallocate memory if some objects in your source code are allocated
        // through this allocator. Future access to such object's memory is unpredictable
        void reserve( size_t size )
        {
            if( size == 0 )
                throw multiCudaException( "Memory size cannot be 0" );

            if( size > _availableSize )
                throw multiCudaException( "Memory size to be allocated is bigger than available size on CUDA device" );

            if( _data != nullptr && size == _size )
                return;

            _allocate( size );

            size_t usedSize = 0;

            while( size > 0 ) {
                uint8_t levelCount = _getAllocationLevel( size );
                size_t value = static_cast<size_t>(1) << levelCount;

                if( value > size ) {
                    value >>= 1;
                    --levelCount;
                }

                if( usedSize == 0 )
                    _freeChunck.resize( levelCount + 1 );

                _freeChunck[levelCount].insert( usedSize );

                usedSize += value;
                size -= value;
            }
        }

        // this function returns a pointer to an allocated memory
        // if memory size on allocated chuck of memory is enough for requested size
        // so the function just assigns a pointer to preallocated memory
        // otherwise the function will allocate a new chuck of memory just for this pointer
        template <typename _DataType>
        _DataType* allocate( size_t size = 1 )
        {
            size = size * sizeof( _DataType );

            if( _data != nullptr && size < _size ) {
                const uint8_t level = _getAllocationLevel( size );

                if( _split( level ) ) {
                    _DataType* address = reinterpret_cast<_DataType*>(static_cast<uint8_t*>(_data) + *_freeChunck[level].begin());
                    _allocatedChunck.insert( std::pair<size_t, uint8_t >( *_freeChunck[level].begin(), level ) );
                    _freeChunck[level].erase( _freeChunck[level].begin() );
                    return address;
                }
            }

            // if no space in preallocated memory just allocate as usual memory
            _DataType* address = nullptr;
            cudaError_t error = cudaMalloc( &address, size );
            if( error != cudaSuccess )
                throw multiCudaException( "Cannot allocate a memory for CUDA device" );

            return address;
        }

        // returns true if memory allocator has enough space for specified size in bytes
        bool isSpaceAvailable( size_t size = 1 ) const
        {
            if( _data != nullptr && size < _size ) {
                const uint8_t level = _getAllocationLevel( size );

                for( uint8_t i = level; i < _freeChunck.size(); ++i ) {
                    if( !_freeChunck[i].empty() )
                        return true;
                }
            }

            return false;
        }

        // deallocates a memory by input address
        // if a pointer points on allocated chuck of memory inside the allocator then
        // the allocator just removes a reference to such area without any cost
        // otherwise CUDA specific function will be called
        void free( void * address )
        {
            if( _data != nullptr && address >= _data ) {
                std::map <size_t, uint8_t>::iterator pos =
                    _allocatedChunck.find( static_cast<uint8_t*>(address) - static_cast<uint8_t*>(_data) );

                if( pos != _allocatedChunck.end() ) {
                    _freeChunck[pos->second].insert( pos->first );
                    _merge( pos->first, pos->second );
                    _allocatedChunck.erase( pos );
                    return;
                }
            }

            cudaError_t error = cudaFree( address );
            if( error != cudaSuccess )
                throw multiCudaException( "Cannot deallocate memory for CUDA device" );
        }

        // this function returns maximum availbale space which could be allocated by allocator
        size_t availableSize() const
        {
            return _availableSize;
        }
    private:
        void * _data; // a pointer to memory allocated chunk
        size_t _size; // a size of memory allocated chunk

        // an array which holds an information about free memory in preallocated memory chunck
        std::vector < std::set < size_t > > _freeChunck;
        // a map which holds an information about allocated memory in preallocated memory chunck
        // first parameter is an offset from preallocated memory
        // second parameter is a power of 2 (level)
        std::map <size_t, uint8_t> _allocatedChunck;

        size_t _availableSize; // maximum available memory size on CUDA device

        // the function for true memory allocation on devices with CUDA support
        void _allocate( size_t size )
        {
            if( _size != size && size > 0 ) {
                if( !_allocatedChunck.empty() )
                    throw multiCudaException( "Cannot free a memory on device with CUDA support. Not all objects were previously deallocated from allocator." );

                _free();

                cudaError_t error = cudaMalloc( &_data, size );
                if( error != cudaSuccess )
                    throw multiCudaException( "Cannot allocate a memory for CUDA device" );

                _size = size;
            }
        }

        // the function for true memory deallocation on devices with CUDA support
        void _free()
        {
            if( _data != nullptr ) {
                cudaError_t error = cudaFree( _data );
                if( error != cudaSuccess )
                    throw multiCudaException( "Cannot deallocate memory for CUDA device" );
            }

            _freeChunck.clear();
            _allocatedChunck.clear();

            _size = 0;
        }

        // returns a level (power of 2) needed for a required size
        static uint8_t _getAllocationLevel( size_t initialSize )
        {
            size_t size = 1;
            uint8_t level = 0;

            while( size < initialSize ) {
                size <<= 1;
                ++level;
            }

            return level;
        }

        // split the preallocated memory by levels
        bool _split( uint8_t from )
        {
            bool levelFound = false;
            uint8_t startLevel = from;

            for( uint8_t i = from; i < _freeChunck.size(); ++i ) {
                if( !_freeChunck[i].empty() ) {
                    startLevel = i;
                    levelFound = true;
                    break;
                }
            }

            if( !levelFound )
                return false;

            if( startLevel > from ) {
                size_t memorySize = static_cast<size_t>(1) << (startLevel - 1);

                for( ; startLevel > from; --startLevel, memorySize >>= 1 ) {
                    _freeChunck[startLevel - 1].insert( *_freeChunck[startLevel].begin() );
                    _freeChunck[startLevel - 1].insert( *_freeChunck[startLevel].begin() + memorySize );
                    _freeChunck[startLevel].erase( _freeChunck[startLevel].begin() );
                }
            }

            return true;
        }

        // merge preallocated memory by levels
        void _merge( size_t offset, uint8_t from )
        {
            size_t memorySize = static_cast<size_t>(1) << from;

            for( std::vector < std::set < size_t > >::iterator level = _freeChunck.begin() + from; level < _freeChunck.end();
                 ++level, memorySize <<= 1 ) {
                std::set< size_t >::iterator pos = level->find( offset );
                std::set< size_t >::iterator neighbour = pos;
                ++neighbour;

                if( neighbour != level->end() ) {
                    if( *(neighbour) - *(pos) == memorySize ) {
                        offset = *pos;
                        (level + 1)->insert( offset );
                        level->erase( pos, ++neighbour );
                        continue;
                    }
                }

                if( pos != level->begin() ) {
                    neighbour = pos;
                    --neighbour;

                    if( *(pos) - *(neighbour) == memorySize ) {
                        offset = *neighbour;
                        (level + 1)->insert( offset );
                        level->erase( neighbour, ++pos );
                        continue;
                    }
                }

                return;
            }
        }

        MemoryAllocator(const MemoryAllocator & ) {}
        MemoryAllocator & operator=( const MemoryAllocator & ) { return (*this); }
    };
}
