#include<cstdio>
#include<iostream>
#include<cstdlib>
#include <fstream>
#include<vector>
#include<cmath>

using namespace std;
typedef pair<int,int> edge;
typedef pair< edge, double > elem;

const double EPS = 10e-6;

int main(int argc, char *argv[]){
	int u, v;
	double w, paths;
	bool frac = false;
	
	vector<elem> E;
	scanf("%lf", &paths);
	while ( scanf("%d %d %lf", &u, &v, &w) && u >= 0 ){
		if ( fabs(w - 1) > EPS ) frac = true;
		E.push_back( elem( edge(u,v) , w ) );
	}
	if ( !frac ){
		puts("No fractional solution.");
	}
	else {
		puts("Fractional Solution");
		printf("Paths = %f\n", paths);
		for ( int i=0; i < (int) E.size(); ++i ){
			printf("%d %d %f\n", E[i].first.first, E[i].first.second, E[i].second);
		}
	}
	return 0;
}
