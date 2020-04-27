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
	vector<float>		av_x;
	vector<float>		av_y;
	vector<float>		av_z;
	vector<float>		heading;
	vector<float>		pitch;
	vector<float>		roll;
	vector<uint64_t>	timeStamp;
	vector<float>		distance;  //distance from the first frame of the loaded images
	int  nCurID;

	CImu() { nCurID = 0; }
	~CImu() {}

	bool readData(string ss)
	{
		acc_x.clear();
		acc_y.clear();
		acc_z.clear();
		av_x.clear();
		av_y.clear();
		av_z.clear();
		heading.clear();
		pitch.clear();
		roll.clear();
		timeStamp.clear();
		distance.clear();

		nCurID = 0;

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

			//1531179517365370000, , 366624, , 1531179517, 365211565, """base_link""", , -0.008614937, -0.005253773, -0.038020708, 0.999225974, "[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]", , -0.010124803, -0.030180408, 9.10E-05, "[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]", , -0.298508728, 0.103697958, -8.644345281, "[0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]"

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

			//angluar velocity
			av_x.push_back(stof(vecss.at(22)));
			av_y.push_back(stof(vecss.at(23)));
			av_z.push_back(stof(vecss.at(24)));

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
	int                 nCurFrameID;
	vector<string>		imgPath;		//the file paths of image
	vector<int32_t>     signposx;
	vector<int32_t>     signposy;
	vector<uint64_t>	timeStamp;		
	vector<float>		distance;		//distance from the first frame
	vector<float>       velocity;		

	CImageData() { nCurFrameID = 0; }
	~CImageData() {}

	void loadsignpos(CString& strfile)
	{
		CMap<CString, LPCTSTR, int, int> mapFile2ID;
		CString strFileName;
		for (int i = 0; i < imgPath.size(); i++)
		{
			strFileName.Format("%s", imgPath[i].c_str());
			strFileName = strFileName.Mid(strFileName.ReverseFind('\\') + 1);
			mapFile2ID.SetAt(strFileName, i);
		}

		CFileFind finder;
		if (!finder.FindFile(strfile)) return;
		
		CStdioFile file(strfile, CFile::modeRead);
		CString strText;
		while (file.ReadString(strText))
		{
			if (strText.GetLength() < 2) continue;

			strFileName = strText.Left(strText.Find(":"));
			strText = strText.Mid(strText.Find(":") + 1);

			strFileName.Replace(".png", ".bmp");

			int nID;
			if (!mapFile2ID.Lookup(strFileName, nID))
				continue;
			signposx[nID] = (atol(strText.Left(strText.Find(","))));
			strText = strText.Mid(strText.Find(",") + 1);
			signposy[nID]=(atol(strText));
		}
		file.Close();
	}

	float getImgDist(string& strFile)
	{
		auto idx = find(imgPath.begin(), imgPath.end(), strFile) - imgPath.begin();
		if (idx < 0 || idx >= imgPath.size())
			return -1;
		return distance.at(idx);
	}

	//record all file paths from root
	bool recordPaths(const string& ssRoot)
	{
		nCurFrameID = 0;
		imgPath.clear();
		timeStamp.clear();
		distance.clear();
		velocity.clear();
		signposx.clear();
		signposy.clear();

		CString strFileName;
		strFileName.Format("%s\\*.bmp", ssRoot.c_str());
		CFileFind finder;
		BOOL bWorking = finder.FindFile(strFileName);
		while (bWorking)
		{
			bWorking = finder.FindNextFile();
			strFileName = finder.GetFileName();
			
			if (strFileName.Find("m.bmp") >= 0 || strFileName.Find("s") >= 0 )
				continue;
			strFileName = finder.GetFileTitle();
			string p = format("%s", strFileName);
			timeStamp.push_back(stoull(p));
		}
		
		//sort
		int nSize = timeStamp.size();
		for (int i = 0; i < nSize - 1; ++i)
		{
			bool bExchange = false;
			for (int j = i + 1; j < nSize; ++j)
			{
				if (timeStamp[j] < timeStamp[i])
				{
					bExchange = true;
					size_t ullTmp = timeStamp[i];
					timeStamp[i] = timeStamp[j];
					timeStamp[j] = ullTmp;
				}
			}
			if (!bExchange) break;
		}

		int nx = -1;
		string p;
		assert(imgPath.empty());
		for (auto it = timeStamp.begin(); it != timeStamp.end(); ++it)
		{
			imgPath.push_back(p.assign(ssRoot).append("\\").
				append(to_string(*it)).append(".bmp"));
			signposx.push_back(nx);
			signposy.push_back(nx);
		}

		nCurFrameID = 0;
 		return true;
	}
};

#endif // !__READDATA_HPP__