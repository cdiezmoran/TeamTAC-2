#include "messaging.h"

void inbox_received_callback(DictionaryIterator *iterator, void *context) {

}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void messaging_init() {
  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  HealthMetric s_metric;

  int i, value;

  for(i = 1; i <= 7; i++) {
    switch (s_metric) {
      case HealthMetricActiveSeconds:
        //send health_get_metric_sum(s_metric)
        value = health_get_metric_sum(s_metric);
        sendValue(value)
        break;
      case HealthMetricActiveKCalories:
        //send health_get_metric_sum(s_metric)
        value = health_get_metric_sum(s_metric);
        sendValue(value);
        break;
      default:
        break;
    }

    s_metric += 1;
  }
}

void sendValue(int value) {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;

  // Prepare the outbox buffer for this message
  AppMessageResult result = app_message_outbox_begin(&out_iter);

  if(result == APP_MSG_OK) {
  // Construct the message
    dict_write_int(out_iter, MESSAGE_KEY_RequestData, &value, sizeof(int), true);
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
