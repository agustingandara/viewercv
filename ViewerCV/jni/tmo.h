//#ifndef __TMO_H__
//#define __TMO_H__
//#include "tmo.h"
//#include <omp.h>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
using namespace std;

//#include <cv.h>
//#include <cxcore.h>
//#include <highgui.h>
//using namespace cv;


void makehdr3(Mat* im1, Mat* im2, Mat* im3, Mat* hdr) {

    Scalar s1 = mean(*im1); // dark
    Scalar s2 = mean(*im2); // med
    Scalar s3 = mean(*im3); // light

    float mean1 = (s1[1] + s1[2] + s1[3]) / 3;
    float mean2 = (s2[1] + s2[2] + s2[3]) / 3;
    float mean3 = (s3[1] + s3[2] + s3[3]) / 3;

    float r1 = mean1 / mean3 ;
    float r2 = 1  ;
    float r3 = mean3 / mean1 ;

    // add
    *hdr = (*im1 / r1 + *im2 / r2 + *im3 / r3) ;
}

void makehdr2(Mat* im1, Mat* im3, Mat* hdr) {

    Scalar s1 = mean(*im1); // dark
    Scalar s3 = mean(*im3); // light

    float mean1 = (s1[1] + s1[2] + s1[3]) / 3;
    float mean3 = (s3[1] + s3[2] + s3[3]) / 3;

    float r1 = mean1 / mean3;
    float r3 = mean3 / mean1;

    // add
    *hdr = (*im1 / r1 + *im3 / r3) ;
}

void makehdr3log(Mat* im1, Mat* im2, Mat* im3, Mat* hdr) {

    (*im1).convertTo(*im1, CV_32FC3);
    (*im2).convertTo(*im2, CV_32FC3);
    (*im3).convertTo(*im3, CV_32FC3);

    *im1 += .01;
    *im2 += .01;
    *im3 += .01;

    Mat temp1((*im1).reshape(1, hdr->rows));
    Mat temp2((*im2).reshape(1, hdr->rows));
    Mat temp3((*im3).reshape(1, hdr->rows));

    cv::log(temp1, temp1);
    cv::log(temp2, temp2);
    cv::log(temp3, temp3);

    temp1 = temp1.reshape(3, hdr->rows);
    temp2 = temp2.reshape(3, hdr->rows);
    temp3 = temp3.reshape(3, hdr->rows);

    *hdr = (temp1 + temp2 + temp3) / 3;
}

void makehdr2log(Mat* im1, Mat* im3, Mat* hdr) {

    (*im1).convertTo(*im1, CV_32FC3);
    (*im3).convertTo(*im3, CV_32FC3);

    *im1 += .01;
    *im3 += .01;

    Mat temp1((*im1).reshape(1, hdr->rows));
    Mat temp3((*im3).reshape(1, hdr->rows));

    cv::log(temp1, temp1);
    cv::log(temp3, temp3);

    temp1 = temp1.reshape(3, hdr->rows);
    temp3 = temp3.reshape(3, hdr->rows);

    *hdr = (temp1  + temp3) / 2;
}



/**
 * Modified by Chris McClanahan for Android JNI
 *
 * @Brief Contrast mapping TMO
 *
 * From:
 *
 * Rafal Mantiuk, Karol Myszkowski, Hans-Peter Seidel.
 * A Perceptual Framework for Contrast Processing of High Dynamic Range Images
 * In: ACM Transactions on Applied Perception 3 (3), pp. 286-308, 2006
 * http://www.mpi-inf.mpg.de/~mantiuk/contrast_domain/
 *
 * This file is a part of LuminanceHDR package, based on pfstmo.
 * ----------------------------------------------------------------------
 * Copyright (C) 2007 Grzegorz Krawczyk
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * ----------------------------------------------------------------------
 *
 * @author Radoslaw Mantiuk, <radoslaw.mantiuk@gmail.com>
 * @author Rafal Mantiuk, <mantiuk@gmail.com>
 * Updated 2007/12/17 by Ed Brambley <E.J.Brambley@damtp.cam.ac.uk>
 *  (more information on the changes:
 *  http://www.damtp.cam.ac.uk/user/ejb48/hdr/index.html)
 * Updated 2008/06/25 by Ed Brambley <E.J.Brambley@damtp.cam.ac.uk>
 *  bug fixes and openMP patches
 *  more on this:
 *  http://groups.google.com/group/pfstools/browse_thread/thread/de2378af98ec6185/0dee5304fc14e99d?hl=en#0dee5304fc14e99d
 *  Optimization improvements by Lebed Dmytry
 *
 * Updated 2008/07/26 by Dejan Beric <dejan.beric@live.com>
 *  Added the detail Factor slider which offers more control over contrast in details
 * Update 2010/10/06 by Axel Voitier <axel.voitier@gmail.com>
 *  detailfactor patch in order to remove potential issues in a multithreading environment
 *
 * $Id: contrast_domain.cpp,v 1.14 2008/08/26 17:08:49 rafm Exp $
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define likely(x)       (x)
#define unlikely(x)     (x)

typedef struct pyramid_s {
    int rows;
    int cols;
    float* Gx;
    float* Gy;
    struct pyramid_s* next;
    struct pyramid_s* prev;
} pyramid_t;

#define PYRAMID_MIN_PIXELS      3
#define LOOKUP_W_TO_R           107

void contrast_equalization(pyramid_t* pp, const float contrastFactor);
void transform_to_luminance(pyramid_t* pyramid, float*   x, const bool bcg);
void matrix_add(const int n, const float*   a, float*   b);
void matrix_subtract(const int n, const float*   a, float*   b);
void matrix_copy(const int n, const float*   a, float*   b);
void matrix_multiply_const(const int n, float*   a, const float val);
void matrix_divide(const int n, const float* a, float* b);
float* matrix_alloc(const int size);
void matrix_free(float* m);
float matrix_DotProduct(const int n, const float*   a, const float*   b);
void matrix_zero(const int n, float*   m);
void calculate_and_add_divergence(const int rows, const int cols, const float*   Gx, const float*   Gy, float*   divG);
void pyramid_calculate_divergence(pyramid_t* pyramid);
void pyramid_calculate_divergence_sum(pyramid_t* pyramid, float* divG_sum);
void calculate_scale_factor(const int n, const float*   G, float*   C);
void pyramid_calculate_scale_factor(pyramid_t* pyramid, pyramid_t* pC);
void scale_gradient(const int n, float*   G, const float*   C);
void pyramid_scale_gradient(pyramid_t* pyramid, pyramid_t* pC);
void pyramid_free(pyramid_t* pyramid);
pyramid_t* pyramid_allocate(const int cols, const int rows);
void calculate_gradient(const int cols, const int rows, const float*   lum, float*   Gx, float*   Gy);
void pyramid_calculate_gradient(pyramid_t* pyramid, float* lum);
void solveX(const int n, const float*   b, float*   x);
void multiplyA(pyramid_t* px, pyramid_t* pyramid, const float*   x, float*   divG_sum);
void linbcg(pyramid_t* pyramid, pyramid_t* pC, const float*   b, float*   x, const int itmax, const float tol);
void lincg(pyramid_t* pyramid, pyramid_t* pC, const float*   b, float*   x, const int itmax, const float tol);
float lookup_table(const int n, const float*   in_tab, const float*   out_tab, const float val);
void transform_to_R(const int n, float*   G, float detail_factor);
void pyramid_transform_to_R(pyramid_t* pyramid, float detail_factor);
void transform_to_G(const int n, float*   R, float detail_factor);
void pyramid_transform_to_G(pyramid_t* pyramid, float detail_factor);
void pyramid_gradient_multiply(pyramid_t* pyramid, const float val);
void dump_matrix_to_file(const int width, const int height, const float*   m, const char* const file_name);
void matrix_show(const char* const text, int rows, int cols, const float*   data);
void pyramid_show(pyramid_t* pyramid);

static float W_table[] = {0.000000, 0.010000, 0.021180, 0.031830, 0.042628, 0.053819, 0.065556, 0.077960, 0.091140, 0.105203, 0.120255, 0.136410, 0.153788, 0.172518, 0.192739, 0.214605, 0.238282, 0.263952, 0.291817, 0.322099, 0.355040, 0.390911, 0.430009, 0.472663, 0.519238, 0.570138, 0.625811, 0.686754, 0.753519, 0.826720, 0.907041, 0.995242, 1.092169, 1.198767, 1.316090, 1.445315, 1.587756, 1.744884, 1.918345, 2.109983, 2.321863, 2.556306, 2.815914, 3.103613, 3.422694, 3.776862, 4.170291, 4.607686, 5.094361, 5.636316, 6.240338, 6.914106, 7.666321, 8.506849, 9.446889, 10.499164, 11.678143, 13.000302, 14.484414, 16.151900, 18.027221, 20.138345, 22.517282, 25.200713, 28.230715, 31.655611, 35.530967, 39.920749, 44.898685, 50.549857, 56.972578, 64.280589, 72.605654, 82.100619, 92.943020, 105.339358, 119.530154, 135.795960, 154.464484, 175.919088, 200.608905, 229.060934, 261.894494, 299.838552, 343.752526, 394.651294, 453.735325, 522.427053, 602.414859, 695.706358, 804.693100, 932.229271, 1081.727632, 1257.276717, 1463.784297, 1707.153398, 1994.498731, 2334.413424, 2737.298517, 3215.770944, 3785.169959, 4464.187290, 5275.653272, 6247.520102, 7414.094945, 8817.590551, 10510.080619};
static float R_table[] = {0.000000, 0.009434, 0.018868, 0.028302, 0.037736, 0.047170, 0.056604, 0.066038, 0.075472, 0.084906, 0.094340, 0.103774, 0.113208, 0.122642, 0.132075, 0.141509, 0.150943, 0.160377, 0.169811, 0.179245, 0.188679, 0.198113, 0.207547, 0.216981, 0.226415, 0.235849, 0.245283, 0.254717, 0.264151, 0.273585, 0.283019, 0.292453, 0.301887, 0.311321, 0.320755, 0.330189, 0.339623, 0.349057, 0.358491, 0.367925, 0.377358, 0.386792, 0.396226, 0.405660, 0.415094, 0.424528, 0.433962, 0.443396, 0.452830, 0.462264, 0.471698, 0.481132, 0.490566, 0.500000, 0.509434, 0.518868, 0.528302, 0.537736, 0.547170, 0.556604, 0.566038, 0.575472, 0.584906, 0.594340, 0.603774, 0.613208, 0.622642, 0.632075, 0.641509, 0.650943, 0.660377, 0.669811, 0.679245, 0.688679, 0.698113, 0.707547, 0.716981, 0.726415, 0.735849, 0.745283, 0.754717, 0.764151, 0.773585, 0.783019, 0.792453, 0.801887, 0.811321, 0.820755, 0.830189, 0.839623, 0.849057, 0.858491, 0.867925, 0.877358, 0.886792, 0.896226, 0.905660, 0.915094, 0.924528, 0.933962, 0.943396, 0.952830, 0.962264, 0.971698, 0.981132, 0.990566, 1.000000};


inline float max(float a, float b) {
    return a > b ? a : b;
}

inline float min(float a, float b) {
    return a < b ? a : b;
}

inline int imin(int a, int b) {
    return a < b ? a : b;
}

// upsample the matrix
// upsampled matrix is twice bigger in each direction than data[]
// res should be a pointer to allocated memory for bigger matrix
// cols and rows are the dimmensions of the output matrix
void matrix_upsample(const int outCols, const int outRows, const float*   in, float*   out) {
    const int inRows = outRows / 2;
    const int inCols = outCols / 2;

    // Transpose of experimental downsampling matrix (theoretically the correct thing to do)

    const float dx = (float)inCols / ((float)outCols);
    const float dy = (float)inRows / ((float)outRows);
    const float factor = 1.0f / (dx * dy); // This gives a genuine upsampling matrix, not the transpose of the downsampling matrix
    // const float factor = 1.0f; // Theoretically, this should be the best.

    for (int y = 0; y < outRows; ++y) {
        const float sy = y * dy;
        const int iy1 = (y   * inRows) / outRows;
        const int iy2 = imin(((y + 1) * inRows) / outRows, inRows - 1);

        for (int x = 0; x < outCols; ++x) {
            const float sx = x * dx;
            const int ix1 = (x   * inCols) / outCols;
            const int ix2 = imin(((x + 1) * inCols) / outCols, inCols - 1);

            out[x + y* outCols] = (((ix1 + 1) - sx) * ((iy1 + 1 - sy)) * in[ix1 + iy1*inCols] +
                                   ((ix1 + 1) - sx) * (sy + dy - (iy1 + 1)) * in[ix1 + iy2*inCols] +
                                   (sx + dx - (ix1 + 1)) * ((iy1 + 1 - sy)) * in[ix2 + iy1*inCols] +
                                   (sx + dx - (ix1 + 1)) * (sy + dx - (iy1 + 1)) * in[ix2 + iy2*inCols]) * factor;

        }
    }
}

// downsample the matrix
void matrix_downsample(const int inCols, const int inRows, const float*   data, float*   res) {
    const int outRows = inRows / 2;
    const int outCols = inCols / 2;
    const float dx = (float)inCols / ((float)outCols);
    const float dy = (float)inRows / ((float)outRows);
    const float normalize = 1.0f / (dx * dy);


    for (int y = 0; y < outRows; ++y) {
        const int iy1 = (y   * inRows) / outRows;
        const int iy2 = ((y + 1) * inRows) / outRows;
        const float fy1 = (iy1 + 1) - y * dy;
        const float fy2 = (y + 1) * dy - iy2;

        for (int x = 0; x < outCols; ++x) {
            const int ix1 = (x   * inCols) / outCols;
            const int ix2 = ((x + 1) * inCols) / outCols;
            const float fx1 = (ix1 + 1) - x * dx;
            const float fx2 = (x + 1) * dx - ix2;
            float pixVal = 0.0f;
            float factorx, factory;

            for (int i = iy1; i <= iy2 && i < inRows; ++i) {
                if (i == iy1) {
                    factory = fy1;    // We're just getting the bottom edge of this pixel
                } else if (i == iy2) {
                    factory = fy2;    // We're just gettting the top edge of this pixel
                } else {
                    factory = 1.0f;    // We've got the full height of this pixel
                }

                for (int j = ix1; j <= ix2 && j < inCols; ++j) {
                    if (j == ix1) {
                        factorx = fx1;    // We've just got the right edge of this pixel
                    } else if (j == ix2) {
                        factorx = fx2;    // We've just got the left edge of this pixel
                    } else {
                        factorx = 1.0f;    // We've got the full width of this pixel
                    }
                    pixVal += data[j + i*inCols] * factorx * factory;
                }
            }
            res[x + y* outCols] = pixVal * normalize;   // Normalize by the area of the new pixel
        }
    }
}


// alloc memory for the float table
inline float* matrix_alloc(int size) {
    //float* m    = (float*)_mm_malloc  (sizeof(float) * size, 16);
    float* m  = (float*)malloc(sizeof(float) * size);
    if (m == NULL) {
        fprintf(stderr, "ERROR: malloc in matrix_alloc() (size:%d)", size);
        exit(155);
    }
    return m;
}

// free memory for matrix
inline void matrix_free(float* m) {
    if (m != NULL) {
        free(m);
        //_mm_free(m);
        m = NULL;
    } else {
        fprintf(stderr, "ERROR: This pointer has already been freed");
    }
}

// return = a + b
inline void matrix_add(const int n, const float*   a, float*   b) {

    for (int i = 0; i < n; ++i) {
        b[i] += a[i];
    }
}

// return = a - b
inline void matrix_subtract(const int n, const float*   a, float*   b) {

    for (int i = 0; i < n; ++i) {
        b[i] = a[i] - b[i];
    }
}

// multiply matrix a by scalar val
inline void matrix_multiply_const(const int n, float*   a, const float val) {

    for (int i = 0; i < n; ++i) {
        a[i] *= val;
    }
}

// multiply vector by vector (each vector should have one dimension equal to 1)
float matrix_DotProduct(const int n, const float*   a, const float*   b) {
    float val = 0;

    for (int j = 0; j < n; ++j) {
        val += a[j] * b[j];
    }
    return val;
}

// copy matix a to b, return = a
inline void matrix_copy(const int n, const float*   a, float*   b) {
    memcpy(b, a, sizeof(float)*n);
}

// set zeros for matrix elements
inline void matrix_zero(int n, float* m) {
    //bzero(m, n*sizeof(float));
    memset(m, 0, n * sizeof(float));
}

// Davide Anastasia <davide.anastasia@gmail.com> (2010 08 31)
// calculate divergence of two gradient maps (Gx and Gy)
// divG(x,y) = Gx(x,y) - Gx(x-1,y) + Gy(x,y) - Gy(x,y-1)
inline void calculate_and_add_divergence(const int cols, const int rows, const float*   Gx, const float*   Gy, float*   divG) {
    register float divGx, divGy;

    // kx = 0 AND ky = 0;
    divG[0] += Gx[0] + Gy[0];

    // ky = 0

    for (int kx = 1; kx < cols; ++kx) {
        divGx = Gx[kx] - Gx[kx - 1];
        divGy = Gy[kx];
        divG[kx] += divGx + divGy;
    }

    for (int ky = 1; ky < rows; ++ky) {
        int kycols = ky * cols;

        // kx = 0
        divGx = Gx[kycols];
        divGy = Gy[kycols] - Gy[kycols - cols];
        divG[ky* cols] += divGx + divGy;

        // kx > 0
        for (int kx = 1; kx < cols; ++kx) {
            divGx = Gx[kx + kycols] - Gx[kx + kycols-1];
            divGy = Gy[kx + kycols] - Gy[kx + kycols - cols];
            divG[kx + kycols] += divGx + divGy;
        }
    }
}

// calculate the sum of divergences for the all pyramid level
// the smaller divergence map is upsamled and added to the divergence map for the higher level of pyramid
// temp is a temporary matrix of size (cols, rows), assumed to already be allocated
void pyramid_calculate_divergence_sum(pyramid_t* pyramid, float* divG_sum) {
    float* temp = matrix_alloc(pyramid->rows * pyramid->cols);

    // Find the coarsest pyramid, and the number of pyramid levels
    int levels = 1;
    while (pyramid->next != NULL) {
        ++levels;
        pyramid = pyramid->next;
    }

    // For every level, we swap temp and divG_sum.  So, if there are an odd number of levels...
    if (levels % 2) {
        float*   dummy = divG_sum;
        divG_sum = temp;
        temp = dummy;
    }

    // Add them all together
    while (pyramid != NULL) {
        // Upsample or zero as needed
        if (pyramid->next != NULL) {
            matrix_upsample(pyramid->cols, pyramid->rows, divG_sum, temp);
        } else {
            matrix_zero(pyramid->rows * pyramid->cols, temp);
        }

        // Add in the (freshly calculated) divergences
        calculate_and_add_divergence(pyramid->cols, pyramid->rows, pyramid->Gx, pyramid->Gy, temp);

        /* matrix_copy(pyramid->rows*pyramid->cols, temp, divG_sum); */
        // Rather than copying, just switch round the pointers: we know we get them the right way round at the end.
        float*   dummy = divG_sum;
        divG_sum = temp;
        temp = dummy;
        pyramid = pyramid->prev;
    }

    matrix_free(temp);
}

// calculate scale factors (Cx,Cy) for gradients (Gx,Gy)
inline void calculate_scale_factor(const int n, const float*   G, float*   C) {
    const float detectT = 0.001f;
    const float a = 0.038737;
    const float b = 0.537756;

    for (int i = 0; i < n; ++i) {
        C[i] = 1.0f / (a * powf((max(detectT, fabsf(G[i]))) , b));
    }
}

// calculate scale factor for the whole pyramid
void pyramid_calculate_scale_factor(pyramid_t* pyramid, pyramid_t* pC) {
    while (pyramid != NULL) {
        const int size = pyramid->rows * pyramid->cols;
        calculate_scale_factor(size, pyramid->Gx, pC->Gx);
        calculate_scale_factor(size, pyramid->Gy, pC->Gy);
        pyramid = pyramid->next;
        pC = pC->next;
    }
}

// Scale gradient (Gx and Gy) by C (Cx and Cy)
// G = G / C
inline void scale_gradient(const int n, float*   G, const float*   C) {

    for (int i = 0; i < n; ++i) {
        G[i] *= C[i];
    }
}

// scale gradients for the whole one pyramid with the use of (Cx,Cy) from the other pyramid
void pyramid_scale_gradient(pyramid_t* pyramid, pyramid_t* pC) {
    while (pyramid != NULL) {
        const int size = pyramid->rows * pyramid->cols;
        scale_gradient(size, pyramid->Gx, pC->Gx);
        scale_gradient(size, pyramid->Gy, pC->Gy);
        pyramid = pyramid->next;
        pC = pC->next;
    }
}


// free memory allocated for the pyramid
void pyramid_free(pyramid_t* pyramid) {
    while (pyramid) {
        if (pyramid->Gx != NULL) {
            matrix_free(pyramid->Gx);   //free(pyramid->Gx);
            pyramid->Gx = NULL;
        }
        if (pyramid->Gy != NULL) {
            matrix_free(pyramid->Gy);   //free(pyramid->Gy);
            pyramid->Gy = NULL;
        }
        pyramid_t* const next = pyramid->next;
        pyramid->prev = NULL;
        pyramid->next = NULL;
        free(pyramid);
        pyramid = next;
    }
}


// allocate memory for the pyramid
pyramid_t* pyramid_allocate(int cols, int rows) {
    pyramid_t* level = NULL;
    pyramid_t* pyramid = NULL;
    pyramid_t* prev = NULL;

    while (rows >= PYRAMID_MIN_PIXELS && cols >= PYRAMID_MIN_PIXELS) {
        level = (pyramid_t*) malloc(sizeof(pyramid_t));
        if (level == NULL) {
            fprintf(stderr, "ERROR: malloc in pyramid_alloc() (size:%zu)", sizeof(pyramid_t));
            exit(155);
        }
        memset(level, 0, sizeof(pyramid_t));

        const int size = rows * cols;
        level->rows = rows;
        level->cols = cols;
        level->Gx = matrix_alloc(size);
        level->Gy = matrix_alloc(size);
        level->prev = prev;

        if (prev != NULL) { prev->next = level; }
        prev = level;
        if (pyramid == NULL) { pyramid = level; }

        rows /= 2;
        cols /= 2;
    }

    return pyramid;
}


// calculate gradients
inline void calculate_gradient(const int cols, const int rows, const float*   lum, float*   Gx, float*   Gy) {

    for (int ky = 0; ky < rows; ++ky) {
        for (int kx = 0; kx < cols; ++kx) {
            const int idx = kx + ky * cols;
            if (kx == (cols - 1)) { Gx[idx] = 0; }
            else { Gx[idx] = lum[idx+1] - lum[idx]; }
            if (ky == (rows - 1)) { Gy[idx] = 0; }
            else { Gy[idx] = lum[idx + cols] - lum[idx]; }
        }
    }
}

// calculate gradients for the pyramid
// lum_temp gets overwritten!
void pyramid_calculate_gradient(pyramid_t* pyramid, float* lum_temp) {
    float* temp = matrix_alloc((pyramid->rows / 2) * (pyramid->cols / 2));
    float*   temp_saved = temp;
    calculate_gradient(pyramid->cols, pyramid->rows, lum_temp, pyramid->Gx, pyramid->Gy);
    pyramid = pyramid->next;
    while (pyramid) {
        matrix_downsample(pyramid->prev->cols, pyramid->prev->rows, lum_temp, temp);
        calculate_gradient(pyramid->cols, pyramid->rows, temp, pyramid->Gx, pyramid->Gy);
        float*   dummy = lum_temp;
        lum_temp = temp;
        temp = dummy;
        pyramid = pyramid->next;
    }
    matrix_free(temp_saved);
}



// x = -0.25 * b
inline void solveX(const int n, const float*   b, float*   x) {

    for (int i = 0; i < n; ++i) {
        x[i] = (-0.25f) * b[i];
    }
}

// divG_sum = A * x = sum(divG(x))
// memory for the temporary pyramid px should be allocated
inline void multiplyA(pyramid_t* px, pyramid_t* pC, const float*   x, float*   divG_sum) {
    matrix_copy(pC->rows * pC->cols, x, divG_sum); // use divG_sum as a temp variable
    pyramid_calculate_gradient(px, divG_sum);
    pyramid_scale_gradient(px, pC); // scale gradients by Cx,Cy from main pyramid
    pyramid_calculate_divergence_sum(px, divG_sum); // calculate the sum of divergences
}


// bi-conjugate linear equation solver
// overwrites pyramid!
void linbcg(pyramid_t* pyramid, pyramid_t* pC, float*   b, float*   x, const int itmax, const float tol) {
    const int rows = pyramid->rows;
    const int cols = pyramid->cols;
    const int n = rows * cols;
    const float tol2 = tol * tol;

    float* const z = matrix_alloc(n);
    float* const zz = matrix_alloc(n);
    float* const p = matrix_alloc(n);
    float* const pp = matrix_alloc(n);
    float* const r = matrix_alloc(n);
    float* const rr = matrix_alloc(n);
    float* const x_save = matrix_alloc(n);

    const float bnrm2 = matrix_DotProduct(n, b, b);
    multiplyA(pyramid, pC, x, r); // r = A*x = divergence(x)
    matrix_subtract(n, b, r); // r = b - r
    float err2 = matrix_DotProduct(n, r, r); // err2 = r.r
    multiplyA(pyramid, pC, r, rr); // rr = A*r

    float bkden = 0;
    float saved_err2 = err2;
    matrix_copy(n, x, x_save);

    // const float ierr2 = err2;
    // const float percent_sf = 100.0f / logf(tol2 * bnrm2 / ierr2);

    int iter = 0;
    bool reset = true;
    int num_backwards = 0;
    const int num_backwards_ceiling = 3;
    for (; iter < itmax; ++iter) {
        // ph->newValue( (int) (logf(err2 / ierr2)*percent_sf) );
        // if (ph->isTerminationRequested()) { //user request abort
        // 	break;
        // }

        solveX(n, r, z);   //  z = ~A(-1) *  r = -0.25 *  r
        solveX(n, rr, zz); // zz = ~A(-1) * rr = -0.25 * rr

        const float bknum = matrix_DotProduct(n, z, rr);

        if (reset) {
            reset = false;
            matrix_copy(n, z, p);
            matrix_copy(n, zz, pp);
        } else {
            const float bk = bknum / bkden; // beta = ...

            for (int i = 0; i < n; ++i) {
                p[i]  =  z[i] + bk *  p[i];
            }

            for (int i = 0; i < n; ++i) {
                pp[i] = zz[i] + bk * pp[i];
            }
        }

        bkden = bknum; // numerato becomes the dominator for the next iteration

        // slow!
        multiplyA(pyramid, pC,  p,  z); //  z = A* p = divergence( p)
        multiplyA(pyramid, pC, pp, zz); // zz = A*pp = divergence(pp)

        const float ak = bknum / matrix_DotProduct(n, z, pp); // alfa = ...


        for (int i = 0 ; i < n ; ++i) {
            r[i]  -= ak *  z[i];	// r =  r - alfa * z
        }

        for (int i = 0 ; i < n ; ++i) {
            rr[i] -= ak * zz[i];	//rr = rr - alfa * zz
        }

        const float old_err2 = err2;
        err2 = matrix_DotProduct(n, r, r);

        // Have we gone unstable?
        if (err2 > old_err2) {
            // Save where we've got to if it's the best yet
            if (num_backwards == 0 && old_err2 < saved_err2) {
                saved_err2 = old_err2;
                matrix_copy(n, x, x_save);
            }

            num_backwards++;
        } else {
            num_backwards = 0;
        }


        for (int i = 0 ; i < n ; ++i) {
            x[i] += ak * p[i];    // x =  x + alfa * p
        }

        if (num_backwards > num_backwards_ceiling) {
            // Reset
            reset = true;
            num_backwards = 0;

            // Recover saved value
            matrix_copy(n, x_save, x);

            // r = Ax
            multiplyA(pyramid, pC, x, r);

            // r = b - r
            matrix_subtract(n, b, r);

            // err2 = r.r
            err2 = matrix_DotProduct(n, r, r);
            saved_err2 = err2;

            // rr = A*r
            multiplyA(pyramid, pC, r, rr);
        }

        // fprintf(stderr, "iter:%d err:%f\n", iter+1, sqrtf(err2/bnrm2));
        if (err2 / bnrm2 < tol2) {
            break;
        }
    }

    // Use the best version we found
    if (err2 > saved_err2) {
        err2 = saved_err2;
        matrix_copy(n, x_save, x);
    }

    if (err2 / bnrm2 > tol2) {
        // Not converged
        // ph->newValue( (int) (logf(err2 / ierr2)*percent_sf));
        if (iter == itmax) {
            MSG(" Warning: Not converged (hit maximum iterations), error = %f (should be below %f).\n", sqrtf(err2 / bnrm2), tol);
        } else {
            MSG(" Warning: Not converged (going unstable), error = %f (should be below %f).\n", sqrtf(err2 / bnrm2), tol);
        }
    } else {
        // ph->newValue(100);
    }


    matrix_free(x_save);
    matrix_free(p);
    matrix_free(pp);
    matrix_free(z);
    matrix_free(zz);
    matrix_free(r);
    matrix_free(rr);
}


// conjugate linear equation solver
// overwrites pyramid!
void lincg(pyramid_t* pyramid, pyramid_t* pC, const float*   b, float*   x, const int itmax, const float tol) {
    const int rows = pyramid->rows;
    const int cols = pyramid->cols;
    const int n = rows * cols;
    const float tol2 = tol * tol;

    float* const x_save = matrix_alloc(n);
    float* const r = matrix_alloc(n);
    float* const p = matrix_alloc(n);
    float* const Ap = matrix_alloc(n);

    // bnrm2 = ||b||
    const float bnrm2 = matrix_DotProduct(n, b, b);

    // r = b - Ax
    multiplyA(pyramid, pC, x, r);
    matrix_subtract(n, b, r);
    float rdotr = matrix_DotProduct(n, r, r); // rdotr = r.r

    // p = r
    matrix_copy(n, r, p);

    // Setup initial vector
    float saved_rdotr = rdotr;
    matrix_copy(n, x, x_save);
    // const float irdotr = rdotr;
    // const float percent_sf = 100.0f / logf(tol2 * bnrm2 / irdotr);
    int iter = 0;
    int num_backwards = 0;
    const int num_backwards_ceiling = 3;
    for (; iter < itmax; iter++) {
        // ph->newValue( (int) (logf(rdotr / irdotr)*percent_sf) );
        // if ( ph->isTerminationRequested() && iter > 0 ) { // User requested abort
        // 	break;
        // }

        // Ap = A p
        multiplyA(pyramid, pC, p, Ap);

        // alpha = r.r / (p . Ap)
        const float alpha = rdotr / matrix_DotProduct(n, p, Ap);

        // r = r - alpha Ap

        for (int i = 0; i < n; ++i) {
            r[i] -= alpha * Ap[i];
        }

        // rdotr = r.r
        const float old_rdotr = rdotr;
        rdotr = matrix_DotProduct(n, r, r);

        // Have we gone unstable?
        if (rdotr > old_rdotr) {
            // Save where we've got to
            if (num_backwards == 0 && old_rdotr < saved_rdotr) {
                saved_rdotr = old_rdotr;
                matrix_copy(n, x, x_save);
            }

            num_backwards++;
        } else {
            num_backwards = 0;
        }

        // x = x + alpha p

        for (int i = 0; i < n; ++i) {
            x[i] += alpha * p[i];
        }

        // Exit if we're done
        if (rdotr / bnrm2 < tol2) {
            break;
        }

        if (num_backwards > num_backwards_ceiling) {
            // Reset
            num_backwards = 0;
            matrix_copy(n, x_save, x);

            // r = Ax
            multiplyA(pyramid, pC, x, r);

            // r = b - r
            matrix_subtract(n, b, r);

            // rdotr = r.r
            rdotr = matrix_DotProduct(n, r, r);
            saved_rdotr = rdotr;

            // p = r
            matrix_copy(n, r, p);
        } else {
            // p = r + beta p
            const float beta = rdotr / old_rdotr;

            for (int i = 0; i < n; ++i) {
                p[i] = r[i] + beta * p[i];
            }
        }
    }

    // Use the best version we found
    if (rdotr > saved_rdotr) {
        rdotr = saved_rdotr;
        matrix_copy(n, x_save, x);
    }

    if (rdotr / bnrm2 > tol2) {
        // Not converged
        // ph->newValue( (int) (logf(rdotr / irdotr)*percent_sf));
        if (iter == itmax) {
            MSG(" Warning: Not converged (hit maximum iterations), error = %f (should be below %f).\n", sqrtf(rdotr / bnrm2), tol);
        } else {
            MSG(" Warning: Not converged (going unstable), error = %f (should be below %f).\n", sqrtf(rdotr / bnrm2), tol);
        }
    } else {
        // ph->newValue(100);
    }

    matrix_free(x_save);
    matrix_free(p);
    matrix_free(Ap);
    matrix_free(r);
}


// in_tab and out_tab should contain inccreasing float values
inline float lookup_table(const int n, const float*   in_tab, const float*   out_tab, const float val) {
    if (unlikely(val < in_tab[0])) { return out_tab[0]; }

    for (int j = 1; j < n; ++j) {
        if (val < in_tab[j]) {
            const float dd = (val - in_tab[j-1]) / (in_tab[j] - in_tab[j-1]);
            return out_tab[j-1] + (out_tab[j] - out_tab[j-1]) * dd;
        }
    }

    return out_tab[n-1];
}


// transform gradient (Gx,Gy) to R
inline void transform_to_R(const int n, float*   G, float detail_factor) {
    const float log10 = 2.3025850929940456840179914546844 * detail_factor;

    for (int j = 0; j < n; ++j) {
        // G to W
        const float absG = fabsf(G[j]);
        int sign;
        if (G[j] < 0) { sign = -1; }
        else { sign = 1; }
        G[j] = (powf(10, absG * log10) - 1.0f) * sign;
        // W to RESP
        if (G[j] < 0) { sign = -1; }
        else { sign = 1; }
        G[j] = sign * lookup_table(LOOKUP_W_TO_R, W_table, R_table, fabsf(G[j]));
    }
}

// transform gradient (Gx,Gy) to R for the whole pyramid
inline void pyramid_transform_to_R(pyramid_t* pyramid, float detail_factor) {
    while (pyramid != NULL) {
        const int size = pyramid->rows * pyramid->cols;
        transform_to_R(size, pyramid->Gx, detail_factor);
        transform_to_R(size, pyramid->Gy, detail_factor);
        pyramid = pyramid->next;
    }
}

// transform from R to G
inline void transform_to_G(const int n, float*   R, float detail_factor) {
    const float log10 = 2.3025850929940456840179914546844 * detail_factor; //here we are actually changing the base of logarithm

    for (int j = 0; j < n; ++j) {
        // RESP to W
        int sign;
        if (R[j] < 0) {	sign = -1; }
        else { sign = 1; }
        R[j] = sign * lookup_table(LOOKUP_W_TO_R, R_table, W_table, fabsf(R[j]));
        // W to G
        if (R[j] < 0) {	sign = -1; }
        else { sign = 1; }
        R[j] = log(fabsf(R[j]) + 1.0f) / log10 * sign;
    }
}

// transform from R to G for the pyramid
inline void pyramid_transform_to_G(pyramid_t* pyramid, float detail_factor) {
    while (pyramid != NULL) {
        transform_to_G(pyramid->rows * pyramid->cols, pyramid->Gx, detail_factor);
        transform_to_G(pyramid->rows * pyramid->cols, pyramid->Gy, detail_factor);
        pyramid = pyramid->next;
    }
}

// multiply gradient (Gx,Gy) values by float scalar value for the whole pyramid
inline void pyramid_gradient_multiply(pyramid_t* pyramid, const float val) {
    while (pyramid != NULL) {
        matrix_multiply_const(pyramid->rows * pyramid->cols, pyramid->Gx, val);
        matrix_multiply_const(pyramid->rows * pyramid->cols, pyramid->Gy, val);
        pyramid = pyramid->next;
    }
}


int sort_float(const void* const v1, const void* const v2) {
    if (*((float*)v1) < *((float*)v2)) {
        return -1;
    }

    if (likely(*((float*)v1) > *((float*)v2))) {
        return 1;
    }

    return 0;
}


// transform gradients to luminance
void transform_to_luminance(pyramid_t* pp, float*   x, const bool bcg, const int itmax, const float tol) {
    pyramid_t* pC = pyramid_allocate(pp->cols, pp->rows);

    pyramid_calculate_scale_factor(pp, pC); // calculate (Cx,Cy)

    pyramid_scale_gradient(pp, pC); // scale small gradients by (Cx,Cy);

    float* b = matrix_alloc(pp->cols * pp->rows);

    pyramid_calculate_divergence_sum(pp, b); // calculate the sum of divergences (equal to b)

    MSG("  cg-lum-grad-solve");
    // calculate luminances from gradients
    if (bcg) {
        linbcg(pp, pC, b, x, itmax, tol);
    } else {
        lincg(pp, pC, b, x, itmax, tol);
    }

    matrix_free(b);
    pyramid_free(pC);
}


struct hist_data {
    float size;
    float cdf;
    int index;
};

int hist_data_order(const void* const v1, const void* const v2) {
    if (((struct hist_data*) v1)->size < ((struct hist_data*) v2)->size) {
        return -1;
    }

    if (((struct hist_data*) v1)->size > ((struct hist_data*) v2)->size) {
        return 1;
    }

    return 0;
}


int hist_data_index(const void* const v1, const void* const v2) {
    return ((struct hist_data*) v1)->index - ((struct hist_data*) v2)->index;
}


void contrast_equalization(pyramid_t* pp, const float contrastFactor) {
    // Count sizes
    int total_pixels = 0;
    pyramid_t* l = pp;
    while (l != NULL) {
        total_pixels += l->rows * l->cols;
        l = l->next;
    }

    // Allocate memory
    struct hist_data* hist = (struct hist_data*) malloc(sizeof(struct hist_data) * total_pixels);
    if (hist == NULL) {
        fprintf(stderr, "ERROR: malloc in contrast_equalization() (size:%zu)", sizeof(struct hist_data) * total_pixels);
        exit(155);
    }

    // Build histogram info
    l = pp;
    int index = 0;
    while (l != NULL) {
        const int pixels = l->rows * l->cols;
        const int offset = index;
        for (int c = 0; c < pixels; ++c) {
            hist[c+offset].size = sqrtf(l->Gx[c] * l->Gx[c] + l->Gy[c] * l->Gy[c]);
            hist[c+offset].index = c + offset;
        }
        index += pixels;
        l = l->next;
    }

    // Generate histogram
    qsort(hist, total_pixels, sizeof(struct hist_data), hist_data_order);

    // Calculate cdf
    const float norm = 1.0f / (float) total_pixels;

    for (int i = 0; i < total_pixels; ++i) {
        hist[i].cdf = ((float) i) * norm;
    }

    // Recalculate in terms of indexes
    qsort(hist, total_pixels, sizeof(struct hist_data), hist_data_index);

    //Remap gradient magnitudes
    l = pp;
    index = 0;
    while (l != NULL) {
        const int pixels = l->rows * l->cols;
        const int offset = index;

        for (int c = 0; c < pixels; ++c) {
            const float scale = contrastFactor * hist[c+offset].cdf / hist[c+offset].size;
            l->Gx[c] *= scale;
            l->Gy[c] *= scale;
        }
        index += pixels;
        l = l->next;
    }

    free(hist);
}


// tone mapping

/**
 * @brief: Tone mapping algorithm [Mantiuk2006]
 *
 * @param R red channel
 * @param G green channel
 * @param B blue channel
 * @param Y luminance channel
 * @param contrastFactor contrast scaling factor (in 0-1 range)
 * @param saturationFactor color desaturation (in 0-1 range)
 * @param bcg true if to use BiConjugate Gradients, false if to use Conjugate Gradients
 * @param itmax maximum number of iterations for convergence (typically 50)
 * @param tol tolerence to get within for convergence (typically 1e-3)
 * @return PFSTMO_OK if tone-mapping was sucessful, PFSTMO_ABORTED if
 * it was stopped from a callback function and PFSTMO_ERROR if an
 * error was encountered.
 */

int tmo_mantiuk06_contmap(int c, int r, float* R, float* G, float* B, float* Y,
                          const float contrastFactor, const float saturationFactor, float detailfactor,
                          const bool bcg, const int itmax, const float tol) {
    const int n = c * r ;

    MSG("  normalize");
    /* Normalize */

    float  Ymax = Y[0];
    for (int j = 1; j < n; ++j) {
        if (Y[j] > Ymax) {
            Ymax = Y[j];
        }
    }

    const float clip_min = 1e-6f * Ymax;
    for (int j = 0; j < n; ++j) {
        if (unlikely(R[j] < clip_min)) { R[j] = clip_min; }
        if (unlikely(G[j] < clip_min)) { G[j] = clip_min; }
        if (unlikely(B[j] < clip_min)) { B[j] = clip_min; }
        if (unlikely(Y[j] < clip_min)) { Y[j] = clip_min; }
    }

    for (int j = 0; j < n; ++j) {
        R[j] /= Y[j];
        G[j] /= Y[j];
        B[j] /= Y[j];
        Y[j] = log10f(Y[j]);
    }

    MSG("  gradient-pyramid");

    pyramid_t* pp = pyramid_allocate(c, r); // create pyramid
    float* tY = matrix_alloc(n);
    matrix_copy(n, Y, tY); // copy Y to tY
    pyramid_calculate_gradient(pp, tY); // calculate gradients for pyramid, destroys tY
    matrix_free(tY);
    pyramid_transform_to_R(pp, detailfactor); // transform gradients to R

    MSG("  contrast-pyramid");

    /* Contrast map */
    if (contrastFactor > 0.0f) {
        pyramid_gradient_multiply(pp, contrastFactor); // Contrast mapping
    } else {
        contrast_equalization(pp, -contrastFactor); // Contrast equalization
    }

    MSG("  pyramid-transform");

    pyramid_transform_to_G(pp, detailfactor);   // transform R to gradients

    MSG("  contrast-luminance");

    transform_to_luminance(pp, Y, bcg, itmax, tol); // transform gradients to luminance Y
    pyramid_free(pp);

    MSG("  re-normalize");

    /* Renormalize luminance */
    float* temp = matrix_alloc(n);

    matrix_copy(n, Y, temp);                    // copy Y to temp
    qsort(temp, n, sizeof(float), sort_float);  // sort temp in ascending order

    const float CUT_MARGIN = 0.1f;
    float trim;
    float delta;

    trim = (n - 1) * CUT_MARGIN * 0.01f;
    delta = trim - floorf(trim);
    const float l_min = temp[(int)floorf(trim)] * delta + temp[(int)ceilf(trim)] * (1.0f - delta);

    trim = (n - 1) * (100.0f - CUT_MARGIN) * 0.01f;
    delta = trim - floorf(trim);
    const float l_max = temp[(int)floorf(trim)] * delta + temp[(int)ceilf(trim)] * (1.0f - delta);

    matrix_free(temp);

    const float disp_dyn_range = 2.3f;

    for (int j = 0; j < n; ++j) {
        Y[j] = (Y[j] - l_min) / (l_max - l_min) * disp_dyn_range - disp_dyn_range;    // x scaled
    }

    MSG("  rgb convert");

    /* Transform to linear scale RGB */

    for (int j = 0; j < n; ++j) {
        Y[j] = powf(10, Y[j]);
        R[j] = powf(R[j], saturationFactor) * Y[j];
        G[j] = powf(G[j], saturationFactor) * Y[j];
        B[j] = powf(B[j], saturationFactor) * Y[j];
    }

    return 1;//PFSTMO_OK;
}


//#endif // __TMO_H__
