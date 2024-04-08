/*
 * Jvm.h
 *
 *  Created on: Apr 7, 2024
 *      Author: mikejyg
 */

#ifndef JVM_H_
#define JVM_H_

#include <jni.h>
#include <global.h>

class Jvm {
private:
	JavaVM *jvmPtr=nullptr;       /* denotes a Java VM */
	JNIEnv *mainThreadEnvPtr=nullptr;       /* pointer to native method interface */

	std::string vmOpt;

	JavaVMOption* options = nullptr;

	void initOptionsArgs(JavaVMInitArgs & vm_args) {
		options = new JavaVMOption[1];

		options[0].optionString = const_cast<char*>(vmOpt.c_str());	// work around for jni imperfection.

		vm_args.options = options;
		vm_args.nOptions = 1;
	}

public:
	Jvm() {
		vmOpt="-Djava.class.path=../java/bin";
	}

	~Jvm() {
		if (options)
			delete options;

		if (jvmPtr) {
			destroyJvm();
		}
	}

	void init() {
		JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */

		vm_args.version = JNI_VERSION_1_6;

		initOptionsArgs(vm_args);

		vm_args.ignoreUnrecognized = false;

		/* load and initialize a Java VM, return a JNI interface
		 * pointer in env */
		JNI_CreateJavaVM(&jvmPtr, (void**)&mainThreadEnvPtr, &vm_args);
		LOG_DEBUG("VM created.");

	}

	void destroyJvm() {
		LOG_DEBUG("destoryJavaVM()...");
		jvmPtr->DestroyJavaVM();
		jvmPtr=nullptr;
	}

	JNIEnv * getMainThreadJniEnvPtr() {
		return mainThreadEnvPtr;
	}

	JavaVM & getJvm() {
		return *jvmPtr;
	}

	JNIEnv * attachCurrentThread() {
		JNIEnv * envPtr;
		auto k = jvmPtr->AttachCurrentThread( (void**) & envPtr, nullptr );
		if (k!=JNI_OK)
			throw std::runtime_error("AttachCurrentThread() failed.");

		return envPtr;
	}

	void detachCurrentThread() {
		auto k = jvmPtr->DetachCurrentThread();
		if (k!=JNI_OK)
			throw std::runtime_error("DetachCurrentThread() failed.");
	}

};

#endif /* JVM_H_ */
