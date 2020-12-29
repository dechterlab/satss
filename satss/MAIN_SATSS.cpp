#include <iostream>
#include <fstream>
#include "BE.h"
#include "GM.h"
#include "Graph.h"
#include "RB_IJGP_Sampler.h"
#include "SampleSearch.h"

using namespace std;

//#define IBOUNDLIMIT 3000 //65646
//#define LIMIT 1000 //0000


#include <sys/time.h>
#include <sys/resource.h>
static inline double get_cpu_time(void) 
{
  struct rusage ru;
  getrusage(RUSAGE_SELF, &ru);
  return (double)ru.ru_utime.tv_sec + (double)ru.ru_utime.tv_usec / 1000000; 
}

int help(){
	cerr<<"Format is : satss [options] filename\n";
	cerr<<"Setting proper options is very important, otherwise you will end up with bad results\n"; 
	cerr<<"The way you set options is by using --option value\n";
	cerr<<"Possible options\n";
	cerr<<" \t--max-restarts value (default value is 1)\n";
	cerr<<" \t--i-bound value (default value is 12)\n";
	cerr<<" \t--rb-bound value (default value is 7, stands for rao-blackwellised bound)\n";
	cerr<<" \t--task [sampling|counting] (default value is counting)\n";
	cerr<<" \t--ordering [minfill|mindegree|lex] (default value is minfill )\n";
	cerr<<" \t--c-bound value (default is 15), use to control large clause size, clauses beyong this size are ignored by IJGP\n";
	cerr<<" \t--ijgp-damp value (default value is 0.9)\n";
	cerr<<" \t--iterations value (default value is 10)\n";
	cerr<<" \t--seed value (default value is 123456789 )\n";
	cerr<<"The following options are just for storing results\n";
	cerr<<" \t--interval value (default value is 100)\n";
	cerr<<" \t--out filename (default value is out.ss)\n";
	
	exit(1);
}
// Parameters
int i_bound=12;
int p_bound=0,w_bound=0;
int rb_bound=7;
// We will stop after a million samples
int num_samples=10000000;
// The time limit is ignored, we run forever or until a million samples are drawn
int time_limit=10;
// The number of iterations of IJGP is set to 10
int num_iterations=10,
// The number of restarts is set to 1
max_restarts=10;
int c_bound=15;
double damp=0.9;
unsigned int seed = 123456789;

string ordering="minfill";
string outfilename="out";
string task="counting";
int interval=100;
string satfilename;

void readParameters(int argc, char* argv[])
{
	int optind=1;
   	while (optind < (argc-1)) {
    	string sw = argv[optind];
     	if(sw == "--max-restarts"){
        	optind++;
        	max_restarts=atoi(argv[optind]);
     	}
     	else if (sw == "--i-bound"){
     		optind++;
     		i_bound=atoi(argv[optind]);
     	}
	else if (sw=="--iterations"){
	  optind++;
	  num_iterations=atoi(argv[optind]);
	}
     	else if (sw == "--rb-bound"){
     		optind++;
     		rb_bound=atoi(argv[optind]);
     	}
	else if (sw == "--c-bound"){
	  optind++;
	  c_bound=atoi(argv[optind]);
	}
     	else if (sw == "--ordering"){
     		optind++;
     		ordering=argv[optind];
     	}
     	else if (sw == "--ijgp-damp"){
     		optind++;
     		damp=atof(argv[optind]);		
     	}
     	else if (sw == "--interval"){
     		optind++;
     		interval=atoi(argv[optind]);
     	}
	else if (sw=="--p-bound"){
	  optind++;
	  p_bound=atoi(argv[optind]);
	}
     	else if (sw == "--out"){
     		optind++;
     		outfilename=argv[optind];
     	}
	else if (sw == "--task"){
	        optind++;
		task=argv[optind];
	}
     	else if (sw == "--seed"){
     		optind++;
     		seed=atoi(argv[optind]);
     	}
     	else if (sw == "--help"){
     		help();
     	}
     	else{
     		help();
     	}
    	optind++;
   }
   if (argc < 2){
   	help();
   }
   satfilename=argv[argc-1];
}
int main(int argc, char* argv[])
{
	cout<<"DBL Min = "<<LDBL_MIN*2<<endl;
	// Read the parameters
        readParameters(argc, argv);
	GM gm;
	gm.damp=damp;
	gm.c_bound=c_bound;
	// Random Generation: Start
	myRandom::seed=static_cast<unsigned int> (seed);
	//myRandom::seed=atoi(argv[3]);
	myRandom::base.seed((int)myRandom::seed);
	myRandom::genDoubleVal= RealGenerator(myRandom::base,myRandom::real_dist);
	myRandom::genIntVal=IntGenerator(myRandom::base,myRandom::int_dist);

	// The following is a hack. should be cleaned up a lot.
	if(gm.readCNF(satfilename.c_str())){
		
	  //if(task=="sampling"){
	    gm.reduceSAT(satfilename.c_str());
	    satfilename="temp2.cnf";
	    gm.writeSAT("temp2.cnf");
	    //}
		vector<set<int> > clusters;
		double estimate;
		double limit=pow((double)2.0,(double)rb_bound);
		vector<int> order;
		GM csp;
		if (ordering == "minfill"){
			gm.getMinFillOrdering(order,clusters,estimate);
		}
		else if (ordering == "mindegree"){
			gm.getMinDegreeOrdering(order,clusters,estimate);
		}
		else if (ordering == "lex"){
			gm.getLexOrdering(order,clusters,estimate);
		}
		else {
			cerr<<"Wrong ordering type\n";
			exit(-1);
		}
		vector<int> new_order;
		gm.rearrangeOrdering(order,clusters,new_order,limit);
		gm.deleteEmptyFunctions();
		//Call Bucketelimination
		RBSampleSearchSAT ss;
		//double out=ss.runBE(gm,order,argv[1]);
		//cout<<"Num-solutions = "<<out<<endl;
		//return 1
		cout<<"Interval = "<<interval<<endl;
		ss.interval=interval;
		ss.outfilename=outfilename;		
		cerr<<"#sampled variables = "<<new_order.size()<<" out of "<<gm.variables.size()<<endl;
		//computeIBound(gm,i_bound);
		
		vector<int> ijgp_order;
		vector<bool> is_sampled(order.size());
		for(int i=0;i<new_order.size();i++)
			is_sampled[new_order[i]]=true;
		for(int i=0;i<order.size();i++)
		{
			if (!is_sampled[order[i]]){
				continue;
			}
			ijgp_order.push_back(order[i]);
		}
		for(int i=0;i<order.size();i++)
		{
			if (is_sampled[order[i]]){
				continue;
			}
			ijgp_order.push_back(order[i]);
		}

		JG jg(gm,i_bound,num_iterations,ijgp_order,LSS);
		//cout<<"Join graph constructed\n";
		jg.propagate();
		
		//Double pe1=ss.computePE(gm,jg,p_bound,order,new_order,num_samples);
		if (task =="counting"){
		  Double pe1=ss.computePEApp(gm,jg,satfilename.c_str(),p_bound,order,new_order,num_samples,max_restarts);
		  //Double pe1=ss.computePE(gm,jg,satfilename.c_str(),p_bound,order,new_order,num_samples);
		  pe1*=gm.mult_factor;
		  cerr<<"---------------------------RESULT------------------------------\n";
		  cerr<<"\t Log Base e probability of evidence = "<<log(pe1.value())<<endl;
		  cerr<<"\t Log Base 10 probability of evidence = "<<log(pe1.value())/log((double)10.0)<<endl;
		  cerr<<"\t Probability of evidence = "<<pe1<<endl;
		  cerr<<"---------------------------RESULT------------------------------\n";
		  return 1;
		}
		else if (task=="sampling"){
		  ss.generateSamples(gm,jg,satfilename.c_str(),p_bound,order,new_order,num_samples,max_restarts);
		  
		}
		else {
		  cerr<<"Wrong Task type\n";
		  exit(-1);
		}
	}
	else{
		cerr<<"Not a SAT problem\n";
	}
	// Random Generation: End
}
