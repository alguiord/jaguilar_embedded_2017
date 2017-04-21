#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#define msg_dbg(msj, ...)  fprintf(stdout, msj"\n", ##__VA_ARGS__);
#else
#define msg_dbg(msj, ...)
#endif
#define msg_err(msj, ...)  fprintf(stderr, msj"\n", ##__VA_ARGS__);

void modify_image(unsigned char *img, int width, int height)
{
  int col = 0, row = 0;

#ifdef USE_COLUMN_MAJOR
  for (col = 0; col < width; col++) {
    for (row = 0; row < height; row++) {
      img[row*width + col]/=2;
    }
  }
#endif
#ifdef USE_ROW_MAJOR
  for (row = 0; row < height; row++) {
    for (col = 0; col < width; col++) {
      img[row*width + col]/=2;
    }
  }
#endif
}

int main (int c, char** v)
{
  char *image_name;
  int width, height;

  if ( c < 4 ) {
    msg_err("Usage: %s <image.gray> <width> <height>", v[0]);
    return -1;
  }
  image_name = v[1];
  width = atoi(v[2]);
  height = atoi(v[3]);

  msg_dbg("Loading image: %s", image_name);
  
  unsigned char img[width * height];
  FILE *fd;

  fd = fopen(image_name, "r");
  if (fd < 0) {
    msg_err("Could not open image file %s", image_name);
    return -1;
  }

  fread(img, 1, width*height, fd);
  fclose(fd);

  modify_image(img, width, height);

  fd = fopen("output.gray", "w+");
  if (fd < 0) {
    msg_err("Could not create output image");
    return -1;
  }

  fwrite(img, 1, width*height, fd);


  fclose(fd);
  return 0;
}
