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

void glcm(Mat &img, int numLevels);

int main(int argc, char* argv[])
{
  Mat img = imread(argv[1]);

  if(img.empty())
  {
    cout << "No image!";
    return 0;
  }

  cvtColor(img, img, CV_BGR2GRAY);

  int numLevels = 256;
  if (argc > 2)
  {
    numLevels = atoi(argv[2]);
  }

  glcm(img, numLevels);

  return 0;
}

void glcm(Mat &img, int numLevels)
{

  Mat glcm = Mat::zeros(numLevels, numLevels, CV_32F);

  // Normalizing
  float slope = float(numLevels) / 255; // 255 is the max pixel value for the image type
  float intercept = 1 - (slope * 0);    // 0 is the min pixel value for the image type

  for(int i = 0; i < img.rows; i++){
    for(int j = 0; j < img.cols; j++){
      img.at<uchar>(i,j) = floor((slope * img.at<uchar>(i,j)) + 1);
    }
  }

  // Creating GLCM matrix with "numLevels", radius = 1 and in the horizontal direction
  for(int i = 0; i < img.rows; i++){
    for(int j = 0; j < img.cols-1; j++)
    {
      glcm.at<float>(img.at<uchar>(i,j) - 1, img.at<uchar>(i,j+1) - 1) += 1;
    }      
  }

  // Normalizing GLCM matrix for parameter determination
  glcm = glcm + glcm.t();
  glcm = glcm / sum(glcm)[0];

  // Means
  float mu_i = 0, mu_j = 0;
  float mu = 0;
  for(int i = 0; i < numLevels; i++){
    for(int j = 0; j < numLevels; j++)
    {
      mu = mu + glcm.at<float>(i,j);
      mu_i = mu_i + ((i+1) * glcm.at<float>(i,j));
      mu_j = mu_j + ((j+1) * glcm.at<float>(i,j));
    }
  }

  mu = mu / pow(numLevels, 2);

  // Standard Deviation
  float sigma_i = 0, sigma_j = 0;
  vector<float> sum_pixels((2 * numLevels)-1);    // sum_pixels[i + j] = p_{x + y}(k), where k = i + j
  vector<float> dif_pixels(2 * numLevels);    // dif_pixels[i + j] = p_{x - y}(k), where k = |i + j|
  vector<float> px(numLevels), py(numLevels); // Marginal-probability matrix obtained by summing the rows of p(i, j) (Matrix gl) [2]

  for(int i = 0; i < numLevels; i++)
  {
    for(int j = 0; j < numLevels; j++)
    {
      sigma_i = sigma_i + (((i+1) - mu_i) * ((i+1) - mu_i) * glcm.at<float>(i,j));
      sigma_j = sigma_j + (((i+1) - mu_j) * ((i+1) - mu_j) * glcm.at<float>(i,j));

      // Implementing savgh - Sum Average [1]
      if (i + j >= 2)
        sum_pixels[i + j] = sum_pixels[i + j] + glcm.at<float>(i,j);

      dif_pixels[abs(i - j)] = dif_pixels[abs(i - j)] + glcm.at<float>(i,j);
      px[j] = px[j] + glcm.at<float>(i,j);
      py[i] = py[i] + glcm.at<float>(i,j);
    }
  }

  sigma_i = sqrt(sigma_i);
  sigma_j = sqrt(sigma_j);

  // for(int i = 0; i < (2*numLevels)-1; i++)
  //     cout << sum_pixels[i] << endl;

  float savgh = 0, senth = 0;
  for(int i = 0; i < (2 * numLevels) - 1; i++)
  {
    savgh = savgh + ((i+2) * sum_pixels[i]); // +2 because matlab
    senth = senth - (sum_pixels[i] * log(sum_pixels[i] + 0.0000000000001)); //¹
  }

  float svarh = 0;
  for(int i = 0; i < (2 * numLevels) - 1; i++)
  {
    svarh = svarh + pow((i+2) - senth, 2) * sum_pixels[i]; // +2 because matlab
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
      autoc = autoc + (i+1) * (j+1) * glcm.at<float>(i,j); //+1 because of MATLAB's notation
      contr = contr + (abs(i-j) * abs(i-j) * glcm.at<float>(i,j));
      corrm = corrm + (((i - mu_i) * (j - mu_j) * glcm.at<float>(i,j)) / (sigma_i * sigma_j));
      cprom = cprom + pow((i + j - mu_i - mu_j), 4) * glcm.at<float>(i,j);
      cshad = cshad + pow((i + j - mu_i - mu_j), 3) * glcm.at<float>(i,j);
      dissi = dissi + (abs(i - j) * glcm.at<float>(i,j));
      energ = energ + glcm.at<float>(i,j) * glcm.at<float>(i,j);

      if(glcm.at<float>(i,j) != 0)
        entro = entro - glcm.at<float>(i,j) * log(glcm.at<float>(i,j));

      homom = homom + glcm.at<float>(i,j) / (1 + abs(i-j));
      homop = homop + (glcm.at<float>(i,j) / (1 + ((i - j) * (i - j))));

      if (glcm.at<float>(i,j) > maxpr)
        maxpr = glcm.at<float>(i,j);

      sosvh = sosvh + (pow((i+1) - mu, 2) * glcm.at<float>(i,j));

      HXY = HXY - glcm.at<float>(i,j) * log(glcm.at<float>(i,j) + 0.0000000000001);
      HXY1 = HXY1 - glcm.at<float>(i,j) * log(px[i] * py[j] + 0.0000000000001);
      HXY2 = HXY2 - px[i] * py[j] * log(px[i] * py[j] + 0.0000000000001);

      indnc = indnc + (glcm.at<float>(i,j) / (1 + (pow(abs(i - j), 2)) / pow(numLevels, 2)));
      idmnc = idmnc + (glcm.at<float>(i,j) / (1 + (pow(i - j, 2)) / pow(numLevels, 2)));
    }
  }

  cout << autoc << endl;
  cout << mu_i << endl;
  cout << mu_j << endl;
  cout << sigma_i << endl;
  cout << sigma_j << endl;
  corrp = (autoc - mu_i * mu_j) / (sigma_i * sigma_j);

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
  cout << "savgh = " << savgh << endl;   // Sum Average                                [1]
  cout << "svarh = " << svarh << endl;   // Sum variance                               [1]
  cout << "senth = " << senth << endl;   // Sum entropy                                [1]
  cout << "dvarh = " << dvarh << endl;   // Difference variance                        [1]
  cout << "denth = " << denth << endl;   // Difference entropy                         [1]
  cout << "inf1h = " << inf1h << endl;   // Information measure of correlation1        [1]
  cout << "inf2h = " << inf2h << endl;   // Information measure of correlation2        [1]
  cout << "indnc = " << indnc << endl;   // Inverse difference normalized (INN)        [3]
  cout << "idmnc = " << idmnc << endl;   // Inverse difference moment normalized (IDN) [3]
}
