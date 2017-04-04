# CMake generated Testfile for 
# Source directory: /home/21603498/Documents/Licence3/be/git/pbrt-v3
# Build directory: /home/21603498/Documents/Licence3/be/git/executable
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pbrt_unit_test "pbrt_test")
subdirs("src/ext/openexr")
subdirs("src/ext/glog")
