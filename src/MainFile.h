#pragma once
#ifndef _MAINFILEH_
#define _MAINFILEH_

void navigate_to_clock_window();

void clock_window_load(Window *window);
void clock_window_unload(Window *window);

void navigate_to_settings_window();

void setting_window_load (Window *window);
void setting_window_unload (Window *window);
int get_total_time();

void select_click_handler(ClickRecognizerRef recognizer, void *context);
void up_click_handler(ClickRecognizerRef recognizer, void *context);
void down_click_handler(ClickRecognizerRef recognizer, void *context);

int get_total_time1();
int get_total_time2();

void set_time1(int i);
void set_time2(int i);
#endif