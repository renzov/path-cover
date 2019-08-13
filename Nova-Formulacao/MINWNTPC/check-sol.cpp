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
const int MAXN = 624;
int deg[MAXN] = {0};

int main(int argc, char *argv[]){
	int u, v;
	double w;
	bool frac = false;
	int N = 0;

	while ( scanf("%d %d %lf", &u, &v, &w) == 3 ){
		if ( fabs(w) > EPS && w > 0 ){
			deg[u]++, deg[v]++;
			if ( fabs(w - 1.0) > EPS && w < 1.0 ) frac = true;			
		}
		N = max( N, u );
		N = max( N, v );
	}
	bool ok = true;
	for ( int i=0; i <= N && ok; ++i )
		if ( deg[i] < 1 || deg[i] > 2 ) ok = false;

	if ( !ok ){
		puts("Not feasible solution.");
	}
	else if ( frac ){
		puts("Fractional Solution");
	}
	return 0;
}

