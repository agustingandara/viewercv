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

import android.graphics.Bitmap;

public class Singleton {

	public static Bitmap updateImageView = null;

	public static final int STATE_INIT_APP = 0;
	public static final int STATE_DEFAULT = 1;
	public static final int STATE_PROC_1 = 2;
	public static final int STATE_PROC_2 = 3;
	public static final int STATE_PROC_3 = 4;
	public static final int STATE_PROC_4 = 5;

	private static int mApplicationState = STATE_INIT_APP;
	private static boolean mControlMsgSent = false;
	private static long mTimeElapse = 0;
	private static long mStateTimerStart = 0;
	private static int mProductID = -1;
	private static float mFPS = 0;
	private static float _k = 0.20f;


	public static synchronized void setApplicationState(int s) {
		mApplicationState = s;
		mControlMsgSent = false;
	}

	public static float getStatePeriodSecs(long t) {
		return (float)(t - mStateTimerStart) / 1000000000.0f;
	}

	public static void resetStateTimer() {
		mStateTimerStart = System.nanoTime();
	}

	public static boolean hasVoiceCommandBeenSent() {
		return mControlMsgSent;
	}

	public static long timeElapsed(long t) {
		return t - mTimeElapse;
	}

	public static int getApplicationState() {
		return mApplicationState;
	}

	public static void setProductID(int id) {
		mProductID = id;
	}

	public static int getProductID() {
		return mProductID;
	}

	public static float getFPS(int frames) {
		float fps = (float)(frames) / getStatePeriodSecs(System.nanoTime());
		// moving avg
		mFPS = (_k) * fps + (1 - _k) * mFPS;
		return mFPS;
	}

}
