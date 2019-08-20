import networkx as nx
import random as rnd
import math
import os
import sys

if len(sys.argv) != 3:
	print ("Usage: python gen-BA.py numInst numVert")
	exit(1)

try:
	numInst = int(sys.argv[1])
	numVert = int(sys.argv[2])
except: 
	print ("Error in parsing arguments")
	exit(2)

directory = './instancias/B-' + str(numVert) + '/'

try: 
	if not os.path.exists(directory):
		os.makedirs(directory)
except OSError as e:
	if e.errno != errno.EEXIST:
		raise
	
try: 
	for i in range(numInst):
		# Generate degree sequence
		m = rnd.randint(1, numVert - 1)
		filename = directory + 'in' + str(i)
		f = open(filename, 'w')
		G = nx.barabasi_albert_graph(numVert, m)
		# print the graph to the file
		f.write( str(G.number_of_nodes()) + ' ' + str(G.number_of_edges()) + '\n' )
		for (u, v) in G.edges():
			f.write(str(u) + ' ' + str(v) + '\n')
		f.close()

except:
	print ("Error while generating the graphs")


