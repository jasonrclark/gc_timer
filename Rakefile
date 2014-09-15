require "bundler/gem_tasks"

require "rake/extensiontask"

Rake::ExtensionTask.new("gc_timer") do |ext|
  ext.lib_dir = "lib/gc_timer"
end

task :default => [:clean, :compile]

task :console => [:clean, :compile] do
  require 'bundler/cli'
  Bundler::CLI.new.console
end

