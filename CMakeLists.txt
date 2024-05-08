cmake_minimum_required(VERSION 3.25)
project(SDiZO2)

set(CMAKE_CXX_STANDARD 17)

add_executable(SDiZO2 main.cpp graphs/matrix.cpp graphs/matrix.h graphs/graph.cpp graphs/graph.h graphs/list.cpp graphs/list.h graphs/util/edge.h structures/list.cpp structures/list.h structures/heap.cpp structures/heap.h structures/listElement.h graphs/util/test.cpp graphs/util/test.h)
