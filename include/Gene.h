/*
 * Gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef GENE_H_
#define GENE_H_


#include <fstream>
#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/format.hpp>
#include <boost/random/normal_distribution.hpp>

/*
why does this extern matters for mlp.cpp?
*/
extern boost::variate_generator<boost::mt19937&, boost::uniform_01<> > uni;
extern boost::mt19937 generator;
extern boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > nor;

using namespace std;
using namespace cv;


//image transforms
#define GABOR_GENE 1
#define SOBEL_GENE 2
#define GAUSSIAN_GENE 3//5
#define GRADIENT_GENE 4//7
#define MEDIAN_BLUR_GENE 5//16
#define LAPLACIAN_GENE 6//20


#define NUM_POSSIBLE_GENES 6//27

class Gene {
	public:
		int gene_type;

		Gene();
		virtual ~Gene();
		virtual Gene* clone();
		virtual unsigned mutate_gene(float mutation_rate);
		virtual Mat process(Mat& input, bool& worked);
		virtual string to_string();

		/** Write binary version of Gene to output file stream.
		 * Virtual function that all genes will inherit from and overwrite. The
		 * function here in Gene.cpp should never be called.
		 * @param os the output file stream to write to
		 */
		virtual void serialize(ofstream & os);
		virtual bool operator== (Gene& other) const;
		virtual bool operator!= (Gene& other) const;
};

string make_gene_type_readable(int gene_type);


#endif /* GENE_H_ */
