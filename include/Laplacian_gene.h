/*
 * Laplacian_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef LAPLACIAN_GENE_H_
#define LAPLACIAN_GENE_H_

#include "Gene.h"

class Laplacian_gene : public Gene {
	public:
		int ksize;

		Laplacian_gene();
		Laplacian_gene(int k);
		Laplacian_gene* clone();

		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		bool operator== (Laplacian_gene& other) const;
		bool operator!= (Laplacian_gene& other) const;
		bool operator!= (Gene& other) const;

		/** Write binary version of Laplacian_gene to the file output
		 * stream.
		 * @param os file output stream to write to
		 */
		void serialize(ofstream & os);

		/** Load Laplacian_gene from file input stream.
		 * Static function so that it can be called anywhere. Does the opposite
		 * of the serialize function. Both functions have to read the parameters
		 * in the same order.
		 * @param fin file input stream to read from.
		 * @return A new Laplacian_gene*.
		 * @seealso serialize
		 */
		static Laplacian_gene* load_from_file(ifstream & fin);
};



#endif /* LAPLACIAN_GENE_H_ */
