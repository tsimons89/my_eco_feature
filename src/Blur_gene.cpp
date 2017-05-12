#include "Blur_gene.h"

Blur_gene::Blur_gene() {
	int r = (int)(uni() * 5);
	ksize = r * 2 + 3;
	gene_type = BLUR_GENE;
}

Blur_gene::Blur_gene(int k) {
	ksize = k;
	gene_type = BLUR_GENE;
}

Blur_gene* Blur_gene::clone() {
	Blur_gene* new_gene = new Blur_gene(ksize);
	return new_gene;
}

unsigned int Blur_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	return num_mutations;
}

Mat Blur_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	input.convertTo(input,CV_8U);
	try {
		blur(input, dest, Size(ksize,ksize));
	}
	catch (Exception& e) {
		worked = false;
	}
	return dest;
}

string Blur_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Blur Gene\n\t\tksize = %d\n", ksize);
	string output(tmp);
	return output;
}

void Blur_gene::serialize(ofstream& os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out member variables
	os.write(reinterpret_cast<char *>(&ksize), sizeof(int));
}

Blur_gene* Blur_gene::load_from_file(ifstream & fin) {
	int k;
	fin.read(reinterpret_cast<char*>(&k), sizeof(int));
	return new Blur_gene(k);
}

bool Blur_gene::operator==(Blur_gene& other) const {
	return (ksize == other.ksize);
}
bool Blur_gene::operator!=(Blur_gene& other) const {
	return !(*this == other);
}

bool Blur_gene::operator!= (Gene& other) const {
	Blur_gene* h = dynamic_cast<Blur_gene*>(&other);
	if (h == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( h != NULL && (*this != *h) );
}
