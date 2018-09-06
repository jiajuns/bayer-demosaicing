#include "Demosaic.h"
#include <cmath>

using namespace std;
using namespace cv;

/**
 * Initialize parameters needed to colorize a bayer mosaic image
 *
 * @param filePath file path to bayer mosaic image
 * @param color file path to the original color image
 */
Demosaic::Demosaic(string filePath) {
    Mat raw = imread(filePath, IMREAD_UNCHANGED);
    raw.convertTo(image, CV_32F);
    
    rows = image.rows;
    cols = image.cols;

    r = Mat::zeros(rows/2, cols/2, CV_32F);
    g = Mat::zeros(rows/2, cols/2, CV_32F);
    b = Mat::zeros(rows/2, cols/2, CV_32F);
    n = Mat::zeros(rows/2, cols/2, CV_32F);

    demosaicImageRGB = Mat::zeros(rows/2, cols/2, CV_32FC3);
    demosaicImageNir = Mat::zeros(rows/2, cols/2, CV_32F);
}

/**
 * Split bayer NBRG mosaic image into separate R, G, and B matrices
 */
void Demosaic::generateRGBComponents() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    // n Component
                    n.at<float>(i/2,j/2) = image.at<float>(i,j);
                }
                else {
                    // b Component
                    b.at<float>(i/2,j/2) = image.at<float>(i,j);
                }
            }
            else {
                if (j % 2 == 0) {
                    // R Component
                    r.at<float>(i/2,j/2) = image.at<float>(i,j);
                } else {
                    // G Component
                    g.at<float>(i/2,j/2) = image.at<float>(i,j);
                }
            }
        }
    }
}

/**
 * Interpolate the missing information for each of the R, G, and B component matrices
 */
void Demosaic::interpolate() {
    // kernel for N
    float kdata1[] = {
        1./16., 3./16.,
        3./16., 9./16.
    };

    // kernel for B
    float kdata2[] = {
        3./16., 1./16.,
        9./16., 3./16.
    };

    // kernel for R
    float kdata3[] = {
        3./16., 9./16.,
        1./16., 3./16.
    };

    // kernel for G
    float kdata4[] = {
        9./16., 3./16.,
        3./16., 1./16.
    };

    Mat k1 = Mat(2, 2, CV_32F, kdata1);
    Mat k2 = Mat(2, 2, CV_32F, kdata2);
    Mat k3 = Mat(2, 2, CV_32F, kdata3);
    Mat k4 = Mat(2, 2, CV_32F, kdata4);

    Mat n2 = Mat(rows/2, cols/2, CV_32F);
    Mat b2 = Mat(rows/2, cols/2, CV_32F);
    Mat r2 = Mat(rows/2, cols/2, CV_32F);
    Mat g2 = Mat(rows/2, cols/2, CV_32F);

    filter2D(n, n2, -1, k1);
    filter2D(b, b2, -1, k2);
    filter2D(r, r2, -1, k3);
    filter2D(g, g2, -1, k4);
    n = n2;
    b = b2;
    r = r2;
    g = g2;
}

/**
 * Combine R, G, and B component matrices into a single three channel color image
 */
void Demosaic::colorize() {
    for (int i = 0; i < rows/2; i++) {
        for (int j = 0; j < cols/2; j++) {
            demosaicImageRGB.at<Vec3f>(i, j) = Vec3f(
                    b.at<float>(i,j),
                    g.at<float>(i,j),
                    r.at<float>(i,j)
            );
        }
    }
    demosaicImageNir = n;
}

/**
 * Display results of the demosaicing process
 */
void Demosaic::display() {
    Mat d8u, n8u;
    demosaicImageRGB = demosaicImageRGB / 4;
    demosaicImageRGB.convertTo(d8u, CV_8UC3);
    imshow("Demosaic", d8u);
    waitKey(0);

    demosaicImageNir = demosaicImageNir / 4;
    demosaicImageNir.convertTo(n8u, CV_8UC3);
    imshow("Demosaic", n8u);
    waitKey(0);
}
