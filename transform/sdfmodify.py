#! /usr/bin/env python


actors = dict({})
actor_times = dict({})
channels = dict({})
channel_sizes = dict({})

def find_pid(ptype,port_name,ports):
	for key, values in ports.iteritems():
		if (ptype == values['type'] and port_name == values['name']):
			return values.get('id',-1)
	return 'error no port id'

def port_name(ptype,pid,ports):
	for key, values in ports.iteritems():
		if (ptype == values['type'] and pid == values.get('id',-1)):
			return values['name']
	return 'error no port name with type: '+ptype+' and pid: '+str(pid)+' in '+str(ports)

def port_rate(ptype,pid,ports):
	for key, values in ports.iteritems():
		if (ptype == values['type'] and pid == values.get('id',-1)):
			return int(values['rate'])
	return 'error no port rate'
	

def find_writer(dst_actor,dst_port,channels):
	for channel, attrs in channels.iteritems():
		if (dst_actor == attrs["dstActor"] and dst_port == attrs['dstPort']):
			return (attrs['srcActor'],attrs['srcPort'])
	for channel, attrs in channels.iteritems():
		print (attrs["dstActor"]+' '+attrs['dstPort'])
	return ('no writer '+dst_actor+' '+dst_port,'error')
	

def find_reader(src_actor,src_port,channels):
	for channel, attrs in channels.iteritems():
		if (src_actor == attrs["srcActor"] and src_port == attrs['srcPort']):
			return (attrs['dstActor'],attrs['dstPort'])
	return ('no reader','error')

def modify(a_, at_, c_, cs_, read_delay, write_delay):
	# global actors, actor_times, channels, channel_sizes, new_actors, new_actor_times, new_channels, new_channel_sizes
	actors = dict(a_)
	actor_times = dict(at_)
	channels = dict(c_)
	channel_sizes = dict(cs_)

	maxsize = str(max([int('0'+x) for x in channel_sizes.values()]+[0]))

	new_actors = dict({})
	new_actor_times = dict({})
	new_channels = dict({})
	new_channel_sizes = dict({})

	### add the readers and writers ###	
	actors_port_count = dict({})
	### at the beginning there are no readers and writers for each actor ###
	for actor in a_.keys():
		actors_port_count[actor] = dict(readers=0, writers=0) 

	### for every channel that is not a self edge ###
	### we find the propper rank (id) in the loop ffor it's ports ###
	for channel, attrs in c_.iteritems():
		if (attrs["srcActor"] == attrs["dstActor"]):
			continue

		### get this channels info ###
		src_actor = attrs["srcActor"]
		dst_actor = attrs["dstActor"]
		src_port = attrs["srcPort"]
		dst_port = attrs["dstPort"]

		# a_[src_actor][src_port]['id'] = -1
		# a_[dst_actor][dst_port]['id'] = -1

		if attrs.get('initialTokens',False):
			a_[dst_actor][dst_port]['init'] = attrs['initialTokens'] 

		### increment our counters ###
		actors_port_count[src_actor]["writers"] += 1
		actors_port_count[dst_actor]["readers"] += 1

	### set the sequence order
	for actor, ports in a_.iteritems():
		for name,port in ports.iteritems():
			if port.get('id',-2)==-2:
				# print 'some ports have missing ids'
				pass
			else:
				port['id']=int(port['id'])


	### now that we have the ranks we construct the main loop ###
	### we create a new actor set to replace the main actors in the transformed sdf ###
	for actor in actors.keys():
		port_count = actors_port_count[actor]

		if port_count['readers'] > 0 :
			### first insert reader actors ###
			for count in range(port_count['readers']):

				new_actor = actor + "_reader_" + str(count)

				###calculating exec time ###
				exectime = read_delay * port_rate('in',count,actors[actor])

				new_actor_times[new_actor]=str(exectime)

				new_ports={}

				### self loop ports ###
				new_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
				new_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
				channel_name = new_actor + "_self"
				channel_val = {'name':channel_name,'srcActor':new_actor,'srcPort':'pOut','dstActor':new_actor,'dstPort':'pIn','initialTokens':"1"}
				new_channels[channel_name] = channel_val
				new_channel_sizes[channel_name]=maxsize

				### the read port and channel ###
				new_ports['pRead']={'name':'pRead','type':'in','rate':str(port_rate('in',count,actors[actor]))}
				dst_actor = actor
				dst_port = port_name('in',count,actors[actor])
				src_actor, src_port = find_writer(dst_actor,dst_port,channels)
				new_src_actor = src_actor + "_writer_" + str(find_pid('out',src_port,actors[src_actor]))
				channel_name = new_src_actor + '__link_to__' + new_actor
				channel_val = {'name':channel_name,'srcActor':new_src_actor,'srcPort':'pWrite','dstActor':new_actor,'dstPort':'pRead'}
				if (actors[dst_actor][dst_port]).get('init',False):
					channel_val['initialTokens']=actors[dst_actor][dst_port]['init']
				new_channels[channel_name] = channel_val
				new_channel_sizes[channel_name]=maxsize

				### back loop construction ###
				if count == 0:
					### check for back actor in loop ###
					if (port_count['writers'] > 0):
						back_loop_actor = actor + "_writer_" + str(port_count['writers']-1)

						### no sync needed ###
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward','initialTokens':'1'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

					### in case there are no writers ###
					elif port_count['readers'] == 1 :
						### only one read exists ###
						back_loop_actor = new_actor
						### place main actor as a sync thread ###
						### create sync thread (main actor) ###
						sync_name = 'main_actor_from__'+back_loop_actor+"__to__"+new_actor
						sync_ports={}
						sync_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + sync_name
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':sync_name,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = sync_name + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward','initialTokens':'1'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
						sync_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
						channel_name = sync_name + "_self"
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pOut','dstActor':sync_name,'dstPort':'pIn','initialTokens':"1"}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						new_actors[sync_name]=sync_ports
						subtraction = (read_delay * sum(i for i in [port_rate('in',count,actors[actor]) for count in range(port_count['readers'])]) +
										write_delay * sum(i for i in [port_rate('out',count,actors[actor]) for count in range(port_count['writers'])]))
						new_actor_times[sync_name]=str(int(actor_times[actor])-subtraction)
						#############################
					else:
						### should be looping to the last reader ###
						back_loop_actor = actor + "_reader_" + str(port_count['readers']-1)
						### place main actor as a sync thread ###
						### create sync thread (main actor) ###
						sync_name = 'main_actor_from__'+back_loop_actor+"__to__"+new_actor
						sync_ports={}
						sync_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + sync_name
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':sync_name,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = sync_name + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward','initialTokens':'1'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
						sync_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
						channel_name = sync_name + "_self"
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pOut','dstActor':sync_name,'dstPort':'pIn','initialTokens':"1"}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						new_actors[sync_name]=sync_ports
						subtraction = (read_delay * sum(i for i in [port_rate('in',count,actors[actor]) for count in range(port_count['readers'])]) +
										write_delay * sum(i for i in [port_rate('out',count,actors[actor]) for count in range(port_count['writers'])]))
						new_actor_times[sync_name]=str(int(actor_times[actor])-subtraction)
				else:
					### should be looping back to previous reader ###
					back_loop_actor = actor + "_reader_" + str(count-1)
					### no sync needed ###
					new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
					channel_name = back_loop_actor + '__to__' + new_actor
					channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward'}
					new_channels[channel_name] = channel_val
					new_channel_sizes[channel_name]=maxsize

				### front loop connection ###
				new_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
				new_actors[new_actor] = new_ports
		

		if port_count['writers'] > 0 :
			# main_actor_deployed = True
			### now insert writer actors ###
			for count in range(port_count['writers']):

				new_actor = actor + "_writer_" + str(count)

				###calculating exec time ###
				exectime = write_delay * port_rate('out',count,actors[actor])

				new_actor_times[new_actor]=str(exectime)

				new_ports={}

				### self loop ports ###
				new_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
				new_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
				channel_name = new_actor + "_self"
				channel_val = {'name':channel_name,'srcActor':new_actor,'srcPort':'pOut','dstActor':new_actor,'dstPort':'pIn','initialTokens':"1"}
				new_channels[channel_name] = channel_val
				new_channel_sizes[channel_name]=maxsize

				### the write port ###
				new_ports['pWrite']={'name':'pWrite','type':'out','rate':str(port_rate('out',count,actors[actor]))}

				### back loop construction ###
				if count == 0:
					### check for back actor in loop ###
					if (port_count['readers'] > 0):
						back_loop_actor = actor + "_reader_" + str(port_count['readers']-1)
						### place main actor as a sync thread ###
						### create sync thread (main actor) ###
						sync_name = 'main_actor_from__'+back_loop_actor+"__to__"+new_actor
						sync_ports={}
						sync_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + sync_name
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':sync_name,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = sync_name + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
						sync_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
						channel_name = sync_name + "_self"
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pOut','dstActor':sync_name,'dstPort':'pIn','initialTokens':"1"}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						new_actors[sync_name]=sync_ports
						subtraction = (read_delay * sum(i for i in [port_rate('in',count,actors[actor]) for count in range(port_count['readers'])]) +
										write_delay * sum(i for i in [port_rate('out',count,actors[actor]) for count in range(port_count['writers'])]))
						new_actor_times[sync_name]=str(int(actor_times[actor])-subtraction)

					### in case there are no readers ###
					elif port_count['writers'] == 1 :
						### only one write exists ###
						back_loop_actor = new_actor
						### place main actor as a sync thread ###
						### create sync thread (main actor) ###
						sync_name = 'main_actor_from__'+back_loop_actor+"__to__"+new_actor
						sync_ports={}
						sync_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + sync_name
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':sync_name,'dstPort':'pBackward','initialTokens':'1'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = sync_name + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
						sync_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
						channel_name = sync_name + "_self"
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pOut','dstActor':sync_name,'dstPort':'pIn','initialTokens':"1"}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						new_actors[sync_name]=sync_ports
						subtraction = (read_delay * sum(i for i in [port_rate('in',count,actors[actor]) for count in range(port_count['readers'])]) +
										write_delay * sum(i for i in [port_rate('out',count,actors[actor]) for count in range(port_count['writers'])]))
						new_actor_times[sync_name]=str(int(actor_times[actor])-subtraction)
						#############################
					else:
						### should be looping to the last writer ###
						back_loop_actor = actor + "_writer_" + str(port_count['writers']-1)
						### place main actor as a sync thread ###
						### create sync thread (main actor) ###
						sync_name = 'main_actor_from__'+back_loop_actor+"__to__"+new_actor
						sync_ports={}
						sync_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = back_loop_actor + '__to__' + sync_name
						channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':sync_name,'dstPort':'pBackward','initialTokens':'1'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
						new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
						channel_name = sync_name + '__to__' + new_actor
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward'}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						sync_ports['pIn']={'name':'pIn','type':'in','rate':'1'}
						sync_ports['pOut']={'name':'pOut','type':'out','rate':'1'}
						channel_name = sync_name + "_self"
						channel_val = {'name':channel_name,'srcActor':sync_name,'srcPort':'pOut','dstActor':sync_name,'dstPort':'pIn','initialTokens':"1"}
						new_channels[channel_name] = channel_val
						new_channel_sizes[channel_name]=maxsize

						new_actors[sync_name]=sync_ports
						subtraction = (read_delay * sum(i for i in [port_rate('in',count,actors[actor]) for count in range(port_count['readers'])]) +
										write_delay * sum(i for i in [port_rate('out',count,actors[actor]) for count in range(port_count['writers'])]))
						new_actor_times[sync_name]=str(int(actor_times[actor])-subtraction)
				else:
					### should be looping back to previous writer ###
					back_loop_actor = actor + "_writer_" + str(count-1)
					### no sync needed ###
					new_ports['pBackward']={'name':'pBackward','type':'in','rate':'1'}
					channel_name = back_loop_actor + '__to__' + new_actor
					channel_val = {'name':channel_name,'srcActor':back_loop_actor,'srcPort':'pForward','dstActor':new_actor,'dstPort':'pBackward'}
					new_channels[channel_name] = channel_val
					new_channel_sizes[channel_name]=maxsize

				### front loop connection ###
				new_ports['pForward']={'name':'pForward','type':'out','rate':'1'}
				new_actors[new_actor] = new_ports

	### return the transformed graph ###
	return new_actors,new_actor_times,new_channels,new_channel_sizes
