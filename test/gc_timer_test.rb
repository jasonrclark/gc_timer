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
end
