#include <Heatmap.h>
#include <opencv2/opencv.hpp>

struct box
{
	int x;
	int y;
	int w;
	int h;
};

int main()
{
	vector<box> vec;
	box box1{ 100,100,50,50 };
	vec.push_back(box1);
	
	cv::Mat img = cv::imread("dog.jpg");

	auto pix_color = heatmap::heatmap(vec, 300, 255, heatmap::Size(img.cols, img.rows), "classic", heatmap::Area(0, 0, img.cols, img.rows));
	
	cv::Mat hm = cv::Mat(img.rows, img.cols, CV_8UC4, pix_color);

	cv::imshow("123", hm);
	cv::waitKey();
	return 0;
}
