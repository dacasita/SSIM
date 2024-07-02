#include <opencv2/opencv.hpp>

using namespace cv;

  // Function to changeIntensity (y = 255*(x/255)^alpha)
  uchar neg(uchar pixel) {
    return 255 - pixel;
  }

int main() {
  // Read the grayscale image
  Mat img = imread("lennagrey.bmp", 0);

  // Check if image is loaded successfully
  if (!img.data) {
    std::cout << "Error: Could not open image!" << std::endl;
    return -1;
  }

  cv::Mat differentIntensity(img.rows, img.cols, CV_8UC1, cv::Scalar(0));

  // Apply transformation to each pixel
  for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
      differentIntensity.at<uchar>(i, j) = neg(img.at<uchar>(i, j));
    }
  }

  double M = img.rows;
  double N = img.cols;
  double reciprocal_MN = 1 / (M * N);
  double uX = 0;
  double uY = 0;

  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      uX += img.at<uchar>(i, j);
      uY += differentIntensity.at<uchar>(i, j);
    }
  }
  uX *= reciprocal_MN;
  uY *= reciprocal_MN;

  double varX = 0;
  double coVarXy = 0;
  double varY = 0;
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      double diffMeanX = img.at<uchar>(i, j) - uX;
      varX += pow(diffMeanX, 2);
      double diffMeanY = differentIntensity.at<uchar>(i, j)  - uY;
      varY += pow(diffMeanY, 2);
      coVarXy += diffMeanX * diffMeanY;
    }
  }
  varX *= reciprocal_MN;
  coVarXy *= reciprocal_MN;
  varY *= reciprocal_MN;
  
  double L = 255;
  double c1 = sqrt(1/L);
  double c2 = sqrt(1/L);
  double c1Lsquare = pow(c1 * L,2);
  double c2Lsquare = pow(c2 * L,2);
  double SSIM = (2 * uX * uY + c1Lsquare) * (2 * coVarXy + c2Lsquare) / ((pow(uX , 2) + pow(uY , 2) + c1Lsquare) * (varX + varY + c2Lsquare));

  printf("SSIM = %lf \n\n", SSIM);
  /*
  double NRMSE = sqrt( sum_square_error /  Sxx );
  double PSNR = 10 * log10( pow(xMax, 2)* M * N / sum_square_error );

printf("sum_square_error = %lf \n\n", sum_square_error);
printf("Sxx = %lf \n\n", Sxx);
printf("rows = %d \n", M);
printf("NRMSE = %lf \n\n", NRMSE);
printf("PSNR = %lf \n\n", PSNR);
*/
    // Display the image change intensity
  cv::imshow("differentIntensity", differentIntensity);

  cv::imshow("Origin", img);

    //cv::Mat imgOrigin = img;
    //cv::imshow("Origin", imgOrigin); 
    cv::waitKey(0); 
    cv::destroyAllWindows(); 

  return 0;
}