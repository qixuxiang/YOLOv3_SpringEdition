#include"YOLOv3SE.h"
#include <fstream>
#include<iostream>
using namespace std;
using namespace cv;

void Yolo3Test() {
	std::cout<<"hahaha"<<std::endl;
	YOLOv3 detector;
	detector.Create("yolov3.weights", "cfg/yolov3.cfg", "coco.names");
	//detector.Create("darknet53_416_random_10000.weights", "darknet53_voc.cfg", "voc2007.names");
	//detector.Create("darknet19.weights", "darknet19.cfg", "voc2007.names");



	std::vector<cv::Scalar> colors;
	for (int i = 0; i < 80; i++) {
		colors.push_back(cv::Scalar(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
	}
	Mat image;
	string ImgName;
	std::string path =  "/home/yolo_slam/yolo_wrap/test/test_images/";
	
	ifstream fin("/home/yolo_slam/yolo_wrap/test/test_images/file.txt");
	
	while(getline(fin,ImgName)) //一行一行读取文件列表 
	{ 
		
		ImgName = path + ImgName;
		image = imread(ImgName);//读取图片
		std::vector<BoxSE> boxes = detector.Detect(image, 0.5F);
        //continue;
        for (BoxSE &box : boxes) {
          cv::putText(image, detector.Names(box.m_class), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, colors[box.m_class], 2);
          cv::rectangle(image, box, colors[box.m_class], 2);
          //cv::rectangle(frame, box.tl(), box.br(), colors[box.m_class], -1, 4);
        }
		imwrite(ImgName,image);

	}




}
int main() {
	Yolo3Test();

	return 0;
}
