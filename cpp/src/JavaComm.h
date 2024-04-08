/*
 * JavaComm.h
 *
 *  Created on: Apr 7, 2024
 *      Author: mikejyg
 */

#ifndef JAVACOMM_H_
#define JAVACOMM_H_

#include "Jvm.h"
#include <global.h>
#include <functional>

/**
 * send and receive using java programs.
 */
class JavaComm {
private:
	jclass senderClass;
	jmethodID sendMethod;

	std::function<void(char const *)> receiveHandler;

public:
	JavaComm(JNIEnv * envPtr) {
		senderClass = envPtr->FindClass("javaComm/Sender");
		sendMethod = envPtr->GetStaticMethodID(senderClass, "send", "(Ljava/lang/String;)I");
	}

	/**
	 * argument: f the incoming message handler.
	 */
	void startListener(JNIEnv * envPtr) {
		jclass cls = envPtr->FindClass("javaComm/Listener");
		jmethodID mid = envPtr->GetStaticMethodID(cls, "startListener", "()I");

		LOG_DEBUG("calling start...");
		jint k = envPtr->CallStaticIntMethod(cls, mid);

		if (k!=0)
			throw std::runtime_error("java listener start() failed.");
	}

	void stopListener(JNIEnv * envPtr) {
		jclass cls = envPtr->FindClass("javaComm/Listener");
		jmethodID mid = envPtr->GetStaticMethodID(cls, "stopListener", "()V");

		LOG_DEBUG("calling stop...");
		envPtr->CallStaticVoidMethod(cls, mid);
	}

	void send(JNIEnv * envPtr, std::string const & str) {
		jstring jstr = envPtr->NewStringUTF(str.c_str());
		auto k = envPtr->CallStaticIntMethod( senderClass, sendMethod, jstr );
		envPtr->DeleteLocalRef(jstr);
		if (!k)
			throw std::runtime_error("java send failed.");
	}

	void handleReceive(char const * cstr) {
		if (receiveHandler)
			receiveHandler(cstr);
		else
			LOG_DEBUG(std::string("C++ received: ") + cstr);
	}

	template<typename F>
	void setReceiveHandler(F && receiveHandler) {
		this->receiveHandler = std::forward<F>(receiveHandler);
	}

	int getJavaSendCnt(JNIEnv * envPtr) const {
		auto method = envPtr->GetStaticMethodID(senderClass, "getSendCnt", "()I");
		auto k = envPtr->CallStaticIntMethod( senderClass, method );
		return k;
	}


};

#endif /* JAVACOMM_H_ */
