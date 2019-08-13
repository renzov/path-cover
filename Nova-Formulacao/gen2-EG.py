import networkx as nx
import os
import sys

if len(sys.argv) != 4:
	print ("Usage: python gen2-EG.py numInst numVert p")
	exit(1)

try:
	numInst = int(sys.argv[1])
	numVert = int(sys.argv[2])
	p = int(sys.argv[3])
except: 
	print ("Error in parsing arguments")
	exit(2)

directory = './instancias/' + str(numVert) + '-P' + str(p) + '/'

try: 
	if not os.path.exists(directory):
		os.makedirs(directory)
except OSError as e:
	if e.errno != errno.EEXIST:
		raise
	
try: 
	prob = p / 100
	for i in range(numInst):
		filename = directory + 'in' + str(i)
		f = open(filename, 'w')
		G = nx.gnp_random_graph(numVert, prob)
		# print the graph to the file
		f.write( str(G.number_of_nodes()) + ' ' + str(G.number_of_edges()) + '\n' )
		for (u, v) in G.edges():
			f.write(str(u) + ' ' + str(v) + '\n')
		f.close()

except:
	print ("Error while generating the graphs")


