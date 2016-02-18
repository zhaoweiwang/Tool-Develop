#include "Head.h"

extern parainfo para;
unordered_map<int, int> mapScanPosPf1;		//改用哈希
extern vector<psmInfo> psmVec;				//PSM缓冲区

vector<int> scanPf1;
vector<int> posPf1;


void readPf1idx(){
	cout << "Step6: Read .pf1idx file." << endl;
	int scan;
	int pos;
	FILE* inputPf1idx = fopen(para.pf1idx_path.c_str(), "rb");
	if (inputPf1idx == NULL){
		cout << "Failed to open " << para.pf1idx_path << " File." << endl;
	}
	else{
		cout << para.pf1idx_path << " File open successfully.\n" << endl;
	}

	while (!feof(inputPf1idx)){
		fread(&scan, sizeof(int), 1, inputPf1idx);
		fread(&pos, sizeof(int), 1, inputPf1idx);
		//mapScanPosPf1[scan] = pos;
		scanPf1.push_back(scan);
		posPf1.push_back(pos);
	}
	//cout << "pf1idx size: " << mapScanPosPf1.size() << endl;
	//getchar();
	fclose(inputPf1idx);
}

void calcuPf1Pos(){
	for (int i = 0; i < psmVec.size(); i++){
		int scanTemp = 0;
		for (int j = 0; j < 100000; j++){
			if (scanPf1[j] > psmVec[i].scan){
				scanTemp = j - 1;
				break;
			}
		}
		psmVec[i].scanPre = scanPf1[scanTemp];
		psmVec[i].pf1Pos = posPf1[scanTemp];
		//cout << scanTemp << " " << psmVec[i].pf1Pos << endl;
		//getchar();
	}
}

void readPf1(){
	cout << "Step7: Read .pf1 file." << endl;
	ifstream input_pf1;
	input_pf1.open(para.pf1_path, ios::binary);
	if (!input_pf1.is_open()){
		cout << "Failed to open " << para.pf1_path << " File." << endl;
	}
	else{
		cout << para.pf1_path << " File open successfully." << endl;
	}

	for (int i = 0; i < psmVec.size(); i++){
		input_pf1.seekg(psmVec[i].pf1Pos + sizeof(int), ios::beg);
		input_pf1.read((char*)&psmVec[i].precuNums, sizeof(int));

		//double preMz = (psmVec[i].mass1 / (double)psmVec[i].charge) - (psmVec[i].charge - 1) * 1.00727647012;
		double preMz = (psmVec[i].mass1 + (double)(psmVec[i].charge - 1) * 1.00727647012) / (double)psmVec[i].charge;
		for (int j = 0; j < psmVec[i].precuNums; j++){
			peakInfo tempPeak;
			input_pf1.read((char*)&tempPeak.mz, sizeof(double));
			input_pf1.read((char*)&tempPeak.iten, sizeof(double));

			if (fabs(tempPeak.mz-preMz) < 1.0/*Da*/)
				psmVec[i].precus.push_back(tempPeak);
		}
		//psmVec[i].precuNums = psmVec[i].precus.size();

		//cout << "\nTitle: " << psmVec[i].title << endl;
		//cout << "precusNums: " << psmVec[i].precuNums << endl;
		//getchar();
		//cout << "M/Z\tIten" << endl;
		//for (int j = 0; j < psmVec[i].precuNums; j++){
		//	cout << psmVec[i].precus[j].mz << "\t" << psmVec[i].precus[j].iten << endl;
		//}
	}
	cout << "Successfully read " << para.pf1_path << endl << endl;

	//for (int i = 0; i < psmVec.size(); i++){
	//	cout << "Title: " << psmVec[i].title << endl;
	//	cout << "precusNums: " << psmVec[i].precuNums << endl;
	//	cout << "M/Z\tIten" << endl;
	//	for (int j = 0; j < psmVec[i].precuNums; j++)
	//		cout << psmVec[i].precus[j].mz << "\t" << psmVec[i].precus[j].iten << endl;
	//	getchar();
	//}
}

void calcuResult(){
	//cout << "sum: " << psmVec.size() << endl;
	for (int i = 0; i < psmVec.size(); i++){
		//double preMz = (psmVec[i].mass1 / (double)psmVec[i].charge) - (psmVec[i].charge - 1) * 1.00727647012;
		double preMz = (psmVec[i].mass1 + (double)(psmVec[i].charge - 1) * 1.00727647012) / (double)psmVec[i].charge;
		double temp1 = 0.0;//PIF分子

		//if (i == 4761){
		//	cout << psmVec[i].title << endl; cout << psmVec[i].scan << endl;
		//	cout << psmVec[i].scanPre << endl;
		//	cout << preMz << endl;
		//	for (int j = 0; j < psmVec[i].precus.size(); j++)
		//		cout << psmVec[i].precus[j].mz << " " << psmVec[i].precus[j].iten << endl;
		//	getchar();
		//}

		//如果precus没有
		//if (0 == psmVec[i].precus.size()){
		//	psmVec[i].PIF = 0.0;
		//	continue;
		//}

		while (1){
			double minDis = 10000.0;
			int minDisIdx = -1;

			for (int j = 0; j < psmVec[i].precus.size(); j++){
				if (fabs(preMz - psmVec[i].precus[j].mz) < (200 * 0.000001*preMz)){
					if (minDis > fabs(preMz - psmVec[i].precus[j].mz)){
						minDis = fabs(preMz - psmVec[i].precus[j].mz);
						minDisIdx = j;
					}
				}
			}
			
			if (-1 == minDisIdx){
				preMz += (1.0 / (double)psmVec[i].charge);
				if (preMz > psmVec[i].precus[psmVec[i].precus.size() - 1].mz)
					break;
				continue;
			}else{
				temp1 += (psmVec[i].precus[minDisIdx].iten * 0.1);
				preMz += (1.0 / (double)psmVec[i].charge);
			}

			if (preMz > psmVec[i].precus[psmVec[i].precus.size() - 1].mz)
				break;
		}//temp1已获得；

		double temp2 = 0.0;
		for (int j = 0; j < psmVec[i].precus.size(); j++)
			temp2 += (psmVec[i].precus[j].iten * 0.1);

		psmVec[i].PIF = (temp1/temp2);

		//cout << i << ": " << psmVec[i].PIF << endl;
		//getchar();
	}
}

void calcuPIF(){
	readPf1idx();
	calcuPf1Pos();
	readPf1();
	calcuResult();
}

