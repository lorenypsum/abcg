project(attentiongame3d)
add_executable(${PROJECT_NAME} main.cpp window.cpp model.cpp game_objects.cpp trackball.cpp)
enable_abcg(${PROJECT_NAME})