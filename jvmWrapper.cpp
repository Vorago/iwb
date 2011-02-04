/* 
 * File:   jvmWrapper.cpp
 * Author: vorago
 * 
 * Created on February 3, 2011, 10:43 PM
 */

#include "include/jvmWrapper.hpp"

namespace iwb {

    JNIEnv* jvmWrapper::create_vm() {
        JavaVM *jvm;
        JNIEnv *env;
        JavaVMInitArgs vm_args;
        JavaVMOption options;
        
        options.optionString = "-Djava.class.path=/usr/lib/jvm/java-6-openjdk/";
        vm_args.version = JNI_VERSION_1_6;
        vm_args.nOptions = 1;
        vm_args.options = &options;
        vm_args.ignoreUnrecognized = 0;

        int ret = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
        return env;
    }
}


