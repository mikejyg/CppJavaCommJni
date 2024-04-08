/*
 * JavaCommTest.h
 *
 *  Created on: Apr 7, 2024
 *      Author: mikejyg
 */

#ifndef JAVACOMMTEST_H_
#define JAVACOMMTEST_H_

#include <global.h>
#include <iostream>
#include "JavaComm.h"
#include <unistd.h>
#include <atomic>

class JavaCommTest {
private:
	JavaComm & javaComm;

	int recvCnt=0;
	int sendCnt=0;

	bool printReceived=true;

	std::atomic<bool> stop;

public:
	JavaCommTest(JavaComm & javaComm) : javaComm(javaComm), stop(false)
	{}

	void run(JNIEnv * envPtr) {
		using namespace std;

		javaComm.setReceiveHandler([&](std::string const & str) {
			if (isPrintReceived())
				cout << "received: " << str << endl;
			recvCnt++;
		});

		while (!stop)
//		for (auto i=0; i<1000000; i++)
		{
			LOG_DEBUG("calling send...");

			std::string str;

//			str = std::to_string(rand());

			for (auto i=0; i<1000; i++) {
				str += std::to_string(rand());
				str += " ";
			}

			javaComm.send(envPtr, str);
			sendCnt++;

//			usleep(1000);
//			sleep(1);	// to essentially not test sending.
		}

		cout << "JavaCommTest: run() done." << endl;

	}

	void printStats(JNIEnv * envPtr) const {
		using namespace std;
		cout << "recvCnt: " << recvCnt << endl;
		cout << "sendCnt: " << sendCnt << endl;
		cout << "java sendCnt: " << javaComm.getJavaSendCnt(envPtr) << endl;
	}

	///////////////////////

	bool isPrintReceived() const {
		return printReceived;
	}

	void setPrintReceived(bool printReceived = true) {
		this->printReceived = printReceived;
	}

	void setStop() {
		stop=true;
	}


};

#endif /* JAVACOMMTEST_H_ */
