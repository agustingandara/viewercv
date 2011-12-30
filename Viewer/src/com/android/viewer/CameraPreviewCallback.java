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

/*
  Used the project MobileEye as a reference for
  getting the camera data and overlaying the camera bitmap.
*/

package com.android.viewer;

import android.hardware.Camera;
import android.hardware.Camera.PreviewCallback;
import android.hardware.Camera.Size;

public class CameraPreviewCallback implements PreviewCallback {
    private Size mPreviewSize;
    private ImageProcessingThread mProcessingThread;
    private boolean mProcessing = true;

    public CameraPreviewCallback(Size previewSize) {
        mPreviewSize = previewSize;
    }

    public void onPreviewFrame(byte[] data, Camera camera) {
        if (mProcessingThread == null || mProcessingThread.isAlive() == false) {
            mProcessingThread = new ImageProcessingThread(mPreviewSize, data, mProcessing);
            mProcessingThread.start();
        }
    }

    public void doProcessing() {
        mProcessing = !mProcessing;
    }

}
