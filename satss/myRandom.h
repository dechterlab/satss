#ifndef MY_RANDOM_H_
#define MY_RANDOM_H_

#include <boost/random.hpp>
#include <cstdlib>
#include <iostream>
#include <climits>
using namespace std;
using namespace boost;

typedef boost::mt19937 BaseGenerator;
typedef boost::uniform_real<double> RealDistribution;
typedef boost::variate_generator<BaseGenerator,RealDistribution> RealGenerator;
typedef boost::uniform_int<int> IntDistribution;
typedef boost::variate_generator<BaseGenerator,IntDistribution> IntGenerator;

class myRandom
{
public:
	static unsigned int seed;
	static BaseGenerator base;
	static RealDistribution real_dist;
	static IntDistribution int_dist;
	static RealGenerator genDoubleVal;
	static IntGenerator genIntVal;

public:
	myRandom()
	{
		//seed=time(NULL);
		//seed=100L;
		//srand(seed);
	}
	double getDouble()
	{ 
	  return genDoubleVal();
	
	}
	int getInt()
	{
	
	  return genIntVal();
	
	}
	int getInt(int max_value)
	{
	  return getInt()%max_value;     
	}	
};


#endif /*RANDOM_H_*/
