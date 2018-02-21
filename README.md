<h1>Gray-Level Co-Occurence Matrix (GLCM)</h1>
This is the implementation of Gray-Level Co-Occurence Matrix (GLCM) in C/C++ (OpenCV).  It was compiled using OpenCV 3.4.0, in a Ubuntu based distro.

This algorithm extracts 23 features and it was proposed by Avinash Uppuluri. The original code (in MATLAB) can be found in:
 https://www.mathworks.com/matlabcentral/fileexchange/22187-glcm-texture-features

The 23 features¹ are:

Autocorrelation: [2]                      				(autoc)
Contrast: MATLAB/[1,2]                    			(contr)
Correlation: MATLAB                       				(corrm)
Correlation: [1,2]                        				(corrp)
Cluster Prominence: [2]                   			(cprom)
Cluster Shade: [2]                       	 			(cshad)
Dissimilarity: [2]                        					(dissi)
Energy: MATLAB / [1,2]                    				(energ)
Entropy: [2]                              					(entro)
Homogeneity: MATLAB                       			(homom)
Homogeneity: [2]                          				(homop)
Maximum probability: [2]                  			(maxpr)
Sum of sqaures: Variance [1]              			(sosvh)
Sum average [1]                           				(savgh)
Sum variance [1]                         			 	(svarh)
Sum entropy [1]                          				(senth)
Difference variance [1]                   				(dvarh)
Difference entropy [1]                    				(denth)
Information measure of correlation1 [1]   	(inf1h)
Informaiton measure of correlation2 [1]   	(inf2h)
Inverse difference (INV) is homom [3]     		(homom)
Inverse difference normalized (INN) [3]   		(indnc) 
Inverse difference moment normalized [3]  	(idmnc)

¹ The number inside de brackets shows where the formula can be found (the papers below are included in this repository). 

<h3>References:</h3>
[1.] R. M. Haralick, K. Shanmugam, and I. Dinstein, Textural Features of Image Classification, IEEE Transactions on Systems, Man and Cybernetics, vol. SMC-3, no. 6, Nov. 1973;
[2] L. Soh and C. Tsatsoulis, Texture Analysis of SAR Sea Ice Imagery Using Gray Level Co-Occurrence Matrices, IEEE Transactions on Geoscience and Remote Sensing, vol. 37, no. 2, March 1999;
[3] D A. Clausi, An analysis of co-occurrence texture statistics as a function of grey level quantization, Can. J. Remote Sensing, vol. 28, no.1, pp. 45-62, 2002. 
[MATLAB] https://www.mathworks.com/help/images/ref/graycoprops.html

<h2>How to use:</h2>
<i> ./main <image path> <number of levels> </i>

Example: <i> ./main 0.jpg 8 </i>

The algorithm will extract the features from "0.jpg" with numLevels = 8.
The second parameter (number of levels) is optional. If you don't want to specify this number, the image will be processed with the default value: 256 levels.
