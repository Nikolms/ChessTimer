#include <pebble.h>
#include "MainFile.h"

MenuLayer *menu_layer;
int menuItemsCount = 3;
static int minutes, seconds, hours;
static char text_hour[2] = "0", text_minute[3] = "00", text_second[3] = "00";
static int totalTimeP1, totalTimeP2;

int Player_1_Seconds, Player_2_Seconds;


void re_load_time () {
  if (persist_exists(1)) hours = persist_read_int(1);
  else hours = 1;
  
  if (persist_exists(2)) minutes = persist_read_int(2);
  else minutes = 0;
  
  if (persist_exists(3)) seconds = persist_read_int(3);
  else seconds = 0;
  
  int time = seconds;
  time = time + minutes * 60;
  time = time + hours * 3600;
  
  Player_1_Seconds = time;
  Player_2_Seconds = time;
}


// Menu callbacks
void setting_draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
   switch (cell_index -> row)
   {
       case 0: 
           menu_cell_basic_draw(ctx, cell_layer, "Hours", text_hour, NULL);
           break;
       case 1: 
           menu_cell_basic_draw(ctx, cell_layer, "Minutes", text_minute, NULL);
           break;
       case 2: 
           menu_cell_basic_draw(ctx, cell_layer, "Seconds", text_second, NULL);
           break;
   }
}
 
uint16_t setting_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
    return menuItemsCount;
}
 
void setting_select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{  
    switch (cell_index -> row) 
    {
       case 0:
          if (hours >= 9) hours = 0;
          else hours ++;
          snprintf (text_hour, sizeof("00"), "%d", hours);
          persist_write_int (1, hours);
          break;
      
       case 1:
          if (minutes >= 59) minutes = 0;    
          else minutes ++;
          snprintf (text_minute, sizeof("000"), "%d", minutes);
          persist_write_int (2, minutes);
          break;
      
       case 2:
          if (seconds >= 59) seconds = 0;
          else seconds ++;
          snprintf (text_second, sizeof("000"), "%d", seconds);
          persist_write_int (3, seconds);
          break;    
    } 
   layer_mark_dirty(menu_layer_get_layer(menu_layer));
   re_load_time();
}


void set_time1(int i) { totalTimeP1 = i;}
void set_time2(int i) { totalTimeP2 = i;}

void setting_window_load (Window *window) {
  // reset time everytime settings window is entered
  re_load_time();
  
  snprintf (text_hour, sizeof("00"), "%d", hours);
  snprintf (text_minute, sizeof("000"), "%d", minutes);
  snprintf (text_second, sizeof("000"), "%d", seconds);
  
  menu_layer = menu_layer_create(GRect(0,0,144,168 - 16));
  
  menu_layer_set_click_config_onto_window(menu_layer, window);
  
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
    .draw_row = (MenuLayerDrawRowCallback) setting_draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) setting_num_rows_callback,
    .select_click = (MenuLayerSelectCallback) setting_select_click_callback
  } );
  
  layer_add_child(window_get_root_layer(window), menu_layer_get_layer(menu_layer));
}


void setting_window_unload (Window *window){
  menu_layer_destroy(menu_layer);
  window_destroy(window);
}