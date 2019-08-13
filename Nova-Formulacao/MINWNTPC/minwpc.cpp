#include<cstdio>
#include<iostream>
#include<algorithm>
#include<ctime>
#include<cassert>
#include<sstream>
#include<vector>
#include "gurobi_c++.h"
#include "blossom.hpp"
using namespace std;

typedef pair<int, int> edge;

int N, M;
vector<int> G[MAXN];
vector<edge> E;	/* List of edges */
vector<int> W;	/* Edge weights */
double C[MAXN][MAXN];
double P[MAXN][MAXN];
bool odd[MAXN];
Separation Alg;

string itos(int i){ stringstream s; s << i; return s.str(); }

void determine_odd_vertices( double *sol, bool *great ){
	int cnt, e;
	/* Mark edges with X[e] > 1/2 */
	for ( int i=0; i < M; ++i ){
		if ( Alg.comp(sol[i], 0.5) > 0 ) great[i] = true;	
		else great[i] = false;
	}
	/* Mark odd vertices */
	for ( int i=0; i < N; ++i ){
		cnt = 0;
		for ( int j=0; j < (int) G[i].size(); ++j ){
			e = G[i][j];
			if ( great[e] ) cnt++;	
		}
		if ( cnt & 1 ) odd[i] = true;
		else odd[i] = false;
	}
	odd[N] = false;
}

void create_capacity_matrix( double *sol ){
	int u, v;
	
	for ( int i=0; i < N; ++i ){
		fill( C[i], C[i] + N, 0.0 );
		fill( P[i], P[i] + N, 0.0 );
	}
	
	for ( int i=0; i < M; ++i ){
		if ( Alg.comp( sol[i] , 0.0 ) > 0 ){
			u = E[i].first, v = E[i].second;
			C[u][v] = C[v][u] = min( sol[i], 1 - sol[i] );
			P[u][v] = P[v][u] = sol[i];
		}
	}
	for ( int i=0; i < N; ++i ) C[i][N] = C[N][i] = 2.0;
	for ( int i=0; i < M; ++i ){
		u = E[i].first, v = E[i].second;
		C[u][N] -= sol[i]; C[v][N] -= sol[i];
		C[N][u] -= sol[i]; C[N][v] -= sol[i];		
	}
	C[N][N] = 0.0;
}

void print_solution( double sol[] ){
	puts("Solution found:");
	for ( int i=0; i<M; ++i ){
		if ( Alg.comp( sol[i], 0.0 ) > 0 ){
			printf("%d %d %f\n", E[i].first, E[i].second, sol[i]);
		}
	}
}

int main(){
	int u, v, w;
	// Read the graph
	scanf("%d %d", &N, &M);
	for ( int i=0; i < M; ++i ){
		//scanf("%d %d %d", &u, &v, &w);
		scanf("%d %d", &u, &v);
		w = 1;
		E.push_back(edge(u,v));
		G[u].push_back(i), G[v].push_back(i);
		W.push_back(w);
	}
	/* Creating the model */	
	GRBEnv *env = NULL;
	GRBVar *vars = NULL;
	vars = new GRBVar[M];

	try {
		env = new GRBEnv();
		GRBModel model = GRBModel(*env);
		model.getEnv().set(GRB_IntParam_OutputFlag, 0);
		/* For maximization */
		// model.set(GRB_IntAttr_ModelSense, -1); 
		/* Creating the variables */
		for ( int i=0; i < M; ++i ){
			vars[i] = model.addVar( 0.0, 1.0, W[i]*1.0, GRB_CONTINUOUS, "x_" + itos(E[i].first) + "_" + itos(E[i].second) );
		}
		model.update();
		/* Adding the constraints */
		for ( u=0; u < N; ++u ){
			GRBLinExpr expr = 0;
			for ( int i=0; i < (int)G[u].size(); ++i ) expr += vars[ G[u][i] ];
			model.addConstr( expr <= 2, "2_degree_" + itos(u) );
			model.addConstr( expr >= 1, "1_degree_" + itos(u) );
		}
		model.update();
		/* Finding the solution */
		bool ok = false;
		bool error = false;
		double *sol = new double[M];
		bool *great = new bool[M];
		int *coef = new int[M];
		int added = 0;
		int rhs;
		while ( !ok ){
			model.optimize();		
			if (model.get(GRB_IntAttr_SolCount) > 0 ){
				sol = model.get(GRB_DoubleAttr_X, vars, M);	
				/* Determine odd vertices */
				determine_odd_vertices( sol, great );
				/* Create the capacity matrix */
				create_capacity_matrix( sol );
				/* Call separation algorithm */
				if ( Alg.separate( N, M, E, sol, C, P, odd, great, coef, rhs ) ){
					/* Add violated constrain - in the form a_1x_1 + ... + a_MX_M <= rhs */
					GRBLinExpr expr = 0;
					for ( int i=0; i < M; ++i ){
						if ( coef[i] == 1 ) expr += vars[i];
						else if ( coef[i] == -1 ) expr -= vars[i];
					}
					model.addConstr( expr <= rhs, "2-matching-" + itos(added) );
					model.update();
					added++;
				}
				else {
					ok = true;
					//print_solution( sol );
					for ( int i=0; i < M; ++i ){
						cout <<  E[i].first << " " << E[i].second << " " << sol[i] << endl;
					}
					/* Check if there is a non-integral solution */
					/*
					bool found = false;
					for ( int i=0; i < M && !found; ++i ){
						if ( Alg.comp( sol[i], 0.0 ) > 0 && Alg.comp( sol[i], 1.0 ) < 0 ){
							found = true;
						}
					}
					if ( found ) puts("Non-integral solution found");
					*/
				}
			}
			else {
				error = true;
				cout << "Error: Solution not found." << endl;
				ok = true;				
			}
		}
		delete[] great;
		delete[] sol;
		delete[] coef;
	}
	catch (GRBException ex){
		cout << ex.getMessage() << endl;
		puts("Excetion 1");
	}
	catch (...){
		puts("Exception 2");
	}
	/* Free memory */
	delete[] vars;
	delete env;

	return 0;
}

