cmake_minimum_required(VERSION 3.20)
project(opensimplex)

add_library(opensimplex OpenSimplexNoise.hh OpenSimplexNoise.cc)
target_include_directories(opensimplex PUBLIC .)
