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

//phân ngưỡng tĩnh
class StaticThreshold{
	//ngưỡng dưới
	int _lowThreshold;
	//ngưỡng trên
	int _highThreshold;
public:
	/*
	Hàm áp dụng phân ngưỡng tĩnh
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	void setThreshold(int low, int up);
	int Apply(const Mat& srcImage, Mat &dstImage);

	StaticThreshold();
	~StaticThreshold();
};

//phân ngưỡng cục bộ dựa vào trung bình
class AverageLocalThreshold{
	//hệ số C
	int _C;
public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ theo trung bình
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
		1: nếu phân ngưỡng thành công
		0: nếu phân ngưỡng không thành công
	*/

	void setC(int c);
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);

	AverageLocalThreshold();
	~AverageLocalThreshold();
};

//phân ngưỡng cục bộ dựa vào trung vị
class MedianLocalThreshold{
	//hệ số C
	int _C;

public:
	/*
	Hàm áp dụng phân ngưỡng cục bộ dựa vào trung vị
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	void setC(int c);
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);

	MedianLocalThreshold();
	~MedianLocalThreshold();
};

//phân ngưỡng cục bộ dựa vào thuật toán Sauvola
class SauvolaLocalThreshold{
	//hệ số r
	int _r;
	//hệ số k
	float _k;
public:
	/*
	Hàm áp dụng thuật toán Sauvola để phân ngưỡng
	- srcImage: ảnh input
	- dstImage: ảnh kết quả
	- winSize: kích thước lân cận
	Hàm trả về
	1: nếu phân ngưỡng thành công
	0: nếu phân ngưỡng không thành công
	*/

	void setValue(float k, int r);
	int Apply(const Mat& srcImage, Mat &dstImage, Size winSize);

	SauvolaLocalThreshold();
	~SauvolaLocalThreshold();
};

