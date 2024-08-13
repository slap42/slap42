cmake_minimum_required(VERSION 3.20)
project(opensimplex)

add_library(opensimplex OpenSimplexNoise.hh OpenSimplexNoise.cc)
target_include_directories(opensimplex PUBLIC .)
target_compile_features(opensimplex PRIVATE cxx_std_17)
