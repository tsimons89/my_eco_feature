#include "Laplacian_gene.h"

Laplacian_gene::Laplacian_gene() {
	ksize = (int)(uni() * 7) * 2 + 1;

	gene_type = LAPLACIAN_GENE;
}

Laplacian_gene::Laplacian_gene(int k) {
	ksize = k;
	gene_type = LAPLACIAN_GENE;
}

Laplacian_gene* Laplacian_gene::clone() {
	Laplacian_gene* new_gene = new Laplacian_gene(ksize);
	return new_gene;
}

unsigned int Laplacian_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;

	if (uni() < mutation_rate) {
		num_mutations++;
		ksize = (int)(uni() * 7) * 2 + 1;
	}
	return num_mutations;
}

Mat Laplacian_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	try {
		Laplacian(input, dest, input.type(), ksize);
	}
	catch(Exception& e) {
		worked = false;
	}
	return dest;
}

string Laplacian_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Laplacian Gene\n\t\t"
			"ksize = %d\n", ksize);
	string output(tmp);
	return output;
}

void Laplacian_gene::serialize(ofstream& os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out member variables
	os.write(reinterpret_cast<char *>(&ksize), sizeof(int));
}

Laplacian_gene* Laplacian_gene::load_from_file(ifstream& fin) {
	int k;
	fin.read(reinterpret_cast<char*>(&k), sizeof(int));
	return new Laplacian_gene(k);
}

bool Laplacian_gene::operator==(Laplacian_gene& other) const {
	return (ksize == other.ksize);
}

bool Laplacian_gene::operator!=(Laplacian_gene& other) const {
	return !(*this == other);
}

bool Laplacian_gene::operator!=(Gene& other) const {
	Laplacian_gene* h = dynamic_cast<Laplacian_gene*>(&other);
	if (h == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return (h != NULL && (*this != *h));
}
