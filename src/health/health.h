/** Filename:   health.h
  * Purpose:    Serve as library for the health.c file.
  * Functions:  health_init, health_is_available, health_get_metric_sum
  */

#pragma once

#include <pebble.h>

#include "../window/window.h"

void health_init();
bool health_is_available();
int health_get_metric_sum(HealthMetric metric);