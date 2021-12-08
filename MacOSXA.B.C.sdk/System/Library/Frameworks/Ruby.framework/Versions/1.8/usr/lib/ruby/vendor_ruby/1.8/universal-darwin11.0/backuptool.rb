# backuptool.rb - BackupTool base class for backup and restore plugin for ServerBackup.
# Consolidates the _backup, _restore, and _verify functions in a single tool
#
# Author:: Apple Inc.
# Documentation:: Apple Inc.
# Copyright © 2011, Apple Inc.
# License:: All rights reserved.
#

require 'ftools'
require 'logger'
require 'optparse'
require 'ostruct'
require 'sysexits'
require 'tempfile'

include SysExits

BEGIN {
	unless $log
		$log = Logger.new(STDOUT)
		$log.level = ($DEBUG ? Logger::DEBUG : Logger::WARN)
		$logerr = Logger.new(STDERR)
		$logerr.level = ($DEBUG ? Logger::DEBUG : Logger::WARN)
		$log.debug("Debug logging enabled by runtime.")
	end
}
END {
	if $log
		$log.close
		$log = nil
	end
	if $logerr
		$logerr.close
		$logerr = nil
	end
}


# == Apple-internal documentation
#
# == BackupTool
#
# === Introduction
#
# BackupTool is a virtual base class for a command-line tool invoked by
# ServerBackup(8). It parses command line options and dispatches the backup,
# restore and verify functions to the appropriate subclass implementations.
#
# === Features
#
# === Minimal example
#
# === Complete example
#
# === Further documentation
#
# For information on how this class is typically invoked, see ServerBackup(8).
#
class BackupTool
	#
	# Constants
	#
	PLIST_PARENT = "/etc/server_backup"
	LOG_PARENT = "/Library/Logs"
	COMMANDS = %w[help version backup browse restore size verify]
	DATASETS = %w[configuration data all]

	#
	# Attributes
	#
	attr_reader :program_name, :command
	protected :program_name, :command

	#
	# Class Methods
	#

	# Initializes the instance.
	def initialize(svc_name = self.class.to_s, ver = "1.0")
		$log.debug("BackupTool.initialize(#{svc_name},#{ver})")
		@program_name = File.basename($0)
		@service_name = svc_name
		@version = ver
		@banner = "Performs various backup and restore operations for #{@service_name} resources.\n"
		@command = nil
		@options = nil
	end

	#
	# Helper methods
	#

	#
	# Execute a command, check its exit status, and log output if error.
	#
	# +cmd+:: Command to execute and output to capture
	# +&block+:: optional block to execute with stdout if execution was successful
	def BackupTool.launch(cmd)
		$log.debug("BackupTool.launch(#{cmd}#{block_given? ? ',&block' : ''})")
		status = 0
		# Cannot use fork/exec here because exec doesn't inherit fd's; need to use a temp file
		Tempfile.open("backup-") do |tempfile|
			$log.debug("Running command \"#{cmd}\"")
			if !system("#{cmd} 1> #{tempfile.path} 2>&1")
				status = $?.exitstatus
				$logerr.error "Failed command: #{cmd}"
				$logerr.error "Error code: #{status.to_s}"
				tempfile.each_line {|line| $logerr.info " stderr+stdout: #{line}" }
			elsif block_given?
				yield tempfile.read
			end
			tempfile.unlink
		end
		return (status == 0)
	end
	# Convenience wrapper for class method
	#
	# +cmd+:: Command to execute and output to capture
	# +&block+:: optional block to execute with stdout if execution was successful
	def launch(cmd, &block)
		self.class.launch(cmd, &block)
	end


	#
	# Instance Methods
	#

	#
	# Default help strings for each command. These should only be called by
	# usage()
	#
	def help_help	# :nodoc:
		"#{@program_name} -cmd help\n\tShow this message\n"
	end
	def help_version	# :nodoc:
		"#{@program_name} -cmd version\n\tPrint the version of this tool\n"
	end
	def help_backup	# :nodoc:
	# Typical invocation via ServerBackup(8):
	#	{program} -cmd backup -path /.ServerBackups/{service} \
	#		-log /private/var/log/server_backup/{service}_backup.log -opt all
	# The log file is defined in the plist and may therefore be arbitrary.
		<<"EOS"
#{@program_name} [-debug] -cmd backup -path <destination> -opt {configuration | data | all}
	Backup the service files of the type specified by -opt.
	<destination>: absolute path to the mounted image where the files are to be backed up
EOS
	end
	def help_browse	# :nodoc:
		<<"EOS"
#{@program_name} [-debug] -cmd browse -path <image>  -opt {configuration | data | all}
	Display the names of the backup snapshots of the type specified by -opt.
	<image>: absolute path to backup repository
EOS
	end
	def help_restore	# :nodoc:
	# Typical invocation via ServerBackup(8):
	#	{program} -cmd restore -opt all -path /{service} \
	#		-log /private/var/log/server_backup/{service}_restore.log -opt all
	# The log file is defined in the plist and may therefore be arbitrary.
		<<"EOS"
#{@program_name} [-debug] -cmd restore -path <image> -target <target> -opt {configuration | data | all}
	Restore the service files of the type specified by -opt.
	<image>: absolute path to backup repository
	<target>: absolute path to directory where files are to be restoreed; normally "/"
EOS
	end
	def help_size	# :nodoc:
		<<"EOS"
#{@program_name} [-debug] -cmd size -opt {configuration | data | all}
	Calculate and display the size, in kb, of the service files of the type specified by -opt
EOS
	end
	def help_verify	# :nodoc:
		<<"EOS"
#{@program_name} [-debug] -cmd verify -path <image> -target <target> -opt {configuration | data | all}
	Verify the that the specified backup snapshot still matches the service files of the 
		type specified by -opt.
	<image>: absolute path to backup snapshot
	<target>: absolute path to directory where files are to be compared; normally "/"
EOS
	end

	protected :help_help, :help_version, :help_backup, :help_browse
	protected :help_restore, :help_size, :help_verify

	#
	# Default implementations for help and version
	#
	def help
		self.usage(false)
		exit EX_OK
	end

	def version
		print("Version #{@version}\n")
		exit EX_OK
	end

	#
	# Command line processing support
	#

	# Print the help message
	# +err+:: send usage to stderr if true (default) or stdout if false
	def usage(err = true)
		dst = err ? $stderr : $stdout
		dst.print(@banner)
		self.class::COMMANDS.each do |cmd|
			if (self.respond_to?(cmd))
				puts("\n")
				if (self.respond_to?("help_" + cmd))
					print(self.send("help_" + cmd))
				else
					print("*** Command '#{cmd}' supported but no help available!\n")
				end
			end
		end
	end

	# Parse the tool's command line options
	# +argv+:: argument array to parse (typically ARGV)
	# +&block+:: optional block to parse unknown arguments
	def parse!(argv)
		$log.debug("BackupTool.parse!('#{argv.join(" ")}'#{block_given? ? ',&block' : ''})")
		if (argv.size == 0)
			raise OptionParser::InvalidArgument, "No arguments specified."
		end

		# @options is used to store recognized command-line args
		@options = Hash.new
		while arg = argv.shift
			case arg
			when "-cmd"
				@command = argv.shift
			when "-debug"
				$log.level = Logger::DEBUG
				$logerr.level = Logger::DEBUG
			when "-opt"
				@options[:dataset] = argv.shift
			when "-path"
				@options[:path] = argv.shift
			when "-target"
				@options[:target] = argv.shift
			when "-log"
				level = $log.level
				log_path = argv.shift
				$log = Logger.new(log_path)
				$log.level = level
				$logerr = Logger.new(log_path)
				$logerr.level = level
			else
				argv.unshift(arg)
				if block_given?
					unless (argv = yield(argv))
						raise OptionParser::InvalidArgument, "Unknown argument."
					end
				else break
				end
			end		
		end
		raise OptionParser::InvalidArgument, "No command specified." unless @command
		unless (self.class::COMMANDS.include?(@command) && self.respond_to?(@command))
			raise OptionParser::InvalidArgument, "Unknown command '#{@command}' specified."
		end
		return argv
	end

	# Execute the method requested by the command-line arguments
	def run
		raise RuntimeError, "Command line not parsed; call parse!()." unless @command
		$log.debug("Performing #{@command}...")
		return self.send(@command)
	end
end


#
# Unit tests (TBD)
#
if __FILE__ == $0
require 'test/unit'

class TestBackupTool < Test::Unit::TestCase
end

end
