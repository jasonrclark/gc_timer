#include "gc_timer.h"

VALUE rb_cGCTimer;

VALUE timers_array;

void
Init_gc_timer(void)
{
  rb_cGCTimer = rb_define_class_under(rb_mGC, "Timer", rb_cObject);

  // Support for the internal collection of all timers we're managing
  rb_define_singleton_method(rb_cGCTimer, "all_timers", gc_timer_all_timers, 0);
  timers_array = rb_ary_new();
  rb_gc_register_mark_object(timers_array);
}

VALUE
gc_timer_all_timers(VALUE klass) {
  return timers_array;
}
