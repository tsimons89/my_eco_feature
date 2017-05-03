#include "Gaussian_gene.h"

Gaussian_gene::Gaussian_gene() {
	int r = (int)(uni() * 5);
	ksize = r * 2 + 3;
	gene_type = GAUSSIAN_GENE;
}

Gaussian_gene::Gaussian_gene(int k) {
	ksize = k;
	gene_type = GAUSSIAN_GENE;
}

Gaussian_gene* Gaussian_gene::clone() {
	Gaussian_gene* new_gene = new Gaussian_gene(ksize);
	return new_gene;
}

unsigned int Gaussian_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	if (uni() < mutation_rate) {
		num_mutations++;
		int r = (int)(uni() * 5);
		ksize = r * 2 + 3;
	}
	return num_mutations;
}

Mat Gaussian_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	try {
		GaussianBlur(input, dest, Size(ksize, ksize), 0, 0);
	}
	catch (Exception& e) {
		worked = false;
	}
	return dest;
}

string Gaussian_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Gaussian Gene\n\t\tksize = %d\n", ksize);
	string output(tmp);
	return output;
}

void Gaussian_gene::serialize(ofstream& os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out member variables
	os.write(reinterpret_cast<char *>(&ksize), sizeof(int));
}

Gaussian_gene* Gaussian_gene::load_from_file(ifstream & fin) {
	int k;
	fin.read(reinterpret_cast<char*>(&k), sizeof(int));
	return new Gaussian_gene(k);
}

bool Gaussian_gene::operator==(Gaussian_gene& other) const {
	return (ksize == other.ksize);
}
bool Gaussian_gene::operator!=(Gaussian_gene& other) const {
	return !(*this == other);
}

bool Gaussian_gene::operator!= (Gene& other) const {
	Gaussian_gene* h = dynamic_cast<Gaussian_gene*>(&other);
	if (h == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( h != NULL && (*this != *h) );
}
