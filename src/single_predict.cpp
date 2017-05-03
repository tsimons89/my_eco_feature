#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "ImageWithClass.hpp"
#include "mlcommon.h" 
#include "mlCreature.h"
#include <dirent.h>

using namespace std;
namespace fs = boost::filesystem;
namespace po = boost::program_options;

vector<mlCreature*>* mlcreatures;
vector<float>* betas;
vector<ImageWithClass>* train_set;
vector<ImageWithClass>* hold_set;
Mat example_image;

// All examples for testing
vector<ImageWithClass>* input_files;
vector<string>* mlcreature_folders;
string adaboost_model_file;
string test_image_file;
Mat test_image;
extern int num_classes;

bool parse_options(int argc, char* argv[]) {
	try {
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help,h", "produce help message")
			("num_classes,n", po::value<int>(),
			 "number of classes")
			("test_image_file,i", po::value<string>(),
			 "File name of the image to be tested")
			("mlcreature_folders,c", po::value< vector<string> >(),
			 "folder with mlcreature files")
			("adaboost_model_file,m", po::value<string>(),
			 "adaboost model to use")
			;
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return false;
        }
		if (!vm.count("adaboost_model_file")) {
			cout << "Need adaboost model file to use.\n";
			cout << desc << endl;
			return false;
		}
		else {
			adaboost_model_file = vm["adaboost_model_file"].as<string>();	
		}
		if (!vm.count("mlcreature_folders")) {
			cout << "Need folder of mlcreature files to choose weak classifiers from.\n";
			cout << desc << "\n";
			return false;
		}
		else {
			mlcreature_folders = new vector<string>(vm["mlcreature_folders"].as< vector<string> >());
		}
		if (vm.count("num_classes")) {
			 num_classes = vm["num_classes"].as<int>();
		}
		if (!vm.count("test_image_file")) {
			cout << "Need test image file to use.\n";
			cout << desc << endl;
			return false;
		}
		else {
			test_image = imread(vm["test_image_file"].as<string>(),0);
			test_image.convertTo(test_image, CV_32F);
			if (test_image.empty()) {
				cout << "Problem loading image."<< endl;
				return false;
			}
	
		}

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


int main(int argc, char* argv[]) {
	// Parse over what folders to use for testing
	bool worked = parse_options(argc, argv);
	if (!worked) {
		cout << "Couldn't parse commandline options\n";
		exit(0);
	}

	mlcreatures = new vector<mlCreature*>();
	betas = new vector<float>();

	// Open adaboost model file and load mlcreatures
	if (!fs::exists(adaboost_model_file) || fs::is_directory(adaboost_model_file)) {
		cout << "Adaboost model file does not exist!\n";
		exit(0);	
	}
	ifstream fin(adaboost_model_file.c_str());
	float beta;
	unsigned int mlcreature_id;
	int num_weak = 0;
	while (fin >> beta >> mlcreature_id) {
		//cout << beta << "  " << mlcreature_id << endl;
		char c_id_str[50];//20, buffer overflow detected
		sprintf(c_id_str, "mlcreature_%u", mlcreature_id);
		fs::path mlcreature_path;
		bool found_mlcreature = false;
		char found_str[300];
		for (unsigned int f = 0; f < mlcreature_folders->size(); f++) {
			fs::path mlcreature_path(fs::path(mlcreature_folders->at(f)) / c_id_str);
			//cout << mlcreature_path << endl;
			if (fs::exists(mlcreature_path)) {
				found_mlcreature = true;
				strcpy(found_str, mlcreature_path.string().c_str());
				break;
			}
		}
		if (!found_mlcreature) {
			cout << "Can't find " << mlcreature_id << endl;
			exit(0);
		}
		mlCreature* c = mlCreature::load_from_file(found_str);
		//cout << c->fitness_score << endl;
		mlcreatures->push_back(c);
		betas->push_back(beta);
		num_weak++;
	}





	// Go through images and get classification accuracy
	cout << "Getting classifications\n";

	vector<int> result;
	for(unsigned int c = 0; c < num_weak; c++) {
		int output = mlcreatures->at(c)->predict(test_image);
		result.push_back(output);
	}

	int num_instances = 0;
	int ff, tt;
	ff = tt = 0;
	int* conf_mat = (int*)malloc(num_classes * num_classes * sizeof(int));
	for(int i = 0; i < num_classes*num_classes; i++) {
		conf_mat[i] = 0;
	}
	// Classify using adaboost
	// Collect response from each weak learner
	float responses[num_classes];
	for(int c = 0; c < num_classes; c++) {
		responses[c] = 0;
	}

    for (int h = 0; h < num_weak; h++) {
        responses[result.at(h)] += betas->at(h);
    }

    // Find the max response
	int index = 0;
	float max = responses[0];
	for(int i = 1;i < num_classes; i++) {
		if(responses[i] > max) {
		    max = responses[i];
		    index = i;
		}
	}

	cout << "Predicted class: " << index << endl;

	// Close files and delete allocated memory
	fin.close();
	delete_vector_creatures(mlcreatures);
	delete betas;
	return 0;
}
