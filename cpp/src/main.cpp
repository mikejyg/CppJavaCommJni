/*
 * main.cpp
 *
 *  Created on: Apr 7, 2024
 *      Author: mikejyg
 */

#include <iostream>
#include "Jvm.h"
#include "JavaComm.h"
#include "JavaCommTest.h"
#include "thread"

using namespace std;

Jvm jvm;
JavaComm * javaCommPtr;

void runTestInMainThread(int duration, JavaCommTest & test) {
	// use a timer thread to stop the test.
	std::thread timerThread([&](){
		sleep(duration);
		test.setStop();
	});

	test.run(jvm.getMainThreadJniEnvPtr());

	timerThread.join();
}

void runTestInNewThread(int duration, JavaCommTest & test) {
	std::thread testThread([&](){
		auto * envPtr = jvm.attachCurrentThread();
		test.run(envPtr);
		jvm.detachCurrentThread();
	});

	sleep(duration);
	test.setStop();

	testThread.join();
}

/**
 * arguments: duration m|n
 *   m: run test in main thread
 *   n: run test in new thread
 */
int main(int argc, char **argv) {
	jvm.init();
	javaCommPtr = new JavaComm(jvm.getMainThreadJniEnvPtr());
	javaCommPtr->startListener(jvm.getMainThreadJniEnvPtr());

	JavaCommTest test(*javaCommPtr);
	test.setPrintReceived(false);

	int duration=60;
	if (argc>1)
		duration=atoi(argv[1]);
	cout << "test duration: " << duration << endl;

	bool runTestInMainThreadFlag=true;
	if (argc>2) {
		if (argv[2][0]=='m') {
			;
		} else if (argv[2][0]=='n') {
			runTestInMainThreadFlag=false;
		} else {
			cout << "invalid argument: " << argv[2] << endl;
			return 1;
		}
	}

	if (runTestInMainThreadFlag) {
		cout << "runTestInMainThread()..." << endl;
		runTestInMainThread(duration, test);
	} else {
		cout << "runTestInNewThread()..." << endl;
		runTestInNewThread(duration, test);
	}

	javaCommPtr->stopListener(jvm.getMainThreadJniEnvPtr());

	test.printStats(jvm.getMainThreadJniEnvPtr());

	//	jvm.destroyJvm();

	cout << "main() done." << endl;
}
