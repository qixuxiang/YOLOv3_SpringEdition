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

void process_video(string video,string path)//process video by frame and frame in yolov3 and save images to a dir
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

    cv::Mat frame;
    int num=1;
    string filename;
    char   temp_file[10];
    YOLOv3 detector;
    detector.Create("yolov3.weights", "cfg/yolov3.cfg", "coco.names");
    std::vector<cv::Scalar> colors;
    for (int i = 0; i < 80; i++) {
      colors.push_back(cv::Scalar(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
    }

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
        //imshow("Extractedframe",frame);
        std::vector<BoxSE> boxes = detector.Detect(frame, 0.5F);
        //continue;
        for (BoxSE &box : boxes) {
          cv::putText(frame, detector.Names(box.m_class), box.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, colors[box.m_class], 2);
          cv::rectangle(frame, box, colors[box.m_class], 2);
          //cv::rectangle(frame, box.tl(), box.br(), colors[box.m_class], -1, 4);
        }


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


int main() {

    string videoFromfile = "test.mp4";  //读取视频
    string Imagespath    = "/home/yolo_slam/yolo_wrap/test/img/";    // 保存图片的文件夹路径一定要有，因为OpenCV不会自动创建文件夹
    process_video(videoFromfile,Imagespath);
    //pubimage pub;
    return 0;
}
