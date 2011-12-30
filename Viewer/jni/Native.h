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

#include <jni.h>

/* Header for class com_android_viewer_imageprocessing_Native */

#ifndef _Included_com_android_viewer_imageprocessing_Native
#define _Included_com_android_viewer_imageprocessing_Native

#ifdef __cplusplus
extern "C" {
#endif



    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    decodeYUV420SP_native
     * Signature: ([BII[I)[I
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_decodeYUV420SP_1native
    (JNIEnv*, jclass, jbyteArray, jint, jint, jintArray);


    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    cvtRGB2GRAY_native
     * Signature:
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_cvtRGB2GRAY_1native
    (JNIEnv*, jclass, jintArray, jint, jint, jintArray);


    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    createHistogram_native
     * Signature:
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_createHistogram_1native
    (JNIEnv*, jclass, jintArray, jint, jintArray);


    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    overlayHistogram_native
     * Signature:
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_overlayHistogram_1native
    (JNIEnv*, jclass, jintArray, jint, jint, jintArray);

    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    sobel_native
     * Signature:
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_sobel_1native
    (JNIEnv*, jclass, jintArray, jint, jint, jintArray, jint);

    /*
     * Class:     com_android_viewer_imageprocessing_Native
     * Method:    histogramEq_native
     * Signature:
     */
    JNIEXPORT jint JNICALL Java_com_android_viewer_imageprocessing_Native_histogramEq_1native
    (JNIEnv*, jclass, jintArray, jintArray, jint, jint);



#ifdef __cplusplus
}
#endif

#endif
