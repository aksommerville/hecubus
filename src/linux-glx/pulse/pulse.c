#include "pulse.h"
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>
#include <endian.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

/* Instance definition.
 */
 
struct pulse {
  int rate,chanc;
  int16_t (*cb)(struct pulse *pulse);
  void *userdata;
  
  pa_simple *pa;
  
  pthread_t iothd;
  pthread_mutex_t iomtx;
  int ioerror;
  int iocancel; // pa_simple doesn't like regular thread cancellation
  
  int16_t *buf;
  int bufa;
};

/* I/O thread.
 */
 
static void *_pulse_iothd(void *arg) {
  struct pulse *pulse=arg;
  while (1) {
    if (pulse->iocancel) return 0;
    
    // Fill buffer while holding the lock.
    if (pthread_mutex_lock(&pulse->iomtx)) {
      pulse->ioerror=-1;
      return 0;
    }
    int i=pulse->bufa;
    int16_t *v=pulse->buf;
    for (;i-->0;v++) *v=pulse->cb(pulse);
    if (pthread_mutex_unlock(&pulse->iomtx)) {
      pulse->ioerror=-1;
      return 0;
    }
    if (pulse->iocancel) return 0;
    
    // Deliver to PulseAudio.
    int err=0,result;
    result=pa_simple_write(pulse->pa,pulse->buf,sizeof(int16_t)*pulse->bufa,&err);
    if (pulse->iocancel) return 0;
    if (result<0) return 0;
  }
}

/* Terminate I/O thread.
 */
 
static void _pulse_abort_io(struct pulse *pulse) {
  if (!pulse->iothd) return;
  pulse->iocancel=1;
  pthread_join(pulse->iothd,0);
  pulse->iothd=0;
}

/* Cleanup.
 */
 
static void _pulse_del(struct pulse *pulse) {
  _pulse_abort_io(pulse);
  pthread_mutex_destroy(&pulse->iomtx);
  if (pulse->pa) {
    pa_simple_free(pulse->pa);
  }
}

/* Init PulseAudio client.
 */
 
static int _pulse_init_pa(struct pulse *pulse) {
  int err;

  pa_sample_spec sample_spec={
    #if BYTE_ORDER==BIG_ENDIAN
      .format=PA_SAMPLE_S16BE, // SAMPLETYPE
    #else
      .format=PA_SAMPLE_S16LE, // SAMPLETYPE
    #endif
    .rate=pulse->rate,
    .channels=pulse->chanc,
  };
  int bufframec=pulse->rate/20; //TODO more sophisticated buffer length decision
  if (bufframec<20) bufframec=20;
  pa_buffer_attr buffer_attr={
    .maxlength=pulse->chanc*sizeof(int16_t)*bufframec,
    .tlength=pulse->chanc*sizeof(int16_t)*bufframec,
    .prebuf=0xffffffff,
    .minreq=0xffffffff,
  };
  
  if (!(pulse->pa=pa_simple_new(
    0, // server name
    APPNAME, // our name
    PA_STREAM_PLAYBACK,
    0, // sink name (?)
    APPNAME, // stream (as opposed to client) name
    &sample_spec,
    0, // channel map
    &buffer_attr,
    &err
  ))) return -1;
  
  if ((sample_spec.rate!=pulse->rate)||(sample_spec.channels!=pulse->chanc)) {
    // We can't handle unexpected rate or chanc. TODO Room for improvement here...
    return -1;
  }
  
  return 0;
}

/* With the final rate and channel count settled, calculate a good buffer size and allocate it.
 */
 
static int _pulse_init_buffer(struct pulse *pulse) {

  const double buflen_target_s= 0.010; // about 100 Hz
  const int buflen_min=           128; // but in no case smaller than N samples
  const int buflen_max=         16384; // ...nor larger
  
  // Initial guess and clamp to the hard boundaries.
  pulse->bufa=buflen_target_s*pulse->rate*pulse->chanc;
  if (pulse->bufa<buflen_min) {
    pulse->bufa=buflen_min;
  } else if (pulse->bufa>buflen_max) {
    pulse->bufa=buflen_max;
  }
  // Reduce to next multiple of channel count.
  pulse->bufa-=pulse->bufa%pulse->chanc;
  
  if (!(pulse->buf=malloc(sizeof(int16_t)*pulse->bufa))) {
    return -1;
  }
  
  return 0;
}

/* Init thread.
 */
 
static int _pulse_init_thread(struct pulse *pulse) {
  if (pthread_mutex_init(&pulse->iomtx,0)) return -1;
  if (pthread_create(&pulse->iothd,0,_pulse_iothd,pulse)) return -1;
  return 0;
}

/* Init.
 */
  
static int _pulse_init(struct pulse *pulse) {

  // These are currently hard-coded in tinyc.c. Ought to kick out to the app layer i guess.
  pulse->rate=22050;
  pulse->chanc=1;

  if (_pulse_init_pa(pulse)<0) return -1;
  if (_pulse_init_buffer(pulse)<0) return -1;
  if (_pulse_init_thread(pulse)<0) return -1;
  
  return 0;
}

/* Public interface.
 */

struct pulse *pulse_new(int16_t (*cb)(struct pulse *pulse),void *userdata) {
  struct pulse *pulse=calloc(1,sizeof(struct pulse));
  if (!pulse) return 0;
  
  pulse->cb=cb;
  pulse->userdata=userdata;
  
  if (_pulse_init(pulse)<0) {
    _pulse_del(pulse);
    free(pulse);
    return 0;
  }
  
  return pulse;
}

void pulse_del(struct pulse *pulse) {
  if (!pulse) return;
  _pulse_del(pulse);
  free(pulse);
}

void *pulse_get_userdata(const struct pulse *pulse) {
  return pulse->userdata;
}
 
int pulse_lock(struct pulse *pulse) {
  if (pthread_mutex_lock(&pulse->iomtx)) return -1;
  return 0;
}

int pulse_unlock(struct pulse *pulse) {
  if (pthread_mutex_unlock(&pulse->iomtx)) return -1;
  return 0;
}
