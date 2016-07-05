#! /usr/bin/env python

def write_graph_to_file(file_name, actors, actor_times, channels, channel_sizes):
	f = open(file_name, "w")

	### write the headers ###
	f.write('<?xml version="1.0"?>\n<sdf3 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" version="1.0" type="sdf" xsi:noNamespaceSchemaLocation="http://www.es.ele.tue.nl/sdf3/xsd/sdf3-sdf.xsd">\n<applicationGraph name=\''+file_name[:-4]+'_xml4streamit\'>\n\t<sdf name="xml4streamit" type="mainGraph">\n')

	### write the actors and their ports ###
	for actor, ports in actors.iteritems():
		f.write('\t\t<actor name="' + actor +'" type="a">\n')
		for port, attrs in ports.iteritems():
			f.write('\t\t\t<port ')
			for attr, value in attrs.iteritems():
				f.write(attr + '="' + value + '" ')
			f.write('/>\n')
		f.write('\t\t</actor>\n')
	
	### write the channels ###
	for channel, attrs in channels.iteritems():
		f.write('\t\t<channel ')
		for attr, value in attrs.iteritems():
			f.write(attr + '="' + value + '" ')
		f.write('/>\n')
	f.write('\t</sdf>\n')
	
	### write the actor properties ###
	f.write('\t<sdfProperties>\n')
	for actor, time in actor_times.iteritems():
		f.write('\t\t<actorProperties actor="' + actor + '">\n\t\t\t<processor type="arm" default="true">\n')
		f.write('\t\t\t\t<executionTime time="' + time + '" />\n\t\t\t\t<memory>\n\t\t\t\t\t<stateSize max="316352"/>'
				+'\n\t\t\t\t</memory>\n\t\t\t</processor>\n\t\t</actorProperties>\n')

	### write the channel properties
	for channel, sz in channel_sizes.iteritems():
		f.write('\t\t<channelProperties channel="' + channel + '"')
		if (sz == ""):
			f.write(' />\n')
		else:
			f.write('>\n\t\t\t<tokenSize sz="' + sz + '" />\n\t\t</channelProperties>\n')

	f.write('\t<graphProperties>\n\t\t<timeConstraints>\n\t\t\t<throughput>0.00000003</throughput> <!-- 15fps (iterations) with 500MHz clock -->\n\t\t</timeConstraints>\n\t</graphProperties>\n')
	f.write('\t</sdfProperties>\n')

	### write the footer ###
	f.write('</applicationGraph>\n</sdf3>\n')
	f.close()
