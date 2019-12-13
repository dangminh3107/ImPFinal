#include "stdafx.h"
#include "FourierTransform.h"
#include <iostream>
using namespace std;
FourierTransform::FourierTransform()
{
}

FourierTransform::~FourierTransform()
{

}


Mat FourierTransform::DFT(cv::Mat data, Direction direction)
{
	Mat C = Mat(Size(data.rows, data.cols), CV_64FC2);
	double inverse = direction;
	double pi2,invs, cosA, sinA, angleTerm;
	int N = data.cols;
	pi2 = -2.0*PI*inverse;
	invs = 1.0 / double(N);
	for (int y = 0; y < N; y++)
	{
		C.at<Vec2d>(0, y)[0] = 0;
		C.at<Vec2d>(0, y)[1] = 0;
		for (int x = 0; x < N; x++)
		{
			angleTerm = pi2 * y * x * invs;
			cosA = cos(angleTerm);
			sinA = sin(angleTerm);
			C.at<Vec2d>(0, y)[0] += data.at<Vec2d>(0, x)[0] * cosA - data.at<Vec2d>(0, x)[1] * sinA;
			C.at<Vec2d>(0, y)[1] += data.at<Vec2d>(0, x)[0] * sinA + data.at<Vec2d>(0, x)[1] * cosA;
		}
		if (inverse == 1)
		{
			C.at<Vec2d>(0, y)[0] *= invs;
			C.at<Vec2d>(0, y)[1] *= invs;
		}
			
	}
	return C;

}

Mat FourierTransform::DFT2(cv::Mat data, Direction direction)
{
	Mat C = Mat(data.rows, data.cols, CV_64FC2);
	double inverse = direction;
	double pi2, invsN, invsM, cosA, sinA, angleTerm, t1, d1, t2, d2;
	int M = data.cols;
	int N = data.rows;
	pi2 = -2.0*PI*inverse;
	invsN = 1.0 / double(N);
	invsM = 1.0 / double(M);
	for (int v = 0; v < N; v++)
	{
		for (int u = 0; u < M; u++)
		{
			C.at<Vec2d>(v, u)[0] = 0;
			C.at<Vec2d>(v, u)[1] = 0;
			for (int y = 0; y < N; y++)
			{
				for (int x = 0; x < M; x++)
				{
					angleTerm = pi2 * (u*x*invsM + v*y*invsN);
					cosA = cos(angleTerm);
					sinA = sin(angleTerm);
					
					//cout <<u<<" "<<v<<", " << x<< " "<< y << endl;
					t1 = data.at<Vec2d>(y, x)[0];
					d1 = t1*cosA;
					t2 = data.at<Vec2d>(y, x)[1];
					d2 = t2*sinA;
					C.at<Vec2d>(v, u)[0] += (d1 - d2);
					d1 = t1*sinA;
					d2 = t2*cosA;
					C.at<Vec2d>(v, u)[1] += (d1 + d2);
					

				}
			}
			if (inverse == 1)
			{
				C.at<Vec2d>(v, u)[0] *= (invsM*invsN);
				C.at<Vec2d>(v, u)[1] *= (invsM*invsN);
			}
		}

	}
	
	
	return C;
}

void FourierTransform::FFT(cv::Mat data, Direction direction)
{

}

Mat FourierTransform::FFT2(cv::Mat data, Direction direction)
{
	Mat C = Mat(data.rows, data.cols, CV_64FC2);


	return C;
}