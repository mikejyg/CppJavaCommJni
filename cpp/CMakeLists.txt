cmake_minimum_required(VERSION 3.0)

project(CppJavaComm)

add_executable(cpp src/main.cpp)

set(CMAKE_CXX_STANDARD 11)

target_include_directories(cpp PUBLIC
  src
  /usr/lib/jvm/java-17-openjdk-amd64/include/
  /usr/lib/jvm/java-17-openjdk-amd64/include/linux
  )

target_link_directories(cpp PUBLIC /usr/lib/jvm/java-17-openjdk-amd64/lib/server)
  

target_link_libraries(cpp PUBLIC jvm)
  
