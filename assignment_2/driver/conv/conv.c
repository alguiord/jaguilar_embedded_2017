#include <linux/init.h>
#include <linux/kernel.h>   /* We're doing kernel work */
#include <linux/module.h>

#include <linux/fs.h> /* For the character driver support*/
#include <linux/cdev.h> /*This is a char driver; makes cdev available*/
#include <linux/semaphore.h> /*used to access semaphores; synchonization behaviors*/
#include <asm/uaccess.h>  /*copy_to_user;copy_from_user*/


#include "chardev.h"

#define SUCCESS 0

static int Major_Number;

MODULE_LICENSE("GPL");

static int kernel;
static int width,height,depth;

struct device{
	unsigned char data[400000];
	struct semaphore sem;
} virtual_device;


int ret;  //Will be used to hold return values


int conv_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	/*
	if(down_interruptible(&virtual_device.sem) != 0){
		printk(KERN_ALERT "%s: Could not lock device during open\n", DEVICE_NAME );
		return -1;
	
	}
	*/
	return 0;
}


int conv_read(struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	ret = copy_to_user(buffer, virtual_device.data,length);
	
	return ret;
}

int conv_write(struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	ret = copy_from_user(virtual_device.data, buffer, length);
	return ret;
}

int conv_close(struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void kernel_left_sobel (unsigned char *result_image, int h, int row, int w, int col, int depth, int z, unsigned char image[h][w][depth]){

	result_image[(row * w + col) * depth + z] = (
						image[row-1][col-1][z]*1 + image[row-1][col][z]*0 + image[row-1][col+1][z]*-1 + 
						image[row]  [col-1][z]*2 + image[row]  [col][z]*0 + image[row]  [col+1][z]*-2 + 
						image[row+1][col-1][z]*1 + image[row+1][col][z]*0 + image[row+1][col+1][z]*-1);
}




void apply_kernel(void){

    unsigned char image[height][width][depth];

	
    int row, col, d;

    for (row = 0; row < height; row++) {
	for (col = 0; col < width; col++) {
	    for (d = 0; d < depth; d++) {
		image[row][col][d] = virtual_device.data[(row * width + col) * depth + d];
	    }
	}
     }



    for (row = 0; row < height; row++) {
	for (col = 0; col < width; col++) {
            for (d = 0; d < depth; d++) {

	        switch (kernel) {
			case 1:
				kernel_left_sobel(virtual_device.data, height, row, width, col, depth, d, image);	
			break;
				
			case 2:
				//kernel_identity(result, h, row, w, col, depth, z, c_image);							
			break;			
			
			case 3:
				//kernel_outline(result, h, row, w, col, depth, z, c_image);		
			break;
	
			case 4:
				//kernel_blur(result, h, row, w, col, depth, z, c_image);		
			break;

			case 5:
				//kernel_sharpen(result, h, row, w, col, depth, z, c_image);		
			break;

			case 6:
				//kernel_topsobel(result, h, row, w, col, depth, z, c_image);		
			break;	
			default:
				printk(KERN_ALERT "-Error- Kernel %i is not found, exiting from the program ....\n", kernel);
		}
            }
	}
     }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/* This function is called whenever a process tries to 
 * do an ioctl on our device file. 
*/
long conv_ioctl(
    //struct inode *inode,
    struct file *file,
    unsigned int ioctl_num,/* The number of the ioctl */
    unsigned long ioctl_param) /* The parameter to it */
{

  printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
 
  printk(KERN_ALERT "ioctl_num: %d \n", ioctl_num);

  /* Switch according to the ioctl called */
  switch (ioctl_num) {
   

    case IOCTL_SET_WIDTH:
      printk(KERN_ALERT "Inside IOCTL_SET_WIDTH case\n");

      /* Get the parameter given to ioctl by the process */
      width = (int) ioctl_param;
      printk(KERN_ALERT "ioctl set width: %d \n",width);
 
      break;

    case IOCTL_SET_HEIGHT:
      printk(KERN_ALERT "Inside IOCTL_SET_WIDTH case\n");

      /* Get the parameter given to ioctl by the process */
      height = (int) ioctl_param;
      printk(KERN_ALERT "ioctl set height: %d \n",height);
 
      break;

    case IOCTL_SET_DEPTH:
      printk(KERN_ALERT "Inside IOCTL_SET_DEPTH case\n");

      /* Get the parameter given to ioctl by the process */
      depth = (int) ioctl_param;
      printk(KERN_ALERT "ioctl set depth: %d \n",depth);
 
      break;



    case IOCTL_SET_MODE:
      printk(KERN_ALERT "Inside IOCTL_SET_MODE case\n");

      /* Get the parameter given to ioctl by the process */
      kernel = (int) ioctl_param;
      printk(KERN_ALERT "ioctl set mode: %d \n",kernel);
 
      break;

    case IOCTL_START_CONV:
      printk(KERN_ALERT "Inside IOCTL_START_CONV case\n");
      apply_kernel();
      break;

    default:
       printk(KERN_ALERT "Inside IOCTL undefined case\n");
  }



  return SUCCESS;
}







////////////////////////////////////////////////////////////////////////////////

/*To hold the file operations performed on this device*/

struct file_operations conv_file_operations = {
  .owner   = THIS_MODULE,
  .open    = conv_open,
  .read    = conv_read,
  .write   = conv_write,
  .release = conv_close,
  .unlocked_ioctl = conv_ioctl,
};

int conv_init(void)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

	/*Register with the kernel and indicate that we are registering a character device driver*/
	Major_Number = register_chrdev(0,
			DEVICE_NAME,
			&conv_file_operations
			);

   	if (Major_Number < 0) {
     		printk ("Registering the character device failed with %d\n", Major_Number);
     		return Major_Number;
   	}


   printk("<1>I was assigned major number %d.  To talk to\n", Major_Number);
   printk("<1>the driver, create a dev file with\n");
   printk("'mknod /dev/%s c %d 0'.\n",DEVICE_NAME, Major_Number);
   printk("<1>Try various minor numbers.  Try to cat and echo to\n");
   printk("the device file.\n");
   printk("<1>Remove the device file and module when done.\n");


	return 0;
}

void conv_exit(void)
{
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

	/*Unregister the character device driver*/
	unregister_chrdev(Major_Number,DEVICE_NAME);
        //if (ret < 0) printk("Error in unregister_chrdev: %d\n", ret);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

module_init(conv_init);
module_exit(conv_exit);


