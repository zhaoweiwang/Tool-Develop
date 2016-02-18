#include "Head.h"

extern parainfo para;
extern vector<psmInfo> psmVec;					//PSM缓冲区

vector<double> medianRatio;						//median of Ratio
vector<double> stdRatio;						//std of Ratio
vector<double> medianRatioCorrect;				//median of RatioCorrect
vector<double> stdRatioCorrect;					//std of RatioCorrect

inline bool comp(const double& a, const double& b){
	return a < b;
}

inline bool compScore(const psmInfo& a, const psmInfo& b){
	return a.score < b.score;
}

bool validRatio(const psmInfo& a){
	for (int i = 0; i < a.ratioReporter.size(); i++){
		if (1024 == a.ratioReporter[i])
			return false;
	}
	return true;
}

bool validRatioCorrect(const psmInfo& a){
	for (int i = 0; i < a.ratioReporterCorrect.size(); i++){
		if (1024 == a.ratioReporterCorrect[i])
			return false;
	}
	return true;
}

void calMedianStd(){						//需要去除非数PSMs结果
	
	size_t size = psmVec.size();
	for (int i = 0; i < psmVec[0].ratioReporter.size(); i++){
		vector<double> ratioTemp;
		double median = 0.0;
		double std = 0.0;
		double sumRatio = 0.0;
		for (int j = 0; j < size; j++){
			if (validRatio(psmVec[j])){
				ratioTemp.push_back(psmVec[j].ratioReporter[i]);
				sumRatio += psmVec[j].ratioReporter[i];
			}
		}
		
		sort(ratioTemp.begin(), ratioTemp.end(), comp);
		if (size % 2 == 0)
			median = (ratioTemp[size / 2 - 1] + ratioTemp[size / 2]) / 2;
		else
			median = ratioTemp[size / 2];
		medianRatio.push_back(median);

		double meanRatio = sumRatio / ratioTemp.size();
		double sTemp = 0.0;
		for (int j = 0; j < ratioTemp.size(); j++)
			sTemp += (ratioTemp[j]-meanRatio)*(ratioTemp[j]-meanRatio);
		std = sqrt(sTemp / ratioTemp.size());
		stdRatio.push_back(std);
	}
	//cout << "median: ";
	//for (int i = 0; i < medianRatio.size(); i++)
	//	cout << medianRatio[i] << " ";
	//cout << endl;
	//cout << "std: ";
	//for (int i = 0; i < stdRatio.size(); i++)
	//	cout << stdRatio[i] << " ";
	//cout << endl;
	//getchar();

	//sort(psmVec.begin(), psmVec.end(), compScore);

	for (int i = 0; i < psmVec[0].ratioReporterCorrect.size(); i++){
		vector<double> ratioCorrectTemp;
		double median = 0.0;
		double std = 0.0;
		double sumRatio = 0.0;
		for (int j = 0; j < size; j++){
			if (validRatioCorrect(psmVec[j])){
				ratioCorrectTemp.push_back(psmVec[j].ratioReporterCorrect[i]);
				sumRatio += psmVec[j].ratioReporterCorrect[i];
			}
		}

		sort(ratioCorrectTemp.begin(), ratioCorrectTemp.end(), comp);
		if (size % 2 == 0)
			median = (ratioCorrectTemp[size / 2 - 1] + ratioCorrectTemp[size / 2]) / 2;
		else
			median = ratioCorrectTemp[size / 2];
		medianRatioCorrect.push_back(median);

		double meanRatio = sumRatio / ratioCorrectTemp.size();
		double sTemp = 0.0;
		for (int j = 0; j < ratioCorrectTemp.size(); j++)
			sTemp += (ratioCorrectTemp[j] - meanRatio)*(ratioCorrectTemp[j] - meanRatio);
		std = sqrt(sTemp / ratioCorrectTemp.size());
		stdRatioCorrect.push_back(std);
	}
	//cout << "medianCorrect: ";
	//for (int i = 0; i < medianRatioCorrect.size(); i++)
	//	cout << medianRatioCorrect[i] << " ";
	//cout << endl;
	//cout << "stdCorrect: ";
	//for (int i = 0; i < stdRatioCorrect.size(); i++)
	//	cout << stdRatioCorrect[i] << " ";
	//cout << endl;
	//getchar();
}

void calcuReporter(){
	cout << "\nStep5: Calculating reporter-ion`s intensity Ratio." << endl << endl;
	for (int i = 0; i < psmVec.size(); i++){
		for (int j = 1; j < psmVec[0].reporter.size(); j++){
			if (-1.0 == psmVec[i].reporter[0]){
				psmVec[i].ratioReporter.push_back(1024);				//114不存在的情况下，赋NAN无穷大给三个比值
				continue;
			}
			if (-1.0 != psmVec[i].reporter[j])
				psmVec[i].ratioReporter.push_back(log(psmVec[i].reporter[j] / psmVec[i].reporter[0]));
			else
				psmVec[i].ratioReporter.push_back(1024);
		}

		//计算Correct Reporter的Ratio
		for (int j = 1; j < psmVec[0].reporterCorrect.size(); j++){
			if (-1.0 == psmVec[i].reporterCorrect[0]){
				psmVec[i].ratioReporterCorrect.push_back(1024);				//114不存在的情况下，赋NAN无穷大给三个比值
				continue;
			}
			if (-1.0 != psmVec[i].reporterCorrect[j])
				psmVec[i].ratioReporterCorrect.push_back(log(psmVec[i].reporterCorrect[j] / psmVec[i].reporterCorrect[0]));
			else
				psmVec[i].ratioReporterCorrect.push_back(1024);
		}

		////输出测试
		//cout << "title: " << psmVec[i].title << endl;
		//cout << "reporter: ";
		//for (int l = 0; l < psmVec[i].reporter.size(); l++)
		//	cout << psmVec[i].reporter[l] << " ";
		//cout << "  reporterCorrect: ";
		//for (int l = 0; l < psmVec[i].reporterCorrect.size(); l++)
		//	cout << psmVec[i].reporterCorrect[l] << " ";
		//cout << endl;
		//cout << "Ratio: ";
		//for (int k = 0; k < psmVec[i].ratioReporter.size(); k++)
		//	cout << psmVec[i].ratioReporter[k] << " ";
		//cout << "  RatioCorrect: ";
		//for (int k = 0; k < psmVec[i].ratioReporterCorrect.size(); k++)
		//	cout << psmVec[i].ratioReporterCorrect[k] << " ";
		//cout << endl;

		//getchar();
	}

	//计算median
	calMedianStd();
}