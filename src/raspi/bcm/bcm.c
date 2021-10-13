#include "bcm.h"
#include <stdio.h>
#include <bcm_host.h>

#define BCM_SIZE_LIMIT 4096 /* for no particular reason */
#define FB_W 96
#define FB_H 64
#define BCM_SCALE_LIMIT 4 /* 2 is too blurry, 4 seems pretty nice */

struct bcm {

  DISPMANX_DISPLAY_HANDLE_T vcdisplay;
  DISPMANX_RESOURCE_HANDLE_T vcresource;
  DISPMANX_ELEMENT_HANDLE_T vcelement;
  DISPMANX_UPDATE_HANDLE_T vcupdate;
          
  int screenw,screenh;
  int scale;
  void *fb; // XRGB8888, FB_W*scale,FB_H*scale
  int vsync_seq;
  uint32_t ctab[256];
};

/* Vsync.
 */

static void _bcm_cb_vsync(DISPMANX_UPDATE_HANDLE_T update,void *arg) {
  struct bcm *bcm=arg;
  bcm->vsync_seq++;
}

/* Generate color table.
 */

static void bcm_generate_ctab(uint32_t *dst) {
  uint8_t src=0;
  while (1) {
    
    uint8_t r=src&0x03; r<<=6; if (r&0x40) r|=0x20; r|=r>>3; r|=r>>6;
    uint8_t g=src&0x1f; g|=g<<3; g|=g>>6;
    uint8_t b=src&0xe0; b|=b>>3; b|=b>>6;

    *dst=(r<<16)|(g<<8)|b;

    if (src==0xff) return;
    src++;
    dst++;
  }
}

/* Calculate an appropriate intermediate framebuffer size, and output bounds.
 * A second framebuffer is necessary to convert the Tiny's pixels to RGB.
 * We also use it for software scaling, otherwise VideoCore interpolates and it's way too blurry.
 */

static int bcm_measure_screen(VC_RECT_T *dstr,struct bcm *bcm) {
  
  // First establish scaling factor. <BCM_SCALE_LIMIT and also within (screenw,screenh).
  int scalex=bcm->screenw/FB_W;
  int scaley=bcm->screenh/FB_H;
  int scale=(scalex<scaley)?scalex:scaley;
  if (scale<1) scale=1;
  else if (scale>BCM_SCALE_LIMIT) scale=BCM_SCALE_LIMIT;

  // Scale to the screen maintaining aspect ratio.
  int wforh=(FB_W*bcm->screenh)/FB_H;
  if (wforh<=bcm->screenw) {
    dstr->width=wforh;
    dstr->height=bcm->screenh;
  } else {
    dstr->width=bcm->screenw;
    dstr->height=(FB_H*bcm->screenw)/FB_W;
  }

  dstr->x=(bcm->screenw>>1)-(dstr->width>>1);
  dstr->y=(bcm->screenh>>1)-(dstr->height>>1);
  return scale;
}

/* New.
 */

struct bcm *bcm_new() {
  struct bcm *bcm=calloc(1,sizeof(struct bcm));
  if (!bcm) return 0;

  bcm_host_init();

  graphics_get_display_size(0,&bcm->screenw,&bcm->screenh);
  if (
    (bcm->screenw<1)||(bcm->screenw>BCM_SIZE_LIMIT)||
    (bcm->screenh<1)||(bcm->screenh>BCM_SIZE_LIMIT)
  ) {
    free(bcm);
    return 0;
  }

  if (!(bcm->vcdisplay=vc_dispmanx_display_open(0))) { free(bcm); return 0; }
  if (!(bcm->vcupdate=vc_dispmanx_update_start(0))) { free(bcm); return 0; }

  VC_RECT_T dstr;
  bcm->scale=bcm_measure_screen(&dstr,bcm);
  VC_RECT_T srcr={0,0,(FB_W*bcm->scale)<<16,(FB_H*bcm->scale)<<16};

  if (!(bcm->fb=malloc(4*FB_W*bcm->scale*FB_H*bcm->scale))) { free(bcm); return 0; }
  if (!(bcm->vcresource=vc_dispmanx_resource_create(
    VC_IMAGE_XRGB8888,FB_W*bcm->scale,FB_H*bcm->scale,bcm->fb
  ))) { free(bcm->fb); free(bcm); return 0; }

  VC_DISPMANX_ALPHA_T alpha={DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS,0xffffffff};
  if (!(bcm->vcelement=vc_dispmanx_element_add(
    bcm->vcupdate,bcm->vcdisplay,1,&dstr,bcm->vcresource,&srcr,DISPMANX_PROTECTION_NONE,&alpha,0,0
  ))) { free(bcm->fb); free(bcm); return 0; }

  if (vc_dispmanx_update_submit_sync(bcm->vcupdate)<0) { free(bcm->fb); free(bcm); return 0; }

  if (vc_dispmanx_vsync_callback(bcm->vcdisplay,_bcm_cb_vsync,bcm)<0) { free(bcm->fb); free(bcm); return 0; }

  bcm_generate_ctab(bcm->ctab);

  return bcm;
}

/* Delete.
 */

void bcm_del(struct bcm *bcm) {
  if (!bcm) return;
  if (bcm->fb) free(bcm->fb);
  bcm_host_deinit();
  free(bcm);
}

/* Swap buffers.
 */

static void bcm_convert(uint32_t *dst,struct bcm *bcm,const uint8_t *src) {
  //TODO If we want to try software scaling, do it here.
  //int i=FB_W*FB_H;
  //for (;i-->0;dst++,src++) *dst=bcm->ctab[*src];

  int dststride=FB_W*bcm->scale; // in words
  int y=FB_H;
  for (;y-->0;src+=FB_W) {
    int yr=bcm->scale;
    for (;yr-->0;dst+=dststride) {
      const uint8_t *srcp=src;
      uint32_t *dstp=dst;
      int x=FB_W;
      for (;x-->0;srcp++) {
        int xr=bcm->scale;
        for (;xr-->0;dstp++) {
          *dstp=bcm->ctab[*srcp];
        }
      }
    }
  }
}

void bcm_swap(struct bcm *bcm,const void *fb) {
  bcm_convert(bcm->fb,bcm,fb);
  VC_RECT_T fbr={0,0,FB_W*bcm->scale,FB_H*bcm->scale};
  vc_dispmanx_resource_write_data(bcm->vcresource,VC_IMAGE_XRGB8888,FB_W*bcm->scale*4,bcm->fb,&fbr);
}
