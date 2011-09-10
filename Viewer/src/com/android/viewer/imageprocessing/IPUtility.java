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

import com.android.viewer.Singleton;

import android.graphics.Bitmap;
import android.util.Log;

public class IPUtility {

	private static final String TAG = "IPUtility";

	public static Bitmap renderBitmap(int[] pixels, int width, int height) {
		if (pixels != null) {
			Bitmap bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888); // 0xaarrggbb
			bitmap.setPixels(pixels, 0, width, 0, 0, width, height);
			return bitmap;
		}
		return null;
	}

	public static void renderBitmapInPlace(int[] pixels, int width, int height) {
		if (pixels != null) {
			if (Singleton.updateImageView == null || Singleton.updateImageView.getWidth() != width) {
				Log.d(TAG, "INIT renderBitmap2" );
				Singleton.updateImageView = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888); // 0xaarrggbb
			}
			Singleton.updateImageView.setPixels(pixels, 0, width, 0, 0, width, height);
		}
	}

//	public static int[] cvtRGB2GRAY(int[] rgb, int width, int height) {
//		int[] gray = rgb;
//		Native.cvtRGB2GRAY_native(rgb, width, height, gray);
//		return gray;
//	}

	public static int[] cvtRGB2GRAY(int[] data, int width, int height) {
		Native.cvtRGB2GRAY_native(data, width, height, data);
		return data;
	}

	public static int[] decodeYUV420SP(byte[] yuv420sp, int width, int height, int[] rgb) {
		Native.decodeYUV420SP_native(yuv420sp, width, height, rgb);
		yuv420sp = null;
		return rgb;
	}

	public static int[] shrinkImage(int[] pixels, int origImgWidth , int origImgHeight , int scaleDownFactor) {
		int targetWidth = origImgWidth / scaleDownFactor;
		int targetHeight = origImgHeight / scaleDownFactor;
		int xRatio = origImgWidth / targetWidth;
		int yRatio = origImgHeight / targetHeight;
		int[] newPixels = pixels; // in-place
		int yNewOffset = 0;
		int yOrigImgIndex = 0;
		for (int y = 0; y < targetHeight; ++y) {
			int xOrigImgIndex = 0;
			for (int x = 0; x < targetWidth; ++x) {
				newPixels[yNewOffset + x] = pixels[(yOrigImgIndex * origImgWidth) + xOrigImgIndex];
				xOrigImgIndex = xOrigImgIndex + xRatio;
			}
			yNewOffset = yNewOffset + targetWidth;
			yOrigImgIndex = yOrigImgIndex + yRatio;
		}
		return newPixels;
	}

}
