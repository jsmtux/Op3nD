project(Op3nD)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

set(BULLET_INCLUDE_DIR /usr/include/bullet/)
find_package(BULLET REQUIRED)

find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

SET(CMAKE_CXX_FLAGS "-std=c++11")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.49.0 COMPONENTS filesystem system) 

set(Op3nD_FORMS src/view/mainWindow.ui
	        src/view/newProjectDialog.ui)

set(Op3nD_SRCS src/Op3nD.cpp
               src/main.cpp
               src/ProjectManagement/FileSys.cpp
               src/ProjectManagement/MXML.cpp
               src/ProjectManagement/Project.cpp
               src/InterfaceModule/ProjectDialog.cpp
               src/InterfaceModule/ResourceTreeModel.cpp
               src/InterfaceModule/ImageTreeView.cpp
               src/InterfaceModule/HeaderTreeViewDelegate.cpp
               src/InterfaceModule/QSFMLCanvas.cpp
               src/Engine/Base.cpp
               src/Engine/Controller.cpp
               src/Engine/lua_typed_enums.cpp
               src/Engine/PhysicsWorld.cpp
               src/Engine/Timer.cpp
               src/Engine/Graphics/GLDebugDrawer.cpp
               src/Engine/Graphics/PickingTexture.cpp
               src/Engine/Graphics/RenderingContext.cpp
               src/Engine/Graphics/RenderingSFML.cpp
               src/Engine/Graphics/Shading.cpp
               src/Engine/Math/Matrix.cpp
               src/Engine/Math/Quaternion.cpp
               src/Engine/Math/Vector2.cpp
               src/Engine/Math/Vector3.cpp
               src/Engine/ObjectTypes/Box.cpp
               src/Engine/ObjectTypes/Camera.cpp
               src/Engine/ObjectTypes/Editable.cpp
               src/Engine/ObjectTypes/Image.cpp
               src/Engine/ObjectTypes/Light.cpp
               src/Engine/ObjectTypes/Line.cpp
               src/Engine/ObjectTypes/Model3d.cpp
               src/Engine/ObjectTypes/Object.cpp
               src/Engine/ObjectTypes/Physical.cpp
               src/Engine/ObjectTypes/Resource.cpp
               src/Engine/ObjectTypes/Scripted.cpp
               src/Engine/ObjectTypes/Text.cpp
               src/Engine/ObjectTypes/Tile.cpp
               src/Engine/States/EditorState.cpp
               src/Engine/States/MeshState.cpp
               src/Engine/States/PlayState.cpp
               src/Engine/States/State.cpp
               )

set(Op3nD_HEADERS src/Op3nD.h
               src/ProjectManagement/FileSys.h
               src/ProjectManagement/MXML.h
               src/ProjectManagement/Project.h
               src/InterfaceModule/ProjectDialog.h
               src/InterfaceModule/ResourceTreeModel.h
               src/InterfaceModule/ImageTreeView.h
               src/InterfaceModule/HeaderTreeViewDelegate.h
               src/InterfaceModule/QSFMLCanvas.h)

qt4_wrap_ui(Op3nD_FORM_HEADERS ${Op3nD_FORMS})
               
qt4_wrap_cpp(Op3nD_MOCS ${Op3nD_HEADERS})

add_executable(Op3nD ${Op3nD_SRCS} ${Op3nD_MOCS} ${Op3nD_FORM_HEADERS})
include_directories(${QT_INCLUDES} 
                    ${BULLET_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Op3nD ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Boost_LIBRARIES} ${BULLET_LIBRARIES} GL GLEW lua5.2 assimp sfml-graphics sfml-window sfml-system X11 GLU)
install(TARGETS Op3nD RUNTIME DESTINATION bin)
