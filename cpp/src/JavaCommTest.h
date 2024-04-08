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
#include <cmath>

class JavaCommTest {
private:
	JavaComm & javaComm;

	int recvCnt=0;
	int64_t recvByteCnt=0;

	int sendCnt=0;
	int64_t sendByteCnt=0;

	bool printReceived=true;

	std::atomic<bool> stop;

public:
	JavaCommTest(JavaComm & javaComm) : javaComm(javaComm), stop(false)
	{}

	void run(JNIEnv * envPtr) {
		using namespace std;

		javaComm.setReceiveHandler([&](uint8_t const * bytes, unsigned len) {
			if (isPrintReceived())
				cout << "received bytes: " << len << endl;
			recvCnt++;
			recvByteCnt+=len;
		});

		// prepare the send buffer
		uint8_t buf[5000];

		for (auto i=0; i<5000; i+=4) {
			*(int*)(buf+i)=rand();
		}

		while (!stop)
//		for (auto i=0; i<1000000; i++)
		{
			LOG_DEBUG("calling send...");

			auto sendLen= (unsigned)floor(drand48() * 5000);

			javaComm.send(envPtr, buf, sendLen);
			sendCnt++;
			sendByteCnt += sendLen;

//			usleep(1000);
//			sleep(1);	// to essentially not test sending.
		}

		cout << "JavaCommTest: run() done." << endl;

	}

	void printStats(JNIEnv * envPtr) const {
		using namespace std;
		cout << "recvCnt: " << recvCnt << endl;
		cout << "recvByteCnt: " << recvByteCnt << endl;
		cout << "java recvCnt: " << javaComm.getJavaRecvCnt(envPtr) << endl;
		cout << "java recvByteCnt: " << javaComm.getJavaRecvByteCnt(envPtr) << endl;

		cout << endl;
		cout << "sendCnt: " << sendCnt << endl;
		cout << "sendByteCnt: " << sendByteCnt << endl;
		cout << "java sendCnt: " << javaComm.getJavaSendCnt(envPtr) << endl;
		cout << "java send byteCnt: " << javaComm.getJavaSendByteCnt(envPtr) << endl;
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
