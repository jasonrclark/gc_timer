# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'gc_timer/version'

Gem::Specification.new do |spec|
  spec.name          = "gc_timer"
  spec.version       = GcTimer::VERSION
  spec.authors       = ["Jason R. Clark"]
  spec.email         = ["jclark@newrelic.com"]
  spec.extensions    = ["ext/gc_timer/extconf.rb"]
  spec.summary       = %q{Experimental safer GC timers for Ruby 2.1}
  spec.description   = %q{Implementation of a GC::Timer class that avoids multiple-consumer problems with GC::Profiler}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.6"
  spec.add_development_dependency "rake"
  spec.add_development_dependency "rake-compiler"
end
