/**
 * @file information.h
 * @authors Jose Antonio Aguilar Bravo/
 *	    Brayan/
 * @brief Assignment 1: Bump it up! 
 * @section USAGE
 * This program performs an image conversion from RGB format to YUV format. 
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
 * @subsection Conversion algorithm 
 *For getting the traditional 'studio swing' 8 bit representation of Y′UV for SDTV/BT.601 the following operations can be used:
 *
 *1. Basic transform from 8 bit RGB to 16 bit values (Y′: unsigned, U/V: signed, matrix values got rounded so that the later on desired Y′ range of [16..235] and U/V range of [16..240] is reached):
 *
 *   [ Y ′ U V ] = [ 66 129 25 − 38 − 74 112 112 − 94 − 18 ] [ R G B ] {\displaystyle {\begin{bmatrix}Y'\\U\\V\end{bmatrix}}={\begin{bmatrix}66&129&25\\-38&-74&112\\112&-94&-18\end{bmatrix}}{\begin{bmatrix}R\\G\\B\end{bmatrix}}} {\begin{bmatrix}Y'\\U\\V\end{bmatrix}}={\begin{bmatrix}66&129&25\\-38&-74&112\\112&-94&-18\end{bmatrix}}{\begin{bmatrix}R\\G\\B\end{bmatrix}}
 *
 *2. Scale down (">>8") to 8 bit with rounding ("+128") (Y′: unsigned, U/V: signed):
 *
 *   Y t ′ = ( Y ′ + 128 ) ≫ 8 U t = ( U + 128 ) ≫ 8 V t = ( V + 128 ) ≫ 8 {\displaystyle {\begin{array}{rcl}Yt'&=&(Y'+128)\gg 8\\Ut&=&(U+128)\gg 8\\Vt&=&(V+128)\gg 8\end{array}}} {\begin{array}{rcl}Yt'&=&(Y'+128)\gg 8\\Ut&=&(U+128)\gg 8\\Vt&=&(V+128)\gg 8\end{array}}
 *
 *3. Add an offset to the values to eliminate any negative values (all results are 8 bit unsigned):
 *
 *   Y u ′ = Y t ′ + 16 U u = U t + 128 V u = V t + 128 {\displaystyle {\begin{array}{rcl}Yu'&=&Yt'+16\\Uu&=&Ut+128\\Vu&=&Vt+128\end{array}}} {\begin{array}{rcl}Yu'&=&Yt'+16\\Uu&=&Ut+128\\Vu&=&Vt+128\end{array}} 
 *
 *
 *More info: https://en.wikipedia.org/wiki/YUV
 *
 *
 * @subsection Input Image format
 * RGB888, no alpha. 
 *
 * Command line to generate the sample image:
 * 	gst-launch-0.10 videotestsrc num-buffers=1 ! video/x-raw-rgb,  bpp=24, depth=24, width=640, height=480 ! filesink location=image.rgb
 *
 * @subsection Output Image format
 * UYVY, where Y is sample every pixel and U/V are sample every two pixels, so we get a conversion from 1:2/3
 *
 *
 *
 * @subsection Example 
 * Example input image:
 * @image html image.rgb
 *
 * Example output image:
 * @image html image.yuv
 *
 * @subsection Processing time:
 * The elapsed time of the function 'rgb2yuv' was: 11.052000 ms
 *
 */

