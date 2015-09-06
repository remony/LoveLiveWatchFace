#include <pebble.h>

  
static Window *s_main_window;
static TextLayer *s_time_layer;
static GBitmap *s_example_bitmap;
static BitmapLayer *s_bitmap_layer;
static GFont s_custom_font_46;

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[30];
  static char bufferAMPM[2];

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H%n%M", tick_time);
  } else {
    // Use 12 hour format
    
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
    strftime(bufferAMPM, sizeof("00:00 AM"), "%p", tick_time);
    
  }

  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}



static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  //Creating the text layer
  s_custom_font_46 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_MONOTON_47));


  s_time_layer = text_layer_create(GRect(5, 10, bounds.size.w, bounds.size.h));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  text_layer_set_text(s_time_layer, "00:00");
                                  
  // Improve the layout to be more like a watchface
  text_layer_set_font(s_time_layer, s_custom_font_60);
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //image
  s_example_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_LOVEBK);

  s_bitmap_layer = bitmap_layer_create(bounds);
  bitmap_layer_set_bitmap(s_bitmap_layer, s_example_bitmap);


  bitmap_layer_set_compositing_mode(s_bitmap_layer, GCompOpSet);

  layer_add_child(window_layer, bitmap_layer_get_layer(s_bitmap_layer));
  

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_time_layer);
  bitmap_layer_destroy(s_bitmap_layer);
}


static void init() {
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);

  window_stack_push(s_main_window, true);
  update_time();
}
static void deinit() {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
