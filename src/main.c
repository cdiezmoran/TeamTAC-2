#include <pebble.h>
#include "health/health.h"
#include "window/window.h"
#include "messaging/messaging.h"

void init() {
  health_init();
  messaging_init();
  window_push();
}

void terminate() {
  window_ui_destroy();
}

int main(void) {
  init();
  app_event_loop();
  terminate();
}
