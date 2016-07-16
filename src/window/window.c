#include "window.h"
Window *my_window;
TextLayer *text_layer, *s_label_layer;
TextLayer *s_value_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static GFont custom_font;

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                                          "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(text_layer, s_buffer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static TextLayer* make_text_layer(int y_inset, char *font_key) {
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);

  TextLayer *this = text_layer_create(grect_inset(bounds, 
                                                GEdgeInsets(y_inset, 0, 0, 0)));
  text_layer_set_text_alignment(this, GTextAlignmentCenter);
  text_layer_set_text_color(this, GColorWhite);
  text_layer_set_background_color(this, GColorClear);
  text_layer_set_font(this, fonts_get_system_font(font_key));
  return this;
}
static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds  = layer_get_bounds(window_layer);
  s_value_layer = make_text_layer(50, FONT_KEY_GOTHIC_24);
  s_label_layer = make_text_layer(80, FONT_KEY_GOTHIC_24);
  layer_add_child(window_layer, text_layer_get_layer(s_value_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
  // New font
  custom_font = fonts_load_custom_font
   (resource_get_handle(RESOURCE_ID_FONT_AVERTA_40));

  // Create GBitmap
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HAPPY);

  // Create BitmapLayer to display the GBitmap
  s_background_layer = bitmap_layer_create(bounds);

  // Set the bitmap onto the layer and add to the window
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));


  text_layer = text_layer_create(
                  GRect(-5, PBL_IF_ROUND_ELSE(35, 0), bounds.size.w, 60));
  text_layer_set_background_color(text_layer, GColorClear);
  text_layer_set_text_color(text_layer, GColorBlack);
  text_layer_set_font(text_layer, custom_font);
  text_layer_set_text_alignment(text_layer, GTextAlignmentRight);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

}

static void set_ui_values(char *label_text, GColor bg_color) {
  text_layer_set_text(s_label_layer, label_text);
  window_set_background_color(my_window, bg_color);
}

static void main_window_unload(Window *window) {
  // Destroy GBitmap
  gbitmap_destroy(s_background_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_background_layer);
  // Destroy Font
  fonts_unload_custom_font(custom_font);
}
void window_push() {
  my_window = window_create();

  window_set_background_color(my_window, GColorBlue);

  window_set_window_handlers(my_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  window_stack_push(my_window, true);
  update_time();
  window_update_ui();
}
void window_ui_destroy() {
  text_layer_destroy(text_layer);
  window_destroy(my_window);
}

void window_update_ui() {
  set_ui_values("Steps taken today", GColorWindsorTan);
}
