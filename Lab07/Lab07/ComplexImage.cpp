#include "stdafx.h"
#include "ComplexImage.h"

ComplexImage::ComplexImage()
{
}

ComplexImage::~ComplexImage()
{
}

ComplexImage ComplexImage::FromOpenCVMat(Mat image)
{
	

	ComplexImage C;
	int row = image.rows; //số dòng của ảnh gốc --> chiều dài
	int col = image.cols; //số cột của ảnh gốc--> chiều rộng
	C.width = col;
	C.height = row;

	Mat planes[] = { Mat_<double>(image), Mat::zeros(image.size(), CV_64F) };
	merge(planes, 2, C.data);
	return C;
}

Mat ComplexImage::ToOpenCVMat()
{
	Mat A = Mat(height, width, CV_64FC1);
	double R;
	double I;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			R = data.at<cv::Vec2d>(y, x)[0]; 
			R *= R;
			I = data.at<cv::Vec2d>(y, x)[1];
			I *= I;

			A.at<double>(y, x) = sqrt(R + I);
		}
	}
	
	return A;
}

void ComplexImage::ForwardFourierTransform()
{	
	FourierTransform FT;
	data = FT.DFT2(data, Forward);
	//data = FT.FFT2(data, Forward);
	
	fourierTransformed = true;
}

void ComplexImage::BackwardFourierTransform()
{
	FourierTransform FT;
	data = FT.DFT2(data, Backward);
	//data = FT.FFT2(data, Backward);
	fourierTransformed = false;
}

Mat ComplexImage::getData()
{
	return data;
}

void ComplexImage::setData(Mat dat)
{
	data = dat;
	width = data.cols;
	height = data.rows;
}