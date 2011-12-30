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

import java.io.ByteArrayOutputStream;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.os.Message;
import android.util.Log;

public class CameraPictureCallback implements PictureCallback {
    private static final float NEW_SCALED_WIDTH = 640;
    private boolean mBusy = false;
    private static final String TAG = "CamPicCallback";

    public void onPictureTaken(final byte[] data, Camera camera) {
        if (mBusy == false) {
            mBusy = true;
            Log.d(TAG, "Picture Taken");
            Thread t = new Thread(new Runnable() {

                public void run() {
                    Bitmap image = BitmapFactory.decodeByteArray(data, 0, data.length);
                    float scale = NEW_SCALED_WIDTH / image.getWidth();
                    Matrix m = new Matrix();
                    m.postScale(scale, scale);
                    m.postRotate(90);
                    image = Bitmap.createBitmap(image, 0, 0, image.getWidth(), image.getHeight(), m, true);
                    ByteArrayOutputStream outStream = new ByteArrayOutputStream();
                    image.compress(Bitmap.CompressFormat.JPEG, 100, outStream);
                    Message msg = CameraWrapper.mHandler.obtainMessage();
                    msg.arg1 = CameraActivity.PICTURE_TAKEN;
                    Bundle b = msg.getData();
                    b.putByteArray(CameraActivity.FIND_OBJECT_DATA_KEY, outStream.toByteArray());
                    msg.setData(b);
                    CameraWrapper.mHandler.dispatchMessage(msg);
                }

            });
            t.start();
        }

        camera.startPreview();
    }

    public void makeFree() {
        mBusy = false;
    }

}
