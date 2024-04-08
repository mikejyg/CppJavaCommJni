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
#include <cstdint>

/**
 * send and receive using java programs.
 */
class JavaComm {
private:
	jclass senderClass;
	jmethodID sendMethod;

	jclass listenerClass;

	std::function<void(uint8_t const *, unsigned)> receiveHandler;

public:
	JavaComm(JNIEnv * envPtr) {
		senderClass = envPtr->FindClass("javaComm/Sender");
		sendMethod = envPtr->GetStaticMethodID(senderClass, "send", "([B)I");

		listenerClass = envPtr->FindClass("javaComm/Listener");
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

	void send(JNIEnv * envPtr, uint8_t const * bytes, unsigned len) {
		jbyteArray jba = envPtr->NewByteArray(len);
		envPtr->SetByteArrayRegion(jba, 0, len, (jbyte const *)bytes);

		auto k = envPtr->CallStaticIntMethod( senderClass, sendMethod, jba );
		
		envPtr->DeleteLocalRef(jba);

		if (!k)
			throw std::runtime_error("java send failed.");
	}

	void handleReceive(uint8_t const * bytes, unsigned length) {
		if (receiveHandler)
			receiveHandler(bytes, length);
		else
			LOG_DEBUG(std::string("C++ received bytes: ") + std::to_string(length));
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

	uint64_t getJavaSendByteCnt(JNIEnv * envPtr) const {
		auto method = envPtr->GetStaticMethodID(senderClass, "getByteCnt", "()J");
		auto k = envPtr->CallStaticLongMethod( senderClass, method );
		return k;
	}

	int getJavaRecvCnt(JNIEnv * envPtr) const {
		auto method = envPtr->GetStaticMethodID(listenerClass, "getRecvCnt", "()I");
		auto k = envPtr->CallStaticIntMethod( listenerClass, method );
		return k;
	}

	uint64_t getJavaRecvByteCnt(JNIEnv * envPtr) const {
		auto method = envPtr->GetStaticMethodID(listenerClass, "getRecvByteCnt", "()J");
		auto k = envPtr->CallStaticLongMethod( listenerClass, method );
		return k;
	}


};

#endif /* JAVACOMM_H_ */
