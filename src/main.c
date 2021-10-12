#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "tinyc.h"
#include "softarcade.h"

/* Audio.
 *************************************************************/

#if SOFTARCADE_AUDIO_ENABLE

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
int16_t tinyc_client_update_synthesizer() { return 0; }
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
  28,28,255,255,255,28,28,28,28,28,28,28,255,28,28,28,28,255,255,255,255,28,28,28,28,255,255,255,255,28,28,28,255,255,28,28,28,255,255,28,255,255,255,255,255,255,255,28,28,28,255,255,255,255,28,28,255,255,255,255,255,255,255,28,28,28,255,255,255,28,28,28,28,28,255,255,255,255,28,28,
  28,255,28,28,28,255,28,28,28,28,28,255,255,28,28,28,255,255,28,28,255,255,28,28,255,255,28,28,255,255,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,28,255,255,28,28,255,255,28,255,255,28,28,28,255,255,28,28,255,255,28,255,255,28,28,28,255,255,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,255,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,255,255,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,
  255,255,28,28,255,255,255,28,28,28,28,255,255,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,255,255,28,
  255,255,28,255,28,255,255,28,28,28,28,255,255,28,28,28,28,28,255,255,255,255,28,28,28,28,255,255,255,255,28,28,255,255,255,255,255,255,255,28,255,255,255,255,255,255,28,28,255,255,255,255,255,255,28,28,28,28,28,28,255,255,28,28,28,28,255,255,255,28,28,28,28,28,255,255,255,255,255,28,
  255,255,255,28,28,255,255,28,28,28,28,255,255,28,28,28,28,255,255,255,255,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,255,255,255,28,28,28,28,255,255,28,28,28,28,255,255,28,255,255,28,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,255,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,28,255,255,28,255,255,28,28,28,255,255,28,28,28,28,255,255,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,255,255,28,
  28,255,28,28,28,255,28,28,28,28,28,255,255,28,28,28,255,255,28,28,28,28,28,28,255,255,28,28,255,255,28,28,28,28,28,28,28,255,255,28,255,255,28,28,255,255,28,28,28,255,255,28,255,255,28,28,28,28,28,255,255,28,28,28,28,255,255,28,255,255,28,28,28,28,28,28,28,255,255,28,
  28,28,255,255,255,28,28,28,28,255,255,255,255,255,255,28,255,255,255,255,255,255,255,28,28,255,255,255,255,28,28,28,28,28,28,28,28,255,255,28,28,255,255,255,255,28,28,28,28,28,255,255,255,28,28,28,28,28,28,255,255,28,28,28,28,28,255,255,255,28,28,28,28,28,28,28,28,255,255,28,
)
static struct softarcade_image fancydigitv[10]={0};

static struct softarcade_font font={
  .metrics={
    96,53,105,181,124,181,184,41,85,85,181,181,83,167,47,181,117,117,117,117,117,117,117,117,117,117,46,83,
    117,142,117,153,149,149,149,117,149,149,149,149,149,117,149,149,117,181,181,181,149,149,149,149,181,149,
    181,181,181,181,117,85,181,85,173,175,108,149,149,114,149,149,117,150,149,50,89,117,85,175,143,146,150,150,
    146,149,117,143,175,175,111,150,114,117,53,117,174,0
  },
  .bits={
    0,3892314112,3019898880,1473639680,1584648192,2290649216,1158764852,3221225472,1782579200,2508193792,2881415808,
    557728256,218103808,4160749568,536870912,143165440,1456734208,3375235072,3310223360,3306946560,3079798784,
    4078698496,1398013952,3844341760,1431568384,1435631616,2684354560,1291845632,706871296,4042260480,2292711424,
    3781428224,1773694976,1777963008,3924418560,1917190144,3919175680,4176015360,4175986688,1756983296,2583269376,
    3912105984,286875648,2596966400,2454585344,2397771904,2389391488,1952651008,3924328448,1771794432,3924398080,
    2019680256,4178067968,2576965632,2354356736,2355844352,2324211840,2354856448,3847094272,3938451456,2181570688,
    3586129920,581042176,4063232,2290089984,1635348480,2297028608,1915748352,293171200,1776836608,732168192,1769037824,
    2297008128,2952790016,1163919360,2464808960,2856321024,3580493824,3918528512,1771438080,1776844800,1769017344,
    3918004224,2019680256,1268908032,2574254080,2324168704,2371092480,2860515328,2574344192,4017094656,1780875264,
    4160749568,3366715392,1162084352,0
  },
};


/* Globals.
 **************************************************************/
 
static uint8_t video_dirty=1;
static uint8_t mode_select=0; // 0..3 = dice,timer,stopwatch,stats
static uint8_t focus_mode_select=1; // nonzero means cursor in header bar

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
  //TODO sound effects
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

static void handle_input_statistics(uint8_t pressed,uint8_t aux) {
//TODO
}

static void statistics_update() {
//TODO
}

static void draw_statistics() {
//TODO
}

/* General input.
 **************************************************************/
 
static uint8_t pvinput=0;

static void handle_input(uint8_t pressed) {

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
  
  tinyc_init_usb_log();
}
