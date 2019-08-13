#include<cstdio>
#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

const int MAXN = 625;
vector<int> G[MAXN];

int main(){
	int N, M;
	int u, v;

	cout << "Names:" << endl;
	cin >> N >> M;
	for ( int i=0; i < M; ++i ){
		cin >> u >> v;
		if ( u > v ) swap(u, v);
		G[u].push_back(v);
		G[v].push_back(u);
		cout << "x_" << u << "_" << v << " ";
	}
	cout << endl << "Inequalities:" << endl;
	// variable bounds
	for ( u = 0; u < N; ++u ){
		for ( int j=0; j < (int)G[u].size(); ++j ){
			v = G[u][j];
			if ( u < v ){
				cout << "x_" << u << "_" << v << " <= 1" << endl;
				cout << "x_" << u << "_" << v << " >= 0" << endl;
			}
		}
	}
	// Degree constraint	
	for ( int i=0; i < N; ++i ){
		for ( int j=0; j < (int)G[i].size(); ++j ){
			u = i;
			v = G[i][j];
			if ( u > v ) swap(u, v);
			cout << "+x_" << u << "_" << v << " "; 
		}
		cout << "<= 2" << endl;
	}

	for ( int i=0; i < N; ++i ){
		for ( int j=0; j < (int)G[i].size(); ++j ){
			u = i;
			v = G[i][j];
			if ( u > v ) swap(u, v);
			cout << "+x_" << u << "_" << v << " ";
		}
		cout << ">= 1" << endl;
	}
	return 0;
}
