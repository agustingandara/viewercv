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

public class Native {


	static {
		System.loadLibrary("ViewerN");
	}

	/**
	 *
	 */
	public static native int decodeYUV420SP_native(byte[] yuv420sp, int width, int height, int[] rgb);

	/**
	 *
	 */
	public static native int cvtRGB2GRAY_native(int[] input, int width, int height, int[] output);

	/**
	 *
	 */
	public static native int createHistogram_native(int[] input, int numpix, int[] output);

	/**
	 *
	 */
	public static native int overlayHistogram_native(int[] input, int width, int height, int[] hist);

	/**
	 *
	 */
	public static native int sobel_native(int[] input, int width, int height, int[] output, int negative);

	/**
	 *
	 */
	public static native int histogramEq_native(int[] data, int[] hist, int width, int height);



}
