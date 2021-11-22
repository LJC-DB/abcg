project(evadethrough)
add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp model.cpp car.cpp ground.cpp camera.cpp walls.cpp)
enable_abcg(${PROJECT_NAME})
