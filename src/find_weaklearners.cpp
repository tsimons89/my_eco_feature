#include "learn_weaklearners.h"

// For GA
unsigned int generation;
unsigned int population_size=0;
float replacement_rate=0.0f;
float mutation_rate=0.0f;
unsigned int generations = 0;
vector<mlCreature*>* current_generation;
vector<mlCreature*>* next_generation;
vector<ImageWithClass>* train_set;
vector<ImageWithClass>* hold_set;
set<int> in_genes;
int runs;
Mat example_image;
string tag;
extern bool use_subregion;
extern int num_classes;

// For AdaBoost
// List of mlcreatures used as weak classifiers for adaboost
vector<mlCreature*>* mlcreatures;
// All examples for training
vector<ImageWithClass>* input_files;


bool parse_options(int argc, char* argv[]) {
	input_files = new vector<ImageWithClass>();
	try {
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help", "produce help message")
			("population_size,s", po::value<int>()->default_value(10),               /////////////////////////////////
			 "size of the GA population")
			("mutation_rate,m", po::value<float>(),
			 "GA mutation rate")
			("replacement_rate,r", po::value<float>(),
			 "GA replacement rate")
			("num_classes,n", po::value<int>(),
			 "number of classes")
			("training_folder,p", po::value< vector<string> >(),
			 "folder with training images")
			("force_genes,f", po::value<string>(),
			 "genes to include in all mlcreatures in initial population")
			("runs,u", po::value<int>()->default_value(10),                        ////////////////////////////////
			 "runs of the program to make")
			("tag,t", po::value<string>()->default_value(""),
			 "use population that all have the same genetic makeup")
			("use_subregion,a", po::value<bool>()->default_value(false),
			 "use subregions for mlcreatures")
			;
		po::variables_map vm;        
		po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);    

		if (vm.count("help")) {
		    cout << desc << "\n";
		    return false;
		}
		if (vm.count("tag")) {
			tag = vm["tag"].as<string>();	
		}
		if (vm.count("replacement_rate")) {
			replacement_rate = vm["replacement_rate"].as<float>();
		}
		else {
			replacement_rate = 0.125f;	
		}
		if (vm.count("mutation_rate")) {
			mutation_rate = vm["mutation_rate"].as<float>();
		}
		else {
			mutation_rate = uni() * .05 + .01;
		}
		if (vm.count("population_size")) {
			population_size = vm["population_size"].as<int>();
		}
		if (vm.count("use_subregion")) {
			use_subregion = vm["use_subregion"].as<bool>();
		}
		if (vm.count("num_classes")) {
			num_classes = vm["num_classes"].as<int>();
		}
		if (!vm.count("training_folder")) {
			cout << "Need training folder to operate on.\n";
			cout << desc << "\n";
			return false;
		}
		else {
			cout << "Loading training examples\n";
			vector<string> p_folders(vm["training_folder"].as< vector<string> >());
			//cout << "p_folders.size() = " << p_folders.size() << endl;
			for(unsigned int f = 0; f < p_folders.size(); f++) {
				fs::directory_iterator itr(p_folders[f]), dir_end;
				int cc = 0;
				for(;itr!=dir_end;++itr) {
					fs::directory_iterator ittr(itr->path()), diir_end;
                	for(;ittr != diir_end; ++ittr) {
                    	input_files->push_back(ImageWithClass(ittr->path().string(), cc));
                    	//cout << "ittr->path().string(): "<< ittr->path().string() << "  cc: "<<cc<< endl;
                	}
                	cout << cc << ", " << input_files->back().file_name << endl;
                	cc++;
				}
				assert(cc==num_classes);
			}
			cout << "training size: " << input_files->size() << endl;
			cout << "end loading training examples\n";
            example_image = input_files->at(0).image;
           	assert(!example_image.empty());
		}
		if (vm.count("force_genes")) {
			set<string> tmp;
			boost::split(tmp, vm["force_genes"].as<string>(),
					boost::is_any_of(","));
			set<string>::iterator itr;
			for( itr = tmp.begin(); itr != tmp.end(); itr++ ) {
				in_genes.insert(atoi((*itr).c_str()));
			}
		}
		if (vm.count("runs")) {
			runs = vm["runs"].as<int>();	
		}
		cout << "replacement_rate " << replacement_rate << "\n";
		cout << "mutation_rate " << mutation_rate << "\n";
		cout << "population_size " << population_size << "\n";
		cout << "generations " << generations << "\n\n";
		flush(cout);
	} // try
	catch(exception& e) {
		cerr << "error: " << e.what() << "\n";
		return false;
	}
	catch(...) {
		cerr << "Exception of unknown type!\n";
	}
	return true;
}

void init_population() {
	current_generation = new vector<mlCreature*>();
	mlCreature* new_mlcreature;
	bool not_accept;
	set<int> mlcreature_genes;
	unsigned int i, g;
	set<int>::iterator itr;
	for (i = 0; i < population_size; i++) {
		not_accept = true;
		while(not_accept) {
			new_mlcreature = new mlCreature();

			mlcreature_genes.clear();
			// Make a set of all the mlcreatures genes
			for (g = 0; g < new_mlcreature->genome->genes->size(); g++) {
				int gene_type = new_mlcreature->genome->genes->at(g)->gene_type;
				mlcreature_genes.insert(gene_type);
			}

			// Check that all the genes in in_genes are in mlcreatures genes
			not_accept = false;
			for(itr = in_genes.begin(); itr != in_genes.end(); itr++) {
				if (mlcreature_genes.count(*itr) != 0) {
					not_accept = true;
					break;
				}
			}

			if (not_accept) {
				delete new_mlcreature;
				new_mlcreature = NULL;
			}
		}
		current_generation->push_back(new_mlcreature);
	}
	cout << "finish_init_population(): population_size " << population_size << "\n";
}

void train_creature(mlCreature* current_mlcreature) {
	// Prepare the training data into the random forest in opencv
	unsigned int num_train = train_set->size();
	Mat train_matset;
	vector<int> train_labels;

	// Train new classifier
	for (unsigned int f = 0; f < num_train; f++) {
		Mat im = train_set->at(f).image;
		bool worked;
		Mat result = current_mlcreature->process(im, worked);

		if (!worked) {
			current_mlcreature->write_to_file("bad_creature", "bad_creature");
			cout << current_mlcreature->to_string() << endl;
			cout << "Could not process mlcreature in find_creatures::train_creature\n";
			exit(-1);
		}
		train_matset.push_back(result);
		train_labels.push_back(train_set->at(f).classification);
	} // training set
	///train_matset.convertTo(train_matset, CV_32F, 1.0, 0);
	Mat train_data = train_matset.reshape(0, num_train);
	///threshold(train, train, 0, -1, THRESH_TOZERO);
	// double min_val, max_val;
	// minMaxLoc(train_data, &min_val, &max_val);
	// //cout << "min_val=" << min_val << ", max_val=" << max_val << endl;
	Mat train_input;
	// train_data.convertTo(train_input, CV_32F, 255.0/(max_val - min_val), -min_val * 255.0/(max_val - min_val));
	train_data.convertTo(train_input, CV_32F);
	//cout << "min_val=" << min_val << ", max_val=" << max_val << endl;cout << endl;
	//cout << "train_matset.size()=" << train_matset.size() << endl;
	//cout << "train_matset.type()=" << train_matset.type() << endl;
	//cout << "train.size()=" << train.size() << endl;
	assert(train_input.type() == CV_32F || train_input.type() == CV_32S);
	Mat label_input = Mat(train_labels);
	current_mlcreature->train(train_input, label_input);///train_matset
}

void update_fitnesses() {
	mlCreature* current_mlcreature;
	unsigned int i;
	cout << "current_generation->size()=" << current_generation->size() << endl;
	// Parallelize along mlcreatures in the population
	// #pragma omp parallel for private(i,current_mlcreature) schedule(dynamic, 1)
	for(i = 0; i < current_generation->size(); i++) {
		current_mlcreature = current_generation->at(i);
		if (current_mlcreature->mutated) {
			train_creature(current_mlcreature);
			current_mlcreature->fitness_score = current_mlcreature->calculate_new_fitness();
		}
	} // population
	cout <<"1:finish_update_fitnesses"<<endl;
}

void creature_select() {
	int num_select = int((1.0f - replacement_rate) * population_size);
	next_generation = new vector<mlCreature*>();
	int size = current_generation->size();
	for(int p = 0; p < num_select; p++) {
		int ri1 = uni() * size;
		int ri2 = uni() * size;
		int ri3 = uni() * size;
		mlCreature* mlcreature1 = current_generation->at(ri1);
		mlCreature* mlcreature2 = current_generation->at(ri2);
		mlCreature* mlcreature3 = current_generation->at(ri3);

		mlCreature* best_mlcreature = mlcreature1;
		if (mlcreature2->fitness_score > best_mlcreature->fitness_score) {
			best_mlcreature = mlcreature2;
		}
		if (mlcreature3->fitness_score > best_mlcreature->fitness_score) {
			best_mlcreature = mlcreature3;
		}
		next_generation->push_back(new mlCreature(best_mlcreature));
	}
	cout <<"4:creature_select"<<endl;
}

void cross_over() {
	int num_select = ceil(replacement_rate * population_size);
	for (int i = 0; i < num_select; i++) {
		bool failed = true;
		for (int trys = 0; trys < 70; trys++) {
			mlCreature* father = current_generation->at(
					(int)(uni() * current_generation->size()));
			mlCreature* mother = current_generation->at(
					(int)(uni() * current_generation->size()));
			int fsplit = (int)(uni() * (father->genome->genes->size() - 1) + 1);
			int msplit = (int)(uni() * (mother->genome->genes->size()));
			mlCreature* new_mlcreature = new mlCreature(father, mother, fsplit, msplit);
			// mlCreature doesn't work
			if (new_mlcreature->valid()) {
				failed = false;
				next_generation->push_back(new_mlcreature);
				break;
			}
			else {
				delete new_mlcreature;
			}
		}
		if (failed) {
			mlCreature* new_mlcreature = new mlCreature();
			next_generation->push_back(new_mlcreature);
			cout << "Could not find valid crossover, made completely new mlcreature\n";
		}
	}
	cout<<"5:cross_over"<<endl;
}

void mutate_population() {
	for (unsigned int i = 0; i < next_generation->size(); i++) {
		unsigned int num_mutations =
			next_generation->at(i)->mutate_creature(mutation_rate);
		if (num_mutations > 0) {
			next_generation->at(i)->mutated = true;
		}
	}
	cout<<"6:mutate_population"<<endl;
}

void mark_not_mutated() {
	for (unsigned i = 0; i < current_generation->size(); i++) {
		current_generation->at(i)->mutated = false;
	}
	cout<<"3:mark_not_mutated"<<endl;
}

int print_best_creature() {
	int best = -1;
	int best_index = -1;
	for (unsigned i = 0; i < current_generation->size(); i++) {
		mlCreature* current_mlcreature = current_generation->at(i);
		if (current_mlcreature->fitness_score > best) {
			best = current_mlcreature->fitness_score;
			best_index = i;
		}
	}
	if (best_index != -1) {
		mlCreature* current_mlcreature = current_generation->at(best_index);
		cout << "Best mlcreature = " << current_mlcreature->fitness_score << endl;
		for (unsigned int i = 0; i < current_mlcreature->genome->genes->size() - 1; i++) {
			int type = current_mlcreature->genome->genes->at(i)->gene_type;
			cout << make_gene_type_readable(type) << ", ";
		}
		int type = current_mlcreature->genome->genes->back()->gene_type;
		cout << make_gene_type_readable(type) << endl;
	}
	cout<<"2:print_best_creature"<<endl;
	return best;
}

void print_distribution() {
	map<string, int> dist;
	for (unsigned i = 0; i < current_generation->size(); i++) {
		mlCreature* current_mlcreature = current_generation->at(i);
		string genome_string = "";
		for (unsigned int g = 0; g < current_mlcreature->genome->genes->size(); g++) {
			char gene_string[30];
			sprintf(gene_string, "%s:", make_gene_type_readable(current_mlcreature->genome->genes->at(g)->gene_type).c_str());
			genome_string += gene_string;
		}
		dist[genome_string]++;
	}
	map<string,int>::iterator iter;
	int i = 0;
	int total = 0;
    for( iter = dist.begin(); iter != dist.end(); ++iter ) {
		total += iter->second;
		//if (iter->second > 20) {
			cout << i++ << " genome: " << iter->first << ", count: " << iter->second << endl;
		//}
    }
}

void get_generation_fitness() {
	//the current generation is saved in current_generation
	int min_fit = 100;
	int max_fit = 0;
	float avg_fit = 0.;
	unsigned int num_mlcreatures = current_generation->size();

	for(unsigned int i = 0; i < num_mlcreatures; i++) {
		mlCreature* current = current_generation->at(i);
		int fitness = current->fitness_score;
		avg_fit += fitness;
		if(fitness > max_fit) {
			max_fit = fitness;
		}
		if(fitness < min_fit) {
			min_fit = fitness;
		}
	}
	avg_fit /= num_mlcreatures;
	cout << "get_generation_fitness(): " << endl;
	cout << "min_fit=" << min_fit << ", max_fit=" << max_fit << ", avg_fit=" << int(avg_fit) << endl;


	//get generation distribution
	map<string, int> gene_dist;//genes, cnt
	map<int, int> fit_dist;//fitness, cnt
	map<string, int> onefit_dist;//max fitness, genes, cnt

	//map<int, int> onegenome_dist;//fixed genes, fitness, cnt
	//int onegenome_cnt = 0;

	for(unsigned int i = 0; i < num_mlcreatures; i++) {
		mlCreature* current_mlcreature = current_generation->at(i);
		string creature_type;
		for(unsigned int j = 0; j < current_mlcreature->genome->genes->size(); j++) {
			int type = current_mlcreature->genome->genes->at(j)->gene_type;
			stringstream ss;
			ss << type;
			creature_type = creature_type + ss.str() + "_";
		}
		gene_dist[creature_type]++;//////////////////////
		int fitness = current_mlcreature->fitness_score;
		fit_dist[fitness]++;//////////////////////////////////////
		if(fitness == max_fit) {
			onefit_dist[creature_type]++;//////////////////////
		}
		/*if(creature_type=="321") {
			onegenome_dist[fitness]++;
			onegenome_cnt++;
		}*/
	}
	cout << "gene_dist = genes : cnt" << endl;
	for( map<string, int>::iterator itr = gene_dist.begin(); itr != gene_dist.end(); ++itr) {
		cout << (*itr).first << ": " << 100 * (*itr).second / (float)num_mlcreatures << endl;
	}
	cout << "fit_dist = fitness : cnt" << endl;
	for( map<int, int>::iterator itr = fit_dist.begin(); itr != fit_dist.end(); ++itr) {
		cout << (*itr).first << ": " << 100 * (*itr).second / (float)num_mlcreatures << endl;
	}
	cout << "maxfit_dist = genes : cnt" << endl;
	for( map<string, int>::iterator itr = onefit_dist.begin(); itr != onefit_dist.end(); ++itr) {
		cout << (*itr).first << ": " << 100 * (*itr).second / (float)num_mlcreatures  << endl; /// (float)onefit_cnt
	}
	cout << "\n";
}

void Get_mlcreatures(vector<ImageWithClass>* resampled_input) {
	mlcreatures = new vector<mlCreature*>();

	// Make a training and a holding set
	random_shuffle(resampled_input->begin(), resampled_input->end());
	int num_holding = (int)(resampled_input->size() * .4f);  //------------------------.6f
	hold_set = new vector<ImageWithClass>(num_holding);
	copy(resampled_input->begin(), resampled_input->begin()+num_holding,
			hold_set->begin());
	train_set = new
		vector<ImageWithClass>(resampled_input->size() - num_holding);
	copy(resampled_input->begin()+num_holding, resampled_input->end(),
			train_set->begin());
	cout << "train_set size "<< train_set->size() << "\n";
	cout << "num_holding " << num_holding << "\n";

	for (int run_idx = 0; run_idx < runs; run_idx++) {
		cout << "Run = " << run_idx << endl;

		init_population();

		int end_counter = 0;
		int best_fitness = -1;
		int generation = 0;
		while(1) {
			//print_distribution();
			cout << "\nGeneration = " << generation << endl;
			update_fitnesses();
			get_generation_fitness();

			int new_best_fitness = print_best_creature();
			if (new_best_fitness != best_fitness) {
				best_fitness = new_best_fitness;	
				end_counter = 0;
			}
			else {
				end_counter++;	
			}
			if (end_counter > 3 || best_fitness == 100) break; /// [10, 200], [18,1000]

			mark_not_mutated();
			creature_select();
			cross_over();
			mutate_population();
			vector<mlCreature*>* tmp = current_generation;
			current_generation = next_generation;
			delete_vector_creatures(tmp);
			next_generation = NULL;
			generation++;
		}
		print_distribution();

		// Save best mlcreature for the run
		int best = 0;
		int best_index = -1;
		for (unsigned i = 0; i < current_generation->size(); i++) {
			mlCreature* current_mlcreature = current_generation->at(i);
			if (current_mlcreature->fitness_score >= best) {
				best = current_mlcreature->fitness_score;
				best_index = i;
			}
			else if (best != 0 && current_mlcreature->fitness_score == best && 
					current_mlcreature->genome->genes->size() < 
					current_generation->at(best_index)->genome->genes->size())
			{
				best = current_mlcreature->fitness_score;
				best_index = i;	
			}
		}
		mlCreature* current_mlcreature = current_generation->at(best_index);
		mlcreatures->push_back(current_mlcreature);
		cout << "best_index push_back\n";
		for (unsigned int i = 0; i < current_mlcreature->genome->genes->size() - 1; i++) {
			int type = current_mlcreature->genome->genes->at(i)->gene_type;
			cout << make_gene_type_readable(type) << ", ";
		}
		int type = current_mlcreature->genome->genes->back()->gene_type;
		cout << make_gene_type_readable(type) << endl;

		// cout << "pushback";
		// cout << current_mlcreature->genome->apply_rect.height << ", " << current_mlcreature->genome->apply_rect.width << endl;

		// Free up memory before exiting. Makes looking at valgrind easier
		delete_vector_exception(current_generation, best_index);
		current_generation = NULL;
		next_generation = NULL;
	}

	delete train_set;
	delete hold_set;
	train_set = NULL;
	hold_set = NULL;
}

vector<ImageWithClass>* dataset_resample(float d[]) {
	Mat train_matset;
	vector<int> train_labels;
	vector<ImageWithClass>* newinput_files = new vector<ImageWithClass>();

	unsigned int num_examples = input_files->size();
	for(int p = 0; p < num_examples; p++) {
		int ri1 = uni() * num_examples;
		int ri2 = uni() * num_examples;
		int ri3 = uni() * num_examples;

		int selected = ri1;
		if(d[ri2] > d[selected]) {
			selected = ri2;
		}
		if(d[ri3] > d[selected]) {
			selected = ri3;
		}
		newinput_files->push_back(input_files->at(selected));
	}
	assert(newinput_files->size() == num_examples);
	return newinput_files;
}

void test() {
	cout << "testing";
	int num_creatures = mlcreatures->size();
	for(int i = 0; i < num_creatures; i++) {
		cout << mlcreatures->at(i)->genome->apply_rect.height << ", " << mlcreatures->at(i)->genome->apply_rect.width << endl;
	}
}

int main(int argc, char* argv[]) {

	redirectError(cvNulDevReport);
	uint seed = get_random_seed();
	generator.seed(seed);
	// Parse the commandline arguments
	if (!parse_options(argc, argv)) {
		return 1;	
	}
	// cout << "seed = " << seed << endl;

	unsigned int num_examples = input_files->size();

	// Initialize weights for the examples
	float d[num_examples];
	for (unsigned int i = 0; i < num_examples; i++) {
		d[i] = 1.0f / (float)num_examples;	
	}
	// Open file for writing adaboost model info
	ofstream fout("resample.model", ios_base::out);
	
	float limit_error = 1./float(num_classes); 
	for (unsigned int rnd = 0; rnd < 1000; rnd++) { //30
		cout << "rnd " << rnd << " starts" << endl;

		vector<ImageWithClass>* newinput;
		if(rnd > 0) {
			newinput = dataset_resample(d);
		}
		else {
			// Only for the rnd = 0
			newinput = new vector<ImageWithClass>();
			*newinput = *input_files;
		}
		cout << "input_files->size() = " << input_files->size() << endl;
		cout << "newinput->size() = " << newinput->size() << endl;

		// Start GA to get a group of creatures
		Get_mlcreatures(newinput);
		unsigned int num_creatures = mlcreatures->size();
		cout << "num_creatures = " << num_creatures << endl;
		
		
		// Collect all results from weak classifiers
		vector<vector<int> > result;
		for(int id = 0; id < num_creatures; id++) {
			mlCreature* current_mlcreature;
			current_mlcreature = mlcreatures->at(id);
			vector<int> weak = current_mlcreature->predict(input_files);// Not newinput_files
			result.push_back(weak);
		}
		delete newinput;
		// find weak classifier with lowest error
		float min_error = 9999999999999.9;
		int best_classifier_idx = -1;
		float norm_factor = 0;
		
		for (unsigned int c = 0; c < num_creatures; c++) {
			float error = 0;
			norm_factor = 0;
			for (unsigned int i = 0; i < num_examples; i++) {
				int label = input_files->at(i).classification;
				if (result.at(c).at(i) != label) {
					error += d[i];
				}
				norm_factor += d[i];
			}// cout << "norm_factor " << norm_factor << endl;
			error = error / norm_factor;
			if (error < min_error) {
				min_error = error;
				best_classifier_idx = c;	
			}
		}

		if (min_error >= (1. - limit_error)) {
			delete_vector_creatures(mlcreatures);
			break;
		}
		float alpha = log((1-min_error)/min_error) + log(num_classes-1);

		// Update weights on the samples
		norm_factor = 0;
		for (unsigned int i= 0; i < num_examples; i++) {
			int label = input_files->at(i).classification;
		    if(result.at(best_classifier_idx).at(i) != label) {
		        d[i] = d[i]*exp(alpha);
		    }
			norm_factor += d[i];
		}
		for (unsigned int i = 0; i < num_examples; i++) {
			d[i] = d[i] / norm_factor;
		}

		char f1[500], f2[500];
		sprintf(f1, "%smlcreature_%u", tag.c_str(), mlcreatures->at(best_classifier_idx)->id);
		sprintf(f2, "%srf_%u.yaml", tag.c_str(), mlcreatures->at(best_classifier_idx)->id);
		mlcreatures->at(best_classifier_idx)->write_to_file(f1, f2);

		// Write weak classifiers out to file
		printf("min_error = %f, ", min_error);
		printf("alpha = %f, ", alpha);
		printf("mlcreature id = %d\n", mlcreatures->at(best_classifier_idx)->id);
		fout << alpha << " " << mlcreatures->at(best_classifier_idx)->id << endl;

		delete_vector_creatures(mlcreatures);
	}
	cout << "finishing line/n";
	delete input_files;
	fout.close();
}
