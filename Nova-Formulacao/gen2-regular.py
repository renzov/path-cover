import networkx as nx
import os
import sys

if len(sys.argv) != 4:
	print ("Usage: python gen2-regular.py numInst numVert degree")
	exit(1)

try:
	numInst = int(sys.argv[1])
	numVert = int(sys.argv[2])
	K = int(sys.argv[3])
except: 
	print ("Error in parsing arguments")
	exit(2)

directory = './instancias/' + str(numVert) + '-R' + str(K) + '/'

try: 
	if not os.path.exists(directory):
		os.makedirs(directory)
except OSError as e:
	if e.errno != errno.EEXIST:
		raise
	
try: 
	for i in range(numInst):
		filename = directory + 'in' + str(i)
		f = open(filename, 'w')
		G = nx.random_regular_graph(K, numVert)
		# print the graph to the file
		f.write( str(G.number_of_nodes()) + ' ' + str(G.number_of_edges()) + '\n' )
		for (u, v) in G.edges():
			f.write(str(u) + ' ' + str(v) + '\n')
		f.close()

except:
	print ("Error while generating the graphs")

