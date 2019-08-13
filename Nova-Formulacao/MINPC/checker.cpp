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

const double EPS = 10e-6;
vector<int> nonInt;

double get_time( ifstream &in ){
	string line;
	getline(in, line);
	stringstream tmp(line);
	string t;
	double res;
	tmp >> t;
	tmp >> t; 
	tmp >> t;
	tmp >> res;
	return res;
}

bool check_solution( ifstream &f, int e, int p, int n,  set<pii> &E  ){
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
}

int main(int argc, char *argv[]){
	stringstream dir;
	if ( argc != 3 ){
		cout << "Usage: checker numVert numInst" << endl;
		return 1;
	}
	int numVert = atoi( argv[1] );
	int numInst = atoi( argv[2] );
	double solVal;
	double mean = 0.0;

	dir << "./instancias/" << numVert << "/";
	string base = dir.str();
	for ( int i=0; i < numInst; ++i ){
		stringstream name;
		name << base << "out-r" << i;
		ifstream f( name.str().c_str() );
		f >> solVal;	
		if ( fabs( solVal - floor(solVal) ) > EPS ){
			nonInt.push_back( i );
		}
		check_solution( f , numVert - numPaths, numPaths, numVert, edges );
		double t = get_time( f );
		mean += t;
		f.close();
	}
	cout << "Results from the analysis:" << endl;
	cout << "Mean time: " << mean/numInst << " seconds." << endl;
	if ( nonInt.empty() ){
		cout << "No non-hamiltonian found." << endl;
	}
	else {
		cout << "Non-hamiltonians:" << endl;
		for ( vector<int>::iterator it=nonHam.begin();  it != nonHam.end(); it++ ){
			cout << *it << endl;
		}
	}
	return 0;
}
