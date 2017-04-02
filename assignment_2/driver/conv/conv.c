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

/* The maximum length of the message for the device */
#define BUF_LEN 80

/* The message the device will give when asked */
static char Message[BUF_LEN];

/* How far did the process reading the message get? 
 * Useful if the message is larger than the size of the 
 * buffer we get to fill in device_read. */
static char *Message_Ptr;





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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* This function is called whenever a process which 
 * has already opened the device file attempts to 
 * read from it. */
static ssize_t device_read(
    struct file *file,
    char *buffer, /* The buffer to fill with the data */   
    size_t length,     /* The length of the buffer */
    loff_t *offset) /* offset to the file */
{
  /* Number of bytes actually written to the buffer */
  int bytes_read = 0;

#ifdef DEBUG
  printk("device_read(%p,%p,%d)\n", file, buffer, length);
#endif

  /* If we're at the end of the message, return 0 
   * (which signifies end of file) */
  if (*Message_Ptr == 0)
    return 0;

  /* Actually put the data into the buffer */
  while (length && *Message_Ptr)  {

    /* Because the buffer is in the user data segment, 
     * not the kernel data segment, assignment wouldn't 
     * work. Instead, we have to use put_user which 
     * copies data from the kernel data segment to the 
     * user data segment. */
    put_user(*(Message_Ptr++), buffer++);
    length --;
    bytes_read ++;
  }

#ifdef DEBUG
   printk ("Read %d bytes, %d left\n", bytes_read, length);
#endif

   /* Read functions are supposed to return the number 
    * of bytes actually inserted into the buffer */
  return bytes_read;
}



/* This function is called when somebody tries to 
 * write into our device file. */ 
static ssize_t device_write(struct file *file,
                            const char *buffer,
                            size_t length,
                            loff_t *offset)
{
  int i;

#ifdef DEBUG
  printk ("device_write(%p,%s,%d)",
    file, buffer, length);
#endif

  for(i=0; i<length && i<BUF_LEN; i++)
    get_user(Message[i], buffer+i);

  Message_Ptr = Message;

  /* Again, return the number of input characters used */
  return i;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/* This function is called whenever a process tries to 
 * do an ioctl on our device file. We get two extra 
 * parameters (additional to the inode and file 
 * structures, which all device functions get): the number
 * of the ioctl called and the parameter given to the 
 * ioctl function.
 *
 * If the ioctl is write or read/write (meaning output 
 * is returned to the calling process), the ioctl call 
 * returns the output of this function.
 */
int device_ioctl(
    struct inode *inode,
    struct file *file,
    unsigned int ioctl_num,/* The number of the ioctl */
    unsigned long ioctl_param) /* The parameter to it */
{
  int i;
  char *temp;
  char ch;

  /* Switch according to the ioctl called */
  switch (ioctl_num) {
    case IOCTL_SET_MSG:
      /* Receive a pointer to a message (in user space) 
       * and set that to be the device's message. */ 

      /* Get the parameter given to ioctl by the process */
      temp = (char *) ioctl_param;
      /* Find the length of the message */
      get_user(ch, temp);
      for (i=0; ch && i<BUF_LEN; i++, temp++)
        get_user(ch, temp);

      /* Don't reinvent the wheel - call device_write */
      device_write(file, (char *) ioctl_param, i, 0);
      break;

    case IOCTL_GET_MSG:
      /* Give the current message to the calling 
       * process - the parameter we got is a pointer, 
       * fill it. */
      i = device_read(file, (char *) ioctl_param, 99, 0); 
      /* Warning - we assume here the buffer length is 
       * 100. If it's less than that we might overflow 
       * the buffer, causing the process to core dump. 
       *
       * The reason we only allow up to 99 characters is 
       * that the NULL which terminates the string also 
       * needs room. */

      /* Put a zero at the end of the buffer, so it 
       * will be properly terminated */
      put_user('\0', (char *) ioctl_param+i);
      break;

    case IOCTL_GET_NTH_BYTE:
      /* This ioctl is both input (ioctl_param) and 
       * output (the return value of this function) */
      return Message[ioctl_param];
      break;
  }

  return SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

module_init(conv_init);
module_exit(conv_exit);


