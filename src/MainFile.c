#include <stdio.h>
#include <time.h>
#include <pebble.h>
#include "MainFile.h"
#include "Settings.h"

// different windows for this app
static Window *menu_window;
static Window *clock_window;
static Window *setting_window;
char donation_text[30] = "";

// layers used by windows
static MenuLayer *menu_layer;

// Misc data
static uint16_t menuItemsCount = 3;
static char *test_Text;


// config for clock window buttons
void click_config_provider(void *context){
  
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

// Mainmenu callbacks
void navigate_to_clock_window () {
  
  clock_window = window_create();
  
  window_set_window_handlers(clock_window, (WindowHandlers) {
    .load = clock_window_load,
    .unload = clock_window_unload
  });
  
  window_set_click_config_provider(clock_window, click_config_provider);
  window_stack_push(clock_window, true);
}

void navigate_to_settings_window () {
  setting_window = window_create();
  
  window_set_window_handlers(setting_window, (WindowHandlers){
    .load = setting_window_load,
    .unload = setting_window_unload
  });
  
  window_stack_push(setting_window, true);
}

void draw_row_callback(GContext *ctx, Layer *cell_layer, MenuIndex *cell_index, void *callback_context)
{
   switch (cell_index -> row)
   {
       case 0: 
           menu_cell_basic_draw(ctx, cell_layer, "Game on!", "(forfeit if vs Cryx)", NULL);
           break;
       case 1: 
           menu_cell_basic_draw(ctx, cell_layer, "Settings", "", NULL);
           break;
       case 2: 
           menu_cell_basic_draw(ctx, cell_layer, "Donations", "(not optional!)", NULL);
           break;
   }
}
 
uint16_t num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
    return menuItemsCount;
}
 
void select_click_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *callback_context)
{  
    switch (cell_index -> row) 
    {
       case 0: 
          navigate_to_clock_window();   // pushes the clock window on top of stack
          break;
      
       case 1: 
          navigate_to_settings_window();
          break;
      
       case 2: 
          test_Text = "Thanks for the money";
          break;    
    } 
}


static void menu_window_load(Window *window) {
  
  // -16 for the top bar
  menu_layer = menu_layer_create(GRect(0,0,144,168 - 16));
  
  // enable receive clicks
  menu_layer_set_click_config_onto_window(menu_layer, window);
  
  //set and give callbacks
  
  menu_layer_set_callbacks(menu_layer, NULL, (MenuLayerCallbacks) {
    .draw_row = (MenuLayerDrawRowCallback) draw_row_callback,
    .get_num_rows = (MenuLayerGetNumberOfRowsInSectionsCallback) num_rows_callback,
    .select_click = (MenuLayerSelectCallback) select_click_callback
  } );
  
  // add to window
  layer_add_child(window_get_root_layer(menu_window), menu_layer_get_layer(menu_layer));
}


static void menu_window_unload(Window *window) {
  //menu_layer_destroy(menu_layer);
}


static void init(){
  
   menu_window = window_create();
  
  window_set_window_handlers(menu_window, (WindowHandlers) {
    .load = menu_window_load,
    .unload = menu_window_unload
  });
 
  // reset clock from persistent memory on application start
  re_load_time();
  
  window_stack_push(menu_window, true);
}


static void deinit() {
  menu_layer_destroy(menu_layer);
  window_destroy(menu_window);
}


int main(void) {
  init();
  app_event_loop();
  deinit();
}