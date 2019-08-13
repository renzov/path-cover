import networkx as nx
import random
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

directory = './instancias/C' + str(numVert) + '-P' + str(p) + '/'

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
		G = nx.random_tree(numVert)
		print ("Generated")
		N = numVert
		# adjMatrix = [ [0 for x in range(N)] for y in range(N) ]
		# Mark the used edges
		# for (u, v) in G.edges():
		# 	adjMatrix[u][v] = 1

		# E = []
		# for u in range(N):	
		# 	for v in range(N):
		#		if adjMatrix[u][v] != 1:
		#			E.append( (u, v) )
		
		# random.shuffle(E)
		# idx = 0
		# while (prob * N * (N - 1)) > 2*G.number_of_edges():
		# 	(u, v) = E[idx]
		# 	idx = idx + 1
		# 	G.add_edge(u,v)

		# print the graph to the file
		f.write( str(G.number_of_nodes()) + ' ' + str(G.number_of_edges()) + '\n' )
		print ("first line")
		for (u, v) in G.edges():
			f.write(str(u) + ' ' + str(v) + '\n')
		f.close()

except:
	print ("Error while generating the graphs")


