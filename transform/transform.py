#! /usr/bin/env python

import sys
import xml.parsers.expat
import sdfparser
import sdftransformer
import sdfwriter


def transform(fileName):
	### open file to read ###
	print ("Loading ---> " + fileName)
	f = open(fileName, "r")

	### setup the parser ###
	parser = xml.parsers.expat.ParserCreate()
	parser.StartElementHandler = sdfparser.start_element
	parser.EndElementHandler = sdfparser.end_element

	print ("Parsing")
	parser.ParseFile(f)
	f.close()

	print ("Transforming")
	(actors, actor_times, channels, channel_sizes)=sdftransformer.transform(sdfparser.actors, sdfparser.actor_times, sdfparser.channels, sdfparser.channel_sizes)

	print ("Writing output file ---> transformed_" + fileName)
	sdfwriter.write_graph_to_file("transformed_"+fileName, actors, actor_times, channels, channel_sizes)

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

	transform(sys.argv[1])


if __name__ == '__main__': main()
