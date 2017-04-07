/*  chardev.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef CHARDEV_H
#define CHARDEV_H

#include <linux/ioctl.h>



/* The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. */
#define MAJOR_NUM 246


/* Set the mode for the convolution */
#define IOCTL_SET_MODE _IOR(MAJOR_NUM, 0, int)
/* _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */

/* Start the convolution process in the device driver */
#define IOCTL_START_CONV _IOR(MAJOR_NUM, 1, char*)


/* Set the width of the image*/
#define IOCTL_SET_WIDTH _IOR(MAJOR_NUM, 2, int)

/* Set the height of the image*/
#define IOCTL_SET_HEIGHT _IOR(MAJOR_NUM, 3, int)

/* Set the depth of the image*/
#define IOCTL_SET_DEPTH _IOR(MAJOR_NUM, 4, int)






/* The name of the device file */
#define DEVICE_NAME "char_conv"


#endif
