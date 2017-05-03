import os

gene_list = Split('''src/Gabor_gene.cpp
src/Sobel_gene.cpp
src/Gaussian_gene.cpp
src/Median_blur_gene.cpp
src/Laplacian_gene.cpp
src/Gene.cpp
src/Gradient_gene.cpp''')

ml_list = Split('''src/mlCreature.cpp src/Genome.cpp src/mlcommon.cpp src/random_generator.cpp''') 




test_samme_list = ['src/test_samme.cpp'] + gene_list + ml_list


find_weaklearners_list = ['src/find_weaklearners.cpp'] + gene_list + ml_list

single_predict_list = ['src/single_predict.cpp'] + gene_list + ml_list



includes = ['include', '/usr/local/include/opencv', '/usr/local/include/opencv2']

#cxx_flags = "-Wall -g -fno-inline -fopenmp"
#cxx_flags = "-Wall -g -fno-inline"
cxx_flags = "-Wall -O3 -fopenmp"
link_flags = "-fopenmp"

# Setup compile environment
env = Environment(CPPPATH=includes, CXXFLAGS=cxx_flags, LINKFLAGS=link_flags, 
		LIBS=['opencv_core', 'opencv_highgui','opencv_imgproc', 'boost_filesystem', 'boost_program_options', 'boost_system', 'boost_regex', 'opencv_objdetect', 'opencv_ml', 'opencv_imgcodecs'],
		LIBPATH=['/usr/local/lib'])
if 'LD_LIBRARY_PATH' in os.environ:
	env['ENV']['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH']




env.Program('bin/find_weak', find_weaklearners_list)

env.Program('bin/test_samme', test_samme_list)

env.Program('bin/single_predict', single_predict_list)