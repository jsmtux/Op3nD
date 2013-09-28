project(Op3nD)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Op3nD_SRCS Op3nD.cpp main.cpp)
qt4_automoc(${Op3nD_SRCS})
add_executable(Op3nD ${Op3nD_SRCS})
target_link_libraries(Op3nD ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS Op3nD RUNTIME DESTINATION bin)
