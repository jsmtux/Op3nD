cmake_minimum_required(VERSION 2.8)

set(BULLET_INCLUDE_DIR /usr/include/bullet/)
find_package(Bullet REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

find_package(Lua51 REQUIRED)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

include_directories(${O3Dsrc_project_SOURCE_DIR}/Editor)

include_directories(${O3Dsrc_project_SOURCE_DIR}/Engine)

include_directories(${O3Dsrc_project_SOURCE_DIR}/ProjectManagement)

set(Op3nD_SRCS
 Base.cpp
 Controller.cpp
 lua_typed_enums.cpp
 PhysicsWorld.cpp
 Timer.cpp
 Graphics/GLDebugDrawer.cpp
 Graphics/PickingTexture.cpp
 Graphics/RenderingContext.cpp
 Graphics/Shading.cpp
 Math/Matrix.cpp
 Math/Quaternion.cpp
 Math/Vector2.cpp
 Math/Vector3.cpp
 ObjectTypes/Camera.cpp
 ObjectTypes/Editable.cpp
 ObjectTypes/Scripted.cpp
 ObjectTypes/Tile.cpp
 ObjectTypes/Physical.cpp
 Resources/BaseResourceLoader.cpp
 Resources/Line.cpp
 Resources/Image.cpp
 Resources/ResourceManager.cpp
 Resources/Box.cpp
 Resources/Model3d.cpp
 Resources/ImageLoader.cpp
 Resources/Font.cpp
 Resources/ModelLoader.cpp
 Resources/Text.cpp
 Resources/Light.cpp
 Resources/FontLoader.cpp
 Resources/Resource.cpp
 States/EditorState.cpp
 States/MeshState.cpp
 States/PlayState.cpp
 States/State.cpp
 States/StateManager.cpp
 States/StateUpdate.cpp
 States/BasicStateUpdate.cpp
 States/PhysicsStateUpdate.cpp
 )

add_library(o3dengine STATIC ${Op3nD_SRCS})
include_directories(
 ${BULLET_INCLUDE_DIR}
 ${LUA_INCLUDE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(o3dengine ${Boost_LIBRARIES} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} GL GLEW assimp sfml-graphics sfml-window sfml-system X11 GLU)

install(TARGETS o3dengine DESTINATION lib)
#install(FILES FileSys.h MXML.h Project.h DESTINATION include)
