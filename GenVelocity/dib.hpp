//This file contains some functions outward from any class
//Date- June 4, 2018   --CopyRight@PATH, CA

#pragma once
#ifndef __DIB_HPP__
#define __DIB_HPP__

#include <io.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>

using namespace std;

//split string with pattern
static vector<string> split(string str, string pattern)
{
	string::size_type pos;
	vector<string> result;
	str += pattern;
	int size = str.size();

	for (int i = 0; i<size; i++)
	{
		pos = str.find(pattern, i);
		if (pos<size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

//calculate eular angles, sequence: roll, pitch, heading
static vector<float> Quat2euler(float x, float y, float z, float w)
{
	vector<float> vecTmp;
	vecTmp.push_back(atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)));
	vecTmp.push_back(asin(2 * (w * y - z * x)));
	vecTmp.push_back(atan2(2 * (w * z + x * y), 1 - 2 * (z * z + y * y)));

	return vecTmp;
}


#endif // !__DIB_HPP__
