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
#include <sstream>
using namespace std;

typedef long long int64;
typedef long double ldouble;
typedef pair< int , int > pii;

int main(int argc, char *argv[]){
	string trash;
	int n; 
	getline(cin, trash);
	int x;
	int size = 0;
	vector< vector<int> > V;
	while ( getline(cin, trash) ){
		istringstream in(trash);
		V.push_back( vector<int>() );
		while ( in >> x ){
			V[size].push_back(x);
		}
		size++;
	}
	if (size) n = (int) V[0].size() - 1;
	cout << "Vertices:" << endl;
	for ( int i=0; i < size; ++i ){
		if ( V[i][n] == 1 ){
			for ( int j=0; j < n; ++j ){
				cout << V[i][j] << ' ';
			}
			cout << endl;
		}
	}
	cout << "Rays:" << endl;
	for ( int i=0; i < size; ++i ){
		if ( V[i][n] == 0 ){
			for ( int j=0; j < n; ++j ){
				cout << V[i][j] << ' ';
			}
			cout << endl;
		}
	}
	return 0;
}

