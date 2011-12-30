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


package com.android.viewer;

import com.android.viewer.imageprocessing.IPUtility;
import com.android.viewer.imageprocessing.ProcHistogram;
import com.android.viewer.imageprocessing.ProcessingFactory;
import com.android.viewer.imageprocessing.ProcSobel;

import android.graphics.Bitmap;
import android.hardware.Camera.Size;
import android.os.Message;
import android.util.Log;

public class ImageProcessingThread extends Thread {

    private static Size mImageSize;
    private static byte[] mData;
    private static boolean mDoProcess = true;
    private static boolean mSendMsg = true;
    private static final String TAG = "ImProcThread";
    private static int[] mRGB = null;

    public ImageProcessingThread(Size imageSize, byte[] data, boolean process) {
        mImageSize = imageSize;
        mData = data;
        mDoProcess = process;
    }

    /*
     * Main Image Processing Loop
     */
    @Override
    public void run() {
        if (Singleton.getApplicationState() != Singleton.STATE_INIT_APP) {

            if (mDoProcess == false) {
                // turn off processing, only once
                if (mSendMsg == true) {
                    mSendMsg = false;
                    passThruImg();
                }
                return;
            } else {
                mSendMsg = true;
            }

            /*
             * Start Timer
             */
            Singleton.resetStateTimer();

            /*
             * !!! ALL IMAGE PROCESSING !!!
             */
            processImage();

            /*
             * End Timer
             */
            Log.w(TAG, " Average  FPS:  " + Singleton.getFPS(1));

        }
    }

    /*
     *
     */
    private void processImage() {

        // convert
        int[] pixels = getRGBPixels();
        int width = mImageSize.width;
        int height = mImageSize.height;

        // display
        if (Singleton.getApplicationState() == Singleton.STATE_DEFAULT) {

            // nothing, show rgb

        } else if (Singleton.getApplicationState() == Singleton.STATE_PROC_1) {

            // b&w
            pixels = IPUtility.cvtRGB2GRAY(pixels, width, height);
            // histogram
            ProcHistogram hs = ProcessingFactory.getHistogram();
            int[] hist = hs.getImageHistogram(pixels);
            // overlay
            pixels = hs.overlayHistogram(pixels, hist, width, height);

        } else if (Singleton.getApplicationState() == Singleton.STATE_PROC_2) {

            // b&w
            pixels = IPUtility.cvtRGB2GRAY(pixels, width, height);
            // histogram
            ProcHistogram qs = ProcessingFactory.getHistogram();
            int[] hist = qs.getImageHistogram(pixels);
            // histogram equalization
            pixels = qs.histogramEq(pixels, hist, width, height);

        } else if (Singleton.getApplicationState() == Singleton.STATE_PROC_3) {

            // b&w
            pixels = IPUtility.cvtRGB2GRAY(pixels, width, height);
            // sobel
            ProcSobel sb = ProcessingFactory.getSobel();
            sb.init(width, height);
            pixels = sb.process_native(pixels, false);

        } else if (Singleton.getApplicationState() == Singleton.STATE_PROC_4) {

            // b&w
            pixels = IPUtility.cvtRGB2GRAY(pixels, width, height);
            // sobel
            ProcSobel sb = ProcessingFactory.getSobel();
            sb.init(width, height);
            pixels = sb.process_native(pixels, true);

        }

        // display
        updateImageView(pixels, width, height);

        // take out the trash
        pixels = null;
        System.gc();
    }

    private int[] getRGBPixels() {
        if (mRGB == null) {
            mRGB = new int[mImageSize.width * mImageSize.height];
            Log.d(TAG, "INIT getRGBPixels");
        }
        IPUtility.decodeYUV420SP(mData, mImageSize.width, mImageSize.height, mRGB);
        return mRGB;
    }

    /*
     * Create and update global bitmap
     */
    private void updateImageView(int[] pixels, int width, int height) {
        // create and set
        IPUtility.renderBitmapInPlace(pixels, width, height);
        // meg live update refresh
        Message msg = CameraWrapper.mHandler.obtainMessage();
        msg.arg1 = CameraActivity.DRAW_IMAGE_PROCESSING;
        CameraWrapper.mHandler.dispatchMessage(msg);
    }

    /*
     *
     */
    private void passThruImg() {
        mData = mData.clone();
        int[] pixels = getRGBPixels();
        Bitmap b = IPUtility.renderBitmap(pixels, mImageSize.width, mImageSize.height);
        Singleton.updateImageView = b;
        Singleton.setApplicationState(Singleton.STATE_DEFAULT);
        Message msg = CameraWrapper.mHandler.obtainMessage();
        msg.arg1 = CameraActivity.DRAW_IMAGE_PROCESSING;
        CameraWrapper.mHandler.dispatchMessage(msg);
    }

}
