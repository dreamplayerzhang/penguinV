#include "../Library/image_function_sse.h"
#include "unit_test_image_function_sse.h"
#include "unit_test_helper.h"

namespace Unit_Test
{
	void addTests_Image_Function_Sse(UnitTestFramework & framework)
	{
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseAnd2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseAnd3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseAnd8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseAnd11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseOr2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseOr3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseOr8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseOr11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseXor2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseXor3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseXor8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::BitwiseXor11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::Invert1ParameterTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Invert2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Invert5ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Invert8ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::Maximum2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Maximum3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Maximum8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Maximum11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::Minimum2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Minimum3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Minimum8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Minimum11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::Subtract2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Subtract3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Subtract8ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Subtract11ParametersTest );

		ADD_TEST( framework, Image_Function_Sse_Test::Threshold2ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Threshold3ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Threshold6ParametersTest );
		ADD_TEST( framework, Image_Function_Sse_Test::Threshold9ParametersTest );
	}

	namespace Image_Function_Sse_Test
	{
		bool BitwiseAnd2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseAnd( input[0], input[1] );

				if( !equalSize( input[0], output ) || !verifyImage( output, intensityValue[0] & intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseAnd3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::BitwiseAnd( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] & intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseAnd8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseAnd(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensityValue[0] & intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseAnd11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::BitwiseAnd( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
												image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensityValue[0] & intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseOr2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseOr( input[0], input[1] );

				if( !equalSize( input[0], output ) || !verifyImage( output, intensityValue[0] | intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseOr3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::BitwiseOr( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] | intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseOr8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseOr(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensityValue[0] | intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseOr11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::BitwiseOr( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
											   image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensityValue[0] | intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseXor2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseXor( input[0], input[1] );

				if( !equalSize( input[0], output ) || !verifyImage( output, intensityValue[0] ^ intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseXor3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::BitwiseXor( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] ^ intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseXor8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::BitwiseXor(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensityValue[0] ^ intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool BitwiseXor11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::BitwiseXor( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
												image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight, intensityValue[0] ^ intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool Invert1ParameterTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 1 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::Invert( input[0] );

				if( !verifyImage( output, ~intensityValue[0] ) )
					return false;
			}

			return true;
		}

		bool Invert2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Image_Function_Sse::Invert( input[0], input[1] );

				if( !verifyImage( input[1], ~intensityValue[0] ) )
					return false;
			}

			return true;
		}

		bool Invert5ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 1 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::Invert( input[0], roiX[0], roiY[0], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, ~intensityValue[0] ) )
					return false;
			}

			return true;
		}

		bool Invert8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::Invert( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, ~intensityValue[0] ) )
					return false;
			}

			return true;
		}

		bool Maximum2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::Maximum( input[0], input[1] );

				if( !equalSize( input[0], output ) ||
					!verifyImage( output, intensityValue[0] > intensityValue[1] ? intensityValue[0] : intensityValue[1] ))
					return false;
			}

			return true;
		}

		bool Maximum3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::Maximum( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] > intensityValue[1] ? intensityValue[0] : intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool Maximum8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::Maximum(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) ||
					!verifyImage( output, intensityValue[0] > intensityValue[1] ? intensityValue[0] : intensityValue[1] ))
					return false;
			}

			return true;
		}

		bool Maximum11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::Maximum( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
											 image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
					intensityValue[0] > intensityValue[1] ? intensityValue[0] : intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool Minimum2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::Minimum( input[0], input[1] );

				if( !equalSize( input[0], output ) ||
					!verifyImage( output, intensityValue[0] < intensityValue[1] ? intensityValue[0] : intensityValue[1] ))
					return false;
			}

			return true;
		}

		bool Minimum3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::Minimum( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] < intensityValue[1] ? intensityValue[0] : intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool Minimum8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::Minimum(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) ||
					!verifyImage( output, intensityValue[0] < intensityValue[1] ? intensityValue[0] : intensityValue[1] ))
					return false;
			}

			return true;
		}

		bool Minimum11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::Minimum( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
											 image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
					intensityValue[0] < intensityValue[1] ? intensityValue[0] : intensityValue[1] ) )
					return false;
			}

			return true;
		}

		bool Subtract2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				Bitmap_Image::Image output = Image_Function_Sse::Subtract( input[0], input[1] );

				if( !equalSize( input[0], output ) ||
					!verifyImage( output, intensityValue[0] > intensityValue[1] ? intensityValue[0] - intensityValue[1] : 0 ))
					return false;
			}

			return true;
		}

		bool Subtract3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image = uniformImages( intensityValue );

				Image_Function_Sse::Subtract( image[0], image[1], image[2] );

				if( !verifyImage( image[2], intensityValue[0] > intensityValue[1] ? intensityValue[0] - intensityValue[1] : 0 ) )
					return false;
			}

			return true;
		}

		bool Subtract8ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				Bitmap_Image::Image output = Image_Function_Sse::Subtract(
					input[0], roiX[0], roiY[0], input[1], roiX[1], roiY[1], roiWidth, roiHeight );

				if( !equalSize( output, roiWidth, roiHeight ) ||
					!verifyImage( output, intensityValue[0] > intensityValue[1] ? intensityValue[0] - intensityValue[1] : 0 ))
					return false;
			}

			return true;
		}

		bool Subtract11ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 3 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				Image_Function_Sse::Subtract( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1],
											  image[2], roiX[2], roiY[2], roiWidth, roiHeight );

				if( !verifyImage( image[2], roiX[2], roiY[2], roiWidth, roiHeight,
					intensityValue[0] > intensityValue[1] ? intensityValue[0] - intensityValue[1] : 0 ) )
					return false;
			}

			return true;
		}

		bool Threshold2ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 1 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				uint8_t threshold = randomValue <uint8_t>( 255 );

				Bitmap_Image::Image output = Image_Function_Sse::Threshold( input[0], threshold );

				if( !verifyImage( output, intensityValue[0] < threshold ? 0 : 255 ) )
					return false;
			}

			return true;
		}

		bool Threshold3ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > input = uniformImages( intensityValue );

				uint8_t threshold = randomValue <uint8_t>( 255 );

				Image_Function_Sse::Threshold( input[0], input[1], threshold );

				if( !verifyImage( input[1], intensityValue[0] < threshold ? 0 : 255 ) )
					return false;
			}

			return true;
		}

		bool Threshold6ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 1 );
				std::vector < Bitmap_Image::Image > input;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ input.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( input, roiX, roiY, roiWidth, roiHeight );

				uint8_t threshold = randomValue <uint8_t>( 255 );

				Bitmap_Image::Image output = Image_Function_Sse::Threshold( input[0], roiX[0], roiY[0], roiWidth, roiHeight, threshold );

				if( !equalSize( output, roiWidth, roiHeight ) || !verifyImage( output, intensityValue[0] < threshold ? 0 : 255 ) )
					return false;
			}

			return true;
		}

		bool Threshold9ParametersTest()
		{
			for( uint32_t i = 0; i < runCount(); ++i ) {
				std::vector < uint8_t > intensityValue = intensityArray( 2 );
				std::vector < Bitmap_Image::Image > image;

				std::for_each( intensityValue.begin(), intensityValue.end(), [&]( uint8_t & value )
					{ image.push_back( uniformImage( value ) ); } );

				std::vector < uint32_t > roiX, roiY;
				uint32_t roiWidth, roiHeight;

				generateRoi( image, roiX, roiY, roiWidth, roiHeight );

				uint8_t threshold = randomValue <uint8_t>( 255 );

				Image_Function_Sse::Threshold( image[0], roiX[0], roiY[0], image[1], roiX[1], roiY[1], roiWidth, roiHeight, threshold );

				if( !verifyImage( image[1], roiX[1], roiY[1], roiWidth, roiHeight, intensityValue[0] < threshold ? 0 : 255 ) )
					return false;
			}

			return true;
		}
	};
};