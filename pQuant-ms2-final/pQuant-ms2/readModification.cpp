/********************************************************
文件名：		readModification.cpp
创建人：		Mako Wan
加注日期：	2016-03-29
描述：		读取modification.ini文件，获取修饰-质量的哈希表
********************************************************/

#include "Head.h"

unordered_map<string, double> mod2mass;

extern parainfo para;

void readModification(){
	ifstream input_modifi;
	input_modifi.open(para.modification_path, ios::binary);

	if (!input_modifi.is_open()){
		cout << "Failed to open " << para.modification_path << " File." << endl;
		exit(0);
	}

	string tempModifi = "";
	double tempMass = 0.0;
	getline(input_modifi, tempModifi);					//过掉表头一行
	while (!input_modifi.eof()){
		getline(input_modifi, tempModifi);
		if (tempModifi.empty())
			break;
		//cout <<tempModifi.substr(tempModifi.find('=')+1, tempModifi.find(' ')-tempModifi.find('=')-1) << endl;
		string tempName = tempModifi.substr(tempModifi.find('=') + 1, tempModifi.find(' ') - tempModifi.find('=') - 1);

		vector<int> pos;
		getline(input_modifi, tempModifi);
		for (int i = 0; i < tempModifi.size(); ++i){
			if (tempModifi[i] == ' ')
				pos.push_back(i);
		}
		//cout << tempModifi.substr(pos[1]+1, pos[2] - pos[1] - 1) << endl;
		tempMass = atof(tempModifi.substr(pos[1] + 1, pos[2] - pos[1] - 1).c_str());

		//cout << tempName << ":" << tempMass << endl;
		//getchar();
		mod2mass[tempName] = tempMass;
	}
}