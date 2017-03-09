/**
 * @file information.h
 * @authors Jose Antonio Aguilar Bravo/
 *	    Brayan/
 * @brief Assignment 1: Bump it up! 
 *
 * @section USAGE
 * This program performs an image conversion from RGB format to YUV format. 
 *
 *
 *
 * 
 * Usage: 
 *
 *		rgb2yuv_c -i image.rgb -o image.yuv  
 *
 *		-i: RGBfile specifies the RGB file to be converted. 
 *		-o: YUVfile specifies the output file name.
 *		-a: displays the information of the author of the program.
 *		-h: displays this help menu.   
 *
 *
 *
 *
 * @section algorithm Conversion algorithm:
 * 
 * For getting the traditional 'studio swing' 8 bit representation of Y′UV for SDTV/BT.601 the following operations can be used:
 *
 *  Basic transform from 8 bit RGB to 16 bit values (Y′: unsigned, U/V: signed, matrix values got rounded so that the later on desired Y′ range of [16..235] and U/V range of [16..240] is reached):
 *
 * @image html rgb2yuv1.svg
 *
 *  Scale down (">>8") to 8 bit with rounding ("+128") (Y′: unsigned, U/V: signed):
 *
 * @image html rgb2yuv2.svg
 *
 *  Add an offset to the values to eliminate any negative values (all results are 8 bit unsigned):
 *
 * @image html rgb2yuv3.svg
 *
 *
 * More info: https://en.wikipedia.org/wiki/YUV
 *
 *
 * @subsection input Input Image format:
 *
 * RGB888, no alpha. 
 *
 * Command line to generate the sample image:
 * 	gst-launch-0.10 videotestsrc num-buffers=1 ! video/x-raw-rgb,  bpp=24, depth=24, width=640, height=480 ! filesink location=image.rgb
 *
 * @subsection output Output Image format:
 *
 * UYVY, where Y is sample every pixel and U/V are sample every two pixels, so we get a reduction from 1 to 2/3
 *
 *
 *
 * @subsection Example Image Samples 
 * Input image:
 * @image html image.rgb
 *
 * Output image:
 * @image html image.yuv
 *
 * @subsection time Processing time:
 * The elapsed time of the function 'rgb2yuv' was: 11.052000 ms
 *
 */

