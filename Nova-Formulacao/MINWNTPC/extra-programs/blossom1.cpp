#include<cmath>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include "blossom1.hpp"
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
	/* N-1 - is the root - artificial vertex */
	fill( p, p + N, N - 1 );
	int t;
	double flow;
	for ( int s=0; s < (N-1); ++s ){
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
	for ( int i=0; i < (N-1); ++i ) deg[ p[i] ]++;
	qf = qt = 0;
	for ( int i=0; i < N; ++i ) if ( deg[i] == 0 ) q[qt++] = i;
	
	while ( qf < qt ){
		v = q[qf++];
		/* Update cut defined by (v, p[v]) */
		Cut[v][v] = true;
		if ( v == (N-1) ) continue;
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
	for ( int i=0; i < (N-1); ++i ){
		cnt = 0;
		size[i] = 0; 
		for ( int j=0; j < N; ++j ){
			if ( Cut[i][j] && odd[j] ) cnt++;
			if ( Cut[i][j] ) size[i]++;
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
			coef[j] = ( great[j] )? -1:0;
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
		else if ( cutset[ E[j].first ] && (cutset[ E[j].first ] == cutset[ E[j].second ])  ) {
			coef[j] = 1;
		}
		else {
			coef[j] = 0;
		}
	}
}

bool Separation::two_matching( int N, int M, vector<edge> E, double sol[], double C[MAXN][MAXN], bool odd[], bool great[], int coef[], int &sz ){
	/* Find the cut tree */
	cut_tree( N, C );
	/* Determine odd cuts */
	mark_odd_cuts( N , odd );
	/* Determine a violated constrain */
	double delta;
	int e1, e2;
	
	for ( int i=0; i < (N-1); ++i ){
		if ( size[i] >= (N-1) ) continue;
		get_cut( N, M, E, i, sol, great, coef, e1, e2 );
		if ( S[i] && comp( fl[i] , 1.0 ) < 0 ){
			sz = size[i];
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
					sz = size[i];					
					return true;
				}
			}
			else if ( e1 != -1 ){
				if ( comp(fl[i] + sol[e1] - (1.0 - sol[e1]) , 1.0) < 0 ){
					coef[e1] = 0;
					sz = size[i];					
					return true;
				}
			}
			else if ( e2 != -1 ){
				if ( comp(fl[i] + (1 - sol[e2]) - sol[e2] , 1.0) < 0 ){
					coef[e2] = -1;
					sz = size[i];					
					return true;
				}
			}
		}
	}
	return false;
}

bool Separation::min_odd_cut( int N, int M, vector<edge> E, double sol[], double W[MAXN][MAXN], int coef[], int &sz ){
	cut_tree( N, W );
	double res = 2.0 * N * N;
	int U;
	bool mincut[MAXN];

	for ( int i=0; i < N-1; ++i ){
		int cnt = 0;
		for ( int j=0; j < N; ++j ){
			if ( Cut[i][j] ) cnt++; 
		}
		if ( cnt & 1 && comp(fl[i],res) < 0 ){ // if its an odd cut
			res = fl[i];
			if ( Cut[i][N-1] ){ // if it contains the root we take the complement 
				U = N - cnt;
				for ( int j=0; j < N; ++j ) mincut[j] = not Cut[i][j];
			}
			else {
				U = cnt;
				for ( int j=0; j < N; ++j ) mincut[j] = Cut[i][j];
			}
		}
	}
	/* Find the minimum cut */
	sz = -( ( U + 1 ) / 2 );
	fill( coef, coef + M, 0 );
	for ( int i=0; i < M; ++i ){
		if ( mincut[ E[i].first ] ^ mincut[ E[i].second ] ) coef[i] = -1;
		else if ( mincut[ E[i].first ] && mincut[ E[i].second ] ) coef[i] = -1;
	}
	return comp( res, 1.0 ) < 0;
}

bool Separation::separate( int N, int M, vector<edge> E, double sol[], double C[MAXN][MAXN], double W[MAXN][MAXN], 
						   bool odd[], bool great[], int coef[], int &sz ){
	/* First look for 2-matching violated inequalities */
	if ( two_matching( N + 1, M, E, sol, C, odd, great, coef, sz ) ){
		int F = 0;
		for ( int i=0; i < M; ++i ){
			if ( coef[i] == -1 ){ 
				F++;
				coef[i] = 1;	
			}
		}
		F /= 2;
		sz += F;
		return true;
	} 
	return false;
} 

