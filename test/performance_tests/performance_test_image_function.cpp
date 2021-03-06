#include "../../src/image_function.h"
#include "performance_test_image_function.h"
#include "performance_test_helper.h"

namespace
{
    void AbsoluteDifference( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::AbsoluteDifference( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void BitwiseAnd( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::BitwiseAnd( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void BitwiseOr( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::BitwiseOr( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void BitwiseXor( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::BitwiseXor( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void ConvertToColor( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image input  = Performance_Test::uniformImage   ( size, size );
        PenguinV_Image::Image output = Performance_Test::uniformRGBImage( size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::ConvertToRgb( input, output );

            timer.stop();
        }
    }

    void ConvertToGrayscale( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image input  = Performance_Test::uniformRGBImage( size, size );
        PenguinV_Image::Image output = Performance_Test::uniformImage   ( size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::ConvertToGrayScale( input, output );

            timer.stop();
        }
    }

    void Fill( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image image = Performance_Test::uniformImage( size, size );
        uint8_t value = Performance_Test::randomValue<uint8_t>( 256 );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Fill( image, value );

            timer.stop();
        }
    }

    void GammaCorrection( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );

        double a     = Performance_Test::randomValue <uint32_t>( 100 ) / 100.0;
        double gamma = Performance_Test::randomValue <uint32_t>( 300 ) / 100.0;

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::GammaCorrection( image[0], image[1], a, gamma );

            timer.stop();
        }
    }

    void Histogram( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image image = Performance_Test::uniformImage( size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Histogram( image );

            timer.stop();
        }
    }

    void Invert( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Invert( image[0], image[1] );

            timer.stop();
        }
    }

    void LookupTable( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );

        std::vector<uint8_t> table(256, 0);

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::LookupTable( image[0], image[1], table );

            timer.stop();
        }
    }

    void Maximum( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Maximum( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void Minimum( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Minimum( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void RgbToBgr( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformRGBImages( 2, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::RgbToBgr( image[0], image[1] );

            timer.stop();
        }
    }

    void ResizeDown( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image input  = Performance_Test::uniformImage( size, size );
        PenguinV_Image::Image output = Performance_Test::uniformImage( size / 2, size / 2 );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Resize( input, output );

            timer.stop();
        }
    }

    void ResizeUp( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image input  = Performance_Test::uniformImage( size, size );
        PenguinV_Image::Image output = Performance_Test::uniformImage( size * 2, size * 2 );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Resize( input, output );

            timer.stop();
        }
    }

    void Subtract( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 3, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Subtract( image[0], image[1], image[2] );

            timer.stop();
        }
    }

    void Sum( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        PenguinV_Image::Image image = Performance_Test::uniformImage( size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Sum( image );

            timer.stop();
        }
    }

    void Threshold( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );
        uint8_t threshold = Performance_Test::randomValue<uint8_t>( 256 );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Threshold( image[0], image[1], threshold );

            timer.stop();
        }
    }

    void ThresholdDouble( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );
        uint8_t minThreshold = Performance_Test::randomValue<uint8_t>( 256 );
        uint8_t maxThreshold = Performance_Test::randomValue<uint8_t>( minThreshold, 256 );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Threshold( image[0], image[1], minThreshold, maxThreshold );

            timer.stop();
        }
    }

    void Transpose( Performance_Test::TimerContainer & timer, uint32_t size )
    {
        std::vector < PenguinV_Image::Image > image = Performance_Test::uniformImages( 2, size, size );

        for( uint32_t i = 0; i < Performance_Test::runCount(); ++i ) {
            timer.start();

            Image_Function::Transpose( image[0], image[1] );

            timer.stop();
        }
    }
}

// Function naming: _functionName_imageSize
#define SET_FUNCTION( function )                                                                            \
namespace image_function_##function                                                                         \
{                                                                                                           \
    std::pair < double, double > _256 () { return Performance_Test::runPerformanceTest( function,  256 ); } \
    std::pair < double, double > _512 () { return Performance_Test::runPerformanceTest( function,  512 ); } \
    std::pair < double, double > _1024() { return Performance_Test::runPerformanceTest( function, 1024 ); } \
    std::pair < double, double > _2048() { return Performance_Test::runPerformanceTest( function, 2048 ); } \
}

namespace
{
    SET_FUNCTION( AbsoluteDifference )
    SET_FUNCTION( BitwiseAnd         )
    SET_FUNCTION( BitwiseOr          )
    SET_FUNCTION( BitwiseXor         )
    SET_FUNCTION( ConvertToColor     )
    SET_FUNCTION( ConvertToGrayscale )
    SET_FUNCTION( Fill               )
    SET_FUNCTION( GammaCorrection    )
    SET_FUNCTION( Histogram          )
    SET_FUNCTION( Invert             )
    SET_FUNCTION( LookupTable        )
    SET_FUNCTION( Maximum            )
    SET_FUNCTION( Minimum            )
    SET_FUNCTION( RgbToBgr           )
    SET_FUNCTION( ResizeDown         )
    SET_FUNCTION( ResizeUp           )
    SET_FUNCTION( Subtract           )
    SET_FUNCTION( Sum                )
    SET_FUNCTION( Threshold          )
    SET_FUNCTION( ThresholdDouble    )
    SET_FUNCTION( Transpose          )
}

#define ADD_TEST_FUNCTION( framework, function )         \
ADD_TEST( framework, image_function_##function::_256 );  \
ADD_TEST( framework, image_function_##function::_512 );  \
ADD_TEST( framework, image_function_##function::_1024 ); \
ADD_TEST( framework, image_function_##function::_2048 );

void addTests_Image_Function( PerformanceTestFramework & framework )
{
    ADD_TEST_FUNCTION( framework, AbsoluteDifference )
    ADD_TEST_FUNCTION( framework, BitwiseAnd         )
    ADD_TEST_FUNCTION( framework, BitwiseOr          )
    ADD_TEST_FUNCTION( framework, BitwiseXor         )
    ADD_TEST_FUNCTION( framework, ConvertToColor     )
    ADD_TEST_FUNCTION( framework, ConvertToGrayscale )
    ADD_TEST_FUNCTION( framework, Fill               )
    ADD_TEST_FUNCTION( framework, GammaCorrection    )
    ADD_TEST_FUNCTION( framework, Histogram          )
    ADD_TEST_FUNCTION( framework, Invert             )
    ADD_TEST_FUNCTION( framework, LookupTable        )
    ADD_TEST_FUNCTION( framework, Maximum            )
    ADD_TEST_FUNCTION( framework, Minimum            )
    ADD_TEST_FUNCTION( framework, RgbToBgr           )
    ADD_TEST_FUNCTION( framework, ResizeDown         )
    ADD_TEST_FUNCTION( framework, ResizeUp           )
    ADD_TEST_FUNCTION( framework, Subtract           )
    ADD_TEST_FUNCTION( framework, Sum                )
    ADD_TEST_FUNCTION( framework, Threshold          )
    ADD_TEST_FUNCTION( framework, ThresholdDouble    )
    ADD_TEST_FUNCTION( framework, Transpose          )
}
