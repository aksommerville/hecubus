/* alsa.h
 * Trivial interface to ALSA for audio out.
 */

#ifndef ALSA_H
#define ALSA_H

#include <stdint.h>

struct alsa;

void alsa_del(struct alsa *alsa);
struct alsa *alsa_new(int16_t (*cb)(struct alsa *alsa),void *userdata);

void *alsa_get_userdata(const struct alsa *alsa);

int alsa_lock(struct alsa *alsa);
int alsa_unlock(struct alsa *alsa);

#endif
