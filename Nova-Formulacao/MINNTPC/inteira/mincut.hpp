#include<climits>
const int MAXN = 256;
const int INF = INT_MAX;

class MinCut {
	public:
		/* Num vert., cost matrix, result val, result cut */
		void min_cut( int, int [MAXN][MAXN], int &, bool []);

	private: 
		int rem[MAXN]; // remaining vertices
		bool A[MAXN]; // A[i] is true iff i is in A
		int W[MAXN]; // Cost of cut induced by A
		int p[MAXN]; // for union find 
	
	inline int find_set( int );
};
