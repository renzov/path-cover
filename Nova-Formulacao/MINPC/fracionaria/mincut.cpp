#include<cmath>
#include<iostream>
#include<cstdlib>
#include<algorithm>
#include "mincut.hpp"
using namespace std;

inline int MinCut::find_set( int x ){ return (x==p[x])? x:(p[x]=find_set(p[x])); }

int MinCut::comp( const double &a, const double &b ){
	if ( fabs(a-b) < EPS ) return 0;
	return (a<b)? -1:1;
}

void MinCut::min_cut( int n, double C[MAXN][MAXN], double &res, bool S[]){
	int left = n;
	res = INF;
	for ( int i=0; i<n; ++i ) rem[i] = p[i] = i;
	
	while ( left > 1 ){ // Minimum Cut Phase
		// Initialize A and w
		A[ rem[0] ] = true;
		for ( int i=1; i < left; ++i ) {
			A[ rem[i] ] = false;
			W[ i ] = C[ rem[0] ][ rem[i] ];
		}
		int prev = rem[0]; // last vertex added to A
		int idx; // Index of the most tightly connected vertex not in A
		for ( int i=1; i < left; ++i ){ 
			idx = -1;
			for ( int j=1; j < left; ++j )
				if ( !A[ rem[j] ] && ( idx==-1 || comp (W[j], W[idx]) > 0 ) )
					idx = j;
			// Add rem[idx] to A
			A[ rem[idx] ] = true;
			if ( i == left - 1 ){ // If its the last vertex
				if ( comp(res, W[idx]) > 0 ){ // Found a better cut 
					for ( int j=0; j < n; ++j ){
						S[j] = find_set( j ) == find_set( rem[idx] );
					}
					res = W[idx];
				}
				// merge the prev and rem[idx] vertex
				p[ rem[idx] ] = prev;
				for ( int j=0; j < left; ++j ){
					C[rem[j]][prev] = C[prev][rem[j]] += C[rem[idx]][rem[j]];
				}
				rem[idx] = rem[--left];
				break;
			}
			prev = rem[idx];
			// Update the weights of neighbors of rem[idx] not in A
			for ( int j=1; j < left; ++j ){
				if ( !A[ rem[j] ] )
					W[j] += C[ rem[idx] ][ rem[j] ];
			}
		}
	}
}

/* For testing 
int main(){
	bool sol[MAXN];
	double M[MAXN][MAXN];
	double res;
	M[0][0] = 0.0, M[0][1] = 1.0, M[0][2] = 4.0;
	M[1][0] = 1.0, M[1][1] = 0.0, M[1][2] = 3.0;
	M[2][0] = 4.0, M[2][1] = 3.0, M[2][2] = 0.0;

	MinCut Alg;
	Alg.min_cut(3, M, res, sol);
	printf("min-cut: %f\nGiven by vertex set:", res);
	for ( int i=0; i < 3; ++i ) if ( sol[i] ) printf(" %d", i);
	puts("");
	return 0;
}
*/
