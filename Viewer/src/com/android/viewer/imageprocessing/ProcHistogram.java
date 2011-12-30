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

public class ProcHistogram {

    private static int[] mPixelBins;
    final int numbins = 256;

    public ProcHistogram() {
        mPixelBins = new int[numbins];
    }

    public int[] getImageHistogram(int[] pixels) {
        Native.createHistogram_native(pixels, pixels.length, mPixelBins);
        return mPixelBins;
    }

    public int[] overlayHistogram(int[] imgIn, int[] hist, int imgWidth, int imgHeight) {
        Native.overlayHistogram_native(imgIn, imgWidth, imgHeight, hist);
        return imgIn;
    }

    public int[] histogramEq(int[] img, int[] hist, int width, int height) {
        Native.histogramEq_native(img, hist, width, height);
        return img;
    }


}
