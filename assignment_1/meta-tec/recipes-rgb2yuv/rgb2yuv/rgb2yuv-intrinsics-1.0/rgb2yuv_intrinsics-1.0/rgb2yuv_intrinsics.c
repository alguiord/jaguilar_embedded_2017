#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>

#include <arm_neon.h>
#include <errno.h>

#define DEBUG 0

/*
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)

*/


int main(int argc, char **argv)
{

    	struct timeval t1, t2;
    	double elapsedTime;


	int c;
	char *src_image = NULL;
	char *out_image = NULL;


	while ((c = getopt (argc, argv, "i:o:ah")) != -1){

		switch (c) {
			case 'a':
			//-a displays the information of the author of the program.
				print_autor();
				break;
			case 'i':
			//-i RGBfile specifies the RGB file to be converted.
				src_image = optarg;
				break;		
			case 'o':
			//-o YUVfile specifies the output file name.
				out_image = optarg;
				break;
			case 'h':
			//-h displays the usage message to let the user know how to execute the application.
				print_help();
				break;
			default:
			//In case the argument read is not supported
				printf("Option '-%i' not recognized.\n",c);
			}


	}



	if(src_image==NULL){
		fprintf(stderr, "-Error- Argument -i <source_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stdout, "-Info- RGB to YUV tool will use the image %s as the source image\n", src_image);
	}

	if(out_image==NULL){
		fprintf(stderr, "-Error- Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stdout, "-Info- RGB to YUV tool will store the image resultant in the file %s\n", out_image);
	}



  	// start timer
    	gettimeofday(&t1, NULL);

	rgb2yuv(src_image, out_image);

    	// stop timer
    	gettimeofday(&t2, NULL);

        if(DEBUG){
        fprintf(stdout, "-Info- t1.tv_sec: %d\n", t1.tv_sec);
        fprintf(stdout, "-Info- t1.tv_usec: %d\n", t1.tv_usec);
        fprintf(stdout, "-Info- t2.tv_sec: %d\n", t2.tv_sec);
        fprintf(stdout, "-Info- t2.tv_usec: %d\n", t2.tv_usec);
	}


    	// compute and print the elapsed time in millisec
    	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms

	if(DEBUG){
        	fprintf(stdout, "-Info- The elapsed time of the funtion 'rgb2yuv' was: %f\n", elapsedTime);
	}

    	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms

        fprintf(stdout, "-Info- The elapsed time of the function 'rgb2yuv' was: %f ms\n", elapsedTime);


	return 0;
}


void rgb2yuv(char *src_image, char *out_image){

	//size of individual image
	int width  = 640;
	int height = 480;
	int depthInBytes = 3;
	int depthYUVInBytes = 4;
	int sizeImage = width * height;

    	FILE* pInput  = NULL;
    	FILE* pOutput = NULL;

    	pInput  = fopen( src_image,"rb" );
    	pOutput = fopen( out_image,"wb" );


        uint8_t* bufRGB = (uint8_t*) malloc (sizeof(uint8_t)*sizeImage*depthInBytes);
    	memset( bufRGB, 0, sizeImage*depthInBytes );

        fread( bufRGB, 1, sizeImage*depthInBytes, pInput );



  int i;
  uint8x8_t rfac_y = vdup_n_u8 (66);
  uint8x8_t gfac_y = vdup_n_u8 (129);
  uint8x8_t bfac_y = vdup_n_u8 (25);


  uint8x8_t rfac_u = vdup_n_u8 (38);
  uint8x8_t gfac_u = vdup_n_u8 (74);
  uint8x8_t bfac_u = vdup_n_u8 (112);

  uint8x8_t rfac_v = vdup_n_u8 (112);
  uint8x8_t gfac_v = vdup_n_u8 (94);
  uint8x8_t bfac_v = vdup_n_u8 (18);


  uint16x8_t v128pq = vdupq_n_u16(128);

  uint8x8_t v16p   = vdup_n_u8(16);
  uint8x8_t v128p = vdup_n_u8(128);


  sizeImage/=8;

  uint16x8_t  temp;
  uint8x8_t result;


  uint8_t* bufY = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufY, 0, 8 );

  uint8_t* bufU = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufU, 0, 8 );

  uint8_t* bufV = (uint8_t*) malloc (sizeof(uint8_t)* 8);
  memset( bufV, 0, 8 );

  uint8_t* bufYUV = (uint8_t*) malloc (sizeof(uint8_t)* 16);
  memset( bufYUV, 0, 16 );


  for (i=0; i<sizeImage; i++)
  {
    uint8x8x3_t rgb  = vld3_u8 (bufRGB);

    //Calculating Y
    temp = vmull_u8 (rgb.val[0],      rfac_y);
    temp = vmlal_u8 (temp,rgb.val[1], gfac_y);
    temp = vmlal_u8 (temp,rgb.val[2], bfac_y);

    temp = vaddq_u16(temp, v128pq);

    result = vshrn_n_u16 (temp, 8);

    result = vadd_u8(result, v16p);
    vst1_u8 (bufY, result);

    //destY += 8;


    //Calculating U

    temp = vmull_u8 (rgb.val[2],      bfac_u);
    temp = vmlsl_u8 (temp,rgb.val[1], gfac_u);
    temp = vmlsl_u8 (temp,rgb.val[0], rfac_u);

    temp = vaddq_u16(temp, v128pq);

    result = vshrn_n_u16 (temp, 8);

    result = vadd_u8(result, v128p);
    vst1_u8 (bufU, result);

    //destU += 8;

    //Calculating V


    temp = vmull_u8 (rgb.val[0],      rfac_v);
    temp = vmlsl_u8 (temp,rgb.val[1], gfac_v);
    temp = vmlsl_u8 (temp,rgb.val[2], bfac_v);

    temp = vaddq_u16(temp, v128pq);

    result = vshrn_n_u16 (temp, 8);

    result = vadd_u8(result, v128p);
    vst1_u8 (bufV, result);

    //destV += 8;

    bufYUV[0] = bufU[1];
    bufYUV[1] = bufY[0];
    bufYUV[2] = bufV[1];
    bufYUV[3] = bufY[1];

    bufYUV[4] = bufU[3];
    bufYUV[5] = bufY[2];
    bufYUV[6] = bufV[3];
    bufYUV[7] = bufY[3];

    bufYUV[8] = bufU[5];
    bufYUV[9] = bufY[4];
    bufYUV[10] = bufV[5];
    bufYUV[11] = bufY[5];

    bufYUV[12] = bufU[7];
    bufYUV[13] = bufY[6];
    bufYUV[14] = bufV[7];
    bufYUV[15] = bufY[7];



    //write pixel, YUYV
    fwrite( bufYUV, 1, 16, pOutput );



    bufRGB  += 8*3;
  }


    	fclose( pInput );
    	fclose( pOutput );


}





void print_autor(){
	printf("#############################################\n");
	printf("#           RGB to YUV tool                #\n");
	printf("#############################################\n");
	printf("# Autores: Antonio Aguilar Bravo            #\n");
	printf("#          Brayan              #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


void print_help(){
	printf("###################################################################\n");
	printf("#     RGB to YUV tool                                             #\n");
	printf("###################################################################\n");
	printf("# Valid arguments:                                                #\n");
	printf("#   -a: displays the information of the author of the program.    #\n");
	printf("#   -h: displays this help menu                                   #\n");
	printf("#   -i: RGBfile specifies the RGB file to be converted.           #\n");
	printf("#   -o: YUVfile specifies the output file name.                   #\n");
	printf("#                                                                 #\n");
	printf("# Command line examples                                           #\n");
	printf("#  ./rgb2yuv_intrinsics -i image.rgb -o image.yuv                 #\n");
	printf("###################################################################\n");


}
