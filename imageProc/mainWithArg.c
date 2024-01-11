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
 * \remark  V1.1, 11/01/24 (DD/MM/YY) Colin Audergon (audec2@bfh.ch)
 *  \remark add argument input to display array of
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

#include <linux/fb.h>

#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

/******************************************************************************/
/* Macros								      */
/******************************************************************************/

#define CONVERT_RGB24(red, green, blue) \
  ((red << 16) | (green << 8) | (blue))

/******************************************************************************/
/* Define some constants						      */
/******************************************************************************/

#define BPP32 32

/*
 * Color definitions   	        R    G    B
 */

#define BLACK CONVERT_RGB24(0, 0, 0)
#define RED CONVERT_RGB24(255, 0, 0)
#define GREEN CONVERT_RGB24(0, 255, 0)
#define YELLOW CONVERT_RGB24(255, 255, 0)
#define BLUE CONVERT_RGB24(0, 0, 255)
#define MAGENTA CONVERT_RGB24(255, 0, 255)
#define CYAN CONVERT_RGB24(0, 255, 255)
#define GREY CONVERT_RGB24(192, 192, 192)
#define WHITE CONVERT_RGB24(255, 255, 255)

#define float32_t float

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

static struct RGB_COLOR *pfb_rgb; // Updated to use RGB_COLOR structure
static int32_t *pfb32;

/******************************************************************************/
/* Global Variables							      */
/******************************************************************************/

/******************************************************************************/
/* Function prototypes						      */
/******************************************************************************/
int validateImageArray(uint8_t ***imageArray, int height, int width);

/*
 ******************************************************************************
 * main
 ******************************************************************************
 */

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <height> <width> <r1> <g1> <b1> ... <rn> <gn> <bn>\n", argv[0]);
    return 1;
  }
  int height = atoi(argv[1]);
  int width = atoi(argv[2]);
  int numArgs = argc - 3;
  int expectedNumArgs = height * width * 3;

  if (numArgs != expectedNumArgs)
  {
    fprintf(stderr, "Error: Unexpected number of arguments. Expected %d, got %d\n", expectedNumArgs, numArgs);
    return 1;
  }

  // Access RGB values
  struct RGB_COLOR *rgbValues = (struct RGB_COLOR *)(argv + 3);
  // get height, width and rgb value
  // resolve resolution -> set correct resolution directly in python
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

  pfb_rgb = malloc(fbVarScreenInfo.yres_virtual * fbVarScreenInfo.xres_virtual * sizeof(struct RGB_COLOR));
  if (pfb_rgb == NULL)
  {
    perror("Error: failed to allocate memory for pfb_rgb");
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
    pfb32 = (int32_t *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if (pfb32 == (int32_t *)-1)
    {
      perror("Error: failed to map 32-BPP framebuffer device to memory");
      exit(errno);
    }
    for (y = 0; y < height; y++)
    {
      for (x = 0; x < width; x++)
      {
        // Access RGB values from the command-line arguments
        uint8_t r = rgbValues[y * width + x].r;
        uint8_t g = rgbValues[y * width + x].g;
        uint8_t b = rgbValues[y * width + x].b;

        // Calculate the pixel position in the framebuffer
        uint32_t pixel_pos = x + y * fbVarScreenInfo.xres_virtual;

        // Assuming 24-bit framebuffer, pack RGB values into a single 32-bit pixel
        uint32_t pixel = (r << 16) | (g << 8) | b;

        // Copy pixel to the framebuffer
        pfb_rgb[pixel_pos].r = r;
        pfb_rgb[pixel_pos].g = g;
        pfb_rgb[pixel_pos].b = b;
      }
    }
    /* Cleanup */
    free(pfb_rgb);
    munmap(pfb32, screensize);
    close(fb_fd);

  }
  return EXIT_SUCCESS;
}

// Function to check if imageArray is a valid 3D array with expected shape
int validateImageArray(uint8_t ***imageArray, int height, int width)
{
  if (imageArray == NULL)
  {
    fprintf(stderr, "Error: imageArray is NULL\n");
    return 0;
  }

  for (int i = 0; i < height; i++)
  {
    if (imageArray[i] == NULL)
    {
      fprintf(stderr, "Error: Row %d of imageArray is NULL\n", i);
      return 0;
    }

    for (int j = 0; j < width; j++)
    {
      if (imageArray[i][j] == NULL)
      {
        fprintf(stderr, "Error: Column %d of Row %d of imageArray is NULL\n", j, i);
        return 0;
      }
    }
  }

  return 1; // imageArray is valid
}
