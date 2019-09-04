/*****
author：vincy
function：手动标注图像的四个点进行透视变换
time：2019/9/4
*****/

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string.h>
#include <sstream>
#include <io.h>
using namespace cv;
using namespace std;


void on_mouse(int EVENT, int x, int y, int flags, void* userdata);
Mat transForm(Point2f corners[], Mat img);
Mat PerspectTran(Mat img);




Point2f corners[4];
int num = 0;

void getFiles(string path, vector<string>& files)
{
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}



int main(int argc, char ** argv)
{
	if (argc != 2)

	{

		cout << "usage : argv[0] imagepath" << endl;
		return 1;

	}
	string filedir = argv[1];
	Mat img;
	img = imread(filedir);
	Mat img1 = PerspectTran(img);//透视变换

	int start = filedir.find_last_of('\\');
	int start2 = filedir.find_last_of('.');
	string temp = filedir.substr(start + 1, start2-start);
	cout << temp << endl;
	string save_filebinary = "D://pic1//"+ temp+"jpg";
	cout << save_filebinary << endl;
	bool r = imwrite(save_filebinary, img1);
    cout << "r=" <<r <<endl;
	return 0;
	//Mat img1 = img.clone();
	//img1=imgPretreat(img1);
	//vector<Rect> HorRoi=HorProjection(img1);
	//Mat erodeStruct = getStructuringElement(MORPH_RECT, Size(5, 5));
	//Mat dilateimg;
	//dilate(img1, dilateimg, erodeStruct);
	//imshow("img1", dilateimg);
	//vector<Rect> roi;
	//for (int i = 0; i < HorRoi.size(); ++i) 
	//{
	//	//imshow("rect", img(HorRoi[i]));
	//	rectangle(img, HorRoi[i],Scalar(255,0,0),1,8,0);
	//	vector<Rect> VerRoi=VerProjection(img1(HorRoi[i]));
	//	roi.insert(roi.begin(), VerRoi.begin(), VerRoi.end());
	//}
	//
	//waitKey();
}




//通过手动标注四个点获得经过透视变换后的图像
Mat PerspectTran(Mat img)
{
	namedWindow("断路由图", 0);
	setMouseCallback("断路由图", on_mouse, &img);
	imshow("断路由图", img);
	waitKey();
	Mat img1;
	//透视变化
	while (1)
	{
		int key = 0;
		if (num == 4)
		{
			cout << "请按任意键进行透视变化" << endl;
			img1 = transForm(corners, img);
			namedWindow("img1", 0);
			imshow("img1", img1);
			num = 0;
			cout << "区域选择结果满意按ESc键进入识别" << endl;
			key = waitKey(0);//waitKey中的参数，小于等于0表示一直等待键值，大于0的表示等待多少毫秒，超时就返回-1，否则就返回对应的键值
			cout << "key" << key << endl;
			return img1;
			break;
		}
		if (key == 27)
		{
			return img1;
			break;
		}
		waitKey();
	}
}

void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{
	Mat& hh = *(Mat*)userdata;
	Point2f p(x, y);

	switch (EVENT)
	{
	case EVENT_LBUTTONDOWN:
	{
		if (num == 0)
		{
			cout << "左上角的点的坐标" << "(" << x << "," << y << ")" << endl;
			corners[0] = p;
		}
		else if (num == 1)
		{
			cout << "右上角的点的坐标" << "(" << x << "," << y << ")" << endl;
			corners[1] = p;
		}
		else if (num == 2)
		{
			cout << "右下角的点的坐标" << "(" << x << "," << y << ")" << endl;
			corners[2] = p;
		}
		else if (num == 3)
		{
			cout << "左下角的点的坐标" << "(" << x << "," << y << ")" << endl;
			corners[3] = p;
		}
		num++;
		//circle(hh, p, 20, Scalar(0,0,255), 10);
	}

	}
}

//透视变化
Mat transForm(Point2f corners[], Mat img)
{
	int len = max(corners[1].x - corners[0].x, corners[3].x - corners[2].x);
	int hei = max(corners[3].y - corners[0].y, corners[2].y - corners[1].y);
	Point2f canvas[]= {
		Point2f(0, 0),
		Point2f(len, 0),
		Point2f(len, hei),
		Point2f(0, hei) };

	Mat img1;
	Size size1(len, hei);
	Mat transform = getPerspectiveTransform(corners, canvas);
	warpPerspective(img, img1, transform, size1);
	return img1;
}

