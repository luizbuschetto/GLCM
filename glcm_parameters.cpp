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

// Some formulas: http://murphylab.web.cmu.edu/publications/boland/boland_node26.html

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
      cvtColor(img, img, CV_BGR2GRAY);
      glcm(img);
      // namedWindow("Image", WINDOW_NORMAL);
      // imshow("Image", img);
      // waitKey(0);
      return 0;
}

void glcm(Mat &img)
{
  int numLevels = 256;
  Mat gl = Mat::zeros(numLevels, numLevels, CV_32F);
  Mat imgCopy = Mat::zeros(img.rows, img.cols, img.type());

  img.copyTo(imgCopy);

  imgCopy.convertTo(img, CV_32F, (numLevels - 1)/255.5);

  for(int i = 0; i < img.rows; i++)
     for(int j = 0; j < img.cols; j++)
        img.at<float>(i,j) = round(img.at<float>(i,j)) + 1;

  //Creating GLCM matrix with 256 levels, radius=1 and in the horizontal direction
  for(int i = 0; i < img.rows; i++)
     for(int j = 0; j < img.cols - 1; j++)
         gl.at<float>(img.at<float>(i,j), img.at<float>(i,j+1)) = gl.at<float>(img.at<float>(i,j), img.at<float>(i,j+1)) + 1;

  //cout << gl << endl;

 // normalizing glcm matrix for parameter determination
  gl = gl + gl.t();
  gl = gl / sum(gl)[0];

  //Means
  float mu_i = 0, mu_j = 0;
  float mu = 0;
  for(int i = 0; i < numLevels; i++)
     for(int j = 0; j < numLevels; j++)
     {
        mu = mu + gl.at<float>(i,j);
        mu_i = mu_i + (i * gl.at<float>(i,j));
        mu_j = mu_j + (j * gl.at<float>(i,j));
     }

     mu = mu / pow(numLevels, 2);

   //Standard Deviation
   float sigma_i = 0, sigma_j = 0;
   vector<float> sum_pixels(2 * numLevels);    // sum_pixels[i + j] = p_{x + y}(k), where k = i + j
   vector<float> dif_pixels(2 * numLevels);    // dif_pixels[i + j] = p_{x - y}(k), where k = |i + j|
   vector<float> px(numLevels), py(numLevels); // Marginal-probability matrix obtained by summing the rows of p(i, j) (Matrix gl) [2]

   for(int i = 0; i < numLevels; i++)
      for(int j = 0; j < numLevels; j++)
      {
         sigma_i = sigma_i + ((i - mu_i) * (i - mu_i) * gl.at<float>(i,j));
         sigma_j = sigma_j + ((i - mu_j) * (i - mu_j) * gl.at<float>(i,j));
         //implementing savgh - Sum Average [1]
         if (i + j >= 2)
            sum_pixels[i + j] = sum_pixels[i + j] + gl.at<float>(i,j);

          dif_pixels[abs(i - j)] = dif_pixels[abs(i - j)] + gl.at<float>(i,j);
          px[j] = px[j] + gl.at<float>(i, j);
          py[i] = py[i] + gl.at<float>(i, j);
      }

      // float test1 = sigma_i;
      // float test2 = sigma_j;

    sigma_i = sqrt(sigma_i);
    sigma_j = sqrt(sigma_j);

    float savgh = 0, senth = 0;
    for(int i = 2; i < 2 * numLevels; i++)
    {
       savgh = savgh + (i * sum_pixels[i]);
       senth = senth - (sum_pixels[i] * log(sum_pixels[i] + 0.0000000000001)); //¹
    }

    float svarh = 0;
    for(int i = 2; i < 2 * numLevels; i++)
    {
        svarh = svarh + pow(i - senth, 2) * sum_pixels[i];
    }

    float dvarh = 0, denth = 0;
    for (int i = 0; i < numLevels; i++)
    {
        dvarh = dvarh + (pow(i, 2) * dif_pixels[i]);
        denth = denth - dif_pixels[i] * log(dif_pixels[i] + 0.0000000000001); //¹
    }

   float energ = 0, contr = 0, homom = 0, homop = 0, entro = 0, corrm = 0, corrp = 0,
        autoc = 0, cprom = 0, cshad = 0, dissi = 0, maxpr = 0, sosvh = 0, HX = 0,
        HY = 0, HXY = 0, HXY1 = 0, HXY2 = 0, inf1h = 0, inf2h = 0, indnc = 0, idmnc = 0;

   for(int i = 0; i < numLevels; i++)
   {
      HX = HX - px[i] * log(px[i] + 0.0000000000001);
      HY = HY - py[i] * log(py[i] + 0.0000000000001);

      for(int j = 0; j < numLevels; j++)
      {
          autoc = autoc + i * j * gl.at<float>(i,j);
          contr = contr + (abs(i-j) * abs(i-j) * gl.at<float>(i,j));
          corrm = corrm + (((i - mu_i) * (j - mu_j) * gl.at<float>(i,j)) / (sigma_i * sigma_j));
          cprom = cprom + pow((i + j - mu_i - mu_j), 4) * gl.at<float>(i, j);
          corrp = corrp + gl.at<float>(i,j) / (1 + pow(i - j, 2) / pow(numLevels, 2));
          cshad = cshad + pow((i + j - mu_i - mu_j), 3) * gl.at<float>(i, j);
          dissi = dissi + (abs(i - j) * gl.at<float>(i, j));
          energ = energ + gl.at<float>(i,j) * gl.at<float>(i,j);

          if(gl.at<float>(i,j) != 0)
            entro = entro - gl.at<float>(i,j) * log(gl.at<float>(i,j));

          homom = homom + gl.at<float>(i,j) / (1 + abs(i-j));
          homop = homop + (gl.at<float>(i,j) / (1 + ((i - j) * (i - j))));

          if (gl.at<float>(i, j) > maxpr)
              maxpr = gl.at<float>(i, j);

          sosvh = sosvh + (pow(i - mu, 2) * gl.at<float>(i, j));

          HXY = HXY - gl.at<float>(i, j) * log(gl.at<float>(i, j) + 0.0000000000001);
          HXY1 = HXY1 - gl.at<float>(i, j) * log(px[i] * py[j] + 0.0000000000001);
          HXY2 = HXY2 - px[i] * py[j] * log(px[i] * py[j] + 0.0000000000001);

          indnc = indnc + (gl.at<float>(i, j) / (1 + (pow(abs(i - j), 2)) / pow(numLevels, 2)));
          idmnc = idmnc + (gl.at<float>(i, j) / (1 + (pow(i - j, 2)) / pow(numLevels, 2)));
      }
    }

      float valueH = 0;

      if (HX >= HY)
        valueH = HX;
      else
        valueH = HY;

      inf1h = (HXY - HXY1) / valueH;
      inf2h = pow((1 - exp(-2 * (HXY2 - HXY))), 0.5);

   cout << "autoc = " << autoc << endl;   // Autocorrelation                            [2]
   cout << "contr = " << contr << endl;   // Contrast                                   [1,2]
   cout << "corrm = " << corrm << endl;   // Correlation                                [MATLAB]
   cout << "corrp = " << corrp << endl;   // Correlation                                [1, 2]
   cout << "cprom = " << cprom << endl;   // Cluster Prominence                         [2]
   cout << "cshad = " << cshad << endl;   // Cluster Shade                              [2]
   cout << "dissi = " << dissi << endl;   // Dissimilarity                              [2]
   cout << "energ = " << energ << endl;   // Energy                                     [1, 2]
   cout << "entro = " << entro << endl;   // Entropy                                    [2]
   cout << "homom = " << homom << endl;   // Inverse difference (INV)                   [3]
   cout << "homop = " << homop << endl;   // Homogenity/IDM                             [2]
   cout << "maxpr = " << maxpr << endl;   // Maximum probability                        [2]
   cout << "sosvh = " << sosvh << endl;   // Sum of Squares: Variance                   [1]
   cout << "senth = " << senth << endl;   // Sum entropy                                [1]
   cout << "savgh = " << savgh << endl;   // Sum Average                                [1]
   cout << "svarh = " << svarh << endl;   // Sum variance                               [1]
   cout << "dvarh = " << dvarh << endl;   // Difference variance                        [1]
   cout << "denth = " << denth << endl;   // Difference entropy                         [1]
   cout << "inf1h = " << inf1h << endl;   // Information measure of correlation1        [1]
   cout << "inf2h = " << inf2h << endl;   // Information measure of correlation2        [1]
   cout << "indnc = " << indnc << endl;   // Inverse difference normalized (INN)        [3]
   cout << "idmnc = " << idmnc << endl;   // Inverse difference moment normalized (IDN) [3]
}
