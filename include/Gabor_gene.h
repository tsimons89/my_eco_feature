/*
 * Gabor_gene.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef GABOR_GENE_H_
#define GABOR_GENE_H_


#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <time.h>
#include <stdio.h>

#include "Gene.h"

class Gabor_gene : public Gene {
	public:
		float sigma;
		float theta;
		float lambda;
		float psi;
		float gamma;
		unsigned int kernel_size;
		Mat gabor_kernel;

		Gabor_gene();
		Gabor_gene(float s, float t, float l, float p, float g,
				unsigned int k);
		~Gabor_gene();
		Gabor_gene* clone();
		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		/** Write Gabor_gene to output file stream.
		 * Write Gabor_gene in binary format to the output file stream.
		 * @param os output file stream
		 */
		void serialize(ofstream & os);

		/** Load Gabor_gene from an input file stream
		 * Static function so that it can be called anywhere
		 * @param fin input file stream to read from
		 * @return New Gabor_gene
		 */
		static Gabor_gene* load_from_file(ifstream & fin);
		bool operator== (Gabor_gene& other) const;
		bool operator!= (Gabor_gene& other) const;
		bool operator!=(Gene& other) const;

		/** Create gabor filter with given parameters
		 * @param sigma smoothing parameter
		 * @param theta orientation parameter
		 * @param lambda wavelength parameter
		 * @param psi
		 * @param gamma
		 * @param kernel_size size of kernel to apply
		 */
		void create_gabor_kernel(float sigma,
				float theta,
				float lambda,
				float psi,
				float gamma,
				int kernel_size);
};


#endif /* GABOR_GENE_H_ */
