#include "stringProcess.h"


void stringProcess::Trim(string& str) {
	int i;
	for (i = 0; i < (int)str.length() && bIsNoUse(str.at(i)); ++i)
		;
	if (i == (int)str.length()) {
		str.erase(0, i - 1);
		return;
	}
	str.erase(0, i);
	for (i = str.length() - 1; i >= 0 && bIsNoUse(str.at(i)); --i)
		;
	str.erase(i + 1, str.length() - 1 - i);
	return;
}

void stringProcess::Trim(const char *&fptr, const char *&lptr) {
	while (fptr < lptr && bIsNoUse(*fptr))
		++fptr;
	while (fptr < lptr && bIsNoUse(*(lptr - 1)))
		--lptr;
	return;
}

bool stringProcess::bIsNoUse(const char ch) {
	if (' ' == ch || '\r' == ch || '\t' == ch) {
		return true;
	}
	return false;
}

bool stringProcess::bIsNumber(const char ch) {
	if ('0' <= ch && '9' >= ch) {
		return true;
	}
	return false;
}

void stringProcess::ToLower(string& str) {
	for (size_t i = 1; i < str.size(); i++) {
		str[i] = tolower(str[i]);
	}
}

void stringProcess::Split(const string& strFullString, const string& strSign,
	string& strPrior, string& strLatter) {
	size_t i = strFullString.find(strSign);
	if (i != string::npos) {
		strPrior = strFullString.substr(0, i);
		strLatter = strFullString.substr(i + strSign.length(),
			strFullString.length() - i - strSign.length());
	}
	else {
		strPrior = strFullString;
		strLatter.clear();
	}
}

bool stringProcess::bMatchingFix(string strFullString, string strFix,
	bool bSuffix, bool bCaseSensitive) {
	size_t tLength = strFix.size();
	if (tLength >= strFullString.size()) {
		return false;
	}
	if (bSuffix) {
		strFullString.erase(strFullString.begin(), strFullString.end()
			- tLength);
	}
	else {
		strFullString.erase(strFullString.begin() + tLength,
			strFullString.end());
	}
	if (!bCaseSensitive) {
		stringProcess::ToLower(strFullString);
		stringProcess::ToLower(strFix);
	}
	return strFullString == strFix;
}


bool helper(int i, int j, vector<vector<char> > &board, string word, vector<vector<bool> > &route){

	if (word == "") return true;

	int m = board.size();
	int n = board[0].size();

	route[i][j] = true;


	while (i < m && j < n){
		//up
		if (i - 1 >= 0 && !route[i - 1][j] && word[0] == board[i - 1][j])
		if (helper(i - 1, j, board, word.substr(1, word.length() - 1), route))
			return true;
		else {

			route[i - 1][j] = false;
		}
		//down
		if (i + 1 < m && !route[i + 1][j] && word[0] == board[i + 1][j])
		if (helper(i + 1, j, board, word.substr(1, word.length() - 1), route))
			return true;
		else {

			route[i + 1][j] = false;
		}
		//left
		if (j - 1 >= 0 && !route[i][j - 1] && word[0] == board[i][j - 1])
		if (helper(i, j - 1, board, word.substr(1, word.length() - 1), route))
			return true;
		else {

			route[i][j - 1] = false;
		}
		//right
		if (j + 1 < n && !route[i][j + 1] && word[0] == board[i][j + 1])
		if (helper(i, j + 1, board, word.substr(1, word.length() - 1), route))
			return true;
		else {

			route[i][j + 1] = false;
		}
		break;
	}

	return false;
}

bool exist(vector<vector<char> > &board, string word) {

	if (word == "") return true;
	if (board.empty()) return false;

	int m = board.size();
	int n = board[0].size();

	vector<vector<bool> >route(m, vector<bool>(n, false));

	for (int i = 0; i < m; ++i)
	{
		for (int j = 0; j < n; j++)
		{
			if (board[i][j] == word[0])
			if (helper(i, j, board, word.substr(1, word.length() - 1), route))
				return true;
			else route[i][j] = false;
		}
	}

	return false;
}

class Solution {
public:

	void dfs(int step, int sum, vector<int>& candidates, int target, int vector<vector<int>>& result, vector<int>& temp){
		if (sum == target){
			result.push_back(temp);
			return;
		}

		for (int i = step; i < candidates.size(); i++){
			if (sum + candidates[i] > target)
				return;
			temp.push_back(candidates[i]);
			dfs(i + 1, sum, candidates, target, result, temp);
			temp.pop_back(candidates[i]);
		}
		return;
	}

	vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
		vector<vector<int>> result;
		vector<int> temp;

		if (candidates.empty()) return result;

		sort(candidates.begin(), candidates.end());
		dfs(0, 0, candidates, target, result, temp);

		return result;
	}
};