# GC::Timer

An experiment adding light-weight, independent GC timers in Ruby 2.1.

`GC::Profiler` stores lots of interesting data, but without frequent calls to
`GC::Profiler.clear` it presents a potential memory leak. (TODO: link!!!)

The API also don't work well for multiple consumers of GC data. If more than
one caller to `GC::Profiler` tries to be well-behaved and `clear`, then one or
both of the consumers will miss out on data.

Additionally `GC::Profiler` is (sensibly) disabled by default. It would be nice
to get aggregated numbers without users making code changes to enable it.

## The Experiment

This repo creates an extension gem with a new class, `GC::Timer`.
It's used like this:

```
# Get the timer going
timer = GC::Timer.new

# ... do some work, collect some garbage ...

# Look at the values
puts timer.total_time # => 0.011235
puts timer.count      # => 22

# Allowed in case clients want to reset their counters
timer.clear
```

Timers are independent, so another consumer can create their own `GC::Timer`
with no impact on our instance.


## Caveats and Plans
My intent in putting this up is to show the type of API I'm interested in
adding, not to actually pubish this as a gem.

This depends on tracepoints only available in Ruby 2.1 (they're removed on
master.) If this feature is desired, it should just be implemented directly in
`gc.c`.

## Contributing

1. Fork it ( https://github.com/jasonrclark/gc_timer/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
