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
int M;				 // Number of edges
/* For separation algorithm */
double C[MAXN][MAXN];
void create_cost_matrix( double *x ){
	for ( int i=0; i<(N+2); ++i )
		fill( C[i], C[i] + (N + 2), 0.0 );
	for ( int i=0; i < (M+2*N); ++i ){
		if ( x[i] > EPS )
			C[ E[i].first ][ E[i].second ] = C[ E[i].second ][ E[i].first ] = x[i];
	}
}

/*
	Format of the input 
		N M - where N = Num. of vertices, M = Number of edges
		List of M edges given in the format "u v" 
*/
string itos(int i){ stringstream s; s << i; return s.str(); }

void print_solution( double *sol ){
	for ( int i=0; i < (M+2*N); ++i ){
		if ( sol[i] > 0.0 )	// edges in the support of optimal value 
			printf("%d %d %f\n", E[i].first, E[i].second, sol[i]);
	}
	printf("-1 -1 -1\n"); // Signals the end of edges
}

int main(){
	int u, v;
	scanf("%d %d", &N, &M);
	E = vector<edge>(M + 2*N);
	for ( int i=0; i < M; ++i ){
		scanf("%d %d", &u, &v);
		E[i] = edge(u, v);
		G[u].push_back( i ), G[v].push_back( i );
	}
	// Vertices s = N and t = N + 1
	// Adding edges from s to every vertex
	int s = N, t = N + 1;
	for ( int i=M; i < (M+N); ++i ){
		E[i] = edge( s , i - M );
		G[s].push_back( i ), G[i-M].push_back( i );
	}
	// Adding edges from t to every vertex
	for ( int i=M+N; i < (M+2*N); ++i ){
		E[i] = edge( t , i - M - N );
		G[t].push_back( i ), G[ i - M - N ].push_back( i );
	}
	/* Creating the model */
	GRBEnv *env = NULL;
	GRBVar *vars = NULL;
	vars = new GRBVar[M + 2*N];
	try {
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);		
		// Creating the variables
		for ( int i=0; i < (M+2*N); ++i ){
			if ( E[i].first == s || E[i].second == s ){
				vars[i] = model.addVar( 0.0, 1.0, 1.0, GRB_CONTINUOUS, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );
			}
			else {
				vars[i] = model.addVar( 0.0, 1.0, 0.0, GRB_CONTINUOUS, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );			
			}
		}
		model.update();
		// Adding 2-degree constraints
		for ( u=0; u < N; ++u ){
			GRBLinExpr expr = 0;
			for ( int i=0; i < (int)G[u].size(); ++i ) expr += vars[ G[u][i] ];
			model.addConstr( expr == 2, "2_degree_" + itos(u) );			
		}
		// Adding "flow constraints" from s to t
		GRBLinExpr expr2 = 0;
		for ( int i=0; i < (int)G[s].size(); ++i ) expr2 += vars[ G[s][i] ];
		for ( int i=0; i < (int)G[t].size(); ++i ) expr2 -= vars[ G[t][i] ];
		model.addConstr( expr2 == 0, "st-flow" );
		model.update();
		// Finding a solution
		bool ok = false;
		double *sol = new double[M + 2*N];
		MinCut Alg;
		set<int> induced;
		bool S[N + 5];
		double minc;
		int size;
		bool error = false;
		clock_t begin = clock(); // Measuring time
		clock_t end;
		while ( !ok ){
			model.optimize();		
			if (model.get(GRB_IntAttr_SolCount) > 0 ){
				sol = model.get(GRB_DoubleAttr_X, vars, M + 2*N);
				// Find MinCut
				create_cost_matrix( sol );
				Alg.min_cut( N + 2, C, minc, S );
				if ( minc > (1.0-EPS) ){
					end = clock(); // Finishing time
					cout << model.get(GRB_DoubleAttr_ObjVal) << endl;
					print_solution( sol );
					ok = true;
				}
				else {
					GRBLinExpr expr = 0;
					for ( int j=0; j < (M+2*N); ++j ){
						if ( S[ E[j].first ] ^ S[ E[j].second ] )
							expr += vars[j];
					}
					model.addConstr( expr >= 1 );
					induced.clear();
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
		delete[] sol;
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
