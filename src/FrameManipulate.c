#include <pebble.h>
#include "FrameManipulate.h"


static bool byte_get_bit(uint8_t *byte, uint8_t bit) {
  return ((*byte) >> bit) & 1;
}

static void byte_set_bit(uint8_t *byte, uint8_t bit, uint8_t value) {
  *byte ^= (-value ^ *byte) & (1 << bit);
}


static GColor get_pixel_color(GBitmapDataRowInfo info, GPoint point) {
#if defined(PBL_COLOR)
  // Read the single byte color pixel
  return (GColor){ .argb = info.data[point.x] };
#elif defined(PBL_BW)
  // Read the single bit of the correct byte
  uint8_t byte = point.x / 8;
  uint8_t bit = point.x % 8; 
  return byte_get_bit(&info.data[byte], bit) ? GColorWhite : GColorBlack;
#endif 
}

static void set_pixel_color(GBitmapDataRowInfo info, GPoint point, 
                                                                GColor color) {

  // Write the pixel's byte color
  memset(&info.data[point.x], color.argb, 1);
}


void 
  Niko_flip_horizontal(GBitmap *bitMap) 
{
  GRect bounds = gbitmap_get_bounds (bitMap);
  // iterate top half rows
  for (int y = 0; y < bounds.size.h / 2; y++) 
  {
    // read current row and its mirror row
    GBitmapDataRowInfo info1 =gbitmap_get_data_row_info(bitMap, y);
    GBitmapDataRowInfo info2 =gbitmap_get_data_row_info(bitMap, 167 - y);
    
    for (int x = 0; x < 144; x++) 
    {
      // save color to temp variable
      GColor temp = get_pixel_color(info1, GPoint(x,y));
      
      // swap colors
      set_pixel_color
        (info1, GPoint(x,y), get_pixel_color(info2, GPoint(x, 167 - y)));
      set_pixel_color
        (info2, GPoint(x, 167 - y), temp);
    }    
  }
}


// DONE
void
  Niko_mirror_image(GBitmap *bitMap)
{
  GRect bounds = gbitmap_get_bounds (bitMap);
  //GColor columns [144];
  
  for (int y = 0; y < bounds.size.h; y++) 
  {
    GBitmapDataRowInfo info = gbitmap_get_data_row_info(bitMap, y);
        
    for (int x = info.min_x; x < info.max_x / 2; x++) 
    {
      //columns [x] = get_pixel_color(info, GPoint(x,y));
      GColor temp = get_pixel_color(info, GPoint(x,y));
      GColor temp2 = get_pixel_color(info, GPoint(143 - x, y));
      
      set_pixel_color (info, GPoint(x,y), temp2);
      set_pixel_color (info, GPoint(143 - x, y), temp);
    }
    /*
    int inverseIndex = info.max_x;
    for (int x = info.min_x; x < info.max_x; x++)
    {
      set_pixel_color
        (info, GPoint(x,y), columns[inverseIndex]);
      inverseIndex--;
    }*/
    
  }
}


