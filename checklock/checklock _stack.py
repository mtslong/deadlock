#!/usr/bin/env python

#demo

import os

def scan_file(filename):
	try:
		#print "filename:=", filename
		fd = open(filename,'r')
		i=0
		for s in fd.xreadlines():
			i = i + 1
			s=s.strip()
			#print i, "-->" , s
			parseline(s)
		#print "file.line.count=",i
	except Exception, e:
		raise e
	finally:
		fd.close()
	return

def parseline(linedata):
	tmp=linedata.split("\t")
	#print tmp
	global callgraph

	if tmp[1]=='E':
		if callgraph.has_key(tmp[0])==False:
			callgraph[tmp[0]]=[tmp[3]]
		else:
			callgraph.get(tmp[0]).append(tmp[3])
			#print callgraph
		
		callstack.append(tmp[3])	

	return

def write_dot2(filename):
	try:
		print "filename:=", filename
		fd = open(filename,'w')
		wstr="digraph trace{\n"
		wstr=wstr+"rankdir=TB;\n"
		fd.write(wstr)
		
		kv=callgraph.keys() 
		for keys in kv:
			#print "keys=",keys
			wstr="\nedge [color=" + colorlist[kv.index(keys)] + "];\n"
			fd.write(wstr)

			head=""
			tmp=""
			clist=callgraph.get(keys) 
			tmplist=clist[:]
			prelist=[]
			wstr=""
			print clist	
			while len(tmplist)>0:
				tmp=tmplist.pop(0)
				#print tmp
				if tmp in thread_list:
					if len(tmplist)<=0:
						print tmp,";\n"
						wstr=tmp + ";\n"
						fd.write(wstr)
						break
					else:
						head=tmp

				if tmp in lock_map:
					print head,"->", tmp, ";" 
					wstr=head + "->" + tmp + ";\n"
					fd.write(wstr)
					prelist.append(head)
					head=tmp

				if tmp in unlock_map:
					print "debug:", tmp," dict[]=", lock_dict[head]
					if tmp==lock_dict[head]:
						print "debug:prelist=", prelist
						head=prelist.pop()
					else:
						for dk,dv in lock_dict.items():
							if dv==tmp: 
								prelist.remove(dk)
								break
					

			#print clist

		fd.write("}\n")
		
	except Exception, e:
		raise e
	finally:
		fd.close()
	return




def write_dot(filename):
	try:
		print "filename:=", filename
		fd = open(filename,'w')
		wstr="digraph trace{\n"
		wstr=wstr+"rankdir=TB;\n"
		fd.write(wstr)
		
		kv=callgraph.keys() 
		for keys in kv:
			#print "keys=",keys
			wstr="\nedge [color=" + colorlist[kv.index(keys)] + "];\n"
			fd.write(wstr)

			i=0
			head=""
			clist=callgraph.get(keys) 
			print clist
			#for tmp in clist:
			while i<len(clist):
				tmp=clist[i]
				print i,":tmp=" + tmp 
				if tmp in thread_list:
					if len(clist)>1:
						fd.write(tmp + "->")
					else:
						fd.write(tmp + ";\n")


				if tmp in lock_map:
					head_index=i #this is head
					print tmp, " index=", head_index
					#tail_index=get_unlock(head_index,clist)
					tail_index=0
					k=head_index
					while k<len(clist):
						#print " lock_dict[k]=", lock_dict[tmp]
						if clist[k]==lock_dict[tmp]:
							tail_index=k  #this is tail
							break
						k=k+1
					print "head=" ,head_index, "=", clist[head_index]
					print "tail=" ,tail_index, "=", clist[tail_index]
					#write relation in [head,tail]	
					wstr=clist[head_index]
					m=head_index+1
					while m<=tail_index:
						if clist[m] in lock_map:
							wstr=wstr + "->" + clist[m] + ";\n"
						else:
							wstr=wstr + ";\n"
						m=m+1
					fd.write(wstr)
					print wstr

				if tmp in unlock_map:
					print "unlock=",tmp  
				i=i+1

			
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
	#test()

	callgraph={}
	callstack=[]
	lock_dict={\
		"locka":"unlocka",\
		"lockb":"unlockb",\
		"lockc":"unlockc",\
		"lockd":"unlockd",\
	}
	lock_map=lock_dict.keys()
	print "lock_map=",lock_map
	unlock_map=["unlocka","unlockb","unlockc","unlockd"]
	thread_list=["main","thread1","thread2","thread3"]
	colorlist=["red","blue","black","green","cyan"]

	scan_file("trace.txt")

	print "callstack: ", callstack 
	print "callgraph: ", callgraph
	
	write_dot2("callgraph.dot")

	pass
	