require 'minitest/autorun'
require 'gc_timer'

class GCTimerTest < Minitest::Test
  def setup
    GC::Timer.all_timers.clear
  end

  def test_created_timers_add_to_class_list
    timer = GC::Timer.new
    assert_equal [timer], GC::Timer.all_timers
  end

  def test_defaults_on_new_timer
    timer = GC::Timer.new
    assert_equal 0, timer.count
    assert_equal 0, timer.total_time
  end

  def test_garbage_collection_bumps_values
    timer = GC::Timer.new
    GC.start
    assert timer.count > 0
  end

  def test_clearing
    timer = GC::Timer.new
    GC.start
    GC.disable
    timer.clear
    assert_equal 0, timer.count
    assert_equal 0, timer.total_time
  ensure
    GC.enable
  end
end
