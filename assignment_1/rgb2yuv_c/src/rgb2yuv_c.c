#include <math.h>
#include <string.h>
#include <unistd.h> // getopt
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>

#define DEBUG 0



#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)

// RGB -> YUV
#define RGB2Y(R, G, B) CLIP(( (  66 * (R) + 129 * (G) +  25 * (B) + 128) >> 8) +  16)
#define RGB2U(R, G, B) CLIP(( ( -38 * (R) -  74 * (G) + 112 * (B) + 128) >> 8) + 128)
#define RGB2V(R, G, B) CLIP(( ( 112 * (R) -  94 * (G) -  18 * (B) + 128) >> 8) + 128)




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

    	FILE* pInput  = NULL;
    	FILE* pOutput = NULL;

        char* buf = (char*) malloc (sizeof(char)*depthInBytes);
    	memset( buf, 0, depthInBytes );

        char* bufYUV = (char*) malloc (sizeof(char)* depthYUVInBytes);
    	memset( bufYUV, 0, depthYUVInBytes );

    	pInput  = fopen( src_image,"rb" );
    	pOutput = fopen( out_image,"wb" );


    	if( pInput && pOutput )
    	{
        	for( int i = 0; i < height; i++ )
        	{
		



			unsigned char Y0 = 0;
			unsigned char Y1 = 0;
			unsigned char U1 = 0;
			unsigned char V1 = 0;


		  	for (int j=0;j< width;j++){

            			//read pixel, RGB
            			fread( buf, 1, depthInBytes, pInput );


				unsigned char R = buf[0];
				unsigned char G = buf[1];
				unsigned char B = buf[2];

				if(DEBUG){ 
				fprintf(stdout, "R: %d \n", R);
				fprintf(stdout, "G: %d \n", G);
				fprintf(stdout, "B: %d \n", B);
				}
	

				if (j%2==1){
					Y1 = RGB2Y(R, G, B);
					U1 = RGB2U(R, G, B);
					V1 = RGB2V(R, G, B);

					bufYUV[0] = U1;
					bufYUV[1] = Y0;
					bufYUV[2] = V1;
					bufYUV[3] = Y1;
	
            				//write pixel, YUYV
            				fwrite( bufYUV, 1, depthYUVInBytes, pOutput );
				}else{
					Y0 = RGB2Y(R, G, B);
				}


				if(DEBUG){
				fprintf(stdout, "U1: %d \n", U1);
				fprintf(stdout, "Y0: %d \n", Y0);
				fprintf(stdout, "V1: %d \n", V1);
				fprintf(stdout, "Y1: %d \n", Y1);
				}

			}
			
        	}
    	}


    	fclose( pInput );
    	fclose( pOutput );

	free(buf);
	free(bufYUV);

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
	printf("#  ./rgb2yuv_c -i image.rgb -o image.yuv                          #\n");
	printf("###################################################################\n");


}
