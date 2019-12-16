#include <opencv2/opencv.hpp>
#include <raspicam_cv.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <vector>

using namespace std;
using namespace cv;
using namespace raspicam;

//Creates the frames
Mat Frame, FramePerspective, FrameGray, Matrix, FrameThreshold, FrameEdge, FrameFinal;
Mat RnLane, FrameFinalDuplicate;
RaspiCam_Cv Camera;

void Capture();
void Threshold();
void RegionofInterest();
void Histogram();
void LineDetector();
void LineCenter();

Point2f Source[] ={Point2f(80,160), Point2f(340,160), Point2f(30,210), Point2f(390,210)};
Point2f Destination[] ={Point2f(80,0), Point2f(280,0), Point2f(80,240), Point2f(280,240)};

//Global variables
stringstream ss;
vector<int> HistogramLane;
int LeftLane, RightLane, linecenter, framecenter, diff;

void Setup (int argc, char **argv, RaspiCam_Cv &Camera){
	Camera.set( CAP_PROP_FRAME_WIDTH,("-w", argc, argv, 400));
	Camera.set( CAP_PROP_FRAME_HEIGHT, ("-h", argc, argv, 240));
	Camera.set( CAP_PROP_BRIGHTNESS, ("-br", argc, argv, 50));
	Camera.set( CAP_PROP_CONTRAST, ("-co", argc, argv, 50));
	Camera.set( CAP_PROP_SATURATION, ("-sa", argv, argc, 50));
	Camera.set( CAP_PROP_GAIN, ("-g", argc, argv, 50));
	Camera.set( CAP_PROP_FPS, ("-fps", argc, argv, 0));
	}
	


int main(int argc, char **argv){
	
	
	Setup(argc, argv, Camera);
	cout << "Connection to the Camera " << endl;
	if(!Camera.open()){
		cout << "Failed to connect to camera " << endl;
		return -1;
		}
	cout << "Camera ID= " << Camera.getId() << endl;
	
	while(1){
		auto start = std::chrono::system_clock::now();
		
		Capture();
		RegionofInterest();
		Threshold();
		Histogram();
		LineDetector();
		LineCenter();
		
		ss.str(" ");
		ss.clear();
		ss << "Dist: "  << diff;
		putText(Frame, ss.str(), Point2f(1,50), 0, 1, Scalar(0,0,255), 2);
		
		namedWindow("Frame_RGB", WINDOW_KEEPRATIO);
		moveWindow("Frame_RGB", 640,10);
		resizeWindow("Frame_RGB",360,240);
		imshow("Frame_RGB", Frame);
		
		namedWindow("Perspective", WINDOW_KEEPRATIO);
		moveWindow("Perspective", 640,250);
		resizeWindow("Perspective",360,240);
		imshow("Perspective", FramePerspective);
		
		namedWindow("Frame_Edge", WINDOW_KEEPRATIO);
		moveWindow("Frame_Edge", 640,490);
		resizeWindow("Frame_Edge",360,240);
		imshow("Frame_Edge", FrameFinal);
		
		//((namedWindow("Frame_Gray", WINDOW_KEEPRATIO);
		//moveWindow("Frame_Gray", 280,10);
		//resizeWindow("Frame_Gray",360,240);
		//imshow("Frame_Gray", FrameThreshold);
		

		waitKey(1);
		auto end = std::chrono::system_clock::now();
		
		std::chrono::duration<double> elapsed_seconds = end-start;
		
		float t = elapsed_seconds.count();
		int FPS = 1/t;
		cout << "FPS = " <<FPS << endl;

		
		
		}
	return 0;
	}
	
void RegionofInterest(){
	line(Frame,Source[0], Source[1], Scalar(0,0,255), 1);
	line(Frame,Source[1], Source[3], Scalar(0,0,255), 1);
	line(Frame,Source[3], Source[2], Scalar(0,0,255), 1);
	line(Frame,Source[2], Source[0], Scalar(0,0,255), 1);
	
	//line(Frame,Destination[0], Destination[1], Scalar(0,255,0), 1);
	//line(Frame,Destination[1], Destination[3], Scalar(0,255,0), 1);
	//line(Frame,Destination[3], Destination[2], Scalar(0,255,0), 1);
	//line(Frame,Destination[2], Destination[0], Scalar(0,255,0), 1);
	
	Matrix = getPerspectiveTransform(Source, Destination);   //Perspective transformation of Region of interest
	
	warpPerspective(Frame,FramePerspective, Matrix, Size(400,240));
	}
	void Capture(){
		
		Camera.grab();
		Camera.retrieve(Frame);
		cvtColor(Frame,Frame,COLOR_BGR2RGB);
}

void Threshold(){
	cvtColor(FramePerspective, FrameGray, COLOR_RGB2GRAY);
	inRange(FrameGray,180,255,FrameThreshold);
	inRange(FrameGray,165,255,FrameGray);
	Canny(FrameGray, FrameEdge, 300, 450 ,3,false);
	add(FrameThreshold, FrameEdge, FrameFinal);
	cvtColor(FrameFinal, FrameFinal, COLOR_GRAY2RGB);
	cvtColor(FrameFinal, FrameFinalDuplicate, COLOR_RGB2GRAY); // for histogram only 
	}
	
void Histogram(){
	
	HistogramLane.resize(400);
	HistogramLane.clear();

	for(size_t i{0}; i < Frame.size().width; ++i){
		
		RnLane = FrameFinalDuplicate(Rect(i,140,1,100));
		divide(255, RnLane, RnLane);
		HistogramLane.push_back((int)(sum(RnLane)[0]));
		
		}
}

void LineCenter(){
	linecenter = (RightLane - LeftLane) / 2 + LeftLane;
	framecenter = 175;
	
	line(FrameFinal, Point2f(linecenter, 0 ), Point2f(linecenter, 240), Scalar(255,255,0), 3);
	line(FrameFinal, Point2f(framecenter, 0 ), Point2f(framecenter, 240), Scalar(255,0,0), 3);
	
	diff = linecenter - framecenter;
	}

void LineDetector(){
	vector<int>:: iterator LeftPtr;
	LeftPtr = max_element(HistogramLane.begin(), HistogramLane.begin() +150);
	LeftLane = distance(HistogramLane.begin(),LeftPtr);
	
	vector<int>:: iterator RightPtr;
	RightPtr = max_element(HistogramLane.begin() + 240 , HistogramLane.end());
	RightLane = distance(HistogramLane.begin(), RightPtr);
	
	line(FrameFinal, Point2f(LeftLane, 0), Point2f(LeftLane, 240), Scalar(0,255,0),2);
	line(FrameFinal, Point2f(RightLane, 0), Point2f(RightLane, 240), Scalar(0,255,0),2);
	}
