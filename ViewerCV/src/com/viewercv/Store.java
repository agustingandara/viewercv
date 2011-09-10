
package com.viewercv;

import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class Store {

    private static final String TAG = "viewercv-store";

    private static final String PREFS_NAME = "viewercv";

    private static final String PREFS_RESDIV = "resolution_divisor"; // base: 640x480

    private static final int defaultdiv = 4;

    private static Context context;

    private static Activity mainActivity;

    public static Context getContext() {
        return context;
    }

    public static void setContext(final Context context) {
        Store.context = context;
    }

    public static Activity getMainActivity() {
        return mainActivity;
    }

    public static void setMainActivity(Activity mainActivity) {
        Store.mainActivity = mainActivity;
    }

    public static int getResolutionDiv() {
        final SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, 0);
        return prefs.getInt(PREFS_RESDIV, defaultdiv);
    }

    public static void setResolutionDiv(final int div) {
        final SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, 0);
        final Editor editor = prefs.edit();
        editor.putInt(PREFS_RESDIV, div);
        editor.commit();
    }


}
