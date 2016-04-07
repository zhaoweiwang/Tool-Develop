#include "Head.h"

extern parainfo para;
double fdr = 0.0;

extern unordered_map<string, double> mod2mass;

//vector<vector<int>> pf2Vec;			//pf2idx缓冲区
unordered_map<int, int> mapScanPos;		//改用哈希
vector<psmInfo> psmVec;					//PSM缓冲区

vector<string> reType{ "iTRAQ-4plex", "iTRAQ-8plex", "TMT-6plex", "TMT-10plex", "pIDL-Nplex"};
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

const double massZI = 1.00727647012;		//质子质量

vector<double> MassTable = { 71.03711, 166.99836, 103.00919, 115.02694, 129.04259, 147.06841, 57.02146, 137.05891, 113.08406, 181.01401, 128.09496, 113.08406, 131.04048, 114.04293, 243.02965, 97.05276, 128.05858, 156.10111, 87.03203, 101.04768, 0.00000, 99.06841, 186.07931, 113.08406, 163.06332, 128.55059 };

const double massO = 15.99491463;
const double massC = 12.00000;
const double A_Mass = - massC - massO;
const double B_Mass = 0.0;
const double massH = 1.007825035;
const double massH2O = massH * 2 + massO;
const double Y_Mass = massH2O;


void readPf2idx(){

	cout << "Step2: Read .pf2idx file." << endl;

	for (int i = 0; i < psmVec.size(); i++){
	
		int scan = 0;
		int pos = 0;

		FILE* inputPf2idx = fopen(psmVec[i].pf2idx.c_str(), "rb");
		if (inputPf2idx == NULL){
			cout << "Failed to open " << psmVec[i].pf2idx << " File." << endl;
		}

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

	cout << "\nStep1: Read .spectra file." << endl;

	ifstream input_spectra;
	input_spectra.open(para.input_spectra_path, ios::binary);

	if (!input_spectra.is_open()){
		cout << "Failed to open " << para.input_spectra_path << " File." << endl;
		exit(0);
	}else{
		cout << para.input_spectra_path << " File open successfully." << endl;
	}

	string tempPsm;
	getline(input_spectra, tempPsm);					//过掉表头一行
	while (para.PsmFDR - fdr >= 0 && !input_spectra.eof()){

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

			psm.title = tempPsm.substr(0, t_pos[0]);
			psm.scan = atoi(tempPsm.substr(t_pos[0] + 1, t_pos[1] - t_pos[0] - 1).c_str());

			psm.pf1idx = psm.title.substr(0, psm.title.find('.')) + ".pf1idx";
			psm.pf1 = psm.title.substr(0, psm.title.find('.')) + ".pf1";
			psm.pf2idx = psm.title.substr(0, psm.title.find('.')) + "_HCDFT.pf2idx";
			psm.pf2 = psm.title.substr(0, psm.title.find('.')) + "_HCDFT.pf2";

			psm.pf1idx = para.pf_path + psm.pf1idx;
			psm.pf1 = para.pf_path + psm.pf1;
			psm.pf2idx = para.pf_path + psm.pf2idx;
			psm.pf2 = para.pf_path + psm.pf2;

			//cout << "title:" << psm.title << endl;
			//cout << "pf1idx:" << psm.pf1idx << endl;
			//cout << "pf1:" << psm.pf1 << endl;
			//cout << "pf2idx" << psm.pf2idx << endl; 
			//cout << "pf2:" << psm.pf2 << endl;
			//getchar();

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

	cout << "\nStep3: Read .pf2 file." << endl;

	for (int i = 0; i < psmVec.size(); i++){

		ifstream input_pf2;
		input_pf2.open(psmVec[i].pf2, ios::binary);
		if (!input_pf2.is_open()){
			cout << "Failed to open " << psmVec[i].pf2 << " File." << endl;
		}

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

	cout << "Successfully read all .pf2 File." << endl;
}

int getpIDLplexPPM(double mass, int *mass_inten, double &mass_error, int i){
	int start = (int)(mass - mass*para.detaFragment);
	int end = (int)(mass + mass*para.detaFragment);
	if (start <= 0 || end >= 10000)
		return -1;
	double max_intensity = DBL_MIN;
	int max_k = -1;
	for (int k = mass_inten[start - 1]; k < mass_inten[end]; ++k){
		double tmpd_error = fabs(((psmVec[i].peaks[k].mz - mass) / mass));
		double tmpd = psmVec[i].peaks[k].iten;
		if (tmpd_error <= para.detaFragment && tmpd > max_intensity){
			max_intensity = tmpd;
			max_k = k;
		}
	}

	if (max_k != -1){
		mass_error = (psmVec[i].peaks[max_k].mz - mass) * 1.0e6 / mass;
	}

	return max_k;
}

int getpIDLplexDa(double mass, int *mass_inten, double &mass_error, int i){
	int start = (int)(mass - para.detaFragment);
	int end = (int)(mass + para.detaFragment);
	if (start <= 0 || end >= 10000)
		return -1;
	double max_intensity = DBL_MIN;
	int max_k = -1;
	for (int k = mass_inten[start - 1]; k < mass_inten[end]; ++k){
		double tmpd_error = fabs(((psmVec[i].peaks[k].mz - mass) / mass));
		double tmpd = psmVec[i].peaks[k].iten;
		if (tmpd_error <= para.detaFragment && tmpd > max_intensity){
			max_intensity = tmpd;
			max_k = k;
		}
	}

	if (max_k != -1){
		mass_error = (psmVec[i].peaks[max_k].mz - mass) - mass;
	}

	return max_k;
}

void calpIDL(){
	for (int i = 0; i < psmVec.size(); ++i){

		//cout << psmVec[i].pepSq << endl;
		//cout << psmVec[i].modification << endl;

		vector<vector<peakInfo>> peaks;
		peaks.resize(para.pIDLplex.size());

		modificationInfo singleMod;
		if (!psmVec[i].modification.empty()){
			string tempMod = psmVec[i].modification;
			while (tempMod.find(';') != string::npos){
				string temp = tempMod.substr(0, tempMod.find(','));
				singleMod.index = atoi(temp.c_str());

				temp = tempMod.substr(tempMod.find(',') + 1, tempMod.find(';') - tempMod.find(',') - 1);
				singleMod.name = temp;
				singleMod.mass = mod2mass[temp];
				psmVec[i].mod.push_back(singleMod);
				tempMod = tempMod.substr(tempMod.find(';') + 1);
			}
		}

		vector<double> all_mass;								//all_mass存用户设置的修饰质量对
		for (int j = 0; j < para.pIDLplex.size(); ++j){
			all_mass.push_back(para.pIDLplex[j].massN);
			all_mass.push_back(para.pIDLplex[j].massC);
		}

		vector<double> mod_mass;							//mod_mass存肽段氨基酸修饰发生与否质量表
		for (int j = 0; j < psmVec[i].pepSq.size() + 2; j++){
			mod_mass.push_back(0.0);
		}
		const double minGap = 0.00001;
		for (int j = 0; j < psmVec[i].mod.size(); j++){
			bool inFlag = false;
			for (int k = 0; k < all_mass.size(); ++k){
				if (fabs(all_mass[k] - psmVec[i].mod[j].mass) <= minGap){
					inFlag = true;
					break;
				}
			}
			if (!inFlag)
				mod_mass[psmVec[i].mod[j].index] = psmVec[i].mod[j].mass;
		}


		int mass_inten[10000] = { 0 };
		for (int j = 0; j < psmVec[i].peaks.size(); ++j){
			int massi = (int)(psmVec[i].peaks[j].mz);
			mass_inten[massi] = j + 1;
		}
		int currindex = 0;
		for (int j = 0; j < 10000; ++j){
			if (mass_inten[j] == 0)
				mass_inten[j] = currindex;
			else
				currindex = mass_inten[j];
		}

		//开始捞谱峰
		for (int j = 0; j < para.pIDLplex.size(); ++j){
			mod_mass[0] = para.pIDLplex[j].massN;
			for (int k = 0; k < psmVec[i].pepSq.size(); ++k){
				if (psmVec[i].pepSq[k] == 'K' || psmVec[i].pepSq[k] == 'k')
					mod_mass[k + 1] = para.pIDLplex[j].massC;
			}


			double tmpmass = mod_mass[0];
			double a1 = tmpmass + MassTable[psmVec[i].pepSq[0] - 'A'] + mod_mass[1] + A_Mass + massZI;
			double mass_error0 = 0.0;
			int index0 = -1;

			if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
				index0 = getpIDLplexPPM(a1, mass_inten, mass_error0, i);
			else
				index0 = getpIDLplexDa(a1, mass_inten, mass_error0, i);

			peakInfo tmp;
			tmp.mz = a1;
			tmp.iten = 0.0;
			if (index0 != -1){
				peaks[j].push_back(psmVec[i].peaks[index0]);
				psmVec[i].annotation.push_back("a1+");

				psmVec[i].a1Iten.push_back(psmVec[i].peaks[index0].iten);
			}
			else{
				peaks[j].push_back(tmp);

				psmVec[i].a1Iten.push_back(0.0);
			}

			//cout << a1 << ":" << psmVec[i].peaks[index0].iten << endl;

			//获取b离子列表
			for (int k = 0; k < psmVec[i].pepSq.size(); ++k){
				tmpmass += MassTable[psmVec[i].pepSq[k] - 'A'] + mod_mass[k + 1];
				double bmass1 = tmpmass + B_Mass + massZI;
				double mass_error = 0.0;
				int index = -1;
				if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
					index = getpIDLplexPPM(bmass1, mass_inten, mass_error, i);
				else
					index0 = getpIDLplexDa(bmass1, mass_inten, mass_error0, i);


				tmp.mz = bmass1;
				if (index != -1){
					peaks[j].push_back(psmVec[i].peaks[index]);
					psmVec[i].annotation.push_back("b" + to_string(k+1) + "+");
				}
				else
					peaks[j].push_back(tmp);

				double bmass2 = (tmpmass + B_Mass + 2 * massZI) / 2.0;
				if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
					index = getpIDLplexPPM(bmass2, mass_inten, mass_error, i);
				else
					index0 = getpIDLplexDa(bmass2, mass_inten, mass_error0, i);

				tmp.mz = bmass2;
				if (index != -1){
					peaks[j].push_back(psmVec[i].peaks[index]);
					psmVec[i].annotation.push_back("b" + to_string(k + 1) + "++");
				}
				else
					peaks[j].push_back(tmp);
			}

			//获取y离子列表
			tmpmass = mod_mass[psmVec[i].pepSq.size() - 1];
			for (int k = psmVec[i].pepSq.size() - 1; k > 0; --k){
				tmpmass += MassTable[psmVec[i].pepSq[k] - 'A'] + mod_mass[j + 1];
				double ymass1 = tmpmass + Y_Mass + massZI;
				double mass_error = 0.0;
				int index = -1;

				if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
					index = getpIDLplexPPM(ymass1, mass_inten, mass_error, i);
				else
					index0 = getpIDLplexDa(ymass1, mass_inten, mass_error0, i);

				tmp.mz = ymass1;
				if (index != -1){
					peaks[j].push_back(psmVec[i].peaks[index]);
					psmVec[i].annotation.push_back("y" + to_string(psmVec[i].pepSq.size() - k) + "+");
				}
				else
					peaks[j].push_back(tmp);

				double ymass2 = (tmpmass + Y_Mass + 2 * massZI) / 2;

				if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
					index = getpIDLplexPPM(ymass2, mass_inten, mass_error, i);
				else
					index0 = getpIDLplexDa(ymass2, mass_inten, mass_error0, i);

				tmp.mz = ymass2;
				if (index != -1){
					peaks[j].push_back(psmVec[i].peaks[index]);
					psmVec[i].annotation.push_back("y" + to_string(psmVec[i].pepSq.size() - k) + "++");
				}
				else
					peaks[j].push_back(tmp);

			}
		}


		vector<double> a1_ratio;
		vector<double> all_ratio;
		double fz = 0.0, fm = 0.0;
		for (int k = 1; k < para.pIDLplex.size(); ++k){
			for (int j = 0; j < peaks[k].size(); ++j){
				if (peaks[0][j].iten != 0.0 && peaks[k][j].iten != 0.0 &&
					peaks[0][j].mz != peaks[k][j].mz){
					//if (peaks[0][j].mz >= para.pIDLplex[0].massN && peaks[0][j].mz <= para.pIDLplex[0].massC &&
						//peaks[k][j].mz >= para.pIDLplex[k].massN && peaks[k][j].mz <= para.pIDLplex[k].massC){
						fz += peaks[k][j].iten;
						fm += peaks[0][j].iten;
					//}
				}
			}

			//计算a1离子ratio
			if (peaks[0][0].iten != 0.0)
				a1_ratio.push_back(peaks[k][0].iten / peaks[0][0].iten);
			else
				a1_ratio.push_back(0.0);

			//计算by离子ratio
			if (fm != 0.0)
				all_ratio.push_back(fz / fm);
			else
				all_ratio.push_back(0.0);

		}
		psmVec[i].a1Ratio.swap(a1_ratio);
		psmVec[i].allRatio.swap(all_ratio);


		////输出测试
		//for (int j = 0; j < psmVec[i].a1Ratio.size(); ++j)
		//	cout << psmVec[i].a1Ratio[j] << " ";
		//cout << endl;

		//for (int j = 0; j < psmVec[i].allRatio.size(); ++j)
		//	cout << psmVec[i].allRatio[j] << " ";
		//cout << endl;
		//getchar();
	}
	
}

void getReporter(){

	if (para.quantMethod > -1 && para.quantMethod < 5){
		cout << "\nStep4: Get " << reType[para.quantMethod] << " reporter-ion`s intensity." << endl;
	}
	else{
		cout << "\nQuant-parameter: " << para.quantMethod << " is not valid!" << endl;
		cout << "Currently, we only support 0=>iTRAQ-4plex, 1=>iTRAQ-6plex, 2=>TMT-6plex, 3=>TMT-10plex, 4=>pIDL-nplex!" << endl << endl;
		exit(0);
	}

	if (para.FTMSType == "ppm" || para.FTMSType == "PPM" || para.FTMSType == "Ppm")
		para.detaFragment = (para.detaFragment)/1000000;

	//TODO: iTRAQ和TMT方法中需要考虑Da匹配

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
	else if (3 == para.quantMethod){
	
	}
	else if (4 == para.quantMethod){
		readModification();				//获取修饰-质量表
		calpIDL();
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

	readPsms();									//Step1: 再将.spectra所有PSMload到内存中

	readPf2idx();								//Step2: 获取每个PSM的pf2pos

	readPf2();										//Step3: 读取pf2获取所有谱峰信息

	getReporter();								//Step4: 获取Reporter峰信息

	if (para.correct && (para.quantMethod == 0 || para.quantMethod == 1 || para.quantMethod == 2)){					
		correctIsotopeImpurities();		//Step5: 矫正reporter ions强度
	}
}
