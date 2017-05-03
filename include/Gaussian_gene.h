/*
 * Gaussian_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef GAUSSIAN_GENE_H_
#define GAUSSIAN_GENE_H_


#include "Gene.h"

class Gaussian_gene : public Gene {
	public:
		// Using kernel size to reduce dimentions. Sigma computed from ksize.
		int ksize;

		Gaussian_gene();
		Gaussian_gene(int k);
		Gaussian_gene* clone();

		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		bool operator== (Gaussian_gene& other) const;
		bool operator!= (Gaussian_gene& other) const;
		bool operator!= (Gene& other) const;

		/** Write binary version of Gaussian_gene to the file output
		 * stream.
		 * @param os file output stream to write to
		 */
		void serialize(ofstream & os);

		/** Load Gaussian_gene from file input stream.
		 * Static function so that it can be called anywhere. Does the opposite
		 * of the serialize function. Both functions have to read the parameters
		 * in the same order.
		 * @param fin file input stream to read from.
		 * @return A new Gaussian_gene*.
		 * @seealso serialize
		 */
		static Gaussian_gene* load_from_file(ifstream & fin);

};


#endif /* GAUSSIAN_GENE_H_ */
