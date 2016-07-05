#! /usr/bin/env python

import sys
import xml.parsers.expat
import sdfparser
import sdftransformer
import sdfwriter

### do we have an input file ###
if (len(sys.argv) < 2):
	print "Usage: " + sys.argv[0] + " <graph_file>"
	exit(0)

print "Hello"
print "Transforms SDF3 SDF XML file"
print "Make all actors to do fine-grained reading and writing in sequential order to channels"

### sobreity check ###
if (sys.hexversion < 33948912):
	print "WARNING: This program was designed using Python 2.6.4"
	print "WARNING: You are running:\nPython: " + sys.version
	print "It could still work..."

### open file to read ###
print "Loading ---> " + sys.argv[1]
f = open(sys.argv[1], "r")

### setup the parser ###
parser = xml.parsers.expat.ParserCreate()
parser.StartElementHandler = sdfparser.start_element
parser.EndElementHandler = sdfparser.end_element

print "Parsing"
parser.ParseFile(f)
f.close()

print "Transforming"
(actors, actor_times, channels, channel_sizes)=sdftransformer.transform(sdfparser.actors, sdfparser.actor_times, sdfparser.channels, sdfparser.channel_sizes)

print "Writing output file ---> transformed_" + sys.argv[1]
sdfwriter.write_graph_to_file("transformed_"+sys.argv[1], actors, actor_times, channels, channel_sizes)

print "Success"
