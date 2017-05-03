#ifndef LEARN_WEAKLEARNERS_H
#define LEARN_WEAKLEARNERS_H

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <ctime>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <signal.h>

#include "mlcommon.h"
#include "random_generator.h"
#include "mlCreature.h"
#include "Genome.h"
#include "Gene.h"
#include <omp.h>



namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace std;



bool parse_options(int argc, char* argv[]);
int main(int argc, char* argv[]);

// Genetic Algorithm
void init_population();
void train_creature(mlCreature* current_mlcreature);
void update_fitnesses();
int get_new_fitness(mlCreature* current_mlcreature);
void cross_over();
void print_distribution();

// Train AdaBoost Classifier


#endif
