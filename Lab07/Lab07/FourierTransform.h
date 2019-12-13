#pragma once
#define PI 3.14159265
#include "Complex.h"
#include "ComplexImage.h"
/// <summary>
/// Xác định Fourier thuận hay nghịch
/// </summary>
enum Direction
{
	/// <summary>
	/// Fourier thuận
	/// </summary>
	Forward = 1,

	/// <summary>
	/// Fourier nghịch
	/// </summary>
	Backward = -1
};



/// <summary>
/// Fourier transformation.
/// </summary>
/// 
/// <remarks>Class cài đặt phép biến đổi
/// Discrete and Fast Fourier Transformation.</remarks>
///

// Copyright © Andrew Kirillov, 2005-2009
// andrew.kirillov@aforgenet.com
//
// FFT idea from Exocortex.DSP library
// http://www.exocortex.org/dsp/
//

class FourierTransform
{
public:
	/// <summary>
	/// Discrete Fourier Transform 1 chiều
	/// </summary>
	/// 
	/// <param name="data">Data to transform.</param>
	/// <param name="direction">Transformation direction.</param>
	/// 
	Mat DFT(cv::Mat data, Direction direction);

	/// <summary>
	/// Discrete Fourier Transform 2 chiều
	/// </summary>
	/// 
	/// <param name="data">Data to transform.</param>
	/// <param name="direction">Transformation direction.</param>
	/// 
	Mat DFT2(cv::Mat data, Direction direction);

	/// <summary>
	/// Fast Fourier Transform 1 chiều
	/// </summary>
	/// 
	/// <param name="data">Data to transform.</param>
	/// <param name="direction">Transformation direction.</param>
	/// 	
	void FFT(cv::Mat data, Direction direction);

	/// <summary>
	/// Fast Fourier Transform 2 chiều
	/// </summary>
	/// 
	/// <param name="data">Data to transform.</param>
	/// <param name="direction">Transformation direction.</param>
	/// 	
	Mat FFT2(cv::Mat data, Direction direction);
	FourierTransform();
	~FourierTransform();
};

