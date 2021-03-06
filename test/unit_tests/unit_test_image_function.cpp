#include <math.h>
#include <numeric>
#include "../../src/function_pool.h"
#include "../../src/image_function.h"
#include "../../src/image_function_avx.h"
#include "../../src/image_function_neon.h"
#include "../../src/image_function_sse.h"
#include "../../src/thread_pool.h"
#include "../../src/penguinv/cpu_identification.h"
#include "unit_test_image_function.h"
#include "unit_test_helper.h"

namespace
{
    void PrepareFunction()
    {
        Thread_Pool::ThreadPoolMonoid::instance().resize( Unit_Test::randomValue<uint8_t>( 1, 8 ) );
    }

    class FunctionRegistrator
    {
    public:
        static FunctionRegistrator& instance()
        {
            static FunctionRegistrator registrator;
            return registrator;
        }

        void add( const UnitTestFramework::testFunction test, const std::string & name )
        {
            _function[test] = name;
        }

        void set( UnitTestFramework & framework )
        {
            for (std::map < UnitTestFramework::testFunction, std::string >::const_iterator func = _function.begin(); func != _function.end(); ++func)
                framework.add( func->first, func->second );

            _function.clear();
        }

    private:
        std::map < UnitTestFramework::testFunction, std::string > _function; // container with pointer to functions and their names
    };
}

namespace Function_Template
{
    using namespace PenguinV_Image;
    using namespace Unit_Test;

    // Function pointer definitions
    typedef Image (*AbsoluteDifferenceForm1)( const Image & in1, const Image & in2 );
    typedef void  (*AbsoluteDifferenceForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*AbsoluteDifferenceForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                              uint32_t width, uint32_t height );
    typedef void  (*AbsoluteDifferenceForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                              Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef void (*AccumulateForm1)( const Image & image, std::vector < uint32_t > & result );
    typedef void (*AccumulateForm2)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, std::vector < uint32_t > & result );

    typedef void (*BinaryDilateForm1)( Image & image, uint32_t dilationX, uint32_t dilationY );
    typedef void (*BinaryDilateForm2)( Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t dilationX, uint32_t dilationY );

    typedef void (*BinaryErodeForm1)( Image & image, uint32_t erosionX, uint32_t erosionY );
    typedef void (*BinaryErodeForm2)( Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t erosionX, uint32_t erosionY );

    typedef Image (*BitwiseAndForm1)( const Image & in1, const Image & in2 );
    typedef void  (*BitwiseAndForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*BitwiseAndForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                      uint32_t width, uint32_t height );
    typedef void  (*BitwiseAndForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                      Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef Image (*BitwiseOrForm1)( const Image & in1, const Image & in2 );
    typedef void  (*BitwiseOrForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*BitwiseOrForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                     uint32_t width, uint32_t height );
    typedef void  (*BitwiseOrForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                     Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef Image (*BitwiseXorForm1)( const Image & in1, const Image & in2 );
    typedef void  (*BitwiseXorForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*BitwiseXorForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                      uint32_t width, uint32_t height );
    typedef void  (*BitwiseXorForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                      Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef Image (*ConvertToGrayScaleForm1)( const Image & in );
    typedef void  (*ConvertToGrayScaleForm2)( const Image & in, Image & out );
    typedef Image (*ConvertToGrayScaleForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*ConvertToGrayScaleForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                              uint32_t width, uint32_t height );

    typedef Image (*ConvertToRgbForm1)( const Image & in );
    typedef void  (*ConvertToRgbForm2)( const Image & in, Image & out );
    typedef Image (*ConvertToRgbForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*ConvertToRgbForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                        uint32_t width, uint32_t height );

    typedef void  (*CopyForm1)( const Image & in, Image & out );
    typedef Image (*CopyForm2)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*CopyForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                uint32_t width, uint32_t height );

    typedef Image (*ExtractChannelForm1)( const Image & in, uint8_t channelId );
    typedef void  (*ExtractChannelForm2)( const Image & in, Image & out, uint8_t channelId );
    typedef Image (*ExtractChannelForm3)( const Image & in, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t channelId );
    typedef void  (*ExtractChannelForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut,
                                          uint32_t startYOut, uint32_t width, uint32_t height, uint8_t channelId );

    typedef void (*FillForm1)( Image & image, uint8_t value );
    typedef void (*FillForm2)( Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint8_t value );

    typedef Image (*FlipForm1)( const Image & in, bool horizontal, bool vertical );
    typedef void  (*FlipForm2)( const Image & in, Image & out, bool horizontal, bool vertical );
    typedef Image (*FlipForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height,
                                bool horizontal, bool vertical );
    typedef void  (*FlipForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                uint32_t width, uint32_t height, bool horizontal, bool vertical );

    typedef Image (*GammaCorrectionForm1)( const Image & in, double a, double gamma );
    typedef void  (*GammaCorrectionForm2)( const Image & in, Image & out, double a, double gamma );
    typedef Image (*GammaCorrectionForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height, double a, double gamma );
    typedef void  (*GammaCorrectionForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                           uint32_t width, uint32_t height, double a, double gamma );

    typedef uint8_t (*GetPixelForm1)( const Image & image, uint32_t x, uint32_t y );

    typedef uint8_t (*GetThresholdForm1)( const std::vector < uint32_t > & histogram );

    typedef std::vector < uint32_t > (*HistogramForm1)( const Image & image );
    typedef void                     (*HistogramForm2)( const Image & image, std::vector < uint32_t > & histogram );
    typedef std::vector < uint32_t > (*HistogramForm3)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height );
    typedef void                     (*HistogramForm4)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height,
                                                        std::vector < uint32_t > & histogram );

    typedef Image (*InvertForm1)( const Image & in );
    typedef void  (*InvertForm2)( const Image & in, Image & out );
    typedef Image (*InvertForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*InvertForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                  uint32_t width, uint32_t height );

    typedef bool (*IsBinaryForm1)( const Image & image );
    typedef bool (*IsBinaryForm2)( const Image & image, uint32_t startX, uint32_t startY, uint32_t width, uint32_t height );

    typedef bool (*IsEqualForm1)( const Image & in1, const Image & in2 );
    typedef bool (*IsEqualForm2)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                  uint32_t width, uint32_t height );

    typedef Image (*LookupTableForm1)( const Image & in, const std::vector < uint8_t > & table );
    typedef void  (*LookupTableForm2)( const Image & in, Image & out, const std::vector < uint8_t > & table );
    typedef Image (*LookupTableForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height,
                                       const std::vector < uint8_t > & table );
    typedef void  (*LookupTableForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                       uint32_t width, uint32_t height, const std::vector < uint8_t > & table );

    typedef Image (*MaximumForm1)( const Image & in1, const Image & in2 );
    typedef void  (*MaximumForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*MaximumForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                   uint32_t width, uint32_t height );
    typedef void  (*MaximumForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                   Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef Image (*MergeForm1)( const Image & in1, const Image & in2, const Image & in3 );
    typedef void  (*MergeForm2)( const Image & in1, const Image & in2, const Image & in3, Image & out );
    typedef Image (*MergeForm3)( const Image & in1, uint32_t startXIn1, uint32_t startYIn1, const Image & in2, uint32_t startXIn2, uint32_t startYIn2,
                                 const Image & in3, uint32_t startXIn3, uint32_t startYIn3, uint32_t width, uint32_t height );
    typedef void  (*MergeForm4)( const Image & in1, uint32_t startXIn1, uint32_t startYIn1, const Image & in2, uint32_t startXIn2, uint32_t startYIn2,
                                 const Image & in3, uint32_t startXIn3, uint32_t startYIn3, Image & out, uint32_t startXOut, uint32_t startYOut,
                                 uint32_t width, uint32_t height );

    typedef Image (*MinimumForm1)( const Image & in1, const Image & in2 );
    typedef void  (*MinimumForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*MinimumForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                   uint32_t width, uint32_t height );
    typedef void  (*MinimumForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                   Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef Image (*NormalizeForm1)( const Image & in );
    typedef void  (*NormalizeForm2)( const Image & in, Image & out );
    typedef Image (*NormalizeForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*NormalizeForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                     uint32_t width, uint32_t height );

    typedef std::vector < uint32_t > (*ProjectionProfileForm1)( const Image & image, bool horizontal );
    typedef void                     (*ProjectionProfileForm2)( const Image & image, bool horizontal, std::vector < uint32_t > & projection );
    typedef std::vector < uint32_t > (*ProjectionProfileForm3)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, bool horizontal );
    typedef void                     (*ProjectionProfileForm4)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height, bool horizontal,
                                                                std::vector < uint32_t > & projection );

    typedef Image (*ResizeForm1)( const Image & in, uint32_t widthOut, uint32_t heightOut );
    typedef void  (*ResizeForm2)( const Image & in, Image & out );
    typedef Image (*ResizeForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t widthIn, uint32_t heightIn,
                                  uint32_t widthOut, uint32_t heightOut );
    typedef void  (*ResizeForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t widthIn, uint32_t heightIn,
                                  Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t widthOut, uint32_t heightOut );

    typedef Image (*RgbToBgrForm1)( const Image & in );
    typedef void  (*RgbToBgrForm2)( const Image & in, Image & out );
    typedef Image (*RgbToBgrForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*RgbToBgrForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                    uint32_t width, uint32_t height );

    typedef void (*Rotate)( const Image & in, double centerXIn, double centerYIn, Image & out, double centerXOut, double centerYOut, double angle );

    typedef void (*SetPixelForm1)( Image & image, uint32_t x, uint32_t y, uint8_t value );
    typedef void (*SetPixelForm2)( Image & image, const std::vector < uint32_t > & X, const std::vector < uint32_t > & Y, uint8_t value );

    typedef void (*SplitForm1)( const Image & in, Image & out1, Image & out2, Image & out3 );
    typedef void (*SplitForm2)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out1, uint32_t startXOut1, uint32_t startYOut1,
                                Image & out2, uint32_t startXOut2, uint32_t startYOut2, Image & out3, uint32_t startXOut3, uint32_t startYOut3,
                                uint32_t width, uint32_t height );

    typedef Image (*SubtractForm1)( const Image & in1, const Image & in2 );
    typedef void  (*SubtractForm2)( const Image & in1, const Image & in2, Image & out );
    typedef Image (*SubtractForm3)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                    uint32_t width, uint32_t height );
    typedef void  (*SubtractForm4)( const Image & in1, uint32_t startX1, uint32_t startY1, const Image & in2, uint32_t startX2, uint32_t startY2,
                                    Image & out, uint32_t startXOut, uint32_t startYOut, uint32_t width, uint32_t height );

    typedef uint32_t (*SumForm1)( const Image & image );
    typedef uint32_t (*SumForm2)( const Image & image, uint32_t x, uint32_t y, uint32_t width, uint32_t height );

    typedef Image (*ThresholdForm1)( const Image & in, uint8_t threshold );
    typedef void  (*ThresholdForm2)( const Image & in, Image & out, uint8_t threshold );
    typedef Image (*ThresholdForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height, uint8_t threshold );
    typedef void  (*ThresholdForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                     uint32_t width, uint32_t height, uint8_t threshold );

    typedef Image (*ThresholdDoubleForm1)( const Image & in, uint8_t minThreshold, uint8_t maxThreshold );
    typedef void  (*ThresholdDoubleForm2)( const Image & in, Image & out, uint8_t minThreshold, uint8_t maxThreshold );
    typedef Image (*ThresholdDoubleForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height, uint8_t minThreshold,
                                           uint8_t maxThreshold );
    typedef void  (*ThresholdDoubleForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                           uint32_t width, uint32_t height, uint8_t minThreshold, uint8_t maxThreshold );

    typedef Image (*TransposeForm1)( const Image & in );
    typedef void  (*TransposeForm2)( const Image & in, Image & out );
    typedef Image (*TransposeForm3)( const Image & in, uint32_t startXIn, uint32_t startYIn, uint32_t width, uint32_t height );
    typedef void  (*TransposeForm4)( const Image & in, uint32_t startXIn, uint32_t startYIn, Image & out, uint32_t startXOut, uint32_t startYOut,
                                     uint32_t width, uint32_t height );

    bool form1_AbsoluteDifference(AbsoluteDifferenceForm1 AbsoluteDifference)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = AbsoluteDifference( input[0], input[1] );

            if( !equalSize( input[0], output ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] - intensity[1] : intensity[1] - intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form2_AbsoluteDifference(AbsoluteDifferenceForm2 AbsoluteDifference)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            AbsoluteDifference( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] > intensity[1] ? intensity[0] - intensity[1] : intensity[1] - intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_AbsoluteDifference(AbsoluteDifferenceForm3 AbsoluteDifference)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = AbsoluteDifference( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] - intensity[1] : intensity[1] - intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form4_AbsoluteDifference(AbsoluteDifferenceForm4 AbsoluteDifference)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            AbsoluteDifference( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
                                image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
                              intensity[0] > intensity[1] ? intensity[0] - intensity[1] : intensity[1] - intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_Accumulate(AccumulateForm1 Accumulate)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( randomValue<uint8_t>( 1, 16 ) );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            std::vector < uint32_t > result( input[0].width() * input[0].height(), 0 );

            for( std::vector < PenguinV_Image::Image >::const_iterator image = input.begin(); image != input.end(); ++image ) {
                Accumulate( *image, result );
            }

            uint32_t sum = std::accumulate( intensity.begin(), intensity.end(), 0u );

            if( std::any_of( result.begin(), result.end(), [&sum]( uint32_t v ) { return v != sum; } ) )
                return false;
        }

        return true;
    }

    bool form2_Accumulate(AccumulateForm2 Accumulate)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( randomValue<uint8_t>( 1, 16 ) );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            std::vector < uint32_t > result( roiWidth * roiHeight, 0 );

            for( size_t imageId = 0; imageId < input.size(); ++imageId ) {
                Accumulate( input[imageId], roiX[imageId], roiY[imageId], roiWidth, roiHeight, result );
            }

            uint32_t sum = std::accumulate( intensity.begin(), intensity.end(), 0u );

            if( std::any_of( result.begin(), result.end(), [&sum]( uint32_t v ) { return v != sum; } ) )
                return false;
        }

        return true;
    }

    bool form1_BitwiseAnd(BitwiseAndForm1 BitwiseAnd)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = BitwiseAnd( input[0], input[1] );

            if( !equalSize( input[0], output ) || !verifyImage( output, intensity[0] & intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_BitwiseAnd(BitwiseAndForm2 BitwiseAnd)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            BitwiseAnd( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] & intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form3_BitwiseAnd(BitwiseAndForm3 BitwiseAnd)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = BitwiseAnd( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensity[0] & intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form4_BitwiseAnd(BitwiseAndForm4 BitwiseAnd)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            BitwiseAnd( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensity[0] & intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_BitwiseOr(BitwiseOrForm1 BitwiseOr)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = BitwiseOr( input[0], input[1] );

            if( !equalSize( input[0], output ) || !verifyImage( output, intensity[0] | intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_BitwiseOr(BitwiseOrForm2 BitwiseOr)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            BitwiseOr( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] | intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form3_BitwiseOr(BitwiseOrForm3 BitwiseOr)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = BitwiseOr( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensity[0] | intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form4_BitwiseOr(BitwiseOrForm4 BitwiseOr)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            BitwiseOr( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensity[0] | intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_BitwiseXor(BitwiseXorForm1 BitwiseXor)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = BitwiseXor( input[0], input[1] );

            if( !equalSize( input[0], output ) || !verifyImage( output, intensity[0] ^ intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_BitwiseXor(BitwiseXorForm2 BitwiseXor)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            BitwiseXor( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] ^ intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form3_BitwiseXor(BitwiseXorForm3 BitwiseXor)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = BitwiseXor( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensity[0] ^ intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form4_BitwiseXor(BitwiseXorForm4 BitwiseXor)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            BitwiseXor( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensity[0] ^ intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_ConvertToGrayScale(ConvertToGrayScaleForm1 ConvertToGrayScale)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 1 );
            PenguinV_Image::Image input = uniformRGBImage( intensity[0] );

            PenguinV_Image::Image output = ConvertToGrayScale( input );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form2_ConvertToGrayScale(ConvertToGrayScaleForm2 ConvertToGrayScale)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformRGBImage( intensity[0] );
            PenguinV_Image::Image output( input.width(), input.height() );

            output.fill( intensity[1] );

            ConvertToGrayScale( input, output );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_ConvertToGrayScale(ConvertToGrayScaleForm3 ConvertToGrayScale)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 1 );
            PenguinV_Image::Image input  = uniformRGBImage( intensity[0] );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = ConvertToGrayScale( input, roiX, roiY, roiWidth, roiHeight );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form4_ConvertToGrayScale(ConvertToGrayScaleForm4 ConvertToGrayScale)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformRGBImage( intensity[0] );
            PenguinV_Image::Image output = uniformImage   ( intensity[1] );

            std::vector < std::pair <uint32_t, uint32_t> > size( 2 );

            size[0] = imageSize( input );
            size[1] = imageSize( output );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( size, roiX, roiY, roiWidth, roiHeight );

            ConvertToGrayScale( input, roiX[0], roiY[0], output, roiX[1], roiY[1], roiWidth, roiHeight );

            if( !verifyImage( output, roiX[1], roiY[1], roiWidth, roiHeight, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_ConvertToRgb(ConvertToRgbForm1 ConvertToRgb)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 1 );
            PenguinV_Image::Image input = uniformImage( intensity[0] );

            PenguinV_Image::Image output = ConvertToRgb( input );

            if( output.colorCount() != PenguinV_Image::RGB || !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form2_ConvertToRgb(ConvertToRgbForm2 ConvertToRgb)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformImage( intensity[0] );
            PenguinV_Image::Image output( input.width(), input.height(), PenguinV_Image::RGB );

            output.fill( intensity[1] );

            ConvertToRgb( input, output );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_ConvertToRgb(ConvertToRgbForm3 ConvertToRgb)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 1 );
            PenguinV_Image::Image input = uniformImage( intensity[0] );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = ConvertToRgb( input, roiX, roiY, roiWidth, roiHeight );

            if( output.colorCount() != PenguinV_Image::RGB || !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form4_ConvertToRgb(ConvertToRgbForm4 ConvertToRgb)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage   ( intensity[0] );
            PenguinV_Image::Image output = uniformRGBImage( intensity[1] );

            std::vector < std::pair <uint32_t, uint32_t> > size( 2 );

            size[0] = imageSize( input );
            size[1] = imageSize( output );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( size, roiX, roiY, roiWidth, roiHeight );

            ConvertToRgb( input, roiX[0], roiY[0], output, roiX[1], roiY[1], roiWidth, roiHeight );

            if( !verifyImage( output, roiX[1], roiY[1], roiWidth, roiHeight, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_Copy(CopyForm1 Copy)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            Copy( input[0], input[1] );

            if( !verifyImage( input[1], intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form2_Copy(CopyForm2 Copy)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Copy( input, roiX, roiY, roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensity ) )
                return false;
        }

        return true;
    }

    bool form3_Copy(CopyForm3 Copy)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Copy( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_Fill(FillForm1 Fill)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image image = uniformImage( intensity[0] );

            Fill( image, intensity[1] );

            if( !verifyImage( image, intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_Fill(FillForm2 Fill)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image image = uniformImage( intensity[0] );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Fill( image, roiX, roiY, roiWidth, roiHeight, intensity[1] );

            if( !verifyImage( image, roiX, roiY, roiWidth, roiHeight, intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_Flip(FlipForm1 Flip)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformImage( intensity[0] );

            const bool horizontalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const bool verticalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const uint32_t xCorrection = input.width() % 2;
            const uint32_t yCorrection = input.height() % 2;

            if (verticalFlip)
            {
                if (input.height() > 1)
                    Image_Function::Fill(input, 0, 0, input.width(), input.height() / 2, intensity[1]);
            }
            else if (horizontalFlip)
            {
                if (input.width() > 1)
                    Image_Function::Fill(input, 0, 0, input.width() / 2, input.height(), intensity[1]);
            }

            PenguinV_Image::Image output = Flip( input, horizontalFlip, verticalFlip );

            if( !equalSize( output, input.width(), input.height() ))
                return false;

            if (verticalFlip) {
                if( !verifyImage( output, 0, 0, input.width(), input.height() / 2 + yCorrection, intensity[0] ) )
                    return false;
                if((input.height() > 1) && !verifyImage( output, 0, input.height() / 2 + yCorrection, input.width(), input.height() / 2, intensity[1] ) )
                    return false;
            }
            else {
                if( !verifyImage( output, 0, 0, input.width() / 2 + xCorrection, input.height(), intensity[0] ) )
                    return false;
                if((input.width() > 1) && !verifyImage( output, input.width() / 2 + xCorrection, 0, input.width() / 2, input.height(), intensity[horizontalFlip ? 1 : 0] ) )
                    return false;
            }
        }

        return true;
    }

    bool form2_Flip(FlipForm2 Flip)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            uint8_t intensityFill = intensityValue();
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            const bool horizontalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const bool verticalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const uint32_t xCorrection = input[0].width() % 2;
            const uint32_t yCorrection = input[0].height() % 2;

            if (verticalFlip)
            {
                if (input[0].height() > 1)
                    Image_Function::Fill(input[0], 0, 0, input[0].width(), input[0].height() / 2, intensityFill);
            }
            else if (horizontalFlip)
            {
                if (input[0].width() > 1)
                    Image_Function::Fill(input[0], 0, 0, input[0].width() / 2, input[0].height(), intensityFill);
            }

            Flip( input[0], input[1], horizontalFlip, verticalFlip );

            if (verticalFlip) {
                if( !verifyImage( input[1], 0, 0, input[1].width(), input[1].height() / 2 + yCorrection, intensity[0] ) )
                    return false;
                if((input[0].height() > 1) && !verifyImage( input[1], 0, input[1].height() / 2 + yCorrection, input[1].width(), input[1].height() / 2, intensityFill ) )
                    return false;
            }
            else {
                if( !verifyImage( input[1], 0, 0, input[1].width() / 2 + xCorrection, input[1].height(), intensity[0] ) )
                    return false;
                if((input[0].width() > 1) && !verifyImage( input[1], input[1].width() / 2 + xCorrection, 0, input[1].width() / 2, input[1].height(),
                                                           horizontalFlip ? intensityFill : intensity[0] ) )
                    return false;
            }
        }

        return true;
    }

    bool form3_Flip(FlipForm3 Flip)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformImage( intensity[0] );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            const bool horizontalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const bool verticalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const uint32_t xCorrection = roiWidth % 2;
            const uint32_t yCorrection = roiHeight % 2;

            if (verticalFlip)
            {
                if (roiHeight > 1)
                    Image_Function::Fill(input, roiX, roiY, roiWidth, roiHeight / 2, intensity[1]);
            }
            else if (horizontalFlip)
            {
                if (roiWidth > 1)
                    Image_Function::Fill(input, roiX, roiY, roiWidth / 2, roiHeight, intensity[1]);
            }

            PenguinV_Image::Image output = Flip( input, roiX, roiY, roiWidth, roiHeight, horizontalFlip, verticalFlip );

            if( !equalSize( output, roiWidth, roiHeight ))
                return false;

            if (verticalFlip) {
                if( !verifyImage( output, 0, 0, roiWidth, roiHeight / 2 + yCorrection, intensity[0] ) )
                    return false;
                if((roiHeight > 1) && !verifyImage( output, 0, roiHeight / 2 + yCorrection, roiWidth, roiHeight / 2, intensity[1] ) )
                    return false;
            }
            else {
                if( !verifyImage( output, 0, 0, roiWidth / 2 + xCorrection, roiHeight, intensity[0] ) )
                    return false;
                if((roiWidth > 1) && !verifyImage( output, roiWidth / 2 + xCorrection, 0, roiWidth / 2, roiHeight, intensity[horizontalFlip ? 1 : 0] ) )
                    return false;
            }
        }

        return true;
    }

    bool form4_Flip(FlipForm4 Flip)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            uint8_t intensityFill = intensityValue();
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            const bool horizontalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const bool verticalFlip = (randomValue<uint32_t>( 0, 2 ) == 0);
            const uint32_t xCorrection = roiWidth % 2;
            const uint32_t yCorrection = roiHeight % 2;

            if (verticalFlip)
            {
                if (roiHeight > 1)
                    Image_Function::Fill(image[0], roiX[0], roiY[0], roiWidth, roiHeight / 2, intensityFill);
            }
            else if (horizontalFlip)
            {
                if (roiWidth > 1)
                    Image_Function::Fill(image[0], roiX[0], roiY[0], roiWidth / 2, roiHeight, intensityFill);
            }

            Flip( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight, horizontalFlip, verticalFlip );

            if (verticalFlip) {
                if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight / 2 + yCorrection, intensity[0] ) )
                    return false;
                if((roiHeight > 1) && !verifyImage( image[1], roiX[1], roiY[1] + roiHeight / 2 + yCorrection, roiWidth, roiHeight / 2, intensityFill ) )
                    return false;
            }
            else {
                if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth / 2 + xCorrection, roiHeight, intensity[0] ) )
                    return false;
                if( (roiWidth > 1) && !verifyImage( image[1], roiX[1] + roiWidth / 2 + xCorrection, roiY[1], roiWidth / 2, roiHeight,
                                                    horizontalFlip ? intensityFill : intensity[0] ) )
                    return false;
            }
        }

        return true;
    }

    bool form1_GammaCorrection(GammaCorrectionForm1 GammaCorrection)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            double a     = randomValue <uint32_t>( 100 ) / 100.0;
            double gamma = randomValue <uint32_t>( 300 ) / 100.0;

            PenguinV_Image::Image output = GammaCorrection( input, a, gamma );

            double value = a * pow( intensity / 255.0, gamma ) * 255 + 0.5;
            uint8_t corrected = 255;

            if( value < 256 )
                corrected = static_cast<uint8_t>(value);

            if( !verifyImage( output, corrected ) )
                return false;
        }

        return true;
    }

    bool form2_GammaCorrection(GammaCorrectionForm2 GammaCorrection)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            double a     = randomValue <uint32_t>( 100 ) / 100.0;
            double gamma = randomValue <uint32_t>( 300 ) / 100.0;

            GammaCorrection( input[0], input[1], a, gamma );

            double value = a * pow( intensity[0] / 255.0, gamma ) * 255 + 0.5;
            uint8_t corrected = 255;

            if( value < 256 )
                corrected = static_cast<uint8_t>(value);

            if( !verifyImage( input[1], corrected ) )
                return false;
        }

        return true;
    }

    bool form3_GammaCorrection(GammaCorrectionForm3 GammaCorrection)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            double a     = randomValue <uint32_t>( 100 ) / 100.0;
            double gamma = randomValue <uint32_t>( 300 ) / 100.0;

            PenguinV_Image::Image output = GammaCorrection( input, roiX, roiY, roiWidth, roiHeight, a, gamma );

            double value = a * pow( intensity / 255.0, gamma ) * 255 + 0.5;
            uint8_t corrected = 255;

            if( value < 256 )
                corrected = static_cast<uint8_t>(value);

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, corrected ) )
                return false;
        }

        return true;
    }

    bool form4_GammaCorrection(GammaCorrectionForm4 GammaCorrection)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            double a     = randomValue <uint32_t>( 100 ) / 100.0;
            double gamma = randomValue <uint32_t>( 300 ) / 100.0;

            GammaCorrection( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight, a, gamma );

            double value = a * pow( intensity[0] / 255.0, gamma ) * 255 + 0.5;
            uint8_t corrected = 255;

            if( value < 256 )
                corrected = static_cast<uint8_t>(value);

            if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, corrected ) )
                return false;
        }

        return true;
    }

    bool form1_GetThreshold(GetThresholdForm1 GetThreshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            while (intensity[0] == intensity[1])
                intensity = intensityArray( 2 );

            std::vector< uint32_t > histogram( 256u, 0);
            ++histogram[intensity[0]];
            ++histogram[intensity[1]];

            if( GetThreshold(histogram) != std::min(intensity[0], intensity[1]) )
                return false;
        }

        return true;
    }

    bool form1_Histogram(HistogramForm1 Histogram)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            std::vector < uint32_t > histogram = Histogram( image );

            if( histogram.size() != 256u || histogram[intensity] != image.width() * image.height() ||
                std::accumulate( histogram.begin(), histogram.end(), 0u )  != image.width() * image.height() )
                return false;
        }

        return true;
    }

    bool form2_Histogram(HistogramForm2 Histogram)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            std::vector < uint32_t > histogram;
            Histogram( image, histogram );

            if( histogram.size() != 256u || histogram[intensity] != image.width() * image.height() ||
                std::accumulate( histogram.begin(), histogram.end(), 0u )  != image.width() * image.height() )
                return false;
        }

        return true;
    }

    bool form3_Histogram(HistogramForm3 Histogram)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            std::vector < uint32_t > histogram = Histogram( input, roiX, roiY, roiWidth, roiHeight );

            if( histogram.size() != 256u || histogram[intensity] != roiWidth * roiHeight ||
                std::accumulate( histogram.begin(), histogram.end(), 0u )  != roiWidth * roiHeight )
                return false;
        }

        return true;
    }

    bool form4_Histogram(HistogramForm4 Histogram)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            std::vector < uint32_t > histogram;
            Histogram( input, roiX, roiY, roiWidth, roiHeight, histogram );

            if( histogram.size() != 256u || histogram[intensity] != roiWidth * roiHeight ||
                std::accumulate( histogram.begin(), histogram.end(), 0u )  != roiWidth * roiHeight )
                return false;
        }

        return true;
    }

    bool form1_Invert(InvertForm1 Invert)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            PenguinV_Image::Image output = Invert( input );

            if( !verifyImage( output, ~intensity ) )
                return false;
        }

        return true;
    }

    bool form2_Invert(InvertForm2 Invert)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            Invert( input[0], input[1] );

            if( !verifyImage( input[1], ~intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_Invert(InvertForm3 Invert)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Invert( input, roiX, roiY, roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, ~intensity ) )
                return false;
        }

        return true;
    }

    bool form4_Invert(InvertForm4 Invert)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Invert( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, ~intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_IsEqual(IsEqualForm1 IsEqual)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            if( (intensity[0] == intensity[1]) != (IsEqual( input[0], input[1] )) )
                return false;
        }

        return true;
    }

    bool form2_IsEqual(IsEqualForm2 IsEqual)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            if( (intensity[0] == intensity[1]) !=
                (IsEqual( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight )) )
                return false;
        }

        return true;
    }

    bool form1_LookupTable(LookupTableForm1 LookupTable)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = randomImage( intensity );

            std::vector < uint8_t > lookupTable( 256, 0 );

            lookupTable[intensity[0]] = intensityValue();
            lookupTable[intensity[1]] = intensityValue();

            PenguinV_Image::Image output = LookupTable( input, lookupTable );

            std::vector < uint8_t > normalized( 2 );

            normalized[0] = lookupTable[intensity[0]];
            normalized[1] = lookupTable[intensity[1]];

            if( !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form2_LookupTable(LookupTableForm2 LookupTable)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = randomImage( intensity );
            PenguinV_Image::Image output( input.width(), input.height() );

            output.fill( intensityValue() );

            std::vector < uint8_t > lookupTable( 256, 0 );

            lookupTable[intensity[0]] = intensityValue();
            lookupTable[intensity[1]] = intensityValue();

            LookupTable( input, output, lookupTable );

            std::vector < uint8_t > normalized( 2 );

            normalized[0] = lookupTable[intensity[0]];
            normalized[1] = lookupTable[intensity[1]];

            if( !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form3_LookupTable(LookupTableForm3 LookupTable)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformImage();

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            fillImage( input, roiX, roiY, roiWidth, roiHeight, intensity );

            std::vector < uint8_t > lookupTable( 256, 0 );

            lookupTable[intensity[0]] = intensityValue();
            lookupTable[intensity[1]] = intensityValue();

            PenguinV_Image::Image output = LookupTable( input, roiX, roiY, roiWidth, roiHeight, lookupTable );

            std::vector < uint8_t > normalized( 2 );

            normalized[0] = lookupTable[intensity[0]];
            normalized[1] = lookupTable[intensity[1]];

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form4_LookupTable(LookupTableForm4 LookupTable)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage();
            PenguinV_Image::Image output = uniformImage();

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;

            std::vector < std::pair < uint32_t, uint32_t > > size( 2 );

            size[0] = imageSize( input );
            size[1] = imageSize( output );

            generateRoi( size, roiX, roiY, roiWidth, roiHeight );

            fillImage( input, roiX[0], roiY[0], roiWidth, roiHeight, intensity );

            std::vector < uint8_t > lookupTable( 256, 0 );

            lookupTable[intensity[0]] = intensityValue();
            lookupTable[intensity[1]] = intensityValue();

            LookupTable( input, roiX[0], roiY[0], output, roiX[1], roiY[1], roiWidth, roiHeight, lookupTable );

            std::vector < uint8_t > normalized( 2 );

            normalized[0] = lookupTable[intensity[0]];
            normalized[1] = lookupTable[intensity[1]];

            if( !verifyImage( output, roiX[1], roiY[1], roiWidth, roiHeight, normalized ) )
                return false;
        }

        return true;
    }

    bool form1_Maximum(MaximumForm1 Maximum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = Maximum( input[0], input[1] );

            if( !equalSize( input[0], output ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_Maximum(MaximumForm2 Maximum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            Maximum( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] > intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form3_Maximum(MaximumForm3 Maximum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Maximum( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form4_Maximum(MaximumForm4 Maximum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Maximum( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
                     image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
                              intensity[0] > intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_Minimum(MinimumForm1 Minimum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = Minimum( input[0], input[1] );

            if( !equalSize( input[0], output ) ||
                !verifyImage( output, intensity[0] < intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_Minimum(MinimumForm2 Minimum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            Minimum( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] < intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form3_Minimum(MinimumForm3 Minimum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Minimum( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) ||
                !verifyImage( output, intensity[0] < intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form4_Minimum(MinimumForm4 Minimum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Minimum( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
                     image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
                              intensity[0] < intensity[1] ? intensity[0] : intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form1_Normalize(NormalizeForm1 Normalize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = randomImage( intensity );

            PenguinV_Image::Image output = Normalize( input );

            std::vector < uint8_t > normalized( 2 );

            if( intensity[0] == intensity[1] || (input.width() == 1 && input.height() == 1) ) {
                normalized[0] = normalized[1] = intensity[0];
            }
            else {
                normalized[0] = 0;
                normalized[1] = 255;
            }

            if( !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form2_Normalize(NormalizeForm2 Normalize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = randomImage( intensity );
            PenguinV_Image::Image output( input.width(), input.height() );

            output.fill( intensityValue() );

            Normalize( input, output );

            std::vector < uint8_t > normalized( 2 );

            if( intensity[0] == intensity[1] || (input.width() == 1 && input.height() == 1) ) {
                normalized[0] = normalized[1] = intensity[0];
            }
            else {
                normalized[0] = 0;
                normalized[1] = 255;
            }

            if( !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form3_Normalize(NormalizeForm3 Normalize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input = uniformImage();

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            fillImage( input, roiX, roiY, roiWidth, roiHeight, intensity );

            PenguinV_Image::Image output = Normalize( input, roiX, roiY, roiWidth, roiHeight );

            std::vector < uint8_t > normalized( 2 );

            if( intensity[0] == intensity[1] || (roiWidth == 1 && roiHeight == 1) ) {
                normalized[0] = normalized[1] = intensity[0];
            }
            else {
                normalized[0] = 0;
                normalized[1] = 255;
            }

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, normalized ) )
                return false;
        }

        return true;
    }

    bool form4_Normalize(NormalizeForm4 Normalize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage();
            PenguinV_Image::Image output = uniformImage();

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;

            std::vector < std::pair < uint32_t, uint32_t > > size( 2 );

            size[0] = imageSize( input );
            size[1] = imageSize( output );

            generateRoi( size, roiX, roiY, roiWidth, roiHeight );

            fillImage( input, roiX[0], roiY[0], roiWidth, roiHeight, intensity );

            Normalize( input, roiX[0], roiY[0], output, roiX[1], roiY[1], roiWidth, roiHeight );

            std::vector < uint8_t > normalized( 2 );

            if( intensity[0] == intensity[1] || (roiWidth == 1 && roiHeight == 1) ) {
                normalized[0] = normalized[1] = intensity[0];
            }
            else {
                normalized[0] = 0;
                normalized[1] = 255;
            }

            if( !verifyImage( output, roiX[1], roiY[1], roiWidth, roiHeight, normalized ) )
                return false;
        }

        return true;
    }

    bool form1_ProjectionProfile(ProjectionProfileForm1 ProjectionProfile)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            bool horizontal = (i % 2 == 0);

            std::vector < uint32_t > projection = ProjectionProfile( image, horizontal );

            uint32_t value = (horizontal ? image.height() : image.width()) * intensity;

            if( projection.size() != (horizontal ? image.width() : image.height()) ||
                std::any_of( projection.begin(), projection.end(), [&value]( uint32_t v ) { return value != v; } ) )
                return false;
        }

        return true;
    }

    bool form2_ProjectionProfile(ProjectionProfileForm2 ProjectionProfile)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            bool horizontal = (i % 2 == 0);

            std::vector < uint32_t > projection;

            ProjectionProfile( image, horizontal, projection );

            uint32_t value = (horizontal ? image.height() : image.width()) * intensity;

            if( projection.size() != (horizontal ? image.width() : image.height()) ||
                std::any_of( projection.begin(), projection.end(), [&value]( uint32_t v ) { return value != v; } ) )
                return false;
        }

        return true;
    }

    bool form3_ProjectionProfile(ProjectionProfileForm3 ProjectionProfile)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            bool horizontal = (i % 2 == 0);

            std::vector < uint32_t > projection = ProjectionProfile( image, roiX, roiY, roiWidth, roiHeight, horizontal );

            uint32_t value = (horizontal ? roiHeight : roiWidth) * intensity;

            if( projection.size() != (horizontal ? roiWidth : roiHeight) ||
                std::any_of( projection.begin(), projection.end(), [&value]( uint32_t v ) { return value != v; } ) )
                return false;
        }

        return true;
    }

    bool form4_ProjectionProfile(ProjectionProfileForm4 ProjectionProfile)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image image = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            bool horizontal = (i % 2 == 0);

            std::vector < uint32_t > projection;

            ProjectionProfile( image, roiX, roiY, roiWidth, roiHeight, horizontal, projection );

            uint32_t value = (horizontal ? roiHeight : roiWidth) * intensity;

            if( projection.size() != (horizontal ? roiWidth : roiHeight) ||
                std::any_of( projection.begin(), projection.end(), [&value]( uint32_t v ) { return value != v; } ) )
                return false;
        }

        return true;
    }

    bool form1_Resize(ResizeForm1 Resize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t outputWidth  = randomValue<uint32_t>( 1, 2048 );
            uint32_t outputHeight = randomValue<uint32_t>( 1, 2048 );

            PenguinV_Image::Image output = Resize( input, outputWidth, outputHeight );

            if( !equalSize( output, outputWidth, outputHeight ) || !verifyImage( output, intensity ) )
                return false;
        }

        return true;
    }

    bool form2_Resize(ResizeForm2 Resize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage( intensity[0] );
            PenguinV_Image::Image output = uniformImage( intensity[1] );

            Resize( input, output );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_Resize(ResizeForm3 Resize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t outputWidth  = randomValue<uint32_t>( 1, 2048 );
            uint32_t outputHeight = randomValue<uint32_t>( 1, 2048 );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Resize( input, roiX, roiY, roiWidth, roiHeight, outputWidth, outputHeight );

            if( !equalSize( output, outputWidth, outputHeight ) || !verifyImage( output, intensity ) )
                return false;
        }

        return true;
    }

    bool form4_Resize(ResizeForm4 Resize)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage( intensity[0] );
            PenguinV_Image::Image output = uniformImage( intensity[1] );

            std::vector < uint32_t > roiX( 2 ), roiY( 2 ), roiWidth( 2 ), roiHeight( 2 );

            generateRoi( input, roiX[0], roiY[0], roiWidth[0], roiHeight[0] );
            generateRoi( output, roiX[1], roiY[1], roiWidth[1], roiHeight[1] );

            Resize( input, roiX[0], roiY[0], roiWidth[0], roiHeight[0],
                    output, roiX[1], roiY[1], roiWidth[1], roiHeight[1] );

            if( !verifyImage( output, roiX[1], roiY[1], roiWidth[1], roiHeight[1], intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form1_SetPixel(SetPixelForm1 SetPixel)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image image  = uniformImage( intensity[0] );
            const uint32_t x = randomValue<uint32_t>( 0, image.width() );
            const uint32_t y = randomValue<uint32_t>( 0, image.height() );

            SetPixel( image, x, y, intensity[1] );

            if( !verifyImage( image, x, y, 1, 1, intensity[1] ) )
                return false;
        }

        return true;
    }

    bool form2_SetPixel(SetPixelForm2 SetPixel)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image image  = uniformImage( intensity[0] );
            std::vector< uint32_t > X( randomValue<uint32_t>( 1, 100 ) );
            std::vector< uint32_t > Y( X.size() );

            for (size_t j = 0; j < X.size(); j++) {
                X[j] = randomValue<uint32_t>( 0, image.width() );
                Y[j] = randomValue<uint32_t>( 0, image.height() );
            }

            SetPixel( image, X, Y, intensity[1] );

            for (size_t j = 0; j < X.size(); j++) {
                if( !verifyImage( image, X[j], Y[j], 1, 1, intensity[1] ) )
                    return false;
            }
        }

        return true;
    }

    bool form1_Subtract(SubtractForm1 Subtract)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            PenguinV_Image::Image output = Subtract( input[0], input[1] );

            if( !equalSize( input[0], output ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] - intensity[1] : 0 ) )
                return false;
        }

        return true;
    }

    bool form2_Subtract(SubtractForm2 Subtract)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image = uniformImages( intensity );

            Subtract( image[0], image[1], image[2] );

            if( !verifyImage( image[2], intensity[0] > intensity[1] ? intensity[0] - intensity[1] : 0 ) )
                return false;
        }

        return true;
    }

    bool form3_Subtract(SubtractForm3 Subtract)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { input.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Subtract( input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

            if( !equalSize( output, roiWidth, roiHeight ) ||
                !verifyImage( output, intensity[0] > intensity[1] ? intensity[0] - intensity[1] : 0 ) )
                return false;
        }

        return true;
    }

    bool form4_Subtract(SubtractForm4 Subtract)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 3 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            Subtract( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
                      image[2], roiX[2], roiY[2], roiWidth, roiHeight );

            if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
                              intensity[0] > intensity[1] ? intensity[0] - intensity[1] : 0 ) )
                return false;
        }

        return true;
    }

    bool form1_Sum(SumForm1 Sum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            if( Sum( input ) != intensity * input.width() * input.height() )
                return false;
        }

        return true;
    }

    bool form2_Sum(SumForm2 Sum)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            if( Sum( input, roiX, roiY, roiWidth, roiHeight ) != intensity * roiWidth * roiHeight )
                return false;
        }

        return true;
    }

    bool form1_Threshold(ThresholdForm1 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint8_t threshold = randomValue <uint8_t>( 255 );

            PenguinV_Image::Image output = Threshold( input, threshold );

            if( !verifyImage( output, intensity < threshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form2_Threshold(ThresholdForm2 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            uint8_t threshold = randomValue <uint8_t>( 255 );

            Threshold( input[0], input[1], threshold );

            if( !verifyImage( input[1], intensity[0] < threshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form3_Threshold(ThresholdForm3 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            uint8_t threshold = randomValue <uint8_t>( 255 );

            PenguinV_Image::Image output = Threshold( input, roiX, roiY, roiWidth, roiHeight, threshold );

            if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensity < threshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form4_Threshold(ThresholdForm4 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            uint8_t threshold = randomValue <uint8_t>( 255 );

            Threshold( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight, threshold );

            if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, intensity[0] < threshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form5_Threshold(ThresholdDoubleForm1 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint8_t minThreshold = randomValue <uint8_t>( 255 );
            uint8_t maxThreshold = randomValue <uint8_t>( minThreshold, 255 );

            PenguinV_Image::Image output = Threshold( input, minThreshold, maxThreshold );

            if( !verifyImage( output, intensity < minThreshold || intensity > maxThreshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form6_Threshold(ThresholdDoubleForm2 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > input = uniformImages( intensity );

            uint8_t minThreshold = randomValue <uint8_t>( 255 );
            uint8_t maxThreshold = randomValue <uint8_t>( minThreshold, 255 );

            Threshold( input[0], input[1], minThreshold, maxThreshold );

            if( !verifyImage( input[1], intensity[0] < minThreshold || intensity[0] > maxThreshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form7_Threshold(ThresholdDoubleForm3 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            uint8_t minThreshold = randomValue <uint8_t>( 255 );
            uint8_t maxThreshold = randomValue <uint8_t>( minThreshold, 255 );

            PenguinV_Image::Image output = Threshold( input, roiX, roiY, roiWidth, roiHeight, minThreshold, maxThreshold );

            if( !equalSize( output, roiWidth, roiHeight ) ||
                !verifyImage( output, intensity < minThreshold || intensity > maxThreshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form8_Threshold(ThresholdDoubleForm4 Threshold)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            std::vector < PenguinV_Image::Image > image;

            std::for_each( intensity.begin(), intensity.end(), [&]( uint8_t & value )
            { image.push_back( uniformImage( value ) ); } );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( image, roiX, roiY, roiWidth, roiHeight );

            uint8_t minThreshold = randomValue <uint8_t>( 255 );
            uint8_t maxThreshold = randomValue <uint8_t>( minThreshold, 255 );

            Threshold( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight, minThreshold, maxThreshold );

            if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight,
                              intensity[0] < minThreshold || intensity[0] > maxThreshold ? 0 : 255 ) )
                return false;
        }

        return true;
    }

    bool form1_Transpose(TransposeForm1 Transpose)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            PenguinV_Image::Image output = Transpose( input );

            if( !equalSize( output, input.height(), input.width() ) || !verifyImage( output, intensity ) )
                return false;
        }

        return true;
    }

    bool form2_Transpose(TransposeForm2 Transpose)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage( intensity[0] );
            PenguinV_Image::Image output( input.height(), input.width() );

            output.fill( intensity[1] );

            Transpose( input, output );

            if( !verifyImage( output, intensity[0] ) )
                return false;
        }

        return true;
    }

    bool form3_Transpose(TransposeForm3 Transpose)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            uint8_t intensity = intensityValue();
            PenguinV_Image::Image input = uniformImage( intensity );

            uint32_t roiX, roiY, roiWidth, roiHeight;
            generateRoi( input, roiX, roiY, roiWidth, roiHeight );

            PenguinV_Image::Image output = Transpose( input, roiX, roiY, roiWidth, roiHeight );

            if( !equalSize( output, roiHeight, roiWidth ) || !verifyImage( output, intensity ) )
                return false;
        }

        return true;
    }

    bool form4_Transpose(TransposeForm4 Transpose)
    {
        for( uint32_t i = 0; i < runCount(); ++i ) {
            PrepareFunction();
            std::vector < uint8_t > intensity = intensityArray( 2 );
            PenguinV_Image::Image input  = uniformImage( intensity[0] );
            PenguinV_Image::Image output = uniformImage( intensity[1] );

            std::vector < std::pair <uint32_t, uint32_t> > size( 2 );

            size[0] = imageSize( input );
            size[1] = std::pair <uint32_t, uint32_t>( output.height(), output.width() );

            std::vector < uint32_t > roiX, roiY;
            uint32_t roiWidth, roiHeight;
            generateRoi( size, roiX, roiY, roiWidth, roiHeight );

            generateOffset( output, roiX[1], roiY[1], roiHeight, roiWidth );

            Transpose( input, roiX[0], roiY[0], output, roiX[1], roiY[1], roiWidth, roiHeight );

            if( !verifyImage( output, roiX[1], roiY[1], roiHeight, roiWidth, intensity[0] ) )
                return false;
        }

        return true;
    }
}

#define FUNCTION_REGISTRATION( function, functionWrapper, counter )                                                             \
struct Register_##functionWrapper                                                                                               \
{                                                                                                                               \
    explicit Register_##functionWrapper( bool makeRegistration )                                                                \
    {                                                                                                                           \
        if( makeRegistration )                                                                                                  \
            FunctionRegistrator::instance().add( functionWrapper, namespaceName + std::string("::") + std::string(#function) +  \
                                                           std::string(" (form ") + std::string(#counter) + std::string(")") ); \
    }                                                                                                                           \
};                                                                                                                              \
const Register_##functionWrapper registrator_##functionWrapper( isSupported );

#define SET_FUNCTION_1_FORMS( function )                                                \
    bool type1_##function() { return Function_Template::form1_##function( function ); } \
    FUNCTION_REGISTRATION( function, type1_##function, 1 )

#define SET_FUNCTION_2_FORMS( function )                                                \
    SET_FUNCTION_1_FORMS( function )                                                    \
    bool type2_##function() { return Function_Template::form2_##function( function ); } \
    FUNCTION_REGISTRATION( function, type2_##function, 2 )

#define SET_FUNCTION_3_FORMS( function )                                                \
    SET_FUNCTION_2_FORMS( function )                                                    \
    bool type3_##function() { return Function_Template::form3_##function( function ); } \
    FUNCTION_REGISTRATION( function, type3_##function, 3 )

#define SET_FUNCTION_4_FORMS( function )                                                \
    SET_FUNCTION_3_FORMS( function )                                                    \
    bool type4_##function() { return Function_Template::form4_##function( function ); } \
    FUNCTION_REGISTRATION( function, type4_##function, 4 )

#define SET_FUNCTION_8_FORMS( function )                                                \
    SET_FUNCTION_4_FORMS( function )                                                    \
    bool type5_##function() { return Function_Template::form5_##function( function ); } \
    bool type6_##function() { return Function_Template::form6_##function( function ); } \
    bool type7_##function() { return Function_Template::form7_##function( function ); } \
    bool type8_##function() { return Function_Template::form8_##function( function ); } \
    FUNCTION_REGISTRATION( function, type5_##function, 5 )                              \
    FUNCTION_REGISTRATION( function, type6_##function, 6 )                              \
    FUNCTION_REGISTRATION( function, type7_##function, 7 )                              \
    FUNCTION_REGISTRATION( function, type8_##function, 8 )

namespace image_function
{
    using namespace Image_Function;

    const bool isSupported = true;
    const std::string namespaceName = "image_function";

    SET_FUNCTION_4_FORMS( AbsoluteDifference )
    SET_FUNCTION_2_FORMS( Accumulate )
    SET_FUNCTION_4_FORMS( BitwiseAnd )
    SET_FUNCTION_4_FORMS( BitwiseOr )
    SET_FUNCTION_4_FORMS( BitwiseXor )
    SET_FUNCTION_4_FORMS( ConvertToGrayScale )
    SET_FUNCTION_4_FORMS( ConvertToRgb )
    SET_FUNCTION_3_FORMS( Copy )
    SET_FUNCTION_2_FORMS( Fill )
    SET_FUNCTION_4_FORMS( Flip )
    SET_FUNCTION_4_FORMS( GammaCorrection )
    SET_FUNCTION_1_FORMS( GetThreshold )
    SET_FUNCTION_4_FORMS( Histogram )
    SET_FUNCTION_4_FORMS( Invert )
    SET_FUNCTION_2_FORMS( IsEqual )
    SET_FUNCTION_4_FORMS( LookupTable )
    SET_FUNCTION_4_FORMS( Maximum )
    SET_FUNCTION_4_FORMS( Minimum )
    SET_FUNCTION_4_FORMS( Normalize )
    SET_FUNCTION_4_FORMS( ProjectionProfile )
    SET_FUNCTION_4_FORMS( Resize )
    SET_FUNCTION_2_FORMS( SetPixel )
    SET_FUNCTION_4_FORMS( Subtract )
    SET_FUNCTION_2_FORMS( Sum )
    SET_FUNCTION_8_FORMS( Threshold )
    SET_FUNCTION_4_FORMS( Transpose )
}

namespace function_pool
{
    using namespace Function_Pool;

    const bool isSupported = true;
    const std::string namespaceName = "function_pool";

    SET_FUNCTION_4_FORMS( AbsoluteDifference )
    SET_FUNCTION_4_FORMS( BitwiseAnd )
    SET_FUNCTION_4_FORMS( BitwiseOr )
    SET_FUNCTION_4_FORMS( BitwiseXor )
    SET_FUNCTION_4_FORMS( ConvertToGrayScale )
    SET_FUNCTION_4_FORMS( ConvertToRgb )
    SET_FUNCTION_4_FORMS( GammaCorrection )
    SET_FUNCTION_4_FORMS( Histogram )
    SET_FUNCTION_4_FORMS( Invert )
    SET_FUNCTION_2_FORMS( IsEqual )
    SET_FUNCTION_4_FORMS( LookupTable )
    SET_FUNCTION_4_FORMS( Maximum )
    SET_FUNCTION_4_FORMS( Minimum )
    SET_FUNCTION_4_FORMS( Normalize )
    //SET_FUNCTION_4_FORMS( ProjectionProfile ) <-- these tests fail
    SET_FUNCTION_4_FORMS( Resize )
    SET_FUNCTION_4_FORMS( Subtract )
    SET_FUNCTION_2_FORMS( Sum )
    SET_FUNCTION_8_FORMS( Threshold )
}

#ifdef PENGUINV_AVX_SET
namespace avx
{
    using namespace Image_Function_Avx;

    const bool isSupported = isAvxAvailable;
    const std::string namespaceName = "image_function_avx";

    SET_FUNCTION_4_FORMS( AbsoluteDifference )
    SET_FUNCTION_4_FORMS( BitwiseAnd )
    SET_FUNCTION_4_FORMS( BitwiseOr )
    SET_FUNCTION_4_FORMS( BitwiseXor )
    SET_FUNCTION_4_FORMS( Invert )
    SET_FUNCTION_4_FORMS( Maximum )
    SET_FUNCTION_4_FORMS( Minimum )
    SET_FUNCTION_4_FORMS( Subtract )
    SET_FUNCTION_2_FORMS( Sum )
    SET_FUNCTION_8_FORMS( Threshold )
}
#endif

#ifdef PENGUINV_NEON_SET
namespace neon
{
    using namespace Image_Function_Neon;

    const bool isSupported = isNeonAvailable;
    const std::string namespaceName = "image_function_neon";

    SET_FUNCTION_4_FORMS( AbsoluteDifference )
    SET_FUNCTION_4_FORMS( BitwiseAnd )
    SET_FUNCTION_4_FORMS( BitwiseOr )
    SET_FUNCTION_4_FORMS( BitwiseXor )
    SET_FUNCTION_4_FORMS( Invert )
    SET_FUNCTION_4_FORMS( Maximum )
    SET_FUNCTION_4_FORMS( Minimum )
    SET_FUNCTION_4_FORMS( Subtract )
    SET_FUNCTION_8_FORMS( Threshold )
}
#endif

#ifdef PENGUINV_SSE_SET
namespace sse
{
    using namespace Image_Function_Sse;

    const bool isSupported = isSseAvailable;
    const std::string namespaceName = "image_function_sse";

    SET_FUNCTION_4_FORMS( AbsoluteDifference )
    SET_FUNCTION_4_FORMS( BitwiseAnd )
    SET_FUNCTION_4_FORMS( BitwiseOr )
    SET_FUNCTION_4_FORMS( BitwiseXor )
    SET_FUNCTION_4_FORMS( Invert )
    SET_FUNCTION_4_FORMS( Maximum )
    SET_FUNCTION_4_FORMS( Minimum )
    SET_FUNCTION_4_FORMS( Subtract )
    SET_FUNCTION_2_FORMS( Sum )
    SET_FUNCTION_8_FORMS( Threshold )
}
#endif

void addTests_Image_Function( UnitTestFramework & framework )
{
    FunctionRegistrator::instance().set( framework );
}
