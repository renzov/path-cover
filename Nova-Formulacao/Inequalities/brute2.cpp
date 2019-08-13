#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
#include<sstream>
using namespace std;
const int MAXE = 1024;
const double EPS = 1e-6;
// x_0_1 x_0_2 x_0_3 x_1_2 x_1_4 x_2_5 x_3_4 x_3_5 x_4_5
int coef[] = {-1, -1, 0, -1, 0, 0, 1, 1, 1};
int V[MAXE];


int main(){
	int M, x;
	string line;
	getline(cin, line);
	while ( getline(cin, line) ){
		istringstream in(line);
		M = 0;
		while ( in >> x ){
			V[M++] = x;
		}
		M--;
		double res = 0.0;
		double f;
		for ( int i=0; i < M; ++i ){
			f = (double)V[i] / V[M];
			res += f * coef[i];
		}
		if ( fabs( res - 1 ) > EPS && res > 1 ){
			cout << "vertex violation:";
			for ( int i=0; i < M; ++i ) 
				cout << " " << V[i] << "/" << V[M];
			cout << endl;
		}
	}
	return 0;
}
