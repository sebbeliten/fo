/*
    http://www.merriam-webster.com/dictionary/askance
    http://www.merriam-webster.com/dictionary/impede
    http://www.merriam-webster.com/dictionary/implode
    http://www.merriam-webster.com/dictionary/buffoon
    http://www.merriam-webster.com/dictionary/coerce
    http://www.merriam-webster.com/dictionary/ludicrous
    http://www.merriam-webster.com/dictionary/facsimile
    http://www.merriam-webster.com/dictionary/adjunct
    http://www.merriam-webster.com/dictionary/petition
      earnest
    http://www.merriam-webster.com/dictionary/solicit
      entice
      entreat
      importune

*/


#include "fo.c"
#include "macroopt.h"

//###
//### himain
//###
//### http://www.libsdl.org/release/SDL-1.2.15/docs/html/index.html
//### http://www.libsdl.org/projects/SDL_image/docs/SDL_image_frame.html
//### http://stackoverflow.com/questions/12489/how-do-you-get-a-directory-listing-in-c
//#include "jh_fallout.c"
#include <SDL/SDL_ttf.h>
int
main(int argc, char **argv)
{
  int posend = 0;
  {
    macroopt
    case 'h':
      printf("foview files\n"
             "\tview .frm and .fofrm files\n");
      exit(1);
    case 'v':
      printf("foview 1.0\n");
      exit(1);
    done
    else 
    {
      argv[posend ++] = argv[optind];
    }
  }
  
  if (posend == 0)
    return 1;

  SDL_Color *palette = (SDL_Color *)pal_read(NULL);//pal_read_mem();//pal_read("/home/sebelius/default.pal");
  pal_setbrightness((rgba_t *)palette, 0x04);

  #define SDL_VFLAGS SDL_DOUBLEBUF|SDL_RESIZABLE|SDL_HWSURFACE
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Surface *s;
  s = SDL_SetVideoMode(0,0,32,SDL_VFLAGS);
  
  char *home = getenv("HOME");
  char *path = malloc(strlen(home) + strlen("Xfoview") + 1);
  strcpy(path,home);
  path[strlen(path) + 1] = '\0';
  path[strlen(path)] = FILEPATH_SLASH;
  path[strlen(path) + 1] = '\0';
  path[strlen(path)] = FILENAME_HIDDEN;
  strcat(path,"foview");
  printf("`%s'\n", path);

  TTF_Init();
  TTF_Font *font = TTF_OpenFont("data/JH_FALLOUT.TTF", 16);

  int n = 0;
  {
    int optind = 0;
    while(optind < posend)
    {
      int l = strlen(argv[optind]);
      if (l > 6 && !stricmp(argv[optind] + l - 6,".fofrm") || l > 4 && !stricmp(argv[optind] + l - 4,".frm"))
      {
        argv[n++] = argv[optind];
        printf("%s\n", argv[n-1]);
      }
      ++optind;
    }
  }
  if (n == 0)
    exit(1);
  posend = n;
  n = 0;

  ganim_t anim;
  LoadFRM(&anim,argv[n],palette);

  SDL_Surface *msgln; 
  msgln = TTF_RenderText_Solid(font,argv[n],gmakecolor(0,224,0));
  SDL_Surface *gtxtln;
  char txtln[256];
  sprintf(txtln,"%d/%d", n + 1, posend);
  gtxtln = TTF_RenderText_Solid(font,txtln,gmakecolor(0,224,0));
  SDL_Surface *trdmrk;
  trdmrk = TTF_RenderText_Solid(font,"(N)ext (P)revious (S)top (H)ide (Q)uit",gmakecolor(0,224,0));

  puts("------------------------------");
  int running = 1, fps = 30, active = 1, hide = 0; //fps = frm.fps;
  uint32_t fpstimer = SDL_GetTicks();
  while(running)
  {
    SDL_Event event;
    while(SDL_PollEvent(&event))
      switch(event.type)
      {
        case SDL_QUIT:
          running = false;
          break;
        case SDL_VIDEORESIZE:
          s = SDL_SetVideoMode(event.resize.w,event.resize.h,32,SDL_VFLAGS);
          break;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_ESCAPE)
            running = false;
          else
          if (event.key.keysym.sym == SDLK_j || event.key.keysym.sym == SDLK_n) //next
          {
            if (n == posend -1)
              break;
            ++n;
            //puts(argv[n]);
            LoadFRM(&anim,argv[n],palette);
            SDL_FreeSurface(msgln);
            msgln = TTF_RenderText_Solid(font,argv[n],gmakecolor(0,224,0));
            SDL_FreeSurface(gtxtln);
            sprintf(txtln,"%d/%d", n + 1, posend);
            gtxtln = TTF_RenderText_Solid(font,txtln,gmakecolor(0,224,0));
          }
          else
          if (event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_p) //previous
          {
            if (n == 0)
              break;
            --n;
            //puts(argv[n]);
            LoadFRM(&anim,argv[n],palette);
            SDL_FreeSurface(msgln);
            msgln = TTF_RenderText_Solid(font,argv[n],gmakecolor(0,224,0));
            SDL_FreeSurface(gtxtln);
            sprintf(txtln,"%d/%d", n + 1, posend);
            gtxtln = TTF_RenderText_Solid(font,txtln,gmakecolor(0,224,0));
          }
          else
          if (event.key.keysym.sym == SDLK_1)
          {
            pal_setbrightness((rgba_t *)palette, 0x01);
            gpalette(&anim, palette);
          }
          else
          if (event.key.keysym.sym == SDLK_2)
          {
            pal_setbrightness((rgba_t *)palette, 0x02);
            gpalette(&anim, palette);
          }
          else
          if (event.key.keysym.sym == SDLK_3)
          {
            pal_setbrightness((rgba_t *)palette, 0x03);
            gpalette(&anim, palette);
          }
          else
          if (event.key.keysym.sym == SDLK_4)
          {
            pal_setbrightness((rgba_t *)palette, 0x04);
            gpalette(&anim, palette);
            //pal_setbrightness((rgba_t *)anim.frame[n]->format->palette->colors, 0x04);
            //gpalette(&anim,anim.frame[n]->format->palette->colors);
          }
          else
          if (event.key.keysym.sym == SDLK_s)
            active = !active;
          else
          if (event.key.keysym.sym == SDLK_h)
            hide = !hide;
          break;
        case SDL_MOUSEBUTTONDOWN:
          break;
        case SDL_MOUSEMOTION:
          break;
      }
    
    //FRAMEBUFFER UPDATE
    if (SDL_GetTicks() - fpstimer  >= 1000 / fps)
    {
      SDL_FillRect(s,0,0);
      SDL_BlitSurface(anim.frame[anim.cur_index],0,s,
                      &gmakerect((s->w-anim.frame[anim.cur_index]->w)/2,(s->h-anim.frame[anim.cur_index]->h)/2,
                      0,0));
      if (active && anim.fps && (SDL_GetTicks() - anim.timer >= 1000 / anim.fps))
      {
        ++anim.cur_index;
        if (anim.cur_index == anim.max_indices)
          anim.cur_index = 0;
        anim.timer = SDL_GetTicks();
      }

      if (!hide)
      {
        SDL_BlitSurface(msgln,0,s,&gmakerect(0,s->h - msgln->h,0,0));
        SDL_BlitSurface(gtxtln,0,s,&gmakerect(s->w - gtxtln->w,s->h - gtxtln->h, 0, 0));
        SDL_BlitSurface(trdmrk,0,s,0);
        gline(s,0,trdmrk->h,trdmrk->w,trdmrk->h,SDL_MapRGB(s->format,224,0,0));
      }

      SDL_Flip(s);
      fpstimer = SDL_GetTicks();
    }
    else
      SDL_Delay(1000 / fps - (SDL_GetTicks() - fpstimer));
  }
  SDL_Quit();
  return 0;
}
