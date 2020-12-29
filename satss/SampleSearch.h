#ifndef SAMPLESEARCH_H_
#define SAMPLESEARCH_H_

#include "GM.h"
#include "JG.h"
#include "IJGPSamplingHelper.h"
#include "Vec.h"

#include "Heap.h"
#include "Alg.h"
#include "SolverTypes.h"
class SampleSearch
{
private:
	GM& gm;
	JG& jg;
	vector<int> order;
	IJGPSamplingHelper helper;
	Double getWeight(vec<lbool>& assignment);
	void getWeight (vec<lbool>& assignment, vector<Double>& bf);
	myRandom random;
public:
	SampleSearch(GM& gm, JG& jg, int p_bound, vector<int>& order);
	Double computePE(int time_limit, int num_samples=INVALID_VALUE);
	void computeBeliefs(int time_limit, int num_samples, vector<vector<Double> >& beliefs);
	int getSample(int& variable, bool& value, vec<char>& assignment);
	// Return a sample which is implictly stored as a value for each variable in the GM and its backtrack-free
	// distribution
	void getSample(vector<Double>& bf);
	static bool isconsistent(GM& gm);

};

class RBSampleSearch
{
public:
	int interval;
	string outfilename;
	RBSampleSearch();
	Double computePE(GM& gm, JG& jg, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples);
	void reduce(GM& gm, vector<vector<bool> >& new_domains);
	void computeBeliefs(GM& gm, JG& jg, int p_bound, vector<set<int> >& clusters, vector<int>& order,vector<int>& sampling_order, int num_samples);
	Double computePEApp(GM& gm, JG& jg, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples, int max_restarts);
	void computeBeliefsApp(GM& gm, JG& jg, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples, int max_restarts);
	void computeBayesBeliefsApp(GM& gm, GM& other_gm, JG& jg, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples, int max_restarts);
	//void addTrace(GM& gm, vector<int>& sampling_order, ORStore& store, vec<Lit>& trail, vec<int> trail_lim);
};

class RBSampleSearchSAT
{
	public:
	int interval;
	string outfilename;
	RBSampleSearchSAT();
	void reduce(const char* satfilename,GM& gm, vector<vector<bool> >& new_domains);
	Double computePE(GM& gm, JG& jg, const char* satfile, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples);
	Double computePEApp(GM& gm, JG& jg, const char* satfile, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples, int max_restarts);
	double runBE(GM& gm, vector<int>& order, char* infile);	
	void generateSamples(GM& gm, JG& jg, const char* satfile, int p_bound, vector<int>& order,vector<int>& sampling_order, int num_samples, int max_restarts);
};
#endif
