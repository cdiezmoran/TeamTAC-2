/** Filename:   messaging.h
  * Purpose:    Serve as library for messaging.c
  * Functions:  messaging_init, inbox_received_callback, inbox_dropped_callback, 
  *             outbox_failed_callback, outbox_sent_callback
  */

#include <pebble.h>
#include "../health/health.h"

void messaging_init();
void inbox_received_callback(DictionaryIterator *iterator, void *context);
void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);
