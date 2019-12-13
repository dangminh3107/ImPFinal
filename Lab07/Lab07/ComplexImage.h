#pragma once
#include "stdafx.h"
#include "FourierTransform.h"
#include <iostream>

using namespace std;

/// <summary>
/// Complex image.
/// </summary>
/// 
/// <remarks><para>class dùng để biểu diễn ảnh khi chuyển sang miền tần số
/// </para>
///  
/// <para>Cách dùng:</para>
/// <code>
/// // tạo complex image từ cấu trúc Mat của OpenCV
/// ComplexImage complexImage = ComplexImage.FromOpenCVMat( image );
/// // thực hiện phép biến đổi Fourier thuận
/// complexImage.ForwardFourierTransform( );
/// // chuyển lại sang cấu trúc Mat
/// Mat fourierImage = complexImage.ToOpenCVMat( );
/// </code>
/// 


class ComplexImage
{

	// ma trận số phức, mỗi phần tử là 1 số phức gồm 2 thành phần thực và ảo
	// tạo ma trận số thực với 2 channels
	Mat data;
	// image dimension
	int width;
	int height;
	// current state of the image (transformed with Fourier ot not)
	bool fourierTransformed;
public:
	ComplexImage();
	~ComplexImage();

	/// <summary>
	/// tạo complex image từ cấu trúc Mat của OpenCV
	/// </summary>
	static ComplexImage FromOpenCVMat(Mat image);
	Mat getData();
	void setData(Mat dat);
	/// <summary>
	/// chuyển từ complex image sang cấu trúc Mat
	/// </summary>	
	Mat ToOpenCVMat();

	/// <summary>
	/// Áp dụng phép Fourier thuận cho complex image
	/// </summary>
	/// 
	void ForwardFourierTransform();


	/// <summary>
	/// Áp dụng phép Fourier nghịch cho complex image
	/// </summary>
	/// 
	void BackwardFourierTransform();
};

