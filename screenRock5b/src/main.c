/*
 ***************************************************************************
 * \brief   Embedded Linux BTE5446 FrameBuffer Exercise.
 *	        Fill the screen on the Linux frame buffer device with a color.
 *	        Not optimized and only a minimal error handling is implemented.
 * \file    AppFramefufferShapes.c
 * \version 1.0
 * \date    28.10.2013
 * \author  Martin Aebersold
 *
 * \remark  Last Modifications:
 * \remark  V1.0, AOM1, 28.10.2013
 ***************************************************************************
 *
 * Stop console blinking cursor
 * @target sudo echo 0 > /sys/class/graphics/fbcon/cursor_blink
 *
 ******************************************************************************
 */

/* Declare the function prototypes headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <math.h>

#include <linux/fb.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************/
/* Includes								      */
/******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include <linux/fb.h>

#include <sys/mman.h>
#include <sys/ioctl.h>

/******************************************************************************/
/* Macros								      */
/******************************************************************************/

#define CONVERT_RGB24(red, green, blue) \
	    ((red<<16) | (green<<8) | (blue))

/******************************************************************************/
/* Define some constants						      */
/******************************************************************************/

#define	BPP32	32

/*
 * Color definitions   	        R    G    B
 */

#define BLACK	CONVERT_RGB24(  0,   0,   0)
#define RED	CONVERT_RGB24(255,   0,   0)
#define GREEN	CONVERT_RGB24(  0, 255,   0)
#define YELLOW	CONVERT_RGB24(255, 255,   0)
#define BLUE	CONVERT_RGB24(  0,   0, 255)
#define MAGENTA	CONVERT_RGB24(255,   0, 255)
#define CYAN	CONVERT_RGB24(  0, 255, 255)
#define GREY	CONVERT_RGB24(192, 192, 192)
#define WHITE	CONVERT_RGB24(255, 255, 255)

#define float32_t	float

/******************************************************************************/
/* Define some data types						      */
/******************************************************************************/

struct RGB_COLOR
 {
  uint8_t r;
  uint8_t g;
  uint8_t b;
 };

/******************************************************************************/
/* Static Variables							      */
/******************************************************************************/

/* Screen Info */
static struct fb_var_screeninfo fbVarScreenInfo;

static uint16_t y;
static uint16_t x;

static int32_t  *pfb32;

/******************************************************************************/
/* Global Variables							      */
/******************************************************************************/

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */

int main(int argc, char *argv[])
 {
  int32_t fb_fd = 0;
  int32_t screensize = 0;

  /*
   * Open device file for reading and writing
   */
  fb_fd = open("/dev/fb0", O_RDWR);
  if (fb_fd == -1)
   {
    perror("Error: cannot open framebuffer device");
    exit(errno);
   }
  printf("The framebuffer device fb0 was opened successfully.\n");

  /*
   * Get variable screen information
   */
  if (ioctl(fb_fd, FBIOGET_VSCREENINFO, &fbVarScreenInfo) == -1)
   {
    perror("Error reading variable information");
    close(fb_fd);
    exit(errno);
   }

  /* Figure out the size of the screen in bytes */
  screensize = (fbVarScreenInfo.xres * fbVarScreenInfo.yres * fbVarScreenInfo.bits_per_pixel) / 8;

  /*
   * Map the frame buffer device memory to user space.
   * Starting address in user space is pfb32.
   */
   if (fbVarScreenInfo.bits_per_pixel == BPP32)
    {
     pfb32 = (int32_t*) mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
     if (pfb32 == (int32_t*) -1)
      {
       perror("Error: failed to map 32-BPP framebuffer device to memory");
       exit(errno);
      }

     /* Fill the screen with 32 bpp, do it for all [x,y] pixel with desired color */
     for (y=0; y<fbVarScreenInfo.yres; y++)
      {
         sleep(1);
       for (x=0; x<fbVarScreenInfo.xres; x++)
        {
         pfb32[x + y * fbVarScreenInfo.xres]  = BLUE;
        }
      }

     /* Cleanup */
     munmap(pfb32, screensize);
     close(fb_fd);
   }
  return EXIT_SUCCESS;
 }