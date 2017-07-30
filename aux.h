#ifndef __mylib__ 
#define __mylib__ 

#include <string>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;

#define MESSAGE(a) cout << a << endl;
class RawData{
public:
	static const int MaxSize = 1 << 14; //16k
	int size;
	uint64_t *ws;
	uint32_t *fp;
	double *unknown;
	double *mr;
	int *id;

	uint64_t N;
	uint64_t M;
	uint64_t T;
	string name;

	RawData(){
		ws = new uint64_t[MaxSize];
		fp = new uint32_t[MaxSize];
		unknown = new double[MaxSize];
		mr = new double[MaxSize];
		id = new int[MaxSize];
	}

	double GetMissCount(int cid){
		return N * mr[cid];
	}
	void SwapData(int src, int dest){
		swap(ws[src], ws[dest]);
		swap(fp[src], fp[dest]);
		swap(unknown[src], unknown[dest]);
		swap(mr[src], mr[dest]);
		swap(id[src], id[dest]);
	}
};

inline uint64_t getNumber(string x){
	uint64_t ret = 0;
	int pos = 0;
	while(!isdigit(x[pos]))
		pos++;
	while(pos < x.length()){
		ret *= 10;
		ret += x[pos++] - '0';
	}
	return ret;
}

int strip = 128;
//get size from system command "wc"
RawData* GetData(string file){
	ifstream fin(file.c_str());
	RawData *r = new RawData();
	r->name = file;
	string n, m, t;
	fin >> n >> m >> t;
	r->N = getNumber(n);
	r->M = getNumber(m);
	r->T = getNumber(t);
	int pos = 0;
	int tmp;
	while(fin >> tmp){
		double fp;
		fin >> r->ws[pos];
		fin >> fp;
		tmp = floor(fp);
		fin >> r->unknown[pos];
		fin >> r->mr[pos];
		
		if(fp < strip) continue;
		//Strip with a length of 128*64
		if(pos >= 1 && tmp/strip == r->fp[pos-1])
			pos--;
		r->fp[pos] = tmp/strip;
		pos++;
	}
	r->size = pos;
	fin.close();
	return r;
}

const uint64_t IINFI = (uint64_t)-1;
const double DINFI = 1e100;


#endif
