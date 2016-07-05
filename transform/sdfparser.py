#! /usr/bin/env python

###	This parser does not support all tags in the SDF3 XML format
###	Only tags relevant to the transformation are analyzed

### Our data structures ###
actors = dict({})
actor_times = dict({})
channels = dict({})
channel_sizes = dict({})
ports = dict({})

cur_actor = ""
cur_channel = ""

### XML parsing functions ###
def start_element(name, attrs):
	global actors, actor_times, channels, channel_sizes, ports, cur_actor, cur_channel
	if (name == "actor"):
		cur_actor = attrs["name"]
	elif (name == "port"):
		ports[attrs["name"]] = dict(attrs)
	elif (name == "actorProperties"):
		cur_actor = attrs["actor"]
	elif (name == "executionTime"):
		actor_times[cur_actor] = attrs["time"]
	elif (name == "channel"):
		channels[attrs["name"]] = dict(attrs)
	elif (name == "channelProperties"):
		cur_channel = attrs["channel"]
		channel_sizes[cur_channel] = ""
	elif (name == "tokenSize"):
		channel_sizes[cur_channel] = attrs["sz"]
		
def end_element(name):
	global actors, ports, cur_actor
	if (name == "actor"):
		actors[cur_actor] = ports
		ports = dict({})
