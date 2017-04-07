# CMake generated Testfile for 
# Source directory: /home/21303468/workspace/analyseEspaceRayons/pbrt-v3
# Build directory: /home/21303468/workspace/analyseEspaceRayons/exe
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pbrt_unit_test "pbrt_test")
subdirs("src/ext/openexr")
subdirs("src/ext/glog")
