const int MAXN = 512;
const double EPS = 1e-6;
const double INF = 10e10;

class MinCut {
	public:
		/* Num vert., cost matrix, result val, result cut */
		void min_cut( int, double [MAXN][MAXN], double &, bool []);

	private: 
		int rem[MAXN]; // remaining vertices
		bool A[MAXN]; // A[i] is true iff i is in A
		double W[MAXN]; // Cost of cut induced by A
		int p[MAXN]; // for union find 
	
	int comp( const double &a, const double &b );
	inline int find_set( int );
};
