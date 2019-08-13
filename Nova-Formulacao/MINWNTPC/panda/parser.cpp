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
#include <numeric>
#include<sstream>

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
const double EPS = 1e-6;
const int MAXN = 500;

map<string,int> idx; // Variavel -> Indice
string X[MAXN];
double V[MAXN];
vector< vector<int> > coef;
vector< double > rhs;


int main(){
	string name;
	string line;
	double val;
	int N;
	int M;
	int id;
	// Leemos o numero de variaveis
	cin >> N;
	for ( int i=0; i < N; ++i ){
		cin >> name >> val;
		idx[ name ] = i;
		X[ i ] = name;
		V[ i ] = val;
		cout << X[ i ] << " = " << V[i] << endl;
	}
	// read the empty string
	getline(cin, line);	
	M = 0; // Number of inequalities
	// now, we process each inequality
	while ( getline(cin, line) ){
		istringstream in(line);
		coef.push_back( vector<int>(N, 0) );
		while ( in >> name && name != "<=" ){
			// We suppose that coefficient is +1 or -1
			string var;
			if ( name[0] == '+' || name[0] == '-' ){
				var = name.substr(1);
			}
			else {
				var = name;
			}
			
			if ( idx.count(var) == 0 ){
				cout << "ERROR - variable" << var << " not found." << endl;
				return 1;
			}
			else {
				id = idx[var];
			}
			if ( name[0] == '-' ){
				coef[M][id] = -1;
			}
			else {
				coef[M][id] = 1;
			}
		}
		M++;
		in >> val; 
		rhs.push_back(val);
	}
	// Look for violated constraints
	cout << "Violated inequalities:" << endl; 
	for ( int i=0; i < (int)coef.size(); ++i ){
		double lhs = 0.0;
		for ( int j=0; j < N; ++j ){
			lhs += coef[i][j] * V[ j ];
		}
		if (  fabs(lhs - rhs[i]) > EPS && lhs > rhs[i] ){
			for  ( int j=0; j < N; ++j ){
				if ( coef[i][j] != 0 ){
					if ( coef[i][j] > 0 ) cout << " +";
					else cout << " -";
					cout << X[j];
				}
			}
			cout << " <= " << rhs[i] << endl; 
		}
	}
	return 0;
}

