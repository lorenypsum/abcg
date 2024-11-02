project(attentiontraining)
add_executable(${PROJECT_NAME} main.cpp window.cpp distraction_objects.cpp
                               target_objects.cpp)
enable_abcg(${PROJECT_NAME})