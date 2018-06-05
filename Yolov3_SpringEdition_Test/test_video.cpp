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

class pubimage
{
public:

    pubimage()
    {
        aoutputVideo.open("./yolov3_outdoor.avi", CV_FOURCC('M','J','P','G'), 30, cv::Size(1920, 1080), true);

        if (aoutputVideo.isOpened()){
            for(int i = 2000; i < 6000; i ++){
                string num;
                ostringstream convert;
                convert << i;
                num = convert.str();

                cv::Mat img = cv::imread("/home/yolo_slam/yolo_wrap/test/img/" + num + ".jpg");
                aoutputVideo.write(img);
            }
        }
        else
            std::cout << "Could not open the output video for write " << std::endl;
    }

    ~pubimage() { }
private:
    cv::VideoWriter aoutputVideo;

};

void video2image(string video,string path)    
{    
    VideoCapture capture(video);   
    long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);    
    cout << "total frames is:" << totalFrameNumber << "." << endl;  
    //设置开始帧    
    long frameToStart = 1;    
    capture.set(CV_CAP_PROP_POS_FRAMES, frameToStart);    
    cout << "from" <<frameToStart << "read" << endl;      
    //设置结束帧    
    int frameToStop = totalFrameNumber;    
     
    //获取帧率    
    double rate = capture.get(CV_CAP_PROP_FPS);    
    cout << "rate is:" << rate<< endl;    
    double delay = 1000 / rate;   
    //定义一个用来控制读取视频循环结束的变量    
    bool stop = false;    
    long currentFrame = frameToStart;    
  
    if(!capture.isOpened())    
    {    
        cerr<<"Failed to open a video"<<endl;    
        return ;    
    }    
    
    Mat frame;    
    int num=1;    
    string filename;    
    char   temp_file[10];    
    
    while (!stop)     
    {    
        capture>>frame;    
        if(frame.empty())    
            break;    
        sprintf(temp_file, "%d", num); //4表示字符长度,10表示十进制,_itoa_s实现整型转字符串    
        filename = temp_file;    
        filename = path+filename+".jpg";   
		cout <<filename<<endl;   
          
        cout << "now is reading" << currentFrame << "." << endl;    
        imshow("Extractedframe",frame);    
           
        cout << "now is writing" << currentFrame << "." << endl;     
        imwrite(filename,frame);    
  
         int c = waitKey(delay);    
        //按下ESC或者到达指定的结束帧后退出读取视频    
        if ((char)c == 27 ||currentFrame > frameToStop)    
        {    
            stop = true;    
        }    
        //按下按键后会停留在当前帧，等待下一次按键    
        if (c >= 0)    
        {    
            waitKey(0);    
        }    
  
        num++;    
        currentFrame++;  
    }    
    capture.release();    
    waitKey(0);    
}   

void Yolo3Test() {
	
	YOLOv3 detector;
	detector.Create("yolov3.weights", "cfg/yolov3.cfg", "coco.names");
	//detector.Create("darknet53_416_random_10000.weights", "darknet53_voc.cfg", "voc2007.names");
	//detector.Create("darknet19.weights", "darknet19.cfg", "voc2007.names");
	cv::VideoCapture cap;
	cap.open("test.mov");
	//检测是否正常打开:成功打开时，isOpened返回ture
	if(!cap.isOpened())
	    cout<<"fail to open!"<<endl;
	long totalFrameNumber = cap.get(CV_CAP_PROP_FRAME_COUNT);
	//获取整个帧数
	cv::Mat frame;
	cout<<"整个视频共"<<totalFrameNumber<<"帧"<<endl;
	

	//cv::imshow("Extracted frame",frame);


	//std::cout <<"haha"<<std::endl;
	std::vector<cv::Scalar> colors;
	for (int i = 0; i < 80; i++) {
		colors.push_back(cv::Scalar(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
	}

	int num = 2000;    
    string filename;    
    char   temp_file[50]; 
	//7794
    
	while(num<7700)
	{
		sprintf(temp_file, "%d", num); //4表示字符长度,10表示十进制,_itoa_s实现整型转字符串
		filename = temp_file;  
		string path = "/home/yolo_slam/yolo_wrap/test/img/";
		 cout<<path<<endl;
		filename = path+filename+".jpg";
		cout<<filename<<endl;
		 cv::Mat img = cv::imread(filename);
         clock_t start = clock();
		std::vector<BoxSE> boxes = detector.Detect(img, 0.5F);
	//continue;
	for (BoxSE &box : boxes) {
		cv::putText(img, detector.Names(box.m_class), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, colors[box.m_class], 2);
		cv::rectangle(img, box, colors[box.m_class], 2);
		//cv::rectangle(img, box.tl(), box.br(), colors[box.m_class], -1, 4);
	}
    clock_t end = clock();
    cout << (end - start)/1000 <<"ms"<<endl;
	cout <<filename<<endl;
    //cv::imshow("frame",img);
	cv::imwrite(filename, img);
	++num;
	}


	// std::fstream fin("test.txt", std::ios::in);
	// while (fin.eof() == false) {
	// std::string line;
	// std::getline(fin, line);
	// if (line.length() == 0)break;
	// std::string line = "image24.png";
	// //clock_t start = clock();
	// cv::Mat img = cv::imread(line);
	// while(1) 
    // { 
		
    //     cap>>frame; 
    //     if(frame.empty()) break;
    //     //cv::imshow("当前视频",frame); 
	// 	std::vector<BoxSE> boxes = detector.Detect(frame, 0.5F);
	// 	//continue;
	// 	for (BoxSE &box : boxes) {
	// 	cv::putText(frame, detector.Names(box.m_class), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, colors[box.m_class], 2);
	// 	cv::rectangle(frame, box, colors[box.m_class], 2);
	// 		}
	// 		cv::imshow("frame",frame);
    //     if(cv::waitKey(30) >=0) 
    //         break;
    // }  

}
int main() {
	// Yolo3Test();
	// // string videoFromfile = "test.mov";  //读取视频    
    // // string Imagespath    = "/home/yolo_slam/yolo_wrap/test/img/";    // 保存图片的文件夹路径一定要有，因为OpenCV不会自动创建文件夹    
    // // video2image(videoFromfile,Imagespath);   
	pubimage pub;
	return 0;
}
