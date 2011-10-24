/*
 * Chris McClanahan
 *
 * modified android-opencv example
 */
package com.viewercv.cvcamera.jni;

import com.opencv.jni.*; //import the android-opencv jni wrappers

class cvcameraJNI {

    static {
        try {
            //load the cvcamera library, make sure that libcvcamera.so is in your <project>/libs/armeabi directory
            System.loadLibrary("android-opencv");
            System.loadLibrary("cvcamera");
        } catch (UnsatisfiedLinkError e) {
            //badness
            throw e;
        }
    }

    public final static native int DETECT_FAST_get();
    public final static native int DETECT_SQUARES_get();
    public final static native int DETECT_SOBEL_get();
    public final static native int DETECT_HISTEQ_get();
    public final static native int DETECT_HDR_get();

    public final static native long new_Processor();
    public final static native void delete_Processor(long jarg1);

    public final static native void Processor_detectAndDrawFeatures(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_);
    public final static native void Processor_drawText(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_, String jarg4);
    public final static native void Processor_detectAndDrawContours(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_);
    public final static native void Processor_runSobel(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_);
    public final static native void Processor_runHistEq(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_);

    public final static native void Processor_setMode(long jarg1, Processor jarg1_, int jarg2);
    public final static native void Processor_snapPic(long jarg1, Processor jarg1_);
    public final static native void Processor_runHDR(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_, int jarg4);
    public final static native void Processor_runNEON(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_, int jarg4);
    public final static native void Processor_runVivid(long jarg1, Processor jarg1_, int jarg2, long jarg3, image_pool jarg3_, int jarg4);

}
