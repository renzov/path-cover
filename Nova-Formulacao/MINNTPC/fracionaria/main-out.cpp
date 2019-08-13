#include<iostream>
#include<sstream>
#include<set>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<cassert>
#include<ctime>
#include "gurobi_c++.h"
#include "mincut.hpp"
using namespace std;

typedef pair<int,int> edge;

vector<int> G[MAXN]; // List of the edges adjacent to a vertex (by index in E)
vector<edge> E;		 // List of edges
int N; 				 // Number of vertices
int M, m;			 // Number of edges of G, and total
/* For separation algorithm */
double C[MAXN][MAXN];

int transform( int x ){ 
	if ( x < N ) return x;
	return x - N;
}

void create_cost_matrix( double *x ){
	for ( int i=0; i< (N+2); ++i )
		for ( int j=0; j < (N+2); ++j )
			C[i][j] = 0.0;
	int u, v;
	for ( int i=0; i < m; ++i ){
		if ( x[i] > EPS ){
			u = transform( E[i].first ); 
			v = transform( E[i].second );
			C[ u ][ v ]++; 
			C[ v ][ u ]++;
		}
	}
}
/*
	Format of the input 
		N M - where N = Num. of vertices, M = Number of edges
		List of M edges given in the format "u v" 
*/
string itos(int i){ stringstream s; s << i; return s.str(); }

void print_solution( double *sol ){
	for ( int i=0; i < m; ++i ){
		if ( sol[i] > EPS )
			printf("%d %d %f\n", E[i].first, E[i].second, sol[i]);
	}
	printf("-1 -1 -1\n");
}

int main(){
	int u, v; 
	int s, t;
	scanf("%d %d", &N, &M);
	assert( (2*N + 2) <= MAXN );
	E = vector<edge> (3*M + 2*N);
	s = 2*N;
	t = 2*N + 1;
	for ( int i=0; i < M; ++i ){
		scanf("%d %d", &u, &v);
		assert( u >= 0 && u < N ); assert( v >= 0 && v < N ); 
		G[u].push_back(i), G[v].push_back(i);
		E[i] = edge(u, v);
	}
	// Adding edges from s to copy of v
	for ( int i=M; i < (M+N); ++i ){
		G[s].push_back(i), G[i-M+N].push_back(i);
		E[i] = edge( s, i-M+N );
	}
	// Adding edges from v to the t
	for ( int i=M+N; i < (M+2*N); ++i ){
		G[t].push_back(i), G[i-M-N].push_back(i);
		E[i] = edge( t, i - M - N );
	}
	// Edges from copy to the adjacents in G
	m = M + 2*N;
	for ( int i=0; i < M; ++i ){
		u = E[i].first, v = E[i].second;
		G[u+N].push_back(m), G[v].push_back(m);
		E[m++] = edge( u+N, v );
		G[u].push_back(m), G[v+N].push_back(m);
		E[m++] = edge( u, v+N );		
	}
	assert( m == (3*M + 2*N) );
	/* Creating the model */
	GRBEnv *env = NULL;
	GRBVar *vars = NULL;
	vars = new GRBVar[m];
	try {
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);		
		// Creating the variables
		for ( int i=0; i < m; ++i ){
			if ( E[i].first == s || E[i].second == s ){
				vars[i] = model.addVar( 0.0, 1.0, 1.0, GRB_CONTINUOUS, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );
			}
			else {
				vars[i] = model.addVar( 0.0, 1.0, 0.0, GRB_CONTINUOUS, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );			
			}
		}
		model.update();
		// Adding 2-deg constraints
		for ( int i=0; i < N; ++i ){
			GRBLinExpr expr = 0;
			for ( int j=0; j < (int)G[i].size(); ++j ) expr += vars[ G[i][j] ];
			for ( int j=0; j < (int)G[i+N].size(); ++j ) expr += vars[ G[i+N][j] ];
			model.addConstr( expr == 2, "2_degree_" + itos(i) );						
		}
		for ( int i=M; i < (M+N); ++i ){
			GRBLinExpr expr3 = 0;
			u = E[i].second;
			for ( int j=0; j < (int)G[u].size(); ++j ){
				if ( G[u][j] != i ){ 
					expr3 += vars[ G[u][j] ];
				}
			}
			expr3 -= vars[ i ];
			model.addConstr( expr3 == 0, "block-s-" + itos(u) );			
			u = E[i].second - N; // Vertex in G
			for ( int j=0; j < (int)G[u].size(); ++j ){
				GRBLinExpr expr = 0;
				expr += vars[ G[u][j] ];
				expr += vars[ i ];
				model.addConstr( expr <= 1, "block-s-" + itos(u) );
			}
		}
		// st-flow
		GRBLinExpr expr2 = 0;
		for ( int i=0; i < (int)G[s].size(); ++i ) expr2 += vars[ G[s][i] ];
		for ( int i=0; i < (int)G[t].size(); ++i ) expr2 -= vars[ G[t][i] ];
		model.addConstr( expr2 == 0, "st-flow" );
		model.update();
		// Optimization
		bool ok = false;
		double *sol = new double[m];
		MinCut Alg;
		bool S[2*N + 5];
		double minc;
		bool error = false;
		clock_t begin = clock(); // Measuring time
		clock_t end;
		while ( !ok ){
			model.optimize();		
			if (model.get(GRB_IntAttr_SolCount) > 0 ){
				sol = model.get(GRB_DoubleAttr_X, vars, m);
				// Make a contraction of the vertex and the copy
				create_cost_matrix( sol );
				// Separate
				fill( S, S + (2*N + 2), false );
				Alg.min_cut( N + 2, C, minc, S );
				if ( minc >= (1-EPS) ){
					end = clock(); // Finishing time
					cout << model.get(GRB_DoubleAttr_ObjVal) << endl;
					print_solution( sol );
					ok = true;
				}
				else {
					// Take care of the contraction
					if ( S[N] ) S[s] = true, S[N] = false;
					if ( S[N+1] ) S[t] = true, S[N+1] = false;
					for ( int i=0; i < N; ++i ) S[i+N] = S[i];
					GRBLinExpr expr = 0;
					for ( int j=0; j < m; ++j ){
						if ( S[ E[j].first ] ^ S[ E[j].second ] ){
							expr += vars[j];
						}
					}
					model.addConstr( expr >= 1 );
					model.update();
				}
			}
			else {
				error = true;
				cout << "Error: Solution not found." << endl;
				ok = true;
			}	
		}
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		if ( !error ){
			cout << elapsed_secs << endl;
		}
		delete [] sol;
	}
	catch (GRBException ex){
		cout << "Error numnber: " << ex.getErrorCode() << endl;
		cout << ex.getMessage() << endl;
	}
	catch (...){
		cout << "Error during optimization" << endl;
	}
	delete[] vars;
	delete env;
	return 0;
}
