/*
 * Gradient_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef GRADIENT_GENE_H_
#define GRADIENT_GENE_H_


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#include <stdio.h>

#include "Gene.h"

class Gradient_gene : public Gene {
	public:
		Mat gradient_kernel;
		int type;

		Gradient_gene();
		Gradient_gene(int t);
		~Gradient_gene();
		Gradient_gene* clone();
		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();
		void create_kernel(int i);

		/** Write Gradient_gene to output file stream.
		 * Write Gradient_gene in binary format to the output file stream.
		 * @param os output file stream
		 */
		void serialize(ofstream & os);

		/** Load Gradient_gene from an input file stream
		 * Static function so that it can be called anywhere
		 * @param fin input file stream to read from
		 * @return New Gradient_gene
		 */
		static Gradient_gene* load_from_file(ifstream & fin);
		bool operator== (Gradient_gene& other) const;
		bool operator!= (Gradient_gene& other) const;
		bool operator!=(Gene& other) const;

};


#endif /* GRADIENT_GENE_H_ */
