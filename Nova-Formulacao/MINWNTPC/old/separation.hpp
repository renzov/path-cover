#include<algorithm>
#include<vector>
using namespace std;

typedef pair<int, int> edge;

const int MAXN = 512;
const double EPS = 1e-6;
const double INF = 10e10;

class Separation {
	public:
		/* Num vert., cost matrix, result val, result cut */
		/* Num vert. - Cap Matrix - Odd Vertices - double - bool [] - bool [] */
		void separate( int, double [MAXN][MAXN], double &, bool [], bool []);
		int comp( const double &, const double & );
		double max_flow( int , int , double [MAXN][MAXN], int );
		void cut_tree( int, double [MAXN][MAXN] );
		bool separate( int, int, vector<edge> , double [], double [MAXN][MAXN], bool [], bool [], int [] );
		void mark_odd_cuts( int , bool [] );
		void get_cut( int , int , vector<edge>, int , double [], bool [], int [], int &, int & );
		

	private: 
		double f[MAXN][MAXN]; // remaining vertices
		bool S[MAXN];  /* In max-flow : S[i] is true if i is part of set that defines mincut
					   	  In separate : mark the odd cuts	
					   */	
		bool Cut[MAXN][MAXN];
		/* For Gomury-Hu tree */
		int p[MAXN];
		double fl[MAXN];
		
};
