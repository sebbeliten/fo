#include <SDL/SDL.h>

/*
    You only have power because we let you, as power is not money, but people.
    Face soldiers towards their families and their chains will vanish.
*/


/*
    https://en.wikipedia.org/wiki/Adder_(electronics)
    https://en.wikipedia.org/wiki/VHDL
    https://en.wikipedia.org/wiki/Semiconductor_intellectual_property_core
    https://en.wikipedia.org/wiki/Trusted_Computing_Group
    https://en.wikipedia.org/wiki/Microsoft
*/

//###
//### higeneral
//### 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if ON_OS_LINUX
#define FILEPATH_SLASH '/'
#define FILENAME_HIDDEN '.'
#elif ON_OS_WINDOWS
#define FILEPATH_SLASH '\\'
#define FILENAME_HIDDEN '_'
#else
#error unknown os, cannot define FILEPATH_SLASH. therefore cannot read .fofrm correctly
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

#ifndef isletter
#define isletter(character) (character >= 'a' && character <= 'z' || character >= 'A' && character <= 'Z')
#endif
#ifndef isnumber
#define isnumber(number) (number >= '0' && number <= '9')
#endif
#ifndef iscblank
#define iscblank(character) (character == ' ' || character == '\t')
#endif

#ifndef tolower
#define tolower(character) ((character >= 'A' && character <= 'Z') ? (character+'a'-'A'):(character))
#endif

#if ON_OS_LINUX
int stricmp(char *str1,char *str2)
{
  while(*str1 && *str2 && tolower(*str1) == tolower(*str2))
    str1++,str2++;
  if (*str1 < *str2)
    return -1;
  //printf("%d\n", 'a' - 'A');
  //printf("%c = %c\n", tolower(*str1), tolower(*str2));
  return *str1 != *str2;
}
#endif

int isstrnum(char *str)
{
  if (*str == '-')
    ++str;
  while(isnumber(*str))
    ++str;
  return !*str;
}

char *file_read_entire(char *name, char *mode, long plusalloc, long *nrbytes)
{
  FILE *stream;
  if (!(stream=fopen(name,mode)))
    exit(1);
  fseek(stream,0,SEEK_END);
  //long b = ftell(stream);
  if (*nrbytes == 0)
    *nrbytes = ftell(stream);
  char *txt = malloc(*nrbytes + plusalloc);
  fseek(stream,0,SEEK_SET);
  if(*nrbytes != fread(txt,1,*nrbytes,stream))
    exit(1);
  fclose(stream);
  return txt;
}

int aritnmemb(void *p, char *foo, int value, long nmemb)
{
  //aritnmemb(pal,"b1;e3;+1;*=;",PAL_HIGH_NOON,256 * 4);
  int nsize = 0; //size of those
  int those = 0; //operate on
  int  skip = 0; //skip skip bytes after those
  #define multiplyequal 0
  int  oper = 0; //operation
  while(*foo)
  {
    char *a = foo;
    while(*foo != ';')
      ++foo;
    *foo = '\0';
    if (*a == 'b')
      nsize = atoi(a + 1);
    else
    if (*a == 'e')
      those = atoi(a + 1);
    else
    if (*a == '+' && isnumber(a[1]))
      skip = atoi(a + 1);
    else
    if (!strcmp("*=",a + 1))
      oper = multiplyequal;
    ++foo;
  }
  void *P = p + nmemb;
  while(p < P)
  {
    if(oper == multiplyequal)
    {
      int n;
      for(n = 0; n < those; ++n)
        if (nsize == 1)
          ;
          /*
          if (
          *(uint8_t *)p *= */
    }
  }
}

//###
//### hiini
//### stdlib.h stdio.h
#include <stdlib.h>
#include <stdio.h>
typedef struct ini_t
{
  char *id;
  char *string;
  int   integer[4];
  int   count;
  char *comment;
  struct ini_t *nxt;
}ini_t;

/*
    
    .ini   ;; default.ini, faction.ini
    .fofrm ;; works for .fofrms as well
    TODO: error checking / reporting
    [OK]  investigate what DOS file is. iface.fofrm reads 1 more byte and corrupts ini reading
          -> '\r\n' = DOS newline, '\n' = UNIX newline

    the most danger is ignorance.
    people with secrets maneuvering unknowing men, men whose desires are adjusted to the level of 
    their perceived selfs.
    they cover their needs, kneeling, begging, but quietly complaining. because the illusion of social
    standing is too great. a commoner can't resolve his mind, the place in which his world is perceived.
    he must turn to the ancient writings of the past. they were made by people like any else.
*/

ini_t *ini_read(char *name)
{
  long b = 0;
  char *txt = file_read_entire(name,"r",1,&b);
  txt[b] = '\n';
  ini_t *ini = calloc(sizeof(ini_t),1);
  ini_t *first = ini;
  char *s = txt, *a = txt + b;
  for(; s < a;)
  {
    /*
        A=1
        B =1
        C= 1
        D = 1
    */
    /* ID = VALUE #... */
    /* resolution 640 480 #... */
    /* [section] #... */
    char *S = s;
    //A=1 \n
    for(;*s != '\n'; ++s)
      if (ini->comment)
        continue;
      else
      if (*s == '#')
        ini->comment = s, *s = '\0';
      else
      if (iscblank(*s) || *s == '=' || /* DOS CLRF Line Terminator: \r\n */ *s == '\r')
        *s = '\0';
    while(*S == '\0')
      ++S;
    if (S == s || (ini->comment && S >= ini->comment)) /* empty line / comment line */
    {
      ini->comment = NULL;
      ++s;
      continue;
    }
    ini->id = S;
    #define getvalue \
    while(*S != '\0') \
    { \
      if (S == ini->comment || S == s) \
        goto next; \
      ++S; \
    } \
    while(*S == '\0') \
      ++S; \
    if (S == ini->comment || S == s) \
      goto next;
    getvalue;
    {
      char *p = S;
      if (*p == '-')
        ++p;
      while(isnumber(*p))
        ++p;
      if (*p == '\0' || p == s)
      {
        ini->integer[0] = atoi(S);
        ini->count = 1;
      }
      else
        ini->string = S;
    }
    getvalue;
    ini->integer[1] = atoi(S);
    ini->count = 2;
    getvalue;
    ini->integer[2] = atoi(S);
    ini->count = 3;
    getvalue;
    ini->integer[3] = atoi(S);
    ini->count = 4;
    next:
      *s = '\0';
      ++s;
      ini->nxt = calloc(sizeof(ini_t),1);
      ini = ini->nxt;
  }
  return first;
}

/*
    Generate a lookup of INI parameter types based on existing INIs (default interface)
      * STRING: IMAGE
      * COORDINATE: X0,Y0,X1,Y0
      * COORDINATE: X0,Y0
      * COORDINATE: X
      * COORDINATE: Y
      * OFFSET: X
      * OFFSET: Y
      * STEP: X
      * STEP: Y
      * COUNT: N
*/
void ini_generate_ini_lookup(char **name_list, int count, char *name)
{
  
  //ini_generate_ini_lookup(list,"data\\lookup.ini");
  ini_t *ini[count +1];// = malloc(sizeof(ini_t) * (count + 1));
  int n;
  n = 0;
  while(n < count)
  {
    ini[n] = ini_read(name_list[n]);
    ini_t *m;
    while(m->nxt != NULL)
    {
      
      m = m->nxt;
    }
    //++ini;
    ++n;
  }
  //ini -= count;
}

//###
//### hipalette
//###
#include <stdio.h>
#include <stdint.h>
#include "endianess.h"
/*
    palette is 256 * 3 bytes of RGB data
*/
typedef struct
{
  uint8_t r, g, b, a;
}rgba_t;

/*
    Multiplying palette gives the desired brightness of FRM in night, morning, forenoon or high noon (in-game).
*/
#define PAL_NIGHT                  0x01 //NIGH = there is no sun
#define PAL_MORNING_AND_DAWN       0x02 //DAWN = beginning of twilight before sunrise (sun is not in horizon)
#define PAL_FORENOON_AND_AFTERNOON 0x03 //FORENOON = sunrise to noon, AFTERNOON = noon to evening
#define PAL_HIGH_NOON              0x04 //HIGH NOON = SOLAR NOON = sun is highest above horizon

/*
    TODO: better way to set brightness
*/
void pal_setbrightness(rgba_t *pal, int brightness)
{
  static int prev = PAL_NIGHT;
  rgba_t *lap = pal + 256;
  while(pal < lap)
  {
    pal->r /= prev;
    pal->g /= prev;
    pal->b /= prev;
    pal->r *= brightness;
    pal->g *= brightness;
    pal->b *= brightness;
    ++pal;
  }
  prev = brightness;
}

/*
    this is the first 768 bytes of color.pal
*/
unsigned char pal_default_palette[] = 
{
  0xff, 0xff, 0xff, 0x3b, 0x3b, 0x3b, 0x37, 0x37, 0x37, 0x33, 0x33, 0x33,
  0x2f, 0x2f, 0x2f, 0x2c, 0x2c, 0x2c, 0x28, 0x28, 0x28, 0x24, 0x24, 0x24,
  0x20, 0x20, 0x20, 0x1d, 0x1d, 0x1d, 0x19, 0x19, 0x19, 0x15, 0x15, 0x15,
  0x12, 0x12, 0x12, 0x0e, 0x0e, 0x0e, 0x0a, 0x0a, 0x0a, 0x08, 0x08, 0x08,
  0x3f, 0x3b, 0x3b, 0x3b, 0x36, 0x36, 0x37, 0x31, 0x31, 0x34, 0x2c, 0x2c,
  0x30, 0x28, 0x28, 0x2c, 0x24, 0x24, 0x29, 0x20, 0x20, 0x25, 0x1c, 0x1c,
  0x21, 0x18, 0x18, 0x1e, 0x15, 0x15, 0x1a, 0x11, 0x11, 0x16, 0x0e, 0x0e,
  0x13, 0x0b, 0x0b, 0x0f, 0x09, 0x09, 0x0b, 0x06, 0x06, 0x08, 0x04, 0x04,
  0x3b, 0x3b, 0x3f, 0x36, 0x36, 0x3b, 0x31, 0x31, 0x37, 0x2c, 0x2c, 0x34,
  0x28, 0x28, 0x30, 0x24, 0x24, 0x2c, 0x20, 0x20, 0x29, 0x1c, 0x1c, 0x25,
  0x18, 0x18, 0x21, 0x15, 0x15, 0x1e, 0x11, 0x11, 0x1a, 0x0e, 0x0e, 0x16,
  0x0b, 0x0b, 0x13, 0x09, 0x09, 0x0f, 0x06, 0x06, 0x0b, 0x04, 0x04, 0x08,
  0x3f, 0x2c, 0x3c, 0x31, 0x18, 0x2a, 0x1a, 0x09, 0x18, 0x13, 0x05, 0x12,
  0x0e, 0x03, 0x0d, 0x0a, 0x04, 0x09, 0x09, 0x01, 0x09, 0x07, 0x03, 0x06,
  0x3f, 0x3f, 0x32, 0x3f, 0x3f, 0x1f, 0x39, 0x36, 0x03, 0x33, 0x2e, 0x07,
  0x2e, 0x27, 0x0a, 0x29, 0x22, 0x0c, 0x24, 0x1e, 0x09, 0x1f, 0x1a, 0x06,
  0x1b, 0x16, 0x04, 0x16, 0x12, 0x02, 0x12, 0x0e, 0x01, 0x0d, 0x0a, 0x00,
  0x08, 0x06, 0x00, 0x36, 0x3f, 0x27, 0x2d, 0x36, 0x21, 0x26, 0x2e, 0x1c,
  0x1e, 0x26, 0x17, 0x17, 0x1e, 0x12, 0x10, 0x16, 0x0d, 0x0a, 0x0e, 0x08,
  0x1c, 0x18, 0x14, 0x15, 0x12, 0x0d, 0x0e, 0x0c, 0x08, 0x1a, 0x1e, 0x14,
  0x1c, 0x1e, 0x08, 0x1c, 0x1a, 0x0a, 0x18, 0x18, 0x09, 0x13, 0x11, 0x09,
  0x0e, 0x0c, 0x08, 0x27, 0x2b, 0x27, 0x1e, 0x25, 0x1e, 0x16, 0x1f, 0x16,
  0x10, 0x1a, 0x10, 0x0e, 0x16, 0x16, 0x0c, 0x13, 0x12, 0x0a, 0x11, 0x0f,
  0x08, 0x0f, 0x0b, 0x07, 0x0c, 0x09, 0x05, 0x0a, 0x06, 0x04, 0x08, 0x04,
  0x06, 0x0c, 0x06, 0x04, 0x09, 0x03, 0x02, 0x07, 0x01, 0x01, 0x05, 0x00,
  0x01, 0x03, 0x00, 0x23, 0x27, 0x27, 0x1e, 0x25, 0x26, 0x19, 0x22, 0x25,
  0x14, 0x1f, 0x24, 0x10, 0x1b, 0x23, 0x0c, 0x16, 0x23, 0x0b, 0x13, 0x1f,
  0x0a, 0x11, 0x1b, 0x08, 0x0e, 0x17, 0x07, 0x0c, 0x13, 0x06, 0x0a, 0x10,
  0x27, 0x29, 0x29, 0x0e, 0x12, 0x1a, 0x14, 0x16, 0x16, 0x16, 0x1a, 0x21,
  0x0e, 0x10, 0x14, 0x2f, 0x2f, 0x2f, 0x2b, 0x29, 0x26, 0x28, 0x24, 0x1f,
  0x25, 0x1f, 0x18, 0x22, 0x1a, 0x13, 0x1f, 0x16, 0x0d, 0x1c, 0x12, 0x09,
  0x19, 0x0f, 0x05, 0x16, 0x0c, 0x02, 0x3f, 0x33, 0x33, 0x3f, 0x2c, 0x2c,
  0x3f, 0x26, 0x26, 0x3f, 0x1f, 0x1f, 0x3f, 0x19, 0x19, 0x3f, 0x12, 0x12,
  0x3f, 0x0c, 0x0c, 0x3f, 0x00, 0x00, 0x38, 0x00, 0x00, 0x31, 0x00, 0x00,
  0x2a, 0x00, 0x00, 0x24, 0x00, 0x00, 0x1d, 0x00, 0x00, 0x16, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x3f, 0x38, 0x32, 0x3f, 0x31, 0x25, 0x3f, 0x2e, 0x1e,
  0x3f, 0x2b, 0x18, 0x3f, 0x27, 0x12, 0x3f, 0x25, 0x0b, 0x3f, 0x22, 0x05,
  0x3f, 0x1f, 0x00, 0x37, 0x1b, 0x00, 0x30, 0x18, 0x00, 0x29, 0x14, 0x00,
  0x21, 0x11, 0x00, 0x1a, 0x0d, 0x00, 0x13, 0x09, 0x00, 0x0c, 0x06, 0x00,
  0x3e, 0x35, 0x29, 0x36, 0x2c, 0x1e, 0x32, 0x28, 0x19, 0x2f, 0x24, 0x15,
  0x2b, 0x20, 0x11, 0x27, 0x1d, 0x0d, 0x23, 0x19, 0x0a, 0x1f, 0x16, 0x07,
  0x1c, 0x13, 0x05, 0x18, 0x10, 0x02, 0x14, 0x0d, 0x01, 0x10, 0x0a, 0x00,
  0x0d, 0x08, 0x00, 0x3f, 0x39, 0x2e, 0x3a, 0x32, 0x26, 0x35, 0x2b, 0x1f,
  0x31, 0x24, 0x19, 0x2c, 0x1d, 0x13, 0x28, 0x17, 0x0e, 0x24, 0x13, 0x0b,
  0x21, 0x0f, 0x08, 0x1e, 0x0b, 0x06, 0x1b, 0x08, 0x04, 0x17, 0x05, 0x02,
  0x12, 0x03, 0x01, 0x0f, 0x01, 0x00, 0x3f, 0x3a, 0x37, 0x3e, 0x35, 0x2f,
  0x3d, 0x30, 0x28, 0x3c, 0x2c, 0x21, 0x3c, 0x28, 0x1b, 0x3c, 0x25, 0x17,
  0x36, 0x20, 0x15, 0x30, 0x1c, 0x12, 0x2a, 0x18, 0x10, 0x24, 0x14, 0x0e,
  0x1e, 0x10, 0x0c, 0x18, 0x0c, 0x09, 0x12, 0x09, 0x07, 0x0e, 0x06, 0x05,
  0x19, 0x39, 0x19, 0x05, 0x26, 0x05, 0x00, 0x29, 0x00, 0x14, 0x14, 0x12,
  0x00, 0x1b, 0x00, 0x23, 0x23, 0x21, 0x07, 0x07, 0x07, 0x1a, 0x14, 0x0e,
  0x0c, 0x0a, 0x08, 0x23, 0x1c, 0x18, 0x12, 0x0e, 0x0a, 0x03, 0x03, 0x03,
  0x0f, 0x0f, 0x0f, 0x1b, 0x1d, 0x1b, 0x1e, 0x21, 0x1e, 0x22, 0x25, 0x22,
  0x25, 0x29, 0x25, 0x16, 0x1a, 0x18, 0x18, 0x1c, 0x1a, 0x0f, 0x3e, 0x00,
  0x0e, 0x35, 0x02, 0x0d, 0x2d, 0x04, 0x0c, 0x25, 0x05, 0x0a, 0x1d, 0x06,
  0x3f, 0x3f, 0x3f, 0x3c, 0x3b, 0x34, 0x34, 0x2e, 0x22, 0x26, 0x1f, 0x14,
  0x1a, 0x16, 0x0f, 0x14, 0x10, 0x09, 0x0d, 0x0a, 0x07, 0x06, 0x04, 0x03,
  0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

rgba_t *pal_read_mem()
{
  char *pal = malloc(256 * 3 + 256);
  memcpy(pal,pal_default_palette,256 * 3);
  
  //copy RGB[3*256] to RGBA[4*256] format (easily cast to (SDL_Color *))
  char *c, *d;
  c = pal + 256 * 3 -3, d = pal + 256 * 4 -4;
  for(; c != pal; c -= 3, d -= 4)
    memcpy(d,c,3);
  return (rgba_t *)pal;
}

rgba_t *pal_read(char *name)
{
  long b = 256 * 3;
  char *pal;
  if (!name)
  {
    pal = malloc(b + 256);
    memcpy(pal,pal_default_palette,b);
  }
  else
    file_read_entire(name,"rb",256,&b);
  
  //copy RGB[3*256] to RGBA[4*256] format (easily cast to (SDL_Color *))
  char *c, *d;
  c = pal + 256 * 3 -3, d = pal + 256 * 4 -4;
  for(; c != pal; c -= 3, d -= 4)
    memcpy(d,c,3);
  /*
  int n;
  rgba_t *p = (rgba_t *)pal;
  for (n = 0; n < 256; ++ n)
    printf("%d,%d,%d\n",p[n].r,p[n].g,p[n].b);
  exit(1);
  */
  /*
      HardCoded Animated RGB's
       Source: https://github.com/Primagen/Fallout-FRM-Viewer/blob/master/ImageManager.cpp
      Article: http://falloutmods.wikia.com/wiki/Pal_animations
         TODO: read article
  */
  {
    rgba_t *c = (rgba_t *)pal;
    //Slime
    c[229].r = 0;
    c[229].g = 108;
    c[229].b = 0;

    c[230].r = 11;
    c[230].g = 115;
    c[230].b = 7;

    c[231].r = 27;
    c[231].g = 123;
    c[231].b = 15;

    c[232].r = 43;
    c[232].g = 131;
    c[232].b = 27;
    //Monitors
    c[233].r = 107;
    c[233].g = 107;
    c[233].b = 111;

    c[234].r = 99;
    c[234].g = 103;
    c[234].b = 127;

    c[235].r = 87;
    c[235].g = 107;
    c[235].b = 143;

    c[236].r = 0;
    c[236].g = 147;
    c[236].b = 163;

    c[237].r = 107;
    c[237].g = 187;
    c[237].b = 255;
    //Slow Fire
    c[238].r = 255;
    c[238].g = 0;
    c[238].b = 0;

    c[239].r = 215;
    c[239].g = 0;
    c[239].b = 0;

    c[240].r = 147;
    c[240].g = 43;
    c[240].b = 11;

    c[241].r = 255;
    c[241].g = 119;
    c[241].b = 0;

    c[242].r = 255;
    c[242].g = 59;
    c[242].b = 0;
    //Fast Fire
    c[243].r = 71;
    c[243].g = 0;
    c[243].b = 0;

    c[244].r = 123;
    c[244].g = 0;
    c[244].b = 0;

    c[245].r = 123;
    c[245].g = 0;
    c[245].b = 0;

    c[246].r = 71;
    c[246].g = 0;
    c[246].b = 0;

    c[247].r = 71;
    c[247].g = 0;
    c[247].b = 0;
    //Shoreline
    c[248].r = 83;
    c[248].g = 63;
    c[248].b = 43;

    c[249].r = 75;
    c[249].g = 59;
    c[249].b = 43;

    c[250].r = 67;
    c[250].g = 55;
    c[250].b = 39;

    c[251].r = 63;
    c[251].g = 51;
    c[251].b = 39;

    c[252].r = 55;
    c[252].g = 47;
    c[252].b = 35;

    c[253].r = 51;
    c[253].g = 43;
    c[253].b = 35;
    //Alarm
    c[254].r = 252;
    c[254].g = 0;
    c[254].b = 0;
    //-----
    c[255].r = 255;
    c[255].g = 255;
    c[255].b = 255;
  }
  return (rgba_t *)pal;
}

//###
//### hifrm
//### stdio.h stdlib.h stdint.h endianess.h SDL.h (SDL_BYTEORDER)
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "endianess.h"
/*
    Sources: http://falloutmods.wikia.com/wiki/FRM_File_Format
             http://falloutmods.wikia.com/wiki/FRM_files
    ----------------------------------------------------------
    FRM File Format - PixMap format used in Fallout, Fallout 2 and FOnline to store image data
                      for up to 6 orientations (1 for each hex/tile direction),
                      animations are also supported by it.
            Endianess: Big Endian
      Pixel Bit Depth: 8
           Palettized: true (external file: color.pal)
         Transparency: Pixel Value 0 Is 100% Transparent (Regardless of Palette)

       u 32-bit :: version
       u 16-bit :: frames per second
       u 16-bit :: the frame (0-N) on which an in-game action first occurs (opening a door, firing weapon,etc)
       u 16-bit :: count of frames, each orientation will contain this amount of frames: N
   [6] i 16-bit :: ? pixel shift X (whatever pixel shift is): binary shift <</>> on pixel coordinates?
   [6] i 16-bit :: ? pixel shift Y: allegedly required to center images to their logical centers (eg: feet)
   [6] u 32-bit :: offset from 0x003E to the given orientation(0-5)'s 0,th frame
       u 32-bit :: byte count summing all frames for all orientations. aka: size of frame area
    ?  u 16-bit :: pixel  width of 1 frame
    ?  u 16-bit :: pixel height of 1 frame
    ?  u 32-bit :: count of pixels in 1 frame
    ?  i 16-bit :: ? offset in X direction of frame 0: offset from the previous frame to this frame
    ?  i 16-bit :: ? offset in Y direction of frame 0

    whatever this means:
      left = center_x - (frame_width / 2)
      top  = center_y - frame_height

    TODO: most animations and static images are not rendered correctly
          I think this is because I have ignored pixel shift and x/y offset
*/

#define FRM_SIZEOF_GLOBAL_HEADER (0x003E)
#define FRM_SIZEOF_FRAME_HEADER  (0x004A - FRM_SIZEOF_GLOBAL_HEADER)
#define FRM_MAX_ORIENTATIONS     6

typedef struct
{
  uint32_t version;
  uint16_t fps;
  uint16_t nr_actionframe;
  uint16_t fpo;
  uint16_t orientation_count;
  struct frm_orientation_t
  {
     int16_t x_shift;
     int16_t y_shift;
    uint32_t offset; //from framearea: 0x003E
    struct frm_frame_t
    {
      uint16_t width;
      uint16_t height;
      uint32_t nr_pixels;
       int16_t x_offset;
       int16_t y_offset;
    } *frame;
  }orientation[FRM_MAX_ORIENTATIONS];
  uint32_t sizeof_framearea;
  uint8_t *framearea; //free(framearea - FRM_SIZE_OF_GLOBAL_HEADER)
}frm_t;

frm_t frm_read(char *name)
{
  long b = 0;
  char *bts = file_read_entire(name,"rb",0,&b);

  #if SDL_BYTEORDER != SDL_BIG_ENDIAN
    #define get(field) \
      if (sizeof(frm.field) == 2) \
        frm.field = biglittle2(*bts), bts += 2; \
      else \
        frm.field = biglittle4(*bts), bts += 4;
  #else
    #define get(field) \
      if (sizeof(frm.field) == 2) \
        frm.field = *(uint16_t *)bts, bts += 2; \
      else \
        frm.field = *(uint32_t *)bts, bts += 4;
  #endif
  frm_t frm;
  /* retrieve global header */
  get(version);
  get(fps);
  get(nr_actionframe);
  get(fpo);
  {
    int o;
    for(o = 0; o < FRM_MAX_ORIENTATIONS; ++o)
      get(orientation[o].x_shift);
    for(o = 0; o < FRM_MAX_ORIENTATIONS; ++o)
      get(orientation[o].y_shift);
    for(o = 0; o < FRM_MAX_ORIENTATIONS; ++o)
    {
      get(orientation[o].offset);
      //printf("Offset: %d\n", frm.orientation[o].offset);
    }
    
    get(sizeof_framearea);
    frm.framearea = bts;

    /* each frame got a header denoting its size + xy offset from previous frame */
    
    #define orient frm.orientation[o]
    o = 0;
    do
    {
      orient.frame = malloc(frm.fpo * sizeof(struct frm_frame_t));
      
      //bts = frm.framearea + orient.offset;

      int f;
      for(f = 0; f < frm.fpo; ++f)
      {
        #define frame orientation[o].frame[f]
        get(frame.width);
        get(frame.height);
        get(frame.nr_pixels);
        get(frame.x_offset);
        get(frame.y_offset);
        #undef frame
        //printf("%d,%d\n", orient.frame[f].x_offset, orient.frame[f].y_offset);
        bts += orient.frame[f].nr_pixels;
      }
      
      ++o;
    }while(o < FRM_MAX_ORIENTATIONS && /* break if next offset is 0 (= 2nd offset) */ orient.offset != 0);
    #undef orient
    frm.orientation_count = o;
  }
  return frm;
}

/*
    rest depends on SDL 1.2 and graphics.c
*/

//###
//### hifofrm
//### general ini stdint.h SDL.h SDL_image.h
#include <SDL/SDL_image.h>
#include <stdint.h>

typedef struct
{
  uint16_t fps;
  uint32_t count;
  uint32_t offs_x;
  uint32_t offs_y;
  SDL_Surface **frame[6];
}fofrm_t;

/*
    .FOFRM File Format - Equivalent to .FRM for .PNGs except the header information is stored in plain .FOFRM
                         text files while the actual PNGs are stored in separate files.
      fps     ;;
      count   ;; total frames per orientation
      offs_x  ;;
      offs_y  ;;
      frm     ;; .PNG
      [dir_X] ;; orientation X (.PNGs follows)
      frm_X   ;; Xth frame of orientation [dir_X]
*/

fofrm_t fofrm_read(char *name)
{
  fofrm_t fofrm;
  memset(&fofrm,0,sizeof(fofrm_t));
  ini_t *ini = ini_read(name);
  while(ini->nxt)
  {
    if (!stricmp(ini->id,"fps"))
      fofrm.fps = ini->integer[0];
    else
    if (!stricmp(ini->id,"count"))
      fofrm.count = ini->integer[0];
    else
    if (!stricmp(ini->id,"offs_x"))
      fofrm.offs_x = ini->integer[0];
    else
    if (!stricmp(ini->id,"offs_y"))
      fofrm.offs_y = ini->integer[0];
    else
    if (!stricmp(ini->id,"frm"))
      fofrm.frame[0][fofrm.count++] = IMG_Load(ini->string);
    else
    if (   !stricmp(ini->id,"[dir_0]")
        || !stricmp(ini->id,"[dir_1]") 
        || !stricmp(ini->id,"[dir_2]") 
        || !stricmp(ini->id,"[dir_3]") 
        || !stricmp(ini->id,"[dir_4]") 
        || !stricmp(ini->id,"[dir_5]") )
    {
      int orient = ini->id[6 - 1] - '0';
      int count = 0;
      fofrm.frame[orient] = malloc(sizeof(SDL_Surface *) * fofrm.count);
      while(count < fofrm.count)
      {
        ini = ini->nxt;
        fofrm.frame[orient][atoi(ini->id + 4)] = IMG_Load(ini->string);
        ++count;
      }
    }
    ini = ini->nxt;
  }
  return fofrm;
}

//###
//### 
//###
#include "graphics.c"

int gpalette(ganim_t *anim, void *palette)
{
  int n;
  for(n = 0; n < anim->max_indices; ++n)
    SDL_SetPalette(anim->frame[n], SDL_LOGPAL, (SDL_Color *)palette, 0, 255);
}

/*
    TODO: select orientation
*/
ganim_t frm_ganim(char *name)
{
  
  frm_t frm = frm_read(name);
  ganim_t anim;
  memset(&anim,0,sizeof(ganim_t));
  anim.frame = malloc(sizeof(SDL_Surface *) * frm.fpo);
  {
    #define orient frm.orientation[0]
    uint32_t offset = FRM_SIZEOF_FRAME_HEADER;
    int f;
    for(f = 0; f < frm.fpo; ++f)
    {
      anim.frame[f] = gsurface(orient.frame[f].width,
                               orient.frame[f].height,
                               8,
                               SDL_SWSURFACE);
      memcpy(anim.frame[f]->pixels, 
             frm.framearea + orient.offset + offset,
             orient.frame[f].nr_pixels);
      offset += orient.frame[f].nr_pixels + FRM_SIZEOF_FRAME_HEADER;
      //SDL_SetPalette(anim.frame[f], SDL_LOGPAL, palette, 0, 255);
      //anim.frame[f]->format->palette->colors = palette;
      SDL_SetColorKey(anim.frame[f], SDL_SRCCOLORKEY, 0);
    }
    #undef orient
  }
  
  free(frm.framearea - FRM_SIZEOF_GLOBAL_HEADER);
  
  anim.max_indices = frm.fpo;
  anim.fps = frm.fps;
  anim.timer = SDL_GetTicks();
  return anim;
}

/*
    TODO: select orientation
*/
ganim_t fofrm_ganim(char *name)
{
  fofrm_t fofrm = fofrm_read(name);
  ganim_t anim;
  memset(&anim,0,sizeof(ganim_t));
  anim.frame = fofrm.frame[0];
  anim.max_indices = fofrm.count;
  anim.fps = fofrm.fps;
  anim.timer = SDL_GetTicks();
  return anim;
}

int LoadFRM(ganim_t *ganim, char *name, void *palette)
{
  ganim_t anim;
  int l = strlen(name);
  if (l > 6 && !stricmp(name + l - 6,".fofrm"))
  {
    char pwd[256];
    getcwd(pwd, 256);
    while(name[--l] != FILEPATH_SLASH)
    {
      if (l == 0)
        break;
    }
    if (name[l] == FILEPATH_SLASH)
    {
      name[l] = '\0';
      chdir(name);
      name[l] = FILEPATH_SLASH;
      anim = fofrm_ganim(name + l + 1);
      chdir(pwd);
    }
    else
      anim = fofrm_ganim(name);
  }
  else
  //TODO: Name.fr[0-5] and Name.pal
  if (l > 4 && !stricmp(name + l - 4,".frm"))
  {
    anim = frm_ganim(name);
    //SDL_Color *palette2 = (SDL_Color *)pal_read(NULL);
    gpalette(&anim, palette);
  }
  *ganim = anim;
  return 0;
  //exit(0);
}
