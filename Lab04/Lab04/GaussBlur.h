#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

class GaussBlur 
{
public:
	int BlurImageWithGauss(Mat& srcImage, Mat& destImage, int kWidth, int kHeight);
	GaussBlur();
	~GaussBlur();
};