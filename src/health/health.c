#include "health.h"

static bool s_health_available;

static void health_handler(HealthEventType event, void *context) {
  window_update_ui();
}

void health_init() {
  s_health_available = health_service_events_subscribe(health_handler, NULL);
  if (!s_health_available) {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Pebble Health not available!");
  }
}

bool is_health_available() {
  return s_health_available;
}

int health_get_metric_sum(HealthMetric metric) {
  HealthServiceAccessibilityMask mask = health_service_metric_accessible(metric, time_start_of_today(),
    time(NULL));
  if (mask == HealthServiceAccessibilityMaskAvailable) {
    return (int)health_service_sum_today(metric);
  }
  else {
    APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to load data!");
    return 0;
  }
}
