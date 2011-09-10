/*
 * Main activity
 *
 * Chris McClanahan
 *
 * modified android-opencv example
 */

package com.viewercv;

import java.io.File;
import java.util.LinkedList;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.opencv.camera.NativePreviewer;
import com.opencv.camera.NativeProcessor;
import com.opencv.camera.NativeProcessor.PoolCallback;
import com.opencv.jni.image_pool;
import com.opencv.opengl.GL2CameraViewer;
import com.viewercv.cvcamera.jni.Processor;

public class MainActivity extends Activity {

    private static final String TAG             = "ViewerCV-Main";

    private static final int    DIALOG_OPENING  = 1;
    private static final int    DIALOG_FAST     = 2;
    private static final int    DIALOG_SQUARES  = 3;
    private static final int    DIALOG_SOBEL    = 4;
    private static final int    DIALOG_MODE     = 5;
    private static final int    DIALOG_HDR      = 6;
    private static final int    DIALOG_ABOUT    = 7;
    private static final int    DIALOG_HISTEQ   = 8;
    private static final int    DIALOG_SETTINGS = 9;
    private static final int    DIALOG_SNAP     = 10;
    private static final int    DIALOG_NEON     = 11;

    private static int          CURR_VIEW       = 1;

    void toasts(int id) {

        switch (id) {
        case DIALOG_OPENING:
            Toast.makeText(this, "Click the Menu Button for options.", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_FAST:
            Toast.makeText(this, "Detecting and Displaying FAST features", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_SQUARES:
            Toast.makeText(this, "Detecting and Displaying Squares", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_SOBEL:
            Toast.makeText(this, "Running Sobel Filter", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_HISTEQ:
            Toast.makeText(this, "Running Histogram Equalization", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_MODE:
            String s = "Mode: " + proc_mode + "\n";
            if (CURR_VIEW == DIALOG_HDR) {
                if (proc_mode == 0) {
                    s = s + "raw HDRi";
                }
                if (proc_mode == 1) {
                    s = s + "contrast mapping";
                }
                if (proc_mode == 2) {
                    s = s + "contrast equalization";
                }
            }
            Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
            break;
        case DIALOG_HDR:
            Toast.makeText(this, "Running in HDR Mode", Toast.LENGTH_SHORT).show();
            break;
        case DIALOG_SETTINGS:
            selectResDiv();
            Toast.makeText(this, "Please restart ViewerCV to apply changes", Toast.LENGTH_LONG).show();
            break;
        case DIALOG_ABOUT:
            Toast.makeText(this, R.string.about_viewercv, Toast.LENGTH_SHORT).show();
            break;
        case DIALOG_SNAP:
            Toast.makeText(this, "Snap!", Toast.LENGTH_SHORT).show();
            break;
        case DIALOG_NEON:
            Toast.makeText(this, "Running NEON", Toast.LENGTH_SHORT).show();
            break;
        default:
            break;
        }
    }

    /**
     * Avoid that the screen get's turned off by the system.
     */
    public void disableScreenTurnOff() {
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON, WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    /**
     * Set's the orientation to landscape, as this is needed by AndAR.
     */
    public void setOrientation() {
        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    }

    /**
     * Maximize the application.
     */
    public void setFullscreen() {
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
    }

    /**
     * Resolution Settings.
     */
    public void selectResDiv() {
        Store.setContext(this);
        Store.setMainActivity(this);
        int d = Store.getResolutionDiv();
        CharSequence[] items = { "640x480 " + ((d == 1) ? "*" : ""), "320x240 " + ((d == 2) ? "*" : ""), "160x120 " + ((d == 4) ? "*" : ""), "Cancel", };
        AlertDialog.Builder builder = new AlertDialog.Builder(Store.getContext());
        builder.setTitle("Select Resolution");
        builder.setItems(items, new DialogInterface.OnClickListener() {
            public void onClick(final DialogInterface dialog, final int item) {
                int div = 4;
                try {
                    switch (item) {
                    case 0:
                        div = 1;
                        break;
                    case 1:
                        div = 2;
                        break;
                    case 2:
                        div = 4;
                        break;
                    default:
                        break;
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
                Store.setResolutionDiv(div);
            }
        });
        AlertDialog alert = builder.create();
        alert.show();
    }

    /* ================================================================ */

    private NativePreviewer mPreview;
    private GL2CameraViewer glview;
    final Processor         processor = new Processor();
    static int              proc_mode = 0;

    /**
     * JNI
     */
    class FastProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {
            processor.detectAndDrawFeatures(idx, pool);
        }
    }

    /**
     * JNI
     */
    class SquaresProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {
            processor.detectAndDrawContours(idx, pool);
        }
    }

    /**
     * JNI
     */
    class SobelProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {
            processor.runSobel(idx, pool);
        }
    }

    /**
     * JNI
     */
    class HistProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {
            processor.runHistEq(idx, pool);
        }
    }

    /**
     * JNI
     */
    final int  set_minmaxexposure = 2; // desired +/- exposure
    final int  set_extraframes    = 5; // waiting period
    static int exposure_cnt       = 0; // range: [0-2] for 3 images
    static int extraframes_cnt    = 5; // start: > 0

    class HDRProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {

            int currexp = mPreview.getExposureValue();
            int targetexp = exposure_cnt * set_minmaxexposure - set_minmaxexposure ;
            // Log.d(TAG, " current-exp " + currexp + ", target-exp " + targetexp);
            if ((currexp) == (targetexp)) {
                // make sure
                if (extraframes_cnt > 0) {
                    extraframes_cnt--;
                    // waiting
                    Log.d(TAG, "waiting for exposure " + extraframes_cnt);
                    processor.runHDR(idx, pool, 1);
                    return;
                }
                extraframes_cnt = set_extraframes;
                // correct
                Log.d(TAG, "got exposures ");
                exposure_cnt++;
                if (exposure_cnt > 2) {
                    exposure_cnt = 0;
                }
                processor.runHDR(idx, pool, 0);
            } else {
                // waiting
                Log.d(TAG, "waiting for exposure change");
                mPreview.setExposureValue(targetexp);
                processor.runHDR(idx, pool, 1);
            }

        }
    }

    /**
     * JNI
     */

    class NEONProcessor implements NativeProcessor.PoolCallback {
        @Override
        public void process(int idx, image_pool pool, long timestamp, NativeProcessor nativeProcessor) {
            processor.runNEON(idx, pool, 0);
        }
    }

    /* ================================================================ */

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add("FAST");
        menu.add("Squares");
        menu.add("Sobel");
        menu.add("NEON");
        menu.add("HistEq");
        menu.add("HDR");
        menu.add("Settings");
        menu.add("About");
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        LinkedList<PoolCallback> defaultcallbackstack = new LinkedList<PoolCallback>();
        defaultcallbackstack.addFirst(glview.getDrawCallback());
        if (item.getTitle().equals("FAST")) {
            CURR_VIEW = DIALOG_FAST;
            toasts(DIALOG_FAST);
            defaultcallbackstack.addFirst(new FastProcessor());
        } else if (item.getTitle().equals("Squares")) {
            CURR_VIEW = DIALOG_SQUARES;
            toasts(DIALOG_SQUARES);
            defaultcallbackstack.addFirst(new SquaresProcessor());
        } else if (item.getTitle().equals("Sobel")) {
            CURR_VIEW = DIALOG_SOBEL;
            toasts(DIALOG_SOBEL);
            defaultcallbackstack.addFirst(new SobelProcessor());
        } else if (item.getTitle().equals("HistEq")) {
            CURR_VIEW = DIALOG_HISTEQ;
            toasts(DIALOG_HISTEQ);
            defaultcallbackstack.addFirst(new HistProcessor());
        } else if (item.getTitle().equals("HDR")) {
            CURR_VIEW = DIALOG_HDR;
            toasts(DIALOG_HDR);
            defaultcallbackstack.addFirst(new HDRProcessor());
        } else if (item.getTitle().equals("NEON")) {
            CURR_VIEW = DIALOG_NEON;
            toasts(DIALOG_NEON);
            defaultcallbackstack.addFirst(new NEONProcessor());
        } else if (item.getTitle().equals("Settings")) {
            toasts(DIALOG_SETTINGS);
        } else if (item.getTitle().equals("About")) {
            toasts(DIALOG_ABOUT);
        }

        mPreview.addCallbackStack(defaultcallbackstack);
        return true;
    }

    @Override
    public void onOptionsMenuClosed(Menu menu) {
        super.onOptionsMenuClosed(menu);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setFullscreen();
        disableScreenTurnOff();
        FrameLayout frame = new FrameLayout(this);

        // resolution settings
        Store.setContext(getApplication());
        Store.setMainActivity(getParent());
        int div = Store.getResolutionDiv();

        // Create our Preview view and set it as the content of our activity.
        mPreview = new NativePreviewer(getApplication(), 640 / div, 480 / div); /* change resolution here! */

        LayoutParams params = new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        params.height = getWindowManager().getDefaultDisplay().getHeight();
        params.width = (int)(params.height * 4.0 / 2.88);

        LinearLayout vidlay = new LinearLayout(getApplication());
        vidlay.setGravity(Gravity.CENTER);
        vidlay.addView(mPreview, params);
        frame.addView(vidlay);

        // Make the glview overlay ontop of video preview
        mPreview.setZOrderMediaOverlay(false);

        glview = new GL2CameraViewer(getApplication(), false, 0, 0);
        glview.setZOrderMediaOverlay(true);
        glview.setLayoutParams(new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));

        // buttons
        LinearLayout buttons = new LinearLayout(getApplicationContext());
        buttons.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));

        // focus button
        final Button focus_button = new Button(getApplicationContext());
        focus_button.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        focus_button.setText("[Focus]");
        focus_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mPreview.postautofocus(100);
            }
        });
        buttons.addView(focus_button);

        // mode button
        final Button mode_button = new Button(getApplicationContext());
        mode_button.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        mode_button.setText("[Mode: " + proc_mode + "]");
        mode_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                ++proc_mode;
                if (proc_mode > 2) {
                    proc_mode = 0;
                } // wrap
                mode_button.setText("[Mode: " + proc_mode + "]");
                processor.setMode(proc_mode);
                toasts(DIALOG_MODE);
            }
        });
        buttons.addView(mode_button);

        // save picture button
        final Button pic_button = new Button(getApplicationContext());
        pic_button.setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        pic_button.setText("[Pic]");
        pic_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pic_button.setText("[Snap]");
                // create folder
                try {
                    File sdCardFile = Environment.getExternalStorageDirectory();
                    if (sdCardFile.canWrite() == true) {
                        File viewerFile = new File(sdCardFile, "ViewerCV");
                        if (!viewerFile.exists()) { viewerFile.mkdirs(); }
                    } else {
                        Log.e(TAG, "ViewerCV - Cannot write to SD Card");
                    }
                } catch (Exception e) {
                    Log.e(TAG, "ViewerCV - Error - " + e);
                    e.printStackTrace();
                }
                // take picture
                processor.snapPic();
                toasts(DIALOG_SNAP);
            }
        });
        buttons.addView(pic_button);

        // add view/buttons
        frame.addView(glview);
        frame.addView(buttons);
        setContentView(frame);

        toasts(DIALOG_OPENING);
    }

    @Override
    protected void onPause() {
        super.onPause();
        // clears the callback stack
        mPreview.onPause();
        glview.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        glview.onResume();
        // add an initiall callback stack to the preview on resume...
        // this one will just draw the frames to opengl
        LinkedList<NativeProcessor.PoolCallback> cbstack = new LinkedList<PoolCallback>();
        cbstack.add(glview.getDrawCallback());
        mPreview.addCallbackStack(cbstack);
        mPreview.onResume();
        /* ===== */
        mPreview.setExposureValue(0);
    }

}
