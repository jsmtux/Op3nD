cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.49.0 COMPONENTS filesystem system) 

add_library(projectmanagement STATIC FileSys.cpp MXML.cpp Project.cpp)
target_link_libraries(projectmanagement ${Boost_LIBRARIES})

install(TARGETS projectmanagement DESTINATION lib)
install(FILES FileSys.h MXML.h Project.h DESTINATION include)
