#include "Gene.h"

Gene::Gene() {
}

Gene* Gene::clone() {
	cerr << "Error: Called base clase clone!\n";
	exit(-1);
}

unsigned int Gene::mutate_gene(float mutation_rate) {
	cerr << "Error: Called base clase mutate_gene!\n";
	exit(-1);
}

Mat Gene::process(Mat& input, bool& worked) {
	cerr << "Error: Called base class process!\n";
	exit(-1);
}

string Gene::to_string() {
	cerr << "Error: Called base class to_string!\n";
	exit(-1);
}

void Gene::serialize(ofstream & os) {
	cerr << "Error: Called base class serialize!\n";
	exit(-1);
}

bool Gene::operator==(Gene& other) const {
	cerr << "Error: Called base class operator==!\n";
	exit(-1);
}

bool Gene::operator!=(Gene& other) const {
	cerr << "Error: Called base class operator!=\n";
	exit(-1);
}

string make_gene_type_readable(int gene_type) {
	switch (gene_type) {
		case GABOR_GENE:
			return "Gabor";
		case SOBEL_GENE:
			return "Sobel";
		case GAUSSIAN_GENE:
			return "Gauss";
		case MEDIAN_BLUR_GENE:
			return "Med Blur";
		case LAPLACIAN_GENE:
			return "Lapl";
		case GRADIENT_GENE:
			return "Gradient";
		default:
			cout << "Can't decode gene type!!, " << gene_type << "\n";
			exit(-1);
	}	
	return "Don't know gene type in make_gene_type_readable";
}

Gene::~Gene() {
}
