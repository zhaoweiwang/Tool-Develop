/**************************************************
�ļ�����		Head.h
�����ˣ�		Mako Wan
��ע���ڣ�	2015-10-26
������		��ͷ�ļ����������趫��
**************************************************/
#ifndef IHEAD_H
#define IHEAD_H

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <stdio.h>

using namespace std;

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
void checkDate();

/*
��������	readCmdline
���ܣ�	��ȡcmd�����У�
���룺	��
���أ�	��
*/
void readCmdline(const int argc, char* argv[]);

#endif