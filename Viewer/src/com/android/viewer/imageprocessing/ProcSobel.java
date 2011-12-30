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

public class ProcSobel {

    int[] output;
    int width;
    int height;
    int numPix;
    static boolean init = false;

    public ProcSobel() {
    }

    public void init(int widthIn, int heightIn) {
        if (init) { return; }
        init = true;
        width = widthIn;
        height = heightIn;
        numPix = width * height;
        output = new int[numPix];
    }

    public int[] process_native(int[] input, boolean negative) {
        Native.sobel_native(input, width, height, output, negative ? 1 : 0);
        return output;
    }


}
