#define PNGSUITE_PRIMARY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DEFINE
#include "stb.h"

#include <math.h>
#include <string.h>
#include <unistd.h> // getopt

int main(int argc, char **argv)
{

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


	rgb2yuv(src_image, out_image);

	return 0;
}


void rgb2yuv(char *src_image, char *out_image){

	uint8 *org_image, *result;
	int *x; int *y; int req_comp;
	int w, h, depth;
	//org_image = stbi_load(src_image, &w, &h, &depth, req_comp);

        fprintf(stdout, "w %d\n", w);
        fprintf(stdout, "h %d\n", h);
        fprintf(stdout, "depth %d\n", depth);

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
	exit(0);	
}



