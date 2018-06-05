#include"YOLOv3SE.h"
#include <ctime>  
#include <iostream>
#include <vector> 
#include <string> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace std;   
using namespace cv; 




int main()
{
    string modelBinary = "yolov3.weights";
    string modelConfiguration = "cfg/yolov3.cfg";
    string class_names = "coco.names";
    string infile = "test04.mp4";
    string out_file = "hahahaha.avi";
    cout <<"hahha"<<endl;
    YOLOv3 detector;
	detector.Create(modelBinary, modelConfiguration, class_names);
    //cout <<"hahha"<<endl;
    std::vector<cv::Scalar> colors;
	for (int i = 0; i < 80; i++) {
		colors.push_back(cv::Scalar(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
	}
    cout <<"heihei"<<endl;
    VideoCapture cap(infile);
    cout <<"heihei"<<endl;
    VideoWriter writer;
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);

    int codec = CV_FOURCC('M', 'J', 'P', 'G');
    writer.open(out_file, codec, 30, Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT)), 1);

    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera/video or read image

        if (frame.empty())
        {
            waitKey();
            break;
        }
    	std::vector<BoxSE> boxes = detector.Detect(frame, 0.5F);
		//continue;
		for (BoxSE &box : boxes) {
		cv::putText(frame, detector.Names(box.m_class), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, colors[box.m_class], 2);
		cv::rectangle(frame, box, colors[box.m_class], 2);
		}

        if(writer.isOpened())
        {
            writer.write(frame);
        }
    }
        


}


