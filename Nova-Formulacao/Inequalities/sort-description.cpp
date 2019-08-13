#include<iostream>
#include<string>
#include<map>
#include<sstream>
#include<algorithm>
#include<vector>
using namespace std;
/*
	Format of the file

	Name:
		----- Name of each variable
	Inqualities:
		----- Each line describes an inequality of the form:
			ax <= b
*/

map<string, int> idx;	// Maps variables to integer (0 ... N-1)
int N;			// Number of variables

struct ineq {
	int nonzero;
	vector<int> coef;
	int rhs;
	string str;

	ineq(){
		coef.resize(N);
		fill( coef.begin(), coef.end(), 0 );
		nonzero = 0;
	}
	
};

bool cmp( const ineq &A, const ineq &B ){
	if (A.nonzero != B.nonzero) return A.nonzero < B.nonzero;
	for ( int i=0; i < N; ++i ){
		if ( abs(A.coef[i]) != abs(B.coef[i]) ) 
			return abs(A.coef[i]) < abs(B.coef[i]);
	}
	return A.rhs < B.rhs;
}

void parse_variables(const string &L){
	istringstream in(L);
	string name;
	N = 0;
	while (in >> name)
		idx[name] = N++;
}

ineq parse_inequality( string L ){
	ineq e;
	e.str = L;
	
	istringstream in(L);
	string var;

	while ( in >> var ){
		if ( var == "<=" ){
			in >> e.rhs;	// Read the rhs
		}
		else { 
			e.nonzero++;
			if ( var[0] == '+' ){
				var = var.substr(1);
				e.coef[ idx[var] ] = 1;
			}
			else if ( var[0] == '-' ){
				var = var.substr(1);
				e.coef[ idx[var] ] = -1;
			}
			else {
				e.coef[ idx[var] ] = 1;
			}
		}
	}
	return e;
}

int main(){
	int M = 0;		// Number of inequalities
	string line;
	getline(cin, line);	// Read "Name:"
	getline(cin, line);	// Read the name of variables
	parse_variables(line);	
	getline(cin, line);	// Read "Inequalities:"
	vector<ineq> V;
	while ( getline(cin, line)  ){
		V.push_back( parse_inequality(line) );
		M++;
	}
	
	// Sort the inequalities
	sort( V.begin(), V.end(), cmp );
	for ( int i=0; i < M; i++ )
		cout << V[i].str << endl;
	return 0;
}
