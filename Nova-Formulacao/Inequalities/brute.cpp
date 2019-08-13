#include<iostream>
#include<string>
#include<cmath>
#include<cstdlib>
#include<sstream>
using namespace std;
const int MAXE = 1024;
const double EPS = 1e-6;
// x_0_1 x_0_2 x_0_3 x_1_2 x_1_4 x_2_5 x_3_4 x_3_6 x_4_7 x_5_6 x_5_7 x_6_7 
int coef[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 };
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
		if ( fabs( res - 6 ) > EPS && res > 6 ){
			cout << "vertex violation:";
			for ( int i=0; i < M; ++i ) 
				cout << " " << V[i] << "/" << V[M];
			cout << endl;
		}
	}
	return 0;
}
