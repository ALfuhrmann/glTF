// Copyright (c) 2012, Motorola Mobility, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of the Motorola Mobility, Inc. nor the names of its
//    contributors may be used to endorse or promote products derived from this
//    software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//find . -name '*.dae' -exec dae2json {} \;

#import <getopt.h>

#include <iostream>

#include "GLTF.h"
#include "GLTF-OpenCOLLADA.h"
#include "GLTFConverterContext.h"

#include "COLLADA2GLTFWriter.h"

#define STDOUT_OUTPUT 0
#define OPTIONS_COUNT 5

typedef struct {
    const char* name;
    int has_arg;
    const char* help;
} OptionDescriptor;

option* opt_options;
std::string helpMessage = "";

static const OptionDescriptor options[] = {
	{ "f",				required_argument,  "-f -> path of input file, argument [string]" },
	{ "o",				required_argument,  "-o -> path of output file argument [string]" },
	{ "a",              required_argument,  "-a -> export animations, argument [bool], default:true" },
	{ "i",              no_argument,        "-i -> invert-transparency, argument [bool], default:false" },
	{ "h",              no_argument,        "-h -> help" }
};

static void buildOptions() {
    helpMessage += "*COLLADA2GLTF V 0.1*\n\n";
    helpMessage += "usage: collada2gltlf -f [file] [options]\n";
    helpMessage += "options:\n";
    
    opt_options = (option*)malloc(sizeof(option) * OPTIONS_COUNT);
    
    for (size_t i = 0 ; i < OPTIONS_COUNT ; i++) {
        opt_options[i].flag = 0;
        opt_options[i].val = 'f';
        opt_options[i].name = options[i].name;
        opt_options[i].has_arg = options[i].has_arg;
        
        helpMessage += options[i].help;
        helpMessage += "\n";
    }
}

static void dumpHelpMessage() {
    printf("%s\n", helpMessage.c_str());
}

static std::string replacePathExtensionWithJSON(const std::string& inputFile)
{
    COLLADABU::URI inputFileURI(inputFile.c_str());
    
    std::string pathDir = inputFileURI.getPathDir();
    std::string fileBase = inputFileURI.getPathFileBase();
    
    return pathDir + fileBase + ".json";
}

static bool processArgs(int argc, char * const * argv, GLTF::GLTFConverterContext *converterArgs) {
	int ch;
    std::string file;
    std::string output;
    converterArgs->invertTransparency = false;
    converterArgs->exportAnimations = true;
    bool hasOutputPath = false;
    bool hasInputPath = false;
 
    buildOptions();
    
    if (argc == 2) {
        converterArgs->inputFilePath = argv[1];
        converterArgs->outputFilePath = replacePathExtensionWithJSON(converterArgs->inputFilePath);
        return true;
    }
    
    while ((ch = getopt_long(argc, argv, "f:o:a:ih", opt_options, 0)) != -1) {
        switch (ch) {
            case 'h':
                return false;
            case 'f':
                converterArgs->inputFilePath = optarg;
                hasInputPath = true;
				break;
            case 'o':
                converterArgs->outputFilePath = replacePathExtensionWithJSON(optarg);
                hasOutputPath = true;
				break;
            case 'i':
                converterArgs->invertTransparency = true;
                break;
            case 'a':
                //converterArgs->exportAnimations = true;
                break;
			case 0:
				break;
		}
	}
    
    if (!hasInputPath) {
        dumpHelpMessage();
        return false;
    }
    
    if (!hasOutputPath) {
        converterArgs->outputFilePath = replacePathExtensionWithJSON(converterArgs->inputFilePath);
    }
        
    return true;
}

int main (int argc, char * const argv[]) {
    GLTF::GLTFConverterContext converterArgs;
    
    if (processArgs(argc, argv, &converterArgs)) {
#if !STDOUT_OUTPUT
        FILE* fd = fopen(converterArgs.outputFilePath.c_str(), "w");
        if (fd) {
            rapidjson::FileStream s(fd);
#else
            rapidjson::FileStream s(stdout);
#endif
            rapidjson::PrettyWriter <rapidjson::FileStream> jsonWriter(s);
            printf("converting:%s ... as %s \n",converterArgs.inputFilePath.c_str(), converterArgs.outputFilePath.c_str());
            GLTF::COLLADA2GLTFWriter* writer = new GLTF::COLLADA2GLTFWriter(converterArgs, &jsonWriter);
            writer->write();
            printf("[completed conversion]\n");
#if !STDOUT_OUTPUT
            fclose(fd);
            delete writer;
        }
#endif
    }
    return 0;
}
