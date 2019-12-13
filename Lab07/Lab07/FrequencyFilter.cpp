#include "stdafx.h"
#include "FrequencyFilter.h"

FrequencyFilter::FrequencyFilter(int minRange, int maxRange)
{
	_minRange = minRange;
	_maxRange = maxRange;
}

FrequencyFilter::~FrequencyFilter()
{

}

ComplexImage FrequencyFilter::Apply(ComplexImage complexImage)
{
	double R, I, F, D, H;
	
	Mat C = complexImage.getData();
	for (int v = 0; v < C.rows; v++)
	{
		for (int u = 0; u < C.cols; u++)
		{
			R = C.at<cv::Vec2d>(v, u)[0];
			
			I = C.at<cv::Vec2d>(v, u)[1];
			
			D = sqrt(u*u + v*v);
			if (D < _minRange)
			{
				C.at<cv::Vec2d>(v, u)[0] = 0;
				C.at<cv::Vec2d>(v, u)[1] = 0;
			}
			if (D > _maxRange)
			{
				C.at<cv::Vec2d>(v, u)[0] = 0;
				C.at<cv::Vec2d>(v, u)[1] = 0;
				

				
			}
		}
	}
	complexImage.setData(C);
	return complexImage;
}