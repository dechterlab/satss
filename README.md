# SampleSearch for Approximate Solution Counting
This repository mirrors [software page from Professor Vibhav Gogate at UT Dallas](http://www.hlt.utdallas.edu/~vgogate/SampleSearch.html).

### Papers
* Vibhav Gogate and Rina Dechter, [“Approximate Counting by Sampling the Backtrack-free Search Space,”](http://www.hlt.utdallas.edu/~vgogate/papers/aaai07.pdf) In 22nd Conference on Artificial Intelligence (AAAI), 2007.
* Vibhav Gogate and Rina Dechter, [“SampleSearch: Importance Sampling in presence of Determinism,”](http://www.hlt.utdallas.edu/~vgogate/papers/aij09-samplesearch.pdf) Artificial Intelligence Journal, 2011.

### Usage
```
$ satss [options] < cnf-file >

# The code will output the result in the following form
z < 2.97207e+11,2.97207e+12 > , Storage space = 0.567871MB
```

The two values are upper and lower bounds on the sample mean (see the second Reference paper above). 
The approximate solution count is an average of these two values. The solver is designed to run forever, with the hope that its solution quality improves over time.

**Caution**: 
If you get a segmentation fault saying that the number of sampled variables is zero, it means that your cnf instance is very easy and can be solved using Bucket elimination. 
To run SampleSearch on it, set the rb-bound to 0 (see below).

**Options**: 
Default options are always not that good. The strength of SampleSearch lies in its options, which are detailed below.

The way you set options is by using --option value
1. --max-restarts value (default value is 1. This is used to control the quality of the upper and lower bound of the sample mean. Higher the value smaller is the difference between the upper and lower bounds. This increases time however.)
2. --i-bound value (default value is 12. The i-bound used by IJGP, which is our Generalized Belief propagation scheme.)
3. --rb-bound value (default value is 7, stands for Rao-Blackwellised bound)
4. --task [sampling|counting] (default value is counting. Ignore this option for counting solutions.)
5. --ordering [minfill|mindegree|lex] (default value is minfill. If the number of variables in the problem is large, minfill which has a complexity of n^4 will utilize all the time. Therefore, for larger problems use either mindegree or lex. )
6. --ijgp-damp value (default value is 0.9)
7. --seed value (default value is 123456789 )

The following options are just for storing results
1. --interval value (default value is 100. The number of samples after which z is output. )
2. --out filename (default value is out.ss)


### Example
```
$ satss –ordering mindegree –i-bound 3 –rb-bound 3 –seed 100 2bitmax_6.cnf
```
This will run satss on 2bitmax_6.cnf. The samples will be generated along the (reverse) mindegree ordering. The i-bound is 3, the rb-bound is 3 and the seed is 100.
