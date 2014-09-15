#ifndef GC_TIMER_H
#define GC_TIMER_H 1

#include "ruby.h"
#include "time.h"

VALUE gc_timer_all_timers(VALUE klass);

static VALUE gc_timer_alloc(VALUE klass);

VALUE gc_timer_count(VALUE self);
VALUE gc_timer_clear(VALUE self);
VALUE gc_timer_total_time(VALUE self);

#endif /* GC_TIMER_H */
