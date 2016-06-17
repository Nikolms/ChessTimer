#include <stdio.h>
#include <string.h>
#include <pebble.h>
#include "MainFile.h"
#include "FrameManipulate.h"
#include "Settings.h"

static TextLayer * pause_text;
//static RotBitmapLayer * bitmap_rotation1;
//static RotBitmapLayer * bitmap_rotation2;

static Layer * clock_display1;
static Layer * clock_display2;

static GRect rect1;
static GRect rect2;

int current_player_id;
int secondsP1, secondsP2;

char time_text1 [8]; char time_text2 [8]; 

int clock_is_running;

// create custom drawing for text display
static void custom_update_proc1(Layer *layer, GContext *ctx) {
    
  GFont font = fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  graphics_context_set_text_color(ctx, GColorBlack);
  
  graphics_draw_text(ctx, time_text1, font, rect1, 
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
  
  GBitmap *GMap = graphics_capture_frame_buffer (ctx);
  Niko_flip_horizontal(GMap);
  Niko_mirror_image(GMap);
  graphics_release_frame_buffer(ctx, GMap);
}

static void custom_update_proc2(Layer *layer, GContext *ctx) {
  GFont font = fonts_get_system_font( FONT_KEY_BITHAM_34_MEDIUM_NUMBERS);
  graphics_context_set_text_color(ctx, GColorBlack);
  
  graphics_draw_text(ctx, time_text2, font, rect2, 
                     GTextOverflowModeWordWrap, GTextAlignmentCenter, NULL);
}


// function that transforms total amount of seconds into a clock string
void transform_time (int player_id) {
  
  int tempH = 0, tempMin = 0, tempTotal = 0;
  if (player_id == 1) tempTotal = Player_1_Seconds;
  if (player_id == 2) tempTotal = Player_2_Seconds;
  
  char h[2] = "0"; char min[3] = "00"; char s[3] = "00";
  static char * temp = "0:00:00";
  
  // divide into hours and minutes and seconds
    tempH = tempTotal / 3600;
    tempTotal = tempTotal - tempH * 3600;
    tempMin = tempTotal / 60;
    tempTotal = tempTotal - tempMin * 60;
  
   // transform values into strings
    snprintf (h, sizeof("000"), "%d", tempH);
    snprintf (min, sizeof("000"), "%d", tempMin);
    snprintf (s, sizeof("000"), "%d", tempTotal);
  
  temp = h;
    temp = strcat (temp, ":");
    temp = strcat (temp, min);
    temp = strcat (temp, ":");
    temp = strcat (temp, s);
  
  //printf("%d:%d:%d", tempH, tempMin, tempTotal);
  
  if (player_id == 1)
  strcpy(time_text1, h);
  if (player_id == 2)
  strcpy(time_text2, h);
}


// function that updates the clock every second
void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  //Do nothing if either timer has reached 0
  if (Player_1_Seconds <= 0 || Player_2_Seconds <= 0) return;
  
  // check which time to update
  if (current_player_id == 1) {
    Player_1_Seconds --;
    transform_time(1);    
  }
  if (current_player_id == 2) {
    Player_2_Seconds --;
    transform_time(2);    
  }
    layer_mark_dirty(clock_display1);
    layer_mark_dirty(clock_display2);
}


// function that start counting time
void start_clock () {
  // register tick timer
  tick_timer_service_subscribe(SECOND_UNIT,(TickHandler) tick_handler );
  text_layer_set_text (pause_text, "");
}


// button handlers
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    // this button pauses the clock or starts it depending on current state
    if (clock_is_running == 1) 
    {
        tick_timer_service_unsubscribe();
        clock_is_running = 0; 
        text_layer_set_text (pause_text, "Paused");
    }
    else
    {
        start_clock();
        clock_is_running = 1;
        text_layer_set_text (pause_text, "");
    }
}

void up_click_handler(ClickRecognizerRef recognizer, void *context){
  
  if (current_player_id == 2) return;
  
  if (clock_is_running == 0) start_clock();
  current_player_id = 2;
}

void down_click_handler(ClickRecognizerRef recognizer, void *context) {
   
   if (current_player_id == 1) return;
  
   if (clock_is_running == 0) start_clock();
   current_player_id = 1;
}


void clock_window_load (Window * window) {
  
  current_player_id = 1;
  clock_is_running = 0;
  
  GRect bounds = layer_get_bounds(window_get_root_layer(window));
  clock_display1 = layer_create(bounds);
  clock_display2 = layer_create(bounds);
  
  layer_set_update_proc(clock_display1, custom_update_proc1);
  layer_set_update_proc(clock_display2, custom_update_proc2);
  
  rect1 = GRect(0, 120, 144, 40);  
  rect2 = GRect(0,120,144, 40);
  
  pause_text = text_layer_create(
  GRect(0, 64, 144, 30));
    
  text_layer_set_font(pause_text, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text(pause_text, "Paused");
  text_layer_set_text_alignment(pause_text, GTextAlignmentCenter);
      
  layer_add_child(window_get_root_layer(window), clock_display1);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(pause_text));
  layer_add_child(window_get_root_layer(window), clock_display2);
  
  
  transform_time(1);
  transform_time(2);
  
  layer_mark_dirty(clock_display1);
  layer_mark_dirty(clock_display2);
}


void clock_window_unload (Window * window) {
  window_destroy(window);
  tick_timer_service_unsubscribe();
}