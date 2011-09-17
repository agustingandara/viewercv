/*
   Copyright [2011] [Chris McClanahan]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <string.h>
#include <jni.h>
#include "Native.h"
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif

#include "native_core.h"


    jint
    Java_com_android_viewer_imageprocessing_Native_decodeYUV420SP_1native
    (JNIEnv* env, jclass jc, jbyteArray _yuv420sp, jint width, jint height, jintArray _rgb) {
        jbyte* yuv420sp = (*env)->GetByteArrayElements(env, _yuv420sp, NULL);
        jint*   rgb     = (*env)->GetIntArrayElements(env, _rgb,      NULL);
        cvtYUV2RGB(yuv420sp, width, height, rgb);
        (*env)->ReleaseByteArrayElements(env, (jbyteArray) _yuv420sp, yuv420sp, JNI_ABORT);
        (*env)->ReleaseIntArrayElements(env, (jintArray) _rgb,           rgb, 0);
        return (jint)0;
    }


    jint
    Java_com_android_viewer_imageprocessing_Native_cvtRGB2GRAY_1native
    (JNIEnv* env, jclass jc, jintArray input, jint width, jint height, jintArray output) {
        jint* rgb  = (*env)->GetIntArrayElements(env, input, NULL);
        //jint* gray = (*env)->GetIntArrayElements (env, output, NULL);
        jint* gray = rgb;
        cvtRGB2GRAY(rgb, width, height, gray);
        //(*env)->ReleaseIntArrayElements(env, (jintArray) input,   rgb, JNI_ABORT);
        (*env)->ReleaseIntArrayElements(env, (jintArray) output, gray, 0);
        return (jint)0;
    }



    jint
    Java_com_android_viewer_imageprocessing_Native_createHistogram_1native
    (JNIEnv* env, jclass jc, jintArray input, jint numpix, jintArray output) {
        jint* pixels  = (*env)->GetIntArrayElements(env, input, NULL);
        jint* bins = (*env)->GetIntArrayElements(env, output, NULL);
        createHistogram(pixels, numpix, bins);
        (*env)->ReleaseIntArrayElements(env, (jintArray) input,  pixels,  JNI_ABORT);
        (*env)->ReleaseIntArrayElements(env, (jintArray) output, bins, 0);
        return (jint)0;
    }


    jint
    Java_com_android_viewer_imageprocessing_Native_overlayHistogram_1native
    (JNIEnv* env, jclass jc, jintArray input, jint width, jint height, jintArray hist) {
        jint* pixels  = (*env)->GetIntArrayElements(env, input, NULL);
        jint* bins = (*env)->GetIntArrayElements(env, hist,  NULL);
        overlayHistogram(pixels, width, height, bins);
        (*env)->ReleaseIntArrayElements(env, (jintArray) input,  pixels, 0);
        (*env)->ReleaseIntArrayElements(env, (jintArray) hist,  bins, JNI_ABORT);
        return (jint)0;
    }

    jint
    Java_com_android_viewer_imageprocessing_Native_sobel_1native
    (JNIEnv* env, jclass jc, jintArray input, jint width, jint height, jintArray output, jint negative) {
        jint* inpx  = (*env)->GetIntArrayElements(env, input,  NULL);
        jint* outpx = (*env)->GetIntArrayElements(env, output, NULL);
        sobelFilter(inpx, width, height, outpx, negative);
        (*env)->ReleaseIntArrayElements(env, (jintArray) input,  inpx,  JNI_ABORT);
        (*env)->ReleaseIntArrayElements(env, (jintArray) output, outpx, 0);
        return (jint)0;
    }

    jint
    Java_com_android_viewer_imageprocessing_Native_histogramEq_1native
    (JNIEnv* env, jclass jc, jintArray image, jintArray histogram, jint width, jint height) {
        jint* img  = (*env)->GetIntArrayElements(env, image,     NULL);
        jint* hist = (*env)->GetIntArrayElements(env, histogram, NULL);
        histogramEq(img, hist, width, height);
        (*env)->ReleaseIntArrayElements(env, (jintArray) histogram, hist,  JNI_ABORT);
        (*env)->ReleaseIntArrayElements(env, (jintArray) image,      img, 0);
        return (jint)0;
    }



#ifdef __cplusplus
}
#endif
