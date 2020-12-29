#ifndef JT_H_
#define JT_H_

#include <vector>
#include "GM.h"
#include "Double.h"
#include "LogFunction.h"
#include "JG.h"
struct JT
{
	vector<Function> marginals;
	vector<Variable*> variables;
	vector<JGNode*> nodes;
	vector<JGEdge*> edges;
	vector<JGEdge*> message_order;
	vector<JGNode*> var_to_node;
	JT(vector<Variable*>& variables_, vector<Function*>& functions, vector<set<int> >& clusters, vector<int>& order);
	void propagate();
	
};
#endif