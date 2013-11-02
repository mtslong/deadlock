#!/usr/bin/env python

#demo

import os



def scan_file(filename):
	try:
		print "filename:=", filename
		fd = open(filename,'r')
		i=0
		for s in fd.xreadlines():
			i = i + 1
			s=s.strip()
			#print i, "-->" , s
			parseline(s)
		print "file.line.count=",i
	except Exception, e:
		raise e
	finally:
		fd.close()
	return

def parseline(linedata):
	tmp=linedata.split("\t")
	print tmp
	global callgraph

	if tmp[1]=='E':
		if callgraph.has_key(tmp[0])==False:
			callgraph[tmp[0]]=[tmp[3]]
		else:
			callgraph.get(tmp[0]).append(tmp[3])
			#print callgraph
		
		callstack.append(tmp[3])	

	return

def write_dot(filename):
	try:
		print "filename:=", filename
		fd = open(filename,'w')
		fd.write("digraph trace{\n")
		fd.write("node [shape=box];edge [color=black style=solid];\n")
		for keys in callgraph.keys():
			print keys
			i=0
			head=""
			fd.write("\n")	
			for value in callgraph.get(keys):
				#print "value=" + value
				i=i+1
				if i==1:
					head=value								
				else:
					print head, "->", value
					wd=head+ "->"+ value+"\n"
					fd.write(wd)
					head=value
			
		fd.write("}\n")
		
	except Exception, e:
		raise e
	finally:
		fd.close()
	return


def test():
	print "curdir=" , os.path.abspath(os.curdir)
	return
	

if __name__ == '__main__':
	#demo
	test()

	callgraph={}
	callstack=[]

	scan_file("trace-967.txt")

	print "callstack: ", callstack 
	print "callgraph: ", callgraph
	
	write_dot("callgraph.dot")

	pass
	