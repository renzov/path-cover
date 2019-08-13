#include <iostream>
#include <cstdio>
#include <algorithm>
#include <set>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <stack>
#include <cmath>
#include <fstream>


#define repn( i , a , b ) for( int i = ( int ) a ; i < ( int ) b ; i ++ )
#define rep( i , n ) repn( i , 0 , n ) 
#define all( x )  x.begin() , x.end()
#define rall( x ) x.rbegin() , x.rend()
#define mp make_pair
#define fst first
#define snd second
using namespace std;

typedef long long int64;
typedef long double ldouble;
typedef pair< int , int > pii;

/*bool check_solution( ifstream &f, int e, int p, int n,  set<pii> &E  ){
	int u, v;
	string line;
	int deg[ n ];
	fill( deg, deg + n, 0 ); 
	int one = 0, two = 0;
	set<pii> used;
	for ( int i=0; i < e; ++i ){
		getline( f, line );
		stringstream tmp(line);
		tmp >> u >> v;
		if ( u > v ) swap(u,v);
		deg[u]++, deg[v]++;
		if ( E.find( pii(u,v) ) == E.end() ) return false;
		if ( used.find( pii(u,v) ) != used.end() ) return false;
		used.insert( pii(u,v) );
	}
	for ( int i=0; i < n; ++i ){
		if ( deg[i] > 2 || deg[i] == 0 ) return false;
		if ( deg[i] == 1 ) one++;
		else two++;
	}
	for( int i=0; i < p; ++i ){
		getline( f , line );
		getline( f , line );
	}
	return one == (2*p);
}*/

int main(int argc, char *argv[]){
	stringstream dir;
	if ( argc != 4 ){
		cout << "Usage: checker-grau numVert degree numInst" << endl;
		return 1;
	}
	int numVert = atoi( argv[1] );
	int deg = atoi( argv[2] );
	int numInst = atoi( argv[3] );
	int solVal;
	double mean = 0.0;
	int u, v;

	dir << "./instancias/" << numVert << "-R" << deg << "/";
	string base = dir.str();
	for ( int i=0; i < numInst; ++i ){
		stringstream name;
		name << base << "out" << i;
		ifstream f( name.str().c_str() );
		f >> solVal;	
		//check_solution( f , numVert - numPaths, numPaths, numVert, edges );
		while ( f >> u >> v && u != -1 );
		double t;
		f >> t;
		mean += t;
		f.close();
	}
	cout << "Results from the analysis:" << endl;
	cout << "Mean time: " << mean/numInst << " seconds." << endl;
	return 0;
}
