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


package com.android.viewer.imageprocessing;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.os.Environment;
import android.util.Log;

public class IOUtility {
	private static String mPrependFileName = "IMG_";
	private static final String TAG = "IOUtility";

	public static boolean saveImageToSDCard(Bitmap b, String appendFileName) {
		if (b != null) {
			try {
				File sdCardFile = Environment.getExternalStorageDirectory();
				if (sdCardFile.canWrite() == true) {
					File viewerFile = new File(sdCardFile, "Viewer");
					viewerFile.mkdirs();
					File imageFile = new File(viewerFile, mPrependFileName + appendFileName + ".png");
					FileOutputStream fileStream = new FileOutputStream(imageFile);
					b.compress(CompressFormat.PNG, 100, fileStream);
					fileStream.close();
				} else {
					Log.e(TAG, "IOUtility - Cannot write to SD Card");
				}
				return true;
			} catch (Exception e) {
				Log.e(TAG, "IOUtility - Error - " + e);
				e.printStackTrace();
			}
		}
		return false;
	}

	public static void setFilePrePend(String fileName) {
		Log.v(TAG, "IOUtility - Settings the prepended filename to: " + fileName);
		mPrependFileName = fileName;
	}

	public static boolean saveTextToSDCard(String content, String appendFileName) {
		try {
			File sdCardFile = Environment.getExternalStorageDirectory();
			if (sdCardFile.canWrite() == true) {
				File viewerFile = new File(sdCardFile, "Viewer");
				viewerFile.mkdirs();
				File textFile = new File(viewerFile, mPrependFileName + appendFileName);
				FileWriter fileWriter = new FileWriter(textFile);
				BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
				try {
					bufferedWriter.write(content);
				} finally {
					bufferedWriter.close();
				}
			} else {
				Log.e(TAG, "IOUtility - Cannot write to SD Card");
			}
			return true;
		} catch (Exception e) {
			Log.e(TAG, "IOUtility - Error [String empty string, depends on state] - " + e);
			e.printStackTrace();
		}
		return false;
	}

}
