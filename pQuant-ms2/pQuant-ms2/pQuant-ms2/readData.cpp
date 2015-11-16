﻿#include "Head.h"

extern parainfo para;
double fdr = 0.0;

//vector<vector<int>> pf2Vec;		//pf2idx缓冲区
unordered_map<int, int> mapScanPos;	//改用哈希
vector<psmInfo> psmVec;			//PSM缓冲区

vector<string> reType{ "iTRAQ-4plex", "iTRAQ-8plex", "TMT-6plex"};
vector<double> iTRAQ4{ 114.1, 115.1, 116.1, 117.1 };
vector<double> iTRAQ8{ 113.1, 114.1, 115.1, 116.1, 117.1, 118.1, 119.1, 121.1 };
vector<double> TMT{ 126.127726, 127.131080, 128.134435, 129.137790, 130.141145, 131.138180 };

double isotopeImpuritiesSolveItraq4plex[][4] = {
	{ 1.07791091996579, -0.0689622324110812, 0.00184705189405072, -1.53193753476204e-05 },
	{ -0.023402797437985, 1.0870599409944, -0.0659198132570745, 0.00203390871815362 },
	{ 0.000760200309746091, -0.0353113043877059, 1.08667888629714, -0.0528844573365622 },
	{ -7.58140146305044e-06, 0.000352156097958693, -0.0469710342409398, 1.08453825149864 }
};

double isotopeImpuritiesSolveItraq8plex[][8] = {
	{ 1.07735027781641, -0.0798588365599547, 0.00251434802463821, 5.02166602326827e-06, -2.90947289083881e-06, 8.61292793293383e-08, -1.73481325562699e-09, 1.63857076581857e-11 },
	{ -0.0109021489672849, 1.07734108252244, -0.0682863186315669, 0.00174044203804341, 5.052441515462e-07, -1.32272654664217e-06, 2.66423168120358e-08, -2.51642770887756e-10 },
	{ 0.000220454628342396, -0.0217851387518353, 1.07697703346751, -0.056627764541002, 0.00121263547557211, 5.05925977028333e-06, -1.01903452362433e-07, 9.62501395671661e-10 },
	{ -6.6750467510151e-06, 0.000659622439044461, -0.0326093042464951, 1.07637860292146, -0.0449925447493336, 0.00058185126209967, -1.1719629958055e-05, 1.10694583253804e-07 },
	{ 1.2796331597944e-07, -1.26452259801385e-05, 0.00062513340487531, -0.0434975128854994, 1.07541903927455, -0.0331970272818948, 0.000668653487228529, -6.31558499499316e-06 },
	{ -1.70822704128515e-11, 1.6880554240955e-09, -8.34512436978625e-08, 0.00115739928708735, -0.054268416092368, 1.07456833066468, -0.0216439217723613, 0.000204431787449292 },
	{ -1.90900636636197e-10, 1.8864638443762e-08, -9.32598253333981e-07, -4.94151235105094e-06, 0.00167778924686243, -0.0651122264592302, 1.07269280672266, -0.010131828703167 },
	{ 5.59582801995148e-13, -5.5297496252478e-11, 2.73370457496661e-09, 1.4484945551881e-08, -4.91806640595871e-06, 0.000190862025230617, -0.00314436063201736, 1.0856881510558 }
};

double isotopeImpuritiesSolveTMT6plex[][6] = {
	{ 1.11453082574093, -0.121240216479605, 0.00670706275650708, 1.71332720667745e-05, -1.56309091475288e-05, 8.49935296438809e-07 },
	{ -0.00866078003085075, 1.11105435252914, -0.105607592865366, 0.00320434060652233, 1.73005770636855e-05, -7.97850569907894e-06 },
	{ 1.8731916534043e-05, -0.00240303729251009, 1.08165043636562, -0.082352764438788, 0.00308527496555992, 4.66291557692142e-06 },
	{ -3.45261249481356e-07, 4.4292086004894e-05, -0.0199366669439801, 1.08995311280806, -0.0717456501349052, 0.00169308687925649 },
	{ -1.31806793005487e-08, 1.69089285884182e-06, -0.000761101379622512, -0.0222333537299289, 1.08259814509038, -0.0614089921803927 },
	{ 8.94940725219551e-10, -1.14808110178165e-07, 5.16772015397277e-05, -0.000316105216736487, -0.0424692004073348, 1.09411466226159 }

};



void readPf2idx(){

	cout << "Step1: Read .pf2idx file." << endl;
	int scan;
	int pos;
	FILE* inputPf2idx = fopen(para.pfidx_path.c_str(), "rb");
	if (inputPf2idx == NULL){
		cout << "Failed to open " << para.pfidx_path << " File." << endl;
	}
	else{
		cout << para.pfidx_path << " File open successfully.\n" << endl;
	}

	while (!feof(inputPf2idx)){
		//vector<int> temp;
		fread(&scan, sizeof(int), 1, inputPf2idx);
		fread(&pos, sizeof(int), 1, inputPf2idx);
		//cout << scan << " " << pos << endl;
		//getchar();
		//temp.push_back(scan); temp.push_back(pos);
		//pf2Vec.push_back(temp);
		mapScanPos[scan] = pos;
	}
	fclose(inputPf2idx);
	//cout << pf2Vec.size() << endl;
	//cout << mapScanPos.size() << endl;
	//for (int i = 0; i < 10; i++){
	//	cout << pf2Vec[i][0] << " " << pf2Vec[i][1] << endl;
	//	cout << mapScanPos[pf2Vec[i][0]] << endl;
	//	getchar();
	//}
	//getchar();
}

void readPsms(){

	cout << "Step2: Read .spectra file." << endl;

	ifstream input_spectra;
	input_spectra.open(para.input_spectra_path, ios::binary);
	if (!input_spectra.is_open()){
		cout << "Failed to open " << para.input_spectra_path << " File." << endl;
	}
	else{
		cout << para.input_spectra_path << " File open successfully.\n" << endl;
	}

	string tempPsm;

	while (0.01 - fdr >= 0 && !input_spectra.eof()){

		psmInfo psm;

		getline(input_spectra, tempPsm);
		if (tempPsm == ""){
			cout << "Successfully read " << para.input_spectra_path << "." << endl;
			break;
		}		//防止最后一行是换行的处理，原则上不会出现这种情况；
		//cout << tempPsm << endl;
		//getchar();
		vector<int> t_pos;		//\t位点所在；
		/*cout << t_pos.size() << endl;*/
		for (int i = 0; i < tempPsm.size(); i++){
			if (tempPsm[i] == '\t')
				t_pos.push_back(i);
		}
		//cout << tempPsm.substr(t_pos[3] + 1, t_pos[4] - t_pos[3] - 1) << endl;
		//getchar();
		fdr = atof(tempPsm.substr(t_pos[3] + 1, t_pos[4] - t_pos[3] - 1).c_str());
		//cout << "fdr: " << fdr << endl;
		//getchar();
		if (0.01 - fdr < 0)	break;
		else{

			if (tempPsm.substr(t_pos[14] + 1, t_pos[15] - t_pos[14] - 1) == "decoy")	   //TODO: 如果有污染库则需要加；
				continue;
			//cout << tempPsm.substr(t_pos[14] + 1, t_pos[15] - t_pos[14] - 1) << endl;
			//getchar();
			psm.title = tempPsm.substr(1, t_pos[0] - 1);
			psm.scan = atoi(tempPsm.substr(t_pos[0] + 1, t_pos[1] - t_pos[0] - 1).c_str());
			psm.mass1 = atof(tempPsm.substr(t_pos[1]+1, t_pos[2]-t_pos[1]-1).c_str());
			psm.charge = atoi(tempPsm.substr(t_pos[2]+1, t_pos[3]-t_pos[2]-1).c_str());
			psm.fdr = atof(tempPsm.substr(t_pos[3] + 1, t_pos[4] - t_pos[3] - 1).c_str());
			psm.pepSq = tempPsm.substr(t_pos[4] + 1, t_pos[5] - t_pos[4] - 1);
			psm.mass2 = atof(tempPsm.substr(t_pos[5] + 1, t_pos[6] - t_pos[5] - 1).c_str());
			psm.massGapDa = atof(tempPsm.substr(t_pos[6] + 1, t_pos[7] - t_pos[6] - 1).c_str());
			psm.massGapPpm = atof(tempPsm.substr(t_pos[7] + 1, t_pos[8] - t_pos[7] - 1).c_str());	
			psm.score = atof(tempPsm.substr(t_pos[8] + 1, t_pos[9] - t_pos[8] - 1).c_str());
			psm.modification = tempPsm.substr(t_pos[9] + 1, t_pos[10] - t_pos[9] - 1);
			psm.proAc = tempPsm.substr(t_pos[11] + 1, t_pos[12] - t_pos[11] - 1);
			psm.prosandCons = tempPsm.substr(t_pos[14] + 1, t_pos[15] - t_pos[14] - 1);
			psm.pf2Pos = mapScanPos[psm.scan];
			
			//cout << psm.title << endl;
			//cout << psm.scan << endl;
			//cout << psm.mass1 << endl;
			//cout << psm.charge << endl;
			//cout << psm.fdr << endl;
			//cout << psm.pepSq << endl;
			//cout << psm.mass2 << endl;
			//cout << psm.massGapDa << endl;
			//cout << psm.massGapPpm << endl;
			//cout << psm.score << endl;
			//cout << psm.modification << endl;
			//cout << psm.proAc << endl;
			//cout << psm.prosandCons << endl;
			//cout << psm.pf2Pos << endl;
			psmVec.push_back(psm);
			//getchar();

		}

	}
	cout << "Successfully read " << para.input_spectra_path << "." << endl;
	cout << "Total PSM number: " << psmVec.size() << endl << endl;
}

void readPf2(){

	cout << "Step3: Read .pf2 file." << endl;

	ifstream input_pf2;
	input_pf2.open(para.pf_path, ios::binary);
	if (!input_pf2.is_open()){
		cout << "Failed to open " << para.pf_path << " File." << endl;
	}
	else{
		cout << para.pf_path << " File open successfully.\n" << endl;
	}

	for (int i = 1; i < psmVec.size(); i++){
	
		//cout << psmVec[i].scan << endl;
		input_pf2.seekg(psmVec[i].pf2Pos + sizeof(int), ios::beg);
		input_pf2.read((char*)&psmVec[i].peakNums, sizeof(int));
		//cout << psmVec[i].peakNums << endl;
		//getchar();
		for (int j = 0; j < psmVec[i].peakNums; j++){
		
			peakInfo tempPeak;
			input_pf2.read((char*)&tempPeak.mz, sizeof(double));
			input_pf2.read((char*)&tempPeak.iten, sizeof(double));
			//cout << tempPeak.mz << " " << tempPeak.iten << endl;
			//getchar();
			psmVec[i].peaks.push_back(tempPeak);
		
		}
	
	}
	cout << "Successfully read " << para.pf_path << endl;
}

void getReporter(){
	
	if (para.quantMethod > 0 && para.quantMethod < 4){
		cout << "\nStep4: Get " << reType[para.quantMethod - 1] << " reporter-ion`s intensity." << endl;
	}
	else{
		cout << "\nQuant-parameter: " << para.quantMethod << " is not valid!" << endl;
		cout << "Currently, we only support 1=>iTRAQ-8plex, 2=>iTRAQ-4plex, 3=>TMT-6plex!" << endl;
	}



	if (1 == para.quantMethod){			//捞iTRAQ-4plex信息

		for (int i = 0; i < psmVec.size(); i++){

			for (int j = 0; j < iTRAQ4.size(); j++){
			
				double minDis = 10000.0;
				int minDisIdx = -1;

				for (int k = 0; k < psmVec[i].peaks.size(); k++){
				
					if (fabs((iTRAQ4[j] - psmVec[i].peaks[k].mz)) < para.detaFragment*iTRAQ4[j]){
						minDisIdx = k;
						if (minDis > fabs(iTRAQ4[j] - psmVec[i].peaks[k].mz)){
							minDis = fabs(iTRAQ4[j] - psmVec[i].peaks[k].mz);
							minDisIdx = k;
						}
					}

					if (psmVec[i].peaks[k].mz > iTRAQ4[j]+2){
						break;
					}
				
				}

				if (-1 == minDisIdx)
					psmVec[i].reporter.push_back(-1.0);
				else
					psmVec[i].reporter.push_back(psmVec[i].peaks[minDisIdx].iten);
			}

			cout << "reporter: " << endl;
			for (int l = 0; l < psmVec[i].reporter.size(); l++)
				cout << psmVec[i].reporter[l] << " ";
			cout << endl;
			getchar();


		}

	}
	else if (2 == para.quantMethod){	//捞iTRAQ-8plex信息


	}
	else if (3 == para.quantMethod){	//捞TMT-6plex信息


	}


}

void correctIsotopeImpurities(){

	vector<double> newReport;
	for (int i = 0; i < psmVec.size(); i++){
	
		for (int j = 0; j < psmVec[i].reporter.size(); j++){
		
			if (psmVec[i].reporter[j] > 0){
			
				for (int k = 0; k < psmVec[i].reporter.size(); k++)
					 += ps
			
			}
		
		}
	
	}

}

void readData(){

	//Step1: 先将.pf2idx索引load到内存中管理；
	readPf2idx();

	//Step2: 再将.spectra所有PSMload到内存中；
	readPsms();

	//Step3: 读取pf2获取所有谱峰信息；
	readPf2();

	//Step4: 获取Reporter峰信息；
	getReporter();

	//Step5: 矫正工作；
	correctIsotopeImpurities();


}