#include "capture_helper.h"

/*
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
			("
			;
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            cout << desc << "\n";
            return true;
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
}*/

VideoCapture get_camera(int width = 0,int height = 0){
	VideoCapture camera(0);
	camera.set(CAP_PROP_FRAME_WIDTH,width);
	camera.set(CAP_PROP_FRAME_HEIGHT,height);
    if(!camera.isOpened()){  
		cout << "Failed to open camera\n";
		exit(-1);
	}
    return camera;	
}

bool capture_tick(VideoCapture camera){
    Mat frame;
    camera >> frame; 

    imshow("frame", frame);
    return (waitKey(30) >= 0);
}
