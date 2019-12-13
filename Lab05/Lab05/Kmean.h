#pragma once
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "opencv2/highgui/highgui_c.h"

using namespace cv;
using namespace std;

class Kmean{
	//số cụm K
	int _numClusters;
public:
	/*
	Hàm áp dụng thuật toán Kmeans để phân đoạn ảnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân đoạn thành công
	0: nếu phân đoạn không thành công
	*/

	void setNumClusters(int num);
	int Apply(const Mat& srcImage, Mat &dstImage);

	Kmean();
	~Kmean();
};

