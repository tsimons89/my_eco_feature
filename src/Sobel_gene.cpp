#include "Sobel_gene.h"

Sobel_gene::Sobel_gene() {
	// Get destination depth of CV_8U or CV_32F
	int r = (int)(uni() * 2);
	/*if (r == 0) dest_depth = CV_8U;
	else dest_depth = CV_32F;*/
	dest_depth = CV_32F;

	// Choose kernel size 1,3,5,7,CV_SHARR
	r = (int)(uni() * 5);
	if (r == 0)	ksize = 1;
	else if (r == 1) ksize = 3;
	else if (r == 2) ksize = 5;
	else if (r == 3) ksize = 7;
	else if (r == 4) ksize = CV_SCHARR;
	else {
		cout << "Bug picking Sobel kernel size\n";
		exit(-1);
	}

	// Choose xorder and yorder. Only allowing 0 or 1 for orders
	xorder = (int)(uni() * 2);
	// With CV_SCHARR the orders have to be 0,1 or 1,0
	if (ksize == CV_SCHARR) {
		yorder = 1 - xorder;
	}
	else {
		// Can't have orders 0,0
		if (xorder == 0) yorder = 1;
		else yorder = (int)(uni() *2);
	}
	gene_type = SOBEL_GENE;
}

Sobel_gene::Sobel_gene(int d, int k, int x, int y) {
	dest_depth = d; ksize = k; xorder = x; yorder = y;
	gene_type = SOBEL_GENE;
}

Sobel_gene* Sobel_gene::clone() {
	Sobel_gene* new_gene = new Sobel_gene(dest_depth, ksize, xorder, yorder);
	return new_gene;
}

unsigned int Sobel_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	//if (uni() < mutation_rate) {
	//	num_mutations++;
	//	int r = (int)(uni() * 2);
	//	if (r == 0) dest_depth = CV_8U;
	//	else dest_depth = CV_32F;
	//}

	if (uni() < mutation_rate) {
		num_mutations++;

		// Do both ksize and orders so there are dependancies
		// Choose kernel size 1,3,5,7,CV_SHARR
		int r = (int)(uni() * 5);
		if (r == 0)	ksize = 1;
		else if (r == 1) ksize = 3;
		else if (r == 2) ksize = 5;
		else if (r == 3) ksize = 7;
		else if (r == 4) ksize = CV_SCHARR;
		else {
			cout << "Bug picking Sobel kernel size\n";
			exit(-1);
		}

		// Choose xorder and yorder. Only allowing 0 or 1 for orders
		xorder = (int)(uni() * 2);
		// With CV_SCHARR the orders have to be 0,1 or 1,0
		if (ksize == CV_SCHARR) {
			yorder = 1 - xorder;
		}
		else {
			// Can't have orders 0,0
		if (xorder == 0) yorder = 1;
		else yorder = (int)(uni() *2);
		}
	}
	return num_mutations;
}

Mat Sobel_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	try {
		Sobel(input, dest, dest_depth, xorder, yorder, ksize);
	}
	catch(Exception& e) {
		worked = false;
	}
	return dest;
}

string Sobel_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Sobel Gene\n\t\tdest_depth = %d\n\t\tksize = %d\n\t\t"
			"xorder = %d\n\t\tyorder = %d\n", dest_depth, ksize, xorder, yorder);
	string output(tmp);
	return output;
}

void Sobel_gene::serialize(ofstream& os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out member variables
	os.write(reinterpret_cast<char *>(&dest_depth), sizeof(int));
	os.write(reinterpret_cast<char *>(&ksize), sizeof(int));
	os.write(reinterpret_cast<char *>(&xorder), sizeof(int));
	os.write(reinterpret_cast<char *>(&yorder), sizeof(int));
}

Sobel_gene* Sobel_gene::load_from_file(ifstream & fin) {
	int d;
	int k;
	int x;
	int y;
	fin.read(reinterpret_cast<char*>(&d), sizeof(int));
	fin.read(reinterpret_cast<char*>(&k), sizeof(int));
	fin.read(reinterpret_cast<char*>(&x), sizeof(int));
	fin.read(reinterpret_cast<char*>(&y), sizeof(int));
	return new Sobel_gene(d,k,x,y);
}

bool Sobel_gene::operator==(Sobel_gene& other) const {
	return (dest_depth == other.dest_depth && ksize == other.ksize &&
			xorder == other.xorder && yorder == other.yorder);
}

bool Sobel_gene::operator!=(Sobel_gene& other) const {
	return !(*this == other);
}

bool Sobel_gene::operator!= (Gene& other) const {
	Sobel_gene* h = dynamic_cast<Sobel_gene*>(&other);
	if (h == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( h != NULL && (*this != *h) );
}
