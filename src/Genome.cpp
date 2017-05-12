#include "Genome.h"

extern Mat example_image;
bool use_subregion;

Genome::Genome() {
	genes = new vector<Gene*>();
}

void Genome::select_apply_rect(int image_width, int image_height) {
	if (use_subregion) {
		apply_rect.x = (int)(uni() * (image_width - 1));
		apply_rect.width = (int)((uni() * ((image_width - 1) - apply_rect.x)) + 1);
		assert(apply_rect.x + apply_rect.width < image_width);

		apply_rect.y = (int)(uni() * (image_height - 1));
		apply_rect.height = (int)((uni() * ((image_height-1) - apply_rect.y)) + 1);
		assert(apply_rect.y + apply_rect.height < image_height);
	}
	else {
		apply_rect.x = 0;
		apply_rect.y = 0;
		apply_rect.width = image_width;
		apply_rect.height = image_height;	
	}
}

Genome* Genome::copy_genes(Genome* copy_from) {
	Genome* new_genome = new Genome();
	int image_width = example_image.cols;
	int image_height = example_image.rows;
	new_genome->select_apply_rect(image_width, image_height);

	bool valid = false;
	while (!valid) {
		// Clear genes vector
		for (unsigned idx = 0; idx < new_genome->genes->size(); idx++) {
			Gene* g = new_genome->genes->at(idx);
			delete g;
			g = NULL;
		}
		new_genome->genes->clear();
		for (uint i = 0; i < copy_from->genes->size(); i++) {
			Gene* g = copy_from->genes->at(i);
			/*
			if (g->gene_type == COLOR_GENE) {
				Color_gene* cg = new Color_gene();
				new_genome->genes->push_back(cg);
			}
			else */
			{
				int gene_type = g->gene_type;
				Gene* new_gene = new_genome->create_gene(gene_type);	
				new_genome->genes->push_back(new_gene);
			}
				
		}
		valid = new_genome->valid();
	}
	return new_genome;
}

Genome::Genome(bool add_genes) {
	genes = new vector<Gene*>();
	if (example_image.empty()) {
		cout << "Example image is empty!\n";
		exit(-1);
	}
	int image_width = example_image.cols;
	int image_height = example_image.rows;

	select_apply_rect(image_width, image_height);

	if (add_genes) {
		bool valid = false;
		while (!valid) {
			// Clear genes vector
			for (unsigned idx = 0; idx < genes->size(); idx++) {
				Gene* g = genes->at(idx);
				delete g;
				g = NULL;
			}
			genes->clear(); /*
			if (example_image.channels() == 3) {
				// Make first gene a color gene
				Color_gene* cg = new Color_gene();
				genes->push_back(cg);
			}*/
			// Add rest of genes
			int gene_count = (uni() * (MAX_NUMBER_GENES - 1)) + 2;
			for (int j = 0; j < gene_count; j++) {
				int gene_type = (uni() * NUM_POSSIBLE_GENES) + 1; // [1, NUM_POSSIBLE_GENES]
				Gene* gene = create_gene(gene_type);
				genes->push_back(gene);
			}
			//before using this section, you have to replace j < gene_count with j < gene_count - 1 above this. 
			valid = this->valid();
		}
	}
}

bool Genome::valid() {
	bool valid = true;
	Mat result;
	assert(!example_image.empty());
	Mat tmp = Mat(example_image, apply_rect).clone();
	assert(!tmp.empty());
	result = process(tmp, valid);
	return valid;
}

Genome::Genome(Genome* g) {
	genes = new vector<Gene*>();
	for (unsigned int i = 0; i < g->genes->size(); i++) {
		genes->push_back(g->genes->at(i)->clone());
	}
	apply_rect.x = g->apply_rect.x;
	apply_rect.y = g->apply_rect.y;
	apply_rect.width = g->apply_rect.width;
	apply_rect.height = g->apply_rect.height;
}

Gene* Genome::create_gene(int gene_type) {
	Gene* g;
	switch(gene_type) {
		case GABOR_GENE:
			g = new Gabor_gene();
			break;
		case SOBEL_GENE:
			g = new Sobel_gene();
			break;
		case GAUSSIAN_GENE:
			g = new Gaussian_gene();
			break;
		case MEDIAN_BLUR_GENE:
			g = new Median_blur_gene();
			break;
		case LAPLACIAN_GENE:
			g = new Laplacian_gene();
			break;
		case GRADIENT_GENE:
			g = new Gradient_gene();
			break;
		case BLUR_GENE:
			g = new Blur_gene();
			break;
		default:
			cout << "Selecting random gene that is out of index " << 
				gene_type << "\n";
			exit(-1);
	}
	return g;
}

Mat Genome::process(Mat& image, bool& worked) {
	Mat tmp = image;
	Mat next_tmp;
	// Apply image processing function contained in genes to the image
	for (unsigned i = 0; i < genes->size(); i++) {
		next_tmp = genes->at(i)->process(tmp, worked);
		tmp = next_tmp;
		/*double min_val, max_val;
		minMaxLoc(next_tmp, &min_val, &max_val);
		next_tmp.convertTo(tmp, CV_32F, 255.0/(max_val - min_val), -min_val * 255.0/(max_val - min_val));*/
	
		if (!worked) {
			return Mat();
		}
	}
	return tmp;
}

unsigned int Genome::mutate_apply_rect(float mutation_rate) {
	unsigned int num_mutations = 0;

	int image_width = example_image.cols;
	int image_height = example_image.rows;

	if (uni() < mutation_rate) {
		num_mutations++;
		select_apply_rect(image_width, image_height);
	}
	return num_mutations;
}

unsigned int Genome::mutate_genome(float mutation_rate) {
	unsigned int num_mutations = 0;
	num_mutations += mutate_apply_rect(mutation_rate);
	for (unsigned i = 0; i < genes->size(); i++) {
		num_mutations += genes->at(i)->mutate_gene(mutation_rate);
	}
	return num_mutations;
}

string Genome::to_string() {
	string output = "\tApply Rect\n";
	char part[1024];
	sprintf(part, "\t\tx: %d\n\t\ty: %d\n\t\twidth: %d\n\t\theight: %d\n\n",
			apply_rect.x, apply_rect.y, apply_rect.width, apply_rect.height);
	output += part;
	for (unsigned int i = 0; i < genes->size(); i++) {
		memset(part, '\0', 1024);
		sprintf(part, "\tGene %d: ", i);
		output += part;
		output += genes->at(i)->to_string() + "\n";
	}
	return output;
}

void Genome::serialize(ofstream & os) {
	// Write out apply_rect paramenters
	unsigned gene_count = genes->size();
	os.write(reinterpret_cast<char *>(&apply_rect.x), sizeof(int));
	os.write(reinterpret_cast<char *>(&apply_rect.y), sizeof(int));
	os.write(reinterpret_cast<char *>(&apply_rect.width), sizeof(int));
	os.write(reinterpret_cast<char *>(&apply_rect.height), sizeof(int));
	os.write(reinterpret_cast<char *>(&gene_count), sizeof(unsigned));
	//os << "gene_count: " << gene_count << endl;

	//Write out genes
	for (unsigned int i = 0; i < gene_count; i++) {
		genes->at(i)->serialize(os);
	}
}

Genome* Genome::load_from_file(ifstream & fin) {
	Genome* new_genome = new Genome();
	unsigned gene_count;

	// Load apply_rect parameters
	fin.read(reinterpret_cast<char*>(&new_genome->apply_rect.x), sizeof(int));
	fin.read(reinterpret_cast<char*>(&new_genome->apply_rect.y), sizeof(int));
	fin.read(reinterpret_cast<char*>(&new_genome->apply_rect.width), sizeof(int));
	fin.read(reinterpret_cast<char*>(&new_genome->apply_rect.height), sizeof(int));
	fin.read(reinterpret_cast<char*>(&gene_count), sizeof(unsigned));

	for (unsigned int i = 0; i < gene_count; i++) {
		// Get type of gene to load
		int type;
		Gene* g;
		fin.read(reinterpret_cast<char*>(&type), sizeof(int));

		// switch on gene type
		switch (type) {
			case GABOR_GENE:
				g = Gabor_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			case SOBEL_GENE:
				g = Sobel_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			case GAUSSIAN_GENE:
				g = Gaussian_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			case MEDIAN_BLUR_GENE:
				g = Median_blur_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			case LAPLACIAN_GENE:
				g = Laplacian_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			case GRADIENT_GENE:
				g = Gradient_gene::load_from_file(fin);
				new_genome->genes->push_back(g);
				break;
			default:
				cout << "Trying to load gene of a type, " << type << ", but we don't understand\n";
				exit(-1);
		}
	}
	return new_genome;
}

bool Genome::operator==(Genome& other) const {
	bool result = true;

	// Check if apply_rect is the same
	if (apply_rect.x != other.apply_rect.x ||
			apply_rect.y != other.apply_rect.y ||
			apply_rect.width != other.apply_rect.width ||
			apply_rect.height != other.apply_rect.height)
	{
		result = false;
	}

	// Check that all the genes are the same
	for (unsigned i = 0; i < genes->size(); i++) {
		if ( *(genes->at(i)) != *(other.genes->at(i)) ) {
			result = false;
			break;
		}
	}
	return result;
}

Genome::~Genome() {
	// Delete the individual genes
	for (unsigned int gene_index = 0; gene_index < genes->size(); gene_index++) {
		Gene* g = genes->at(gene_index);
		delete g;
		g = NULL;
	}

	// Delete vector
	delete genes;
	genes = NULL;
}
