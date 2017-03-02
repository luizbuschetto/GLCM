#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void glcm(Mat &);

int main()
{
      Mat img = imread("/home/luiz/Desktop/0.jpg"); //ERA 0
      if(img.empty())
      {
         cout << "No image!";
         return 0;
      }
      cvtColor(img, img, CV_RGB2GRAY);
      glcm(img);
      // namedWindow("Image", WINDOW_NORMAL);
      // imshow("Image", img);
      // waitKey(0);
      return 0;
}

void glcm(Mat &img)
{
  Mat gl = Mat::zeros(256, 256, CV_32FC1);

  //Creating GLCM matrix with 256 levels, radius=1 and in the horizontal direction
  for(int i = 0; i < img.rows; i++)
     for(int j = 0; j < img.cols - 1; j++)
         gl.at<float>(img.at<uchar>(i,j), img.at<uchar>(i,j+1)) = gl.at<float>(img.at<uchar>(i,j), img.at<uchar>(i,j+1)) + 1;

 // normalizing glcm matrix for parameter determination
  gl = gl + gl.t();
  gl = gl / sum(gl)[0];

  //Mean: Surface Defect Isolation in Ceramic Tile Based on Texture Feature Analysis Using Radon Transform and FCM
  float mu_i = 0, mu_j = 0;
  for(int i = 0; i < 256; i++)
     for(int j = 0; j < 256; j++)
     {
        mu_i = mu_i + (i * gl.at<float>(i,j));
        mu_j = mu_j + (j * gl.at<float>(i,j));
     }

   //Standard Deviation: http://www.fp.ucalgary.ca/mhallbey/glcm_variance.htm
   float sigma_i = 0, sigma_j = 0;
   for(int i = 0; i < 256; i++)
      for(int j = 0; j < 256; j++)
      {
         sigma_i = sigma_i + ((i - mu_i) * (i - mu_i) * gl.at<float>(i,j));
         sigma_j = sigma_j + ((i - mu_j) * (i - mu_j) * gl.at<float>(i,j));
      }

    sigma_i = sqrt(sigma_i);
    sigma_j = sqrt(sigma_j);

   //Equations: http://www.fp.ucalgary.ca/mhallbey/equations.htm
   float energy = 0, contrast = 0, homogenity = 0, IDM = 0, entropy = 0, mean = 0, correlation = 0;
   for(int i = 0; i < 256; i++)
      for(int j = 0; j < 256; j++)
      {
          energy = energy + gl.at<float>(i,j) * gl.at<float>(i,j);
          contrast = contrast + (abs(i-j) * abs(i-j) * gl.at<float>(i,j));
          homogenity = homogenity + gl.at<float>(i,j) / (1 + abs(i-j));
          mean = mean + 0.5 * (i * gl.at<float>(i,j) + j * gl.at<float>(i,j));
          correlation = correlation + (((i - mu_i) * (j - mu_j) * gl.at<float>(i,j)) / (sigma_i * sigma_j));

          if(i != j)
            //IDM = IDM + gl.at<float>(i,j) / ((i-j) * (i-j)); //Taking k=2;
            IDM = IDM + (gl.at<float>(i,j) / abs(i-j));

          if(gl.at<float>(i,j) != 0)
            entropy = entropy - gl.at<float>(i,j) * log(gl.at<float>(i,j));
      }

   cout << "Contrast = " << contrast << endl;
   cout << "Correlation = " << correlation << endl;
   cout << "Energy = " << energy << endl;
   cout << "Homogenity = " << homogenity << endl;
   // cout << "IDM = " << IDM << endl;                     //Inverse difference moment
   cout << "Entropy = " << entropy << endl;
   // cout << "Mean = " << mean << endl;

}
