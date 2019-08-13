#include<iostream>
#include<sstream>
#include<set>
#include<cstdlib>
#include<algorithm>
#include<vector>
#include<cassert>
#include<ctime>
#include "gurobi_c++.h"
using namespace std;

const int MAXV = 1024;
const int MAXE = 500000;

typedef pair<int,int> edge;

vector<int> G[MAXV]; // List of the edges adjacent to a vertex (by index in E)
vector<edge> E;		 // List of edges
int N; 				 // Number of vertices
int M;				 // Number of edges
/* Variables for cycle detection */
int tour[MAXV];
bool visited[MAXV];
int nTour;
/*
	Format of the input 
		N M - where N = Num. of vertices, M = Number of edges
		List of M edges given in the format "u v" 
*/
string itos(int i){ stringstream s; s << i; return s.str(); }

void print_solution( double *sol ){
	for ( int i=0; i < (M+2*N); ++i ){
		if ( sol[i] > 0.5 ) // if edge belongs to the support
			printf("%d %d\n", E[i].first, E[i].second);
	}
	printf("-1 -1\n"); // Signals end of edges in solution
}


void dfs( int u, double *x ){
	visited[u] = true;
	tour[nTour++] = u;
	int v, e;
	for ( int i=0; i < (int)G[u].size(); i++ ){
		e = G[u][i];
		if ( x[ e ] > 0.5 ){
			v = ( E[e].first == u )? E[e].second:E[e].first;
			if ( !visited[v] ){
				dfs(v, x );
			}
		}
	}
}

bool check_subtours( double *x, vector< vector<int> > &cycles ){
	fill( visited, visited + (N + 2), false );
	nTour = 0;
	cycles.clear();
	dfs( N, x ); // dfs from s
	if ( !visited[ N+1 ] && nTour >= 3 ){ // s is inside a cycle
		cycles.push_back( vector<int>( tour, tour + nTour ) );
	}
	for ( int i=0; i < N; ++i ){
		if (!visited[i]){
			nTour = 0;
			dfs( i, x );
			if ( nTour >= 3 ){
				cycles.push_back( vector<int>( tour, tour + nTour ) );
			}
		}
	}
	return (int)cycles.size() == 0;	
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
				vars[i] = model.addVar( 0.0, 1.0, 1.0, GRB_BINARY, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );
			}
			else {
				vars[i] = model.addVar( 0.0, 1.0, 0.0, GRB_BINARY, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );			
			}
		}
		model.update();
		// Adding 2-degree constraints
		for ( u=0; u < N; ++u ){
			GRBLinExpr expr = 0;
			for ( int i=0; i < (int)G[u].size(); ++i ) expr += vars[ G[u][i] ];
			model.addConstr( expr == 2, "2_degree_" + itos(u) );			
		}
		// Adding "no trivial path" constraint
		int idx;
		for ( u=0; u < N; ++u ){
			GRBLinExpr expr = 0;
			for ( int i=0; i < (int)G[u].size(); ++i ){ 
				idx = G[u][i];
				if ( E[idx].first >= N || E[idx].second >= N ) // s or t
					expr += vars[ idx ];	
			}
			model.addConstr( expr <= 1, "trivial_" + itos(u) );			
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
		vector< vector<int> > subtours;
		set<int> induced;
		bool S[N + 5];	
		int size;
		bool error = false;
		clock_t begin = clock(); // Measuring time
		clock_t end;
		while ( !ok ){
			model.optimize();		
			if (model.get(GRB_IntAttr_SolCount) > 0 ){
				sol = model.get(GRB_DoubleAttr_X, vars, M + 2*N);
				if ( check_subtours( sol, subtours ) ){
					end = clock(); // Finishing time
					cout << model.get(GRB_DoubleAttr_ObjVal) << endl;
					print_solution( sol );
					ok = true;
				}
				else {
					for ( int i=0; i < (int) subtours.size(); ++i ){
						GRBLinExpr expr = 0;
						vector<int> V = subtours[i];
						size = (int) V.size();
						fill( S, S + (N + 2), false );
						for ( int j=0; j < (int)V.size(); ++j ) S[ V[j] ] = true;
						for ( int j=0; j < (int)V.size(); ++j ){
							u = V[j];
							for ( int k=0; k < (int)G[u].size(); ++k ){ 
								if ( S[ E[ G[u][k] ].first ] && S[ E[ G[u][k] ].second ] )
									induced.insert( G[u][k] );
							}
						}
						for ( set<int>::iterator e=induced.begin(); e != induced.end(); ++e ){
							expr += vars[*e];
						}
						model.addConstr( expr <= (size-1) );
						induced.clear();
					}
					model.update();
				}
			}
			else {
				end = clock(); // Finishing time
				error = true;
				cout << -1 << endl; // Signals no solution found
				ok = true;
			}
		}
  		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		//if ( !error ){
		cout << elapsed_secs << endl;
		//}
		delete[] sol;
	}
	catch (GRBException ex){
		cout << -2 << endl; // Signals unexpected error ocurred
		//cout << "Error numnber: " << ex.getErrorCode() << endl;
		//cout << ex.getMessage() << endl;
	}
	catch (...){
		cout << -3 << endl;
	}
	delete[] vars;
	delete env;
	return 0;
}

