#ifndef GM_H_
#define GM_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>
#include "Double.h"

using namespace std;
#include "Variable.h"
#include "Function.h"
#include "Heap.h"
#include "Alg.h"
#include "SolverTypes.h"

#define INVALID_VALUE -1

// A graphical model has Random Variables and Functions

typedef enum {BAYES, MARKOV} NETWORK_TYPE;
typedef enum {POSITIVE, DET} NETWORK_MODE;
struct GM
{
	NETWORK_TYPE type;
	NETWORK_MODE mode;
        
  void addToWCSP(Double& value);
        vector<Double> wcsp_probs;
    double damp;
  int c_bound;
    vector<vector<Lit> > clauses;
	vector<Variable*> variables;
	vector<Function*> functions;
	vector<Variable*> copy_of_variables;
	vector<int> sat_to_csp_variables;
	Double mult_factor;
	vector<vector <int> > csp_to_sat_variables;
	vector<int> var_to_ids;
  GM():mult_factor(Double(1.0)), type(BAYES), mode(POSITIVE),damp(0.0),c_bound(15) {}
	void makeCopy(GM& gm);
	bool readCNF(const char* infile);
	bool readCF(char* infile);
	bool readVIB(char* infile);
	void convertToSAT();
	void writeSAT(char* satfilename);
	void readEvidenceErgo(char* evidencefile);
	void getIrrelevantNodes(vector<int>& evidence, vector<int>& irrelevant_nodes);
	void readEvidenceVIB(char* evidencefile);
	void readEvidence1(char* evidencefile);
	void writeConstraintGraph(char* cgraph);
	void convertToCN(GM& cn);
	void readUAI08(const char* infile);
	void readUAIO8Evidence(char* evidencefile);
	void readErgo(char* infile);
	void reduceBN(GM* gm, int i_bound, vector<int>& order);
	void writeErgo(char* outfile);
	void removeIrrelevantNetwork(vector<int>& evidence);
	void setEvidenceBeliefsUAI08(vector<int>& evidence);
	void reduce(int i_bound);
	void reduceDomains();
	void reduceSAT(const char* satfilename);
	void eliminate(int i_bound);
	void getMinFillOrdering(vector<int>& order, vector<set<int> >& clusters, double& estimate);
	void getMinDegreeOrdering(vector<int>& order, vector<set<int> >& clusters, double& estimate);
	void getLexOrdering(vector<int>& order, vector<set<int> >& clusters, double& estimate);
	void getTopologicalOrdering(vector<int>& order, vector<set<int> >& clusters, double& estimate,vector<int>& suggestion);
	void getClusters(vector<int>& order, vector<set<int> >& clusters);
	void rearrangeOrdering(vector<int>& order, vector<set<int> >& clusters, vector<int>& new_order, double& limit);
	void printMarginalsUAI08(vector<vector<Double> >& marginals);
	void printMarginalsUAI08(vector<Function>& marginals);
	void deleteEmptyFunctions();

};



#endif
