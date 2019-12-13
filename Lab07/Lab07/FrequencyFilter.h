#pragma once
#include "ComplexImage.h"


/// <remarks><para>Class cài đặt filter theo tần số. Chỉ giữ lại các giá trị nằm trong khoảng
///tần số quy định</para>
/// 
/// <para>Cách dùng:</para>
/// <code>
/// // tạo complex image
/// ComplexImage complexImage.FromOpenCVMat( image );
/// // thực hiện Fourier transformation thuận
/// complexImage.ForwardFourierTransform( );
/// // tạo filter theo tần số với 2 khoảng tần số 
/// FrequencyFilter filter = FrequencyFilter( 20, 128 );
/// // áp dụng filter
/// filter.Apply( complexImage );
/// // thực hiện Fourier transformation nghịch
/// complexImage.BackwardFourierTransform( );
/// // chuyển về lại opencv mat
/// Mat fourierImage = complexImage.ToOpenCVMat( );
/// </code>

class FrequencyFilter
{
	/// khoảng tần số xác định 
	/// <para>Default value is set to <b>[0, 1024]</b>.</para></remarks>
	///
	int _minRange;
	int _maxRange;
public:
	FrequencyFilter(int minRange, int maxRange);
	~FrequencyFilter();

	/// <summary>
	/// Áp dụng filter cho complex image.
	/// </summary>	 
	ComplexImage Apply(ComplexImage complexImage);
};

