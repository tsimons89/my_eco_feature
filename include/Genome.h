/*
 * Genome.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef GENOME_H_
#define GENOME_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>

#include "Gene.h"
#include "Gabor_gene.h"
#include "Sobel_gene.h"
#include "Gaussian_gene.h"
#include "Median_blur_gene.h"
#include "Laplacian_gene.h"
#include "Gradient_gene.h"
#include "Blur_gene.h"

using namespace std;
using namespace cv;

#define MAX_NUMBER_GENES 6 //


class Genome {
	public:
		CvRect apply_rect; /**< Location on image to process */
		vector<Gene*>* genes;

		/** Create new Genome.
		 * Does not randomly generate any parameters of the Genome.
		 */
		Genome();

		/** Create new Genome.
		 * Will randomly generate where on the image we will processs.
		 * @param add_genes indicates whether to randomly generate genes.
		 */
		Genome(bool add_genes);

		/** Copy constructor
		 * @param g Genome to copy
		 */
		Genome(Genome* g);

		/** Destructor
		 */
		~Genome();

		/** Apply genes in Genome to an image
		 * @param image image to process
		 * @param worked whether the genome could be process correctly
		 * @ return pointer to the processed image
		 */
		Mat process(Mat& image, bool& worked);

		/* Get Gene at a certain index
		 * @param index index of the Gene wanted
		 * @return pointer to Gene
		 */
		//Gene* get_gene(unsigned index);

		/** Get a string representation of Genome
		* @return string that represents Genome
		*/
		string to_string();

		/** Mutate apply_rect. Normally when mutating variables we want to do
		 * it at the bit level but it is important here that the apply_rect
		 * still be on the image is 64x128 in size. So we do mutations at a
		 * higher level.
		 * @param mutation_rate Probability that a mutation will happen.
		 */
		unsigned int mutate_apply_rect(float mutation_rate);
		void select_apply_rect(int image_width, int image_height);

		/** Mutate the genome
		 * @param mutation_rate probability of mutation
		 * @return the number of mutations that occurred
		 */
		unsigned int mutate_genome(float mutation_rate);
		Gene* create_gene(int gene_type);
		bool operator == (Genome& other) const;

		/** Load the Genome from an input stream
		 * Static function so it can be called anywhere. Will call the
		 * load_from file on the genes that make up the Genome.
		 * @param fin the stream to read from
		 * @returns a new Genome
		 */
		static Genome* load_from_file(ifstream & fin);

		static Genome* copy_genes(Genome*);

		/** Write the Genome to an output file stream
		 * Will call the serialize function on genes that make up the Genome.
		 * @param os file stream to write to
		 */
		void serialize(ofstream & os);

		bool valid();
};



#endif /* GENOME_H_ */
