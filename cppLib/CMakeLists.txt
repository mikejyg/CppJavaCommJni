cmake_minimum_required(VERSION 3.0)

project(CppJavaCommLib)
set(CMAKE_CXX_STANDARD 11)

add_library(cppLib SHARED src/javaCommReceive.cpp)

target_include_directories(cppLib PUBLIC
  ../java/include
  ../cpp/src
  /usr/lib/jvm/java-17-openjdk-amd64/include/
  /usr/lib/jvm/java-17-openjdk-amd64/include/linux
  )

