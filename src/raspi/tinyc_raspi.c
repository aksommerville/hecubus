#include "tinyc.h"
#include "raspi/evdev/evdev.h"
#include "raspi/os/poller.h"
#include "raspi/alsa/alsa.h"
#include "raspi/bcm/bcm.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

/* Globals.
 */
 
static struct poller *poller=0;
static struct evdev *evdev=0;
static uint8_t input=0;
static int quit_requested=0;
static struct alsa *alsa=0;
static struct bcm *bcm=0;

/* Cleanup and report status -- not part of tinyc, specific to linux.
 */
 
void tinyc_quit() {
  alsa_del(alsa);
  alsa=0;
  
  evdev_del(evdev);
  evdev=0;
  
  poller_del(poller);
  poller=0;
  
  bcm_del(bcm);
  bcm=0;
}

/* evdev callbacks.
 */
 
static int tinyc_cb_evdev_connect(struct evdev *evdev,struct evdev_device *device) {
  fprintf(stderr,"%s %p\n",__func__,device);
  return 0;
}
 
static int tinyc_cb_evdev_disconnect(struct evdev *evdev,struct evdev_device *device) {
  fprintf(stderr,"%s %p\n",__func__,device);
  // Don't overthink it... Drop all input when any device disconnects, whatever.
  input=0;
  return 0;
}
 
static int tinyc_cb_evdev_event(struct evdev *evdev,struct evdev_device *device,int btnid,int value) {
  //fprintf(stderr,"%s %p.%d=%d\n",__func__,device,btnid,value);
  // Hard-coding config for my Xbox 360 joystick. TODO generalize.
  switch (btnid) {
  
    // dpad
    case 0x30010: if (value<0) {
        input=(input&~(TINYC_BUTTON_LEFT|TINYC_BUTTON_RIGHT))|TINYC_BUTTON_LEFT;
      } else if (value>0) {
        input=(input&~(TINYC_BUTTON_LEFT|TINYC_BUTTON_RIGHT))|TINYC_BUTTON_RIGHT;
      } else {
        input=(input&~(TINYC_BUTTON_LEFT|TINYC_BUTTON_RIGHT));
      } break;
    case 0x30011: if (value<0) {
        input=(input&~(TINYC_BUTTON_UP|TINYC_BUTTON_DOWN))|TINYC_BUTTON_UP;
      } else if (value>0) {
        input=(input&~(TINYC_BUTTON_UP|TINYC_BUTTON_DOWN))|TINYC_BUTTON_DOWN;
      } else {
        input=(input&~(TINYC_BUTTON_UP|TINYC_BUTTON_DOWN));
      } break;
      
    // buttons
    case 0x10130: if (value) input|=TINYC_BUTTON_A; else input&=~TINYC_BUTTON_A; break;
    case 0x10133: if (value) input|=TINYC_BUTTON_B; else input&=~TINYC_BUTTON_B; break;
      
  }
  return 0;
}

/* Init.
 */
 
void tinyc_init() {
  
  if (!(poller=poller_new())) {
    fprintf(stderr,"Failed to create poller\n");
    return;
  }
  
  struct evdev_delegate delegate={
    .cb_connect=tinyc_cb_evdev_connect,
    .cb_disconnect=tinyc_cb_evdev_disconnect,
    .cb_event=tinyc_cb_evdev_event,
  };
  if (!(evdev=evdev_new(0,poller,&delegate))) {
    fprintf(stderr,"Failed to initialize evdev. Proceeding anyway.\n");
  }
  
  if (!(bcm=bcm_new())) {
    fprintf(stderr,"Failed to initialize Broadcom video.\n");
    return;
  }
  
  if (!(alsa=alsa_new((void*)tinyc_client_update_synthesizer,0))) {
    fprintf(stderr,"Failed to initialize ALSA. Proceeding anyway.\n");
  }
}

/* Send 96x64 framebuffer.
 */

void tinyc_send_framebuffer(const void *fb) {
  if (bcm) {
    bcm_swap(bcm,fb);
  }
}

/* Collect input.
 */

uint8_t tinyc_read_input() {
  if (poller) {
    poller_update(poller,0);
  }
  return input;
}

// linux only
int tinyc_quit_requested() {
  if (quit_requested) {
    quit_requested=0;
    return 1;
  }
  return 0;
}

/* Files.
 */
 
// Fails if it doesn't fit.
static int tinyc_mangle_path(char *dst,int dsta,const char *src) {
  const char *HOME=getenv("HOME");
  if (!HOME) HOME=".";
  int dstc=snprintf(dst,dsta,"%s/%s",HOME,src);
  if ((dstc<1)||(dstc>=dsta)) return -1;
  return dstc;
}
 
int32_t tinyc_file_read(void *dst,int32_t dsta,const char *path) {
  char hpath[1024];
  if (tinyc_mangle_path(hpath,sizeof(hpath),path)<0) return -1;
  int fd=open(hpath,O_RDONLY);
  if (fd<0) return -1;
  int err=read(fd,dst,dsta);
  close(fd);
  return err;
}

int8_t tinyc_file_write(const char *path,const void *src,int32_t srcc) {
  char hpath[1024];
  if (tinyc_mangle_path(hpath,sizeof(hpath),path)<0) return -1;
  int fd=open(hpath,O_WRONLY|O_CREAT|O_TRUNC,0666);
  if (fd<0) return -1;
  if (write(fd,src,srcc)!=srcc) {
    close(fd);
    unlink(hpath);
    return -1;
  }
  close(fd);
  return 0;
}
