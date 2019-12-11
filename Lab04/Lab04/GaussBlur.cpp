#include "stdafx.h"
#include <math.h>
#include "GaussBlur.h"
#include "Convolution.h"
#define pi 3.141592653589793238462643383279502884L /* pi */ 

int GaussBlur::BlurImageWithGauss(Mat& srcImage, Mat& destImage, int kWidth, int kHeight)
{
	if (srcImage.empty())
		return 0;
	Convolution Convo;
	vector<float> kernel;
	vector<int> offsets;

	float sigma = 0.1; //phương sai
	float inverse_sqrt2pi_sigma = 1 / (sqrt(2 * pi)*sigma), inverse_2_sigmasquare = 1 / (2 * sigma*sigma);
	int n = kHeight*kWidth;

	//Tạo kernel (bộ lọc h)
	int kHalfHeight = kHeight >> 1;
	int kHalfWidth = kWidth >> 1;
	float h, sum = 0.0;
	for (int y = -kHalfHeight; y <= kHalfHeight; y++)
		for (int x = -kHalfWidth; x <= kHalfWidth; x++) {
			h = inverse_sqrt2pi_sigma*exp(-(y*y + x*x)*inverse_2_sigmasquare);
			sum += h;
			kernel.push_back(h);
		}

	//Chuẩn hóa kernel
	for (int i = 0; i < n; i++)
		kernel[i] /= sum;

	//nhân chập ảnh với kernel
	Convo.SetKernel(kernel, kWidth, kHeight);
	return Convo.DoConvolution(srcImage, destImage);
}

GaussBlur::GaussBlur() {}
GaussBlur::~GaussBlur() {}