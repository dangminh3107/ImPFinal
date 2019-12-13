#include "stdafx.h"
#include "Kmean.h"
#include "Threshold.h"

int main(int argc, char * argv[]){
	Mat srcImage, dstImage;
	int result = -1;
	string cmd = string(argv[1]);
	srcImage = imread(argv[2], -1);

	if (srcImage.empty())
		return 0;

	if (cmd == "--static") //Phân ngưỡng toàn cục tĩnh
	{
		//đối số: size kích thước cửa sổ, hệ số ngưỡng trên và ngưỡng dưới		
		float low = atof(argv[3]), high = atof(argv[4]);
		StaticThreshold _static;
		_static.setThreshold(low, high);
		result = _static.Apply(srcImage, dstImage);
	}
	else if (cmd == "--mean") //Phân ngưỡng cục bộ theo trung bình
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		AverageLocalThreshold _mean;
		_mean.setC(C);
		result = _mean.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (cmd == "--median") //Phân ngưỡng cục bộ theo trung vị
	{
		//đối số: size kích thước cửa sổ, hệ số C
		int size = atoi(argv[3]), C = atoi(argv[4]);
		MedianLocalThreshold _median;
		_median.setC(C);
		result = _median.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (cmd == "--sauvola") //Phân ngưỡng cục bộ áp dụng Sauvola
	{
		//đối số: size:kích thước của sổ, hệ số k và r
		int size = atoi(argv[3]);
		float k = atof(argv[4]);
		int r = atoi(argv[5]);	
		SauvolaLocalThreshold _sauvola;
		_sauvola.setValue(k, r);
		result = _sauvola.Apply(srcImage, dstImage, Size(size, size));
	}
	else if (cmd == "--kmeans") //Phân đoạn ảnh bằng phương pháp k-means
	{
		//đối số: số cụm (clusters)
		int clusters = atoi(argv[3]);
		Kmean _kmean;
		_kmean.setNumClusters(clusters);
		result = _kmean.Apply(srcImage, dstImage);
	}

	imshow("Source Image", srcImage);
	if (result)
		imshow("Destination Image", dstImage);

	waitKey(0);
	return 0;
}