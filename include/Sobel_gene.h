/*
 * Sobel_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef SOBEL_GENE_H_
#define SOBEL_GENE_H_


#include "Gene.h"

class Sobel_gene : public Gene {
	public:
		int dest_depth;
		int ksize;
		int xorder;
		int yorder;

		Sobel_gene();
		Sobel_gene(int d, int k, int x, int y);
		Sobel_gene* clone();

		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		bool operator== (Sobel_gene& other) const;
		bool operator!= (Sobel_gene& other) const;
		bool operator!= (Gene& other) const;

		/** Write binary version of Sobel_gene to the file output
		 * stream.
		 * @param os file output stream to write to
		 */
		void serialize(ofstream & os);

		/** Load Sobel_gene from file input stream.
		 * Static function so that it can be called anywhere. Does the opposite
		 * of the serialize function. Both functions have to read the parameters
		 * in the same order.
		 * @param fin file input stream to read from.
		 * @return A new Sobel_gene*.
		 * @seealso serialize
		 */
		static Sobel_gene* load_from_file(ifstream & fin);
};


#endif /* SOBEL_GENE_H_ */
