/*
 * Median_blur_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef MEDIAN_BLUR_GENE_H_
#define MEDIAN_BLUR_GENE_H_


#include "Gene.h"

class Median_blur_gene : public Gene {
	public:
		// Using kernel size to reduce dimentions. Sigma computed from ksize.
		int ksize;

		Median_blur_gene();
		Median_blur_gene(int k);
		Median_blur_gene* clone();

		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		bool operator== (Median_blur_gene& other) const;
		bool operator!= (Median_blur_gene& other) const;
		bool operator!= (Gene& other) const;

		/** Write binary version of Median_blur_gene to the file output
		 * stream.
		 * @param os file output stream to write to
		 */
		void serialize(ofstream & os);

		/** Load Median_blur_gene from file input stream.
		 * Static function so that it can be called anywhere. Does the opposite
		 * of the serialize function. Both functions have to read the parameters
		 * in the same order.
		 * @param fin file input stream to read from.
		 * @return A new Median_blur_gene*.
		 * @seealso serialize
		 */
		static Median_blur_gene* load_from_file(ifstream & fin);

};


#endif /* MEDIAN_BLUR_GENE_H_ */
