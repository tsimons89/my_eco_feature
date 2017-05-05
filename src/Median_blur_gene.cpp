#include "Median_blur_gene.h"

Median_blur_gene::Median_blur_gene() {
	int r = (int)(uni() * 5);
	ksize = r * 2 + 3;
	gene_type = MEDIAN_BLUR_GENE;
}

Median_blur_gene::Median_blur_gene(int k) {
	ksize = k;
	gene_type = MEDIAN_BLUR_GENE;
}

Median_blur_gene* Median_blur_gene::clone() {
	Median_blur_gene* new_gene = new Median_blur_gene(ksize);
	return new_gene;
}

unsigned int Median_blur_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	return num_mutations;
}

Mat Median_blur_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	input.convertTo(input,CV_8U);
	try {
		medianBlur(input, dest, ksize);
	}
	catch (Exception& e) {
		worked = false;
	}
	return dest;
}

string Median_blur_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Median_blur Gene\n\t\tksize = %d\n", ksize);
	string output(tmp);
	return output;
}

void Median_blur_gene::serialize(ofstream& os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out member variables
	os.write(reinterpret_cast<char *>(&ksize), sizeof(int));
}

Median_blur_gene* Median_blur_gene::load_from_file(ifstream & fin) {
	int k;
	fin.read(reinterpret_cast<char*>(&k), sizeof(int));
	return new Median_blur_gene(k);
}

bool Median_blur_gene::operator==(Median_blur_gene& other) const {
	return (ksize == other.ksize);
}
bool Median_blur_gene::operator!=(Median_blur_gene& other) const {
	return !(*this == other);
}

bool Median_blur_gene::operator!= (Gene& other) const {
	Median_blur_gene* h = dynamic_cast<Median_blur_gene*>(&other);
	if (h == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( h != NULL && (*this != *h) );
}
