#pragma once
#include "stdafx.h"
#include "Threshold.h"
#include "KMean.h"

//--------------------------phân ngưỡng tĩnh------------------
void StaticThreshold::setThreshold(int low, int high) {
	_lowThreshold = low;
	_highThreshold = high;
}

int StaticThreshold::Apply(const Mat& srcImage, Mat &dstImage) {
	//trả về 0 nếu ảnh không tồn tại
	if (srcImage.data == NULL)
		return 0;

	dstImage = srcImage.clone(); //khởi tạo ảnh đích từ ảnh gốc
	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, dstImage, CV_BGR2GRAY); //chuyển ảnh màu về ảnh xám
	else
		dstImage = srcImage.clone();

	int width = dstImage.cols; //số cột của ảnh đích -> chiều rộng
	int height = dstImage.rows; //số hàng của ảnh đích -> chiều dài
	int nChannels = dstImage.channels(); //số kênh màu
	int widthStep = dstImage.step[0]; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	uchar* pDataDes = (uchar*)dstImage.data;  //pData là con trỏ quản lý vùng nhớ ảnh
	
	uchar groupOfObject[256] = { 0 };
	
	for (int y = 0; y < height; y++, pDataDes += widthStep) {
		uchar* pRowDes = pDataDes; //lấy con trỏ đầu mỗi dòng của ảnh đích
		for (int x = 0; x < width; x++, pRowDes += nChannels) {
			for (int i = _lowThreshold + 1; i < _highThreshold; i++)
				groupOfObject[i] = 255;			
			pRowDes[0] = groupOfObject[(int)pRowDes[0]];
		}
	}

	//trả về 0 nếu ảnh không thể tồn tại
	if (dstImage.data == NULL)
		return 0;

	return 1;
}

StaticThreshold::StaticThreshold() {}
StaticThreshold::~StaticThreshold() {}

//----------------Phân ngưỡng động theo trung bình------------------

//tạo ma trận ảnh
void createIntergral(const Mat& srcImage, Mat &intergral, int size, int k){
	//size: kích thước ma trận
	int width = srcImage.cols; //số cột của ảnh đích -> chiều rộng
	int height = srcImage.rows; //số dòng của ảnh đích -> chiều dài
	int widthStep = srcImage.step[0]; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	int nChannels = srcImage.channels(); //số kênh màu
	int half = size >> 1;
	int widthIntegral = width + half * 2 + 1;
	int heightIntegral = height + half * 2 + 1;

	//Khởi tạo intergral là ma trận 0 kích thước 2 height * 2 width 
	intergral = Mat::zeros(heightIntegral, widthIntegral, CV_32FC1);
	int widthStepIntegral = widthIntegral;
	uchar* pData = (uchar*)srcImage.data;  //pData là con trỏ quản lý vùng nhớ ảnh gốc
	float* pDataIntegral = (float*)intergral.data;  //pData là con trỏ quản lý vùng nhớ ảnh ma trận
	pDataIntegral += (half + 1) * widthStepIntegral + half + 1;

	int offsets[3] = { -1, -widthStepIntegral, -widthStepIntegral - 1 };
	int temp;

	for (int y = 0; y < height; y++, pData += widthStep, pDataIntegral += widthStepIntegral){
		float* pRowIntegral = pDataIntegral;
		uchar* pRowData = pData;
		for (int x = 0; x < width; x++, pRowData += nChannels, pRowIntegral += nChannels){
			temp = 1;
			for (int i = 0; i < k; i++)
				temp *= pRowData[0];
			pRowIntegral[0] = pRowIntegral[offsets[0]] + pRowIntegral[offsets[1]] - pRowIntegral[offsets[2]] + temp;
		}
	}
}

void AverageLocalThreshold::setC(int c) {
	_C = c;
}

int AverageLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize) {
	//trả về 0 nếu ảnh không tồn tại
	if (srcImage.data == NULL)
		return 0;

	Mat integral, gray;

	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, gray, CV_BGR2GRAY);
	else
		gray = srcImage.clone();
	dstImage.create(gray.rows, gray.cols, gray.type());

	createIntergral(gray, integral, winSize.width, 1); //tạo ma trận ảnh integral

	int width = dstImage.cols; //số cột của ảnh đích -> chiều rộng
	int height = dstImage.rows; //số hàng của ảnh đích -> chiều dài
	int nChannels = dstImage.channels(); //số kênh màu
	int widthStep = dstImage.step[0]; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	
	int widthIntegral = integral.cols; //số cột của ảnh ma trận -> chiều rộng
	int heightIntegral = integral.rows; //số dòng của ảnh ma trận -> chiều dài
	int widthStepIntegral = integral.cols; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp

	//tạo bảng offsets
	int halfWidth = winSize.width >> 1;
	int halfHeight = winSize.height >> 1;
	int n = winSize.width*winSize.height;
	float tb;
	vector<int> offsets;

	//tạo offsets cho ảnh
	for (int y = -halfHeight; y <= halfHeight; y++)
		for (int x = -halfWidth; x <= halfWidth; x++)
			offsets.push_back(y * widthStepIntegral + x);

	int pos[4];
	pos[0] = offsets[0] + (-widthStepIntegral - 1); //bên trái trên cùng
	pos[1] = offsets[winSize.width - 1] + (-widthStepIntegral); //bên phỉ trên cùng
	pos[2] = offsets[n - winSize.width] + (-1); //bên trái dưới cùng
	pos[3] = offsets[n - 1]; //bên trái dưới cùng

	uchar* pDataDes = (uchar*)dstImage.data;  //pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pDataGray = (uchar*)gray.data;  //pDataGray là con trỏ quản lý vùng nhớ ảnh
	float* pIntegral = (float*)integral.data;
	// dịch chuyển con trỏ data của  intergral tới ô (1, 1) ứng với ma trận
	pIntegral += (halfHeight + 1) * widthStepIntegral + (halfWidth + 1);
	
	for (int y = 0; y < height; y++, pDataDes += widthStep, pDataGray += widthStep,pIntegral+=widthStepIntegral) {
		uchar* pRowDes = pDataDes; //lấy con trỏ đầu mỗi dòng của ảnh đích 
		uchar* pRowGray = pDataGray; //lấy con trỏ đầu mỗi dòng của ảnh xám
		float* pRowIntegral = pIntegral;
		for (int x = 0; x < width; x++, pRowDes += nChannels, pRowGray += nChannels, pRowIntegral += nChannels) {
			tb = 0.0f;
			for (int i = 0; i < 4; i++) {
				if (i == 0 || i == 3)
					tb += pRowIntegral[pos[i]];
				else
					tb -= pRowIntegral[pos[i]];
			}
			tb /= n;
			pRowDes[0] = pRowGray[0] > (tb - _C) ? 255 : 0;
		}
	}

	gray.release();

	if (dstImage.data == NULL)
		return 0;

	return 1;
}

AverageLocalThreshold::AverageLocalThreshold(){
	_C = 0;
}

AverageLocalThreshold::~AverageLocalThreshold(){}
//-----------------Phân ngưỡng động theo trung vị----------------

//sắp xếp mảng tăng dần
void sort(uchar *&a, int n) {
	for (int i = 0; i < n - 1; i++)
		for (int j = i + 1; j < n; j++)
			if (a[i] > a[j]) {
				int temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
}

void MedianLocalThreshold::setC(int c) {
	_C = c;
}

int MedianLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize) {
	Mat gray;

	//trả về 0 nếu ảnh không tồn tại
	if (srcImage.data == NULL)
		return 0;

	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, gray, CV_BGR2GRAY); 
	else
		gray = srcImage.clone();
	dstImage.create(gray.rows, gray.cols, gray.type()); //tảo ảnh đích là ảnh xám

	int width = dstImage.cols; //số cột của ảnh gốc -> chiều rộng
	int height = dstImage.rows; //số hàng của ảnh gốc -> chiều dài
	int nChannels = dstImage.channels(); //số kênh màu
	int widthStep = dstImage.step[0]; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	uchar* pDataDes = (uchar*)dstImage.data;  //pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pDataGray = (uchar*)gray.data;  //pDataGray là con trỏ quản lý vùng nhớ ảnh

	//tạo bảng offsets
	int halfWidth = winSize.width >> 1;
	int halfHeight = winSize.height >> 1;
	int n = winSize.width*winSize.height;
	vector<int> offsets;

	for (int y = -halfHeight; y <= halfHeight; y++)
		for (int x = -halfWidth; x <= halfWidth; x++)
			offsets.push_back(y*widthStep + x);

	uchar *I = new uchar[n];
	
	for (int y = 0; y < height; y++, pDataDes += widthStep, pDataGray += widthStep) {
		uchar* pRowDes = pDataDes; //lấy con trỏ đầu mỗi dòng của ảnh đích
		uchar* pRowGray = pDataGray; //lấy con trỏ đầu mỗi dòng của ảnh xám
		for (int x = 0; x < width; x++, pRowDes += nChannels, pRowGray += nChannels) {
			for (int i = 0; i < n; i++) {
				I[i] = pRowGray[offsets[i]]; //tìm trung vị của từng điểm ảnh theo size
			}
			sort(I, n);
			//gán giá trị của điểm ảnh là giá trị trung vị
			pRowDes[0] = pRowGray[0] > ((int)I[n / 2] - _C) ? 255 : 0;
		}
	}

	gray.release();
	delete[] I;

	if (dstImage.data == NULL)
		return 0;

	return 1;
}

MedianLocalThreshold::MedianLocalThreshold(){
	_C = 0;
}
MedianLocalThreshold::~MedianLocalThreshold(){}

//-------------------Phân ngưỡng động Sauvola ------------------------
void SauvolaLocalThreshold::setValue(float k, int r) {
	_r = r;
	_k = k;
}

int SauvolaLocalThreshold::Apply(const Mat& srcImage, Mat &dstImage, Size winSize) {
	//trả về 0 nếu không tồn tại ảnh
	if (srcImage.data == NULL)
		return 0;

	Mat gray, integral, squareIntegral;
	const float byte2float = 1.0f;

	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, gray, CV_BGR2GRAY);
	else
		gray = srcImage.clone();
	dstImage.create(gray.rows, gray.cols, gray.type()); //khởi tạo ảnh đích là ảnh xám

	//tạo ma trận ảnh integral, squareIntegral
	createIntergral(gray, integral, winSize.width, 1);
	createIntergral(gray, squareIntegral, winSize.width, 2);

	int width = dstImage.cols; //số cột của ảnh gốc -> chiều rộng
	int height = dstImage.rows; //số hàng của ảnh gốc -> chiều dài
	int nChannels = dstImage.channels(); //số kênh màu
	int widthStep = dstImage.step[0]; //khoảng cách tính theo byte giữa 2 pixel cùng cột trên 2 dòng kế tiếp
	
	int widthIntegral = integral.cols;
	int heightIntegral = integral.rows;
	int widthStepIntegral = integral.cols;

	//tạo bảng offsets
	int halfWidth = winSize.width >> 1;
	int halfHeight = winSize.height >> 1;
	int n = winSize.width*winSize.height;
	vector<int> offsets, dx, dy;
	
	for (int y = -halfHeight; y <= halfHeight ; y++)
		for (int x = -halfWidth; x <= halfWidth; x++) {
			dy.push_back(x);
			dx.push_back(y);
			offsets.push_back(y*widthStepIntegral + x);
		}

	int pos[4];
	pos[0] = offsets[0] + (-widthStepIntegral - 1); //bến trài trên cùng
	pos[1] = offsets[winSize.width - 1] + (-widthStepIntegral); //bên phải trên cùng 
	pos[2] = offsets[n - winSize.width] + (-1); //bên trái dưới cùng
	pos[3] = offsets[n - 1]; //bên phải dưới cùng

	uchar* pDataDes = (uchar*)dstImage.data;  //pData là con trỏ quản lý vùng nhớ ảnh
	uchar* pDataGray = (uchar*)gray.data;  //pDataGray là con trỏ quản lý vùng nhớ ảnh
	float* pIntegral = (float*)integral.data;
	float* pSquareIntegral = (float*)squareIntegral.data;
	// dịch chuyển con trỏ data của  intergral tới ô(1, 1) ứng với ma trận
	pIntegral += (halfHeight + 1) * widthStepIntegral + (halfWidth + 1);
	pSquareIntegral += (halfHeight + 1) * widthStepIntegral + (halfWidth + 1);

	float mean, sqrtSum, DoLechChuan;
	for (int y = 0; y < height - halfHeight; y++, pDataDes += widthStep, pDataGray += widthStep, pIntegral += widthStepIntegral, pSquareIntegral += widthStepIntegral) {
		uchar* pRowDes = pDataDes;
		uchar* pRowGray = pDataGray;
		float* pRowIntegral = pIntegral;
		float* pRowSquareIntegral = pSquareIntegral;
		for (int x = 0; x < width - halfWidth; x++, pRowDes += nChannels, pRowGray += nChannels, pRowIntegral += nChannels, pRowSquareIntegral += nChannels) {
			mean = sqrtSum =  DoLechChuan = 0.0f;
			for (int i = 0; i < 4; i++) {
				if (i == 0 || i == 3) {
					mean += pRowIntegral[pos[i]];
					sqrtSum += pRowSquareIntegral[pos[i]];
				}
				else{
					mean -= pRowIntegral[pos[i]];
					sqrtSum -= pRowSquareIntegral[pos[i]];
				}
			}
			mean /= n;
			DoLechChuan = sqrtf((sqrtSum - n * mean*mean) / (n - 1));
			pRowDes[0] = pRowGray[0] > mean*(1 + _k * (DoLechChuan / _r - 1)) ? 255 : 0;
		}
	}

	//xử lý cho cạnh đáy ảnh
	for (int y = height - halfHeight; y < height; y++) {
		for (int x = 0; x < width; x++) {
			sqrtSum = mean = 0;
			float value;
			for (int i = 0; i < n; i++) {
				if (y + dx[i] >= 0 && y + dx[i] < height && x + dy[i] >= 0 && x + dy[i] < width) {
					value = gray.at<uchar>(y + dx[i], x + dy[i]);
					mean += value;
					sqrtSum += value * value;
				}
			}
			mean /= n;
			DoLechChuan = sqrtf((sqrtSum - n * mean*mean) / (n - 1));
			dstImage.at<uchar>(y, x) = value > mean*(1 + _k * (DoLechChuan / _r - 1)) ? 255 : 0;
		}
	}

	//xử lý cho cạnh bên phải ảnh
	for (int x = width - halfWidth; x < width; x++) {
		for (int y = 0; y < height; y++) {
			sqrtSum = mean = 0;
			float value;
			for (int i = 0; i < n; i++) {
				if (y + dx[i] >= 0 && y + dx[i] < height && x + dy[i] >= 0 && x + dy[i] < width) {
					value = gray.at<uchar>(y + dx[i], x + dy[i]);
					mean += value;
					sqrtSum += value * value;
				}
			}
			mean /= n;
			DoLechChuan = sqrtf((sqrtSum - n * mean*mean) / (n - 1));
			dstImage.at<uchar>(y, x) = value > mean*(1 + _k * (DoLechChuan / _r - 1)) ? 255 : 0;
		}
	}

	gray.release();
	if (dstImage.data == NULL)
		return 0;
	return 1;
}

SauvolaLocalThreshold::SauvolaLocalThreshold() {
	_k = 0.5;
	_r = 128;
}

SauvolaLocalThreshold::~SauvolaLocalThreshold(){}

//-----------------Phân đoạn theo K-means--------------------
struct Histogram{
	int count, id;
	void operator=(Histogram & his){
		int temp;
		temp = count;
		count = his.count;
		his.count = temp;
		temp = id;
		id = his.id;
		his.id = temp;
	}
};

void initClusters(const Mat& srcImage, Histogram his[], int nClusters, vector<Scalar> & clustersCenters, vector< vector<Point> > &ElementsOfNthCluster) {
	RNG random(getTickCount()); //khởi tạo lớp Random

	for (int i = 0; i < nClusters; i++) {
		//lấy ngẫu nhiên các điểm ảnh để khởi tạo centroid cluster
		Point centerPoint;
		centerPoint.x = random.uniform(0, srcImage.rows);
		centerPoint.y = random.uniform(0, srcImage.cols);
		Scalar centerPixel = srcImage.at<uchar>(centerPoint.x, centerPoint.y);

		//lấy giá trị màu của điểm ảnh
		Scalar center(centerPixel.val[0]);
		clustersCenters.push_back(center);

		vector<Point> Cluster;
		Cluster.push_back(centerPoint);
		ElementsOfNthCluster.push_back(Cluster);
	}
}

//Tính khoảng cách theo màu
double ColorDistance(Scalar pixel, Scalar clusterPixel){
	double delta = pixel.val[0] - clusterPixel.val[0];
	return sqrt(delta * delta);
}

double computeError(double pixel, double mean){
	double delta = pixel - mean;
	return delta * delta;
}

// Chia các điểm ảnh về các cluster tương ứng
void updateClusters(const Mat &srcImg, int nclusters, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster){
	double * W, value = 0;
	int height = srcImg.rows, width = srcImg.cols;

	W = new double[nclusters];
	for (int i = 0; i < nclusters; i++)
		W[i] = 0.0;

	//tại mỗi pixel, tìm cụm gần nó nhất
	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++){

			double minDistance = INFINITY; 
			int id = 0; // Lưu lại vị trí cluster mà điểm ảnh gần nó nhất

			Scalar pixel = srcImg.at<uchar>(y, x); // Lấy ra điểm ảnh tại vị trí (y, x)
			value = pixel.val[0];
			for (int i = 0; i < nclusters; i++){
				Scalar centroid = clustersCenters[i];
				// Tính khoảng cách màu của điểm ảnh với centroid
				double distance = ColorDistance(pixel, centroid);
				// Cập nhật
				if (distance < minDistance){
					minDistance = distance;
					id = i;
				}
			}
			// Cập nhật W[id] và Lưu điểm ảnh vào cluster id tương ứng
			W[id] += value;
			ElementsOfKthCluster[id].push_back(Point(y, x));
		}
	}
	//Cập nhật lại giá trị centroid của cluster
	for (int i = 0; i < nclusters; i++){
		W[i] /= ElementsOfKthCluster[i].size();
		clustersCenters[i].val[0] = W[i];
	}
}

// Cập nhật lại center của các Clusters
// Trả về giá trị là độ chênh lệch giữa 2 giá trị lỗi trước và hiện hành
double ErrorFunction(const Mat & srcImg, int nclusters, vector<Scalar> & clustersCenters, vector<vector<Point>> & ElementsOfKthCluster, double & oldError) {
	double diffChange;
	double newError = 0;
	// Tính lại Error
	for (int k = 0; k < nclusters; k++){
		vector<Point> KthCluster = ElementsOfKthCluster[k];
		double mean = clustersCenters[k].val[0];
		for (int i = 0; i < KthCluster.size(); i++)
			newError += computeError(srcImg.at<uchar>(KthCluster[i].x, KthCluster[i].y), mean);
	}
	// Tính độ chênh lệch giữa giá trị lỗi
	diffChange = abs(oldError - newError);
	//printf("Error change is: %0.4f\n", diffChange);
	oldError = newError;

	return diffChange;
}

void Kmean::setNumClusters(int num) {
	_numClusters = num;
}

int Kmean::Apply(const Mat& srcImage, Mat &dstImage) {
	Mat Gray;
	vector<Scalar> clustersCenters; // Danh sách các giá trị centroid của các clusters
	vector< vector<Point> > ElementsOfKthCluster; // Danh sách chứa các điểm thuộc Cluster thứ k
	Histogram his[256];
	double epsilon = 0.05;
	double oldError = INFINITY;
	double newError = 0;
	double diffChange = oldError - newError;
	
	if (srcImage.empty())
		return 0;
	if (srcImage.type() != CV_8UC1)
		cvtColor(srcImage, Gray, CV_BGR2GRAY);
	else
		Gray = srcImage.clone();
	imshow("Gray", Gray);
	dstImage.create(Gray.rows, Gray.cols, Gray.type());

	// Khởi tạo các centroids thuộc các Clusters
	initClusters(Gray, his, _numClusters, clustersCenters, ElementsOfKthCluster);

	// Lặp đến khi mức độ thay đổi nhỏ hơn epsilon
	while (diffChange > epsilon){
		for (int k = 0; k < _numClusters; k++)
			ElementsOfKthCluster[k].clear();

		// Cập nhật cluster và centroid
		updateClusters(Gray, _numClusters, clustersCenters, ElementsOfKthCluster);

		// Tính độ lệch hàm lỗi
		diffChange = ErrorFunction(srcImage, _numClusters, clustersCenters, ElementsOfKthCluster, oldError);
	}
	// Lưu ra ảnh đích
	for (int k = 0; k < _numClusters; k++){
		vector<Point> ClusterKth = ElementsOfKthCluster[k];
		int bin = 255 / _numClusters * (k + 1);
		//for each pixel in cluster change color to fit cluster
		for (int i = 0; i < ClusterKth.size(); i++)
			dstImage.at<uchar>(ClusterKth[i].x, ClusterKth[i].y) = clustersCenters[k].val[0];
	}
	Gray.release();
	if (dstImage.empty())
		return 0;
	return 1;
}

Kmean::Kmean() {
	_numClusters = 3;
}

Kmean::~Kmean(){}