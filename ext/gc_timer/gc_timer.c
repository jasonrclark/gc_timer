#include "gc_timer.h"

typedef struct {
    int started;
    int total_time_s;
    int count;
} gc_timer_t;

VALUE rb_cGCTimer;
VALUE timers_array;

void
Init_gc_timer(void)
{
  // GC::Timer
  rb_cGCTimer = rb_define_class_under(rb_mGC, "Timer", rb_cObject);

  // Collection of all timers we're managing
  rb_define_singleton_method(rb_cGCTimer, "all_timers", gc_timer_all_timers, 0);
  timers_array = rb_ary_new();
  rb_gc_register_mark_object(timers_array);

  // Struct support for our GC::Timer class
  rb_define_alloc_func(rb_cGCTimer, gc_timer_alloc);
}

VALUE
gc_timer_all_timers(VALUE _) {
  return timers_array;
}

static VALUE
gc_timer_alloc(VALUE klass) {
  gc_timer_t *ptimer = xmalloc(sizeof(gc_timer_t));
  VALUE obj = Data_Wrap_Struct(klass, 0, xfree, ptimer);

  /*gc_timer_clear(obj);*/
  rb_ary_push(timers_array, obj);
  return obj;
}
