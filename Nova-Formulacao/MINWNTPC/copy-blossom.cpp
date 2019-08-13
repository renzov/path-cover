#include<cmath>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include "blossom.hpp"
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

inline bool bit_set( const int &msk, const int &b ){
	return ( (msk >> b) & 1 ) != 0;
}

bool Separation::brute_violated_subset( int N, int M, vector<edge> E, double sol[], int coef[], int &sz ){
	/* test every subset */
	bool setA, setB;
	for ( int mask=1; mask < (1 << N); ++mask ){
		double cut_val = 0.0;
		fill( coef, coef + M, 0 );
		for ( int j=0; j < M; ++j ){
			setA = bit_set( mask, E[j].first );
			setB = bit_set( mask, E[j].second );
			if ( (setA && setB) || (setA ^ setB) ){
				cut_val += sol[j];
				coef[j] = -1;
			}
		}
		/* Test if the set violates the constrain */
		int U = ( __builtin_popcount(mask) + 1 ) / 2; 
		if ( comp( cut_val, U ) < 0 ){
			sz = -U;
			return true;
		}
	}
	return false;
}

bool Separation::separate( int N, int M, vector<edge> E, double sol[], double C[MAXN][MAXN], bool odd[], bool great[], int coef[], int &sz ){
	/* First look for 2-matching violated inequalities */
	if ( two_matching( N, M, E, sol, C, odd, great, coef, sz ) ){
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
	if ( brute_violated_subset( N - 1, M, E, sol, coef, sz ) ){
		return true;
	}
	return false;
} 
/*
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
		scanf("%lf", &w);
		C[N][i] = C[i][N] = w;
	}

	for ( int i=0; i <= N; ++i ){
		for ( int j=0; j <= N; ++j )
			printf(" %f", C[i][j]);
		puts("");
	}

	for ( int i=0; i < N; ++i ){
		scanf("%d", &g);
		if ( g ) odd[i] = true;
		else odd[i] = false;
	}
	odd[N] = false;

	Separation Alg;

	printf("0 -> 2 : %f\n", Alg.max_flow(0, 2, C, N+1));
	printf("0 -> 1 : %f\n", Alg.max_flow(0, 1, C, N+1));
	printf("0 -> 3 : %f\n", Alg.max_flow(0, 3, C, N+1));

	if ( Alg.separate( N + 1, M, E, sol, C, odd, great, coef ) ){
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
*/
