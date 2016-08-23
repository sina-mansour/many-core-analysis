#! /usr/bin/python

import sys
import xml.parsers.expat
import sdfparser
import sdftransformer
import sdfmodify
import sdfwriter


def transform(infileName,outfileName,read_delay,write_delay):
	### open file to read ###
	print ("Loading ---> " + infileName)
	f = open(infileName, "r")

	### Initialize data structures ###
	sdfparser.actors = dict({})
	sdfparser.actor_times = dict({})
	sdfparser.channels = dict({})
	sdfparser.channel_sizes = dict({})
	sdfparser.ports = dict({})

	### setup the parser ###
	parser = xml.parsers.expat.ParserCreate()
	parser.StartElementHandler = sdfparser.start_element
	parser.EndElementHandler = sdfparser.end_element

	print ("Parsing")
	parser.ParseFile(f)
	f.close()

	print ("Transforming")
	(actors, actor_times, channels, channel_sizes)=sdftransformer.transform(sdfparser.actors, sdfparser.actor_times, sdfparser.channels, sdfparser.channel_sizes,read_delay,write_delay)

	print ("Writing output file ---> " + outfileName)
	sdfwriter.write_graph_to_file(outfileName, actors, actor_times, channels, channel_sizes)

	print ("Success")

def modify(infileName,outfileName,read_delay,write_delay):
	### open file to read ###
	print ("Loading ---> " + infileName)
	f = open(infileName, "r")

	### Initialize data structures ###
	sdfparser.actors = dict({})
	sdfparser.actor_times = dict({})
	sdfparser.channels = dict({})
	sdfparser.channel_sizes = dict({})
	sdfparser.ports = dict({})

	### setup the parser ###
	parser = xml.parsers.expat.ParserCreate()
	parser.StartElementHandler = sdfparser.start_element
	parser.EndElementHandler = sdfparser.end_element

	print ("Parsing")
	parser.ParseFile(f)
	f.close()

	print ("midifying")
	(actors, actor_times, channels, channel_sizes)=sdfmodify.modify(sdfparser.actors, sdfparser.actor_times, sdfparser.channels, sdfparser.channel_sizes,read_delay,write_delay)

	print ("Writing output file ---> " + outfileName)
	sdfwriter.write_graph_to_file(outfileName, actors, actor_times, channels, channel_sizes)

	print ("Success")

def main():
	### do we have an input file ###
	if (len(sys.argv) < 2):
		print ("Usage: " + sys.argv[0] + " <graph_file>")
		sys.exit(0)

	print ("Hello")
	print ("Transforms SDF3 SDF XML file")
	print ("Make all actors to do fine-grained reading and writing in sequential order to channels")

	### sobreity check ###
	if (sys.hexversion < 34014960):
		print ("WARNING: This program was designed using Python 2.7.6")
		print ("WARNING: You are running:\nPython: " + sys.version)
		print ("It could still work...")

	transform(sys.argv[1],("transformed_"+sys.argv[1]),0,0)


if __name__ == '__main__': main()
