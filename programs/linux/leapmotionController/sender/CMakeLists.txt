# leapmotionController: Sender
cmake_minimum_required(VERSION 2.8)
project(leapmotionControllerSender)
add_executable(leapmotionControllerSender main.cpp)
find_package(YARP REQUIRED)
target_link_libraries(leapmotionControllerSender ${YARP_LIBRARIES})
