#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>  /* ioctl */

#include "chardev.h"


#define PNGSUITE_PRIMARY
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_DEFINE
#include "stb.h"

#include <string.h>


#define DEVICE "/dev/conv"
#define DEBUG 1



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

	printf("1\n");

	device_check();
        write_image_to_device(in_image_path);

	printf("2\n");

        device_check();
	read_image_from_device(out_image_path);

	printf("3\n");



  	int file_desc, ret_val;
  	char *msg = "Message passed by ioctl\n";

        device_check();

  	ioctl_get_nth_byte(file_desc);
  	ioctl_get_msg(file_desc);
  	ioctl_set_msg(fd, msg);

  	close(fd);

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


void read_image_from_device(char *dstfile){
        printf("inside read_image_from_device\n");
	printf("------- Image Information on read_image_from_device-----------\n");
	printf("-I- Src Image width     %i\n", w);	
	printf("-I- Src Image height    %i\n", h);	
	printf("-I- Src Image depth     %i\n", depth);


        int size_buffer_image = w*h*depth;
	

        uint8* result_image = (uint8*) malloc (sizeof(uint8)*size_buffer_image);
        memset( result_image, 0, size_buffer_image );
	
	read(fd,result_image, size_buffer_image);
        stbi_write_bmp(dstfile,w,h,depth,result_image);
        close(fd);
	
}

/* Functions for the ioctl calls */

ioctl_set_msg(int file_desc, char *message)
{
  int ret_val;

  ret_val = ioctl(file_desc, IOCTL_SET_MSG, message);

  if (ret_val < 0) {
    printf ("ioctl_set_msg failed:%d\n", ret_val);
    exit(-1);
  }
}


ioctl_get_msg(int file_desc)
{
  int ret_val;
  char message[100]; 

  /* Warning - this is dangerous because we don't tell 
   * the kernel how far it's allowed to write, so it 
   * might overflow the buffer. In a real production 
   * program, we would have used two ioctls - one to tell
   * the kernel the buffer length and another to give 
   * it the buffer to fill
   */
  ret_val = ioctl(file_desc, IOCTL_GET_MSG, message);

  if (ret_val < 0) {
    printf ("ioctl_get_msg failed:%d\n", ret_val);
    exit(-1);
  }

  printf("get_msg message:%s\n", message);
}

ioctl_get_nth_byte(int file_desc)
{
  int i;
  char c;

  printf("get_nth_byte message:");

  i = 0;
  while (c != 0) {
    c = ioctl(file_desc, IOCTL_GET_NTH_BYTE, i++);

    if (c < 0) {
      printf(
      "ioctl_get_nth_byte failed at the %d'th byte:\n", i);
      exit(-1);
    }

    putchar(c);
  } 
  putchar('\n');
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


