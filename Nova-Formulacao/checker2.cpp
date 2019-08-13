#include <bits/stdc++.h>
#include "testlib.h"
using namespace std;
typedef pair<int,int> edge;
const int MAXN = 1024;

set<edge> E;
int N, M;
int p[MAXN]; 
int deg[MAXN];

inline int find_set( const int &x ){ return p[x] == x? x:(p[x] = find_set(p[x])); }

void readGraph(InStream& in) {
	N = in.readInt(); in.readSpace();
	M = in.readInt(); in.readEoln();
	int u, v;
	for ( int i=0; i < M; ++i ){
		u = in.readInt(); in.readSpace();
		v = in.readInt(); in.readEoln();
		if ( u > v ) swap(u, v);
		E.insert( edge(u, v) );
	}
}

int readAndCheckAnswer( InStream& in ){
	int res = in.readInt(); in.readEoln();
	int u, v;
	edge e;
	set<edge> S;	// solution
	int ncomp = N;
	
	if ( res <= 0 ) return -1;
	if ( res > 1 ) printf("No Hamiltonian path\n");
	for ( int i=0; i < N; ++i ) p[i] = i, deg[i] = 0;	// Initialize the DS and the degree 
	while ( 1 ){
		u = in.readInt(); in.readSpace();
		v = in.readInt(); in.readEoln();
		if ( u >= N || v >= N ) continue;
		if ( u == -1 || v == -1 ) break;			
		if ( u > v ) swap(u, v);
		e = edge(u, v);
		if ( E.find(e) == E.end() ) in.quitf(_wa, "Edge (%d, %d) does not exist.", u, v);
		if ( S.find(e) != S.end() ) in.quitf(_wa, "Edge (%d, %d) duplicated.", u, v );
		S.insert( e );
		deg[u]++, deg[v]++;
		u = find_set(u), v = find_set(v);
		if ( u == v ) in.quitf(_wa, "The solution contains a cycle.");
		p[v] = u; // Join the two sets
		ncomp--;
		if ( deg[u] > 2 ) in.quitf(_wa, "The vertex %d has degree greater than 2 in the solution.", u);
		if ( deg[v] > 2 ) in.quitf(_wa, "The vertex %d has degree greater than 2 in the solution.", v);
	}
	if ( ncomp != res ) in.quitf(_wa, "Number of components = %d - Optimal value = %d.", ncomp, res);
	in.readDouble(); in.readEoln();
	in.readEof();
	return 1;	
}

int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);	
	readGraph( inf );	
	int ja = readAndCheckAnswer(ans);
	int pa = readAndCheckAnswer(ouf);
	if(ja != -1 && pa == -1)
		quitf(_wa, "Jury has the answer but participant has not");
	if(ja == -1 && pa != -1)
		quitf(_fail, "Participant has the answer but jury has not");
	quitf(_ok, "N = %d M = %d", N, M);	
	return 0;
}
