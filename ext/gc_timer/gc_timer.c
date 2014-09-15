#include "gc_timer.h"

// TODO: This struct should probably have a better API for distiguishing major/
// minor GC phases for a real implementation.
typedef struct {
    int started;
    int total_time;
    int count;
} gc_timer_t;

VALUE rb_cGCTimer;
VALUE timers_array;

#ifdef RUBY_INTERNAL_EVENT_GC_START
#include "ruby/debug.h"
VALUE tp_gc_start, tp_gc_end;

void on_gc_start(VALUE tpval, void *data);
void on_gc_end(VALUE tpval, void *data);

#endif

void
Init_gc_timer(void)
{
  // GC::Timer
  rb_cGCTimer = rb_define_class_under(rb_mGC, "Timer", rb_cObject);

  // Collection of all timers we're managing
  timers_array = rb_ary_new();
  rb_gc_register_mark_object(timers_array);
  rb_define_singleton_method(rb_cGCTimer, "all_timers", gc_timer_all_timers, 0);

  // Struct support for our GC::Timer class
  rb_define_alloc_func(rb_cGCTimer, gc_timer_alloc);
  rb_define_method(rb_cGCTimer, "clear",      gc_timer_clear, 0);
  rb_define_method(rb_cGCTimer, "count",      gc_timer_count, 0);
  rb_define_method(rb_cGCTimer, "total_time", gc_timer_total_time, 0);

#ifdef RUBY_INTERNAL_EVENT_GC_START
  // Register tracepoints as simple way to hook in on 2.1
  // In the real implementation if this gets taken, this would get backed into
  // the GC_PROF_TIMER_START and STOP macros used directly from the gc.c code
  //
  // TODO: These may not be precisely the right points, and/or there may be
  // other tracepoints around lazy sweeping that that should be timed.
  tp_gc_start = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_GC_START,     on_gc_start, NULL);
  tp_gc_end   = rb_tracepoint_new(0, RUBY_INTERNAL_EVENT_GC_END_SWEEP, on_gc_end, NULL);
  rb_tracepoint_enable(tp_gc_start);
  rb_tracepoint_enable(tp_gc_end);
#endif
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

  // TODO: Use TypedData_Wrap_Struct instead
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

// TODO: This timing is totally the wrong precision. Figure out the right calls
// when not on a plane without network. Probably like getrusage_time in gc.c.
int
get_time() {
  return time(NULL);
}

void
on_gc_start(VALUE tpval, void *data) {
  int count = RARRAY_LEN(timers_array);
  int now = get_time();
  for (int i = 0; i < count; i++) {
    VALUE timer = rb_ary_entry(timers_array, i);
    get_timer(timer)->started = now;
  }
}

void
on_gc_end(VALUE tpval, void *data) {
  int count = RARRAY_LEN(timers_array);
  int now = get_time();
  for (int i = 0; i < count; i++) {
    gc_timer_t *timer = get_timer(rb_ary_entry(timers_array, i));
    if (timer->started > 0) {
      timer->total_time += (now - timer->started);
    }

    timer->started = 0;
    timer->count += 1;
  }
}
