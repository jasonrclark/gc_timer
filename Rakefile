require "bundler/gem_tasks"

require "rake/extensiontask"
require "rake/testtask"

task :default => [:clean, :compile, :test]

Rake::TestTask.new do |test|
  test.verbose = true
  test.libs << "test"
  test.libs << "lib"
  test.test_files = FileList["test/**/*_test.rb"]
end

Rake::ExtensionTask.new("gc_timer") do |ext|
  ext.lib_dir = "lib/gc_timer"
end

task :console => [:clean, :compile] do
  require "bundler/cli"
  Bundler::CLI.new.console
end

