import os

gene_list = Split('''src/Gabor_gene.cpp
src/Sobel_gene.cpp
src/Gaussian_gene.cpp
src/Median_blur_gene.cpp
src/Laplacian_gene.cpp
src/Gene.cpp
src/Gradient_gene.cpp''')

ml_list = Split('''src/mlCreature.cpp src/Genome.cpp src/mlcommon.cpp src/random_generator.cpp''')

capture_list = Split('''src/Camera_capture.cpp src/Component_finder.cpp src/Image_writer.cpp src/Point_grey_capture.cpp src/Webcam_capture.cpp''') 

opencv_libs = Split('''opencv_core opencv_highgui opencv_imgproc opencv_objdetect opencv_ml opencv_imgcodecs opencv_videoio''')

boost_libs = Split('''boost_filesystem boost_program_options boost_system boost_regex''')

flycapture_libs = Split('''flycapture flycapturegui''')



test_samme_list = ['src/test_samme.cpp'] + gene_list + ml_list


find_weaklearners_list = ['src/find_weaklearners.cpp'] + gene_list + ml_list

single_predict_list = ['src/single_predict.cpp'] + gene_list + ml_list

image_capture_list = ['src/image_capture.cpp'] + capture_list



includes = ['include', '/usr/local/include/opencv2', '/usr/local/include/opencv','/usr/local/include/flycapture/']

#cxx_flags = "-Wall -g -fno-inline -fopenmp"
#cxx_flags = "-Wall -g -fno-inline"
cxx_flags = "-Wall -O3 -fopenmp -std=c++11"
link_flags = "-fopenmp"

# Setup compile environment
env = Environment(CPPPATH=includes, CXXFLAGS=cxx_flags, LINKFLAGS=link_flags, 
		LIBS=boost_libs + opencv_libs ,
		LIBPATH=['/usr/local/lib'],
		LD_LIBRARY_PATH=['/usr/local/lib'])
#if 'LD_LIBRARY_PATH' in os.environ:
#	env['ENV']['LD_LIBRARY_PATH'] = os.environ['LD_LIBRARY_PATH']


#['opencv_core', 'opencv_highgui','opencv_imgproc', 'boost_filesystem', 'boost_program_options', 'boost_system', 'boost_regex', 'opencv_objdetect', 'opencv_ml', 'opencv_imgcodecs','opencv_videoio']

env.Program('bin/find_weak', find_weaklearners_list)

env.Program('bin/test_samme', test_samme_list)

#env.Program('bin/single_predict', single_predict_list)

#env.Program('bin/image_capture', image_capture_list)
