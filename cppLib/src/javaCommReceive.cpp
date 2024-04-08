// Save as "HelloJNI.c"
#include <jni.h>		// JNI header provided by JDK
#include "javaComm_Listener.h"   // Generated
#include <iostream>

#include <JavaComm.h>

extern JavaComm * javaCommPtr;

extern "C"
/*
 * Class:     javaComm_Listener
 * Method:    javaToCpp
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_javaComm_Listener_javaToCpp(JNIEnv *jenv, jobject, jbyteArray jba) {
	auto * jbytes = jenv->GetByteArrayElements(jba, nullptr);
	auto size = jenv->GetArrayLength(jba);

	javaCommPtr->handleReceive( (uint8_t const *)jbytes, size );

	jenv->ReleaseByteArrayElements(jba, jbytes, JNI_ABORT);
}
