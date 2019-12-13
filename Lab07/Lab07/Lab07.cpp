// Lab07.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "ComplexImage.h"
#include "FourierTransform.h"
#include "FrequencyFilter.h"

using namespace std;

int main(int argc, char* argv[])
{

	Mat srcImage, srcImage2, dest_Image;

	string cmd = string(argv[1]);
	//CV_LOAD_IMAGE_GRAYSCALE
	srcImage = imread(argv[2], 0);
	string outputName = string(argv[3]);
	if (srcImage.data == NULL) {
		cout << "Can't open image" << endl;
		return 0;
	}
	ComplexImage complexImage = ComplexImage::FromOpenCVMat(srcImage);

	if (cmd == "--fft")
	{
		/// // thực hiện phép biến đổi Fourier thuận
		complexImage.ForwardFourierTransform();
	
	}
	if (cmd == "--ifft")
	{
		/// // thực hiện Fourier transformation nghịch
		complexImage.BackwardFourierTransform( );
	}
	if (cmd == "--freq")
	{
		FrequencyFilter filter = FrequencyFilter(atof(argv[5]), atof(argv[4]));
		/// // thực hiện phép biến đổi Fourier thuận
		complexImage.ForwardFourierTransform();
		/// // áp dụng filter
		complexImage = filter.Apply( complexImage );
		/// // thực hiện Fourier transformation nghịch
		complexImage.BackwardFourierTransform( );

	}

	namedWindow("Show Image", WINDOW_AUTOSIZE);  
	imshow("Show Image", srcImage);

	/// // thực hiện phép biến đổi Fourier thuận
	
	//complexImage.BackwardFourierTransform( );
	

	/// // áp dụng filter
	//complexImage = filter.Apply( complexImage );
	/// // thực hiện Fourier transformation nghịch
	//complexImage.BackwardFourierTransform( );
	
	/// // chuyển lại sang cấu trúc Mat
	Mat fourierImage = complexImage.ToOpenCVMat();
	imwrite(outputName, fourierImage);
	namedWindow("fourier image", WINDOW_AUTOSIZE);
	imshow("fourier image", fourierImage);
	
	waitKey(0);

	return 0;
}


