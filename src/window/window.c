/** Filename:  window.c
  * Purpose:   setting up the pebble window and updating all the windom ui.
  * Functions:  update_time, tick_handler, make_text_layer, set_ui_values,
  *             loadWatchFace
  */

#include "window.h"
#include "../levels/levels.h"

Window *my_window;
TextLayer *text_layer, *s_label_layer, *s_value_layer, *s_steps_to_evolve_layer;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static GFont custom_font, custom_font_24;

static HealthMetric s_metric;

#define levelOneSteps   1000
#define levelTwoSteps   2000
#define levelThreeSteps 3000
#define levelFourSteps  5000
#define levelFiveSteps  8000

void sendValue(int value, bool isMeters) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;

  // Prepare the outbox buffer for this message
  AppMessageResult result = app_message_outbox_begin(&out_iter);

  if(result == APP_MSG_OK) {
  // Construct the message
    if(isMeters) {
        dict_write_int(out_iter, MESSAGE_KEY_metersWalked, &value, sizeof(int), true);
    }
    else {
        dict_write_int(out_iter, MESSAGE_KEY_kaloriesBurned, &value, sizeof(int), true);
    }
  } else {
    // The outbox cannot be used right now
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
  }

  // Send this message
  result = app_message_outbox_send();

  // Check the result
  if(result != APP_MSG_OK) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
  }
}

void iterateOverMetrics() {
  int i, value;

  for(i = 1; i <= 7; i++) {
    switch (s_metric) {
      case HealthMetricWalkedDistanceMeters:
        //send health_get_metric_sum(s_metric)
        value = health_get_metric_sum(s_metric);
        sendValue(value, true);
        break;
      case HealthMetricActiveKCalories:
        //send health_get_metric_sum(s_metric)
        value = health_get_metric_sum(s_metric);
        sendValue(value, false);
        break;
      default:
        break;
    }

    s_metric += 1;
  }

  s_metric = 1;
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                                          "%H:%M" : "%I:%M", tick_time);
  text_layer_set_text(text_layer, s_buffer);

  int current_level = get_current_level();

  set_current_level(current_level + 1);

  iterateOverMetrics();
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
    update_time();
}

static TextLayer* make_text_layer(int y_inset, GFont font_key) {
  Layer *window_layer = window_get_root_layer(my_window);
  GRect bounds = layer_get_bounds(window_layer);

  TextLayer *this = text_layer_create(grect_inset(bounds,
                                                GEdgeInsets(y_inset, 0, 0, 0)));
  text_layer_set_text_alignment(this, GTextAlignmentCenter);
  text_layer_set_text_color(this, GColorWhite);
  text_layer_set_background_color(this, GColorClear);
  text_layer_set_font(this, font_key);
  return this;
}

static void set_ui_values(char *label_text, GColor bg_color) {
  text_layer_set_text(s_label_layer, label_text);
  window_set_background_color(my_window, bg_color);
}

static void loadWatchFace(Layer *window_layer, GRect bounds, int image_num) {
  custom_font_24 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_AVERTA_24));

  s_value_layer = make_text_layer(80, custom_font_24);
  s_label_layer = make_text_layer(50, custom_font_24);
  layer_add_child(window_layer, text_layer_get_layer(s_value_layer));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
  set_ui_values("Steps taken today", GColorGreen);

  // New font
  custom_font = fonts_load_custom_font
   (resource_get_handle(RESOURCE_ID_FONT_AVERTA_40));

  switch (image_num) {
    case 1:
      s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ANGRY);
      break;
    case 2:
      s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_MEDIUM);
      break;
    case 3:
      s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_FACEONE);
      break;
    case 4:
      s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_HAPPY);
      break;
  }
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

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds  = layer_get_bounds(window_layer);

  int current_level = get_current_level();

  switch (current_level) {
    case 1:
      loadWatchFace(window_layer, bounds, 1);
      break;
    case 2:
      loadWatchFace(window_layer, bounds, 2);
      break;
    case 3:
      loadWatchFace(window_layer, bounds, 3);
      break;
    case 4:
      loadWatchFace(window_layer, bounds, 4);
      break;
    case 5:
      loadWatchFace(window_layer, bounds, 5);
      break;
  }
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
  if (health_is_available() && my_window) {
    static char s_value_buffer[8];
    int totalStepsToEvolve = 0;

    int current_level = get_current_level();

    switch (current_level) {
      case 1:
        totalStepsToEvolve = levelOneSteps;
        break;
      case 2:
        totalStepsToEvolve = levelTwoSteps;
        break;
      case 3:
        totalStepsToEvolve = levelThreeSteps;
        break;
      case 4:
        totalStepsToEvolve = levelFourSteps;
        break;
      case 5:
        totalStepsToEvolve = levelFiveSteps;
        break;
    }

    int stepsToEvolve = totalStepsToEvolve - health_get_metric_sum(s_metric);

    snprintf(s_value_buffer, sizeof(s_value_buffer), "%d", stepsToEvolve);

    /*if(stepsToEvolve <= 0) {
      set_current_level(current_level + 1);
    }*/

    text_layer_set_text(s_value_layer, s_value_buffer);
  }
}
