cmake_minimum_required(VERSION 3.0.0)
project(FunDataStructures VERSION 0.1.0)

include(CTest)
include_directories(include/)

FILE(GLOB CSources *.c)
ADD_EXECUTABLE(FunDataStructures ${CSources})

target_link_libraries(FunDataStructures m)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
