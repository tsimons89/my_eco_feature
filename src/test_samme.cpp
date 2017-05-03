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
extern int num_classes;

bool parse_options(int argc, char* argv[]) {
	input_files = new vector<ImageWithClass>();
	try {
		po::options_description desc("Allowed Options");
		desc.add_options()
			("help,h", "produce help message")
			("num_classes,n", po::value<int>(),
			 "number of classes")
			("testing_folder,p", po::value< vector<string> >(),
			 "folder with testing images")
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
		if (!vm.count("testing_folder")) {
			cout << "Need testing folder to operate on.\n";
			cout << desc << "\n";
			return false;
		}
		else {
			cout << "Loading testing examples\n";
			vector<string> p_folders(vm["testing_folder"].as< vector<string> >());
			fs::directory_iterator itr(p_folders[0]), dir_end;
            int cc = 0;
            for(;itr != dir_end; ++itr) {
                fs::directory_iterator ittr(itr->path()), diir_end;
                for(;ittr != diir_end; ++ittr) {
                    input_files->push_back(ImageWithClass(ittr->path().string(), cc));
                    //cout << "ittr->path().string(): "<< ittr->path().string() << "  cc: "<<cc<< endl;
                }
                cout << cc << ", " << input_files->back().file_name << endl;
                cc++;
            }
            cout << "cc: " << cc << "\n";
            cout << "input_files->size()=" << input_files->size() << endl;
            example_image = input_files->at(0).image;
            assert(!example_image.empty());
            assert(cc==num_classes);
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

	// Create file to write out results in csv file format
	ofstream fout("samme.csv");

	int total_files = input_files->size();

	// Go through images and get classification accuracy
	cout << "Getting classifications\n";

	vector<vector<int> > result;
	for(unsigned int c = 0; c < num_weak; c++) {
		vector<int> output = mlcreatures->at(c)->predict(input_files);
		result.push_back(output);
	}
	// for(unsigned int i = 0; i < 10; i++) {
	// 	cout << result.at(i).at(i) << ", ";
	// }
	// cout << endl;

	int num_instances = 0;
	int ff, tt;
	ff = tt = 0;
	int* conf_mat = (int*)malloc(num_classes * num_classes * sizeof(int));
	for(int i = 0; i < num_classes*num_classes; i++) {
		conf_mat[i] = 0;
	}
	// Classify using adaboost
	for (int cnt = 0; cnt < total_files; ++cnt) {
		num_instances++;
		// Collect response from each weak learner
		float responses[num_classes];
		int label = input_files->at(cnt).classification;
		for(int c = 0; c < num_classes; c++) {
			responses[c] = 0;
		}

	    for (int h = 0; h < num_weak; h++) {
	        responses[result.at(h).at(cnt)] += betas->at(h);
	    }

	 //    for(int c = 0; c < num_classes; c++) {
		// 	cout << responses[c] << ",";
		// }
	 //    cout << endl;
	    // Find the max response
		int index = 0;
		float max = responses[0];
		for(int i = 1;i < num_classes; i++) {
			if(responses[i] > max) {
			    max = responses[i];
			    index = i;
			}
		}
		conf_mat[label * num_classes + index]++;
		if(index == label) {
			tt++;
		}
		else {
			ff++;
			cout << label << ":" << index << ", " << input_files->at(cnt).file_name << endl;
		}
		cerr << "Finished: " << (int)((num_instances / (float)total_files) * 100) << "%\r" << std::flush;
    }

	cout << "Calculating Accuracy\n";
	printf("true = %d, ", tt);
	printf("false = %d, ", ff);
	printf("Classification Rate = %f\n", tt / (float)total_files);
	for(int i =0;i<num_classes;i++) {
	    int sum = 0;
	    for(int j=0;j<num_classes;j++) {
	        fout << conf_mat[i * num_classes + j] << " ";
	        sum += conf_mat[i * num_classes + j];
	    }
	    fout << conf_mat[i * num_classes + i]*1.0/sum << "\n";
	}

	// Close files and delete allocated memory
	fin.close();
	fout.close();
	delete_vector_creatures(mlcreatures);
	delete betas;
	return 0;
}
