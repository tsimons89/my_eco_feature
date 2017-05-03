#include "Gabor_gene.h"

Gabor_gene::Gabor_gene() {
	theta = uni() * 2*M_PI;
	lambda = uni() * 11 + 2; // [2,13)
	sigma = .56 * lambda;
	psi = uni() * 2*M_PI - M_PI;
	gamma = uni() * .5 + .5;
	kernel_size = (int)(uni() * 11) * 2 + 1;
	create_gabor_kernel(sigma, theta, lambda, psi, gamma, kernel_size);
	gene_type = GABOR_GENE;
}

Gabor_gene::Gabor_gene(float s, float t, float l, float p, float g,
		unsigned int k)
{
	sigma = s;
	theta = t;
	lambda = l;
	psi = p;
	gamma = g;
	kernel_size = k;
	create_gabor_kernel(sigma, theta, lambda, psi, gamma, kernel_size);
	gene_type = GABOR_GENE;
}

Gabor_gene* Gabor_gene::clone() {
	Gabor_gene* new_gene = new Gabor_gene(sigma, theta, lambda, psi, gamma, kernel_size);
	return new_gene;
}

unsigned int Gabor_gene::mutate_gene(float mutation_rate) {
	unsigned int num_mutations = 0;
	if (uni() < mutation_rate) {
		num_mutations++;
		theta = uni() * 2*M_PI;
	}
	if (uni() < mutation_rate) {
		num_mutations++;
		lambda = uni() * 11 + 2; // [2,13)
	}
	if (uni() < mutation_rate) {
		num_mutations++;
		psi = uni() * 2*M_PI - M_PI;
	}
	if (uni() < mutation_rate) {
		num_mutations++;
		gamma = uni() * .5 + .5;
	}
	if (uni() < mutation_rate) {
		num_mutations++;
		kernel_size = (int)(uni() * 11) * 2 + 1;
	}
	if (num_mutations > 0) {
		create_gabor_kernel(sigma, theta, lambda, psi, gamma, kernel_size);
	}
	return num_mutations;
}

Mat Gabor_gene::process(Mat& input, bool& worked) {
	worked = true;
	Mat dest;
	try {
		filter2D(input, dest, -1, gabor_kernel, Point(-1,-1), 0,
			   BORDER_REPLICATE);
		// input.copyTo(dest);
	}
	catch (Exception& e) {
		worked = false;
	}
	return dest;
}

string Gabor_gene::to_string() {
	char tmp[1000];
	sprintf(tmp, "Gabor Gene\n\t\tsigma = %f\n\t\ttheta = %f\n\t\t"
			"lambda = %f\n\t\tpsi = %f\n\t\tgamma = %f\n\t\t"
			"kernel_size = %d\n", sigma, theta, lambda, psi, gamma,
			kernel_size);
	string output(tmp);
	return output;
}

void Gabor_gene::serialize(ofstream & os) {
	// write out type of gene this is
	os.write(reinterpret_cast<char *>(&gene_type), sizeof(int));

	// Write out parameters
	os.write(reinterpret_cast<char *>(&sigma), sizeof(float));
	os.write(reinterpret_cast<char *>(&theta), sizeof(float));
	os.write(reinterpret_cast<char *>(&lambda), sizeof(float));
	os.write(reinterpret_cast<char *>(&psi), sizeof(float));
	os.write(reinterpret_cast<char *>(&gamma), sizeof(float));
	os.write(reinterpret_cast<char *>(&kernel_size), sizeof(float));

}

Gabor_gene* Gabor_gene::load_from_file(ifstream & fin) {
	float tmp_sigma, tmp_theta, tmp_lambda, tmp_psi, tmp_gamma;
	unsigned int tmp_kernel_size;
	fin.read(reinterpret_cast<char*>(&tmp_sigma), sizeof(float));
	fin.read(reinterpret_cast<char*>(&tmp_theta), sizeof(float));
	fin.read(reinterpret_cast<char*>(&tmp_lambda), sizeof(float));
	fin.read(reinterpret_cast<char*>(&tmp_psi), sizeof(float));
	fin.read(reinterpret_cast<char*>(&tmp_gamma), sizeof(float));
	fin.read(reinterpret_cast<char*>(&tmp_kernel_size), sizeof(int));
	return new Gabor_gene(tmp_sigma, tmp_theta, tmp_lambda, tmp_psi,
		   tmp_gamma, tmp_kernel_size);
}

bool Gabor_gene::operator==(Gabor_gene& other) const {
	return (sigma == other.sigma && theta == other.theta &&
			lambda == other.lambda && psi == other.psi &&
			gamma == other.gamma && kernel_size == other.kernel_size);
}
bool Gabor_gene::operator!=(Gabor_gene& other) const {
	return !(*this == other);
}

bool Gabor_gene::operator!=(Gene& other) const {
	Gabor_gene* g = dynamic_cast<Gabor_gene*>(&other);
	if (g == NULL) {
		cerr << "dynamic cast didn't work\n";
	}
	return ( g != NULL && (*this != *g) );
}

void Gabor_gene::create_gabor_kernel(float sigma,
		float theta,
		float lambda,
		float psi,
		float gamma,
		int kernel_size)
{
	gabor_kernel = Mat(kernel_size, kernel_size, CV_32F);
	for(int x = 0; x < kernel_size; x++) {
		for (int y = 0; y < kernel_size; y++) {
			float x_prime = x * cos(theta) + y * sin(theta);
			float y_prime = -1 * x * sin(theta) + y * cos(theta);
			float g = exp(-1 * (x_prime * x_prime + gamma * gamma *
						y_prime * y_prime) / (2 * sigma * sigma)) * cos(2 *
						M_PI * (x_prime / lambda) + psi);
			gabor_kernel.at<float>(y,x) = g;
		}
	}
}

Gabor_gene::~Gabor_gene() {
}
