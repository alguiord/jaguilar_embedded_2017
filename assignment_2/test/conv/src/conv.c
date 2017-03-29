#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define PNGSUITE_PRIMARY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DEFINE
#include "stb.h"

#include <string.h>


#define DEVICE "/dev/conv"

static int fd;
static int w,h, depth, req_comp; 

int main(int argc, char **argv)
{

	int c;
	char *in_image_path = NULL;
	char *out_image_path = NULL;
	int kernel = -1;

	//This while is used to read the arguments added to through the command line to the application
	while ((c = getopt (argc, argv, "i:o:m:ha")) != -1){

		/*
		The following switch will identify the arguments read, and it will call the proper method
		and set the proper variables in order to execute the required methods
		*/

		switch (c) {
			case 'a':
				print_autor();
				break;
			case 'h':
				print_help();
				break;
			case 'i':
				in_image_path = optarg;
				break;			
			case 'o':
				out_image_path = optarg;
				break;
			case 'm':
				kernel = atoi(optarg);
				break;

			default:
			//In case the argument read is not supported
				printf("Option '-%i' not recognized.\n",c);
		}
	}


	if(in_image_path==NULL){
		fprintf(stderr, "-Error- Argument -i <input_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will use the image %s as the input image\n", in_image_path);
	}

	if(out_image_path==NULL){
		fprintf(stderr, "-Error- Argument -o <output_image> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will store the image resultant in the file %s\n", out_image_path);
	}

	if(kernel==-1){
		fprintf(stderr, "-Error- Argument -m <convolution algorith desired> is required\n-Error- Use -h to see the help menu\n");
		exit(0);
	}else{
		fprintf(stderr, "-Info- Convolution program will apply the algorith number %i to the src image\n", kernel);
	}


	device_check();

	


        write_image_to_device(in_image_path);

	//uint8 *org_image;
	//read_image_from_device(org_image,w,h, depth, req_comp);

	//run_kernel(in_image, out_image, kernel);

	return 0;

}


void device_check(){
	
	fd = open(DEVICE,O_RDWR);  //Open for reading and writing

	if(fd == -1){
		printf("file %s either does not exist or has been locked by another process\n", DEVICE);
		exit(-1);
	}

	printf("Device %s exists \n", DEVICE);
}


void write_image_to_device(char *srcfile){
	uint8 *org_image;
	org_image = stbi_load(srcfile, &w, &h, &depth, req_comp);

	printf("------- Image Information on write_image_to_device-----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);

	int size_buffer_image = w*h*depth;

	write(fd, org_image, size_buffer_image);
        close(fd);
}

void read_image_from_device(uint8 *result_image){

	printf("------- Image Information on read_image_from_device-----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);


        int size_buffer_image = w*h*depth;

	read(fd,result_image, size_buffer_image);
}


//This method will print the autors information
void print_autor(){
	printf("#############################################\n");
	printf("#    Convolution driver test                #\n");
	printf("#############################################\n");
	printf("# Autores: Brayan Alfaro	            #\n");
	printf("#          Antonio Aguilar                  #\n");
	printf("# Maestria Sistemas Embebidos               #\n");
	printf("# Instituo Tecnologico de Costa Rica        #\n");
	printf("#############################################\n");
	exit(0);
}


//This method will print the help menu

void print_help(){
	printf("####################################################################################\n");
	printf("#                       Convolution driver test                                    #\n");
	printf("####################################################################################\n");
	printf("# Valid arguments:                                                                 #\n");
	printf("#   -a: Display the program autor information                                      #\n");
	printf("#   -h: Display this help menu                                                     #\n");
	printf("#   -i: path to input image                                                        #\n");
	printf("#   -o: path to output image                                                       #\n");
	printf("#   -m: convolution algorithm to use:                                              #\n");
	printf("#            1: kernel_left_sobel                                                  #\n");
	printf("#            2: kernel_identity                                                    #\n");
	printf("#            3: kernel_outline                                                     #\n");
	printf("#                                                                                  #\n");
	printf("# Command line examples                                                            #\n");
	printf("#  ./conv -i image_input   -o image_output -m 5                                    #\n");
	printf("####################################################################################\n");
	exit(0);
	
}


