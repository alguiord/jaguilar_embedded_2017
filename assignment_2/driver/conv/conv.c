#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h> /* For the character driver support*/
#include <linux/cdev.h> /*This is a char driver; makes cdev available*/
#include <linux/semaphore.h> /*used to access semaphores; synchonization behaviors*/
#include <asm/uaccess.h>  /*copy_to_user;copy_from_user*/

#define DEVICE_NAME "char_conv"
#define BUF_LEN 80            /* Max length of the message from the device */

static int Major_Number;

MODULE_LICENSE("GPL");

//uint8 *org_image;

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


/*To hold the file operations performed on this device*/

struct file_operations conv_file_operations = {
  .owner   = THIS_MODULE,
  .open    = conv_open,
  .read    = conv_read,
  .write   = conv_write,
  .release = conv_close,
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

module_init(conv_init);
module_exit(conv_exit);


