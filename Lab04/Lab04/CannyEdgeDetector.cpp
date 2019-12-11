#include "stdafx.h"
#include "CannyEdgeDetector.h"
#include "Convolution.h"
#include "GaussBlur.h"
#include "Converter.h"

// Hàm đệ quy phân ngưỡng
void CannyEdgeDetector::HysteresisThresholding(const Mat& G, Mat& dstImage, int x, int y)
{
	// Kiểm tra xem x, y có vượt quá giới hạn không
	if (x < 0 || y < 0 || x >= dstImage.rows || y >= dstImage.cols || dstImage.at<uchar>(x, y) != 0)
		return;
	//G[x][y] đã bị loại bỏ hay dstImage[x][y] đã được đi qua
	if (G.at<float>(x, y) < _lowThreshold)
		return;
	dstImage.at<uchar>(x, y) = 255;
	// Hysteresis Threshold theo 8 hướng (lân cận 8)
	HysteresisThresholding(G, dstImage, x - 1, y - 1);
	HysteresisThresholding(G, dstImage, x - 1, y);
	HysteresisThresholding(G, dstImage, x - 1, y + 1);
	HysteresisThresholding(G, dstImage, x, y - 1);
	HysteresisThresholding(G, dstImage, x, y + 1);
	HysteresisThresholding(G, dstImage, x + 1, y - 1);
	HysteresisThresholding(G, dstImage, x + 1, y);
	HysteresisThresholding(G, dstImage, x + 1, y + 1);
}

int CannyEdgeDetector::Apply(const Mat& srcImage, Mat &dstImage)
{
	if (srcImage.empty())
		return 0;

	GaussBlur Gauss;
	Converter cvt;
	Mat GrayImage, Temp1, Temp2;
	srcImage.copyTo(Temp1);

	//B1: Giảm nhiễu
	//Nếu là ảnh màu thì chuyển sang ảnh xám
	if (srcImage.channels() != 1)
		cvt.Convert(Temp1, Temp2, 0);
	else
		srcImage.convertTo(Temp2, CV_8UC1);

	//Làm trơn ảnh với Gaussian có kích thước bộ lọc là 5x5
	Gauss.BlurImageWithGauss(Temp2, GrayImage, 5, 5);
	dstImage = Mat::zeros(srcImage.rows, srcImage.cols, CV_8UC1);

	//Lấy ra các thông số
	//Rộng
	int width = dstImage.cols;
	//Cao
	int height = dstImage.rows;
	//Kênh màu
	int nChannels = dstImage.channels();
	//Khoảng cách 2 điểm ảnh
	int widthStep = dstImage.step[0];

	// Khởi tạo offsets
	vector<int> offset;
	int kHalfWidth = 3 >> 1;
	int kHalfHeight = 3 >> 1;
	for (int y = -kHalfHeight; y <= kHalfHeight; y++)
		for (int x = -kHalfWidth; x <= kHalfWidth; x++)
			offset.push_back(y * widthStep + x);

	//B2: Tìm Gradient
	//Ma trận của bộ lọc Sobel
	float Wx[9] = { -1, 0, 1,
				  -2, 0, 2,
				  -1, 0, 1 };
	float Wy[9] = { 1, 2, 1,
				  0, 0, 0,
				 -1, -2, -1 };

	Mat Gx, Gy, G, Mark;
	//Khởi tạo 3 ma trận đạo hàm theo x, theo y và gradient với bộ lọc sobel
	Gx.create(height, width, CV_32FC1);
	Gy.create(height, width, CV_32FC1);
	G.create(height, width, CV_32FC1);
	//Ma trận đánh dấu
	Mark.create(height, width, GrayImage.type());

	//Con trỏ vùng nhớ ảnh xám
	uchar *pGray = (uchar *)GrayImage.data, *pGrayRow;
	//Con trỏ vùng nhớ ảnh đích
	uchar *pData = (uchar *)dstImage.data, *pRow;

	float Sx, Sy, S;
	int n = 9;
	for (int i = 0; i < height; i++, pGray += widthStep)
	{
		pGrayRow = pGray;
		for (int j = 0; j < width; j++, pGrayRow += nChannels)
		{
			Sx = Sy = 0;
			for (int k = 0; k < n; k++)
			{
				//Tính đạo hàm theo x, y của ảnh xám
				Sx += pGrayRow[offset[k]] * Wx[n - 1 - k];
				Sy += pGrayRow[offset[k]] * Wy[n - 1 - k];
			}
			/*Các biên độ gradient sau đó có thể được xác định như là 
			một thước đo khoảng cách Euclide bằng cách áp dụng luật của Pythago*/
			S = hypot(Sx, Sy); // S = sqrt(Sx^2 + Sy^2):
			Gx.at<float>(i, j) = Sx;
			Gy.at<float>(i, j) = Sy;
			G.at<float>(i, j) = S;
		}
	}
	
	//B3: Xác định vị trí cạnh dựa vào Non-maximal suppression
	//Hay loại bỏ một số biên dư thừa
	//Pxy là điểm xét với 2 điểm lân cận a, b
	//a, b nằm trên đường thẳng chứa hướng của đường biên góc Angle
	float X, Y, Angle, Pxy, a, b;
	uchar * pMark = (uchar *)Mark.data, *pMarkRow;
	for (int i = 0; i < height; i++, pMark += widthStep)
	{
		pMarkRow = pMark;
		for (int j = 0; j < width; j++, pMarkRow += nChannels)
		{
			X = Gx.at<float>(i, j); Y = Gy.at<float>(i, j);
			//Tính hướng
			Angle = fastAtan2(Y, X);
			Pxy = G.at<float>(i, j);
			if (Angle <= 22.5 || Angle > 157.5)
			{
				//(x, y-1) (x, y+1)
				if (j - 1 >= 0)
					a = G.at<float>(i, j - 1);
				if (j + 1 < width)
					b = G.at<float>(i, j + 1);
			}
			else if (Angle > 22.5 && Angle <= 67.5)
			{
				//(x - 1, y - 1), (x + 1, y + 1)
				if (j - 1 >= 0 && i - 1 >= 0)
					a = G.at<float>(i - 1, j - 1);
				if (j + 1 < width && i + 1 < height)
					b = G.at<float>(i + 1, j + 1);
			}
			else if (Angle > 67.5 && Angle <= 112.5)
			{
				//(x - 1, y + 1), (x + 1, y - 1)
				if (j + 1 < width && i - 1 >= 0)
					a = G.at<float>(i - 1, j + 1);
				if (j - 1 >= 0 && i + 1 < height)
					b = G.at<float>(i + 1, j - 1);
			}
			else if (Angle > 112.5 && Angle <= 157.5)
			{
				//(x-1, y), (x+1, y)
				if (i - 1 >= 0)
					a = G.at<float>(i - 1, j);
				if (i + 1 < height)
					b = G.at<float>(i + 1, j);
			}
			//Non-maximal suppression
			//đánh dấu các điểm loại nếu điểm đang xét nhỏ hơn một trong 2 điểm hướng
			if (Pxy < a || Pxy < b)
				pMarkRow[0] = 0;
			else
				pMarkRow[0] = 1;
		}
	}

	//Loại bỏ điểm không phải biên
	pMark = (uchar *)Mark.data;
	for (int i = 0; i < height; i++, pMark += widthStep)
	{
		pMarkRow = pMark;
		for (int j = 0; j < width; j++, pMarkRow += nChannels)
		{
			if (pMarkRow[0] == 0)
				G.at<float>(i, j) = 0;
		}
	}

	//B4: Phân ngưỡng để loại bỏ kết quả dư thừa
	int * edges = new int[height * width];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{

			Pxy = G.at<float>(i, j);
			if (Pxy >= _highThreshold && dstImage.at<uchar>(i, j) == 0)
			{
				//Recursive
				HysteresisThresholding(G, dstImage, i, j);
			}
		}
	}

	//Giải phóng bộ nhớ
	GrayImage.release();
	G.release();
	Gx.release();
	Gy.release();
	Mark.release();

	if (dstImage.empty())
		return 0;
	return 1;
}

void CannyEdgeDetector::setThreshold(int low, int high)
{
	_lowThreshold = low;
	_highThreshold = high;
}

CannyEdgeDetector::CannyEdgeDetector()
{
	_lowThreshold = _highThreshold = 20; //giá trị mặc định
}
CannyEdgeDetector::~CannyEdgeDetector() {}