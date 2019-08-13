#include<cmath>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include "separation.hpp"
using namespace std;

int Separation::comp( const double &a, const double &b ){
	if ( fabs(a-b) < EPS ) return 0;
	return (a<b)? -1:1;
}

double Separation::max_flow( int s, int t, double C[MAXN][MAXN], int N ){
	/* queue */
	int q[MAXN];
	int qt, qf;
	/* For flow update */
	int prev[MAXN];
	double bneck[MAXN];
	/**/
	bool ok = true;
	int u, v;
	double delta, res;

	res = 0.0;

	for ( int i=0; i < N; ++i )
		fill( f[i], f[i] + N, 0.0 );

	do {
		/* Look for augmenting path */
		fill( S, S + N, false );
		fill( prev, prev + N, -1 );
		
		qf = qt = 0;
		S[s] = true;
		q[ qt++ ] = s;
		bneck[s] = INF;

		while ( qf < qt && prev[t] == -1 ){
			u = q[qf++];
			for ( v = 0; v < N; ++v ){
				if ( v != u && prev[v] == -1 && comp( C[u][v] - f[u][v], 0 ) > 0 ){
					prev[v] = u;
					S[v] = true; 
					bneck[v] = min( bneck[u], C[u][v] - f[u][v] );
					q[qt++] = v;
				}
			}
		}
		/* Augment the flow along the path found */
		if ( prev[t] == -1 ) ok = false;
		else {
			v = t;
			delta = bneck[t];
			res += delta;
			while ( v != s ){
				u = prev[v];
				/* update flow */
				f[u][v] += delta;
				f[v][u] = -f[u][v];
				v = u;
			}	
		}
	}
	while ( ok );
	return res;
}

void Separation::cut_tree(int N, double C[MAXN][MAXN]){
	fill( p, p + N, 0 );
	int t;
	double flow;
	for ( int s=1; s < N; ++s ){
		t = p[s];
		flow = max_flow( s, t, C, N );
		fl[s] = flow;
		/* Update the values */
		for ( int i=0; i < N; ++i ){
			if ( i != s && S[i] && p[i] == t )
				p[i] = s;
		}
		if ( S[p[t]] ){
			p[s] = p[t];
			p[t] = s;
			fl[s] = fl[t];
			fl[t] = flow;
		}
	}
	/* Find cut-sets */
	int deg[MAXN];
	int q[MAXN];
	int qt, qf;
	int u, v;
	fill( deg, deg + N, 0 );
	for ( int i=0; i < N; ++i )
		fill( Cut[i], Cut[i] + N, false );
	
	/* Process the vertices in topological order */
	for ( int i=1; i < N; ++i ) deg[ p[i] ]++;
	qf = qt = 0;
	for ( int i=0; i < N; ++i ) if ( deg[i] == 0 ) q[qt++] = i;
	
	while ( qf < qt ){
		v = q[qf++];
		/* Update cut defined by (v, p[v]) */
		Cut[v][v] = true;
		if ( v == 0 ) continue;
		u = p[v];
		/* Retire edge v - p[v] = u */
		deg[u]--;
		if ( deg[u] == 0 ) q[qt++] = u;
		/* Propagate Cut-set to parent */
		for ( int i=0; i < N; ++i ){
			if ( Cut[v][i] ) Cut[u][i] = true;
		}
	}
}

void Separation::mark_odd_cuts( int N, bool odd[] ){
	int cnt;
	for ( int i=1; i < N; ++i ){
		cnt = 0;
		for ( int j=0; j < N; ++j ){
			if ( Cut[i][j] && odd[j] ) cnt++;
		}
		if ( cnt & 1 ) S[i] = true;
		else S[i] = false;
	}
}

void Separation::get_cut( int N, int M, vector<edge> E, int vertex, double sol[], bool great[], int coef[], int &e1, int &e2 ){
	bool *cutset = Cut[vertex];
	e1 = e2 = -1;
	for ( int j=0; j < M; ++j ){
		/* If is an edge of the cutset and has x(e) > 0 */
		if ( (cutset[ E[j].first ] ^ cutset[ E[j].second ]) ){
			coef[j] = ( great[j] )? -1:1;
			/* Find e1 and e2 */
			if ( comp(sol[j], 0.0) > 0 ){
				if ( great[j] ){
					if ( e1 == -1 || comp( sol[e1], sol[j] ) > 0 ){
						e1 = j;
					}
				} 
				else {
					if ( e2 == -1 || comp( sol[e2], sol[j] ) < 0 ){
						e2 = j;
					}
				}
			}
		}
		else {
			coef[j] = 0;
		}
	}
}


bool Separation::separate( int N, int M, vector<edge> E, double sol[], double C[MAXN][MAXN], bool odd[], bool great[], int coef[] ){
	/* Find the cut tree */
	cut_tree( N, C );
	/* Determine odd cuts */
	mark_odd_cuts( N , odd );
	/* Determine a violated constrain */
	double delta;
	int e1, e2;
	
	for ( int i=1; i < N; ++i ){
		get_cut( N, M, E, i, sol, great, coef, e1, e2 );
		if ( S[i] && comp( fl[i] , 1.0 ) < 0 ){
			return true;
		}
		else if ( !S[i] ){
			/* Find e1 and e2 */
			if ( e1 != -1 && e2 != -1 ){
				delta = min( sol[e1] - (1 - sol[e1]) , (1 - sol[e2]) - sol[e2] );
				if ( comp( fl[i] + delta, 1.0 ) < 0 ){
					if ( comp( sol[e1] - (1 - sol[e1]), (1 - sol[e2]) - sol[e2] ) <= 0 ){
						coef[e1] = 0;					
					}
					else {
						coef[e2] = -1;
					}
					return true;
				}
			}
			else if ( e1 != -1 ){
				if ( comp(fl[i] + sol[e1] - (1.0 - sol[e1]) , 1.0) < 0 ){
					coef[e1] = 0;
					return true;
				}
			}
			else if ( e2 != -1 ){
				if ( comp(fl[i] + (1 - sol[e2]) - sol[e2] , 1.0) < 0 ){
					coef[e2] = -1;
					return true;
				}
			}
		}
	}
	return false;
} 

int main(){
	int N, M;
	int u, v, g;
	double w;
	double C[MAXN][MAXN];
	double sol[MAXN];
	bool great[MAXN];
	bool odd[MAXN];
	int coef[MAXN];
	vector<edge> E;

	scanf("%d %d", &N, &M);

	for ( int i=0; i < N; ++i )
		fill( C[i], C[i] + N, 0.0 );

	for ( int i=0; i < M; ++i ){
		scanf("%d %d %lf %lf %d", &u, &v, sol + i, &w, &g);
		E.push_back( edge(u,v) );
		C[u][v] = C[v][u] = w;
		if ( g ) great[i] = true;
		else great[i] = false;
	}
	
	for ( int i=0; i < N; ++i ){
		scanf("%d", &g);
		if ( g ) odd[i] = true;
		else odd[i] = false;
	}

	Separation Alg;
	if ( Alg.separate( N, M, E, sol, C, odd, great, coef ) ){
		for ( int i=0; i < M; ++i )
			if ( coef[i] ){
				printf("%d %d - %d\n", E[i].first, E[i].second, coef[i]);
			}
	}
	else {
		puts("No violation found");
	}
	return 0;
}
