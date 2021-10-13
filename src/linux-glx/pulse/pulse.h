/* pulse.h
 * Generalized interface to PulseAudio.
 */
 
#ifndef PULSE_H
#define PULSE_H

#include <stdint.h>

struct pulse;

struct pulse *pulse_new(int16_t (*cb)(struct pulse *pulse),void *userdata);
void pulse_del(struct pulse *pulse);

void *pulse_get_userdata(const struct pulse *pulse);

int pulse_lock(struct pulse *pulse);
int pulse_unlock(struct pulse *pulse);

#endif
