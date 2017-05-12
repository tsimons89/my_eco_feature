#ifndef BLUR_GENE_H_
#define BLUR_GENE_H_


#include "Gene.h"

class Blur_gene : public Gene {
	public:
		// Using kernel size to reduce dimentions. Sigma computed from ksize.
		int ksize;

		Blur_gene();
		Blur_gene(int k);
		Blur_gene* clone();

		unsigned int mutate_gene(float mutation_rate);
		Mat process(Mat& input, bool& worked);
		string to_string();

		bool operator== (Blur_gene& other) const;
		bool operator!= (Blur_gene& other) const;
		bool operator!= (Gene& other) const;

		/** Write binary version of Blur_gene to the file output
		 * stream.
		 * @param os file output stream to write to
		 */
		void serialize(ofstream & os);

		/** Load Blur_gene from file input stream.
		 * Static function so that it can be called anywhere. Does the opposite
		 * of the serialize function. Both functions have to read the parameters
		 * in the same order.
		 * @param fin file input stream to read from.
		 * @return A new Blur_gene*.
		 * @seealso serialize
		 */
		static Blur_gene* load_from_file(ifstream & fin);

};


#endif /* BLUR_GENE_H_ */
