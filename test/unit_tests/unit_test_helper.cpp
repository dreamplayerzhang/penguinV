#include "unit_test_helper.h"
#include "../../src/image_exception.h"
#include "../../src/image_function.h"
#include "../../src/parameter_validation.h"

namespace
{
    uint32_t randomSize()
    {
        return Unit_Test::randomValue<uint32_t>( 1, 2048 );
    }

    PenguinV_Image::Image generateImage( uint32_t width, uint32_t height, uint8_t colorCount, uint8_t value )
    {
        PenguinV_Image::Image image( width, height, colorCount );

        image.fill( value );

        return image;
    }
}

namespace Unit_Test
{
    PenguinV_Image::Image uniformImage()
    {
        return uniformImage( randomValue<uint8_t>( 256 ) );
    }

    PenguinV_Image::Image uniformImage( uint8_t value )
    {
        return generateImage( randomSize(), randomSize(), PenguinV_Image::GRAY_SCALE, value );
    }

    PenguinV_Image::Image uniformRGBImage()
    {
        return uniformRGBImage( randomValue<uint8_t>( 256 ) );
    }

    PenguinV_Image::Image uniformRGBImage( uint8_t value )
    {
        return generateImage( randomSize(), randomSize(), PenguinV_Image::RGB, value );
    }

    PenguinV_Image::Image blackImage()
    {
        return uniformImage( 0u );
    }

    PenguinV_Image::Image whiteImage()
    {
        return uniformImage( 255u );
    }

    PenguinV_Image::Image randomImage()
    {
        PenguinV_Image::Image image( randomSize(), randomSize() );

        uint8_t * outY = image.data();
        const uint8_t * outYEnd = outY + image.height() * image.rowSize();

        for( ; outY != outYEnd; outY += image.rowSize() ) {
            uint8_t * outX = outY;
            const uint8_t * outXEnd = outX + image.width();

            for( ; outX != outXEnd; ++outX )
                (*outX) = randomValue<uint8_t>( 256 );
        }

        return image;
    }

    PenguinV_Image::Image randomImage( const std::vector <uint8_t> & value )
    {
        if( value.empty() )
            return randomImage();

        PenguinV_Image::Image image( randomSize(), randomSize() );

        uint8_t * outY = image.data();
        const uint8_t * outYEnd = outY + image.height() * image.rowSize();

        size_t id = 0;

        for( ; outY != outYEnd; outY += image.rowSize() ) {
            uint8_t * outX = outY;
            const uint8_t * outXEnd = outX + image.width();

            for( ; outX != outXEnd; ++outX ) {
                (*outX) = value[id++];
                if( id == value.size() )
                    id = 0;
            }
        }

        return image;
    }

    std::vector < PenguinV_Image::Image > uniformImages( uint32_t images )
    {
        if( images == 0 )
            throw imageException( "Invalid parameter" );

        std::vector < PenguinV_Image::Image > image;

        image.push_back( uniformImage() );

        image.resize( images );

        for( size_t i = 1; i < image.size(); ++i ) {
            image[i].resize( image[0].width(), image[0].height() );
            image[i].fill( randomValue<uint8_t>( 256 ) );
        }

        return image;
    }

    template <typename data>
    std::vector < data > generateArray( uint32_t size, uint32_t maximumValue )
    {
        std::vector < data > fillArray( size );

        std::for_each( fillArray.begin(), fillArray.end(), [&]( data & value ) { value = randomValue<data>( maximumValue ); } );

        return fillArray;
    }

    uint8_t intensityValue()
    {
        return randomValue<uint8_t>( 255 );
    }

    std::vector < uint8_t > intensityArray( uint32_t size )
    {
        return generateArray<uint8_t>( size, 256u );
    }

    std::vector < PenguinV_Image::Image > uniformImages( std::vector < uint8_t > intensityValue )
    {
        if( intensityValue.size() == 0 )
            throw imageException( "Invalid parameter" );

        std::vector < PenguinV_Image::Image > image;

        image.push_back( uniformImage( intensityValue[0] ) );

        image.resize( intensityValue.size() );

        for( size_t i = 1; i < image.size(); ++i ) {
            image[i].resize( image[0].width(), image[0].height() );
            image[i].fill( intensityValue[i] );
        }

        return image;
    }

    bool equalSize( const PenguinV_Image::Image & image, uint32_t width, uint32_t height )
    {
        return image.width() == width && image.height() == height && !image.empty();
    }

    bool verifyImage( const PenguinV_Image::Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t value )
    {
        if( image.empty() || width == 0 || height == 0 || x + width > image.width() || y + height > image.height() )
            throw imageException( "Bad input parameters in image function" );

        const uint8_t * outputY = image.data() + y * image.rowSize() + x * image.colorCount();
        const uint8_t * endY    = outputY + image.rowSize() * height;

        for( ; outputY != endY; outputY += image.rowSize() ) {
            const uint8_t * outputX = outputY;
            const uint8_t * endX    = outputX + width * image.colorCount();

            for( ; outputX != endX; ++outputX ) {
                if( (*outputX) != value )
                    return false;
            }
        }

        return true;
    }

    bool verifyImage( const PenguinV_Image::Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                      const std::vector < uint8_t > & value )
    {
        if( image.empty() || width == 0 || height == 0 || x + width > image.width() || y + height > image.height() )
            throw imageException( "Bad input parameters in image function" );

        const uint8_t * outputY = image.data() + y * image.rowSize() + x * image.colorCount();
        const uint8_t * endY    = outputY + image.rowSize() * height;

        for( ; outputY != endY; outputY += image.rowSize() ) {
            const uint8_t * outputX = outputY;
            const uint8_t * endX    = outputX + width * image.colorCount();

            for( ; outputX != endX; ++outputX ) {
                bool equal = false;

                for( std::vector < uint8_t >::const_iterator v = value.begin(); v != value.end(); ++v ) {
                    if( (*outputX) == (*v) ) {
                        equal = true;
                        break;
                    }
                }

                if( !equal )
                    return false;
            }
        }

        return true;
    }

    bool verifyImage( const PenguinV_Image::Image & image, uint8_t value )
    {
        return verifyImage( image, 0, 0, image.width(), image.height(), value );
    }

    bool verifyImage( const PenguinV_Image::Image & image, const std::vector < uint8_t > & value )
    {
        return verifyImage( image, 0, 0, image.width(), image.height(), value );
    }

    void fillImage( PenguinV_Image::Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t value )
    {
        Image_Function::Fill( image, x, y, width, height, value );
    }

    void fillImage( PenguinV_Image::Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                    const std::vector < uint8_t > & value )
    {
        Image_Function::ParameterValidation( image, x, y, width, height );

        uint8_t * outputY = image.data() + y * image.rowSize() + x;
        const uint8_t * endY    = outputY + image.rowSize() * height;

        size_t id = 0;

        for( ; outputY != endY; outputY += image.rowSize() ) {
            uint8_t * outputX = outputY;
            const uint8_t * endX    = outputX + width;

            for( ; outputX != endX; ++outputX ) {
                (*outputX) = value[id++];
                if( id == value.size() )
                    id = 0;
            }
        }
    }

    void generateRoi( const PenguinV_Image::Image & image, uint32_t & x, uint32_t & y, uint32_t & width, uint32_t & height )
    {
        width  = randomValue<uint32_t>( 1, image.width()  + 1 );
        height = randomValue<uint32_t>( 1, image.height() + 1 );

        x = randomValue<uint32_t>( image.width()  - width );
        y = randomValue<uint32_t>( image.height() - height );
    }

    void generateRoi( const std::vector < PenguinV_Image::Image > & image, std::vector < uint32_t > & x, std::vector < uint32_t > & y,
                      uint32_t & width, uint32_t & height )
    {
        std::vector < std::pair < uint32_t, uint32_t> > imageSize( image.size() );

        for( size_t i = 0; i < image.size(); ++i )
            imageSize[i] = std::pair < uint32_t, uint32_t >( image[i].width(), image[i].height() ) ;

        generateRoi( imageSize, x, y, width, height );
    }

    void generateRoi( const std::vector < std::pair< uint32_t, uint32_t > > & imageSize, std::vector < uint32_t > & x,
                      std::vector < uint32_t > & y, uint32_t & width, uint32_t & height )
    {
        uint32_t maximumWidth  = 0;
        uint32_t maximumHeight = 0;

        for( std::vector < std::pair< uint32_t, uint32_t > >::const_iterator im = imageSize.begin();
             im != imageSize.end(); ++im ) {
            if( maximumWidth == 0 )
                maximumWidth = im->first;
            else if( maximumWidth > im->first )
                maximumWidth = im->first;

            if( maximumHeight == 0 )
                maximumHeight = im->second;
            else if( maximumHeight > im->second )
                maximumHeight = im->second;
        }

        width  = randomValue<uint32_t>( 1, maximumWidth  + 1 );
        height = randomValue<uint32_t>( 1, maximumHeight + 1 );

        x.resize( imageSize.size() );
        y.resize( imageSize.size() );

        for( size_t i = 0; i < imageSize.size(); ++i ) {
            x[i] = randomValue<uint32_t>( imageSize[i].first  - width );
            y[i] = randomValue<uint32_t>( imageSize[i].second - height );
        }
    }

    void generateOffset( const PenguinV_Image::Image & image, uint32_t & x, uint32_t & y, uint32_t width, uint32_t height )
    {
        x = randomValue<uint32_t>( image.width()  - width );
        y = randomValue<uint32_t>( image.height() - height );
    }

    std::pair <uint32_t, uint32_t> imageSize( const PenguinV_Image::Image & image )
    {
        return std::pair <uint32_t, uint32_t>( image.width(), image.height() );
    }

    uint32_t rowSize( uint32_t width, uint8_t colorCount, uint8_t alignment )
    {
        uint32_t size = width * colorCount;
        if( size % alignment != 0 )
            size = ((size / alignment) + 1) * alignment;

        return size;
    }

    uint32_t runCount()
    {
        return 1024u; // some magic number for loop. Higher value = higher chance to verify all possible situations
    }
}
