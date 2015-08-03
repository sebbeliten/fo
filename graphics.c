#include <SDL/SDL.h>
#include <stdint.h>

typedef struct
{
  SDL_Surface **frame;
  uint32_t fps, cur_index, max_indices, timer;
}ganim_t;

#define gmakecolor(r,g,b) ((SDL_Color){r,g,b})
#define gmakerect(x1,y1,x2,y2) ((SDL_Rect){x1,y1,x2,y2})

SDL_Surface *gsurface(int width, int height, int bpp /*bits per pixel*/, uint32_t flags);
SDL_Surface *gsurfpix(void *pixels, int width, int height, int bpp)
{
  uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  return SDL_CreateRGBSurfaceFrom(pixels, width, height, bpp, width * bpp, rmask, gmask, bmask, amask);
}

/* address of the pixel at x,y */
#define gpixa(surface,x,y) (((surface)->pixels + (y) * (surface)->pitch + (x) * (surface)->format->BytesPerPixel))
/* write a pixel at x,y */
#define gpixw(surface,x,y,pixel) gpixaw(gpixa(surface,x,y), (surface)->format->BytesPerPixel, pixel)

/* get rgb color as integer */
#define gpixel(surface,r,g,b) SDL_MapRGB((surface)->format,r,g,b)

/* read the pixel at x,y */
#define gpixr(surface,x,y) gpixar(gpixa(surface,x,y), (surface)->format->BytesPerPixel)

void gpixaw (uint8_t *p, int bytesperpixel, uint32_t pixel);
uint32_t gpixar (uint8_t *p, int bytesperpixel);
void gline (SDL_Surface *target, int x0, int y0, int x1, int y1, uint32_t pixel);
void goutrect (SDL_Surface *target, int x0, int y0, int x1, int y1, uint32_t pixel);

void intswap (int *a, int *b)
{
  int tmp;
  tmp = *b;
  *b  = *a;
  *a  = tmp;
}

/*
    flags
      SDL_HWSURFACE -- SDL_LockSurface() and SDL_UnlockSurface()
        surfaces reside in video memory
        calling lock and unlock, copies from video mem to ram, thus pixel access is slower
      SDL_SWSURFACE -- 
        surfaces reside in system memory, ie, ram.
        thus pixel access is faster. alpha blitting (transparency) requires pixel access,
        so this is good option for transparent blits and individual pixel access.
*/

SDL_Surface *gsurface(int width, int height, int bpp /*bits per pixel*/, uint32_t flags)
{
  uint32_t rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  return SDL_CreateRGBSurface(flags, width, height, bpp, rmask, gmask, bmask, amask);
}
/* address of the pixel at x,y */
//#define gpixa(surface,x,y) (((surface)->pixels + (y) * (surface)->pitch + (x) * (surface)->format->BytesPerPixel))
///* write a pixel at x,y */
//#define gpixw(surface,x,y,pixel) gpixaw(gpixa(surface,x,y), (surface)->format->BytesPerPixel, pixel)
//
///* get rgb color as integer */
//#define gpixel(surface,r,g,b) SDL_MapRGB((surface)->format,r,g,b)
//
///* read the pixel at x,y */
//#define gpixr(surface,x,y) gpixar(gpixa(surface,x,y), (surface)->format->BytesPerPixel)

//pixel address write
void gpixaw (uint8_t *p, int bytesperpixel, uint32_t pixel)
{
  switch (bytesperpixel)
    {
    case 1:
      *p = pixel;
      break;
    case 2:
      *(uint16_t *)p = pixel;
      break;
    case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        p[0] = (pixel >> 16) & 0xff, p[1]
          = (pixel >> 8) & 0xff, p[2] = pixel & 0xff;
#else
        p[0] = pixel & 0xff, p[1] 
          = (pixel >> 8) & 0xff, p[2] = (pixel >> 16) & 0xff;
#endif
      break;
    case 4:
      *(uint32_t *)p = pixel;
      break;
    }
}

//pixel address read
uint32_t gpixar (uint8_t *p, int bytesperpixel)
{
  switch(bytesperpixel)
    {
    case 1:
      return *p;
    case 2:
      return *(uint16_t *)p;
    case 3:
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        return p[0] << 16 | p[1] << 8 | p[2];
#else
        return p[0] | p[1] << 8 | p[2] << 16;
#endif
    case 4:
      return *(uint32_t *)p;
    }
}


void gline (SDL_Surface *target, int x0, int y0, int x1, int y1, uint32_t pixel)
{
  int 
    steep, deltax, deltay, error, ystep;
  steep = abs (y1 - y0) > abs (x1 - x0);
  if (steep)
    { intswap (&y0,&x0); intswap (&y1, &x1); }
  if (x0 > x1)
    { intswap (&x1,&x0); intswap (&y1, &y0); }
  deltax = x1 - x0;
  deltay = abs (y1 - y0);
  /*
    squares got diagonal equal to a direct line, each pixel at a new y
    rectangles's diagonals requires some pixels to occur on the same y
   */
  error  = deltax / 2;
  if (y0 < y1)
    ystep =  1;
  else
    ystep = -1;
  for (; x0 < x1; ++ x0)
    {
      if (steep)
        gpixw (target, y0, x0, pixel);
      else
        gpixw (target, x0, y0, pixel);
      error = error - deltay;
      if (error < 0)
        {
          y0    = y0 + ystep;
          error = error + deltax; 
        }
    }
}

void goutrect (SDL_Surface *target, int x0, int y0, int x1, int y1, uint32_t pixel)
{
  gline (target, x0, y0, x0, y1, pixel);
  gline (target, x0, y0, x1, y0, pixel);
  gline (target, x1, y0, x1, y1, pixel);
  gline (target, x0, y1, x1, y1, pixel);
}
