/* 
 * File:   jvmWrapper.hpp
 * Author: vorago
 *
 * Created on February 3, 2011, 10:43 PM
 */

#ifndef JVMWRAPPER_HPP
#define	JVMWRAPPER_HPP

#include <jni.h>

namespace iwb {

    class jvmWrapper {
    public:
        /**
         * Function for Java Virtual Machine loading
         *
         * @return pointer to JNI Environment used for working with VM
         */
        static JNIEnv* create_vm();
    };
}
#endif	/* JVMWRAPPER_HPP */

