#Project CppJavaCommJni

This project is to prove out a way to use JNI Invocation API for C++ to utilize Java to provide communication send and receive.

Multi-threading within the JVM is required.

It provides a demo and an example of such usage.

## Organization

### directory java
Contains the Java project that provides communication send and receive.

### cppLib
To build a DLL for Java to call C++.

### cpp
The main program in C++.
