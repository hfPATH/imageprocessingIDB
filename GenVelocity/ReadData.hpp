//This file is to read gps and/or imu data from csvfile
//Date- June 4, 2018   --CopyRight@PATH, CA

#pragma once
#ifndef __READDATA_HPP__
#define __READDATA_HPP__

#include <io.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include "dib.hpp"

using namespace std;

//imu_data
class CImu
{
public:
	ifstream			iFile;
	vector<float>		acc_x;
	vector<float>		acc_y;
	vector<float>		acc_z;
	vector<float>		heading;
	vector<float>		pitch;
	vector<float>		roll;
	vector<uint64_t>	timeStamp;
	vector<float>		distance;  //distance from the first frame of the loaded images

	CImu() {}
	~CImu() {}

	bool readData(string ss)
	{
		acc_x.clear();
		acc_y.clear();
		acc_z.clear();
		heading.clear();
		pitch.clear();
		roll.clear();
		timeStamp.clear();
		distance.clear();

		iFile.open(ss);

		int nRows = 0;
		string ssTmp;
		float fOrientX, fOrientY, fOrientZ, fOrientW;
		while (getline(iFile, ssTmp))
		{
			if (nRows == 0)
			{
				nRows++;
				continue;  //The first row is not required data
			}

			vector<string> vecss = split(ssTmp, ",");

			if (vecss[8] == "" || vecss[9] == "" || vecss[10] == "" ||
				vecss[11] == "" || vecss[35] == "" || vecss[36] == "" ||
				vecss[37] == "")
			{
				continue;
			}


			//time stamp
			timeStamp.push_back(stoull(vecss.at(0)));

			//Eular angles
			fOrientX = stof(vecss.at(8));
			fOrientY = stof(vecss.at(9));
			fOrientZ = stof(vecss.at(10));
			fOrientW = stof(vecss.at(11));
			vector<float> vecTmp = Quat2euler(fOrientX, fOrientY, fOrientZ, fOrientW);
			heading.push_back(vecTmp[2]);
			pitch.push_back(vecTmp[1]);
			roll.push_back(vecTmp[0]);

			//accelerate
			acc_x.push_back(stof(vecss.at(35)));
			acc_y.push_back(stof(vecss.at(36)));
			acc_z.push_back(stof(vecss.at(37)));

			ssTmp.clear();
		}
		iFile.close();

		if (timeStamp.empty())
			return false;
		return true;
	}
};


//gps_data
class CGps
{
public:
	ifstream			iFile;
	vector<float>		longitude;
	vector<float>		latitude;
	vector<uint64_t>	timeStamp;

	CGps() {}
	~CGps() {}

	bool readData(string ss)
	{
		longitude.clear();
		latitude.clear();
		timeStamp.clear();

		iFile.open(ss);
		string ssTmp;
		while (getline(iFile, ssTmp))
		{
			if (ssTmp.find("SOL_COMPUTED") == string::npos)
			{
				continue;
			}

			vector<string> vecss = split(ssTmp, ",");

			if (vecss.size() == 8)	//06_16 GPS数据格式异常，临时处理方式
			{
				string ssTmp1 = vecss[7];
				vector<string> vecTmp = split(ssTmp1, " ");
				
				timeStamp.push_back(stoull(vecss.at(0)));
				latitude.push_back(stof(vecTmp.at(7)));
				longitude.push_back(stof(vecTmp.at(8)));
			}
			else	//GPS正常时的处理方式
			{
				if (vecss.at(18) == "" || vecss.at(19) == "")
				{
					continue;
				}

				//time stamp
				timeStamp.push_back(stoull(vecss.at(0)));
				latitude.push_back(stof(vecss.at(18)));
				longitude.push_back(stof(vecss.at(19)));
			}

			ssTmp.clear();
		}
		iFile.close();

		if (timeStamp.empty())
			return false;
		return true;
	}
};

//velocity_data
class CVelocity
{
public:
	ifstream			 iFile;
	vector<float>		 velocity;
	vector<uint64_t>	 timeStamp;
	vector<float>        distance;

	CVelocity() {}
	~CVelocity() {}

	bool readData(string ss)
	{
		velocity.clear();
		timeStamp.clear();
		distance.clear();

		iFile.open(ss);

		int nRows = 0;
		string ssTmp;
		float fTmp;
		while (getline(iFile, ssTmp))
		{
			if (nRows == 0)
			{
				nRows++;
				continue;  //The first row is not required data
			}

			vector<string> vecss = split(ssTmp, ",");

			if (vecss[7] == "" || vecss[8] == "" || vecss[9] == "" || vecss[10] == "")
				continue;

			timeStamp.push_back(stoull(vecss[0]));

			//average velovity of four wheels' measured value
			fTmp = stof(vecss[7]) + stof(vecss[8]) + stof(vecss[9]) + stof(vecss[10]);
			velocity.push_back(fTmp / 4.0);

			ssTmp.clear();
		}
		iFile.close();

		if (timeStamp.empty())
			return false;
		return true;
	}

};


class CImageData
{
public:
	vector<string>		imgPath;		//the file paths of image
	vector<uint64_t>	timeStamp;		
	vector<float>		distance;		//distance from the first frame
	vector<float>       velocity;		

	CImageData() {}
	~CImageData() {}

	//record all file paths from root
	bool recordPaths(const string& ssRoot)
	{
		imgPath.clear();
		timeStamp.clear();
		distance.clear();
		velocity.clear();

		__int64  hFile = 0;
		struct _finddatai64_t fileinfo;
		string p;
		if ((hFile = _findfirst64(p.assign(ssRoot).append("\\*").c_str(), &fileinfo)) != -1)//若查找成功，则进入  
		{
			do
			{
				if ((fileinfo.attrib &  _A_SUBDIR))
					continue;
				else
				{
					//time Stamp
					string ssName = fileinfo.name;
					string ssTmp = ".";
					size_t pos = ssName.find(ssTmp);
					if (pos == string::npos)
					{
						_findclose(hFile);
						return false;
					}
					ssName.erase(pos, ssTmp.length());
					timeStamp.push_back(stoull(ssName));
				}
			} while (_findnext64(hFile, &fileinfo) == 0);
			_findclose(hFile);
		}

		//sort
		for (int i = 0; i < timeStamp.size()-1; ++i)
		for (int j = i + 1; j < timeStamp.size(); ++j)
		{
			if (timeStamp[j] < timeStamp[i])
			{
				size_t ullTmp = timeStamp[i];
				timeStamp[i] = timeStamp[j];
				timeStamp[j] = ullTmp;
			}
		}

		assert(imgPath.empty());
		for (auto it = timeStamp.begin(); it != timeStamp.end(); ++it)
		{
			imgPath.push_back(p.assign(ssRoot).append("\\").
				append(to_string(*it)).append(".bmp"));
		}

 		return true;
	}
};

#endif // !__READDATA_HPP__