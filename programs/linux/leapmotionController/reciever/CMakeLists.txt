# leapmotionController: Reciever
add_executable(leapmotionControllerReciever main.cpp)
find_package(YARP REQUIRED)
target_link_libraries(leapmotionControllerReciever ${YARP_LIBRARIES})
