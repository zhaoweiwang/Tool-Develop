#include "Head.h"

extern parainfo para;
double fdr = 0.0;

//vector<vector<int>> pf2Vec;			//pf2idx缓冲区
unordered_map<int, int> mapScanPos;		//改用哈希
vector<psmInfo> psmVec;					//PSM缓冲区

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

const bool bCorrect = true;

void readPf2idx(){
	cout << "Step2: Read .pf2idx file." << endl;

	for (int i = 0; i < psmVec.size(); i++){
	
		int scan = 0;
		int pos = 0;

		FILE* inputPf2idx = fopen(psmVec[i].pf2idx.c_str(), "rb");
		if (inputPf2idx == NULL){
			cout << "Failed to open " << psmVec[i].pf2idx << " File." << endl;
		}
		//else{
		//	cout << psmVec[i].pf2idx << " File open successfully.\n" << endl;
		//}

		while (!feof(inputPf2idx)){
			fread(&scan, sizeof(int), 1, inputPf2idx);
			fread(&pos, sizeof(int), 1, inputPf2idx);

			if (scan == psmVec[i].scan){
				psmVec[i].pf2Pos = pos;
				break;
			}
		}
		fclose(inputPf2idx);
	}
}

void readPsms(){

	cout << "Step1: Read .spectra file." << endl;
	ifstream input_spectra;
	input_spectra.open(para.input_spectra_path, ios::binary);
	if (!input_spectra.is_open()){
		cout << "Failed to open " << para.input_spectra_path << " File." << endl;
	}
	else{
		cout << para.input_spectra_path << " File open successfully." << endl;
	}

	string tempPsm;
	getline(input_spectra, tempPsm);					//过掉表头一行
	while (0.01 - fdr >= 0 && !input_spectra.eof()){

		psmInfo psm;
		getline(input_spectra, tempPsm);
		if (tempPsm == ""){
			cout << "Successfully read " << para.input_spectra_path << "." << endl;
			break;
		}												//防止最后一行是换行的处理，原则上不会出现这种情况；

		vector<int> t_pos;								//'\t'位点所在；
		for (int i = 0; i < tempPsm.size(); i++){
			if (tempPsm[i] == '\t')
				t_pos.push_back(i);
		}

		fdr = atof(tempPsm.substr(t_pos[3] + 1, t_pos[4] - t_pos[3] - 1).c_str());
		if (0.01 - fdr < 0)	break;
		else{
			if (tempPsm.substr(t_pos[14] + 1, t_pos[15] - t_pos[14] - 1) == "decoy")	  //TODO: 如果有污染库则需要加；
				continue;

			psm.title = tempPsm.substr(0, t_pos[0] - 1);
			psm.scan = atoi(tempPsm.substr(t_pos[0] + 1, t_pos[1] - t_pos[0] - 1).c_str());

			psm.pf1idx = psm.title.substr(0, psm.title.find('.')) + ".pf1idx";
			psm.pf1 = psm.title.substr(0, psm.title.find('.')) + ".pf1";
			psm.pf2idx = psm.title.substr(0, psm.title.find('.')) + "_HCDFT.pf2idx";
			psm.pf2 = psm.title.substr(0, psm.title.find('.')) + "_HCDFT.pf2";

			psm.pf1idx = para.pfidx_path + psm.pf1idx;
			psm.pf1 = para.pfidx_path + psm.pf1;
			psm.pf2idx = para.pfidx_path + psm.pf2idx;
			psm.pf2 = para.pfidx_path + psm.pf2;

			//cout << "title:" << psm.title << endl;
	/*		cout << "pf1idx:" << psm.pf1idx << endl;
			cout << "pf1:" << psm.pf1 << endl;
			cout << "pf2idx" << psm.pf2idx << endl; 
			cout << "pf2:" << psm.pf2 << endl;
			getchar();*/

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

			psmVec.push_back(psm);
		}
	}
	cout << "Successfully read " << para.input_spectra_path << "." << endl;
	cout << "Total PSM number: " << psmVec.size() << endl << endl;
}

void readPf2(){

	cout << "Step3: Read .pf2 file." << endl;


	for (int i = 0; i < psmVec.size(); i++){

		ifstream input_pf2;
		input_pf2.open(psmVec[i].pf2, ios::binary);
		if (!input_pf2.is_open()){
			cout << "Failed to open " << psmVec[i].pf2 << " File." << endl;
		}
		//else{
		//	cout << psmVec[i].pf2 << " File open successfully." << endl;
		//}

		input_pf2.seekg(psmVec[i].pf2Pos + sizeof(int), ios::beg);
		input_pf2.read((char*)&psmVec[i].peakNums, sizeof(int));

		for (int j = 0; j < psmVec[i].peakNums; j++){
			peakInfo tempPeak;
			input_pf2.read((char*)&tempPeak.mz, sizeof(double));
			input_pf2.read((char*)&tempPeak.iten, sizeof(double));

			psmVec[i].peaks.push_back(tempPeak);
		}
		input_pf2.close();
	}
	cout << "Successfully read pf2 File." << endl;
}

void getReporter(){

	if (para.quantMethod > -1 && para.quantMethod < 3){
		cout << "\nStep4: Get " << reType[para.quantMethod] << " reporter-ion`s intensity." << endl;
	}
	else{
		cout << "\nQuant-parameter: " << para.quantMethod << " is not valid!" << endl;
		cout << "Currently, we only support 1=>iTRAQ-8plex, 2=>iTRAQ-4plex, 3=>TMT-6plex!" << endl;
	}

	para.detaFragment = (para.detaFragment+180)/1000000;
	if (0 == para.quantMethod){									//捞iTRAQ-4plex信息

		for (int i = 0; i < psmVec.size(); i++){
			for (int j = 0; j < para.reporterMZ.size(); j++){
				double minDis = 10000.0;
				int minDisIdx = -1;
				for (int k = 0; k < psmVec[i].peaks.size(); k++){//A=5，B=10，B比A多100%，是（10 - 5）/5,基准一般都是真实的,被减数一般都是测量的
					if (fabs((psmVec[i].peaks[k].mz - para.reporterMZ[j])) < para.detaFragment*para.reporterMZ[j]){
						//cout << "进网的鱼儿: " << psmVec[i].peaks[k].mz << " " << psmVec[i].peaks[k].iten << endl;
						if (minDis > fabs(para.reporterMZ[j] - psmVec[i].peaks[k].mz)){
							minDis = fabs(para.reporterMZ[j] - psmVec[i].peaks[k].mz);
							minDisIdx = k;
							//cout << "被选中的孩子： " << psmVec[i].peaks[k].mz << endl;
						}
					}
					if (psmVec[i].peaks[k].mz > para.reporterMZ[j] + 2)	break;	//离开区间了就跳掉不往下找了，节省时间
					//getchar();
				}

				if (-1 == minDisIdx)											//这是在所开的区间内没有捞到峰的情况
					psmVec[i].reporter.push_back(-1.0);
				else
					psmVec[i].reporter.push_back(psmVec[i].peaks[minDisIdx].iten * 0.1);
			}

			////输出测试
			//cout << psmVec[i].title << endl;
			//cout << "reporter: " << endl;
			//for (int l = 0; l < psmVec[i].reporter.size(); l++)
			//	cout << psmVec[i].reporter[l] << " ";
			//cout << endl;
			//getchar();
		}
	}
	else if (1 == para.quantMethod){	//捞iTRAQ-8plex信息
	}
	else if (2 == para.quantMethod){	//捞TMT-6plex信息
	}
}

void correctIsotopeImpurities(){
	double tempInt = 0.0;
	if (0 == para.quantMethod){
		for (int i = 0; i < psmVec.size(); i++){
			for (int j = 0; j < para.reporterMZ.size(); j++){
				tempInt = 0.0;
				if (psmVec[i].reporter[j] > 0){
					for (int k = 0; k < para.reporterMZ.size(); k++)
						tempInt += (psmVec[i].reporter[k] * isotopeImpuritiesSolveItraq4plex[j][k]);

					if (tempInt <= 0.0){
						tempInt = -1.0;
						psmVec[i].reporterCorrect.push_back(tempInt);
					}
					else
						psmVec[i].reporterCorrect.push_back(tempInt);
				}
				else{
					psmVec[i].reporterCorrect.push_back(-1.0);
				}
			}

			//输出测试
			//cout << psmVec[i].title << endl;
			//cout << "reporter: " << endl;
			//for (int l = 0; l < psmVec[i].reporter.size(); l++)
			//	cout << psmVec[i].reporter[l] << " ";
			//cout << endl;
			//for (int l = 0; l < psmVec[i].reporterCorrect.size(); l++)
			//	cout << psmVec[i].reporterCorrect[l] << " ";
			//cout << endl;
			//getchar();
		}
	}
	else if (1 == para.quantMethod){
	}
	else if (2 == para.quantMethod){
	}
}



void readData(){

	//Step1: 再将.spectra所有PSMload到内存中；
	readPsms();

	//Step2: 获取每个PSM的pf2pos；
	readPf2idx();

	//Step3: 读取pf2获取所有谱峰信息；
	readPf2();

	//Step4: 获取Reporter峰信息；
	getReporter();

	//Step5: 矫正reporter ions强度；
	if (bCorrect){			//矫正reporter ions强度，由试剂厂商提供（参考iQuant）；
		correctIsotopeImpurities();
	}
}
