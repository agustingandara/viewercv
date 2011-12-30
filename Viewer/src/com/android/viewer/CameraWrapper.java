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

import java.util.List;

import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Handler;
import android.util.Log;
import android.view.SurfaceHolder;

public class CameraWrapper {
    private Camera mCamera;
    private boolean mPreviewing;
    private boolean mProcessing;
    private static final String TAG = "CamWrapper";

    private CameraErrorCallback mErrorCallback;
    private CameraPreviewCallback mPreviewCallback;
    private CameraAutoFocusCallback mAutoFocusCallback;
    private CameraPictureCallback mPictureCallback;

    public static Handler mHandler;

    public CameraWrapper(Handler h) {
        mPreviewing = false;
        mProcessing = true;
        mHandler = h;
    }

    public void startPreview(SurfaceHolder holder) throws CameraHardwareException {
        Log.v(TAG, "Camera - startPreview");

        ensureCameraDevice();

        if (mPreviewing == true) {
            mCamera.stopPreview();
        }

        setPreviewDisplay(holder);

        Size previewSize = setCameraParameters();

        mErrorCallback = new CameraErrorCallback();
        mCamera.setErrorCallback(mErrorCallback);

        mPreviewCallback = new CameraPreviewCallback(previewSize);
        mCamera.setPreviewCallback(mPreviewCallback);

        mPictureCallback = new CameraPictureCallback();

        try {
            mCamera.startPreview();
        } catch (Throwable e) {
            throw new RuntimeException("Camera - startPreview failed", e);
        }

        mPreviewing = true;
    }

    private Size setCameraParameters() {
        Log.v(TAG, "Camera - setCameraParameters");

        Camera.Parameters params = mCamera.getParameters();
        params.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        params.setPreviewFormat(PixelFormat.YCbCr_420_SP);
        Camera.Size previewSize = params.getPreviewSize();

        List<Camera.Size> previewSizes = params.getSupportedPreviewSizes();
        int i = 0;
        for (Camera.Size cs : previewSizes) {
            Log.d(TAG, "Camera - supports:(" + (i++) + ") " + cs.width + "x" + cs.height);
        }
        if (previewSizes.size() > 1) {
            previewSize = previewSizes.get(1); // 320x240 on Droid X
            //previewSize = previewSizes.get(4); // 640x480 on Tegra 1 Dev Tablet
        } else {
            previewSize.width = 320;
            previewSize.height = 240;
        }
        params.setPreviewSize(previewSize.width, previewSize.height);
        Log.e(TAG, "Camera - Set Size: (" + previewSize.width + "x" + previewSize.height + ")");

        try {
            mCamera.setParameters(params);
        } catch (Exception e) {
            Log.e(TAG, "Camera - Exception - " + ((e == null) ? "null" : e.getMessage()));
        }

        return previewSize;
    }

    public void stopPreview() {
        Log.v(TAG, "Camera - stopPreview");

        if (mPreviewing == true) {
            mCamera.stopPreview();
        }

        mPreviewing = false;
    }

    public void setPreviewDisplay(SurfaceHolder holder) {
        Log.v(TAG, "Camera - setPreviewDisplay");

        try {
            mCamera.setPreviewDisplay(holder);
        } catch (Throwable e) {
            closeCamera();
            throw new RuntimeException("Camera - setPreviewDisplay failed", e);
        }
    }

    public void closeCamera() {
        Log.v(TAG, "Camera - closeCamera");

        if (mCamera != null) {
            mCamera.cancelAutoFocus();
            mCamera.setPreviewCallback(null);

            stopPreview();
            mCamera.release();

            mCamera = null;
            mPreviewing = false;
        }
    }

    private void ensureCameraDevice() throws CameraHardwareException {
        if (mCamera == null) {
            mCamera = Camera.open();
        }
    }

    public boolean isNull() {
        return (mCamera == null);
    }

    public boolean isPreviewing() {
        return mPreviewing;
    }

    public void startAutoFocus() {
        mAutoFocusCallback = new CameraAutoFocusCallback();
        mCamera.autoFocus(mAutoFocusCallback);
    }

    public void doProcessingPic() {
        mCamera.takePicture(null, null, mPictureCallback);
    }

    public void freePictureCallback() {
        mPictureCallback.makeFree();
    }

    public void doProcessingLive() {
        mProcessing = !mProcessing;
        mPreviewCallback.doProcessing();
    }

    public boolean isProcessing() {
        return mProcessing;
    }

    public void setProcessing(boolean processing) {
        this.mProcessing = processing;
    }

}
