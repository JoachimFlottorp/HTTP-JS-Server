cmake_minimum_required(VERSION 3.27)
project(HTTP_JS_Server)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)

include(FeatureSummary)
feature_summary(WHAT ALL)