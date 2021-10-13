#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tinyc.h"
#include "softarcade.h"

/* Audio.
 *************************************************************/

#if SOFTARCADE_AUDIO_ENABLE
//XXX Sorry for being so mysterious... Softarcade's synthesizer is not in play here.
// We're doing a much simpler audio regime, see a bit below.

struct softarcade_synth synth={0};

int16_t tinyc_client_update_synthesizer() {
  return softarcade_synth_update(&synth);
}

static const int16_t *get_wave(uint8_t waveid,void *userdata) {
  return 0;
}

static uint16_t get_pcm(void *pcmpp,uint8_t waveid,void *userdata) {
  return 0;
}

#else

static uint16_t auphase=0;
static uint16_t auhalfperiod=30;
static uint16_t autonerate=10000;
static uint16_t autonetime=10000;
static int16_t aulevel=0;
static uint16_t auramprate=40;
static uint16_t auramptime=40;

int16_t tinyc_client_update_synthesizer() {

  if (++auphase>=auhalfperiod) {
    auphase=0;
    aulevel=-aulevel;
  }
  
  if (auramptime) auramptime--;
  else {
    auramptime=auramprate;
    if (aulevel>0) aulevel--;
    else if (aulevel<0) aulevel++;
  }
  
  if (autonetime) autonetime--;
  else {
    autonetime=autonerate;
    if (auhalfperiod==40) auhalfperiod=30;
    else auhalfperiod=40;
  }
  
  return aulevel;
}

static void beep() {
  aulevel=3000;
  auhalfperiod=30;
  auphase=0;
  autonetime=autonerate;
  auramptime=auramprate;
}

#endif

/* Video.
 ***********************************************************************/
 
SOFTARCADE_IMAGE_DECLARE(fb,96,64,0)

SOFTARCADE_IMAGE_DECLARE(tiles,32,32,
  28,255,255,255,255,255,255,28,28,232,232,232,232,232,232,28,28,28,28,5,5,28,28,28,28,28,255,255,255,255,28,28,
  255,255,255,255,255,255,255,255,28,28,255,255,255,255,28,28,28,5,28,5,5,28,28,28,28,28,255,255,255,255,28,28,
  255,255,0,255,255,0,255,255,28,28,255,255,255,255,28,28,28,5,255,255,255,255,28,28,255,255,146,146,146,146,255,255,
  255,255,255,255,255,255,255,255,28,28,28,255,255,28,28,28,28,255,255,0,255,255,255,28,255,255,146,255,255,146,255,255,
  255,255,255,255,255,255,255,255,28,28,28,255,5,28,28,28,28,255,255,0,0,255,255,28,255,255,146,255,255,146,255,255,
  255,255,0,255,255,0,255,255,28,28,255,5,255,5,28,28,28,255,255,0,255,255,255,28,255,255,146,146,146,146,255,255,
  255,255,255,255,255,255,255,255,28,28,5,255,5,255,28,28,28,255,255,255,255,255,255,28,28,28,255,255,255,255,28,28,
  28,255,255,255,255,255,255,28,28,232,232,232,232,232,232,28,28,28,255,255,255,255,28,28,28,28,255,255,255,255,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
  28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,
)

static struct softarcade_image tilev[16]={0};
static uint8_t tilew=8,tileh=8;

SOFTARCADE_IMAGE_DECLARE(fancydigits,80,13,
  28,28,255,255,255,28,28,28,28,28,28,28,255,28,28,28,28,255,255,255,255,28,28,28,28,255,255,255,255,28,28,28,
  255,255,28,28,28,255,255,28,255,255,255,255,255,255,255,28,28,28,255,255,255,255,28,28,255,255,255,255,255,255,
  255,28,28,28,255,255,255,28,28,28,28,28,255,255,255,255,28,28,
  28,255,28,28,28,255,28,28,28,28,28,255,255,28,28,28,255,255,28,28,255,255,28,28,255,255,28,28,255,255,28,28,
  255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,28,255,255,28,28,255,255,28,255,255,28,28,28,255,255,28,
  28,255,255,28,255,255,28,28,28,255,255,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,255,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,255,28,28,28,28,28,28,255,255,28,255,
  255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,255,255,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,255,
  255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,255,
  255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,255,255,255,28,28,28,28,255,255,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,
  255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,
  255,255,28,255,255,28,28,28,255,255,28,28,255,255,28,
  255,255,28,255,28,255,255,28,28,28,28,255,255,28,28,28,28,28,255,255,255,255,28,28,28,28,255,255,255,255,28,
  28,255,255,255,255,255,255,255,28,255,255,255,255,255,255,28,28,255,255,255,255,255,255,28,28,28,28,28,28,255,
  255,28,28,28,28,255,255,255,28,28,28,28,28,255,255,255,255,255,28,
  255,255,255,28,28,255,255,28,28,28,28,255,255,28,28,28,28,255,255,255,255,28,28,28,28,28,28,28,255,255,28,28,28,28,
  28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,255,255,255,28,28,28,28,255,255,28,28,28,28,255,255,28,
  255,255,28,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,
  28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,
  255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,
  28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,
  255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,
  28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,
  255,255,28,28,28,28,28,28,255,255,28,
  28,255,28,28,28,255,28,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,255,255,28,28,28,28,28,
  28,28,255,255,28,255,255,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,255,255,28,28,28,28,255,255,28,255,
  255,28,28,28,28,28,28,28,255,255,28,
  28,28,255,255,255,28,28,28,28,255,255,255,255,255,255,28,255,255,255,255,255,255,255,28,28,255,255,255,255,28,28,28,
  28,28,28,28,28,255,255,28,28,255,255,255,255,28,28,28,28,28,255,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,255,
  255,255,28,28,28,28,28,28,28,28,255,255,28,
)
static struct softarcade_image fancydigitv[10]={0};

static struct softarcade_font font={
  .metrics={
    96,53,105,181,124,181,184,41,85,85,181,181,83,167,47,181,117,117,117,117,117,117,117,117,117,117,
    46,83,117,142,117,153,149,149,149,117,149,149,149,149,149,117,149,149,117,181,181,181,149,149,149,
    149,181,149,181,181,181,181,117,85,181,85,173,175,108,149,149,114,149,149,117,150,149,50,89,117,85,
    175,143,146,150,150,146,149,117,143,175,175,111,150,114,117,53,117,174,0
  },
  .bits={
    0x0,0xe8000000,0xb4000000,0x57d5f500,0x5e73d000,0x88888880,0x45115934,0xc0000000,0x6a400000,0x95800000,
    0xabbeea80,0x213e4200,0xd000000,0xf8000000,0x20000000,0x8888800,0x56d40000,0xc92e0000,0xc54e0000,0xc51c0000,
    0xb7920000,0xf31c0000,0x53540000,0xe5240000,0x55540000,0x55920000,0xa0000000,0x4d000000,0x2a220000,
    0xf0f00000,0x88a80000,0xe1640400,0x69b87000,0x69f99000,0xe9e9e000,0x72460000,0xe999e000,0xf8e8f000,
    0xf8e88000,0x68b97000,0x99f99000,0xe92e0000,0x11196000,0x9aca9000,0x924e0000,0x8eeb1880,0x8e6b3880,
    0x74631700,0xe9e88000,0x699b7000,0xe9e99000,0x7861e000,0xf9084200,0x99996000,0x8c54a200,0x8c6b5500,
    0x8a88a880,0x8c5c4200,0xe54e0000,0xeac00000,0x82082080,0xd5c00000,0x22a20000,0x3e0000,0x88800000,
    0x61797000,0x88e9e000,0x72300000,0x11797000,0x69e86000,0x2ba40000,0x69716000,0x88e99000,0xb0000000,
    0x45600000,0x92ea0000,0xaa400000,0xd56a0000,0xe9900000,0x69960000,0x69e88000,0x69711000,0xe9880000,
    0x7861e000,0x4ba20000,0x99700000,0x8a880000,0x8d540000,0xaa800000,0x99716000,0xef700000,0x6a260000,
    0xf8000000,0xc8ac0000,0x45440000,0x0
  },
};


/* Globals.
 **************************************************************/
 
static uint8_t video_dirty=1;
static uint8_t mode_select=0; // 0..3 = dice,timer,stopwatch,stats
static uint8_t focus_mode_select=1; // nonzero means cursor in header bar
static uint8_t seeded=0; // defer srand() until the first input

/* Dice mode.
 ***************************************************************/
 
#define DICE_ROWC 6
#define DICE_COUNT_LIMIT 9
#define DICE_INDIVIDUAL_REPORT_LIMIT 6 /* <=n we report each die, >n only min/max/sum */
#define DICE_BLACKOUT_TIME 10

static uint8_t dice_selected_row=0;
static uint8_t dice_blackout=0; // clear screen and count down after rolling, as a visual cue
 
static struct dice_row {
  uint8_t size;
  uint8_t count;
} dice_rowv[DICE_ROWC]={
  { 4},
  { 6,2}, // might as well start with a most typical config
  { 8},
  {10},
  {12},
  {20},
};

// Last roll's outcome:
static uint8_t dice_individualv[DICE_INDIVIDUAL_REPORT_LIMIT];
static uint8_t dice_individualc=0;
static uint8_t dice_min=1,dice_max=0;
static uint16_t dice_sum=0;

// Roll so many dice of a fixed size and reduce outcome into globals.
static void dice_roll_1(uint8_t size,uint8_t count) {
  while (count-->0) {
    uint8_t v=rand()%size+1;
    if (v<dice_min) dice_min=v;
    if (v>dice_max) dice_max=v; // can be both, eg a single die is always min and max
    dice_sum+=v;
    if (dice_individualc<DICE_INDIVIDUAL_REPORT_LIMIT) {
      dice_individualv[dice_individualc]=v;
    }
    dice_individualc++;
  }
}

static void dice_roll() {
  
  // Clear everything, even if request invalid.
  dice_individualc=0;
  dice_min=0xff;
  dice_max=0;
  dice_sum=0;
  
  // Roll!
  struct dice_row *row=dice_rowv;
  int8_t i=DICE_ROWC;
  for (;i-->0;row++) {
    if (row->count) dice_roll_1(row->size,row->count);
  }
  
  // Just one die? min/max is not helpful.
  if (dice_individualc<2) dice_min=0xff;
  
  // Too many dice, nix the individuals.
  if (dice_individualc>DICE_INDIVIDUAL_REPORT_LIMIT) dice_individualc=0;
  
  // No sense report individuals if fewer than three -- sum or min/max describes them.
  if (dice_individualc<3) dice_individualc=0;
  
  dice_blackout=DICE_BLACKOUT_TIME;
  video_dirty=1;
}

static void dice_adjust_count(int8_t d) {
  struct dice_row *row=dice_rowv+dice_selected_row;
  int8_t nc=row->count+d;
  if (nc<0) nc=0;
  else if (nc>DICE_COUNT_LIMIT) nc=DICE_COUNT_LIMIT;
  if (row->count==nc) return;
  row->count=nc;
  video_dirty=1;
}

static void handle_input_dice(uint8_t pressed,uint8_t aux) {

  // A or B means "roll", regardless of (aux).
  if (pressed&(TINYC_BUTTON_A|TINYC_BUTTON_B)) {
    dice_roll();
    return;
  }
  
  // Up or down in (aux) turns off (focus_mode_select) and highlights the bottom or top row respectively.
  // Anything else in (aux) is ignored.
  if (aux) {
    if (pressed&TINYC_BUTTON_UP) {
      focus_mode_select=0;
      dice_selected_row=DICE_ROWC-1;
      video_dirty=1;
    } else if (pressed&TINYC_BUTTON_DOWN) {
      focus_mode_select=0;
      dice_selected_row=0;
      video_dirty=1;
    }
    return;
  }
  
  // Up and down change the selection, with the mode select inserted above row zero.
  if (pressed&TINYC_BUTTON_UP) {
    if (dice_selected_row==0) {
      focus_mode_select=1;
    } else {
      dice_selected_row--;
    }
    video_dirty=1;
  } else if (pressed&TINYC_BUTTON_DOWN) {
    if (dice_selected_row==DICE_ROWC-1) {
      focus_mode_select=1;
    } else {
      dice_selected_row++;
    }
    video_dirty=1;
    
  // Left and right change the count on the selected row.
  } else if (pressed&TINYC_BUTTON_LEFT) {
    dice_adjust_count(-1);
  } else if (pressed&TINYC_BUTTON_RIGHT) {
    dice_adjust_count(1);
  }
}

static void dice_update() {
  if (dice_blackout>0) {
    dice_blackout--;
    if (!dice_blackout) video_dirty=1;
  }
}

static void dice_draw_digit(int8_t x,int8_t y,int8_t digit) {
  if ((digit<0)||(digit>9)) return;
  softarcade_blit_unchecked(&image_fb,x,y,fancydigitv+digit);
}

static void draw_dice() {

  // Interactive list.
  int8_t y=13;
  struct dice_row *row=dice_rowv;
  int8_t i=0;
  for (;i<DICE_ROWC;i++,row++,y+=8) {
    char label[32];
    int8_t labelc=snprintf(label,sizeof(label),"%dd%d",row->count,row->size);
    uint8_t color=(!focus_mode_select&&(i==dice_selected_row))?0x1f:0xff;
    softarcade_font_render(&image_fb,3,y,&font,label,labelc,color);
  }
  
  if (!dice_blackout) {
    int8_t y=17;
  
    // Sum if we have it.
    // With the count-limit of 9, this can't reach 1000, but allow for 4 or 5 digits anyway.
    if (dice_sum>0) {
      int8_t x=30;
      if (dice_sum>=10000) { dice_draw_digit(x,y, dice_sum/10000      ); x+=fancydigitv[0].w; }
      if (dice_sum>= 1000) { dice_draw_digit(x,y,(dice_sum%10000)/1000); x+=fancydigitv[0].w; }
      if (dice_sum>=  100) { dice_draw_digit(x,y,(dice_sum% 1000)/100 ); x+=fancydigitv[0].w; }
      if (dice_sum>=   10) { dice_draw_digit(x,y,(dice_sum%  100)/10  ); x+=fancydigitv[0].w; }
                             dice_draw_digit(x,y, dice_sum%   10      );
      y+=fancydigitv[0].h+5;
    }
  
    // Range if we have it.
    if (dice_min<=dice_max) {
      char msg[32];
      int8_t msgc=snprintf(msg,sizeof(msg),"%d..%d",dice_min,dice_max);
      softarcade_font_render(&image_fb,30,y,&font,msg,msgc,0xff);
      y+=8;
    }
  
    // Individual counts if we have them.
    if (dice_individualc>0) {
      char msg[32]; // take care to keep overflow impossible: 4*DICE_INDIVIDUAL_REPORT_LIMIT<n
      int8_t msgc=0;
      for (i=0;i<dice_individualc;i++) {
        if (msgc) msg[msgc++]=',';
        msgc+=snprintf(msg+msgc,sizeof(msg)-msgc,"%d",dice_individualv[i]);
      }
      softarcade_font_render(&image_fb,30,y,&font,msg,msgc,0xff);
      y+=8;
    }
  }
}

/* Countdown mode.
 * TODO The timer stops updating when you move to another page.
 * Can we keep it alive in the background? Seems doable?
 ***************************************************************/
 
#define CD_ALERT_TIME 180 /* 1/60 s */
#define CD_ALERT_PERIOD 10 /* frames; half-period to be exact */
 
static uint32_t cd_end_time=0; // nonzero = absolute expiry time in ms
static uint16_t cd_alert_time=0; // frames
static struct { uint8_t min,sec; } cd_remaining={0};
static uint8_t cd_editv[4]={0}; // min*10,min,sec*10,sec
static uint8_t cd_editp=0; // 0..3

static void countdown_start() {
  uint32_t ms=(cd_editv[0]*600+cd_editv[1]*60+cd_editv[2]*10+cd_editv[3])*1000;
  if (ms<1000) return;
  cd_end_time=millis()+ms;
  cd_alert_time=0;
  cd_remaining.min=ms/60000;
  cd_remaining.sec=(ms/1000)%60;
  video_dirty=1;
}

static void countdown_cancel() {
  cd_end_time=0;
  cd_alert_time=0;
  video_dirty=1;
}

static void handle_input_countdown(uint8_t pressed,uint8_t aux) {

  // B returns to the menu.
  if (pressed&TINYC_BUTTON_B) {
    focus_mode_select=1;
    video_dirty=1;
    return;
  }
  
  // Up/Down while in the menu: unfocus menu.
  if (aux&&(pressed&(TINYC_BUTTON_UP|TINYC_BUTTON_DOWN))) {
    focus_mode_select=0;
    video_dirty=1;
    return;
  }

  // A cancels the timer if running, or starts if stopped.
  if (pressed&TINYC_BUTTON_A) {
    if (cd_end_time) countdown_cancel();
    else countdown_start();
    return;
  }
  
  if (aux) return;

  // Left/right: cd_editp
  if (pressed&TINYC_BUTTON_LEFT) {
    if (cd_editp) cd_editp--;
    else cd_editp=3;
    video_dirty=1;
  } else if (pressed&TINYC_BUTTON_RIGHT) {
    if (cd_editp<3) cd_editp++;
    else cd_editp=0;
    video_dirty=1;
  }
  
  // Up/down: edit focused digit, if not running
  if (!cd_end_time&&!cd_alert_time) {
    uint8_t *dst=cd_editv+cd_editp;
    if (pressed&TINYC_BUTTON_UP) {
      if (*dst>=9) *dst=0;
      else (*dst)++;
      if ((cd_editp==2)&&(*dst>5)) *dst=0;
      video_dirty=1;
    } else if (pressed&TINYC_BUTTON_DOWN) {
      if (!*dst) *dst=9;
      else (*dst)--;
      if ((cd_editp==2)&&(*dst>5)) *dst=5;
      video_dirty=1;
    }
  }
}

static void countdown_expire() {
  beep();
  cd_end_time=0;
  cd_alert_time=CD_ALERT_TIME;
  video_dirty=1;
}

static void countdown_end_alert() {
  cd_alert_time=0;
  video_dirty=1;
}

static void countdown_update() {
  if (cd_end_time) {
    uint32_t now=millis();
    if (now>=cd_end_time) {
      countdown_expire();
    } else {
      // Should tick about 1 in 60 frames, so it's worth checking whether something changed.
      uint32_t remaining=cd_end_time-now;
      uint8_t min=remaining/60000;
      uint8_t sec=(remaining/1000)%60;
      if ((min!=cd_remaining.min)||(sec!=cd_remaining.sec)) {
        cd_remaining.min=min;
        cd_remaining.sec=sec;
        video_dirty=1;
      }
    }
  } else if (cd_alert_time>0) {
    cd_alert_time--;
    if (!cd_alert_time) countdown_end_alert();
    else if (cd_alert_time%CD_ALERT_PERIOD==CD_ALERT_PERIOD-1) video_dirty=1;
  }
}

static void draw_countdown() {
  
  // Timer running...
  if (cd_end_time) {
    softarcade_fill_rect(&image_fb,0,11,96,53,0x20);
    uint8_t colw=fancydigitv[0].w;
    int8_t x=30,y=27;
    dice_draw_digit(x,y,cd_remaining.min/10); x+=colw;
    dice_draw_digit(x,y,cd_remaining.min%10); x+=colw;
    softarcade_fill_rect(&image_fb,x,y+4,2,2,0xff);
    softarcade_fill_rect(&image_fb,x,y+8,2,2,0xff);
    x+=3;
    dice_draw_digit(x,y,cd_remaining.sec/10); x+=colw;
    dice_draw_digit(x,y,cd_remaining.sec%10);
    
  // Flash due to expiry.
  } else if (cd_alert_time) {
    uint8_t bgcolor,fgcolor;
    if ((cd_alert_time/CD_ALERT_PERIOD)&1) {
      bgcolor=0x03;
      fgcolor=0xff;
    } else {
      bgcolor=0x1f;
      fgcolor=0x00;
    }
    softarcade_fill_rect(&image_fb,0,11,96,53,bgcolor);
    softarcade_font_render(&image_fb,20,30,&font,"Time up!",8,fgcolor);
    
  // Collect input.
  } else {
    uint8_t colw=fancydigitv[0].w;
    uint8_t rowh=fancydigitv[0].h;
    int8_t x=30,y=27;
    #define DIGIT(n) { \
      if (!focus_mode_select&&(cd_editp==n)) softarcade_fill_rect(&image_fb,x-1,y-1,colw+1,rowh+2,0x70); \
      dice_draw_digit(x,y,cd_editv[n]); x+=colw; \
    }
    DIGIT(0)
    DIGIT(1)
    softarcade_fill_rect(&image_fb,x,y+4,2,2,0xff);
    softarcade_fill_rect(&image_fb,x,y+8,2,2,0xff);
    x+=3;
    DIGIT(2)
    DIGIT(3)
    #undef DIGIT
  }
}

/* Stopwatch mode.
 ****************************************************************/
 
#define SW_STAMP_LIMIT 4
 
static uint32_t sw_starttime=0; // absolute ms, or zero if not running
static uint16_t sw_elapsed=0; // BCD: 0xMMSS, for display only
static uint32_t sw_stampv[SW_STAMP_LIMIT]; // BCD: 0x0MMSSmmm
static uint8_t sw_stampc=0;

static void sw_begin() {
  sw_starttime=millis();
  sw_stampc=0;
  video_dirty=1;
}

static void sw_end() {
  sw_starttime=0;
  video_dirty=1;
}

static void sw_record_stamp() {
  uint32_t ms=millis()-sw_starttime;
  uint32_t stamp=
    ((ms/600000)<<24)|
    (((ms%600000)/60000)<<20)|
    (((ms%60000)/10000)<<16)|
    (((ms%10000)/1000)<<12)|
    (((ms%1000)/100)<<8)|
    (((ms%100)/10)<<4)|
    (ms%10)|
  0;
  if (sw_stampc>=SW_STAMP_LIMIT) {
    sw_stampc--;
    uint8_t i=0;
    for (;i<sw_stampc;i++) sw_stampv[i]=sw_stampv[i+1];
  }
  sw_stampv[sw_stampc++]=stamp;
  video_dirty=1;
}

static void sw_clear() {
  sw_starttime=0;
  sw_elapsed=0;
  sw_stampc=0;
  video_dirty=1;
}

static void handle_input_stopwatch(uint8_t pressed,uint8_t aux) {
  // Very easy inputwise! We don't care about (aux), and only use A and B.
  if (pressed&TINYC_BUTTON_A) {
    if (sw_starttime) sw_record_stamp();
    else sw_begin();
  }
  if (pressed&TINYC_BUTTON_B) {
    if (sw_starttime) sw_end();
    else sw_clear();
  }
}

static void stopwatch_update() {
  if (!sw_starttime) return;
  uint32_t ms=millis()-sw_starttime;
  uint32_t s=ms/1000;
  uint16_t stamp=
    ((s/600)<<12)|
    (((s%600)/60)<<8)|
    (((s%60)/10)<<4)|
    (s%10)
  ;
  if (stamp!=sw_elapsed) {
    sw_elapsed=stamp;
    video_dirty=1;
  }
}

static void draw_stamp(uint32_t stamp,int8_t x,int8_t y) {
  char text[9]={
    '0'+((stamp>>24)&15),
    '0'+((stamp>>20)&15),
    ':',
    '0'+((stamp>>16)&15),
    '0'+((stamp>>12)&15),
    '.',
    '0'+((stamp>>8)&15),
    '0'+((stamp>>4)&15),
    '0'+(stamp&15),
  };
  softarcade_font_render(&image_fb,x,y,&font,text,sizeof(text),0x0b);
}

static void draw_stopwatch() {

  if (sw_starttime) {
    softarcade_fill_rect(&image_fb,0,11,96,53,0x20);
  }
  
  int8_t x=30,y=27;
  int8_t colw=fancydigitv[0].w;
  dice_draw_digit(x,y,(sw_elapsed>>12)&15); x+=colw;
  dice_draw_digit(x,y,(sw_elapsed>>8)&15); x+=colw;
  softarcade_fill_rect(&image_fb,x,y+4,2,2,0xff);
  softarcade_fill_rect(&image_fb,x,y+8,2,2,0xff);
  x+=3;
  dice_draw_digit(x,y,(sw_elapsed>>4)&15); x+=colw;
  dice_draw_digit(x,y,sw_elapsed&15);
  
  y=46;
  x=8;
  int8_t i=0;
  for (;i<sw_stampc;i++) {
    draw_stamp(sw_stampv[i],x,y);
    x+=48;
    if (x>=96) {
      x=8;
      y+=8;
    }
  }
}

/* Statistics mode.
 **************************************************************/
 
#define ST_COLC 4
#define ST_ROWC 8
#define ST_COLW (96/ST_COLC)
#define ST_ROWH (53/ST_ROWC)
#define ST_CONTENT_LIMIT 8 /* max size of cell content in chars */
#define ST_KEY_LABEL_LIMIT 4
#define ST_STORAGE_LIMIT (9*ST_COLC*ST_ROWC) /* 216 bytes */
#define ST_SAVE_PATH "/Hecubus/stats.csv"

static uint8_t st_col=0,st_row=0;
static uint8_t st_editing=0;

static struct st_cell {
  char content[ST_CONTENT_LIMIT];
  uint8_t contentc;
  char backup[ST_CONTENT_LIMIT];
  uint8_t backupc;
} st_cellv[ST_COLC*ST_ROWC]={0};

static struct st_key {
  char label[ST_KEY_LABEL_LIMIT];
  uint8_t labelc;
  uint8_t bgcolor;
} st_keyv[16]={
#define _(lbl,c) {lbl,sizeof(lbl)-1,c},
  _("7",0xb6)
  _("8",0xb6)
  _("9",0xb6)
  _("*",0xed)
  _("4",0xb6)
  _("5",0xb6)
  _("6",0xb6)
  _("-",0xed)
  _("1",0xb6)
  _("2",0xb6)
  _("3",0xb6)
  _("+",0xed)
  _("0",0xb6)
  _("<<",0x4f)
  _("X",0x03)
  _("OK",0x1c)
#undef _
};

static uint8_t st_keyp=0; // 0..15
static char st_edit_input[ST_CONTENT_LIMIT];
static uint8_t st_edit_inputc=0;
static char st_edit_output[ST_CONTENT_LIMIT];
static uint8_t st_edit_outputc=0;

static void st_load() {
  char src[ST_STORAGE_LIMIT];
  int32_t srcc=tinyc_file_read(src,sizeof(src),ST_SAVE_PATH);
  if ((srcc>0)&&(srcc<=sizeof(src))) {
    uint16_t srcp=0;
    uint8_t col=0,row=0;
    while (srcp<srcc) {
    
      const char *content=src+srcp;
      uint8_t contentc=0;
      while ((srcp<srcc)&&(src[srcp]!=',')&&(src[srcp]!=0x0a)) { srcp++; contentc++; }
      if ((col<ST_COLC)&&(row<ST_ROWC)) {
        struct st_cell *cell=st_cellv+row*ST_COLC+col;
        while (contentc&&((uint8_t)content[0]<=0x20)) { contentc--; content++; }
        if (contentc>ST_CONTENT_LIMIT) contentc=ST_CONTENT_LIMIT;
        while (contentc&&((uint8_t)content[contentc-1]<=0x20)) contentc--;
        memcpy(cell->content,content,contentc);
        cell->contentc=contentc;
      }
      
      if (src[srcp]==0x0a) {
        srcp++;
        col=0;
        row++;
        if (row>=ST_ROWC) return; // no sense reading the rest, if there is any
      } else if (src[srcp]==',') {
        srcp++;
        col++;
      }
    }
  }
}

static void st_save() {
  char src[ST_STORAGE_LIMIT];
  uint16_t srcc=0;
  const struct st_cell *cell=st_cellv;
  int8_t yi=ST_ROWC;
  for (;yi-->0;) {
    int8_t xi=ST_COLC;
    for (;xi-->0;cell++) {
      memcpy(src+srcc,cell->content,cell->contentc);
      srcc+=cell->contentc;
      src[srcc++]=',';
    }
    src[srcc-1]=0x0a; // turn that last comma into a linefeed
  }
  tinyc_file_write(ST_SAVE_PATH,src,srcc);
}

// Clear cell or restore cleared content.
static void st_clear_cell(uint8_t col,uint8_t row,uint8_t persist) {
  if (col>=ST_COLC) return;
  if (row>=ST_ROWC) return;
  struct st_cell *cell=st_cellv+row*ST_COLC+col;
  if (cell->contentc) {
    memcpy(cell->backup,cell->content,cell->contentc);
    cell->backupc=cell->contentc;
    cell->contentc=0;
  } else if (cell->backupc) {
    memcpy(cell->content,cell->backup,cell->backupc);
    cell->contentc=cell->backupc;
  } else return;
  video_dirty=1;
  if (persist) st_save();
}

static void st_clear_all() {
  uint8_t row=0; for (;row<ST_ROWC;row++) {
    uint8_t col=0; for (;col<ST_COLC;col++) {
      st_clear_cell(col,row,0);
    }
  }
  st_save();
}

// Evaluate a signed decimal integer, return length consumed or zero on error.
static uint8_t st_edit_eval_literal(int32_t *dst,const char *src,uint8_t srcc) {
  *dst=0;
  uint8_t positive=1,srcp=0,digitc=0;
  if (srcp>=srcc) return 0;
  if (src[srcp]=='-') {
    positive=0;
    if (++srcp>=srcc) return 0;
  } else if (src[srcp]=='+') {
    if (++srcp>=srcc) return 0;
  }
  while ((srcp<srcc)&&(src[srcp]>='0')&&(src[srcp]<='9')) {
    uint8_t digit=src[srcp++]-'0';
    // Not checking for overflow; inputs are limited to 8 decimal digits.
    (*dst)*=10;
    if (positive) (*dst)+=digit;
    else (*dst)-=digit;
    digitc++;
  }
  if (!digitc) return 0;
  return srcp;
}

// Evaluate some combined expression into st_edit_output, or put an appropriate error message there.
// I'm not going to respect any order of operations: Left to right, as we encounter things.
static void st_edit_eval(const char *src,uint8_t srcc) {
  #define SYNTAX_ERROR { \
    memcpy(st_edit_output,"STX ERR",7); \
    st_edit_outputc=7; \
    return; \
  }
  #define OVERFLOW_ERROR { \
    memcpy(st_edit_output,"OVR ERR",7); \
    st_edit_outputc=7; \
    return; \
  }
  int32_t acc=0;
  st_edit_outputc=0;
  uint8_t srcp=st_edit_eval_literal(&acc,src,srcc);
  if (!srcp) SYNTAX_ERROR
  while (srcp<srcc) {
    
    char operator=src[srcp++];
    
    int32_t r;
    uint8_t rc=st_edit_eval_literal(&r,src+srcp,srcc-srcp);
    if (!rc) SYNTAX_ERROR
    srcp+=rc;
    
    switch (operator) {
      case '+': acc+=r; break;
      case '-': acc-=r; break;
      case '*': acc*=r; break;
      default: SYNTAX_ERROR
    }
    
    // Restrict to 6 characters
    if ((acc<-99999)||(acc>999999)) OVERFLOW_ERROR
  }
  
  st_edit_outputc=snprintf(st_edit_output,sizeof(st_edit_output),"%d",acc);
  if (st_edit_outputc>=ST_CONTENT_LIMIT) { // sic '>=', snprintf always emits a terminator
    OVERFLOW_ERROR
  }
  #undef SYNTAX_ERROR
  #undef OVERFLOW_ERROR
}

static void st_edit_generate_output() {
  const struct st_cell *cell=st_cellv+st_row*ST_COLC+st_col;
  
  // Empty input means keep the prior.
  if (!st_edit_inputc) {
    memcpy(st_edit_output,cell->content,cell->contentc);
    st_edit_outputc=cell->contentc;
    
  // If input begins with an operator, combine and evaluate.
  } else if ((st_edit_input[0]=='+')||(st_edit_input[0]=='-')||(st_edit_input[0]=='*')) {
    char combined[ST_CONTENT_LIMIT<<1];
    memcpy(combined,cell->content,cell->contentc);
    memcpy(combined+cell->contentc,st_edit_input,st_edit_inputc);
    st_edit_eval(combined,cell->contentc+st_edit_inputc);
  
  // Begins with a digit, evaluate input and overwrite prior.
  } else {
    st_edit_eval(st_edit_input,st_edit_inputc);
  }
}

static void st_edit_cell(uint8_t col,uint8_t row) {
  st_editing=1;
  st_keyp=15; // start with "OK" focussed
  st_edit_inputc=0;
  st_edit_outputc=0;
  video_dirty=1;
  st_edit_generate_output();
}

static void st_edit_backspace() {
  if (st_edit_inputc>0) {
    st_edit_inputc--;
    video_dirty=1;
    st_edit_generate_output();
  }
}

static void st_edit_append(char ch) {
  if (st_edit_inputc<sizeof(st_edit_input)) {
    st_edit_input[st_edit_inputc++]=ch;
    video_dirty=1;
    st_edit_generate_output();
  }
}

static void st_edit_cancel() {
  st_editing=0;
  video_dirty=1;
}

static void st_edit_commit() {
  struct st_cell *cell=st_cellv+st_row*ST_COLC+st_col;
  if ((cell->contentc!=st_edit_outputc)||memcmp(cell->content,st_edit_output,st_edit_outputc)) {
    memcpy(cell->content,st_edit_output,st_edit_outputc);
    cell->contentc=st_edit_outputc;
    st_save();
  }
  st_editing=0;
  video_dirty=1;
}

static void st_edit_press_key() {
  switch (st_keyp) {
    case 13: st_edit_backspace(); break;
    case 14: st_edit_cancel(); break;
    case 15: st_edit_commit(); break;
    default: { // keys 0..12 are literals, append the first character of the label.
        st_edit_append(st_keyv[st_keyp].label[0]);
      }
  }
}

static void st_edit_adjust(int8_t dx,int8_t dy) {
  uint8_t col=((st_keyp&3)+dx)&3;
  uint8_t row=((st_keyp>>2)+dy)&3;
  uint8_t np=(row<<2)|col;
  if (np==st_keyp) return;
  st_keyp=np;
  video_dirty=1;
}

static void handle_input_statistics(uint8_t pressed,uint8_t aux) {

  // In (aux), UP/DOWN to focus, or B to clear all.
  if (aux) {
    if (pressed&TINYC_BUTTON_DOWN) {
      focus_mode_select=0;
      st_row=0;
      video_dirty=1;
    } else if (pressed&TINYC_BUTTON_UP) {
      focus_mode_select=0;
      st_row=ST_ROWC-1;
      video_dirty=1;
    } else if (pressed&TINYC_BUTTON_B) {
      st_clear_all();
    }
    return;
  }
  
  // Everything's different when editing...
  if (st_editing) {
         if (pressed&TINYC_BUTTON_DOWN) st_edit_adjust(0,1);
    else if (pressed&TINYC_BUTTON_UP) st_edit_adjust(0,-1);
    else if (pressed&TINYC_BUTTON_LEFT) st_edit_adjust(-1,0);
    else if (pressed&TINYC_BUTTON_RIGHT) st_edit_adjust(1,0);
    else if (pressed&TINYC_BUTTON_A) st_edit_press_key();
    else if (pressed&TINYC_BUTTON_B) st_edit_backspace();
    return;
  }

  // D-pad to move selection. Vertical OOB relinquishes focus.
  if (pressed&TINYC_BUTTON_DOWN) {
    if (st_row==ST_ROWC-1) focus_mode_select=1;
    else st_row++;
    video_dirty=1;
  } else if (pressed&TINYC_BUTTON_UP) {
    if (st_row==0) focus_mode_select=1;
    else st_row--;
    video_dirty=1;
  }
  if (pressed&TINYC_BUTTON_LEFT) {
    if (st_col) st_col--;
    else st_col=ST_COLC-1;
    video_dirty=1;
  } else if (pressed&TINYC_BUTTON_RIGHT) {
    if (st_col<ST_COLC-1) st_col++;
    else st_col=0;
    video_dirty=1;
  }
  
  // B to cancel edit or clear the selected cell.
  if (pressed&TINYC_BUTTON_B) {
    st_clear_cell(st_col,st_row,1);
  }
  
  // A to edit the selected cell.
  if (pressed&TINYC_BUTTON_A) {
    st_edit_cell(st_col,st_row);
  }
}

static void statistics_update() {
  // Nothing to do here.
}

static void st_edit_draw() {

  // Blue background.
  const struct st_cell *cell=st_cellv+st_row*ST_COLC+st_col;
  softarcade_fill_rect(&image_fb,0,11,96,53,0xe0);

  // Cell name in upper left corner.
  char cellname[2]={'A'+st_col,'0'+st_row};
  softarcade_font_render(&image_fb,1,11,&font,cellname,sizeof(cellname),0xff);
  
  // Boxes containing virgin prior value, input, and a preview of the output if accepted.
  softarcade_fill_rect(&image_fb,1,19,42,7,0xda);
  softarcade_font_render(&image_fb,2,19,&font,cell->content,cell->contentc,0x00);
  softarcade_fill_rect(&image_fb,1,27,42,7,0xff);
  softarcade_font_render(&image_fb,2,27,&font,st_edit_input,st_edit_inputc,0x00);
  softarcade_fill_rect(&image_fb,1,35,42,7,0xde);
  softarcade_font_render(&image_fb,2,35,&font,st_edit_output,st_edit_outputc,0x00);
  
  // 4x4 grid of keys.
  const int8_t btnsx=44;
  const int8_t btnsy=12;
  const int8_t btnw=13;
  const int8_t btnh=13;
  const struct st_key *key=st_keyv;
  int8_t row=0,y=btnsy,p=0;
  for (;row<4;row++,y+=btnh) {
    int8_t col=0,x=btnsx;
    for (;col<4;col++,x+=btnw,key++,p++) {
      if (p==st_keyp) {
        softarcade_fill_rect(&image_fb,x-1,y-1,btnw+1,btnh+1,0x1f);
      }
      softarcade_fill_rect(&image_fb,x,y,btnw-1,btnh-1,key->bgcolor);
      uint8_t labelw=softarcade_font_measure(&font,key->label,key->labelc);
      softarcade_font_render(&image_fb,x+(btnw>>1)-(labelw>>1),y+3,&font,key->label,key->labelc,0x00);
    }
  }
}

static void draw_statistics() {

  if (st_editing) {
    st_edit_draw();
    return;
  }

  // White background, black OOB and light blue grid lines.
  // Columns fill the screen exactly, so we cheat the rightmost one pixel short.
  // Rows leave some margin at the bottom, we black it out.
  softarcade_fill_rect(&image_fb,0,11,96,53,0xff);
  
  // If focussed, highlight the selected cell.
  if (!focus_mode_select) {
    int8_t x=st_col*ST_COLW;
    int8_t y=11+st_row*ST_ROWH;
    int8_t w=ST_COLW;
    if (st_col==ST_COLC-1) w--;
    int8_t h=ST_ROWH;
    softarcade_fill_rect(&image_fb,x,y,w,h,0x1f);
  }
  
  // Grid lines and blackout.
  softarcade_fill_rect(&image_fb, 0,11,1,53,0xf2);
  softarcade_fill_rect(&image_fb,24,11,1,53,0xf2);
  softarcade_fill_rect(&image_fb,48,11,1,53,0xf2);
  softarcade_fill_rect(&image_fb,72,11,1,53,0xf2);
  softarcade_fill_rect(&image_fb,95,11,1,53,0xf2);
  int8_t i;
  for (i=11;i<64;i+=ST_ROWH) softarcade_fill_rect(&image_fb,0,i,96,1,0xf2);
  softarcade_fill_rect(&image_fb,0,12+ST_ROWH*ST_ROWC,96,64,0x00);
  
  // Render cell content.
  int8_t y=11,x=0;
  const struct st_cell *cell=st_cellv;
  for (i=ST_COLC*ST_ROWC;i-->0;cell++) {
    if (cell->contentc) {
      int8_t textw=softarcade_font_measure(&font,cell->content,cell->contentc);
      softarcade_font_render(&image_fb,x+1+ST_COLW-textw,y,&font,cell->content,cell->contentc,0x00);
    }
    x+=ST_COLW;
    if (x>=96) {
      x=0;
      y+=ST_ROWH;
    }
  }
}

/* General input.
 **************************************************************/
 
static uint8_t pvinput=0;

static void handle_input(uint8_t pressed) {
  if (!pressed) return;
  if (!seeded) {
    srand(millis());
    seeded=1;
  }

  // When mode select is focused, it monopolizes LEFT/RIGHT and forwards other keys.
  uint8_t aux=0;
  if (focus_mode_select) {
    if (pressed&TINYC_BUTTON_LEFT) {
      if (!mode_select) mode_select=3;
      else mode_select--;
      video_dirty=1;
    } else if (pressed&TINYC_BUTTON_RIGHT) {
      if (mode_select==3) mode_select=0;
      else mode_select++;
      video_dirty=1;
    }
    if (!(pressed&=~(TINYC_BUTTON_LEFT|TINYC_BUTTON_RIGHT))) return;
    aux=1;
  }
  
  switch (mode_select) {
    case 0: handle_input_dice(pressed,aux); break;
    case 1: handle_input_countdown(pressed,aux); break;
    case 2: handle_input_stopwatch(pressed,aux); break;
    case 3: handle_input_statistics(pressed,aux); break;
  }
}

static void update(uint8_t input) {
  if (input!=pvinput) {
    handle_input(input&~pvinput);
    pvinput=input;
  }
  switch (mode_select) {
    case 0: dice_update(); break;
    case 1: countdown_update(); break;
    case 2: stopwatch_update(); break;
    case 3: statistics_update(); break;
  }
}

/* General rendering.
 *****************************************************************/
 
static void init_tiles() {
  image_tiles.colorkey=28;
  tilew=image_tiles.w>>2;
  tileh=image_tiles.h>>2;
  uint8_t i=0;
  for (;i<16;i++) {
    uint8_t x=(i&3)*tilew;
    uint8_t y=(i>>4)*tileh;
    softarcade_image_crop(tilev+i,&image_tiles,x,y,tilew,tileh);
  }
  
  uint8_t colw=image_fancydigits.w/10;
  image_fancydigits.colorkey=28;
  for (i=0;i<10;i++) {
    softarcade_image_crop(fancydigitv+i,&image_fancydigits,colw*i,0,colw,image_fancydigits.h);
  }
}

static void draw_mode_tile(int8_t x,int8_t y,uint8_t mode) {
  if (mode==mode_select) {
    uint8_t color=focus_mode_select?0x18:0x08;
    softarcade_fill_rect(&image_fb,x-1,y-1,tilew+2,tileh+2,color);
  }
  softarcade_blit_unchecked(&image_fb,x,y,tilev+mode);
}
 
static void draw_scene() {
  memset(image_fb.v,0x04,96*64);
  
  draw_mode_tile( 1,1,0);
  draw_mode_tile(10,1,1);
  draw_mode_tile(19,1,2);
  draw_mode_tile(28,1,3);
  
  const char *label=0;
  switch (mode_select) {
    case 0: label="Dice"; break;
    case 1: label="Countdown"; break;
    case 2: label="Stopwatch"; break;
    case 3: label="Statistics"; break;
  }
  if (label) {
    uint8_t labelc=0;
    while (label[labelc]) labelc++;
    //int16_t w=softarcade_font_measure(&font,label,labelc);
    softarcade_font_render(
      &image_fb,40,2,&font,label,labelc,0x92
    );
  }
  
  softarcade_fill_rect(&image_fb,0,10,96,1,0x92);
  
  switch (mode_select) {
    case 0: draw_dice(); break;
    case 1: draw_countdown(); break;
    case 2: draw_stopwatch(); break;
    case 3: draw_statistics(); break;
  }
}

/* Main loop.
 ******************************************************************/

static double next_time=0.0;

void loop() {

  unsigned long now=millis();
  if (now<next_time) {
    delay(1);
    return;
  }
  next_time+=16.66666;
  if (next_time<now) {
    tinyc_usb_log("dropped frame\n");
    next_time=now+16.66666;
  }

  update(tinyc_read_input());

  if (video_dirty) {
    draw_scene();
    tinyc_send_framebuffer(image_fb.v);
    video_dirty=0;
  }
}

/* Initialize.
 */

void setup() {

  #if SOFTARCADE_AUDIO_ENABLE
  softarcade_synth_init(&synth,22050,get_wave,get_pcm,0);
  #endif
  
  tinyc_init();
  init_tiles();
  st_load();
  
  tinyc_init_usb_log();
}
