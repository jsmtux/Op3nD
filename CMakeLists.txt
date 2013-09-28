project(Op3nd)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(Op3nd_SRCS Op3nd.cpp main.cpp)
qt4_automoc(${Op3nd_SRCS})
add_executable(Op3nd ${Op3nd_SRCS})
target_link_libraries(Op3nd ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS Op3nd RUNTIME DESTINATION bin)
