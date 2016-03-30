#include <jni.h>

JNIEXPORT jstring JNICALL
Java_com_turbulent_spacerush_SpaceRunnerActivity_getMsgFromJni(JNIEnv *env, jobject instance) {

    // TODO

    return env->NewStringUTF("Huita");
}