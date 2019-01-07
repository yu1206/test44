#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "fstream"
#include <direct.h>
#include <io.h>
#include <Windows.h>

using namespace std;
using namespace cv;

extern int readDir(char *dirName, vector<string> &filesName);



struct imfo2
{
	string name;
	int srcw;
	int srch;
	int srcow;
	int  srcoh;
	int stdw;
	int stdh;
	int stdow;
	int stdoh;
	float objpersrcw;
	float objpersrch;
};

//降序
bool GreaterSort2(imfo2 a, imfo2 b)
{
	int sidea = min(a.stdow, a.stdoh);
	int sideb = min(b.stdow, b.stdoh);
	return (sidea>sideb);
}
//升序
bool LessSort2(imfo2 a, imfo2 b)
{
	int sidea = max(a.stdow, a.stdoh);
	int sideb = max(b.stdow, b.stdoh);
	return (sidea<sideb);
}


int test44(int argc, char *argv[])
{

	string imgpath = "G:/F/YOLO/yolo-v2/darknet-master/train/VOCdevkit/VOC2007/JPEGImages";
	string stxtpath = "G:/F/YOLO/yolo-v2/darknet-master/train/VOCdevkit/VOC2007/labels";
	string txtpath = "G:/F/Traffic-signs-voc/data/0-per.txt";

	

	fstream finWrite;
	finWrite.open(txtpath, ios::out);
	if (!finWrite.is_open())
	{
		cout << "finRead 文件数据打开错误！" << endl;
		system("pause");
		return false;
	}

	vector<string> v_img_; int num = 0;

	readDir((char*)imgpath.c_str(), v_img_);
	int sidew = 224; int sideh = 224;

	int perminw = 10000; int perminh = 10000;
	int permaxw = 0; int permaxh = 0;
	vector<imfo2>vfo;
	for (int i = 0; i<v_img_.size(); i++)
	{
		int npos = v_img_[i].find_last_of('/');
		int npos2 = v_img_[i].find_last_of('.');

		string name1 = v_img_[i].substr(npos + 1, npos2 - npos - 1);
		Mat img = imread(v_img_[i].c_str());
		if (img.data == NULL)
		{
			printf("img.data = NULL！\n");
			system("pause");
			continue;
		}

		float scale_factorw = static_cast<float>(sidew) / img.cols;
		float scale_factorh = static_cast<float>(sideh) / img.rows;


		int stdw = scale_factorw*img.cols;
		int stdh = scale_factorh*img.rows;
		


		string str = stxtpath + "/" + name1 + ".txt";
		fstream finRead1;
		finRead1.open(str, ios::in);
		if (!finRead1.is_open())
		{
			cout << "finRead 文件数据打开错误！" << endl;
			system("pause");
			return false;
		}

		string line; int num1 = 0;

		while (getline(finRead1, line))
		{

			if (line.empty())
			{
				break;
			}

			num1++;

			int label; float cenx, ceny, cenw, cenh;
			stringstream str_s(line);
			str_s >> label >> cenx >> ceny >> cenw >> cenh;

			int objw = cenw*img.cols;
			int objh = cenh*img.rows;

			int stdow = objw*scale_factorw;
			int stdoh = objh*scale_factorh;

			float objperw = objw*1.0 / img.cols;
			float objperh = objh*1.0 / img.rows;

			imfo2 vi;
			vi.name = name1;
			vi.srcw = img.cols;
			vi.srch = img.rows;
			vi.srcow = objw;
			vi.srcoh = objh;
			vi.stdw = stdw;
			vi.stdh = stdh;
			vi.stdow = stdow;
			vi.stdoh = stdoh;

			vi.objpersrcw = objperw;
			vi.objpersrch = objperh;
			vfo.push_back(vi);


			if (stdow<perminw)
			{
				perminw = stdow;
			}
			if (stdoh < perminh)
			{
				perminh = stdoh;
			}

			if (stdow>permaxw)
			{
				permaxw = stdow;
			}
			if (stdoh > permaxh)
			{
				permaxh = stdoh;
			}

		}

		finRead1.close();

	}
	printf("perminw=%d,perminh=%d,permaxw=%d,permaxh=%d\n", perminw, perminh, permaxw, permaxh);

	sort(vfo.begin(), vfo.end(), LessSort2);//升序排列

	for (int j = 0; j < vfo.size(); j++)
	{
		imfo2 vi = vfo[j];
		finWrite << vi.name << " " << vi.srcw << " " << vi.srch << " " << vi.srcow << " " << vi.srcoh << " " <<
			vi.stdw << " " << vi.stdh << " " << vi.stdow << " " << vi.stdoh << " " << vi.objpersrcw <<
			" " << vi.objpersrch << endl;
	}

	finWrite.close();

	return 0;
}


