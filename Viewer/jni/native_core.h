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

//#define MIN(a, b)  (((a) < (b)) ? (a) : (b))
//#define MAX(a, b)  (((a) > (b)) ? (a) : (b))

void cvtYUV2RGB(jbyte* yuv420sp, int width, int height, jint* rgb) {
    int frameSize = width * height;
    int j, yp, uvp, i, y, u, v, y1192, r, g, b;
    for (j = 0, yp = 0; j < height; ++j) {
        uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
        for (i = 0; i < width; ++i, ++yp) {
            y = (0xff & ((int) yuv420sp[yp])) - 16;
            if (y < 0) { y = 0;	}
            y1192 = 1192 * y;
            if ((i & 1) == 0) {
                v = (0xff & yuv420sp[uvp++]) - 128;
                u = (0xff & yuv420sp[uvp++]) - 128;
            }
            r = (y1192 + 1634 * v);
            g = (y1192 -  833 * v - 400 * u);
            b = (y1192 + 2066 * u);
            if (r < 0) { r = 0; }
            else if (r > 262143) { r = 262143; }
            if (g < 0) { g = 0; }
            else if (g > 262143) { g = 262143; }
            if (b < 0) { b = 0; }
            else if (b > 262143) { b = 262143; }
            rgb[yp] = 0xff000000 | ((r << 6) & 0xff0000) | ((g >> 2) & 0xff00) | ((b >> 10) & 0xff);
        }
    }
}

inline int toGrayPix(int color, int alpha) {
    float r = (float)((color >> 16) & 0xff);
    float g = (float)((color >> 8) & 0xff);
    float b = (float)((color) & 0xff);
    r *= 0.299f;
    g *= 0.587f;
    b *= 0.114f;
    int sum = (int)((r + g + b)) & 0xff;
    return ((alpha * 0x01000000) | sum | (sum << 8) | (sum << 16));
}

void cvtRGB2GRAY(jint* rgb, int width, int height, jint* gray) {
    int numpix = width * height;
    int i;
    int* rgbptr = rgb;
    int* grayptr = gray;
    for (i = 0; i < numpix; ++i) {
        *grayptr = toGrayPix(*rgbptr, 255);
        ++rgbptr;
        ++grayptr;
    }
}

int getGrayVal(int color) {
    float r = (float)((color >> 16) & 0xff);
    float g = (float)((color >>  8) & 0xff);
    float b = (float)((color) & 0xff);
    r *= 0.299f;
    g *= 0.587f;
    b *= 0.114f;
    int sum = (int)((r + g + b)) & 0xff;
    return sum;
}

void averageArray(int* array, int length) {
    const int AVG_WINDOW_SIZE = 1;
    const int adiv = ((2 * AVG_WINDOW_SIZE) + 1);
    int j, startWindowIndex, endWindowIndex, k;
    float averagedValue;
    for (j = 0; j < length; ++j) {
        averagedValue = 0;
        startWindowIndex = j - AVG_WINDOW_SIZE;
        endWindowIndex = j + AVG_WINDOW_SIZE;
        for (k = startWindowIndex; k <= endWindowIndex; ++k) {
            if (k >= 0 && k < length) {
                averagedValue += array[k];
            }
        }
        averagedValue /= adiv;
        array[j] = (int) averagedValue;
    }
}

void createHistogram(int* pixels, int numpix, int* pixelBins) {
    const int numbins = 256;
    int i, pixelValue;
    int* ptr = pixelBins;
    // clear
    for (i = 0; i < numbins; ++i) {
        *ptr = 0;
        ++ptr;
    }
    ptr = pixels;
    // create
    for (i = 0; i < numpix; ++i) {
        pixelValue = getGrayVal(*ptr);
        pixelBins[pixelValue] += 1;
        ++ptr;
    }
    //averageArray(pixelBins, numbins);
}

int cvtScale(int val, int old_max, int new_max) {
    val = val > old_max ? old_max : val;  // bounds hack
    // new_value = ( (old_value - old_min) / (old_max - old_min) ) * (new_max - new_min) + new_min
    return ((val) * (new_max)) / (old_max); // assumes min is 0
}

void overlayHistogram(int* img, int imgWidth, int imgHeight, int* hist) {
    int histlen = 255;
    int x, hx, hy, y;
    imgHeight -= 1;
    for (x = 0; x <= histlen; ++x) {
        hx = cvtScale(x, histlen, imgWidth - 1);
        hy = imgHeight - cvtScale(hist[x], 255, imgHeight);
        // fill
        for (y = hy; y < imgHeight; ++y) {
            img[y* imgWidth + hx] = 0xff00ff00;  // green
        }
    }
}

/* Adapted from:                                                            */
/*   TEXAS INSTRUMENTS, INC.                                                */
/*   IMGLIB  DSP Image/Video Processing Library                             */
void sobelFilter(int* input, int width, int height, int* output, int negative) {
    int H, O, V, i;
    int i00, i01, i02;
    int i10,      i12;
    int i20, i21, i22;
    int w = width;
    int numpx = width * (height - 1);
    for (i = 0; i < numpx - 1; ++i) {
        i00 = input[i    ] & 0xff; i01 = input[i    +1] & 0xff; i02 = input[i    +2] & 0xff;
        i10 = input[i+  w] & 0xff;                     	        i12 = input[i+  w+2] & 0xff;
        i20 = input[i+2*w] & 0xff; i21 = input[i+2*w+1] & 0xff; i22 = input[i+2*w+2] & 0xff;
        H = -  i00 - 2 * i01 -  i02 +
            +  i20 + 2 * i21 +  i22;
        V = -     i00  +     i02
            - 2 * i10  + 2 * i12
            -     i20  +     i22;
        O = abs(H) + abs(V);
        if (O > 255) { O = 255; }
        if (negative) { O = 255 - O; }
        output[i + 1] = 0xff000000 | ((int) O << 16 | (int) O << 8 | (int) O);
    }
}

/* Adapted from:                                                            */
/*   http://stereolab.googlecode.com/svn                                    */
void histogramEq(int* data, int* hist, int width, int height) {
    int idx, len, val;
    float r;
    float* fhist;

    len = width * height;
    fhist = (float*) malloc(256 * sizeof(float));

    // normalize and accumulate the histogram
    r = 255.0f / (len);
    fhist[0] = r * hist[0];
    for (idx = 1; idx < 256; ++idx) {
        fhist[idx] = r * hist[idx];
        fhist[idx] += fhist[idx - 1];
    }
    // equalize grayscale image
    for (idx = 0; idx < len; ++idx) {
        val = (int)(fhist[((*data)&0xff)]);
        if (val > 255) { val = 255; }
        else if (val < 0) { val = 0; }
        *data = 0xff000000 | ((int) val << 16 | (int) val << 8 | (int) val);
        ++data;
    }

    free(fhist);
}


//#undef MIN
//#undef MAX
