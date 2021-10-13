/* bcm.h
 * Interface to Broadcom VideoCore for Raspberry Pi.
 * Keeping it simple, just a raw framebuffer (no GX).
 */

#ifndef BCM_H
#define BCM_H

struct bcm;

struct bcm *bcm_new();
void bcm_del(struct bcm *bcm);

void bcm_swap(struct bcm *bcm,const void *fb);

#endif
