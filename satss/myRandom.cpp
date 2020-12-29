#include "myRandom.h"
#include <climits>

unsigned int myRandom::seed=0;
BaseGenerator myRandom::base;
RealDistribution myRandom::real_dist;
IntDistribution myRandom::int_dist(0,INT_MAX);
RealGenerator myRandom::genDoubleVal= RealGenerator(base,real_dist);
IntGenerator myRandom::genIntVal=IntGenerator(base,int_dist);
