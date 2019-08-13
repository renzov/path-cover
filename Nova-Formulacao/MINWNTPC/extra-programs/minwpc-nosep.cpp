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
				ok = true;
				//print_solution( sol );
				for ( int i=0; i < M; ++i ){
					cout <<  E[i].first << " " << E[i].second << " " << sol[i] << endl;
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

