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

import com.android.viewer.R;
import com.android.viewer.imageprocessing.IOUtility;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.WindowManager;
import android.view.SurfaceHolder.Callback;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class CameraActivity extends Activity implements Callback {

    public static final int START_AUTO_FOCUS = 0;
    public static final int DRAW_IMAGE_PROCESSING = 1;
    public static final int APPLICATION_STATE_CHANGED = 2;
    public static final int SHOW_TOAST = 3;
    public static final int PICTURE_TAKEN = 4;
    public static final int AUTO_FOCUS_SUCCESSFUL = 5;
    public static final int AUTO_FOCUS_UNSUCCESSFUL = 6;

    private static final String TAG = "CamActivity";
    public static final String FIND_OBJECT_DATA_KEY = "FindObjectRequestKey";
    public static final String TOAST_STRING_KEY = "ShowToastStringKey";

    private SurfaceView mSurfaceView;
    private ImageView mImageProcessedSurfaceView;
    private boolean mStartPreviewFail;
    private CameraWrapper mCamera;
    private SurfaceHolder mSurfaceHolder;
    private Handler mHandler;
    private Button mProcessBtn;
    private Button mProc1Btn;
    private Button mProc2Btn;
    private Button mProc3Btn;
    private Button mProc4Btn;
    private TextView mStatusTextView;
    private boolean mPictureAfterFocus = false;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.camera);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        initActivity();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    }

    @Override
    public void onStart() {
        super.onStart();
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        Log.v(TAG, "Viewer - onStart");
    }

    @Override
    public void onResume() {
        super.onResume();
        Log.v(TAG, "Viewer - onResume");
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        // Start the preview if it is not started.
        if ((mCamera.isPreviewing() == false) && (mStartPreviewFail == false)) {
            try {
                mCamera.startPreview(mSurfaceHolder);
            } catch (CameraHardwareException e) {
                // Show Error and finish
                return;
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "Viewer - onPause");
        mCamera.stopPreview();
        // Close the camera now because other activities may need to use it.
        mCamera.closeCamera();
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "Viewer - onStop");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "Viewer - onDestroy");
    }

    private void updateApplicationState() {
        Singleton.setApplicationState(Singleton.STATE_DEFAULT);
    }

    private void initActivity() {

        mStatusTextView = (TextView) findViewById(R.id.StatusTextView);
        mStatusTextView.setText("Initialising...");
        mStatusTextView.setOnClickListener(new OnClickListener() {
            public void onClick(View view) {
                mStatusTextView.setText("Saving...");
                mCamera.startAutoFocus();
                mPictureAfterFocus = true;
            }
        });

        Singleton.setApplicationState(Singleton.STATE_INIT_APP);
        initHandler();
        updateApplicationState();

        mProcessBtn = (Button) findViewById(R.id.ProcessBtn);
        mProcessBtn.setOnClickListener(new OnClickListener() {
            public void onClick(View view) {
                String txt = (!mCamera.isProcessing()) ? "Processing On" : "Processing Off";
                mStatusTextView.setText(txt);
                mCamera.startAutoFocus();
                mCamera.doProcessingLive();
            }
        });

        mProc1Btn = (Button) findViewById(R.id.Type1Btn);
        mProc1Btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                String txt = "Type 1";
                mStatusTextView.setText(txt);
                mCamera.startAutoFocus();
                Singleton.setApplicationState(Singleton.STATE_PROC_1);
            }
        });

        mProc2Btn = (Button) findViewById(R.id.Type2Btn);
        mProc2Btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                String txt = "Type 2";
                mStatusTextView.setText(txt);
                mCamera.startAutoFocus();
                Singleton.setApplicationState(Singleton.STATE_PROC_2);
            }
        });

        mProc3Btn = (Button) findViewById(R.id.Type3Btn);
        mProc3Btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                String txt = "Type 3";
                mStatusTextView.setText(txt);
                mCamera.startAutoFocus();
                Singleton.setApplicationState(Singleton.STATE_PROC_3);
            }
        });

        mProc4Btn = (Button) findViewById(R.id.Type4Btn);
        mProc4Btn.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                String txt = "Type 4";
                mStatusTextView.setText(txt);
                mCamera.startAutoFocus();
                Singleton.setApplicationState(Singleton.STATE_PROC_4);
            }
        });

        mCamera = new CameraWrapper(mHandler);
        mSurfaceView = (SurfaceView) findViewById(R.id.CameraSurfaceView);
        mImageProcessedSurfaceView = (ImageView) findViewById(R.id.ImageProcessedSurfaceView);

        Thread startPreviewThread = new Thread(new Runnable() {
            public void run() {
                try {
                    mStartPreviewFail = false;
                    mCamera.startPreview(mSurfaceHolder);
                } catch (CameraHardwareException e) {
                    mStartPreviewFail = true;
                }
            }
        });
        startPreviewThread.start();

        SurfaceHolder surfaceHolder = mSurfaceView.getHolder();
        surfaceHolder.addCallback(this);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        try {
            startPreviewThread.join();
            if (mStartPreviewFail == true) {
                Log.e(TAG, "Viewer - ERROR: Start Preview of the camera failed");
                finish();
                return;
            }
        } catch (InterruptedException ex) {
            ex.printStackTrace();
        }
    }

    /**
     * The SurfaceView Callback methods
     */
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        Log.v(TAG, "Viewer - surfaceChanged");
        if (mCamera.isNull() == true) {
            return;
        }
        // Make sure we have a surface in the holder before proceeding.
        if (holder.getSurface() == null) {
            Log.v(TAG, "Viewer - Camera surfaceChanged holder.getSurface() == null");
            return;
        }
        mSurfaceHolder = holder;
        if (holder.isCreating() == true) {
            mCamera.setPreviewDisplay(mSurfaceHolder);
        }
        mCamera.startAutoFocus();
        mStatusTextView.setText("Processing Off");
    }

    public void surfaceCreated(SurfaceHolder holder) {
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.v(TAG, "Viewer - surfaceDestroyed");
        mCamera.stopPreview();
        mSurfaceHolder = null;
    }

    /**
     * Set up the Activity Handler
     */
    private void initHandler() {
        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                if (msg.arg1 == START_AUTO_FOCUS) {
                    // take picture ?
                    if (mPictureAfterFocus == true) {
                        mCamera.doProcessingPic();
                        mPictureAfterFocus = false;
                    }
                    // Previous Auto Focus
                    if (msg.arg2 == AUTO_FOCUS_SUCCESSFUL) {
                    } else {
                    }
                    // Re-start Auto Focus
                    if (mCamera.isNull() == false && mCamera.isPreviewing() == true) {
                        // mCamera.startAutoFocus();
                    }
                } else if (msg.arg1 == DRAW_IMAGE_PROCESSING) {
                    mHandler.post(new Runnable() {
                        public void run() {
                            // live view
                            mImageProcessedSurfaceView.setImageBitmap(Singleton.updateImageView);
                            String fps = "FPS: " + String.valueOf(Singleton.getFPS(1));
                            mStatusTextView.setText(fps);
                        }
                    });
                } else if (msg.arg1 == APPLICATION_STATE_CHANGED) {
                    if (Singleton.getApplicationState() == Singleton.STATE_DEFAULT) {
                        mHandler.post(new Runnable() {
                            public void run() {
                                // status
                                mStatusTextView.setText("Processing");
                            }
                        });
                    }
                } else if (msg.arg1 == SHOW_TOAST) {
                    final String s = msg.getData().getString(TOAST_STRING_KEY);
                    mHandler.post(new Runnable() {
                        public void run() {
                            // msg
                            Toast t = Toast.makeText(getApplicationContext(), s, Toast.LENGTH_LONG);
                            t.show();
                        }
                    });
                } else if (msg.arg1 == PICTURE_TAKEN) {
                    mHandler.post(new Runnable() {
                        public void run() {
                            // save image
                            mStatusTextView.setText("Saving...");
                            String fname = Float.toString((System.nanoTime() / 1000000000.0f));
                            IOUtility.saveImageToSDCard(Singleton.updateImageView, fname);
                            mCamera.freePictureCallback();
                            mStatusTextView.setText("Saved.");
                        }
                    });
                }
            }
        }; // handler
    }
}
