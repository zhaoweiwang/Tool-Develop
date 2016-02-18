#ifndef _STRINGPROCESSOR_H_
#define _STRINGPROCESSOR_H_

#include <string>
#include <vector>
using namespace std;

class stringProcess {

public:

	static void Trim(string& str);
	static void Trim(const char *&fptr, const char *&lptr);
	static bool bIsNoUse(const char ch);
	static bool bIsNumber(const char ch);
	static void ToLower(string& str);
	static void Split(const string& strFullString, const string& strSign, string& strPrior, string& strLatter);
	static bool bMatchingFix(string strFullString, string strFix, bool bSuffix, bool bCaseSensitive);

};

#endif