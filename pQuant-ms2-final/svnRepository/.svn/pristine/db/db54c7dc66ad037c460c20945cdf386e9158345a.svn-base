#include "Head.h"

extern parainfo para;
unordered_map<string, string>  acDe;

extern vector<psmInfo> psmVec;					//PSM缓冲区

unordered_map<string, vector<int>> proMap;

vector<proteinInfo> proteinVec;

double PIF = 0.75;

set<string> ProteinFDR;
unordered_map<string, int> uniqueMap;



inline bool comp(const double& a, const double& b){
	return a < b;
}

inline bool compScore(const psmInfo& a, const psmInfo& b){
	return a.score < b.score;
}

inline bool compSize(const proteinInfo& a, const proteinInfo& b){
	return a.pepNum > b.pepNum;
}

void readFasta(){

	cout << "\nStep7: Read .fasta file." << endl;

	ifstream inputFasta;
	inputFasta.open(para.fasta_path, ios::binary);
	if (!inputFasta.is_open()){
		cout << "Failed to open " << para.fasta_path << " File." << endl;
	}else{
		cout << para.fasta_path << " File open successfully." << endl;
	}

	string tempDe = "";
	while (!inputFasta.eof()){
		getline(inputFasta, tempDe);
		if ('>' == tempDe[0]){
			//cout << tempDe.substr(1, tempDe.find(' ')-1) << endl;
			//cout << tempDe.substr(tempDe.find(' ') + 1, tempDe.find('\0')) << endl;
			//getchar();
			acDe[tempDe.substr(1, tempDe.find(' ') - 1)] = tempDe.substr(tempDe.find(' ') + 1, tempDe.find('\0'));
		}
		else{
			continue;
		}
	}
}

void proteinMap(){
	for (int i = 0; i < psmVec.size(); i++){
		if ((psmVec[i].PIF - PIF) >= 0){
			string tempStr = psmVec[i].proAc;
			while (tempStr.find('/') != string::npos){
				string acTemp = tempStr.substr(0, tempStr.find('/'));
				tempStr = tempStr.substr(tempStr.find('/') + 1, tempStr.size());

				if (acTemp.find("REV") != string::npos){
					//cout << "過濾掉反庫：" << acTemp << endl;
					//getchar();
					continue;
				}
				else
					proMap[acTemp].push_back(i);
				//cout << acTemp << endl;
				//cout << tempStr << endl;
				//getchar();

			}
			//cout << psmVec[i].proAc << endl;
			//cout << psmVec[i].score << endl;
			//getchar();
		}
		else continue;
	}
}

void calcuPro(){
	
	for (auto iter = proMap.begin(); iter != proMap.end(); iter++){		//ProMap是PSMs中的蛋白，卡FDR 1%读取PSMs，原则上这个蛋白是acDe中蛋白的子集；

		proteinInfo tempPro;
		tempPro.ac = iter->first;

		set<string>::iterator it;
		if ((it = ProteinFDR.find(tempPro.ac)) == ProteinFDR.end())
			continue;

		tempPro.uniqueNum = uniqueMap[tempPro.ac];
		//cout << tempPro.uniqueNum << endl;
		//getchar();

		//tempPro.index.clear();
		//for (int i = 0; i < iter->second.size(); i++)
		//	tempPro.index.push_back(iter->second[i]);

		tempPro.de = acDe[tempPro.ac];

		//cout << tempPro.ac << ": " << tempPro.de << endl;
		//getchar();

		for (int i = 0; i < iter->second.size(); i++)
			tempPro.peptide.insert(psmVec[iter->second[i]].pepSq);

		////计算PSMs的Ratio中位数，用以作蛋白的Ratio，因为用到排序，所以用空间换时间
		//vector<double> tempRatio;
		//for (int i = 0; i < psmVec[0].ratioReporter.size(); i++){
		//	tempRatio.clear();
		//	for (int j = 0; j < iter->second.size(); j++){
		//		tempRatio.push_back(psmVec[iter->second[j]].ratioReporter[i]);
		//	}
		//	sort(tempRatio.begin(), tempRatio.end(), comp);
		//	size_t size = iter->second.size();
		//	if (size % 2 == 0)
		//		tempPro.ratio.push_back((tempRatio[size / 2 - 1] + tempRatio[size / 2]) / 2.0);
		//	else
		//		tempPro.ratio.push_back(tempRatio[size / 2]);
		//}

		//计算PSMs的標籤離子強度的中位数，用以作蛋白質的強度量；
		//因为用到排序，所以用空间换时间
		vector<double> tempRatio;
		for (int i = 0; i < psmVec[0].reporterCorrect.size(); i++){
			tempRatio.clear();
			for (int j = 0; j < iter->second.size(); j++){
				tempRatio.push_back(psmVec[iter->second[j]].reporterCorrect[i]);
			}
			sort(tempRatio.begin(), tempRatio.end(), comp);
			size_t size = iter->second.size();
			if (size % 2 == 0)
				tempPro.ratio.push_back((tempRatio[size / 2 - 1] + tempRatio[size / 2]) / 2.0);
			else
				tempPro.ratio.push_back(tempRatio[size / 2]);
		}


		//for (int i = 0; i < tempPro.ratio.size(); i++)
		//	cout << tempPro.ratio[i] << " ";
		//cout << endl;
		//getchar();

		//計算蛋白質的PIF。
		//其實PIF不是在這塊用
		//vector<double> tempPIF;
		//for (int i = 0; i < iter->second.size(); i++)
		//	tempPIF.push_back(psmVec[iter->second[i]].PIF);
		//sort(tempPIF.begin(), tempPIF.end(), comp);
		//size_t size = iter->second.size();
		//if (size % 2 == 0)
		//	tempPro.PIF = (tempPIF[size / 2 - 1] + tempPIF[size / 2]) / 2.0;
		//else
		//	tempPro.PIF = tempPIF[size / 2];

		tempPro.psmNum = iter->second.size();
		//cout << tempPro.PIF << endl;
		//getchar();
		tempPro.pepNum = tempPro.peptide.size();
		//cout << tempPro.ac << endl;
		//for (int i = 0; i < tempPro.index.size(); i++)
		//	cout << tempPro.index[i] << " ";
		//cout << endl;
		//cout << iter->first << endl;
		//for (int i = 0; i < iter->second.size(); i++)
		//	cout << iter->second[i] << " ";
		//cout << endl;
		//getchar();

		//插入蛋白质上的肽段，用到set去冗余
		//tempPro.peptide.clear();
		//for (int i = 0; i < iter->second.size(); i++)
		//	tempPro.peptide.insert(psmVec[iter->second[i]].pepSq);
		//cout << tempPro.psmNum << endl;
		//cout << tempPro.peptide.size() << endl;
		//for (auto i = tempPro.peptide.begin(); i != tempPro.peptide.end(); i++)
		//	cout << *i << " ";
		//cout << endl;
		//getchar();

		proteinVec.push_back(tempPro);
	}

	//sort(proteinVec.begin(), proteinVec.end(), compSize);

	//for (int i = 0; i < proteinVec.size(); i++){
		//cout << proteinVec[i].ac << endl;
		//cout << proteinVec[i].psmNum << endl;
		//cout << proteinVec[i].pepNum << endl;
		//for (auto iter = proteinVec[i].peptide.begin(); iter != proteinVec[i].peptide.end(); iter++)
		//	cout << *iter << " ";
		//getchar();

	//}

}

void proteinRela(){
	ifstream inputProtein;
	inputProtein.open(para.input_protein_path, ios::binary);
	if (!inputProtein.is_open()){
		cout << "Failed to open " << para.input_protein_path << " File." << endl;
	}
	else
	{
		cout << para.input_protein_path << " File open successfully." << endl;
	}

	unordered_map<string, Rela> tempRela;
	string tempStr = "";
	while (!inputProtein.eof()){
		getline(inputProtein, tempStr);
		int sameNum = 0, subNum = 0;
		Rela temp;
		if (isdigit(tempStr[0])){
			//cout << "a: " << tempStr << endl;
			//getchar();
			vector<int> pos_t;
			for (int i = 0; i < tempStr.size(); i++){
				if (tempStr[i] == '\t')
					pos_t.push_back(i);
			}
			sameNum = atoi(tempStr.substr(pos_t[pos_t.size() - 3]+1, 1).c_str());
			subNum = atoi(tempStr.substr(pos_t[pos_t.size() - 2]+1, 1).c_str());

			//cout << sameNum << endl;
			//cout << subNum << endl;
			//getchar();
			
			if (0 != sameNum){
				string str = "";
				for (int i = 0; i < sameNum; i++){
					getline(inputProtein, str);
					vector<int> posTemp;
					for (int j = 0; j < str.size(); j++){
						if (str[j] == '\t')
							posTemp.push_back(j);
					}

					temp.str = str.substr(1, posTemp[1] - posTemp[0] - 1);
					temp.pro.push_back(str.substr(posTemp[1] + 1, (posTemp[2] - posTemp[1] - 1)));
				}
			}
			else if(0 != subNum){
				string str = "";
				for (int i = 0; i < subNum; i++){
					getline(inputProtein, str);
					vector<int> posTemp;
					for (int j = 0; j < str.size(); j++){
						if (str[j] == '\t')
							posTemp.push_back(j);
					}

					temp.str = str.substr(1, posTemp[1] - posTemp[0] - 1);
					temp.pro.push_back(str.substr(posTemp[1]+1, (posTemp[2] - posTemp[1] - 1)));
				}
				//cout << temp.str << endl;
				//for (int i = 0; i < temp.pro.size(); i++)
				//	cout << temp.pro[0] << endl;
				////cout << str << endl;
				//getchar();

			}
			else{
				temp.str = "LeadSet";
				temp.pro.push_back(tempStr.substr(pos_t[0] + 1, pos_t[2] - pos_t[1] + 1));
				//cout << temp.pro[0] << endl; getchar();
			}
			//cout << sameNum << endl;
			//cout << subNum << endl;
			//getchar();
			//cout << tempStr.substr(pos_t[0]+1, pos_t[2]-pos_t[1]+1) << endl;
			string s1 = tempStr.substr(pos_t[0] + 1, pos_t[2] - pos_t[1] + 1);
			tempRela[s1] = temp;
		}
	}
	inputProtein.close();

	for (int i = 0; i < proteinVec.size(); i++){
		proteinVec[i].tag = tempRela[proteinVec[i].ac].str;
		for (int j = 0; j < tempRela[proteinVec[i].ac].pro.size(); j++)
			proteinVec[i].led += (tempRela[proteinVec[i].ac].pro[j] + "/");

		//cout << proteinVec[i].ac << endl;
		//cout << proteinVec[i].tag << ": " << proteinVec[i].led << endl;
		//getchar();
	}

}

void calUniqueNum(){
	ifstream inputProtein;
	inputProtein.open(para.input_protein_path, ios::binary);
	if (!inputProtein.is_open()){
		cout << "Failed to open " << para.input_protein_path << " File." << endl;
	}else{
		cout << para.input_protein_path << " File open successfully." << endl;
	}


	string tempStr = "";
	string ac = "";

	while (!inputProtein.eof()){
		getline(inputProtein, tempStr);

		if (isdigit(tempStr[0])){
			//cout << "a: " << tempStr << endl;
			//getchar();
			vector<int> pos_t;
			for (int i = 0; i < tempStr.size(); i++){
				if (tempStr[i] == '\t')
					pos_t.push_back(i);
			}

			ac = tempStr.substr(pos_t[0] + 1, pos_t[1] - pos_t[0] - 1);
			ProteinFDR.insert(ac);
			//cout << "ac:" << ac + "." << endl;
			//getchar();
		}
		else if (tempStr[1] == '\t'){
			//cout << "b: " << tempStr << endl;
			//getchar();
			int countAc = 0;
			for (int i = 0; i < tempStr.size(); i++){
				if (tempStr[i] == '/')
					countAc++;
			}
			//cout << uniqueMap[ac] << endl;
			if (countAc == 2)
				uniqueMap[ac]++;
			//cout << uniqueMap[ac] << endl;
			//cout << "countAc:" << countAc << endl;
			//getchar();
		}
		else if (tempStr == "----------------------------------------"){
			break;
		}
	}
	inputProtein.close();
}

void proteinInfer(){
	readFasta();	
	proteinMap();	//卡PIF
	calUniqueNum();
	calcuPro();		//獲取ProteinVec
	//proteinRela();
}