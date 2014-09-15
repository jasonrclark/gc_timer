#include "gc_timer.h"

typedef struct {
    int started;
    int total_time;
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
  rb_define_method(rb_cGCTimer, "count",      gc_timer_count, 0);
  rb_define_method(rb_cGCTimer, "total_time", gc_timer_total_time, 0);
}

gc_timer_t *
get_timer(VALUE self) {
  gc_timer_t *timer;
  Data_Get_Struct(self, void, timer);
  return timer;
}

VALUE
gc_timer_all_timers(VALUE _) {
  return timers_array;
}

static VALUE
gc_timer_alloc(VALUE klass) {
  gc_timer_t *ptimer = xmalloc(sizeof(gc_timer_t));
  VALUE obj = Data_Wrap_Struct(klass, 0, xfree, ptimer);

  gc_timer_clear(obj);
  rb_ary_push(timers_array, obj);
  return obj;
}

VALUE
gc_timer_clear(VALUE self) {
  gc_timer_t *timer = get_timer(self);
  timer->count = 0;
  timer->total_time = 0;
  timer->started = 0;
  return self;
}

VALUE
gc_timer_count(VALUE self) {
  return INT2FIX(get_timer(self)->count);
}

VALUE
gc_timer_total_time(VALUE self) {
  return INT2FIX(get_timer(self)->total_time);
}
