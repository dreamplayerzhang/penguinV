#pragma once

#include <stdint.h>
#include "../image_buffer.h"
#include "image_buffer_opencl.h"

namespace Image_Function_OpenCL
{
    using namespace Bitmap_Image_OpenCL;

    Image AbsoluteDifference( const Image & in1, const Image & in2 );
    void  AbsoluteDifference( const Image & in1, const Image & in2, Image & out );

    Image BitwiseAnd( const Image & in1, const Image & in2 );
    void  BitwiseAnd( const Image & in1, const Image & in2, Image & out );

    Image BitwiseOr( const Image & in1, const Image & in2 );
    void  BitwiseOr( const Image & in1, const Image & in2, Image & out );

    Image BitwiseXor( const Image & in1, const Image & in2 );
    void  BitwiseXor( const Image & in1, const Image & in2, Image & out );

    // To archive maximum performance it is recommended that Bitmap_Image::Image has 1 pixel alignment
    // or (width * color count) will be equal to row size
    Image ConvertToOpenCL( const PenguinV_Image::Image & in );
    void  ConvertToOpenCL( const PenguinV_Image::Image & in, Image & out );
    PenguinV_Image::Image ConvertFromOpenCL( const Image & in );
    void                ConvertFromOpenCL( const Image & in, PenguinV_Image::Image & out );

    Image ConvertToGrayScale( const Image & in );
    void  ConvertToGrayScale( const Image & in, Image & out );

    Image ConvertToRgb( const Image & in );
    void  ConvertToRgb( const Image & in, Image & out );

    void  Copy( const Image & in, Image & out );

    Image ExtractChannel( const Image & in, uint8_t channelId );
    void  ExtractChannel( const Image & in, Image & out, uint8_t channelId );

    void  Fill( Image & image, uint8_t value );

    // Make sure that input parameters such as input and output images are not same image!
    // horizontal flip: left-right --> right-left
    // vertical flip: top-bottom --> bottom-top
    Image Flip( const Image & in, bool horizontal, bool vertical );
    void  Flip( const Image & in, Image & out, bool horizontal, bool vertical );

    // Gamma correction works by formula:
    // output = A * ((input / 255) ^ gamma) * 255, where A - multiplication, gamma - power base. Both values must be greater than 0
    // Usually people set A as 1
    Image GammaCorrection( const Image & in, double a, double gamma );
    void  GammaCorrection( const Image & in, Image & out, double a, double gamma );

    uint8_t GetThreshold( const std::vector < uint32_t > & histogram );

    std::vector < uint32_t > Histogram( const Image & image );
    void                     Histogram( const Image & image, std::vector < uint32_t > & histogram );

    // Invert function is Bitwise NOT operation. But to make function name more user-friendly we named it like this
    Image Invert( const Image & in );
    void  Invert( const Image & in, Image & out );

    Image LookupTable( const Image & in, const std::vector < uint8_t > & table );
    void  LookupTable( const Image & in, Image & out, const std::vector < uint8_t > & table );

    Image Maximum( const Image & in1, const Image & in2 );
    void  Maximum( const Image & in1, const Image & in2, Image & out );

    Image Minimum( const Image & in1, const Image & in2 );
    void  Minimum( const Image & in1, const Image & in2, Image & out );

    Image Subtract( const Image & in1, const Image & in2 );
    void  Subtract( const Image & in1, const Image & in2, Image & out );

    // Thresholding works in such way:
        // if pixel intensity on input image is          less (  < ) than threshold then set pixel intensity on output image as 0
        // if pixel intensity on input image is equal or more ( >= ) than threshold then set pixel intensity on output image as 255
    Image Threshold( const Image & in, uint8_t threshold );
    void  Threshold( const Image & in, Image & out, uint8_t threshold );

    // Thresholding works in such way:
        // if pixel intensity on input image is less ( < ) than minimum threshold or more ( > ) than maximum threshold
        // then      set pixel intensity on output image as 0
        // otherwise set pixel intensity on output image as 255
    Image Threshold( const Image & in, uint8_t minThreshold, uint8_t maxThreshold );
    void  Threshold( const Image & in, Image & out, uint8_t minThreshold, uint8_t maxThreshold );
}
