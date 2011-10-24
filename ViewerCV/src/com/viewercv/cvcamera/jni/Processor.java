/*
 * Chris McClanahan
 *
 * modified android-opencv example
 */
package com.viewercv.cvcamera.jni;

import com.opencv.jni.image_pool;// import the image_pool interface for playing nice with
// android-opencv

/** Processor - for processing images that are stored in an image pool
*/
public class Processor {
    private long swigCPtr;
    protected boolean swigCMemOwn;

    public Processor(long cPtr, boolean cMemoryOwn) {
        swigCMemOwn = cMemoryOwn;
        swigCPtr = cPtr;
    }

    public static long getCPtr(Processor obj) {
        return (obj == null) ? 0 : obj.swigCPtr;
    }

    protected void finalize() {
        delete();
    }

    public synchronized void delete() {
        if (swigCPtr != 0) {
            if (swigCMemOwn) {
                swigCMemOwn = false;
                cvcameraJNI.delete_Processor(swigCPtr);
            }
            swigCPtr = 0;
        }
    }

    public Processor() {
        this(cvcameraJNI.new_Processor(), true);
    }

    public void detectAndDrawFeatures(int idx, image_pool pool) {
        cvcameraJNI.Processor_detectAndDrawFeatures(swigCPtr, this, idx, image_pool.getCPtr(pool), pool);
    }

    public void drawText(int idx, image_pool pool, String text) {
        cvcameraJNI.Processor_drawText(swigCPtr, this, idx, image_pool.getCPtr(pool), pool, text);
    }

    public void detectAndDrawContours(int idx, image_pool pool) {
        cvcameraJNI.Processor_detectAndDrawContours(swigCPtr, this, idx, image_pool.getCPtr(pool), pool);
    }

    public void runSobel(int idx, image_pool pool) {
        cvcameraJNI.Processor_runSobel(swigCPtr, this, idx, image_pool.getCPtr(pool), pool);
    }

    public void runHistEq(int idx, image_pool pool) {
        cvcameraJNI.Processor_runHistEq(swigCPtr, this, idx, image_pool.getCPtr(pool), pool);
    }

    public void setMode(int mode) {
        cvcameraJNI.Processor_setMode(swigCPtr, this, mode);
    }

    public void runHDR(int idx, image_pool pool, int skip) {
        cvcameraJNI.Processor_runHDR(swigCPtr, this, idx, image_pool.getCPtr(pool), pool, skip);
    }

    public void snapPic() {
        cvcameraJNI.Processor_snapPic(swigCPtr, this);
    }

    public void runNEON(int idx, image_pool pool, int var) {
        cvcameraJNI.Processor_runNEON(swigCPtr, this, idx, image_pool.getCPtr(pool), pool, var);
    }

    public void runVivid(int idx, image_pool pool, int var) {
        cvcameraJNI.Processor_runVivid(swigCPtr, this, idx, image_pool.getCPtr(pool), pool, var);
    }
}
