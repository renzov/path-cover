import networkx as nx

# G = nx.barbell_graph(40, 7)
# G = nx.circular_ladder_graph(50)
# G = nx.wheel_graph(71)
# G = nx.wheel_graph(88)
# G = nx.chvatal_graph()
# G = nx.cubical_graph()
# G = nx.desargues_graph()
# G = nx.frucht_graph()
# G = nx.icosahedral_graph()
# G = nx.krackhardt_kite_graph()
# G = nx.moebius_kantor_graph()
# G = nx.octahedral_graph()
# G = nx.sedgewick_maze_graph()
# G = nx.petersen_graph()
# G = nx.tutte_graph()
# G = nx.pappus_graph() 

print ('%d %d'%(G.number_of_nodes(), G.number_of_edges()))

for (u,v) in G.edges():
	print ('%d %d'%(u,v))

