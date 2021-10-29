#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <windows.h>
#include <imgcodecs.hpp>
#include <core.hpp>
#include <highgui.hpp>
#include <imgproc.hpp>
#include "config.h"
using namespace std;
using namespace cv;
using namespace Json;
void merge2() {
	//��ȡ��Ļ�ֱ��� (�����Զ�����Ԥ�����ڳߴ�)
	//int width{ GetSystemMetrics(SM_CXSCREEN) };
	//int height{ GetSystemMetrics(SM_CYSCREEN) };
	//cout << width << "	" << height << endl;

	//��ȡ�����ļ�
	int quality;
	Value root;
	ifstream json("config\\config.json", ios::binary);
	Reader reader;
	if (reader.parse(json, root))
	{
		quality = root["qualityPNGfile"].asInt();
	}
	else {
		cout << "�����ļ��������������ļ��Ƿ�ʧ��������﷨����" << endl;
	}

	bool empty = false;
	//��ȡcsv�ļ����� (��ѭ����)
	int csvNum = 0, c;
	int sum1 = 0, sum2 = 0;	//ͳ�ƴ���ͼƬ��
	system("dir char\\*l.csv /b > filelist.txt");	//ֻ�� *l.* �ļ�������
	FILE* filelist = fopen("filelist.txt", "r");
	do {
		c = fgetc(filelist);
		if (c == '\n')		csvNum++;
	} while (c != EOF);

	//��csv�ļ�����������csvName
	rewind(filelist);
	char csvName[2001][100] = { {0} };	//csv���� <= 1000
	for (int i = 1; i <= csvNum * 2; i++) {
		fgets(csvName[i], 10, filelist);		//csv�ļ������� <= 9 (������׺��)
	}

	//��ȡcsv�ļ�����������para
	//���õ�������� (��ѭ����)
	string para[10010];	//ÿ��������������Ĳ���� <= 1000
	string line;
	for (int i = 0; i < csvNum; i++) {	//��ѭ�� (��csv�ļ�Ϊ��λ)
		int diffNum = 0;
		string csvPath = "char\\";
		csvPath += csvName[i * 2 + 1];
		ifstream csv(csvPath);
		getline(csv, line);	//�������б�ͷ
		while (getline(csv, line)) {
			string num, capitalTemp;
			diffNum++;
			stringstream readstr(line);
			for (int j = 0; j < 10; j++) {
				getline(readstr, num, ',');
				capitalTemp = num.c_str();
				transform(capitalTemp.begin(), capitalTemp.end(), capitalTemp.begin(), ::tolower);		//��дתСд
				para[(diffNum - 1) * 10 + j] = capitalTemp;
				//cout << (diffNum - 1) * 10 + j << "	" << para[(diffNum - 1) * 10 + j] << endl;
			}
		}
		//���CG·��
		for (int j = 0; j < diffNum; j++) {
			string imgOpath, imgDpath;		//ԭʼͼƬ Origin ����ͼƬ Diff ·��
			imgOpath = "char\\" + para[j * 10 + 1] + ".png";
			imgDpath = "char\\" + para[j * 10 + 2] + ".png";
			//�ϲ����CG (merge)��������
			vector <int> compression_params;
			compression_params.push_back(IMWRITE_PNG_COMPRESSION);
			compression_params.push_back(quality);	//���pngѹ���̶�
			if (para[j * 10 + 2] != "") {
				Mat imgO = imread(imgOpath, IMREAD_UNCHANGED);	//��ȡ͸��png
				Mat imgD = imread(imgDpath, IMREAD_UNCHANGED);
				if (!imgO.data || !imgD.data) {
					goto jump;		//�ж�ͼƬ�Ƿ����
					empty = true;
				}
				if (imgD.channels() != 4) {
					cvtColor(imgD, imgD, COLOR_BGR2BGRA);	//RGB -> RGBA
				}
				int x = stoi(para[j * 10 + 3]), y = stoi(para[j * 10 + 4]), w = stoi(para[j * 10 + 5]), h = stoi(para[j * 10 + 6]);	//string -> int
				Mat roi(imgO(Rect(x, y, w, h)));
				imgD.copyTo(roi);
				namedWindow("Viewer", WINDOW_NORMAL);
				moveWindow("Viewer", 1, 1);
				resizeWindow("Viewer", imgO.cols / 5.5, imgO.rows / 5.5);
				imwrite("result\\" + para[j * 10] + ".png", imgO, compression_params);
				imshow("Viewer", imgO);
				waitKey(1);
				sum1++;
			}
			else {
				Mat imgO = imread(imgOpath, IMREAD_UNCHANGED);
				if (!imgO.data) {
					goto jump;		//�ж�ͼƬ�Ƿ����
					empty = true;
				}
 				namedWindow("Viewer", WINDOW_NORMAL);
				resizeWindow("Viewer", imgO.cols / 5.5, imgO.rows / 5.5);
 				imwrite("result\\" + para[j * 10 + 1] + ".png", imgO, compression_params);
				cvtColor(imgO, imgO, COLOR_BGRA2BGR);
				imshow("Viewer", imgO);
				waitKey(1);
				sum2++;
			}
		jump: ;
		}
	}
	fclose(filelist);
	system("del filelist.txt");
	if (empty) {
		cout << "���棺�в���ͼƬ�޷���ȡ�����ֶ���飡" << endl;
	}
	cout << "\n���γɹ��ϲ� " << sum1 << " �Ų��ͼ���������� " << sum2 << " ��ԭͼ��" << endl;
	cout << "���ƻ�� " << sum1 + sum2 << " ���������棬��Ү~~\n" << endl;
	waitKey(1000);
	destroyWindow("Viewer");
}