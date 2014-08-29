cmake_minimum_required(VERSION 2.8)

find_package(Bullet REQUIRED)

find_package(Lua51 REQUIRED)

find_package(Qt5Widgets)

SET(CMAKE_CXX_FLAGS "-std=c++11")

include_directories(${O3Dsrc_project_SOURCE_DIR}/Engine)

include_directories(${O3Dsrc_project_SOURCE_DIR}/ProjectManagement)

include_directories(${O3Dsrc_project_SOURCE_DIR}/Editor)

set(Editor_SRCS
    main.cpp
    Op3nD.cpp
    ProjectDialog.cpp
    ResourceTreeModel.cpp
    ImageTreeView.cpp
    HeaderTreeViewDelegate.cpp
    SpinBoxDelegate.cpp
    QSFMLCanvas.cpp
    TestDialog.cpp
    PropertiesTreeModel.cpp
    TreeItem.cpp
    LuaHighlighter.cpp
    LuaEditor.cpp
    PhysicsDialog.cpp
)
 
set(Editor_HEADERS Op3nD.h
    ProjectDialog.h
    ResourceTreeModel.h
    ImageTreeView.h
    HeaderTreeViewDelegate.h
    SpinBoxDelegate.h
    QSFMLCanvas.h
    TestDialog.h
    PropertiesTreeModel.h
    TreeItem.h
    LuaHighlighter.h
    LuaEditor.h
    PhysicsDialog.h
)

set(Op3nD_FORMS ${O3Dsrc_project_SOURCE_DIR}/view/mainWindow.ui
    ${O3Dsrc_project_SOURCE_DIR}/view/newProjectDialog.ui
    ${O3Dsrc_project_SOURCE_DIR}/view/testDialog.ui
    ${O3Dsrc_project_SOURCE_DIR}/view/luaEditor.ui
    ${O3Dsrc_project_SOURCE_DIR}/view/physicsDialog.ui)

qt5_wrap_ui(Op3nD_FORM_HEADERS ${Op3nD_FORMS})
 
qt5_wrap_cpp(Op3nD_MOCS ${Editor_HEADERS})

qt5_add_resources(Op3nD_Resources ${O3Dsrc_project_SOURCE_DIR}/view/op3nd.qrc)

add_executable(O3DEditor ${Editor_SRCS} ${Op3nD_MOCS} ${Op3nD_FORM_HEADERS})
qt5_use_modules(O3DEditor Widgets OpenGL)

include_directories(
    ${QT_INCLUDES} 
    ${BULLET_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(O3DEditor ${O3Dsrc_project_LIBRARIES} ${O3DEngine_project_LIBRARIES} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${Boost_LIBRARIES} ${BULLET_LIBRARIES} ${LUA_LIBRARIES} GL GLEW assimp sfml-graphics sfml-window sfml-system X11 GLU)
target_link_libraries(O3DEditor o3dengine projectmanagement)
