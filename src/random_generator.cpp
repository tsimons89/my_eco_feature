#include <random_generator.h>


// Boost random number generator and distribution
boost::mt19937 generator;



/*
If I put the code block above in the source file, there will be problems, why is that?
*/
boost::normal_distribution<> norm_dist(0.0, 1.0);
boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > nor(generator, norm_dist);


boost::uniform_01<> uniform_dist;
boost::variate_generator<boost::mt19937&, boost::uniform_01<> > uni(generator, uniform_dist);
