
/*
    pbrt source code is Copyright(c) 1998-2016
                        Matt Pharr, Greg Humphreys, and Wenzel Jakob.

    This file is part of pbrt.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:

    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
    IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
    PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */


// main/pbrt.cpp*
#include <string>
#include <iostream>
#include <fstream>
#include "pbrt.h"
#include "api.h"
#include "parser.h"
#include "parallel.h"
#include "logBE.h"
#include <glog/logging.h>
#include <rayPathParser.hpp>

using namespace pbrt;
using namespace std;

static void usage(const char *msg = nullptr) {
	if (msg)
		fprintf(stderr, "pbrt: %s\n\n", msg);

	fprintf(stderr, R"(usage: pbrt [<options>] <filename.pbrt...>
Rendering options:
  --help               Print this help text.
  --nthreads <num>     Use specified number of threads for rendering.
  --outfile <filename> Write the final image to the given filename.
  --quick              Automatically reduce a number of quality settings to
                       render more quickly.
  --quiet              Suppress all text output other than error messages.

Logging options:
  --logdir <dir>       Specify directory that log files should be written to.
                       Default: system temp directory (e.g. $TMPDIR or /tmp).
  --logtostderr        Print all logging messages to stderr.
  --minloglevel <num>  Log messages at or above this level (0 -> INFO,
                       1 -> WARNING, 2 -> ERROR, 3-> FATAL). Default: 0.
  --v <verbosity>      Set VLOG verbosity.

Reformatting options:
  --cat                Print a reformatted version of the input file(s) to
                       standard output. Does not render an image.
  --toply              Print a reformatted version of the input file(s) to
                       standard output and convert all triangle meshes to
                       PLY files. Does not render an image.
Logging options (BE):
  --log <[n][m][o][p|P]]>	--log: Create a log file in which data will
  								be stored during the rendering process.
  								The stored data can be configured using the
  								following options :-
  							n: Normal vector of each intersection.
  							o: Object hit at each intersection.
  							m: material for each object at the beginning of the log file
  							p: Intersection point of each path.
  							P: Intersection point and direction of each path plus color of it at the end.
                            a: Stand for all and is equivalent to nomP
)");
}

// main program
int main(int argc, char *argv[]) {
	google::InitGoogleLogging(argv[0]);
	FLAGS_stderrthreshold = 2; // Warning and above.
	Options options;
	std::vector<std::string> filenames;
	// Process command-line arguments
	for (int i = 1; i < argc; ++i) {
		if (!strcmp(argv[i], "--nthreads") || !strcmp(argv[i], "-nthreads")) {
			if (i + 1 == argc)
				usage("missing value after --nthreads argument");
			options.nThreads = atoi(argv[++i]);
		} else if (!strncmp(argv[i], "--nthreads=", 11)) {
			options.nThreads = atoi(&argv[i][11]);
		} else if (!strcmp(argv[i], "--outfile") || !strcmp(argv[i], "-outfile")) {
			if (i + 1 == argc)
				usage("missing value after --outfile argument");
			options.imageFile = argv[++i];
		} else if (!strncmp(argv[i], "--outfile=", 10)) {
			options.imageFile = &argv[i][10];
		} else if (!strcmp(argv[i], "--logdir") || !strcmp(argv[i], "-logdir")) {
			if (i + 1 == argc)
				usage("missing value after --logdir argument");
			FLAGS_log_dir = argv[++i];
		} else if (!strncmp(argv[i], "--logdir=", 9)) {
			FLAGS_log_dir = &argv[i][9];
		} else if (!strcmp(argv[i], "--minloglevel") ||
				!strcmp(argv[i], "-minloglevel")) {
			if (i + 1 == argc)
				usage("missing value after --minloglevel argument");
			FLAGS_minloglevel = atoi(argv[++i]);
		} else if (!strncmp(argv[i], "--minloglevel=", 14)) {
			FLAGS_minloglevel = atoi(&argv[i][14]);
		} else if (!strcmp(argv[i], "--quick") || !strcmp(argv[i], "-quick")) {
			options.quickRender = true;
		} else if (!strcmp(argv[i], "--quiet") || !strcmp(argv[i], "-quiet")) {
			options.quiet = true;
		} else if (!strcmp(argv[i], "--cat") || !strcmp(argv[i], "-cat")) {
			options.cat = true;
		} else if (!strcmp(argv[i], "--toply") || !strcmp(argv[i], "-toply")) {
			options.toPly = true;
		} else if (!strcmp(argv[i], "--v") || !strcmp(argv[i], "-v")) {
			if (i + 1 == argc)
				usage("missing value after --v argument");
			FLAGS_v = atoi(argv[++i]);
		} else if (!strncmp(argv[i], "--v=", 4)) {
			FLAGS_v = atoi(argv[i] + 4);
		}
		else if (!strcmp(argv[i], "--logtostderr")) {
			FLAGS_logtostderr = true;
		} else if (!strcmp(argv[i], "--help") || !strcmp(argv[i], "-help") ||
				!strcmp(argv[i], "-h")) {
			usage();
			return 0;
		} else if (!strcmp(argv[i], "--log") || !strcmp(argv[i], "-log")) {
			options.nThreads = 1;
			options.log = true;
			if (i + 1 == argc) options.dir=true;
			else{
				if(strchr(argv[++i],(int)'n'))
					options.normal=true;
				if(strchr(argv[i],(int)'o'))
					options.obj=true;
				if(strchr(argv[i],(int)'m'))
					options.mat=true;
				if(strchr(argv[i],(int)'p'))
					options.path=true;
				if(strchr(argv[i],(int)'P'))
					options.dir=true;
				if(strchr(argv[i],(int) 'a')){
					options.normal = true;
					options.obj = true;
					options.mat = true;
					options.dir = true;
				}
			}
		} else
			filenames.push_back(argv[i]);
	}

	// Print welcome banner
	if (!options.quiet && !options.cat && !options.toPly) {
		printf("pbrt version 3 (built %s at %s) [Detected %d cores]\n",
				__DATE__, __TIME__, NumSystemCores());
#ifndef NDEBUG
		LOG(INFO) << "Running debug build";
		printf("*** DEBUG BUILD ***\n");
#endif // !NDEBUG
		printf(
				"Copyright (c)1998-2016 Matt Pharr, Greg Humphreys, and Wenzel "
				"Jakob.\n");
		printf(
				"The source code to pbrt (but *not* the book contents) is covered "
				"by the BSD License.\n");
		printf("See the file LICENSE.txt for the conditions of the license.\n");
		fflush(stdout);
	}
	pbrtInit(options);
	// Process scene description
	if (filenames.empty()) {
		// Parse scene from standard input
		ParseFile("-");
	} else {
		// Parse scene from input files
		for (const std::string &f : filenames){
			string ff=f.substr(f.find_last_of('/')+1);
			if(options.log)
				logBE::logInit(ff.substr(0, ff.find("."))+".txt",options);
			if (!ParseFile(f))
				Error("Couldn't open scene file \"%s\"", f.c_str());
			if(options.log)
				logBE::logClose();
		}
	}
	pbrtCleanup();
	if(options.log)
		for (const std::string &f : filenames){
			string ff=f.substr(f.find_last_of('/')+1);
			rayPathParser * rpp = new rayPathParser(ff.substr(0, ff.find("."))+".txt");
			rpp->parseFile();
		}
	return 0;
}
