/*
 * Chris McClanahan
 *
 *  modified from opencv example
 */

#define SWIGJAVA


#ifdef __cplusplus
/* SwigValueWrapper is described in swig.swg */
template<typename T> class SwigValueWrapper {
    struct SwigMovePointer {
        T* ptr;
        SwigMovePointer(T* p) : ptr(p) { }
        ~SwigMovePointer() { delete ptr; }
        SwigMovePointer& operator=(SwigMovePointer& rhs) { T* oldptr = ptr; ptr = 0; delete oldptr; ptr = rhs.ptr; rhs.ptr = 0; return *this; }
    } pointer;
    SwigValueWrapper& operator=(const SwigValueWrapper<T>& rhs);
    SwigValueWrapper(const SwigValueWrapper<T>& rhs);
public:
    SwigValueWrapper() : pointer(0) { }
    SwigValueWrapper& operator=(const T& t) { SwigMovePointer tmp(new T(t)); pointer = tmp; return *this; }
    operator T&() const { return *pointer.ptr; }
    T* operator&() { return pointer.ptr; }
};

template <typename T> T SwigValueInit() {
    return T();
}
#endif

/* -----------------------------------------------------------------------------
 *  This section contains generic SWIG labels for method/variable
 *  declarations/attributes, and other compiler dependent labels.
 * ----------------------------------------------------------------------------- */

/* template workaround for compilers that cannot correctly implement the C++ standard */
#ifndef SWIGTEMPLATEDISAMBIGUATOR
# if defined(__SUNPRO_CC) && (__SUNPRO_CC <= 0x560)
#  define SWIGTEMPLATEDISAMBIGUATOR template
# elif defined(__HP_aCC)
/* Needed even with `aCC -AA' when `aCC -V' reports HP ANSI C++ B3910B A.03.55 */
/* If we find a maximum version that requires this, the test would be __HP_aCC <= 35500 for A.03.55 */
#  define SWIGTEMPLATEDISAMBIGUATOR template
# else
#  define SWIGTEMPLATEDISAMBIGUATOR
# endif
#endif

/* inline attribute */
#ifndef SWIGINLINE
# if defined(__cplusplus) || (defined(__GNUC__) && !defined(__STRICT_ANSI__))
#   define SWIGINLINE inline
# else
#   define SWIGINLINE
# endif
#endif

/* attribute recognised by some compilers to avoid 'unused' warnings */
#ifndef SWIGUNUSED
# if defined(__GNUC__)
#   if !(defined(__cplusplus)) || (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4))
#     define SWIGUNUSED __attribute__ ((__unused__))
#   else
#     define SWIGUNUSED
#   endif
# elif defined(__ICC)
#   define SWIGUNUSED __attribute__ ((__unused__))
# else
#   define SWIGUNUSED
# endif
#endif

#ifndef SWIG_MSC_UNSUPPRESS_4505
# if defined(_MSC_VER)
#   pragma warning(disable : 4505) /* unreferenced local function has been removed */
# endif
#endif

#ifndef SWIGUNUSEDPARM
# ifdef __cplusplus
#   define SWIGUNUSEDPARM(p)
# else
#   define SWIGUNUSEDPARM(p) p SWIGUNUSED
# endif
#endif

/* internal SWIG method */
#ifndef SWIGINTERN
# define SWIGINTERN static SWIGUNUSED
#endif

/* internal inline SWIG method */
#ifndef SWIGINTERNINLINE
# define SWIGINTERNINLINE SWIGINTERN SWIGINLINE
#endif

/* exporting methods */
#if (__GNUC__ >= 4) || (__GNUC__ == 3 && __GNUC_MINOR__ >= 4)
#  ifndef GCC_HASCLASSVISIBILITY
#    define GCC_HASCLASSVISIBILITY
#  endif
#endif

#ifndef SWIGEXPORT
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   if defined(STATIC_LINKED)
#     define SWIGEXPORT
#   else
#     define SWIGEXPORT __declspec(dllexport)
#   endif
# else
#   if defined(__GNUC__) && defined(GCC_HASCLASSVISIBILITY)
#     define SWIGEXPORT __attribute__ ((visibility("default")))
#   else
#     define SWIGEXPORT
#   endif
# endif
#endif

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif
#endif

/* Deal with Microsoft's attempt at deprecating C standard runtime functions */
#if !defined(SWIG_NO_CRT_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_CRT_SECURE_NO_DEPRECATE)
# define _CRT_SECURE_NO_DEPRECATE
#endif

/* Deal with Microsoft's attempt at deprecating methods in the standard C++ library */
#if !defined(SWIG_NO_SCL_SECURE_NO_DEPRECATE) && defined(_MSC_VER) && !defined(_SCL_SECURE_NO_DEPRECATE)
# define _SCL_SECURE_NO_DEPRECATE
#endif



/* Fix for jlong on some versions of gcc on Windows */
#if defined(__GNUC__) && !defined(__INTEL_COMPILER)
typedef long long __int64;
#endif

/* Fix for jlong on 64-bit x86 Solaris */
#if defined(__x86_64)
# ifdef _LP64
#   undef _LP64
# endif
#endif

#include <jni.h>
#include <stdlib.h>
#include <string.h>


/* Support for throwing Java exceptions */
typedef enum {
    SWIG_JavaOutOfMemoryError = 1,
    SWIG_JavaIOException,
    SWIG_JavaRuntimeException,
    SWIG_JavaIndexOutOfBoundsException,
    SWIG_JavaArithmeticException,
    SWIG_JavaIllegalArgumentException,
    SWIG_JavaNullPointerException,
    SWIG_JavaDirectorPureVirtual,
    SWIG_JavaUnknownError
} SWIG_JavaExceptionCodes;

typedef struct {
    SWIG_JavaExceptionCodes code;
    const char* java_exception;
} SWIG_JavaExceptions_t;


static void SWIGUNUSED SWIG_JavaThrowException(JNIEnv* jenv, SWIG_JavaExceptionCodes code, const char* msg) {
    jclass excep;
    static const SWIG_JavaExceptions_t java_exceptions[] = {
        { SWIG_JavaOutOfMemoryError, "java/lang/OutOfMemoryError" },
        { SWIG_JavaIOException, "java/io/IOException" },
        { SWIG_JavaRuntimeException, "java/lang/RuntimeException" },
        { SWIG_JavaIndexOutOfBoundsException, "java/lang/IndexOutOfBoundsException" },
        { SWIG_JavaArithmeticException, "java/lang/ArithmeticException" },
        { SWIG_JavaIllegalArgumentException, "java/lang/IllegalArgumentException" },
        { SWIG_JavaNullPointerException, "java/lang/NullPointerException" },
        { SWIG_JavaDirectorPureVirtual, "java/lang/RuntimeException" },
        { SWIG_JavaUnknownError,  "java/lang/UnknownError" },
        { (SWIG_JavaExceptionCodes)0,  "java/lang/UnknownError" }
    };
    const SWIG_JavaExceptions_t* except_ptr = java_exceptions;

    while (except_ptr->code != code && except_ptr->code) {
        except_ptr++;
    }

    jenv->ExceptionClear();
    excep = jenv->FindClass(except_ptr->java_exception);
    if (excep) {
        jenv->ThrowNew(excep, msg);
    }
}


/* Contract support */

#define SWIG_contract_assert(nullreturn, expr, msg) if (!(expr)) {SWIG_JavaThrowException(jenv, SWIG_JavaIllegalArgumentException, msg); return nullreturn; } else


#include "Processor.h"
#include "image_pool.h"
using namespace cv;


#ifdef __cplusplus
extern "C" {
#endif

    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1FAST_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_FAST;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1SQUARES_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_SQUARES;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1SOBEL_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_SOBEL;
        jresult = (jint)result;
        return jresult;
    }

    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1HISTEQ_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_HISTEQ;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1HDR_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_HDR;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1NEON_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_NEON;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jint JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_DETECT_1Vivid_1get(JNIEnv* jenv, jclass jcls) {
        jint jresult = 0 ;
        int result;

        (void)jenv;
        (void)jcls;
        result = (int) DETECT_VIVID;
        jresult = (jint)result;
        return jresult;
    }


    SWIGEXPORT jlong JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_new_1Processor(JNIEnv* jenv, jclass jcls) {
        jlong jresult = 0 ;
        Processor* result = 0 ;

        (void)jenv;
        (void)jcls;
        result = (Processor*)new Processor();
        *(Processor**)&jresult = result;
        return jresult;
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_delete_1Processor(JNIEnv* jenv, jclass jcls, jlong jarg1) {
        Processor* arg1 = (Processor*) 0 ;

        (void)jenv;
        (void)jcls;
        arg1 = *(Processor**)&jarg1;
        delete arg1;
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1detectAndDrawFeatures(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        (arg1)->detectAndDrawFeatures(arg2, arg3);
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1drawText(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_, jstring jarg4) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;
        char* arg4 = (char*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        arg4 = 0;
        if (jarg4) {
            arg4 = (char*)jenv->GetStringUTFChars(jarg4, 0);
            if (!arg4) { return ; }
        }
        (arg1)->drawText(arg2, arg3, (char const*)arg4);
        if (arg4) { jenv->ReleaseStringUTFChars(jarg4, (const char*)arg4); }
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1detectAndDrawContours(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        (arg1)->detectAndDrawContours(arg2, arg3);
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1runSobel(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        (arg1)->runSobel(arg2, arg3);
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1runHistEq(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        (arg1)->runHistEq(arg2, arg3);
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1setMode(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        (arg1)->setMode(arg2);
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1snapPic(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_) {
        Processor* arg1 = (Processor*) 0 ;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        arg1 = *(Processor**)&jarg1;
        (arg1)->snapPic();
    }


    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1runHDR(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_, jint jarg4) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;
        int arg4;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        arg4 = (int)jarg4;
        (arg1)->runHDR(arg2, arg3, arg4);
    }

    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1runNEON(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_, jint jarg4) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;
        int arg4;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        arg4 = (int)jarg4;
        (arg1)->runNEON(arg2, arg3, arg4);
    }

    SWIGEXPORT void JNICALL Java_com_viewercv_cvcamera_jni_cvcameraJNI_Processor_1runVivid(JNIEnv* jenv, jclass jcls, jlong jarg1, jobject jarg1_, jint jarg2, jlong jarg3, jobject jarg3_, jint jarg4) {
        Processor* arg1 = (Processor*) 0 ;
        int arg2 ;
        image_pool* arg3 = (image_pool*) 0 ;
        int arg4;

        (void)jenv;
        (void)jcls;
        (void)jarg1_;
        (void)jarg3_;
        arg1 = *(Processor**)&jarg1;
        arg2 = (int)jarg2;
        arg3 = *(image_pool**)&jarg3;
        arg4 = (int)jarg4;
        (arg1)->runVivid(arg2, arg3, arg4);
    }

#ifdef __cplusplus
}
#endif

