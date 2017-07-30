/*
PPDP Algorithm, Assign P program to p partitions
*/

#include <iostream>
#include <stdint.h>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include "aux.h"
using namespace std;

inline double Cost(RawData *R, int pos){
	return R->mr[pos]*R->N;
}

//P programs, Cache size is C, data stored in R, sol stores the optmial partition
//return optimal miss ratio
double DP(int P, int C, RawData **R, int *sol){
	uint64_t *retval[P];
	int *Trace[P];

	const int EPS = 0;
	//seems fp is not continous, there might be better solution.
	C += EPS;

	for(int i = 0; i < P; i++){
		retval[i] = new uint64_t[C+1];
		Trace[i] = new int[C+1];
		//memset(retval[i], 1, sizeof(double) * (C+1));
		fill(retval[i], retval[i]+C+1, IINFI);
	}

	for(int i = 0; i < R[0]->size; i++){
		if(R[0]->fp[i] > C) break;
		retval[0][R[0]->fp[i]] = Cost(R[0], i);
		Trace[0][R[0]->fp[i]] = i;
	}

	for(int i = 0; i < P-1; i++){
		for(int j = 0; j < C; j++){
			if(retval[i][j] == IINFI) continue;
			for(int k = 0; k < R[i+1]->size; k++){
				if(j + R[i+1]->fp[k] > C) break;
				uint64_t x = retval[i][j] + Cost(R[i+1], k);
				uint64_t &y = retval[i+1][j+R[i+1]->fp[k]];
				y = min(y, x);
				if(y == x)
					Trace[i+1][j+R[i+1]->fp[k]] = k;
			}
		}
	}
	C -= EPS;
	uint64_t minAccess = IINFI;
	for(int i = C-EPS; i <= C+EPS; i++){
		if(retval[P-1][i] == DINFI) continue;
		minAccess = min(minAccess, retval[P-1][i]);
		if(minAccess == retval[P-1][i])
			sol[P-1] = Trace[P-1][i];
	}
	for(int i = P-1, c = C; i > 0; i--){
		c -= R[i]->fp[sol[i]];
		sol[i-1]=Trace[i-1][c];
	}

	uint64_t totalAccess = 0;
	for(int i = 0; i < P; i++)
		totalAccess += R[i]->N;
	
	//cout << minAccess << " " << totalAccess << endl;
	return minAccess / (double)totalAccess;
}

//command CacheSize(in MB) Prog1 Prog2 Prog3 Prog4 ....
int main(int argc, char **argv){
	int P = argc - 2;
	int C = atoi(argv[1]);
	C <<= 20;
	C >>= 6;
	C /= strip;
	RawData *R[P];
	for(int i = 2; i < argc; i++){
		R[i-2] = GetData(argv[i]);
	}
	int *sol=new int[P];
	double retval = DP(P, C, R, sol);
	
	cout << retval << endl;
	for(int i = 0; i < P; i++){
		cout << R[i]->mr[sol[i]] << endl;
		//cout << R[i]->fp[sol[i]]*strip << " ";
	}
	//cout << endl;
	
	return 0;
}
