#include "gc_timer.h"

VALUE rb_mGcTimer;

void
Init_gc_timer(void)
{
  rb_mGcTimer = rb_define_module("GcTimer");
}
