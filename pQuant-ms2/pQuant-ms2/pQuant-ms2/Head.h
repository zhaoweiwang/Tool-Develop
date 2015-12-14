/**************************************************
�ļ�����		Head.h
�����ˣ�		Mako Wan
��ע���ڣ�	2015-10-26
������		��ͷ�ļ����������趫��
**************************************************/
#ifndef IHEAD_H
#define IHEAD_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <unordered_map>
#include <map>
// Attention:
// If you use visual studio cl.exe compiler, use this header file
#include "dirent.h"
// else if you use gcc compiler, use this header file
//#include <dirent.h>
#include "stringProcess.h"

using namespace std;

//�Զ��������

typedef struct parainfo{

	string binPath = ".\\";

	int quantMethod = 0;						//1-3����iTRAQ4��iTRAQ8��TMT
	double detaFragment = 0.0;					//�������ڴ�С
	vector<double> reporterMZ;

	string input_spectra_path = "";				//pFind.spectra����ļ�����Ϊ�����ļ���
	string pf_path = "";						//pf2�ļ�·����
	string pfidx_path = "";						//pf2idx�ļ�·��;
	string pf1_path = "";						//pf1�ļ�·����
	string pf1idx_path = "";					//pf1idx�ļ�·����
	string output_ratio_path = "";				//������ֵ���·����


}paraInfo;

//�׷�
typedef struct peakinfo{
	double mz;
	double iten;
}peakInfo;

//����һ��psm��Ҫ��ȫ����Ϣ
typedef struct psminfo{

	string	title			= "";
	int		scan			= 0;
	double	mass1			= 0.0;
	int		charge			= 0;
	double	fdr				= 0.0;
	string	pepSq			= "";
	double	mass2			= 0.0;
	double  massGapDa		= 0.0;
	double  massGapPpm		= 0.0;
	double	score			= 0.0;
	string  modification	= "";
	string	proAc			= "";
	string	prosandCons		= "";
	
	int		pf2Pos			= 0;

	int		peakNums		= 0;
	vector<peakInfo> peaks;

	vector<double> reporter;

	//TODO: ���ü̳����������ֶ���������

}psmInfo;

class screenInfo{
public:
	void printInfoTo(ostream& output, string infoType, string inFo);
	string getTimeStr();
};

class A{

public:
	int a = 1;
	void showInfo(){
		cout << "This is " << this->a << endl;
	}

};

class B : public A{

public:
	int b = 2;
	void showInfo(){
		cout << "This is " << this->b << endl;
	}
	//in this case, class B also have int a menber;
};

//������
const string TIMESTRING = "2015-10-26 16:00";


//����������

/*
��������	printLogo
���ܣ�	��ӡpQuant-ms2`s Logo
���룺	��
���أ�	��
*/
void printLogo();

/*
��������	checkDate
���ܣ�	�������Ƿ���ڣ�
���룺	��
���أ�	��
*/
void checkDate(int year, int month, int day);

/*
��������	readCmdline
���ܣ�	��ȡcmd�����У�
���룺	������
���أ�	��
*/
void readCmdline(const int argc, char* argv[]);

/*
��������	readData
���ܣ�	��ȡpFind.spectra��ȡPSM����ȡpf2.idx����ȡpf2��ȡ�׷���Ϣ��
���룺	��
���أ�	��
*/
void readData();

#endif