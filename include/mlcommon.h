/*
 * common.h
 *
 *  Created on: Jun 13, 2014
 *      Author: meng
 */

#ifndef MLCOMMON_H_
#define MLCOMMON_H_

#include <iostream>
#include <vector>

#include "mlCreature.h"

#include <boost/regex.hpp>
#include <fstream>

using namespace std;

typedef unsigned int uint;



void delete_vector_creatures(vector<mlCreature*>* generation);
uint get_random_seed();
void delete_vector_exception(vector<mlCreature*>* generation, int id);


#endif /* MLCOMMON_H_ */
