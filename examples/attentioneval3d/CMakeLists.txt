project(attentioneval3d)
add_executable(${PROJECT_NAME} main.cpp window.cpp model.cpp distraction_objects.cpp)
enable_abcg(${PROJECT_NAME})