#include "alsa.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <alsa/asoundlib.h>

#define ALSA_BUFFER_SIZE 2048

/* Instance
 */

struct alsa {
  int rate,chanc;
  int16_t (*cb)(struct alsa *alsa);
  void *userdata;

  snd_pcm_t *alsa;
  snd_pcm_hw_params_t *hwparams;

  int hwbuffersize;
  int bufc; // frames
  int bufc_samples;
  int16_t *buf;

  pthread_t iothd;
  pthread_mutex_t iomtx;
  int ioabort;
  int cberror;
};

/* Cleanup.
 */

void alsa_del(struct alsa *alsa) {
  if (!alsa) return;
  alsa->ioabort=1;
  if (alsa->iothd&&!alsa->cberror) {
    pthread_cancel(alsa->iothd);
    pthread_join(alsa->iothd,0);
  }
  pthread_mutex_destroy(&alsa->iomtx);
  if (alsa->hwparams) snd_pcm_hw_params_free(alsa->hwparams);
  if (alsa->alsa) snd_pcm_close(alsa->alsa);
  if (alsa->buf) free(alsa->buf);
  free(alsa);
}

/* I/O thread.
 */

static void *alsa_iothd(void *dummy) {
  struct alsa *alsa=dummy;
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,0);//TODO copied from plundersquad -- is this correct?
  while (1) {
    pthread_testcancel();

    if (pthread_mutex_lock(&alsa->iomtx)) {
      alsa->cberror=1;
      return 0;
    }
    int16_t *dst=alsa->buf;
    int i=alsa->bufc_samples;
    for (;i-->0;dst++) *dst=alsa->cb(alsa);
    pthread_mutex_unlock(&alsa->iomtx);

    int16_t *samplev=alsa->buf;
    int samplep=0,samplec=alsa->bufc;
    while (samplep<samplec) {
      pthread_testcancel();
      int err=snd_pcm_writei(alsa->alsa,samplev+samplep,samplec-samplep);
      if (alsa->ioabort) return 0;
      if (err<=0) {
        if ((err=snd_pcm_recover(alsa->alsa,err,0))<0) {
          alsa->cberror=1;
          return 0;
        }
        break;
      }
      samplep+=err;
    }
  }
  return 0;
}

/* Init.
 */

static int _alsa_init(struct alsa *alsa) {
  
  if (
    (snd_pcm_open(&alsa->alsa,"default",SND_PCM_STREAM_PLAYBACK,0)<0)||
    (snd_pcm_hw_params_malloc(&alsa->hwparams)<0)||
    (snd_pcm_hw_params_any(alsa->alsa,alsa->hwparams)<0)||
    (snd_pcm_hw_params_set_access(alsa->alsa,alsa->hwparams,SND_PCM_ACCESS_RW_INTERLEAVED)<0)||
    (snd_pcm_hw_params_set_format(alsa->alsa,alsa->hwparams,SND_PCM_FORMAT_S16)<0)||
    (snd_pcm_hw_params_set_rate_near(alsa->alsa,alsa->hwparams,&alsa->rate,0)<0)||
    (snd_pcm_hw_params_set_channels(alsa->alsa,alsa->hwparams,alsa->chanc)<0)||
    (snd_pcm_hw_params_set_buffer_size(alsa->alsa,alsa->hwparams,ALSA_BUFFER_SIZE)<0)||
    (snd_pcm_hw_params(alsa->alsa,alsa->hwparams)<0)
  ) return -1;

  if (snd_pcm_nonblock(alsa->alsa,0)<0) return -1;
  if (snd_pcm_prepare(alsa->alsa)<0) return -1;

  alsa->bufc=ALSA_BUFFER_SIZE;
  alsa->bufc_samples=alsa->bufc*alsa->chanc;
  if (!(alsa->buf=malloc(alsa->bufc_samples*sizeof(int16_t)))) return -1;

  pthread_mutexattr_t mattr;
  pthread_mutexattr_init(&mattr);
  pthread_mutexattr_settype(&mattr,PTHREAD_MUTEX_RECURSIVE);
  if (pthread_mutex_init(&alsa->iomtx,&mattr)) return -1;
  pthread_mutexattr_destroy(&mattr);
  if (pthread_create(&alsa->iothd,0,alsa_iothd,alsa)) return -1;

  return 0;
}

struct alsa *alsa_new(int16_t (*cb)(struct alsa *alsa),void *userdata) {
  struct alsa *alsa=calloc(1,sizeof(struct alsa));
  if (!alsa) return 0;

  alsa->cb=cb;
  alsa->userdata=userdata;
  alsa->rate=22050;
  alsa->chanc=1;

  if (_alsa_init(alsa)<0) {
    alsa_del(alsa);
    return 0;
  }
  return alsa;
}

/* Locks and maintenance.
 */

void *alsa_get_userdata(const struct alsa *alsa) {
  return alsa->userdata;
}

int alsa_update(struct alsa *alsa) {
  if (alsa->cberror) return -1;
  return 0;
}

int alsa_lock(struct alsa *alsa) {
  if (pthread_mutex_lock(&alsa->iomtx)) return -1;
  return 0;
}

int alsa_unlock(struct alsa *alsa) {
  pthread_mutex_unlock(&alsa->iomtx);
  return 0;
}
