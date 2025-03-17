#ifndef __UTILITY_H
#define __UTILITY_H

#include <cstdio>
#include <cstdlib>
#include <iostream>

namespace util {
    void print_usage() {
        std::cerr << "Usage: ./<Program name> <Input Video Name> <Pattern Face Picture Name> <Output Video Name>" << std::endl;
        exit(-1);
    }
    void call_exception(std::string message) {
        std::cerr << "[ERROR] " << message << std::endl;
        exit(-1);
    }
    void split_va(std::string input_file) {
        std::string command = "ffmpeg -y -i " + input_file + " -vn -acodec copy ./tmp_work/audio.m4a";
        system(command.c_str());
        command = "ffmpeg -y -i " + input_file + " -codec copy -bsf: h264_mp4toannexb -f h264 ./tmp_work/input.h264";
        system(command.c_str());
    }
}

#endif