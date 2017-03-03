// %       GLCM Features (Soh, 1999; Haralick, 1973; Clausi 2002)
// %           f1. Uniformity / Energy / Angular Second Moment (done)
// %           f2. Entropy (done)
// %           f3. Dissimilarity
// %           f4. Contrast / Inertia (done)
// %           f5. Inverse difference
// %           f6. correlation (done)
// %           f7. Homogeneity / Inverse difference moment (done)
// %           f8. Autocorrelation
// %           f9. Cluster Shade
// %          f10. Cluster Prominence
// %          f11. Maximum probability
// %          f12. Sum of Squares
// %          f13. Sum Average
// %          f14. Sum Variance
// %          f15. Sum Entropy
// %          f16. Difference variance
// %          f17. Difference entropy
// %          f18. Information measures of correlation (1)
// %          f19. Information measures of correlation (2)
// %          f20. Maximal correlation coefficient
// %          f21. Inverse difference normalized (INN)
// %          f22. Inverse difference moment normalized (IDN)

#include <iostream>
#include <math.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

void glcm(Mat &);

int main()
{
      Mat img = imread("0.jpg"); //ERA 0
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

  //Means
  float mu_i = 0, mu_j = 0;
  float mu = 0;
  for(int i = 0; i < 256; i++)
     for(int j = 0; j < 256; j++)
     {
        mu = mu + gl.at<float>(i,j);
        mu_i = mu_i + (i * gl.at<float>(i,j));
        mu_j = mu_j + (j * gl.at<float>(i,j));
     }

     mu = mu / pow(256, 2);

   //Standard Deviation
   float sigma_i = 0, sigma_j = 0;
   for(int i = 0; i < 256; i++)
      for(int j = 0; j < 256; j++)
      {
         sigma_i = sigma_i + ((i - mu_i) * (i - mu_i) * gl.at<float>(i,j));
         sigma_j = sigma_j + ((i - mu_j) * (i - mu_j) * gl.at<float>(i,j));
      }

    sigma_i = sqrt(sigma_i);
    sigma_j = sqrt(sigma_j);


   float energ = 0, contr = 0, homom = 0, homop = 0, entro = 0, corrm = 0, autoc = 0,
         cprom = 0, cshad = 0, dissi = 0, maxpr = 0, sosvh = 0;

   for(int i = 0; i < 256; i++)
      for(int j = 0; j < 256; j++)
      {
          autoc = autoc + i * j * gl.at<float>(i,j);
          contr = contr + (abs(i-j) * abs(i-j) * gl.at<float>(i,j));
          corrm = corrm + (((i - mu_i) * (j - mu_j) * gl.at<float>(i,j)) / (sigma_i * sigma_j));
          cprom = cprom + pow((i + j - mu_i - mu_j), 4) * gl.at<float>(i, j);
          cshad = cshad + pow((i + j - mu_i - mu_j), 3) * gl.at<float>(i, j);
          dissi = dissi + (abs(i - j) * gl.at<float>(i, j));
          energ = energ + gl.at<float>(i,j) * gl.at<float>(i,j);

          if(gl.at<float>(i,j) != 0)
            entro = entro - gl.at<float>(i,j) * log(gl.at<float>(i,j));

          homom = homom + gl.at<float>(i,j) / (1 + abs(i-j));
          homop = homop + (gl.at<float>(i,j) / (1 + ((i - j) * (i - j))));

          if (gl.at<float>(i, j) > maxpr)
              maxpr = gl.at<float>(i, j);

          sosvh = sosvh + ((pow(i - mu, 2)) * gl.at<float>(i, j));
      }

   cout << "autoc = " << autoc << endl;   // Autocorrelation      [2]
   cout << "contr = " << contr << endl;   // Contrast             [1,2]
   cout << "corrm = " << corrm << endl;   // Correlation (MATLAB)
   cout << "cprom = " << cprom << endl;   // Cluster Prominence   [2]
   cout << "cshad = " << cshad << endl;   // Cluster Shade        [2]
   cout << "dissi = " << dissi << endl;   // Dissimilarity        [2]
   cout << "energ = " << energ << endl;   // Energy               [1,2]
   cout << "entro = " << entro << endl;   // Entropy              [2]
   cout << "homom = " << homom << endl;   // Homogenity (MATLAB)
   cout << "homop = " << homop << endl;   // Homogenity/IDM       [2]
   cout << "maxpr = " << maxpr << endl;   // Maximum probability  [2]
   cout << "sosvh = " << sosvh << endl;   // Sum of Squares: Variance [1]

}
