#include "mlcommon.h"

//void delete_vector_pools(vector<Pool*>* generation) {
//	//uint vector_size = generation->size();
//	//for (uint i = 0; i < vector_size; i++) {
//	//	if (generation == NULL || generation->back() == NULL) {
//	//		cerr << "Trying to delete a generation that is NULL or it empty\n";
//	//	}
//	//	delete (Pool*)generation->back();
//	//	generation->pop_back();
//	//}
//	//delete generation;
//	//generation = NULL;
//	uint vector_size = generation->size();
//	for (uint i = 0; i < vector_size; i++) {
//		Pool* p = generation->at(i);
//		delete p;
//		p = NULL;
//	}
//	generation->clear();
//	delete generation;
//	generation = NULL;
//}


void delete_vector_creatures(vector<mlCreature*>* generation) {

	unsigned int vector_size = generation->size();
	for (uint i = 0; i < vector_size; i++) {
		mlCreature* c = generation->at(i);
		delete c;
		c = NULL;
	}

	generation->clear();
	delete generation;
	generation = NULL;
}

void delete_vector_exception(vector<mlCreature*>* generation, int id) {

	unsigned int vector_size = generation->size();
	for (uint i = 0; i < vector_size; i++) {
		if(i != id) {
			mlCreature* c = generation->at(i);
			delete c;
			c = NULL;
		}
	}

	generation->clear();
	delete generation;
	generation = NULL;
}

uint get_random_seed() {
	uint random_seed;
	ifstream file ("/dev/urandom", std::ios::binary);
    if (file.is_open())
    {
        int size = sizeof(int);
        char memblock[size];
		file.read (memblock, size);
		random_seed = *(reinterpret_cast<int*>(memblock));
        file.close();
	}
	else {
		cout << "Problem opening /dev/urandom\n";
		exit(-1);
	}
	return random_seed;
}

