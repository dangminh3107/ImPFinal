// Lab04.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CannyEdgeDetector.h"
#include <string>

int main(int argc, char * argv[])
{
	Mat srcImage, dstImage; //Ma trận lưu trữ ảnh nguồn và ảnh đích
	CannyEdgeDetector canny;

	string command(argv[1]); // lệnh nhập từ tham số
	string InputPath(argv[2]); //Input path
	string OutputPath(argv[3]); //Output path
	int result = 0;
	int lowThreshold = atoi(argv[4]), highThreshold = atoi(argv[5]); //ngưỡng dưới và ngưỡng trên

	srcImage = imread(InputPath, -1); //Đọc ảnh với định dạng gốc (ảnh màu hay ảnh xám)
	if (command == "--canny") {
		canny.setThreshold(lowThreshold, highThreshold);
		result = canny.Apply(srcImage, dstImage);
	}
	if (!srcImage.empty())
		imshow("Source Image", srcImage);

	if (result) {
		imshow("Destination Image", dstImage);
		imwrite(OutputPath, dstImage);
	}
	waitKey(0);
	return 0;
}

