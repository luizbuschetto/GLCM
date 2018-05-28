<h1>Gray-Level Co-Occurence Matrix (GLCM)</h1>
This is the implementation of Gray-Level Co-Occurence Matrix (GLCM) in C/C++ (OpenCV).  It was compiled using OpenCV 3.4.0, in a Ubuntu based distro. <br>

This algorithm extracts 22 features and was proposed by Avinash Uppuluri. The original code (in MATLAB) can be found in:
 https://www.mathworks.com/matlabcentral/fileexchange/22187-glcm-texture-features

The 22 features¹ are:

Autocorrelation: [2]                      				(autoc)<br>
Contrast: MATLAB/[1,2]                    			(contr)<br>
Correlation: MATLAB                       				(corrm)<br>
Correlation: [1,2]                        				(corrp)<br>
Cluster Prominence: [2]                   			(cprom)<br>
Cluster Shade: [2]                       	 			(cshad)<br>
Dissimilarity: [2]                        					(dissi)<br>
Energy: MATLAB / [1,2]                    				(energ)<br>
Entropy: [2]                              					(entro)<br>
Homogeneity: MATLAB                       			(homom)<br>
Homogeneity: [2]                          				(homop)<br>
Maximum probability: [2]                  			(maxpr)<br>
Sum of sqaures: Variance [1]              			(sosvh)<br>
Sum average [1]                           				(savgh)<br>
Sum variance [1]                         			 	(svarh)<br>
Sum entropy [1]                          				(senth)<br>
Difference variance [1]                   				(dvarh)<br>
Difference entropy [1]                    				(denth)<br>
Information measure of correlation1 [1]   	(inf1h)<br>
Informaiton measure of correlation2 [1]   	(inf2h)<br>
Inverse difference normalized (INN) [3]   		(indnc)<br>
Inverse difference moment normalized [3]  	(idmnc)<br>

¹ The number inside the brackets shows where the formula can be found (in the papers below). 

<h3>References:</h3>
[1] R. M. Haralick, K. Shanmugam, and I. Dinstein, Textural Features of Image Classification, IEEE Transactions on Systems, Man and Cybernetics, vol. SMC-3, no. 6, Nov. 1973;<br>
[2] L. Soh and C. Tsatsoulis, Texture Analysis of SAR Sea Ice Imagery Using Gray Level Co-Occurrence Matrices, IEEE Transactions on Geoscience and Remote Sensing, vol. 37, no. 2, March 1999;<br>
[3] D A. Clausi, An analysis of co-occurrence texture statistics as a function of grey level quantization, Can. J. Remote Sensing, vol. 28, no.1, pp. 45-62, 2002;<br>
The features with "MATLAB" uses the definition proposed by the software and can be found here: https://www.mathworks.com/help/images/ref/graycoprops.html 

<h2>How to use:</h2>
<i> ./main image_path number_of_levels </i>

Example: <i> ./main 0.jpg 8 </i>

The algorithm will extract the features from "0.jpg" with numLevels = 8.
The second parameter (number of levels) is optional. If you don't want to specify this number, the image will be processed with the default value: 256 levels.
