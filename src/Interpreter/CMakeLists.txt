cmake_minimum_required(VERSION 2.8)

find_package(Bullet REQUIRED)

find_package(Lua51 REQUIRED)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories(${O3Dsrc_project_SOURCE_DIR}/Engine)

include_directories(${O3Dsrc_project_SOURCE_DIR}/ProjectManagement)

set(Interpreter_SRCS
    main.cpp
    RenderingSFML.cpp
)

add_executable(O3DInterpreter ${Interpreter_SRCS})
include_directories(
 ${BULLET_INCLUDE_DIR}
 ${LUA_INCLUDE_DIR})

target_link_libraries(O3DInterpreter ${Boost_LIBRARIES} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} GL GLEW assimp sfml-graphics sfml-window sfml-system X11 GLU)
target_link_libraries(O3DInterpreter o3dengine projectmanagement)
