#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;

class CannyEdgeDetector
{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;

public:
	/*
		Hàm áp dụng thuật toán Canny để phát hiện biên cạnh
		- srcImage: ảnh input
		- dstImage: ảnh kết quả
		Hàm trả về
			1: nếu detect thành công
			0: nếu detect không thành công
	*/
	void setThreshold(int low, int high);
	void HysteresisThresholding(const Mat& G, Mat& dstImage, int x, int y);
	int Apply(const Mat& srcImage, Mat &dstImage);

	CannyEdgeDetector();
	~CannyEdgeDetector();
};

