/*
 * Creature.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef MLCREATURE_H_
#define MLCREATURE_H_

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "Genome.h"
#include "Gene.h"
#include "ImageWithClass.hpp"

using namespace std;
using namespace cv;
using namespace ml;
namespace fs = boost::filesystem;

extern Mat example_image;

class mlCreature {
	private: //All private members added by taylor
		Mat filter_kernel;

		Rect get_bounding_rect(vector<Point> points);
		Rect get_non_zero_roi(Mat input);
		Mat crop_zeros(Mat input);
		Mat get_initial_filter_mat(Size size);

	public:
		Genome* genome;
		Ptr<RTrees> rforest;
		map<uchar*, float> prev_classified;
		unsigned int id;
		bool mutated;
		int fitness_score;
		char method_of_creation[100];

		mlCreature();
		mlCreature(mlCreature* c);
		mlCreature(mlCreature* father, mlCreature* mother, int fsplit, int msplit);
		mlCreature(Genome* g, Ptr<RTrees> rf, int f, uint i);
		~mlCreature();

		void train(Mat& h, Mat& classification);
		float classify(Mat& image, bool use_previous);
		vector<int> predict(vector<ImageWithClass>* inputs);
		int predict(Mat input_image); // Added by Taylor

		Mat process(Mat& image, bool& worked);

		string to_string();
		void write_to_file(const char* file_name, const char* fclassifier);
		static mlCreature* load_from_file(const char* file_name);
		static Ptr<RTrees> classifier_load(const char* fclassifier);
		static mlCreature* copy_genes(mlCreature*);
		void serialize(ofstream& os, const char* fclassifier);
		void classifier_write(const char* fclassifier);

		bool operator == (mlCreature& other) const;
		bool operator!=(mlCreature& other) const;

		unsigned int mutate_creature(float mutation_rate);
		void visualize(string file_name);
		int calculate_new_fitness();
		bool valid();
		static void load_creatures_from_folder(string folder,
				vector<mlCreature*>* mlcreatures);
		void set_filter_kernel(); //Added by taylor
};


#endif /* MLCREATURE_H_ */
