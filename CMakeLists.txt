project(Op3nD)
cmake_minimum_required(VERSION 2.8.9)
find_package(Qt5Widgets REQUIRED)

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
	        src/view/newProjectDialog.ui
	        src/view/testDialog.ui
	        src/view/luaEditor.ui
	        src/view/physicsDialog.ui)

set(Op3nD_SRCS
               src/main.cpp
               src/ProjectManagement/FileSys.cpp
               src/ProjectManagement/MXML.cpp
               src/ProjectManagement/Project.cpp
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
               src/Engine/ObjectTypes/Physical.cpp
               src/Engine/ObjectTypes/Resource.cpp
               src/Engine/ObjectTypes/Scripted.cpp
               src/Engine/ObjectTypes/Text.cpp
               src/Engine/ObjectTypes/Tile.cpp
               src/Engine/States/EditorState.cpp
               src/Engine/States/MeshState.cpp
               src/Engine/States/PlayState.cpp
               src/Engine/States/State.cpp
               src/Engine/States/StateManager.cpp
               )

set(Op3nD_Editor_SRCS  src/Op3nD.cpp
               src/InterfaceModule/ProjectDialog.cpp
               src/InterfaceModule/ResourceTreeModel.cpp
               src/InterfaceModule/ImageTreeView.cpp
               src/InterfaceModule/HeaderTreeViewDelegate.cpp
               src/InterfaceModule/SpinBoxDelegate.cpp
               src/InterfaceModule/QSFMLCanvas.cpp
               src/InterfaceModule/TestDialog.cpp
               src/InterfaceModule/PropertiesTreeModel.cpp
               src/InterfaceModule/TreeItem.cpp
               src/InterfaceModule/LuaHighlighter.cpp
               src/InterfaceModule/LuaEditor.cpp
               src/InterfaceModule/PhysicsDialog.cpp
               )
               
set(Op3nD_Editor_HEADERS src/Op3nD.h
               src/InterfaceModule/ProjectDialog.h
               src/InterfaceModule/ResourceTreeModel.h
               src/InterfaceModule/ImageTreeView.h
               src/InterfaceModule/HeaderTreeViewDelegate.h
               src/InterfaceModule/SpinBoxDelegate.h
               src/InterfaceModule/QSFMLCanvas.h
               src/InterfaceModule/TestDialog.h
               src/InterfaceModule/PropertiesTreeModel.h
               src/InterfaceModule/TreeItem.h
               src/InterfaceModule/LuaHighlighter.h
               src/InterfaceModule/LuaEditor.h
               src/InterfaceModule/PhysicsDialog.h
               )
               

qt5_wrap_ui(Op3nD_FORM_HEADERS ${Op3nD_FORMS})
               
qt5_wrap_cpp(Op3nD_MOCS ${Op3nD_Editor_HEADERS})

qt5_add_resources(Op3nD_Resources src/view/op3nd.qrc)

add_executable(Op3nD_Editor ${Op3nD_SRCS} ${Op3nD_MOCS} ${Op3nD_FORM_HEADERS} ${Op3nD_Editor_SRCS} ${Op3nD_Resources})
qt5_use_modules(Op3nD_Editor Widgets)

add_executable(Op3nD_Interpreter ${Op3nD_SRCS})
include_directories(${QT_INCLUDES} 
                    ${BULLET_INCLUDE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Op3nD_Editor ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Boost_LIBRARIES} ${BULLET_LIBRARIES} GLEW GL lua5.2 assimp sfml-graphics sfml-window sfml-system X11 GLU)
target_link_libraries(Op3nD_Interpreter ${Boost_LIBRARIES} ${BULLET_LIBRARIES} GL GLEW lua5.2 assimp sfml-graphics sfml-window sfml-system X11 GLU)
set_target_properties(Op3nD_Interpreter PROPERTIES COMPILE_FLAGS -DNOGUI)

install(TARGETS Op3nD_Editor Op3nD_Interpreter RUNTIME DESTINATION bin)

SET(MAJOR_VERSION 0)
SET(MINOR_VERSION 1)
SET(PATCH_VERSION 0)
 
IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)
 
SET(CPACK_SET_DESTDIR "on")
SET(CPACK_PACKAGING_INSTALL_PREFIX "/tmp")
SET(CPACK_GENERATOR "DEB")

set (CPACK_DEBIAN_PACKAGE_DEPENDS "libassimp3 (>= 2.0.863), libboost-filesystem1.49.0 (>= 1.49.0-1), libboost-system1.49.0 (>= 1.49.0-1), libbullet2.80, libc6 (>= 2.14), libgcc1 (>= 1:4.1.1), libglew1.8 (>= 1.8.0), liblua5.2-0, libqt5core5 (>= 5.0.2), libqt5gui5 (>= 5.0.2), libqt5widgets5 (>= 5.0.2), libstdc++6 (>= 4.6), libsfml-system2, libsfml-audio2, libsfml-window2, libsfml-graphics2")

SET(CPACK_PACKAGE_DESCRIPTION "Op3nD game framework")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "bla bla bla... bah")
SET(CPACK_PACKAGE_VENDOR "JSMTUX")
SET(CPACK_PACKAGE_CONTACT "jsmtux ")
SET(CPACK_PACKAGE_VERSION_MAJOR "${MAJOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_MINOR "${MINOR_VERSION}")
SET(CPACK_PACKAGE_VERSION_PATCH "${PATCH_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${MAJOR_VERSION}.${MINOR_VERSION}.${CPACK_PACKAGE_VERSION_PATCH}")
 
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "Editors")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

INSTALL(FILES "Op3nD.desktop" DESTINATION "/usr/local/share/applications/")
INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/files/" DESTINATION "/usr/local/share/O3D")

SET(CPACK_COMPONENTS_ALL Libraries ApplicationData)
INCLUDE(CPack)
 
ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
