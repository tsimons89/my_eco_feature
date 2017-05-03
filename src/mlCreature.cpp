#include "mlCreature.h"

int num_classes;
extern vector<ImageWithClass>* hold_set;

mlCreature::mlCreature() {
	genome = new Genome(true);
	rforest = RTrees::create();
	
	fitness_score = 0;
	id = uni() * UINT_MAX;
	mutated = true;
	sprintf(method_of_creation, "New mlCreature");
}

mlCreature::mlCreature(mlCreature* c) {
	genome = new Genome(c->genome);
	rforest = RTrees::create();
	rforest = c->rforest;
	fitness_score = c->fitness_score;
	id = c->id;
	mutated = c->mutated;
	sprintf(method_of_creation, c->method_of_creation);
}

mlCreature::mlCreature(mlCreature* f, mlCreature* m, int fsplit, int msplit) {
	genome = new Genome(false);
	id = uni() * UINT_MAX;

	// Operate on the part of the image that the father was
	genome->apply_rect.x = f->genome->apply_rect.x;
	genome->apply_rect.y = f->genome->apply_rect.y;
	genome->apply_rect.width = f->genome->apply_rect.width;
	genome->apply_rect.height = f->genome->apply_rect.height;

	// Get fathers genes
	for (int i = 0; i < fsplit; i++) {
		genome->genes->push_back(f->genome->genes->at(i)->clone());
	}
	// Get mothers genes
	if (example_image.channels() == 3 && msplit == 0) msplit = 1;
	for (unsigned i = msplit; i < m->genome->genes->size(); i++) {
		genome->genes->push_back(m->genome->genes->at(i)->clone());
	}

	rforest = RTrees::create();
	rforest = f->rforest;
	fitness_score = 0;
	mutated = true;
	sprintf(method_of_creation, "Cross-over");
}

mlCreature::mlCreature(Genome* g, Ptr<RTrees> rf, int f, uint i) {
	rforest = rf;
	fitness_score = f;
	id = i;
	genome = g;
	mutated = true;
	sprintf(method_of_creation, "Loaded from file");
}

mlCreature* mlCreature::copy_genes(mlCreature* copy_from) {
	// Creature new genome based on another mlcreature
	Genome* new_genome = Genome::copy_genes(copy_from->genome);
	// Create generic random forest and id
	Ptr<RTrees> new_rf = RTrees::create();
	
	uint new_id = uni() * UINT_MAX;
	mlCreature* new_creature = new mlCreature(new_genome, new_rf, 0, new_id);
	sprintf(new_creature->method_of_creation, "Copied new mlCreature");
	return new_creature;
}


void mlCreature::train(Mat& m, Mat& c) {
	/*rforest->setMaxDepth(18);//max depth15
	int min_sample_count = (m.cols>5) ? 5 : m.cols;
	rforest->setMinSampleCount(min_sample_count);//min sample count
	//rforest->setRegressionAccuracy(0.01);//regression accuracy
	//rforest->setUseSurrogates(0);//compute surrogate split
	rforest->setMaxCategories(15);//max number of categories 15
	//rforest->setActiveVarCount(0);//number of variables randomly selected at node and used to find the best split
	//cout << "m.cols=" << m.cols << endl;*/

	rforest->setMaxDepth(15);//4
    rforest->setMinSampleCount(5);//2
    rforest->setRegressionAccuracy(0.f);
    rforest->setUseSurrogates(false);
    rforest->setMaxCategories(16);
    rforest->setPriors(Mat());
    rforest->setCalculateVarImportance(false);
    rforest->setActiveVarCount(0);
    rforest->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 0));// 5
	rforest->train(m, ml::ROW_SAMPLE, c);
}

float mlCreature::classify(Mat& h, bool use_previous) {
	bool worked;
	float result;
	if (!mutated && use_previous) {
		result = prev_classified[h.data];
	}
	else {
		Mat processed = this->process(h, worked);
		if (!worked) {
			this->write_to_file("bad_creature", "bad_creature");
			cout << "Could not process genome in mlCreature::classify\n";
			exit(-1);
		}
		Mat row_processed = Mat(processed).reshape(0, 1);
		Mat output(1, 1, CV_32F);
		rforest->predict(row_processed, output);
		result = round(output.at<float>(0, 0));

		if (use_previous) {
			prev_classified[h.data] = result;
		}
	}
	return float(result);
}

vector<int> mlCreature::predict(vector<ImageWithClass>* inputs) {
	unsigned int num_img = inputs->size();
	vector<int> output_vector(num_img, 0);
	Mat matset;
	//Mat labels = Mat(num_img, 1, CV_32F);
	Mat output = Mat(num_img, 1, CV_32F);

	for(unsigned int f = 0; f < num_img; f++) {
		Mat img = inputs->at(f).image;
		bool worked;
		Mat processed = this->process(img, worked);
		if(!worked) {
			this->write_to_file("bad_creature", "bad_creature");
			cout << "Could not process genome in mlCreature::predict\n";
			exit(-1);
		}
		Mat row_processed = Mat(processed).reshape(0, 1);
		matset.push_back(row_processed);
		//labels.at<float>(f, 0) = float(inputs->at(f).classification);
	}
	matset.convertTo(matset, CV_32F);
	assert(matset.rows == num_img);
	rforest->predict(matset, output);

	for(int i = 0; i < num_img; i++) {
		output_vector.at(i) = int(output.at<float>(i, 0));
	}

	return output_vector;
}

int mlCreature::predict(Mat input_image) { //Added by Taylor
	bool worked;
	Mat output = Mat(1,1,CV_32F);
	Mat processed = this->process(input_image, worked);
	if(!worked) {
		cout << "Could not process genome in mlCreature::predict\n";
		exit(-1);
	}
	Mat row_processed = Mat(processed).reshape(0, 1);
	row_processed.convertTo(row_processed, CV_32F);
	assert(row_processed.rows == 1);
	rforest->predict(row_processed, output);
	return int(output.at<float>(0, 0));
}



Mat mlCreature::process(Mat& image, bool& worked) {
	// I think I'm cloning the images so the original images don't get modified
	Mat sub = Mat(image, genome->apply_rect).clone();
	Mat m = this->genome->process(sub, worked);
	return m;
}

string mlCreature::to_string() {
	char id_string[200];
	sprintf(id_string, "mlCreature %u:\n\tMethod of Creation: %s\n", id,
			method_of_creation);
	return id_string + genome->to_string();
}

void mlCreature::write_to_file(const char* file_name, const char* fclassifier) {
	ofstream fout(file_name, ios_base::out | ios_base::trunc);
	this->serialize(fout, fclassifier);
	fout.close();
}

void mlCreature::serialize(ofstream& os, const char* fclassifier) {
	os.write(reinterpret_cast<char *>(&fitness_score), sizeof(int));
	os.write(reinterpret_cast<char *>(&id), sizeof(int));
	this->classifier_write(fclassifier);
	genome->serialize(os);
}

void mlCreature::classifier_write(const char* fclassifier) {
	rforest->save(fclassifier);
}

mlCreature* mlCreature::load_from_file(const char* file_name) {
	ifstream fin;
	if (!fs::exists(file_name)) {
		string mess = string("File ") + file_name + string(" does not exist");
		throw(invalid_argument(mess));
	}

	// Try and open file
	try {
		fin.open(file_name, ios_base::binary);
	}
	catch(...) {
		cout << "Could not open file " << file_name << " \
			to load mlCreature from\n";
		exit(-1);
	}

	// Read in fitness_score and mlcreature id
	int tmp_fitness;
	uint tmp_id;
	fin.read(reinterpret_cast<char*>(&tmp_fitness), sizeof(int));
	fin.read(reinterpret_cast<char*>(&tmp_id), sizeof(uint));
	
	// Load random forest and genome from file
	char f[500];
	sprintf(f, "rf_%u.yaml", tmp_id);
	Ptr<RTrees> rforest = mlCreature::classifier_load(f);

	Genome* new_genome = Genome::load_from_file(fin);
	fin.close();

	// Create new mlCreature and return it
	return new mlCreature(new_genome, rforest, tmp_fitness, tmp_id);
}

Ptr<RTrees> mlCreature::classifier_load(const char* fclassifier) {
	Ptr<RTrees> newrf = Algorithm::load<RTrees>(string(fclassifier));
	return newrf;
}

/*bool mlCreature::operator==(mlCreature& other) const {
	return (fitness_score == other.fitness_score && this->id == other.id &&
			*(this->genome) == *(other.genome));
}

bool mlCreature::operator!=(mlCreature& other) const {
	return !(*this == other);
}*/

unsigned int mlCreature::mutate_creature(float mutation_rate) {
	unsigned int num_mutations = 0;
	if (uni() < mutation_rate) {
		// could do sth to mutate the parameters
		num_mutations++;
	}
	num_mutations += genome->mutate_genome(mutation_rate);

	if (num_mutations > 0) {
		// Get a new id
		id = uni() * UINT_MAX;

		// Mark mutated
		mutated = true;

	}
	return num_mutations;
}

void mlCreature::visualize(string file_name) {
	Mat start_image = imread(file_name.c_str(),0);
	if (start_image.empty()) {
		cerr << "Can't load image " << file_name << endl;
		exit(-1);
	}
	rectangle(start_image,
			cvPoint(genome->apply_rect.x, genome->apply_rect.y),
			cvPoint(genome->apply_rect.x + genome->apply_rect.width,
				genome->apply_rect.y + genome->apply_rect.height),
			cvScalar(255,255,255));
	/*namedWindow("start", 1);
	imshow("start", start_image);*/

	//start_image = imread(file_name.c_str(),0);
	Mat sub = Mat(start_image, this->genome->apply_rect).clone();
	Mat tmp = sub;
	Mat next_tmp;
	bool worked;
	for (unsigned int i = 0; i < genome->genes->size(); i++) {
		next_tmp = genome->genes->at(i)->process(tmp, worked);
		tmp = next_tmp;
		/*namedWindow("step", 1);
		imshow("step", tmp);
		cvWaitKey(0);*/
		if (!worked) {
			 cout << "Fail\n";
			 exit(-1);
		}
	}
}

mlCreature::~mlCreature() {
	delete genome;
	genome = NULL;
}

int mlCreature::calculate_new_fitness() {
        // Find fitness score
	unsigned int num_hold = hold_set->size();
	Mat hold_matset;
	//Mat hold_labels = Mat(num_hold, 1, CV_32F);
	Mat hold_output(num_hold, 1, CV_32F);
	vector<int> hold_labels;

    for (unsigned int f = 0; f < hold_set->size(); f++) {
        Mat im = hold_set->at(f).image;
        bool worked;
        Mat processed = this->process(im, worked);

        if(!worked) {
        	this->write_to_file("bad_creature", "bad_creature");
        	cout << "Could not process genome in mlCreature::calculate_new_fitness\n";
        	exit(-1);
        }
        ///Mat row_processed = processed.reshape(0, 1);// im
        hold_matset.push_back(processed);
        //hold_labels.at<float>(f, 0) = float(hold_set->at(f).classification);
        hold_labels.push_back(hold_set->at(f).classification);
    }
    hold_matset.convertTo(hold_matset, CV_32F, 1.0, 0);
    //threshold(hold_matset, hold_matset, 0, -1, THRESH_TOZERO);
    Mat hold = hold_matset.reshape(0, num_hold);

    double min_val, max_val;
	minMaxLoc(hold, &min_val, &max_val);
	Mat hold_input;
	hold.convertTo(hold_input, CV_32F, 255.0/(max_val - min_val), -min_val * 255.0/(max_val - min_val));
	
    rforest->predict(hold_input, hold_output);

    Mat confusion_mat = Mat::zeros(num_classes, num_classes, CV_32S);
    for(int i = 0; i < num_hold; i++) {
    	int label = hold_labels.at(i);
    	int predict = round(hold_output.at<float>(i, 0));
    	confusion_mat.at<int>(label, predict)++;
    }
    int hits = 0;
    for(int i = 0; i < num_classes; i++) {
    	hits = hits + confusion_mat.at<int>(i, i);
    }
    int fitness = int(100 * (float)hits/num_hold);

    return fitness;
}

bool mlCreature::valid() {
	return genome->valid();
}

void mlCreature::load_creatures_from_folder(string folder, 
		vector<mlCreature*>* mlcreatures) {
	const boost::regex mlcreature_filter("mlcreature_\\d+");
	fs::directory_iterator end_itr;
	for (fs::directory_iterator itr(folder); itr != end_itr; itr++) {
		// Skip files that don't have naming convention
		boost::smatch what;
		if (!boost::regex_match(itr->path().string(), what, mlcreature_filter)) continue;
		
		// Create full path, load from file, store in vector
		fs::path mlcreature_path(fs::path(folder) / itr->path().filename());
		mlCreature* c = mlCreature::load_from_file(mlcreature_path.string().c_str());
		mlcreatures->push_back(c);
	}
}
