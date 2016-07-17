#! /usr/bin/python

import sys
import os
import subprocess
import shlex
import re
from multiprocessing import Process, Lock

import parse_buffer.parse_buffer as parse_buffer
import transform.transform as transform

default_ip = './data/inputs'
default_op = './data/outputs'
default_tp = './data/#temp'
default_rt = 0
default_rd = 0
default_wd = 0

def run_cmd(cmd,input=None):
	# process = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
	process = subprocess.Popen(shlex.split(cmd), shell=False, stdout=subprocess.PIPE)
	# stdoutdata, stderrdata = process.communicate(input)
	return process.communicate(input)

def connect_path(dir,name):
	if (name[0]=='.'):
		name = name[1:]
	if (name[0]=='/'):
		name = name[1:]
	if (dir[-1]=='/'):
		name = name[:-1]
	return (dir + '/' + name)

def hello():
	print ("################################################################################\n")
	print ("Hello,\n")
	print ("  This python code runs the whole process of calculation")
	print ("and analysis on sdf3 format xmls of streaming applications")
	print ("for mpSoC devices and calculates the buffersize-throughput")
	print ("parreto points of the application.\n")
	print ("################################################################################\n")
	print ("Note: you can use [--help] command to see more instructions.\n")
	print ("################################################################################\n")
	### sobreity check ###
	if (sys.hexversion < 34014960):
		print ("Warning: This program was designed using Python 2.7.6")
		print ("Warning: You are running:\n\tPython: " + sys.version)
		print ("It could still work...")
		print ("################################################################################\n")

def install_dependencies():
	pass

def instruction():
	instructions = ''
	instructions += '\n' + ("Usage: " + sys.argv[0] + " \t[--default] [--input_directory <path>]")
	instructions += '\n' + ("\t\t\t[--output_directory <path>] [--temp_directory <path>]")
	instructions += '\n' + ("\t\t\t[--random_count <integer>] [--read_delay <integer>]")
	instructions += '\n' + ("\t\t\t[--write_delay <integer>] [--help]\n")
	instructions += '\n' + ("################################################################################\n")
	instructions += '\n' + ("  the codes should be placed in a directory without spaces in its path. to run")
	instructions += '\n' + ("the program with the default parameters you can use [--default] command")
	instructions += '\n' + ("without any other commands (you may enter other commands but don't use")
	instructions += '\n' + ("[--default] or [--help] with other commands, you don't have to set all values")
	instructions += '\n' + ("if you want to set only one, others will stay the default.), here is the list")
	instructions += '\n' + ("of default values: \n")
	instructions += '\n' + ("\tinput_directory \t= \t"+default_ip+"\n\toutput_directory \t= \t"+default_op+
					"\n\ttemp_directory"+" \t\t= \t"+default_tp+"\n\trandom_times \t\t= \t"+str(default_rt)+
					"\n\tread_delay \t\t= \t"+str(default_rd)+"\n\twrite_delay \t\t= \t"+str(default_wd)+"\n")
	instructions += '\n' + ("  Now let's dig into these input variables and see what each stands for:\n")
	instructions += '\n' + ("\t--input_directory: this directory is where the input xml files")
	instructions += '\n' + ("\tshould be placed, note that file names better be whitespace free.")
	instructions += '\n' + ("\tyou can use the benchmarks provided in './benchmarks' or any")
	instructions += '\n' + ("\tother sdf xml benchmarks.\n")
	instructions += '\n' + ("\t--output_directory: this directory is used to save all final results.")
	instructions += '\n' + ("\tthe code may clear any file that lives in this directory so it's advised")
	instructions += '\n' + ("\tto choose an empty directory.\n")
	instructions += '\n' + ("\t--temp_directory: this directory is used to save temporary files used")
	instructions += '\n' + ("\tduring code execution. the code may clear any file that lives in this")
	instructions += '\n' + ("\tdirectory so it's advised to choose an empty directory.\n")
	instructions += '\n' + ("\t* make sure to start relative paths with '.' (use './data' not '/data')\n")
	instructions += '\n' + ("\t--random_count: we try to see the effects of changing read/write")
	instructions += '\n' + ("\tpermutations of every actor on the buffersize-throughput tradeoff.")
	instructions += '\n' + ("\ttherefor you may want to have some random values for the permutations")
	instructions += '\n' + ("\tto evaluate the results. random_count must be an integer for the")
	instructions += '\n' + ("\tnumber of random permutations that the code uses. the default value")
	instructions += '\n' + ("\tfor random_count is set to '"+str(default_rt)+"'.\n")
	instructions += '\n' + ("\t--read_delay: the algorithm can take times relative to actor execution")
	instructions += '\n' + ("\ttimes that is used for an actor to consume a single element from its")
	instructions += '\n' + ("\tinput buffer and use it in its models. you can manually enter the value")
	instructions += '\n' + ("\tappropriate for your benchmark. the default value for read_delay")
	instructions += '\n' + ("\tis set to '"+str(default_rd)+"'.\n")
	instructions += '\n' + ("\t--write_delay: the algorithm can take times relative to actor execution")
	instructions += '\n' + ("\ttimes that is used for an actor to fire a single element to its")
	instructions += '\n' + ("\toutput buffer and use it in its models. you can manually enter the value")
	instructions += '\n' + ("\tappropriate for your benchmark. the default value for read_delay")
	instructions += '\n' + ("\tis set to '"+str(default_rd)+"'.\n")
	instructions += '\n' + ("################################################################################\n")

	# subprocess.call(['echo',instructions])
	subprocess.call(['echo "'+instructions+'" | less'], shell=True)

def sdf_child(file_name,sdf_analysis,output_path,in_path,print_lock):
	### run sdf3analysis-sdf (--algo buffersize) on selected xml ###
	args = [sdf_analysis,'--graph',connect_path(in_path,file_name),'--algo','buffersize','--output',(connect_path(output_path,file_name)[:-3]+'res')]
	process = subprocess.Popen(args, shell=False, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	stdoutdata, stderrdata = process.communicate()

	### print out results ###
	print_lock.acquire()
	try:
		print ('results for "'+file_name+'" calculated successfully!')
	finally:
		print_lock.release()

def run_all(inputs_path,output_path,temp_path,random_times,read_delay,write_delay):
	# path of run.py
	file_path = os.path.dirname(os.path.abspath(__file__))

	# path of algorithm calculation executable (c code)
	actor_analysis = file_path + '/actor_analysis/ActorGraphAnalysis.out'

	# path of sdf3 analysis executable
	sdf_analysis = file_path + '/sdf3/latest_version/build/release/Linux/bin/sdf3analysis-sdf'

	### clear temp path ###
	subprocess.call('rm '+connect_path(temp_path,'*'), shell=True)
	# run_cmd('rm '+connect_path(temp_path,'*'))

	### clear output path ###
	subprocess.call('rm '+connect_path(output_path,'*'), shell=True)

	### rebuild c code ###
	# run_cmd('g++ -std=c++0x actor_analysis/*.cpp -o actor_analysis/ActorGraphAnalysis.out')

	### get list of all input xmls ###
	input_xmls = [x for x in (run_cmd('ls "'+inputs_path+'"')[0].strip().split('\n')) if (re.match('.*\.xml$',x) != None)]

	### run graph sorting algorithms (c code) ###
	args = [actor_analysis,temp_path,str(random_times)] + [connect_path(inputs_path,x) for x in input_xmls]
	subprocess.call(args, shell=False)

	### run graph transforming module (transform.py) for all generated xmls ###
	python_xmls = []
	for xml in input_xmls:
		python_xmls.append('combined_'+xml)
		python_xmls.append('matrix_'+xml)
		python_xmls.append('unsorted_'+xml)
		python_xmls.append('simple_'+xml)
		for i in range(random_times):
			python_xmls.append('random_'+str(i+1)+'_'+xml)

	for xml in python_xmls:
		transform.transform(connect_path(temp_path,xml), connect_path(temp_path,'transformed_'+xml),read_delay,write_delay)

	### run sdf3analysis-sdf (--algo buffersize) on all xmls ###
	print_lock = Lock()
	sdf_childs = {}
	for xml in input_xmls:
		sdf_childs[xml] = Process(target=sdf_child, args=(xml,sdf_analysis,output_path,inputs_path,print_lock,))
		sdf_childs[xml].start()

	for xml in python_xmls:
		sdf_childs[xml] = Process(target=sdf_child, args=('transformed_'+xml,sdf_analysis,output_path,temp_path,print_lock,))
		sdf_childs[xml].start()

	for xml in input_xmls:
		sdf_childs[xml].join()

	for xml in python_xmls:
		sdf_childs[xml].join()

	### run buffer analysis code (parse_buffer.py) for all outputs ###
	for xml in (input_xmls + ['transformed_'+xml for xml in python_xmls]):
		parse_buffer.parse_buffer(connect_path(output_path,(xml[:-3]+'res')), connect_path(output_path,(xml[:-3]+'buf')))


# create csv file for all results


def main():
	### send initial messages to user ###
	hello()

	### initialize default values ###
	# path of run.py
	file_path = os.path.dirname(os.path.abspath(__file__))
	# path of input xml files
	inputs_path = connect_path(file_path,default_ip)
	# path to write final output files
	output_path = connect_path(file_path,default_op)
	# path to write temporary files
	temp_path = connect_path(file_path,default_tp)
	# number of random permutations to use for each benchmark
	random_times = default_rt
	# the time consumed to read a single element from input buffer
	read_delay = default_rd
	# the time consumed to write a single element to output buffer
	write_delay = default_wd

	### check for proper input argument ###
	if (len(sys.argv) < 2):
		print ("Error: not enough arguments, run '" + sys.argv[0] + " --help' for instructions")
		sys.exit(0)

	### read and evaluate input arguments ###
	head = 1
	for i in range(len(sys.argv)):
		if (head > i):
			continue
		else:
			command = sys.argv[i]
			if (command == '--help'):
				instruction()
				sys.exit(0)
			elif (command == '--default'):
				inputs_path = connect_path(file_path,default_ip)
				output_path = connect_path(file_path,default_op)
				temp_path = connect_path(file_path,default_tp)
				random_times = default_rt
				read_delay = default_rd
				write_delay = default_wd
				break
			elif (command == '--input_directory'):
				if (len(sys.argv) > i+1):
					inputs_path = sys.argv[i+1]
					head += 1
				else:
					print ("Error: invalid usage for '--input_directory'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			elif (command == '--output_directory'):
				if (len(sys.argv) > i+1):
					output_path = sys.argv[i+1]
					head += 1
				else:
					print ("Error: invalid usage for '--output_directory'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			elif (command == '--temp_directory'):
				if (len(sys.argv) > i+1):
					temp_path = sys.argv[i+1]
					head += 1
				else:
					print ("Error: invalid usage for '--temp_directory'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			elif (command == '--random_count'):
				if (len(sys.argv) > i+1):
					try:
						random_times = int(sys.argv[i+1])
						head += 1
					except ValueError:
						print ("Error: invalid input for '--random_count <integer>'")
				else:
					print ("Error: invalid usage for '--random_count'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			elif (command == '--read_delay'):
				if (len(sys.argv) > i+1):
					try:
						read_delay = int(sys.argv[i+1])
						head += 1
					except ValueError:
						print ("Error: invalid input for '--read_delay <integer>'")
				else:
					print ("Error: invalid usage for '--read_delay'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			elif (command == '--write_delay'):
				if (len(sys.argv) > i+1):
					try:
						write_delay = int(sys.argv[i+1])
						head += 1
					except ValueError:
						print ("Error: invalid input for '--write_delay <integer>'")
				else:
					print ("Error: invalid usage for '--write_delay'\n run '" + sys.argv[0] + " --help' for instructions")
					sys.exit(0)
			head += 1

	### run the main analysis code for all benchmarks ###
	run_all(inputs_path,output_path,temp_path,random_times,read_delay,write_delay)

if __name__ == '__main__': main()
