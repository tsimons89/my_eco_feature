#include "Gradient_gene.h"

void Gradient_gene::create_kernel(int t) {
	if (t == -1) {
		t = (int)(uni() * 7);
	}
	switch (t) {
		case 0: // Prewitt x gradient
			gradient_kernel = Mat(3, 3, CV_32F);
			gradient_kernel.at<float>(0,0) = -1;
			gradient_kernel.at<float>(0,1) =  0;
			gradient_kernel.at<float>(0,2) =  1;
			gradient_kernel.at<float>(1,0) = -1;
			gradient_kernel.at<float>(1,1) =  0;
			gradient_kernel.at<float>(1,2) =  1;
			gradient_kernel.at<float>(2,0) = -1;
			gradient_kernel.at<float>(2,1) =  0;
			gradient_kernel.at<float>(2,2) =  1;
			break;
		case 1: // Prewitt y gradient
			gradient_kernel = Mat(3, 3, CV_32F);
			gradient_kernel.at<float>(0,0) = -1;
			gradient_kernel.at<float>(0,1) = -1;
			gradient_kernel.at<float>(0,2) = -1;
			gradient_kernel.at<float>(1,0) =  0;
			gradient_kernel.at<float>(1,1) =  0;
			gradient_kernel.at<float>(1,2) =  0;
			gradient_kernel.at<float>(2,0) =  1;
			gradient_kernel.at<float>(2,1) =  1;
			gradient_kernel.at<float>(2,2) =  1;
			break;
		case 2: // Kirsch Compass y
			gradient_kernel = Mat(3, 3, CV_32F);
			gradient_kernel.at<float>(0,0) =  3;
			gradient_kernel.at<float>(0,1) =  3;
			gradient_kernel.at<float>(0,2) =  3;
			gradient_kernel.at<float>(1,0) =  3;
			gradient_kernel.at<float>(1,1) =  0;
			gradient_kernel.at<float>(1,2) =  3;
			gradient_kernel.at<float>(2,0) = -5;
			gradient_kernel.at<float>(2,1) = -5;
			gradient_kernel.at<float>(2,2) = -5;
			break;
		case 3: // Kirsch Compass x
			gradient_kernel = Mat(3, 3, CV_32F);
			gradient_kernel.at<float>(0,0) = -5;
			gradient_kernel.at<float>(0,1) =  3;
			gradient_kernel.at<float>(0,2) =  3;
			gradient_kernel.at<float>(1,0) = -5;
			gradient_kernel.at<float>(1,1) =  0;
			gradient_kernel.at<float>(1,2) =  3;
			gradient_kernel.at<float>(2,0) = -5;
			gradient_kernel.at<float>(2,1) =  3;
			gradient_kernel.at<float>(2,2) =  3;
			break;
		case 4: // Kirsch Compass diag
			gradient_kernel = Mat(3, 3, CV_32F);
			gradient_kernel.at<float>(0,0) =  3;
			gradient_kernel.at<float>(0,1) =  3;
			gradient_kernel.at<float>(0,2) =  3;
			gradient_kernel.at<float>(1,0) = -5;
			gradient_kernel.at<float>(1,1) =  0;
			gradient_kernel.at<float>(1,2) =  3;
			gradient_kernel.at<float>(2,0) = -5;
			gradient_kernel.at<float>(2,1) = -5;
			gradient_kernel.at<float>(2,2) =  3;
			break;
		case 5: // Simple x
			//float tmp[9] = {-1,0,1};
			gradient_kernel = Mat(1, 3, CV_32F);
			gradient_kernel.at<float>(0,0) = -1;
			gradient_kernel.at<float>(0,1) =  0;
			gradient_kernel.at<float>(0,2) =  1;
			break;
		case 6: // Simple y
			//float tmp[9] = {-1,0,1};
			gradient_kernel = Mat(3, 1, CV_32F);
			gradient_kernel.at<float>(0,0) = -1;
			gradient_kernel.at<float>(1,0) =  0;
			gradient_kernel.at<float>(2,0) =  1;
			break;
	}
	type = t;
}

Gradient_gene::Gradient_gene() {
	create_kernel(-1);
	gene_type = GRADIENT_GENE;
}

Gradient_gene::Gradient_gene(int t) {
	create_kernel(t);
	gene_type = GRADIENT_GENE;
}

Gradient_gene* Gradient_gene::clone() {
	Gradient_gene* new_gene = new Gradient_gene(type);
	return new_gene;
}

unsigned int Gradient_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	if (uni() < mutation_rate) {
		create_kernel(-1);
		num_mutations++;
	}
	return num_mutations;
}

Mat Gradient_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	try {
		filter2D(input, dest, -1, gradient_kernel, Point(-1,-1), 0,
			   BORDER_REPLICATE);
	}
	catch (Exception& e) {
		worked = false;
	}
	return dest;
}

string Gradient_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Gradient Gene\n\t\ttype = %d\n", type);
	string output(tmp);
	return output;
}

void Gradient_gene::serialize(ofstream & os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out parameters
	os.write(reinterpret_cast<char *>(&type), sizeof(int));
}

Gradient_gene* Gradient_gene::load_from_file(ifstream & fin) {
	int t;
	fin.read(reinterpret_cast<char*>(&t), sizeof(int));
	return new Gradient_gene(t);
}

bool Gradient_gene::operator==(Gradient_gene& other) const {
	return (type == other.type);
}
bool Gradient_gene::operator!=(Gradient_gene& other) const {
	return !(*this == other);
}

bool Gradient_gene::operator!=(Gene& other) const {
	Gradient_gene* g = dynamic_cast<Gradient_gene*>(&other);
	if (g == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( g != NULL && (*this != *g) );
}

Gradient_gene::~Gradient_gene() {
}
