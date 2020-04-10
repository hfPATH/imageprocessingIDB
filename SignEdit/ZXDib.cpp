// ZXDib.cpp: implementation of the CZXDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ZXDib.h"
#include <math.h>
#include <vector>
using namespace std;
using std::complex;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

vector<double> RecordPt;
vector<int> PLParts;


//0-3.99, 步长0.01
double g_pNormal[] = {
		0.5000,	0.5040,	0.5080,	0.5120,	0.5160,	0.5199,	0.5239,	0.5279,	0.5319,	0.5359,	
		0.5398,	0.5438,	0.5478,	0.5517,	0.5557,	0.5596,	0.5636,	0.5675,	0.5714,	0.5753,	
		0.5793,	0.5832,	0.5871,	0.5910,	0.5948,	0.5987,	0.6026,	0.6064,	0.6103,	0.6141,	
		0.6179,	0.6217,	0.6255,	0.6293,	0.6331,	0.6368,	0.6406,	0.6443,	0.6480,	0.6517,	
		0.6554,	0.6591,	0.6628,	0.6664,	0.6700,	0.6736,	0.6772,	0.6808,	0.6844,	0.6879,	
		0.6915,	0.6950,	0.6985,	0.7019,	0.7054,	0.7088,	0.7123,	0.7157,	0.7190,	0.7224,	
		0.7257,	0.7291,	0.7324,	0.7357,	0.7389,	0.7422,	0.7454,	0.7486,	0.7517,	0.7549,	
		0.7580,	0.7611,	0.7642,	0.7673,	0.7703,	0.7734,	0.7764,	0.7794,	0.7823,	0.7852,	
		0.7881,	0.7910,	0.7939,	0.7967,	0.7995,	0.8023,	0.8051,	0.8078,	0.8106,	0.8133,	
		0.8159,	0.8186,	0.8212,	0.8238,	0.8264,	0.8289,	0.8315,	0.8340,	0.8365,	0.8389,	
		0.8413,	0.8438,	0.8461,	0.8485,	0.8508,	0.8531,	0.8554,	0.8577,	0.8599,	0.8621,	
		0.8643,	0.8665,	0.8686,	0.8708,	0.8729,	0.8749,	0.8770,	0.8790,	0.8810,	0.8830,	
		0.8849,	0.8869,	0.8888,	0.8907,	0.8925,	0.8944,	0.8962,	0.8980,	0.8997,	0.9015,	
		0.9032,	0.9049,	0.9066,	0.9082,	0.9099,	0.9115,	0.9131,	0.9147,	0.9162,	0.9177,	
		0.9192,	0.9207,	0.9222,	0.9236,	0.9251,	0.9265,	0.9278,	0.9292,	0.9306,	0.9319,	
		0.9332,	0.9345,	0.9357,	0.9370,	0.9382,	0.9394,	0.9406,	0.9418,	0.9430,	0.9441,	
		0.9452,	0.9463,	0.9474,	0.9484,	0.9495,	0.9505,	0.9515,	0.9525,	0.9535,	0.9545,	
		0.9554,	0.9564,	0.9573,	0.9582,	0.9591,	0.9599,	0.9608,	0.9616,	0.9625,	0.9633,	
		0.9641,	0.9648,	0.9656,	0.9664,	0.9671,	0.9678,	0.9686,	0.9693,	0.9700,	0.9706,	
		0.9713,	0.9719,	0.9726,	0.9732,	0.9738,	0.9744,	0.9750,	0.9756,	0.9762,	0.9767,	
		0.9772,	0.9778,	0.9783,	0.9788,	0.9793,	0.9798,	0.9803,	0.9808,	0.9812,	0.9817,	
		0.9821,	0.9826,	0.9830,	0.9834,	0.9838,	0.9842,	0.9846,	0.9850,	0.9854,	0.9857,	
		0.9861,	0.9864,	0.9868,	0.9871,	0.9874,	0.9878,	0.9881,	0.9884,	0.9887,	0.9890,	
		0.9893,	0.9896,	0.9898,	0.9901,	0.9904,	0.9906,	0.9909,	0.9911,	0.9913,	0.9916,	
		0.9918,	0.9920,	0.9922,	0.9925,	0.9927,	0.9929,	0.9931,	0.9932,	0.9934,	0.9936,	
		0.9938,	0.9940,	0.9941,	0.9943,	0.9945,	0.9946,	0.9948,	0.9949,	0.9951,	0.9952,	
		0.9953,	0.9955,	0.9956,	0.9957,	0.9959,	0.9960,	0.9961,	0.9962,	0.9963,	0.9964,	
		0.9965,	0.9966,	0.9967,	0.9968,	0.9969,	0.9970,	0.9971,	0.9972,	0.9973,	0.9974,	
		0.9974,	0.9975,	0.9976,	0.9977,	0.9977,	0.9978,	0.9979,	0.9979,	0.9980,	0.9981,	
		0.9981,	0.9982,	0.9982,	0.9983,	0.9984,	0.9984,	0.9985,	0.9985,	0.9986,	0.9986,	
		0.9987,	0.9990,	0.9993,	0.9995,	0.9997,	0.9998,	0.9998,	0.9999,	0.9999,	1.0000
};

//0-3.19, 步长0.01
double g_pNormalProb[] = {
	.0000,	.0040,	.0080,	.0120,	.0159,	.0199,	.0239,	.0279,	.0319,	.0359,
	.0398,	.0438,	.0478,	.0517,	.0557,	.0569,	.0636,	.0675,	.0714,	.0753,
	.0793,	.0832,	.0871,	.0910,	.0948,	.0987,	.1026,	.1064,	.1103,	.1141,
	.1179,	.1217,	.1255,	.1293,	.1331,	.1368,	.1406,	.1443,	.1480,	.1517,
	.1554,	.1591,	.1628,	.1664,	.1700,	.1736,	.1772,	.1808,	.1844,	.1879,
	.1915,	.1950,	.1985,	.2019,	.2054,	.2088,	.2123,	.2157,	.2190,	.2224,
	.2257,	.2291,	.2324,	.2357,	.2389,	.2422,	.2454,	.2486,	.2518,	.2549,
	.2580,	.2612,	.2642,	.2673,	.2704,	.2734,	.2764,	.2794,	.2823,	.2852,
	.2881,	.2910,	.2939,	.2967,	.2995,	.3023,	.3051,	.3078,	.3106,	.3133,
	.3159,	.3186,	.3212,	.3238,	.3264,	.3289,	.3315,	.3340,	.3365,	.3389,
	.3413,	.3438,	.3461,	.3485,	.3508,	.3531,	.3554,	.3577,	.3599,	.3621,
	.3643,	.3665,	.3686,	.3708,	.3729,	.3749,	.3770,	.3790,	.3810,	.3830,
	.3849,	.3869,	.3888,	.3907,	.3925,	.3944,	.3962,	.3980,	.3997,	.4015,
	.4032,	.4049,	.4066,	.4083,	.4099,	.4115,	.4131,	.4147,	.4162,	.4177,
	.4192,	.4207,	.4222,	.4236,	.4251,	.4265,	.4279,	.4292,	.4306,	.4319,
	.4332,	.4345,	.4357,	.4370,	.4382,	.4394,	.4406,	.4418,	.4430,	.4441,
	.4452,	.4461,	.4474,	.4485,	.4495,	.4505,	.4515,	.4525,	.4535,	.4545,
	.4554,	.4564,	.4573,	.4582,	.4591,	.4599,	.4608,	.4616,	.4625,	.4633,
	.4641,	.4649,	.4656,	.4664,	.4671,	.4678,	.4686,	.4693,	.4699,	.4706,
	.4713,	.4719,	.4726,	.4732,	.4738,	.4744,	.4750,	.4758,	.4762,	.4767,
	.4773,	.4778,	.4783,	.4788,	.4793,	.4798,	.4803,	.4808,	.4812,	.4817,
	.4821,	.4826,	.4830,	.4834,	.4838,	.4842,	.4846,	.4850,	.4854,	.4857,
	.4861,	.4865,	.4868,	.4871,	.4875,	.4878,	.4881,	.4884,	.4887,	.4890,
	.4893,	.4896,	.4898,	.4901,	.4904,	.4906,	.4909,	.4911,	.4913,	.4916,
	.4918,	.4920,	.4922,	.4925,	.4927,	.4929,	.4931,	.4932,	.4934,	.4936,
	.4938,	.4940,	.4941,	.4943,	.4945,	.4946,	.4948,	.4949,	.4951,	.4952,
	.4953,	.4955,	.4956,	.4957,	.4959,	.4960,	.4961,	.4962,	.4963,	.4964,
	.4965,	.4966,	.4967,	.4968,	.4969,	.4970,	.4971,	.4972,	.4973,	.4974,
	.4974,	.4975,	.4976,	.4977,	.4977,	.4978,	.4979,	.4980,	.4980,	.4981,
	.4981,	.4982,	.4983,	.4984,	.4984,	.4984,	.4985,	.4985,	.4986,	.4986,
	.4986,	.4987,	.4987,	.4988,	.4988,	.4988,	.4989,	.4989,	.4989,	.4990,
	.4990,	.4991,	.4991,	.4991,	.4992,	.4992,	.4992,	.4992,	.4993,	.4993
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZXDib::CZXDib()
{

}

CZXDib::~CZXDib()
{

}
////////////////////////////////////////////////////////////////////////////
// 从内存中装入序列图像数据到指定的矩阵: （只对256色和24位真彩色）
// FileName 是开始图像名字
// pOutput 是待存储的图像二维向量的指针地址
// dwWidth , dwHeight 分别是图像的宽度和长度
//////////////////////////////////////////////////////////////////////////////
bool CZXDib::LoadMultiImageFromGivenFileName(CString strFileName,int nFileNumber,DWORD &dwHeight,DWORD &dwWidth,BYTE** &ppOutput)
{
	//在使用之间先将输出矩阵清空
    if(ppOutput) return false;

	int i,j,nImageSize;
	WORD flag;  //图片类型：8，24两种
	BYTE* pTemp=NULL;
	//申请空间：
	ppOutput = (BYTE**) new BYTE* [nFileNumber];
	

	for(i=0;i<nFileNumber;i++)
	{
		//读文件：
		if(!LoadVectorFromBMPFile(strFileName,pTemp,dwHeight,dwWidth,flag))
		{   //释放空间：
			if(pTemp) delete pTemp;
			for(j=0;j<i;j++)  delete ppOutput[j];
			delete ppOutput;
			return false;
		}
		FindNextFileName(strFileName,1);

		nImageSize = dwHeight*dwWidth;
		ppOutput[i] = new BYTE [nImageSize];

		for(j=0;j<nImageSize;j++)
			ppOutput[i][j] = pTemp[j];
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////
// 从内存中装入图像数据到指定的矩阵: （只对256色和24位真彩色）
// FileName 是图像名字
// ppBitmapData 是待存储的图像一维向量的指针地址
// dwWidth , dwHeight 分别是图像的宽度和长度
// flag 返回图像位数（8或者24）
////////////////////////////////////////////////////////////////////////////
bool CZXDib::LoadVectorFromBMPFile(CString &FileName, BYTE *&pBitmapData, DWORD &dwHeight, DWORD &dwWidth,WORD &flag)
{
	DWORD i,skip,position;

	//打开制定图像文件FileName：
	CFile File;  
	if(!File.Open(FileName,CFile::modeRead|CFile::typeBinary)){
		//AfxMessageBox(" 打不开！LoadBitmap"+FileName);
		return FALSE;
	}
	//读文件头：
	BITMAPFILEHEADER bfh;
	File.Read(&bfh,sizeof(BITMAPFILEHEADER));
	if(bfh.bfType!=0x4d42)  //'BM' 不是bmp位图，返回
		return FALSE;

	int ii= sizeof(BITMAPFILEHEADER);


	//读位图信息头：
	BITMAPINFO bmi;
	File.Read(&bmi,sizeof(BITMAPINFO));

	

	dwHeight = bmi.bmiHeader.biHeight;
	dwWidth = bmi.bmiHeader.biWidth;
	//1，4，8，24 分别对应2，16，256，真彩色
	flag = bmi.bmiHeader.biBitCount; 
	if(flag == 24) 
		dwWidth *= 3; //若是真彩色，则把宽度变为三倍
	DWORD dwSize = dwHeight * dwWidth; //位图矩阵大小
	DWORD dReadWidth = (dwWidth+3)/4*4; //实际存储的宽度
    
	//重新申请空间：
	if(pBitmapData) delete pBitmapData;
	pBitmapData = (BYTE*) new BYTE[dwSize];
	if(!pBitmapData) return FALSE;

	//设置每行跳转的字节数
	skip = dReadWidth - dwWidth; 
	//设置初始的矩阵存放位置：
	
	position = dwSize - dwWidth;
	//读文件：
	File.Seek(bfh.bfOffBits,0);
	for(i=0;i<dwHeight;i++)
	{
		File.Read(pBitmapData+position,dwWidth);	
		File.Seek(skip,1);
		position -= dwWidth; //调整指针
	}

	File.Close();
	
	if(flag==24)
		dwWidth /= 3; //恢复图像的宽度:

/*	if( flag == 24 )
	{
		BYTE* pTemp = NULL;
		Translate24To8Color(pBitmapData,dwHeight,dwWidth,pTemp);
		memcpy(pBitmapData,pTemp,dwHeight*dwWidth);
		delete pTemp;
		flag = 8;
	}*/
	
	{/*
		BYTE* pTemp = NULL;
		ChangeImageSize(pBitmapData, dwHeight, dwWidth,pTemp,dwHeight/2,dwWidth/2);
		delete pBitmapData;
		pBitmapData = pTemp;
		dwHeight /= 2;
		dwWidth  /= 2;*/
	}

	return true;
}
////////////////////////////////////////////////////////////////////
//将一维向量图像数据写入文件
// FileName 文件名
// pBitmapData 一维向量指针
// dwWidth,dwHeight 图像的宽度和长度
// flag : 图像的类型
////////////////////////////////////////////////////////////////////
bool CZXDib::WriteBMPFileFromVector(CString &FileName, BYTE *pBitmapData,DWORD dwHeight,DWORD dwWidth,WORD flag)
{
	//参数有效性检测：
	if(!pBitmapData) 
		return FALSE;
	if((flag!=8)&&(flag!=24))
		return FALSE; 

	if(flag == 24)
		dwWidth *= 3;
	
	DWORD dwWriteWidth = (dwWidth+3)/4*4;
	DWORD dwWriteSize = dwHeight * dwWriteWidth;
	
	//打开文件:
	CFile File;
	File.Open(FileName,CFile::modeCreate|CFile::modeWrite);

	BITMAPFILEHEADER BitmapFileHeader;
	if(flag == 24) 
		BitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)-4;
	else 
		BitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)-4+256*sizeof(RGBQUAD); 
	BitmapFileHeader.bfReserved1 = 0;
	BitmapFileHeader.bfReserved2 = 0;
	BitmapFileHeader.bfType = 0x4d42;//'BM'
	BitmapFileHeader.bfSize =  dwWriteSize + BitmapFileHeader.bfOffBits+2;

	File.Write(&BitmapFileHeader,sizeof(BITMAPFILEHEADER));

	BITMAPINFO BitmapInfo;
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFO)-4;//无论是黑白图还是彩图都是40
	BitmapInfo.bmiHeader.biBitCount = flag;
	BitmapInfo.bmiHeader.biHeight = dwHeight;
	if(flag ==24) 
		BitmapInfo.bmiHeader.biWidth = dwWidth/3;
	else 
		BitmapInfo.bmiHeader.biWidth = dwWidth;
	BitmapInfo.bmiHeader.biSizeImage = 0;
	BitmapInfo.bmiHeader.biCompression= 0;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biClrImportant = 0;
	BitmapInfo.bmiHeader.biClrUsed = 0;
	BitmapInfo.bmiHeader.biXPelsPerMeter = 
		BitmapInfo.bmiHeader.biYPelsPerMeter=2834;
	BitmapInfo.bmiColors[0].rgbBlue = 
		BitmapInfo.bmiColors[0].rgbGreen=
		BitmapInfo.bmiColors[0].rgbRed=
		BitmapInfo.bmiColors[0].rgbReserved=0;
	

	File.Write(&BitmapInfo,sizeof(BITMAPINFO));

	//若是256色图像则，写调色板
	if( flag==8 )
	{
		for(int i=1; i<256; i++)
		{
			BYTE temp = (BYTE) i;
			File.Write(&temp,1);
			File.Write(&temp,1);
			File.Write(&temp,1);
			temp = 0;
			File.Write(&temp,1);
		}
	}

	//写文件：
	DWORD position = dwWidth*dwHeight - dwWidth;
	DWORD skip = dwWriteWidth - dwWidth;

	File.Seek(BitmapFileHeader.bfOffBits,0);
	for(DWORD i=0; i<dwHeight; i++)
	{
		File.Write(pBitmapData+position,dwWidth);
		File.Seek(skip,1);
		position -= dwWidth;
	}
	char ch = 0;
	File.Write(&ch,sizeof(char));
	ch = 0;
	File.Write(&ch,sizeof(char));
	File.Close();

	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为256*256显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateVectorToMetric(BYTE *pHg, DWORD dwLength,BYTE *&pResult)
{
    DWORD i,j,top;
    
    if(pResult) delete pResult;
    pResult = new BYTE[256*dwLength];
    
    for(i=0; i<dwLength; i++)
    {
        top = 256-pHg[i];
        for(j=0; j<top; j++)
            pResult[j*dwLength+i] = 240;
        for(;j<256;j++)
            pResult[j*dwLength+i] = 0;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////
//
//  寻找下一个文件名
//
/////////////////////////////////////////////////////////////////////////
bool CZXDib::FindNextFileName(CString &FileName,int iStep)
{
	int nCurrentPosition;
	int position = FileName.Find(".bmp");
	if(position<0) 
		position = FileName.Find(".BMP");
	position--;

	while(iStep>0)
	{
		nCurrentPosition = position;
		char ch =  FileName.GetAt(nCurrentPosition);
		while (ch=='9' && nCurrentPosition>=0)
		{
			FileName.SetAt(nCurrentPosition,'0');
			nCurrentPosition--;
			ch =  FileName.GetAt(nCurrentPosition);
		}
		ch++;
		FileName.SetAt(nCurrentPosition,ch);

		iStep--;
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////
//
//  寻找下一个文件名
//
/////////////////////////////////////////////////////////////////////////
bool CZXDib::FindLastFileName(CString &FileName,int iStep)
{
	int nCurrentPosition;
	int position = FileName.Find(".bmp");
	if(position<0) 
		position = FileName.Find(".BMP");
	position--;
	
	while(iStep<0)
	{
		nCurrentPosition = position;
		char ch =  FileName.GetAt(nCurrentPosition);
		while (ch=='0' && nCurrentPosition>=0)
		{
			FileName.SetAt(nCurrentPosition,'9');
			nCurrentPosition--;
			ch =  FileName.GetAt(nCurrentPosition);
		}
		ch--;
		FileName.SetAt(nCurrentPosition,ch);
		
		iStep++;
	}
	return true;
}
////////////////////////////////////////////////////
//
//  将向量数据转换为ＢＭＰ图像向量:
//
////////////////////////////////////////////////////
bool CZXDib::TranslateVectorToBitmap(DWORD dwHeight,DWORD dwWidth,BYTE* pVector,BYTE*& pBitmapData,WORD flag)
{
	DWORD i,j;
	if(flag==24) dwWidth *= 3;
	else 
		if(flag!=8) return false;
	DWORD dwWriteWidth = (dwWidth+3)/4*4;

	if(pBitmapData) delete []pBitmapData;
	pBitmapData = (BYTE*) new BYTE[dwHeight*dwWriteWidth];
	
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
		pBitmapData[(dwHeight-1-i)*dwWriteWidth+j]   = pVector[i*dwWidth+j];
	
	return true;
}
////////////////////////////////////////////////////
//
//  将ＢＭＰ图像向量转换为向量数据:
//
////////////////////////////////////////////////////
bool CZXDib::TranslateBitmapToVector(DWORD dwHeight,DWORD dwWidth,BYTE* pBitmapData,BYTE*& pVector,WORD flag)
{
	DWORD i,j;
	if(flag==24) dwWidth *= 3;
	else 
		if(flag!=8) return false;
	DWORD dwWriteWidth = (dwWidth+3)/4*4;

	if(pVector) delete []pVector;
	pVector = (BYTE*) new BYTE[dwHeight*dwWriteWidth];
	
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
		pVector[i*dwWidth+j] = pBitmapData[(dwHeight-1-i)*dwWriteWidth+j];
	
	return true;
}
///////////////////////////////////////////////////////////////////
//
//  得到BMP位图的头信息指针
//
///////////////////////////////////////////////////////////////////
bool CZXDib::GetBMPFileHeader(BITMAPFILEHEADER &bfh,CString &FileName)
{
    CFile File;
	if(!File.Open(FileName,CFile::modeRead|CFile::typeBinary)){
		AfxMessageBox(FileName+" 打不开！LoadBitmap");
		return FALSE;
	}
	File.Read(&bfh,sizeof(BITMAPFILEHEADER));
	File.Close();

   return true;
}
///////////////////////////////////////////////////////////////////
//
//  得到BMP位图的头信息指针
//
///////////////////////////////////////////////////////////////////
bool CZXDib::GetBitmapInfo(BITMAPINFO *&bmi, CString &FileName)
{
	CFile File;
	if(!File.Open(FileName,CFile::modeRead|CFile::typeBinary)){
		AfxMessageBox(FileName+" 打不开！LoadBitmap");
		return FALSE;
	}
	
	File.Seek(sizeof(BITMAPFILEHEADER),0);
	
	if(bmi) delete bmi;
	bmi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)+256*sizeof(RGBQUAD)];
	File.Read(bmi,sizeof(BITMAPINFO)+256*sizeof(RGBQUAD));
	
	File.Close();
    return TRUE;
}
////////////////////////////////////////////////////////////////////////////
//
// Sobel算子
/////////////////////////////////////////////////////////////////////////////
bool CZXDib::Sobel(BYTE *& pImage, DWORD dwHeight, DWORD dwWidth)
{
	DWORD i,j;
	double temp1,temp2;
	DWORD Height,Width,dwSize;
	Height = dwHeight-1;
	Width = dwWidth-1;
	dwSize = dwHeight*dwWidth;

	double * pResult=NULL;
	if(!(pResult = (double*) new double[dwHeight*dwWidth]))
	{
		AfxMessageBox("分配空间失败！CZXDib::Sobel pResult");
		return false;
	}
	
	memset(pResult,0,sizeof(double)*dwHeight*dwWidth);

	for(i=1; i<Height; i++)
	for(j=1; j<Width; j++)
	{
		//水平方向
		temp1 = pImage[j+1+(i-1)*dwWidth]+2*pImage[j+1+i*dwWidth]+pImage[j+1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j-1+i*dwWidth]-pImage[j-1+(i+1)*dwWidth];
		temp1 *= temp1;
		//竖直方向
		temp2 = pImage[j+1+(i+1)*dwWidth]+2*pImage[j+(i+1)*dwWidth]+pImage[j-1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j+(i-1)*dwWidth]-pImage[j+1+(i-1)*dwWidth];
		temp2 *= temp2;
		pResult[j+i*dwWidth]= sqrt(temp1+temp2);
	}

	for(i=1; i<Height; i++)
	for(j=1; j<Width; j++)
	{
		if(pResult[j+i*dwWidth]>255) 
			pImage[j+i*dwWidth] = 255;
		else
		pImage[j+i*dwWidth] = (BYTE) pResult[j+i*dwWidth];
	}
	
	delete  pResult;

	return true;
}
////////////////////////////////////////////////////////////////////////////
//
// Sobel算子
/////////////////////////////////////////////////////////////////////////////
bool CZXDib::Sobel1(BYTE * pImage, DWORD dwHeight, DWORD dwWidth, BYTE* &pResult)
{
	int i, j;
	int temp1, temp2;

	delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	memset(pResult, 0, dwHeight*dwWidth);

	for (i = 1; i < dwHeight -1; i++)
	for (j = 1; j < dwWidth -1; j++)
	{
		//水平方向
		temp1 = pImage[j - 1 + (i - 1)*dwWidth] + 2 * pImage[j - 1 + i*dwWidth] + pImage[j - 1 + (i + 1)*dwWidth] - pImage[j + 1 + (i - 1)*dwWidth] - 2 * pImage[j + 1 + i*dwWidth] - pImage[j + 1 + (i + 1)*dwWidth];
		//竖直方向
		temp2 = pImage[j + 1 + (i + 1)*dwWidth] + 2 * pImage[j + (i + 1)*dwWidth] + pImage[j - 1 + (i + 1)*dwWidth] - pImage[j - 1 + (i - 1)*dwWidth] - 2 * pImage[j + (i - 1)*dwWidth] - pImage[j + 1 + (i - 1)*dwWidth];
		if (abs(temp1) >= abs(temp2) && temp1 < 0)
			continue;
		if (abs(temp1) <= abs(temp2) && temp2 < 0)
			continue;			

		pResult[j + i*dwWidth] = min(255,max(temp1*2,temp2*2));
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////
//
// Sobel算子
/////////////////////////////////////////////////////////////////////////////
bool CZXDib::Sobel(BYTE * pImage, DWORD dwHeight, DWORD dwWidth,BYTE* &pResult)
{
	DWORD i,j;
	double temp1,temp2;
	DWORD Height,Width,dwSize;
	Height = dwHeight-1;
	Width = dwWidth-1;
	dwSize = dwHeight*dwWidth;

	double * pdResult=NULL;
	if(!(pdResult = (double*) new double[dwHeight*dwWidth]))
		return false;
	
	for(i=1; i<Height; i++)
	for(j=1; j<Width; j++)
	{
		//水平方向
		temp1 = pImage[j+1+(i-1)*dwWidth]+2*pImage[j+1+i*dwWidth]+pImage[j+1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j-1+i*dwWidth]-pImage[j-1+(i+1)*dwWidth];
		temp1 *= temp1;
		//竖直方向
		temp2 = pImage[j+1+(i+1)*dwWidth]+2*pImage[j+(i+1)*dwWidth]+pImage[j-1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j+(i-1)*dwWidth]-pImage[j+1+(i-1)*dwWidth];
		temp2 *= temp2;
		pdResult[j+i*dwWidth]= sqrt(temp1+temp2);
	}

	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);

	for(i=1; i<Height; i++)
	for(j=1; j<Width; j++)
	{
		if(pdResult[j+i*dwWidth]>255) 
			pResult[j+i*dwWidth] = 255;
		else
			pResult[j+i*dwWidth] = (BYTE) pdResult[j+i*dwWidth];
	}
	
	delete  pdResult;

	return true;
}
////////////////////////////////////////////////////////////////////////////
//
// Sobel算子
/////////////////////////////////////////////////////////////////////////////
bool CZXDib::Sobel(BYTE* pImage, DWORD dwHeight, DWORD dwWidth,double* &pResult)
{
	DWORD i,j;
	double temp1,temp2;
	DWORD Height,Width,dwSize;
	Height = dwHeight-1;
	Width = dwWidth-1;
	dwSize = dwHeight*dwWidth;
	dwSize -= dwWidth;

	if(pResult) delete pResult;
	if(!(pResult = (double*) new double[dwHeight*dwWidth]))
	{
		AfxMessageBox("分配空间失败！CZXDib::Sobel pResult");
		return false;
	}
	memset(pResult,0,sizeof(double)*dwHeight*dwWidth);
	
	for(i=1; i<Height; i++)
	for(j=1; j<Width; j++)
	{
		//水平方向
		temp1 = pImage[j+1+(i-1)*dwWidth]+2*pImage[j+1+i*dwWidth]+pImage[j+1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j-1+i*dwWidth]-pImage[j-1+(i+1)*dwWidth];
		temp1 *= temp1;
		//竖直方向
		temp2 = pImage[j+1+(i+1)*dwWidth]+2*pImage[j+(i+1)*dwWidth]+pImage[j-1+(i+1)*dwWidth]-pImage[j-1+(i-1)*dwWidth]-2*pImage[j+(i-1)*dwWidth]-pImage[j+1+(i-1)*dwWidth];
		temp2 *= temp2;
		pResult[j+i*dwWidth]= sqrt(temp1+temp2);
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////
//
//  求直方图：
//
////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Histogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j,max;

	if(pResult) delete pResult;
	pResult = new BYTE[256];

	DWORD pTemp[256];
	for(i=0; i<256; i++)
		pTemp[i]=0;

	for(j=0; j<dwHeight; j++)
		for(i=0; i<dwWidth; i++)
			pTemp[pBitmap[j*dwWidth+i]]++;
	
	max = 0;
	for(i=0;i<256; i++)
		if(pTemp[i]>max) max = pTemp[i];

	for(i=0; i<256; i++)
		pResult[i] = (BYTE) (pTemp[i]*255/max);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////
//
//  求直方图：
//
////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Histogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD *&pResult,WORD flag)
{
	DWORD i,j,max;

	if(pResult) delete pResult;
	pResult = new DWORD [256];
	memset(pResult,0,sizeof(DWORD)*256);
	
	if( flag == 8 )
	{
		for(j=0; j<dwHeight; j++)
		for(i=0; i<dwWidth; i++)
			pResult[pBitmap[j*dwWidth+i]]++;
			
	}else if( flag == 24 )
	{
		long nValue = 0;
		for(j=0; j<dwHeight; j++)
		for(i=0; i<dwWidth; i++)
		{
			nValue = pBitmap[(j*dwWidth+i)*3];
			nValue+= pBitmap[(j*dwWidth+i)*3+1];
			nValue+= pBitmap[(j*dwWidth+i)*3+2];
			nValue /= 3;
			pResult[nValue]++;
		}
			
	}
	else 
		return false;	

	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为256*256显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(BYTE *pHg, BYTE *&pResult)
{
	DWORD i,j,top;

	if(pResult) delete pResult;
	pResult = new BYTE[256*256];
	
	for(i=0; i<256; i++)
	{
		top = 256-pHg[i];
		for(j=0; j<top; j++)
			pResult[j*256+i] = 240;
		for(;j<256;j++)
			pResult[j*256+i] = 0;
	}
	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为256*256显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(DWORD *pHg, BYTE *&pResult,bool bRegular )
{
	DWORD i,j,top;
	
	if(pResult) delete pResult;
	pResult = new BYTE[256*256];

	if( bRegular )
	{
		BYTE* pbyHis = NULL;
		RegularToGrayLevel(pHg,1,256,pbyHis);
		
		for(i=0; i<256; i++)
		{
			top = 256-pbyHis[i];
			for(j=0; j<top; j++)
				pResult[j*256+i] = 240;
			for(;j<256;j++)
				pResult[j*256+i] = 0;
		}

		delete pbyHis;
		return true;

	}else 
	{
		for(i=0; i<256; i++)
		{
			top = 256 - min(255,pHg[i]);
			for(j=0; j<top; j++)
				pResult[j*256+i] = 240;
			for(;j<256;j++)
				pResult[j*256+i] = 0;
	}	}
	return true;
}
void CZXDib::GetHorizonMeanColor(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult)
{
	int i,j;
	delete pResult;
	pResult = new BYTE[dwHeight*3];
	memset(pResult,0,sizeof(BYTE)*dwHeight*3);
	
	double dR = 0;
	double dG = 0;
	double dB = 0;
	for( i=0; i<dwHeight; i++ )
	{
		dR = dG = dB = 0;
		for( j=0; j<dwWidth;  j++ )
		{
			dR += pBitmap[(i*dwWidth+j)*3+2];
			dG += pBitmap[(i*dwWidth+j)*3+1];
			dB += pBitmap[(i*dwWidth+j)*3+0];
		}
		pResult[i*3+2] = dR / dwWidth;
		pResult[i*3+1] = dG / dwWidth;
		pResult[i*3+0] = dB / dwWidth;
	}
}
void CZXDib::ProjectHorizon(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double* pResult, long &nPos)
{
	int i,j;
	memset(pResult + nPos,0,sizeof(double)*dwHeight);
	
	double dTemp = 0;
	for( i=0; i<dwHeight; i++ )
	{
		dTemp = 0;
		for( j=0; j<dwWidth;  j++ )
			dTemp += pBitmap[i*dwWidth+j];
		pResult[nPos+i] = dTemp;
	}
	nPos += dwHeight;
}
void CZXDib::ProjectHorizon(int* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult)
{
	delete pResult;
	pResult = new double[dwHeight];

	for(int i=0; i<dwHeight; i++ )
	{
		double dTemp = 0;
		for(int j=0; j<dwWidth;  j++ )
			dTemp += pBitmap[i*dwWidth+j];
		pResult[i] = dTemp;
	}
}

void CZXDib::ProjectHorizon(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult)
{
	int i,j;
	double* pTemp = new double[dwHeight];
	memset(pTemp,0,sizeof(double)*dwHeight);

	double dTemp = 0;
	for( i=0; i<dwHeight; i++ )
	{
		dTemp = 0;
		for( j=0; j<dwWidth;  j++ )
			dTemp += pBitmap[i*dwWidth+j];
		pTemp[i] = dTemp;
	}

	RegularToGrayLevel(pTemp,1,dwHeight,pResult);
	delete pTemp;
}

void CZXDib::MeanFilter(int* pPrj, int nW, int nWnd)
{
	nWnd = nWnd / 2 * 2 + 1;
	int nHalfWnd = nWnd / 2;

	int i;
	int* pPrj1 = new int[nW];

	int nValue = 0;
	for ( i = 1; i <= nHalfWnd; ++i)
		nValue += pPrj[i];
	nValue = ( nValue << 1 ) + pPrj[0];
	pPrj1[0] = nValue;

	for ( i = 1; i <= nHalfWnd; ++i )
	{
		nValue += pPrj[nHalfWnd + i]-pPrj[nHalfWnd - i];
		pPrj1[i] = nValue;
	}

	for (; i < nW-nHalfWnd; ++i)
	{
		nValue += pPrj[ i + nHalfWnd]-pPrj[ i - nHalfWnd];
		pPrj1[i] = nValue;
	}

	for (; i < nW; ++i)
	{
		nValue += pPrj[ nW + nW - 2 - i - nHalfWnd]- pPrj[i - nHalfWnd];
		pPrj1[i] = nValue;
	}

	for (i = 0; i < nW; ++i)
		pPrj[i] = pPrj1[i] / nWnd;

	delete pPrj1;
}


void CZXDib::ProjectVertical(int* pBitmap, DWORD dwHeight, DWORD dwWidth, int*& pResult)
{
	int i, j;
	delete pResult;
	pResult = new int[dwWidth];
	memset(pResult, 0, sizeof(int)*dwWidth);

	int dTemp = 0;
	for (j = 0; j < dwWidth; j++)
	{
		dTemp = 0;
		for (i = 0; i < dwHeight; i++)
			dTemp += pBitmap[i*dwWidth + j];
		pResult[j] = dTemp;
	}
}
void CZXDib::ProjectVertical(int* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult)
{
	int i,j;
	delete pResult;
	pResult = new double[dwWidth];
	memset(pResult,0,sizeof(double)*dwWidth);
	
	double dTemp = 0;
	for( j=0; j<dwWidth;  j++ )
	{
		dTemp = 0;
		for( i=0; i<dwHeight; i++ )
			dTemp += pBitmap[i*dwWidth+j];
		pResult[j] = dTemp;
	}
}
void CZXDib::ProjectVertical(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double*& pResult)
{
	int i,j;
	delete pResult;
	pResult = new double[dwWidth];
	memset(pResult,0,sizeof(double)*dwWidth);
	
	double dTemp = 0;
	for( j=0; j<dwWidth;  j++ )
	{
		dTemp = 0;
		for( i=0; i<dwHeight; i++ )
			dTemp += pBitmap[i*dwWidth+j];
		pResult[j] = dTemp;
	}
}

void CZXDib::ProjectVertical(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult)
{
	int i,j;
	double* pTemp = new double[dwWidth];
	memset(pTemp,0,sizeof(double)*dwWidth);
	
	double dTemp = 0;
	for( j=0; j<dwWidth;  j++ )
	{
		dTemp = 0;
		for( i=0; i<dwHeight; i++ )
			dTemp += pBitmap[i*dwWidth+j];
		pTemp[j] = dTemp;
	}
	
	RegularToGrayLevel(pTemp,1,dwWidth,pResult);
	delete pTemp;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为 显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(int *pHg, DWORD dwLength,BYTE *&pResult)
{
	DWORD i,j,top;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwLength*256];
	
	for(i=0; i<dwLength; i++)
	{
		int nTemp = min(255,pHg[i]);
		top = 256-nTemp;
		for(j=0; j<top; j++)
			pResult[j*dwLength+i] = 240;
		for(;j<256;j++)
			pResult[j*dwLength+i] = 0;
	}
	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为 显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(BYTE *pHg, DWORD dwLength,BYTE *&pResult)
{
	DWORD i,j,top;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwLength*256];
	
	for(i=0; i<dwLength; i++)
	{
		top = 256-pHg[i];
		for(j=0; j<top; j++)
			pResult[j*dwLength+i] = 240;
		for(;j<256;j++)
			pResult[j*dwLength+i] = 0;
	}
	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为 显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(double *pHg, DWORD dwLength, BYTE *&pResult)
{	
	BYTE* pTemp = NULL;
	RegularToGrayLevel(pHg, 1, dwLength, pTemp);
	TranslateHistogramToVector(pTemp, dwLength, pResult);	
	delete pTemp;
	return true;
}
//////////////////////////////////////////////////////////////////
//
//  将直方图转化为 显示矩阵
//
//////////////////////////////////////////////////////////////////
bool CZXDib::TranslateHistogramToVector(BYTE *pHg, DWORD dwHeight,DWORD dwWidth,DWORD dwResHeight,BYTE *&pResult)
{
	int i,j,top;

	if(pResult) delete pResult;
	pResult = new BYTE[dwWidth*dwResHeight];
	
	for(i=0; i<dwWidth; i++)
	{
		top  = dwResHeight;
		top -= pHg[i];
		for(j=0; j<top; j++)
			pResult[j*dwWidth+i] = 240;
		for(;j<dwResHeight;j++)
			pResult[j*dwWidth+i] = 0;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////
//
//  直方图平滑:
//
////////////////////////////////////////////////////////////////////////
bool CZXDib::HistogramSmoothBy5Points(BYTE *pHg,int level)
{
	int i;
	int temp;
	int *pTemp = new int[level];

	for(i=0; i<level; i++)
		pTemp[i] = pHg[i];
	
	temp = (69*pTemp[0]+4*pTemp[1]-6*pTemp[2]+4*pTemp[3]-pTemp[4])/70;
	if(temp<0) temp = 0;
	pHg[0] = (BYTE)temp;
	temp = (2*pTemp[0]+27*pTemp[1]+12*pTemp[2]-8*pTemp[3]+2*pTemp[4])/35;
	if(temp<0) temp = 0;
	pHg[1] = (BYTE)temp;

	for(i=2; i<level-2; i++)
	{
		temp = (-3*pTemp[i-2]+12*pTemp[i-1]+17*pTemp[i]+12*pTemp[i+1]-3*pTemp[i+2])/35;
		if(temp<0) temp=0;
		pHg[i] = (BYTE)temp;
	}

	temp = (2*pTemp[level-5]-8*pTemp[level-4]+12*pTemp[level-3]+27*pTemp[level-2]+2*pTemp[level-1])/35;
	if(temp<0) temp = 0;
	pHg[level-2] = (BYTE)temp;
	temp = (-1*pTemp[level-5]+4*pTemp[level-4]-6*pTemp[level-3]+4*pTemp[level-2]+69*pTemp[level-1])/70;
	if(temp<0)	temp = 0;
	pHg[level-1] = (BYTE)temp;
	
	delete pTemp;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  二值化
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::BlackAndWhite(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, unsigned char flag)
{
	DWORD i,j;

	for(j=0; j<dwHeight; j++)
	for(i=0; i<dwWidth; i++)
	{
		if(pBitmap[j*dwWidth+i]<flag) pBitmap[j*dwWidth+i] = 0;
		else pBitmap[j*dwWidth+i] = 255;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  用小矩形来降低图像的分辨率
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Cover2ValueImageByRect(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,DWORD dwRectHeight,DWORD dwRectWidth)
{
 	DWORD i,j,k,l;
	DWORD temp;
	BYTE* pResult=NULL;
	DWORD upgate = (DWORD)(dwRectHeight*dwRectWidth*0.80);
	DWORD downgate = (DWORD)(dwRectHeight*dwRectWidth*0.30);
	pResult = (BYTE*) new BYTE[dwHeight*dwWidth];
	
	for(j=0; j<dwHeight-dwRectHeight; j++)
	for(i=0; i<dwWidth-dwRectWidth; i++)
	{
		temp = 0;
		for(k=0; k<dwRectHeight; k++)
		for(l=0; l<dwRectWidth; l++)
		{
			if(pBitmap[(j+k)*dwWidth+l+i])
			temp ++;
		}
		if(temp>upgate) 
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]=255;
		else if(temp<downgate)
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]=0;
		else 
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]= pResult[(j+k)*dwWidth+l+i]?255:0;
	}
	for(i=0;i<dwHeight; i++)
	for(j=0;j<dwWidth; j++)
		pBitmap[i*dwWidth+j] = pResult[i*dwWidth+j];
	delete pResult;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  通过投影来确定目标的位置区域
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ProjectAndFindTargetRegion(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, RECT &rect)
{
	DWORD i,j;
	bool contenue;
	int nGate = 8;

	DWORD * pLine=NULL;
	DWORD * pCol=NULL;
	pLine = new DWORD [dwWidth];
	pCol = new DWORD [dwHeight];

	for(i=0; i<dwWidth; i++)
		pLine[i] = 0;
	for(j=0; j<dwHeight;j++)
		pCol[j] =0;

	for(j=0; j<dwHeight; j++)
	for(i=0; i<dwWidth; i++)
	{
		if(pBitmap[j*dwWidth+i])
			pCol[j]++;
	}
	rect.bottom = dwHeight-1;
	rect.right = dwWidth-1;
	rect.top = rect.left = 0;

//	for(i=0;i<5;i++)
//		HistogramSmoothBy5Points(pLine,dwWidth);

	DWORD //	HistogramSmoothBy5Points(pCol,dwHeight);
	i_from=0,i_to=0,counter=0,counter_max=0,i_from_max,i_to_max;
	for(i=0; i<dwHeight-2; i++)
	{
		if(pCol[i]>nGate && !i_from) 
		{
			i_from = i;
			counter =pCol[i];
		}
		else
		{
			if(i_from && (pCol[i]>nGate || pCol[i+1]>nGate))
				counter+=pCol[i];
			else
				if(pCol[i]<nGate && !i_to && i_from && pCol[i+1]<nGate) 
				{
					i_to = i;
					counter += pCol[i];
					if(i_to - i_from < 10) 
						counter = 0;
					if(counter>counter_max) 
					{
						counter_max = counter;
						i_from_max = i_from;
						i_to_max = i_to;
					}
					i_from = i_to =0;
					counter = 0;
				}
		}
	}
	if(i_from && !i_to &&  counter>counter_max)
	{
		counter_max = counter;
		i_from_max = i_from;
		i_to_max = i-1;
	}
	if(counter_max)
	{
	  rect.top = i_from_max;
 	  rect.bottom = i_to_max;
	}
	if(!counter_max) return false;
	counter = counter_max/(i_to_max-i_from_max);
	counter /= 3;
	contenue = true;
	for(i=rect.top;i<rect.bottom && contenue;i++)
		if(pCol[i]<counter) 
			rect.top++;
		else
			contenue=false;
		contenue = true;
	for(i=rect.bottom;i>=rect.top && contenue;i--)
		if(pCol[i]<counter) 
			rect.bottom--;
		else
			contenue=false;
	
	if(rect.bottom>rect.top+100) 
		rect.bottom = rect.top+100;

	//计算在图像竖直方向的投影：
	for(j=rect.top; j<rect.bottom; j++)
	for(i=0; i<dwWidth; i++)
	{
		if(pBitmap[j*dwWidth+i])
			pLine[i]++;
	}


	i_from=0,i_to=0,counter=0,counter_max=0,i_from_max,i_to_max;
	for(i=0; i<dwWidth-2; i++)
	{
		if(pLine[i]>nGate && !i_from) 
		{
			i_from = i;
			counter =pLine[i];
		}
		else
		{
			if(i_from && (pLine[i]>nGate || pLine[i+1]>nGate))
				counter+=pLine[i];
			else
				if(pLine[i]<nGate && !i_to && i_from && pLine[i+1]<nGate) 
				{
					i_to = i;
					counter += pLine[i];
					if(i_to - i_from < 10) 
						counter = 0;
					
					if(counter>counter_max) 
					{
						counter_max = counter;
						i_from_max = i_from;
						i_to_max = i_to;
					}
					i_from = i_to =0;
					counter = 0;
				}
		}
	}
	if(i_from && !i_to &&  counter>counter_max)
	{
		counter_max = counter;
		i_from_max = i_from;
		i_to_max = i-1;
	}
	if(counter_max)
	{
	  rect.left = i_from_max;
	  rect.right = i_to_max;
	}
	if(!counter_max) return false;
	counter = counter_max/(i_to_max-i_from_max);
	counter = counter * 1/3;
	contenue = true;
	for(i=rect.left;i<rect.right && contenue;i++)
		if(pLine[i]<counter) 
			rect.left++;
		else
			contenue=false;
		contenue = true;
	for(i=rect.right;i>=rect.left && contenue;i--)
		if(pLine[i]<counter) 
			rect.right--;
		else
			contenue=false;


	delete pCol;
	delete pLine;
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheMatric(double x1, double y1, double x2, double y2, BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE color)
{
	int nX;
	double pdX[2];
	double pdY[2];
	int nCnt = 0;

	int nW = dwWidth - 1;
	int nH = dwHeight - 1;

	if (x1 > 0 && x1 < nW && y1 > 0 && y1 < nH)
	{
		pdX[nCnt] = x1;
		pdY[nCnt++] = y1;
	}

	if ((x1 - 0)*(x2 - 0) < 0)
	{
		double y = (0 - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = 0;
			pdY[nCnt++] = y;
		}
	}

	if ((x1 - nW)*(x2 - nW) < 0)
	{
		double y = (nW - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = nW;
			pdY[nCnt++] = y;
		}
	}

	if ((y1 - 0)*(y2 - 0) < 0)
	{
		double x = (0 - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = 0;
		}
	}

	if ((y1 - nH)*(y2 - nH) < 0)
	{
		double x = (nH - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = nH;
		}
	}

	if (x2 > 0 && x2 < nW && y2 > 0 && y2 < nH)
	{
		pdX[nCnt] = x2;
		pdY[nCnt++] = y2;
	}

	// 无交点
	if (nCnt != 2)
		return 0;

	x1 = pdX[0];
	x2 = pdX[1];
	y1 = pdY[0];
	y2 = pdY[1];

	if (fabs(x1 - x2) > fabs(y1 - y2))
	{
		if (x2 < x1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = (y2 - y1) / (x2 - x1);
		for (int i = x1; i <= x2; i++)
		{
			nX = ((int)(y1 + 0.5) * dwWidth + i);
			pBitmap[nX] = color;
			y1 += dAdd;
		}
	}
	else //if(fabs(x1 - x2) < fabs(y1 - y2))
	{
		if (y2 < y1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = (x2 - x1) / (y2 - y1);
		for (int i = y1; i <= y2; i++)
		{
			nX = ((int)(x1 + 0.5) + i*dwWidth);
			pBitmap[nX] = color;
			x1 += dAdd;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线: 反走样
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheMatric1(double x1, double y1, double x2, double y2, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE color)
{
	int nX;
	double pdX[2];
	double pdY[2];
	int nCnt = 0;

	int nW = dwWidth - 1;
	int nH = dwHeight - 1;

	if (x1 > 0 && x1 < nW && y1 > 0 && y1 < nH)
	{
		pdX[nCnt] = x1;
		pdY[nCnt++] = y1;
	}

	if ((x1 - 0)*(x2 - 0) < 0)
	{
		double y = (0 - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = 0;
			pdY[nCnt++] = y;
		}
	}

	if ((x1 - nW)*(x2 - nW) < 0)
	{
		double y = (nW - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = nW;
			pdY[nCnt++] = y;
		}
	}

	if ((y1 - 0)*(y2 - 0) < 0)
	{
		double x = (0 - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = 0;
		}
	}

	if ((y1 - nH)*(y2 - nH) < 0)
	{
		double x = (nH - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = nH;
		}
	}

	if (x2 > 0 && x2 < nW && y2 > 0 && y2 < nH)
	{
		pdX[nCnt] = x2;
		pdY[nCnt++] = y2;
	}

	// 无交点
	if (nCnt != 2)
		return 0;

	x1 = pdX[0];
	x2 = pdX[1];
	y1 = pdY[0];
	y2 = pdY[1];

	if (fabs(x1 - x2) > fabs(y1 - y2))
	{
		if (x2 < x1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = (y2 - y1) / (x2 - x1);
		for (int i = x1; i <= x2; i++)
		{
			nX = ((int)(y1 + 0.5) * dwWidth + i);
			pBitmap[nX] = color;
			y1 += dAdd;
		}
	}
	else //if(fabs(x1 - x2) < fabs(y1 - y2))
	{
		if (y2 < y1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = (x2 - x1) / (y2 - y1);
		for (int i = y1; i <= y2; i++)
		{
			nX = ((int)(x1 + 0.5) + i*dwWidth);
			pBitmap[nX] = color;
			x1 += dAdd;
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheColorMatric(double x1, double y1, double x2, double y2, BYTE *pBitmap,int dwHeight,int dwWidth,RGBQUAD color)
{
	int nX;
	double pdX[2];
	double pdY[2];
	int nCnt = 0;
  
	int nW = dwWidth - 1;
	int nH = dwHeight - 1;

	if (x1 > 0 && x1 < nW && y1 > 0 && y1 < nH)
	{
		pdX[nCnt] = x1;
		pdY[nCnt++] = y1;
	}

	if ((x1 - 0)*(x2 - 0) < 0)
	{
		double y = (0 - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = 0;
			pdY[nCnt++] = y;
	}  }

	if ((x1 - nW)*(x2 - nW) < 0)
	{
		double y = (nW - x1) * (y2 - y1) / (x2 - x1) + y1;
		if (y > 0 && y < nH)
		{
			pdX[nCnt] = nW;
			pdY[nCnt++] = y;
	}  }

	if ((y1 - 0)*(y2 - 0) < 0)
	{
		double x = (0 - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = 0;
	}  }

	if ((y1 - nH)*(y2 - nH) < 0)
	{
		double x = (nH - y1) * (x2 - x1) / (y2 - y1) + x1;
		if (x > 0 && x < nW)
		{
			pdX[nCnt] = x;
			pdY[nCnt++] = nH;
	}  }

	if (x2 > 0 && x2 < nW && y2 > 0 && y2 < nH)
	{
		pdX[nCnt] = x2;
		pdY[nCnt++] = y2;
	}

	// 无交点
	if (nCnt != 2) 
		return 0;

	x1 = pdX[0];
	x2 = pdX[1];
	y1 = pdY[0];
	y2 = pdY[1];

	if( fabs(x1-x2) > fabs(y1-y2) )
	{
		if (x2 < x1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = ( y2 - y1 ) / ( x2 - x1 );	
		for(int i=x1; i<=x2; i++)
		{
			nX = ((int)(y1 + 0.5) * dwWidth + i ) * 3;
			pBitmap[nX + 0] = color.rgbBlue;
			pBitmap[nX + 1] = color.rgbGreen;
			pBitmap[nX + 2] = color.rgbRed;
			y1 += dAdd;
		}
	}
	else //if(fabs(x1 - x2) < fabs(y1 - y2))
	{
		if (y2 < y1)
		{
			swap(x2, x1);
			swap(y2, y1);
		}
		double dAdd = (x2 - x1) / (y2 - y1);
		for( int i=y1; i<=y2; i++)
		{
			nX = ((int)(x1 + 0.5) + i*dwWidth ) * 3;
			pBitmap[nX + 0] = color.rgbBlue;
			pBitmap[nX + 1] = color.rgbGreen;
			pBitmap[nX + 2] = color.rgbRed;
			x1 += dAdd;		
		}
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheColorMatric2(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color)
{
	DWORD position=0;
	double position1;
	int temp;
	double temp1,temp2;
	
	//y_from = dwHeight - 1 - y_from;
	//y_to   = dwHeight - 1 - y_to;
	
	if(abs(x_from-x_to)>abs(y_from-y_to))
	{
		if(x_from>x_to)
		{
			temp   = x_to;
			x_to   = x_from;
			x_from = temp;
			temp   = y_to;
			y_to   = y_from;
			y_from = temp;
		}
		temp1  = y_to-y_from;
		temp2  = x_to-x_from;
		temp1 /= temp2;
		
		position1 = y_from;
		position  = y_from;
		int i; for( i=x_from; i<=x_to; i++)
		{
			pBitmap[(i+(position)*dwWidth)*3  ] = max(color.rgbBlue ,pBitmap[(i+(position)*dwWidth)*3  ]);
			pBitmap[(i+(position)*dwWidth)*3+1] = max(color.rgbGreen,pBitmap[(i+(position)*dwWidth)*3+1]);
			pBitmap[(i+(position)*dwWidth)*3+2] = max(color.rgbRed  ,pBitmap[(i+(position)*dwWidth)*3+2]);
			position1 += temp1;
			position  = (DWORD) (position1+0.5);
		}
	}
	else 
	{
		if(y_from>y_to)
		{
			temp   = y_to;
			y_to   = y_from;
			y_from = temp;
			temp   = x_to;
			x_to   = x_from;
			x_from = temp;
		}
		temp1  = x_to-x_from;
		temp2  = y_to-y_from;
		temp1 /= temp2;
		
		position1 = x_from;
		position = x_from;
		int i; for( i=y_from; i<=y_to; i++)
		{
			pBitmap[(position+(i)*dwWidth)*3  ] = max(color.rgbBlue ,pBitmap[(position+(i)*dwWidth)*3  ]);
			pBitmap[(position+(i)*dwWidth)*3+1] = max(color.rgbGreen,pBitmap[(position+(i)*dwWidth)*3+1]);
			pBitmap[(position+(i)*dwWidth)*3+2] = max(color.rgbRed  ,pBitmap[(position+(i)*dwWidth)*3+2]);
			position1 += temp1;
			position  = (DWORD) (position1+0.5);
		}
	}
	return true;
}
void CZXDib::HistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long pnHGRefer[256])
{
	long i,m,n,nTemp,nMinN,dMin;
	long nSize = dwHeight*dwWidth;
	
	long pnHGCal[257];
	long pnHGDiff[257];
	long pnHGReferCal[256];
	long pnMap[256];	
	long pHG[256];	
	memset( pHG, 0, sizeof(long)*256);
	memset( pnMap, -1, sizeof(long)*256);
	memset(pnHGDiff,nSize,sizeof(long)*257);
	
	
	for( i=0; i<nSize; i++ )
		++(pHG[pBitmap[i]]);
	
	
	pnHGReferCal[0] = pnHGRefer[0];
	pnHGCal[0]		= pHG[0];
	pnHGDiff[0]		= nSize;
	
	for( m=1; m<256; m++ )
	{
		pnHGCal[m]		= pnHGCal[m-1]		+ pHG[m];		
		pnHGReferCal[m] = pnHGReferCal[m-1] + pnHGRefer[m];
		pnHGDiff[m]		= nSize;
	}
	
	pnHGCal[256] = pnHGCal[255];
	n = 0;
	for( m=0; m<256; m++)
	{
		nTemp = pnHGReferCal[m];
		
		bool bFound = nTemp <= pnHGCal[n+1];
		
		while( !bFound )
		{			
			n++;
			bFound = nTemp <= pnHGCal[n+1];
		}
		
		
		if( nTemp - pnHGCal[n] <= pnHGCal[n+1] - nTemp )
		{
			if( pnHGDiff[n]  > nTemp - pnHGCal[n] )	
			{
				pnHGDiff[n]  = nTemp - pnHGCal[n];
				pnMap[n] = m;
			}
		}
		else 
		{
			if( pnHGDiff[n+1]  > pnHGCal[n+1] - nTemp )	
			{
				pnHGDiff[n+1]  = pnHGCal[n+1] - nTemp;
				pnMap[n+1] = m;	
			}
		}
	}
	
	for( m=255; m>=0; m-- )
	{
		if( pnMap[m] == -1 )
			pnMap[m] = n;
		else 
			n = pnMap[m];
	}
	
	for( i=0; i<nSize; i++ )
		pBitmap[i] = pnMap[pBitmap[i]];	
}
bool CZXDib::GetHSIFromRGB(BYTE R,BYTE G,BYTE B,double &H,double &S,double &I)
{
	double r,g,b;
	double radian;
    double dmin;
	r = R;
	g = G;
	b = B;
    dmin = min(min(r, g), b);

	if( !(R+B+G) )
	{
		H = S = I = 0;
		return true;
	}

    if ((R - G) * (R - G) + (R - B) * (G - B))
    {
        radian = (0.5 * (r - g + r - b)) / sqrt((r - g) * (r - g) + (r - b) * (g - b));
    }
    else
    {
        H = S =0;
		I = (r+g+b)/3.0;
		return true;
    }
    
    // GET H 
    if (B <= G)
        H = acos(radian);
    else
        H = 2 * 3.1415926 - acos(radian);
    
	// GET I 
    I = (r + g + b) / 3.0;

    // GET S 
    S = 1.0 - 3.0 * dmin / (r + g + b);
	
    return true;
}
bool CZXDib::GetHSIFromRGB(double R,double G,double B,double &H,double &S,double &I)
{/*
	double a,b,c;

	I = R + G + B;
	
	if(R < G && R < B)
	{
		a=(double)(B);
		b=(double)(R);
		c=1;
	}
	else if(G < R && G < B)
	{
		a=(double)(R);
		b=(double)(G);
		c=2;
	}
	else if(B < R && B < G)
	{
		a=(double)(G);
		b=(double)(B);
		c=0;
	}
	else
	{
		a=(double)(G);
		b=(double)(B);
		c=0;
	}


	if(I >0.000001 )
	{
		H=(a-b)/(I-3*b)+c;
		S=(I-3*b)/I;
		I /= 3.0;
	}
	else
	{
		H=0;
		S=0;
		I=0;
		return true;
	}	*/

	double r,g,b;
	double radian;
    double dmin;
	r = R;
	g = G;
	b = B;
    dmin = min(min(r, g), b);

	if( !(R+B+G) )
	{
		H = S = I = 0;
		return true;
	}

    if ((R - G) * (R - G) + (R - B) * (G - B))
    {
        radian = (0.5 * (r - g + r - b)) / sqrt((r - g) * (r - g) + (r - b) * (g - b));
    }
    else
    {
        H = S =0;
		I = (r+g+b)/3.0;
		return true;
    }
    
    // GET H 
    if (B <= G)
        H = acos(radian);
    else
        H = 2 * 3.1415926 - acos(radian);
    
	// GET I 
    I = (r + g + b) / 3.0;

    // GET S 
    S = 1.0 - 3.0 * dmin / (r + g + b);

    return true;
}
bool CZXDib::GetHSVFromRGB(int r,int g,int b,double &hr,double &sr,double &vr)
{
	/* ==================================== */ 
	//void rgb2hsv(int r,int g,int b,double *hr,double *sr,double *vr) 
	//{ 
		double rd, gd, bd, h, s, v, max, min, del, rc, gc, bc; 
		
		/* convert RGB to HSV */ 
		rd = r / 255.0;            /* rd,gd,bd range 0-1 instead of 0-255 */ 
		gd = g / 255.0; 
		bd = b / 255.0; 
		
		/* compute maximum of rd,gd,bd */ 
		if (rd>=gd) { if (rd>=bd) max = rd;  else max = bd; } 
        else { if (gd>=bd) max = gd;  else max = bd; } 
		
		/* compute minimum of rd,gd,bd */ 
		if (rd<=gd) { if (rd<=bd) min = rd;  else min = bd; } 
        else { if (gd<=bd) min = gd;  else min = bd; } 
		
		del = max - min; 
		v = max; 
		if (max != 0.0) s = (del) / max; 
		else s = 0.0; 
		
		h = -2; // NO HUE
		if (s != 0.0) { 
			rc = (max - rd) / del; 
			gc = (max - gd) / del; 
			bc = (max - bd) / del; 
			
			if      (rd==max) h = bc - gc; 
			else if (gd==max) h = 2 + rc - bc; 
			else if (bd==max) h = 4 + gc - rc; 
			
			h = h * 60; 
			if (h<0) h += 360; 
		} 
		
		hr = h;  sr = s;  vr = v; 
		return true;
} 

bool CZXDib::GetRGBFromHSV(double h,double s,double v,BYTE &rr,BYTE &gr,BYTE &br)
/* h 0..36 , S 0..1  V 0..1 */
{
	/* ==================================== */ 
	//void hsv2rgb(double h,double s,double v,int *rr,int *gr,int *br) 
	//{ 
		int j; 
		double rd, gd, bd; 
		double f, p, q, t; 
		long H = h+0.5;
		
		if( s ==0 )
		{
			if( H < 0 )
			{
				rr = gr = br = v * 255;
				return true;
			}
			else return false;
		}
		/* convert HSV back to RGB */ 
		//if (h<-1 || s==0.0) { rd = v;  gd = v;  bd = v; } 
		else { 
			if ( H == 360 ) h = 0.0; 
			h = h / 60.0; 
			j = (int) floor(h); 
			f = h - j; 
			p = v * (1-s); 
			q = v * (1 - (s*f)); 
			t = v * (1 - (s*(1 - f))); 
			
			switch (j) { 
			case 0:  rd = v;  gd = t;  bd = p;  break; 
			case 1:  rd = q;  gd = v;  bd = p;  break; 
			case 2:  rd = p;  gd = v;  bd = t;  break; 
			case 3:  rd = p;  gd = q;  bd = v;  break; 
			case 4:  rd = t;  gd = p;  bd = v;  break; 
			case 5:  rd = v;  gd = p;  bd = q;  break; 
			default: rd = v;  gd = t;  bd = p;  break;  /* never happen */ 
			} 
		} 
		
		rr = (int) floor((rd * 255.0) + 0.5); 
		gr = (int) floor((gd * 255.0) + 0.5); 
		br = (int) floor((bd * 255.0) + 0.5); 
	//} 
	
	return true;
}
bool CZXDib::GetRGBFromHSB(BYTE& R,BYTE& G,BYTE& B,double H,double S,double Y)
{
	long i;
	double f,p,q,t,r,g,b;

	if( S < 0.001 )
	{
		r = Y * 255.0;
		g = Y * 255.0;
		b = Y * 255.0;
	}else 
	{
		H = H / 60.0;
		i = (long)H;
		f = H - i;
		p = Y*(1.0-S);
		q = Y*(1.0-S*f);
		t = Y*(1.0-S*(1.0-f));

		switch(i)
		{
		case 0:		
			r=Y;	g=t;	b=p;	break;
		case 1:
			r=q;	g=Y;	b=p;	break;
		case 2:
			r=p;	g=Y;	b=t;	break;
		case 3:
			r=p;	b=Y;	g=q;	break;
		case 4:
			r=t;	b=Y;	g=p;	break;
		default:
			r=Y;	g=p;	b=q;
		}
		r = r*255;
		g = g*255;
		b = b*255;
	}

	r += 0.5;
	g += 0.5;
	b += 0.5;

	if( r < 0 ) 
		R = 0;
	else if( r > 255 ) 
		R = 255;
	else 
		R = r;

	if( g < 0 ) 
		G = 0;
	else if( g > 255 )
		G = 255;
	else 
		G = g;	
	
	if( b < 0 ) 
		B = 0;
	else if( b > 255 ) 
		B = 255;
	else 
		B = b;
		
	return true;
}
bool CZXDib::GetHSBFromRGB(double &H,double &S,double &Y,BYTE R,BYTE G,BYTE B)
{
	long nMin    = min(min(R,G),B);	 // my_min#=min#(r,g) //	my_min#=min#(my_min#,b)
	long nMax    = max(max(R,G),B);  // my_max#=max#(r,g) //	my_max#=max#(my_max#,b)	
	double delta = nMax - nMin;      // delta#=(my_max#-my_min#)
	
	if( nMax ) //If my_max#<>0
	{
		S = delta / nMax;	//sat#=delta#/my_max#
		Y = nMax / 255.0;	//lumin#=my_max#/255.0
		if( nMax != nMin )	//If delta<>0
		{
			if( R == nMax ) //If r=my_max Then 
			{
				H = (G-B)/delta; //	hue=(g-b)/delta				//goto OKII;//Goto okii
			} //EndIf
			else if( G == nMax ) //If g=my_max Then 
			{
				H = 2+((B-R)/delta);//hue=2+((b-r)/delta)				//goto OKII; //Goto okii
			} //EndIf
			else 
			{
				H = 4 + ((R-G)/delta);//hue=4+((r-g)/delta)
			}
			//.okii
		}//EndIf
		else 
		{
			H = 0;
			S = 0;
			Y = (R+B+G) / 765.0;
		}
	}//EndIf
	else 
	{
		H = 0;
		S = 0;
		Y = (R+B+G) / 765.0;
	}
	
	H = H * 60.0;//hue=hue*60.0
	
	if( H < 0 )
		H = H + 360.0;//If hue<0 Then hue=hue+360.0
	
	return true;
}
bool CZXDib::GetRGBFromHSI(double H,double S,double I,BYTE &R,BYTE &G,BYTE &B)
{/*
	double dr,dg,db,ii;
	ii = I;

	long flage = (long)H;
	switch(flage)
	{
	case 0:
		dr=ii*(1-S);
		dg=ii*(1+5*S-3*S*H);
		db=ii*(1-4*S+3*S*H);
		break;
	case 1:
		dr=ii*(1-7*S+3*S*H);
		dg=ii*(1-S);
		db=ii*(1+8*S-3*S*H);
		break;
	case 2:
		dr=ii*(1+2*S-3*S*H);
		dg=ii*(1-1*S+3*S*H);
		db=ii*(1-S);
		break;
	case 3:
		dr=ii*(1+2*S-3*S*H);
		dg=ii*(1-1*S+3*S*H);
		db=ii*(1-S);
		break;
	default:break;
	}
	
	dr+=0.5;
	dg+=0.5;
	db+=0.5;
	
	if(dr < 0)	dr=0;
	if(dg < 0)	dg=0;
	if(db < 0)	db=0;

	R = dr>255?255:dr;
	B = db>255?255:db;
	G = dg>255?255:dg;*/
	

    double temp;
    double PI_3 = 3.14159265/3.0;
    switch((int)(H / (PI_3 * 2))) 
    {		
    case 0:
        temp = I * (1 - S) + 0.5;
        B = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * (1 + S * cos(H) / cos( PI_3 - H)) + 0.5;
        R = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * 3 - R - B + 0.5;
        G = temp > 255 ? 255 : (unsigned char)temp;
        break;
    case 1:
        H -= (PI_3 * 2);
        temp = I * (1 - S) + 0.5;
        R = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * (1 + S * cos(H) / cos(PI_3 - H)) + 0.5;
        G = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * 3 - R - G + 0.5;
        B = temp > 255 ? 255 : (unsigned char)temp;
        break;
    case 2:
        H -= (PI_3 * 4);
        temp = I * (1 - S) + 0.5;
        G = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * (1 + S * cos(H) / cos(PI_3 - H)) + 0.5;
        B = temp > 255 ? 255 : (unsigned char)temp;
        temp = I * 3 - G - B + 0.5;
        R = temp > 255 ? 255 : (unsigned char)temp;
        break;
    }
    return true;
}
int CZXDib::GetHSIFromRGB(unsigned char *pColorBitmap, LONG dwHeight, LONG dwWidth, unsigned char *&pResult)
{	
	double r,g,b;
	double x;
	LONG i,j;
	double S,H,I;
    LONG temp;
	LONG  position=0;
	LONG DWWidth=dwWidth/3;
	for(i=0; i<dwHeight; i++)
	for(j=0; j<DWWidth; j++)
	{ 
		b = (double)(pColorBitmap[position])/255.0;
		g = (double)(pColorBitmap[position+1])/255.0;
		r = (double)(pColorBitmap[position+2])/255.0;
		
		I = (r+g+b)/3;
		//取RGB中的最小值
		x = r;
		if( x>g ) x = g;
		if( x>b ) x = b;
		if(I<0.000001)
			S = 0;
		else
			S = 1-x/I;
		if(S<0.000001)
			H = 0;
		else
		{
			x = (r-g+r-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b)));
			if(x>=1.0) 
				H = 0;
			else 
				H = (acos(x)/3.1415926)*180; 
		}
		if(g<b) H=360-H;
		
		S=S*255;
		I=I*255;
		//将真彩色图变换为灰度图			
		
		temp =(LONG )( 0.299*pColorBitmap[(i*DWWidth+j)*3]+0.587*pColorBitmap[(i*DWWidth+j)*3+1]+0.114*pColorBitmap[(i*DWWidth+j)*3+2]);
		
		pResult[i*DWWidth+j] = (unsigned char) temp;
		
		
		if(((H<36)&&(H>5)||(H>340))&&(I>80)&&(S>31)&&(S<156))
		{				
			pResult[i*DWWidth+j] = 255;				
		}
		else 
		{
			pResult[i*DWWidth+j] = 0;				
		}
		position += 3;
	}		
	return true;
}
//Image fussion
void CZXDib::FussionHSI(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,BYTE *lpMultiBand)
{
	DWORD dwSize = nHeight*nWidth;
	double R,G,B,dScalor,I;

	static double ddError = 0;

	long i; for( i=0; i<dwSize; i++ )
	{
		R = lpMultiBand[i*3+2];
		G = lpMultiBand[i*3+1];
		B = lpMultiBand[i*3];

		I = ( R + G + B ) / 3.0;
		
		if( I < 0.00001 )
		{
			lpMultiBand[i*3+2] = 0;
			lpMultiBand[i*3+1] = 0;
			lpMultiBand[i*3]   = 0;
			continue;
		}

		//double H0,S0,I0;
		//GetHSIFromRGB(R,G,B,H0,S0,I0);
		
		dScalor = ((double)lpPanBand[i] - I);// / I;
		//dScalor = ((double)lpPanBand[i] / I);
		//R *= dScalor;
		//G *= dScalor;
		//B *= dScalor;

		R = R + dScalor;//*R + 0.5;
		G = G + dScalor;//*G + 0.5;
		B = B + dScalor;//*B + 0.5;

		//double H,S,I;
		//GetHSIFromRGB(R,G,B,H,S,I);

		//ddError += fabs(H - H0);

		R += 0.5;
		G += 0.5;
		B += 0.5;

		if( R < 0 )	R = 0;
		if( G < 0 ) G = 0;
		if( B < 0 ) B = 0;

		lpMultiBand[i*3+2] = R > 255 ? 255 : (int) R;
		lpMultiBand[i*3+1] = G > 255 ? 255 : (int) G;
		lpMultiBand[i*3]   = B > 255 ? 255 : (int) B; 
	}

	return;
}
//Image fussion
void CZXDib::Fussion(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,
					 BYTE *lpMultiBand)
{
    int flage;
    double ii,a,b,c,I,H,S,dr,dg,db;

	DWORD dwSize = nHeight*nWidth;
    for(int i=0; i<dwSize; ++i)
	{
		//if(lpRedBand[i*nWidth+j] < lpGreenBand[i*nWidth+j] && lpRedBand[i*nWidth+j] < lpBlueBand[i*nWidth+j])
		if(lpMultiBand[i*3+2] < lpMultiBand[i*3+1] && lpMultiBand[i*3+2] < lpMultiBand[i*3])
		{
			a=(double)(lpMultiBand[i*3]);
			b=(double)(lpMultiBand[i*3+2]);
			c=1;
			flage=0;
		}
		else if(lpMultiBand[i*3+1] < lpMultiBand[i*3+2] && lpMultiBand[i*3+1] < lpMultiBand[i*3])
		{
			a=(double)(lpMultiBand[i*3+2]);
			b=(double)(lpMultiBand[i*3+1]);
			c=2;
			flage=1;
		}
		else if(lpMultiBand[i*3] < lpMultiBand[i*3+2] && lpMultiBand[i*3] < lpMultiBand[i*3+1])
		{
			a=(double)(lpMultiBand[i*3+1]);
			b=(double)(lpMultiBand[i*3]);
			c=0;
			flage=2;
		}
		else
		{
			a=(double)(lpMultiBand[i*3+1]);
			b=(double)(lpMultiBand[i*3]);
			c=0;
			flage=3;
		}
		
		I = 0;
		for( long j=0; j<3; j++)
			I += lpMultiBand[i*3+j];
		if(flage != 3)
			;//I=(double)(lpMultiBand[i*3+2]+lpMultiBand[i*3+1]+lpMultiBand[i*3]);
		else 
			I++;//I=(double)(lpMultiBand[i*3+2]+lpMultiBand[i*3+1]+lpMultiBand[i*3])+1;
		
		if(I != 0)
		{
			H=(a-b)/(I-3*b)+c;
			S=(I-3*b)/I;
		}
		else
		{
			H=0;
			S=0;
			continue;
		}
		
		//if(lpPanBand[i] == 0) 
		//	ii=0;//(int)(I/3);
		//else 
			ii=lpPanBand[i];		
		//ii = I / 3;
		
		switch(flage)
		{
		case 0:
			dr=ii*(1-S);
			dg=ii*(1+5*S-3*S*H);
			db=ii*(1-4*S+3*S*H);
			break;
		case 1:
			dr=ii*(1-7*S+3*S*H);
			dg=ii*(1-S);
			db=ii*(1+8*S-3*S*H);
			break;
		case 2:
			dr=ii*(1+2*S-3*S*H);
			dg=ii*(1-1*S+3*S*H);
			db=ii*(1-S);
			break;
		case 3:
			dr=ii*(1+2*S-3*S*H);
			dg=ii*(1-1*S+3*S*H);
			db=ii*(1-S);
			break;
		default:break;
		}

		dr+=0.5;
		dg+=0.5;
		db+=0.5;

		if(dr < 0)	dr=0;
		if(dg < 0)	dg=0;
		if(db < 0)	db=0;
		
		
		if(dr >255) 
			lpMultiBand[i*3+2]  = 255;//dr=255;
		else 
			lpMultiBand[i*3+2]  =(int)(dr);

		if(dg >255) 
			lpMultiBand[i*3+1]  = 255;//dg=255;
		else 
			lpMultiBand[i*3+1]  = (int)(dg);

		if(db >255)	
			lpMultiBand[i*3]    = 255;//db=255;
		else 
			lpMultiBand[i*3]    = (int)(db);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheColorMatric1(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color,double dLineWidth)
{
    DWORD position=0;
    double position1,H,S,I,r,g,b;
    int temp,nx,ny,m,n;
    double temp1,temp2,dTemp,dTemp3,dTemp4,dPower;
    double dLengthB = (y_from-y_to)*(y_from-y_to) + (x_from-x_to)*(x_from-x_to);

    double HT,ST,IT,HF,SF,IF;

    if( dLineWidth < 1 )        // 认为线宽需大于1
        return false;

    if( dLengthB < 0.000001 )
        return false;
    
    dLengthB = 1 / dLengthB;

    GetHSIFromRGB(color.rgbRed,color.rgbGreen,color.rgbBlue,HT,ST,IT);
  
    //y_from = dwHeight - 1 - y_from;
    //y_to   = dwHeight - 1 - y_to;

    if(abs(x_from-x_to)>abs(y_from-y_to))
    {
     if(x_from>x_to)
     {
	     temp   = x_to;
	     x_to   = x_from;
	     x_from = temp;
	     temp   = y_to;
	     y_to   = y_from;
	     y_from = temp;
     }
     temp1  = y_to-y_from;
     temp2  = x_to-x_from;
     temp1 /= temp2;
 
     position1 = y_from;
     position  = y_from;
     int i; for( i=x_from; i<=x_to; i++)
     {
         for( m=-1; m<=1; m++)
         for( n=-1; n<=1; n++ )
         {
             ny = position;
             if( i+m < 0 || i+m > dwWidth-1 )
                 continue;
             if( ny+n < 0 || ny+n > dwHeight-1 )
                 continue;
             
             dPower =  ((i+m)-x_from) * ((i+m)-x_from) + ((ny+n)-y_from) * ((ny+n)-y_from);
             dTemp3 = (((i+m)-x_from) *  (x_to-x_from) + ((ny+n)-y_from) *  (y_to-y_from) );
             dTemp4 = dTemp3 * dTemp3 * dLengthB;
             
             if( dPower - dTemp4 > 1 )
                 continue;

             GetHSIFromRGB(
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+2],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+1],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3],HF,SF,IF);             
               
             GetRGBFromHSI(
                 HT,//F + (1 - pow((dPower-dTemp4),0.25)) * ( HT - HF),
                 SF + (1 - pow((dPower-dTemp4),0.25)) * ( ST - SF),
                 IF + (1 - pow((dPower-dTemp4),0.25)) * ( IT - IF),
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+2],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+1],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3]);
         }

	    position1 += temp1;
	    position  = (DWORD) (position1+0.5);
     }
    }
    else 
    {
     if(y_from>y_to)
     {
	     temp   = y_to;
	     y_to   = y_from;
	     y_from = temp;
	     temp   = x_to;
	     x_to   = x_from;
	     x_from = temp;
     }
     temp1  = x_to-x_from;
     temp2  = y_to-y_from;
     temp1 /= temp2;
 
     position1 = x_from;
     position = x_from;
     int i; for( i=y_from; i<=y_to; i++)
     {
         for( m=-1; m<=1; m++)
         for( n=-1; n<=1; n++ )
         {
             nx = position;
             if( i+m < 0 || i+m > dwHeight-1 )
                 continue;
             if( nx+n < 0 || nx+n > dwWidth-1 )
                 continue;
             
             dPower =  ((i+m)-y_from) * ((i+m)-y_from) + ((nx+n)-x_from) * ((nx+n)-x_from);
             dTemp3 = (((i+m)-y_from) *  (y_to-y_from) + ((nx+n)-x_from) *  (x_to-x_from) );
             dTemp4 = dTemp3 * dTemp3 * dLengthB;
             
             if( dPower - dTemp4 > 1 )
                 continue;
             
             GetHSIFromRGB(
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+2],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+1],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3],HF,SF,IF);
                          
             GetRGBFromHSI(
                 HT,//F + (1 - pow((dPower-dTemp4),0.25)) * ( HT - HF),
                 SF + (1 - pow((dPower-dTemp4),0.25)) * ( ST - SF),
                 IF + (1 - pow((dPower-dTemp4),0.25)) * ( IT - IF),
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+2],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+1],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3]);
         }
	    position1 += temp1;
	    position  = (DWORD) (position1+0.5);
     }
    }
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中画线:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawLineInTheColorMatric1(int x_from, int y_from, int x_to, int y_to, BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color)
{
    DWORD position=0;
    double position1,H,S,I,r,g,b;
    int temp,nx,ny,m,n;
    double temp1,temp2,dTemp,dTemp3,dTemp4,dPower;
    double dLengthB = (y_from-y_to)*(y_from-y_to) + (x_from-x_to)*(x_from-x_to);

    double HT,ST,IT,HF,SF,IF;

    if( dLengthB < 0.000001 )
        return false;
    
    dLengthB = 1 / dLengthB;

    GetHSIFromRGB(color.rgbRed,color.rgbGreen,color.rgbBlue,HT,ST,IT);
  
    //y_from = dwHeight - 1 - y_from;
    //y_to   = dwHeight - 1 - y_to;

    if(abs(x_from-x_to)>abs(y_from-y_to))
    {
     if(x_from>x_to)
     {
	     temp   = x_to;
	     x_to   = x_from;
	     x_from = temp;
	     temp   = y_to;
	     y_to   = y_from;
	     y_from = temp;
     }
     temp1  = y_to-y_from;
     temp2  = x_to-x_from;
     temp1 /= temp2;
 
     position1 = y_from;
     position  = y_from;
     int i; for( i=x_from; i<=x_to; i++)
     {
         for( m=-1; m<=1; m++)
         for( n=-1; n<=1; n++ )
         {
             ny = position;
             if( i+m < 0 || i+m > dwWidth-1 )
                 continue;
             if( ny+n < 0 || ny+n > dwHeight-1 )
                 continue;
             
             dPower =  ((i+m)-x_from) * ((i+m)-x_from) + ((ny+n)-y_from) * ((ny+n)-y_from);
             dTemp3 = (((i+m)-x_from) *  (x_to-x_from) + ((ny+n)-y_from) *  (y_to-y_from) );
             dTemp4 = dTemp3 * dTemp3 * dLengthB;
             
             if( dPower - dTemp4 > 1 )
                 continue;

             GetHSIFromRGB(
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+2],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+1],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3],HF,SF,IF);             
               
             GetRGBFromHSI(
                 HT,//F + (1 - pow((dPower-dTemp4),0.25)) * ( HT - HF),
                 SF + (1 - pow((dPower-dTemp4),0.10*(SF+1))) * ( ST - SF),
                 IF + (1 - pow((dPower-dTemp4),0.10*(SF+1))) * ( IT - IF),
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+2],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3+1],
                 pBitmap[(i+m+(ny+n)*dwWidth)*3]);
         }

	    position1 += temp1;
	    position  = (DWORD) (position1+0.5);
     }
    }
    else 
    {
     if(y_from>y_to)
     {
	     temp   = y_to;
	     y_to   = y_from;
	     y_from = temp;
	     temp   = x_to;
	     x_to   = x_from;
	     x_from = temp;
     }
     temp1  = x_to-x_from;
     temp2  = y_to-y_from;
     temp1 /= temp2;
 
     position1 = x_from;
     position = x_from;
     int i; for( i=y_from; i<=y_to; i++)
     {
         for( m=-1; m<=1; m++)
         for( n=-1; n<=1; n++ )
         {
             nx = position;
             if( i+m < 0 || i+m > dwHeight-1 )
                 continue;
             if( nx+n < 0 || nx+n > dwWidth-1 )
                 continue;
             
             dPower =  ((i+m)-y_from) * ((i+m)-y_from) + ((nx+n)-x_from) * ((nx+n)-x_from);
             dTemp3 = (((i+m)-y_from) *  (y_to-y_from) + ((nx+n)-x_from) *  (x_to-x_from) );
             dTemp4 = dTemp3 * dTemp3 * dLengthB;
             
             if( dPower - dTemp4 > 1 )
                 continue;
             
             GetHSIFromRGB(
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+2],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+1],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3],HF,SF,IF);
                          
             GetRGBFromHSI(
                 HT,//F + (1 - pow((dPower-dTemp4),0.25)) * ( HT - HF),
                 SF + (1 - pow((dPower-dTemp4),0.20)) * ( ST - SF),
                 IF + (1 - pow((dPower-dTemp4),0.20)) * ( IT - IF),
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+2],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3+1],
                 pBitmap[((i+m)*dwWidth+(nx+n))*3]);
         }
	    position1 += temp1;
	    position  = (DWORD) (position1+0.5);
     }
    }
    return true;
}
////////////////////////////////////////////////////////////////////////
//
//  直方图平滑:
//
////////////////////////////////////////////////////////////////////////
bool CZXDib::HistogramSmoothBy5Points(DWORD *pHg,int level)
{
	int i;
	int temp;
	int *pTemp = new int[level];

	for(i=0; i<level; i++)
		pTemp[i] = pHg[i];
	
	temp = (69*pTemp[0]+4*pTemp[1]-6*pTemp[2]+4*pTemp[3]-pTemp[4])/70;
	if(temp<0) temp = 0;
	pHg[0] = (WORD)temp;
	temp = (2*pTemp[0]+27*pTemp[1]+12*pTemp[2]-8*pTemp[3]+2*pTemp[4])/35;
	if(temp<0) temp = 0;
	pHg[1] = (WORD)temp;

	for(i=2; i<level-2; i++)
	{
		temp = (-3*pTemp[i-2]+12*pTemp[i-1]+17*pTemp[i]+12*pTemp[i+1]-3*pTemp[i+2])/35;
		if(temp<0) temp=0;
		pHg[i] = (WORD)temp;
	}

	temp = (2*pTemp[level-5]-8*pTemp[level-4]+12*pTemp[level-3]+27*pTemp[level-2]+2*pTemp[level-1])/35;
	if(temp<0) temp = 0;
	pHg[level-2] = (WORD)temp;
	temp = (-1*pTemp[level-5]+4*pTemp[level-4]-6*pTemp[level-3]+4*pTemp[level-2]+69*pTemp[level-1])/70;
	if(temp<0)	temp = 0;
	pHg[level-1] = (WORD)temp;
	
	delete pTemp;
	return true;
}
// 擦出周边
void CZXDib::EreaseRound(double* pBitmap,DWORD dwHeight,DWORD dwWidth,int nLeft,int nTop,int nRight,int nBottom,double bValue)
{
	long i,j;

	for( j=0; j<dwWidth; j++ )
	{
		for( i=0; i<nTop;    i++ )	
			pBitmap[i*dwWidth+j] = bValue;

		for( i=dwHeight-nBottom; i<dwHeight; i++ )	
			pBitmap[i*dwWidth+j] = bValue;
	}
	
	for( i=0; i<dwHeight; i++ )
	{
		for( j=0; j<nLeft; j++ )
			pBitmap[i*dwWidth+j] = bValue;
	
		for( j=dwWidth-nRight; j<dwWidth; j++ )
			pBitmap[i*dwWidth+j] = bValue;
	}
}
double CZXDib::GetMean(double* pdData, int nLengh)
{
	double dSum = 0;
	for( int i=0; i<nLengh; i++ )
		dSum += pdData[i];
	dSum /= nLengh;
	return dSum;
}
void CZXDib::GetCurvHis(int* pHis, int nLength, int nFilterSize, double*& pResult)
{
	delete pResult;
	pResult = new double[nLength];
	memset(pResult, 0, sizeof(double)*nLength);

	int nHW = max(1,nFilterSize / 2);

	int i; for( i=nHW; i<nLength-nHW; i++ )
	{
		double y1 = pHis[i- nHW];
		double y2 = pHis[i+ nHW];
		double x1 = i-nHW;
		double x2 = i+nHW;
		double a = y2 - y1;
		double b = x1 - x2;
		double c = x2 * y1 - x1 * y2;
		double dTemp = ( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) );

		double d1 = a * i + b * pHis[i] + c; 
		pResult[i] = d1 * d1 / dTemp;
		if( d1 < 0 )
			pResult[i] *= 0;
	}	
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
double CZXDib::RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult)
{
	DWORD i,j;
	double max=pBitmap[0];
	double min=pBitmap[0];
	double temp;

	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}

	return max + min;
}
void CZXDib::RegularToGrayLevel(float *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult)
{
	DWORD i,j;
	float max=pBitmap[0];
	float min=pBitmap[0];
	float temp;

	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
int CZXDib::RegularToGrayLevel(int *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult)
{
	DWORD i,j;
	int max=pBitmap[0];
	int min=pBitmap[0];
	int temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}

	return max + min;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
int CZXDib::RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth)
{
	int max=0;
	int min=255;
	int i, nSize = dwHeight * dwWidth;
		
	for(i=nSize-1; i>=0; --i)
	{
		if (pBitmap[i] > max) max = pBitmap[i];
		else if (pBitmap[i] < min) min = pBitmap[i];
	}
	max -= min;
	if (!max)
	{
		memset(pBitmap, 0, dwHeight*dwWidth);
		return 0;
	}

	for (i = nSize - 1; i >= 0; --i)
		pBitmap[i] = (pBitmap[i] - min) * 255 / max;

	return max + min;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
void CZXDib::RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double& dMax,double& dMin,BYTE*& pResult)
{
	DWORD i,j;
	double max=pBitmap[0];
	double min=pBitmap[0];
	double temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}
	dMax = max+min;
	dMin = min;

}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevel(double *pBitmap, DWORD dwHeight, DWORD dwWidth,RECT& rect,BYTE*& pResult)
{
	DWORD i,j;
	double max=pBitmap[0];
	double min=pBitmap[0];
	double temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	for(i=rect.top; i<rect.bottom;i++)
	for(j=rect.left; j<rect.right; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,RECT& rect,BYTE*& pResult)
{
	DWORD i,j;
	double max=pBitmap[0];
	double min=pBitmap[0];
	double temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	for(i=rect.top; i<rect.bottom;i++)
	for(j=rect.left; j<rect.right; j++)
	{
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];
	}
	
	max -= min;
	
	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}
	
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevel1(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double& dMax,double& dMin,BYTE*& pResult)
{
	DWORD i,j;
	double dInter = 1;
	double temp;
	
	dMax=pBitmap[0];
	dMin=pBitmap[0];
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[i*dwWidth+j]>dMax) dMax = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<dMin) dMin = pBitmap[i*dwWidth+j];
	}

	dInter = dMax - dMin;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-dMin)*255.0/dInter + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevelWidthMaxMin(double *pBitmap, DWORD dwHeight, DWORD dwWidth,double dMax,double dMin,BYTE*& pResult)
{
	DWORD i,j;
	double dInter = 1;
	double temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	dInter = dMax - dMin;
		
	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-dMin)*255.0/dInter + 0.5;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
	}
			
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevel(DWORD *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult)
{
	DWORD i,j;
	DWORD max=pBitmap[0];
	DWORD min=pBitmap[0];
	DWORD temp;

	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];

	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max;
		pResult[i*dwWidth+j] = (BYTE) temp;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::RegularToGrayLevel(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult)
{
	DWORD i,j;
	DWORD max=pBitmap[0];
	DWORD min=pBitmap[0];
	DWORD temp;

	BYTE* pTemp = new BYTE[dwHeight*dwWidth];

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
		if(pBitmap[i*dwWidth+j]>max) max = pBitmap[i*dwWidth+j];
		else
		if(pBitmap[i*dwWidth+j]<min) min = pBitmap[i*dwWidth+j];

	max -= min;
	if(max==0) max = min = 1;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255/max;
		pTemp[i*dwWidth+j] = (BYTE) temp;
	}

	if(pResult) delete pResult;
	pResult = pTemp;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  求图像的均值图像
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::MeanImage(BYTE **pBitmap, DWORD dwHeight, DWORD dwWidth, int nNumber, BYTE *&pResult,WORD flag)
{
	DWORD i,j,counter,position;
	int k;
	if(flag == 24) dwWidth*=3;

	if(pResult) delete pResult;
	pResult = (BYTE*) new BYTE[dwHeight*dwWidth];

	position = 0;
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{
		counter = 0;
		for(k=0; k<nNumber; k++)
			counter += pBitmap[k][position];
		counter /= nNumber;
		if(counter>255) counter = 255;
		pResult[position] =(BYTE) counter;
		position ++;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  求多个图像的方差图像：给定均值图像:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::VarianceImage(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *pMeanImage, BYTE *&pResultImage,WORD flag)
{
	DWORD i,j;
	int k;
	int counter;
	int temp;

	if(pResultImage) delete pResultImage;
	if(flag==8)
		pResultImage = new BYTE [dwHeight*dwWidth];
	else
		if(flag==24)
			pResultImage = new BYTE [dwHeight*dwWidth*3];

	if(flag ==8)
	{
		for(i=0;i<dwHeight; i++)
		for(j=0; j<dwWidth; j++)
		{
			counter = pBitmap[i*dwWidth+j];
			counter -= pMeanImage[i*dwWidth+j];
			if(counter<0)
				counter = 0-counter;
			if(counter>255) counter = 255;
			/*counter *= 360;
			counter /= 255;
			if(counter>180) counter = 360-counter;
			counter *=255;
			counter /=360;
			*/

			pResultImage[i*dwWidth+j] = (BYTE) counter;
		}
	}
	else
	if(flag==24)
	{
		for(i=0;i<dwHeight; i++)
		for(j=0; j<dwWidth; j++)
		{
			counter = 0;
			for(k=0;k<2;k++)
			{
				temp = (int)(pBitmap[(i*dwWidth+j)*3+k]) - (int)(pMeanImage[(i*dwWidth+j)*3+k]);
				if(k==0)
				{
					temp *= 360;
					temp /= 255;
				    if(temp>180) temp= 360-temp;
					temp*=255;
					temp/=360;
					temp *= 0.0;
				}
				else
					temp *=1.0;
				if(temp<0) temp = 0-temp;
				if(counter<temp) counter = temp;
				//temp *= temp;
				//counter += temp;
			}
			//counter /= 3;
			//counter = (int) sqrt((double)counter);
			//if(counter>255) counter = 255;

			pResultImage[i*dwWidth+j] = (BYTE) counter;
		}
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  求多个图像的方差图像：给定均值图像:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::VarianceImage(BYTE **ppBitmap, DWORD dwHeight, DWORD dwWidth,int nFileNumber, BYTE *pMeanImage, BYTE *&pResultImage,WORD flag)
{
	DWORD i,j;
	int k,l;
	int counter,counter1;
	int temp;

	if(pResultImage) delete pResultImage;
	if(flag==8)
		pResultImage = new BYTE [dwHeight*dwWidth];
	else
		if(flag==24)
			pResultImage = new BYTE [dwHeight*dwWidth*3];

	if(flag ==8)
	{
		for(i=0;i<dwHeight; i++)
		for(j=0; j<dwWidth; j++)
		{
			counter = 0;
			for(k=0;k<nFileNumber;k++)
			{
				temp =  ppBitmap[k][i*dwWidth+j];
			    temp -= pMeanImage[i*dwWidth+j];
				temp *= temp;
				counter += temp;
			}
			counter /= nFileNumber;
			counter = (int) sqrt((double)counter);
			if(counter>255) counter = 255;
			pResultImage[i*dwWidth+j] = (BYTE) counter;
		}
	}
	else
	if(flag==24)
	{
		for(i=0;i<dwHeight; i++)
		for(j=0; j<dwWidth; j++)
		{
			counter = 0;
			counter1 = 0;
			for(l=0;l<nFileNumber;l++)
			{
				for(k=0;k<3;k++)
				{
					temp = (int)(ppBitmap[l][(i*dwWidth+j)*3+k]) - (int)(pMeanImage[(i*dwWidth+j)*3+k]);
					temp *= temp;
					counter += temp;
				}
				counter /= 3;
				counter = (int) sqrt((double)counter);
				counter1 += counter;
			}

			counter1/=nFileNumber;
			counter1 = (int) sqrt((double)counter1);

			if(counter1>255) counter1 = 255;

			pResultImage[i*dwWidth+j] = (BYTE) counter1;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 粗定位，通过多张背景图像和多张目标图像求目标图像到背景图像的均值背景图像的方差图像：
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CoarsePositionByMeanAndVariance(BYTE **pBKImage, DWORD dwHeight, DWORD dwWidth, int nNumberBK, BYTE *pInputImage,RECT &rect,WORD flag)
{
	DWORD i,j;
	BYTE* pMean=NULL;

	//求均值：
	MeanImage(pBKImage,dwHeight,dwWidth,nNumberBK,pMean,flag);

	//转化为HSI空间：
	BYTE* pBKHSI=NULL;
	GetHSIFromRGB(pMean,dwHeight,dwWidth,pBKHSI);
	BYTE* pTGHSI=NULL;
	GetHSIFromRGB(pInputImage,dwHeight,dwWidth,pTGHSI);
	//求图像的方差：
	BYTE* pVariance=NULL;
//  VarianceImage(pInputImage,dwHeight,dwWidth,pMean,pVariance,flag);
	VarianceByRGB2thColor(pTGHSI,dwHeight,dwWidth,pBKHSI,pVariance);

	//直方图：
//	BYTE* pHg=NULL;
//	Histogram(pVariance,dwHeight,dwWidth,pHg);

	//直方图平滑
//	for(i=0;i<18;i++)
//  	  HistogramSmoothBy5Points(pHg);

	//找到一个门限点,并二值化：
//	j = 1;
//	for(i=255; i>1 && j; i--)
//		if(pHg[i]<=4 && pHg[i-1]>4  ) j=0;
//	BlackAndWhite(pVariance,dwHeight,dwWidth,(BYTE)i);
	SegmentByDownHill(pVariance,dwHeight,dwWidth);
    //SegmentByOTSU(pVariance,dwHeight,dwWidth);

    //用小矩形来过滤输入图像
//	Cover2ValueImageByRect(pVariance,dwHeight,dwWidth,6,6);

	//得到目标矩形框
	ProjectAndFindTargetRegion(pVariance,dwHeight,dwWidth,rect);

//	delete pHg;
	delete pVariance;
	delete pMean;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//
//  基于彩色的图像分割
//
////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ColorBasedSegment(BYTE *pColorBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	if(pResult) delete pResult;
	pResult = new BYTE[dwWidth*dwHeight];

	double r,g,b;
	double x;
	DWORD i,j;
	double S,H,I;

	DWORD position=0;
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{ 
		b = (double)(pColorBitmap[position])/255.0;
		g = (double)(pColorBitmap[position+1])/255.0;
		r = (double)(pColorBitmap[position+2])/255.0;
		
		I = (r+g+b)/3;
		x = r;
		if( x>g ) x = g;
		if( x>b ) x = b;
		if(I<0.000001)
			S = 0;
		else
			S = 1-x/I;
		if(S<0.000001)
			H = 0;
		else
		{
			x = (r-g+r-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b)));
			if(x>=1.0) 
				H = 0;
			else 
			  H = (acos(x)/3.1415926)*180; 
		}
		if(g<b) H=360-H;

		if((H<36)&&(H>5)||(H>340))
			pResult[position/3] = 255;
		else 
			pResult[position/3] = 0;

		position += 3;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////
// 生成一个BITMAPINFO结构
////////////////////////////////////////////////////////////////////////
bool CZXDib::CreateBITMAPINFO(BITMAPINFO *&pbi,DWORD dwHeight, DWORD dwWidth,WORD flag)
{
	if(flag==24) dwWidth *= 3;
	DWORD dwWriteSize  = dwHeight * ((dwWidth+3)/4*4);

	if(pbi) delete pbi;	
	if(flag == 24) 
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];
	else 
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)+255*sizeof(RGBQUAD)];
	pbi->bmiHeader.biSize = sizeof(BITMAPINFO)-4;//无论是黑白图还是彩图都是40
	pbi->bmiHeader.biBitCount = flag;
	pbi->bmiHeader.biHeight = dwHeight;

	if(flag ==24) 
		pbi->bmiHeader.biWidth = dwWidth/3;
	else 
		pbi->bmiHeader.biWidth = dwWidth;

	pbi->bmiHeader.biSizeImage = dwWriteSize;
    pbi->bmiHeader.biCompression= 0;
	pbi->bmiHeader.biPlanes = 1;
	pbi->bmiHeader.biClrImportant = 0;
	pbi->bmiHeader.biClrUsed = 0;
	pbi->bmiHeader.biXPelsPerMeter = 
		pbi->bmiHeader.biYPelsPerMeter=0;
	pbi->bmiColors[0].rgbBlue = 
		pbi->bmiColors[0].rgbGreen=
		pbi->bmiColors[0].rgbRed=
		pbi->bmiColors[0].rgbReserved=0;
	
	//若是256色图像则，写调色板
	if( flag==8 )
	{
		for(int i=1; i<256; i++)
		{
			BYTE temp = (BYTE) i;
			pbi->bmiColors[i].rgbRed =
				pbi->bmiColors[i].rgbGreen=
				pbi->bmiColors[i].rgbBlue = (BYTE) i;
			pbi->bmiColors[i].rgbReserved = 0;

		}
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//pBitmap为灰度图象,flag为目标的灰度值
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetRidOfSmallBlock(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE flag,DWORD dwMinBlockSize)
{
	DWORD temp = 0;
	BYTE colorarray[1000]; //设最大有1000个区域:
	WORD color_area = 0;   //设当前用到了多少个
	DWORD i,j,k,temp_color,m,n;
	DWORD Number;
	WORD  Delete;
	BYTE egg;			   //中间颜色：
	bool modify;		   //修改标志
	
	for(i=0;i<260; i++)
		colorarray[i] = 0;
	
	DWORD position=0;
	DWORD position_l;
	for( i=0;i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[position]==flag)
		{
			Number=0;
			egg = 250;
			Delete = color_area+1;
			SeedGrow(i,j,pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
			modify = false;
			k=1;
			temp_color=0;
			for(k=1;(k<=color_area) && (!modify);k++)
			{  
				if( colorarray[k]==2 )
				{
					modify = TRUE;
					temp_color = k;
					colorarray[k]=1;
					position_l=0;
					for(m=0; m<dwHeight; m++)
					for(n=0; n<dwWidth; n++)
					{
						if(pBitmap[position_l] == egg)
							pBitmap[position_l] = (BYTE) k;
						position_l++;
					}
				}
			}
			while((k<=color_area) && (colorarray[k]!=2))
				k++;
			if (k<=color_area)  //处理多区域杂交的情况
			{
				position_l=0;
				for(m=0; m<dwHeight; m++)
				for(n=0; n<dwWidth; n++)
				{   
					k = temp_color;
					while ((k<=color_area)&&(pBitmap[position_l]))
					{
						if(colorarray[k] == 2)
						{
							if(pBitmap[position_l] == k)
								pBitmap[position_l] = (BYTE) temp_color;
						}
						k++;
					}
					position_l++;
				}
				k=temp_color;
				while (k<=color_area)
				{
					if( colorarray[k] ==2 ) 
						colorarray[k] = 0;
					k++;
				}
			}

			if((!modify)&&(Number<dwMinBlockSize))
			{  //如果该区域的总数比较小，则将之删除
				DWORD position_l = 0;
				for(DWORD m=0; m<dwHeight; m++)
				for(DWORD n=0; n<dwWidth; n++)
				{
					if(pBitmap[position_l] == 250)
						pBitmap[position_l] = 0;
					position_l ++;
				}
			}
			//修改当前大区域为已有的区域编号:
			else if(!modify)
				{
					color_area++;  //总区域数加一
					colorarray[color_area] = 1; //相应的区域标志设为1
					position_l=0;  //修改大区域像素灰度为它的区域编号
					for(m=0; m<dwHeight; m++)
					for(n=0; n<dwWidth; n++)
					{
						if(pBitmap[position_l] == 250)
							pBitmap[position_l] = (BYTE) color_area;
						position_l ++;
					}
				}
		}
		position++;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  种子填充算法: 将在原图上进行修改，
//  i,j表示当前种子坐标，
//  Number 是最大迭代深度
//  pBitmap,dwHeight,dwWidth 为输入图像指针和宽度和高度
//  flag 目标颜色，种子生长空间的颜色
//  egg  处理结果色
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SeedGrow(DWORD i,DWORD j,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, 
                      DWORD& Number,
                      BYTE& flag,
                      BYTE& egg)
{  

    Number++;      //该区域像素个数加一

    if(Number>8000) 
    {
        Number --;
        return false; //设置递归最大层数
    }
    
    //边界条件
    if(i>=dwHeight || i< 0 || j<0 || j>=dwWidth ) 
    {
        Number --;
        return true;
    }

    DWORD position = i*dwWidth+j;    
  
    if(pBitmap[position] != flag)  //如果不是目标
    {
        Number --;
        return true;
    }   
    

    pBitmap[position] = egg;     //修改为目标色

    
    
    //下面使用了四邻域种子填充算法:
    SeedGrow(i+1, j,   pBitmap,dwHeight,dwWidth,Number,flag,egg);
    //	SeedGrow(i+1, j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    SeedGrow(i  , j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    //	SeedGrow(i-1, j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    SeedGrow(i-1, j  , pBitmap,dwHeight,dwWidth,Number,flag,egg);
    //	SeedGrow(i-1, j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    SeedGrow(i  , j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    //	SeedGrow(i+1, j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg);
    
    Number --;
    return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  种子填充算法: 将在原图上进行修改，
//  i,j表示当前种子坐标，
//  Number 是最大迭代深度
//  pBitmap,dwHeight,dwWidth 为输入图像指针和宽度和高度
//  flag 目标颜色，种子生长空间的颜色
//  egg  中间颜色，既不是背景也不是目标，用于标志当前点是否已经被处理过
//  delete 
//  colorarray 标志区域
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SeedGrow(DWORD i,DWORD j,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, 
					  DWORD& Number,
					  BYTE& flag,
					  BYTE& egg,
					  WORD& Delete,
					  BYTE* colorarray)
{  
	//边界条件
	if(i>=dwHeight) return true;
	if(i<0) return true;
	if(j<0) return true;
	if(j>=dwWidth)  return true;
	
	DWORD position = i*dwWidth+j;
	
	if(Delete)   //??改变量的设置是为了一旦检测出本区域过小时就予以删除时，共享此该代码??may not be so
	{
		if(colorarray[pBitmap[position]])  //如果该点为已经扫描过的点，则加上标志，用于两个区域的合并:
			colorarray[pBitmap[position]] = 2;
	}
	if(pBitmap[position] != flag)  //如果不是目标
		return true;
	if(Number>6000) return true; //设置递归最大层数
	pBitmap[position] = egg;     //修改为中间色
	Number++;                    //该区域像素个数加一
   
	//下面使用了四邻域种子填充算法:
	SeedGrow(i+1, j,   pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
//	SeedGrow(i+1, j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
	SeedGrow(i  , j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
//	SeedGrow(i-1, j+1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
	SeedGrow(i-1, j  , pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
//	SeedGrow(i-1, j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
	SeedGrow(i  , j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
//	SeedGrow(i+1, j-1, pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);

	return true;
}

bool CZXDib::ScanRegion(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE bTargetColor,int*& pResult)
{
	DWORD i,j,m,n,k,l;
	DWORD nCurIterNum = 0;
	int  nRegionNumber = 0;
	bool bFound;

	if( pResult ) delete []pResult;
	pResult = new int [dwHeight*dwWidth];
	memset(pResult,0,sizeof(int)*dwHeight*dwWidth);

	for( i=0; i<dwHeight; i++ )
	for( j=0; j<dwWidth;  j++ )
	{
		if( pBitmap[i*dwWidth+j] != bTargetColor && !pResult[i*dwWidth+j] )
			continue;

		nRegionNumber ++;

		if( SeedGrow(i, j, nCurIterNum, bTargetColor, nRegionNumber, pBitmap, dwHeight, dwWidth, pResult) )
			continue;
		
		do
		{		
			bFound = false;
			for( k=0; k<dwHeight && !bFound; k++ )
			for( l=0; l<dwWidth  && !bFound; l++ )
			{				
				if( pResult[k*dwWidth+l] != nRegionNumber )
					continue;

				for( m=k-1; m<=k+1; m++ )
				{
					if( m < 0 || m >= dwHeight )  continue;

					for( n=l-1; n<=l+1; n++ )
					{
						if( n < 0 || n >= dwWidth ) continue;
						if( pBitmap[m*dwWidth+n] != bTargetColor || pResult[m*dwWidth+n] == nRegionNumber)
							continue;
					
						bFound = true;
						break;
					}
					if( bFound ) break;
				}
			}

			if( !bFound ) break;
			
			SeedGrow(m, n, nCurIterNum, bTargetColor, nRegionNumber, pBitmap, dwHeight, dwWidth, pResult);
		}while(bFound);
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  种子填充算法: 将在原图上进行修改，
//  i,j表示当前种子坐标，
//  Number 是最大迭代深度
//  pBitmap,dwHeight,dwWidth 为输入图像指针和宽度和高度
//  flag 目标颜色，种子生长空间的颜色
//  egg  处理结果色
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SeedGrow(DWORD i,DWORD j,DWORD& Number,BYTE flag,int & egg,BYTE *pBitmap, DWORD& dwHeight, DWORD& dwWidth, int* pResult)
{  
    Number++;      //该区域像素个数加一
	
    if(Number>6000) 
    {
        Number --;
        return false; //设置递归最大层数
    }
    
    //边界条件
    if(i>=dwHeight || i< 0 || j<0 || j>=dwWidth ) 
    {
        Number --;
        return true;
    }
	
    if( pBitmap[i*dwWidth+j] != flag || pResult[i*dwWidth+j] )  //如果不是目标
    {
        Number --;
        return true;
    }   
    
	pResult[i*dwWidth+j] = egg; //修改为目标色 
	    
    
    //下面使用了四邻域种子填充算法:
    if( !SeedGrow(i+1, j  ,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
	if( !SeedGrow(i+1, j+1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
    if( !SeedGrow(i  , j+1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
	if( !SeedGrow(i-1, j+1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
    if( !SeedGrow(i-1, j  ,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
	if( !SeedGrow(i-1, j-1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
    if( !SeedGrow(i  , j-1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
	if( !SeedGrow(i+1, j-1,Number, flag,egg, pBitmap,dwHeight,dwWidth,pResult) ) { Number --; return false;} 
    
    Number --;
    return true;
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  用小矩形去腐蚀输入图像
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Uncover2ValueImageByRect(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,DWORD dwRectHeight,DWORD dwRectWidth)
{
 	DWORD i,j,k,l;
	DWORD temp;
	BYTE* pResult=NULL;
	DWORD upgate = (DWORD)(dwRectHeight*dwRectWidth*0.80);
	DWORD downgate = (DWORD)(dwRectHeight*dwRectWidth*0.30);
	pResult = (BYTE*) new BYTE[dwHeight*dwWidth];
	
	for(j=0; j<dwHeight-dwRectHeight; j++)
	for(i=0; i<dwWidth-dwRectWidth; i++)
	{
		temp = 0;
		for(k=0; k<dwRectHeight; k++)
		for(l=0; l<dwRectWidth; l++)
		{
			if(!pBitmap[(j+k)*dwWidth+l+i])
			temp ++;
		}
		if(temp>upgate)   //大于给定上门限时
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]=0;
		else if(temp<downgate) //小于给定下门限时
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]=255; 
		else //二值化的需要
			for(k=0; k<dwRectHeight; k++)
			for(l=0; l<dwRectWidth; l++)
				pResult[(j+k)*dwWidth+l+i]= pResult[(j+k)*dwWidth+l+i]?255:0;
	}

	for(i=0;i<dwHeight; i++)
	for(j=0;j<dwWidth; j++)
		pBitmap[i*dwWidth+j] = pResult[i*dwWidth+j];

	delete pResult;
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  通过彩色进行目标的粗定位
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CoarsePositionByColorSegment(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, RECT &rect)
{
	// TODO: Add your command handler code here
	BYTE * pResult=NULL;
	BYTE* pTemp=NULL;
	DWORD position,i,j;
	DWORD dwRectWidth,dwRectHeight;

	if(!ColorBasedSegment(pBitmap,dwHeight,dwWidth,pResult))
		{   delete pResult;	return false;}
	if(!GetRidOfSmallBlock(pResult,dwHeight,dwWidth,255,600))
		{	delete pResult;	return false;}
	if(!ProjectAndFindTargetRegion(pResult,dwHeight,dwWidth,rect))
		{	delete pResult;	return false;}

	if(rect.right - rect.left == dwWidth-1 || rect.bottom - rect.top == dwHeight-1)
		{	delete pResult;	return false;}
	
	dwRectWidth = rect.right-rect.left;
	dwRectHeight = rect.bottom-rect.top;
	pTemp =(BYTE*) new BYTE[dwRectWidth*dwRectHeight];
	
	position=0;
	for(i=rect.top;i<rect.bottom;i++)
	for(j=rect.left;j<rect.right;j++)
	{
		pTemp[position] = pResult[i*dwWidth+j];
		position++;
	}
	RECT rect1;
	if(dwRectHeight>80)
	{
		ProjectAndFindTargetRegion(pTemp,80,dwRectWidth,rect1);
		rect.left = rect1.left + rect.left;
		rect.right = rect.left + rect1.right-rect1.left;
	//	rect.bottom = rect1.bottom + rect.top;
	}

	delete pTemp;
	delete pResult;
	
	if(rect.right-rect.left <20 || rect.bottom - rect.top< 20) 
		return false;
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pBitmap为灰度图象,flag为目标的灰度值
// 主要思想为：设置了一个迭代深度，将超过深度个像素点的的区域标志为一个小的数字，然后在以后的搜索终若遇
// 到某个区域和该区域相邻，则将两个区域合并之。考虑像“手”一样的情况，有可能和多个已知的区域合并。
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SelectBiggestBlock(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth,BYTE flag,DWORD dwMinBlockSize)
{
	DWORD temp = 0;
	BYTE colorarray[1000];  //用于合并的标志数组，从第1个(非第0个)开始有效
	WORD color_area = 0;    //标志已经有个多少个大区域
	DWORD i,j,k,temp_color,m,n;
	DWORD Number;
	WORD  Delete;
	BYTE egg;               //中间颜色值
	bool modify;            //修改标志
	
	for(i=0;i<260; i++)
		colorarray[i] = 0;
	
	DWORD position=0;
	DWORD position_l;
	for( i=0;i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pBitmap[position]==flag) //对每一个点都要检查是否是目标点
		{
			Number=0;    //设置个数为0
			egg = 250;   //设置中间颜色为250
			Delete = color_area+1; //
			//使用种子填充法找到改点的所属的整个区域：标志为egg
			SeedGrow(i,j,pBitmap,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
			modify = false;
			k=1;
			temp_color=0;
			//首先将刚刚找到的新区域修改为最开始找到的那个大区域编号
			for(k=1; (k<=color_area) && (!modify); k++) //一旦有改变就算完成任务:
			{  
				if( colorarray[k]==2 )
				{
					modify = true;
					temp_color = k;
					colorarray[k]=1;
					position_l=0;
					//扫描整个图像将标志为egg的改为k
					for(m=0; m<dwHeight; m++)
					for(n=0; n<dwWidth; n++)
					{
						if(pBitmap[position_l] == egg)
							pBitmap[position_l] = (BYTE) k;
						position_l++;
					}
				}
			}
			//寻找看看是否还和其它的大区域相邻：
			while((k<=color_area) && (colorarray[k]!=2))
				k++;
			//处理多区域杂交的情况
			if (k<=color_area)  
			{
				position_l=0;
				//扫描整个图像进行如下处理：
				for(m=0; m<dwHeight; m++)
				for(n=0; n<dwWidth; n++)
				{   
					k = temp_color;
					while ((k<=color_area)&&(pBitmap[position_l]))
					{
						if(colorarray[k] == 2)
						{
							if(pBitmap[position_l] == k)
								pBitmap[position_l] = (BYTE) temp_color;
						}
						k++;
					}
					position_l++;
				}
				k=temp_color; //将已经合并了的区域改掉，为0
				while (k<=color_area)
				{
					if( colorarray[k] ==2 ) 
						colorarray[k] = 0;
					k++;
				}
			}
			//如果不牵涉到合并，那么可以确定该区域是一个封闭的完整区域，
			//根据其面积大小是否大于给定的门限来确定是否删除之
			if((!modify)&&(Number<dwMinBlockSize))
			{ //若该区域大小小于给定的阈值，则将其删掉：
				DWORD position_l = 0;
				for(DWORD m=0; m<dwHeight; m++)
				for(DWORD n=0; n<dwWidth; n++)
				{
					if(pBitmap[position_l] == 250)
						pBitmap[position_l] = 0;
					position_l ++;
				}
			}
			//修改当前大区域为已有的区域编号:
			else if(!modify)
				{
					color_area++;  //总区域数加一
					colorarray[color_area] = 1; //相应的区域标志设为1
					position_l=0;  //修改大区域像素灰度为它的区域编号
					for(m=0; m<dwHeight; m++)
					for(n=0; n<dwWidth; n++)
					{
						if(pBitmap[position_l] == 250)
							pBitmap[position_l] = (BYTE) color_area;
						position_l ++;
					}
				}
		}
		position++;
	}


	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//在图像中截取一片区域（8位或24位）
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CutRegionFromPicture(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to, WORD flag)
{
	DWORD i,j;
	if(x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from>x_to || y_from>y_to)
		return false;
	
	if(pResult) delete pResult;
	pResult = (BYTE*) new BYTE[(x_to-x_from)*(y_to-y_from)*flag/8];

	DWORD position=0;
	if(flag==8)
	{
		for(i=y_from; i<y_to; i++)
		for(j=x_from; j<x_to; j++)
		{
			pResult[position] = pBitmap[i*dwWidth+j];
			position++;
		}
	}
	else
	if(flag==24)
	{
		for(i=y_from; i<y_to; i++)
		for(j=x_from; j<x_to; j++)
		{
			pResult[position] = pBitmap[(i*dwWidth+j)*3];
			position++;
			pResult[position] = pBitmap[(i*dwWidth+j)*3+1];
			position++;
			pResult[position] = pBitmap[(i*dwWidth+j)*3+2];
			position++;
		}
	}
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//在图像中截取一片区域
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CutRegionFromPicture(double *pBitmap, DWORD dwHeight, DWORD dwWidth, double *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to)
{
	DWORD i,j;
	if(x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from>x_to || y_from>y_to)
		return false;
	
	if(pResult) delete pResult;
	pResult = (double*) new double[(x_to-x_from)*(y_to-y_from)];

	DWORD position=0;

	for(i=y_from; i<y_to; i++)
	for(j=x_from; j<x_to; j++)
	{
		pResult[position] = pBitmap[i*dwWidth+j];
		position++;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  在矩阵中矩形:
//
//////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::DrawRectInColorImg(RECT rect,BYTE *pBitmap,DWORD dwHeight,DWORD dwWidth,RGBQUAD color)
{
	DrawLineInTheColorMatric(rect.left,  rect.top, rect.right, rect.top, pBitmap,dwHeight,dwWidth,color);
	DrawLineInTheColorMatric(rect.right, rect.top, rect.right, rect.bottom, pBitmap,dwHeight,dwWidth,color);
	DrawLineInTheColorMatric(rect.left, rect.bottom, rect.right, rect.bottom, pBitmap,dwHeight,dwWidth,color);
	DrawLineInTheColorMatric(rect.left,  rect.top, rect.left, rect.bottom, pBitmap,dwHeight,dwWidth,color);
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  将24位彩色图转换为灰度图:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Translate8To24Color(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j;
	DWORD temp;
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth*3];
	
	DWORD dwSize = dwHeight*dwWidth;
	for(i=0;i<dwSize;i++)
	{
		pResult[i*3+0] = pResult[i*3+1] = pResult[i*3+2] = pBitmap[i];
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  将24位彩色图转换为灰度图:
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Translate24To8Color(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j;
	DWORD temp;
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
	{
		temp = pBitmap[(i*dwWidth+j)*3]+pBitmap[(i*dwWidth+j)*3+1]+pBitmap[(i*dwWidth+j)*3+2];
		temp /= 3;
		pResult[i*dwWidth+j] = (BYTE) temp;
	}
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 利用两种方法一起来进行粗定位：
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CoarsePositionByColorAndVariance(BYTE **pBKImage, DWORD dwHeight, DWORD dwWidth, int nBKnumber,BYTE *pBitmap, WORD flag, RECT &rect)
{
    //计算通过均值和方差计算出来的rect
	if(!CoarsePositionByMeanAndVariance(pBKImage,dwHeight,dwWidth,nBKnumber,pBitmap,rect,flag))
		return false;

	//将rect内的内容抠出来：
	BYTE * pResult=NULL;
	CutRegionFromPicture(pBitmap,dwHeight,dwWidth,pResult,rect.left,rect.top,rect.right,rect.bottom,flag);

	//计算在扣出的图像中目标的rect1:
	RECT rect1;
	if(!CoarsePositionByColorSegment(pResult,rect.bottom-rect.top,rect.right-rect.left,rect1))
		return false;

	//修改原来的rect得到结果：
	rect.left += rect1.left;
	rect.right = rect.left + rect1.right - rect1.left;
	rect.top += rect1.top;
	rect.bottom = rect.top + rect1.bottom - rect1.top;

	rect.bottom += (rect.bottom - rect.top)/4;
	if(rect.bottom> dwHeight) rect.bottom = dwHeight;

	delete pResult;
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
// 得到色调空间：
//
/////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetHSIFromRGB(BYTE *pColorBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth*3];

	double r,g,b;
	double x;
	DWORD i,j;
	double S,H,I;

	DWORD position=0;
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{ 
		b = (double)(pColorBitmap[position])/255.0;
		g = (double)(pColorBitmap[position+1])/255.0;
		r = (double)(pColorBitmap[position+2])/255.0;
		
		I = (r+g+b)/3;
		x = r;
		if( x>g ) x = g;
		if( x>b ) x = b;
		if(I<0.000001)
			S = 0;
		else
			S = 1-x/I;
		if(S<0.000001)
			H = 0;
		else
		{
			x = (r-g+r-b)/(2*sqrt((r-g)*(r-g)+(r-b)*(g-b)));
			if(x>=1.0) 
				H = 0;
			else 
			  H = (acos(x)/3.1415926)*180; 
		}
		if(g<b) H=360-H;

		H = H*255;
		H /= 360;
		pResult[position] = (BYTE) H;
		pResult[position+1] = S*255;
		pResult[position+2] = I*255;

		position += 3;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//利用HSI空间中的S分量进行分割
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::VarianceByRGB2thColor(BYTE *pColorBitmap, DWORD dwHeight, DWORD dwWidth,BYTE* pBKImage, BYTE *&pResultImage)
{
   	DWORD i,j;
	int counter;

	if(pResultImage) delete pResultImage;
	pResultImage = new BYTE [dwHeight*dwWidth];

	for(i=0;i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{
		counter = (int)(pColorBitmap[(i*dwWidth+j)*3+1]) - (int)(pBKImage[(i*dwWidth+j)*3+1]);
		if(counter<0) counter = 0-counter;
		if(counter>255) counter = 255;
		pResultImage[i*dwWidth+j] = (BYTE) counter;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  利用OTSU算法进行图像分割:
//  输入图像和输出图像都是pBitmap,输出为2值图像
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SegmentByOTSU(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth)
{
	int i,j,nResultColor,i_from,i_to;
	DWORD dwSize = dwHeight*dwWidth;
	double pHg[256];
	double u,wk,uk,deltk;
	double fMax;
	

	for(i=0;i<256;i++)
		pHg[i] = 0;

	for(i=0;i<dwHeight; i++)
	for(j=0;j<dwWidth; j++)
        pHg[pBitmap[i*dwWidth+j]]++;

	u = 0;
	for(i=0;i<256; i++)
	{
		pHg[i] /= dwSize;
		u += i*pHg[i];
	}
	//目的是避免分母为0的情况:
	for(i=0;i<256 && !pHg[i];i++);
	i_from = i;
	for(i=255;i>=0 && !pHg[i];i--);
	i_to = i;

	i_from += 2;
	i_to -= 2;
	
	fMax = 0;wk=0; uk=0;
	nResultColor = 0;
	for(i=0;i<i_from;i++)
	{
		wk += pHg[i];
		uk += (double)i * pHg[i];
	}
	//寻找门限;
	for(i=i_from;i<i_to;i++)
	{
		wk += pHg[i];
		uk += (double)i * pHg[i];
		deltk = u*wk-uk;
		deltk *= deltk;
		deltk /= wk*(1-wk);
		if(fMax<deltk) 
		{
			fMax = deltk;
			nResultColor = i;
		}
	}
    //nResultColor -=40;
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
		pBitmap[i*dwWidth+j] = pBitmap[i*dwWidth+j]>nResultColor?255:0;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  利用OTSU算法进行图像分割:
//  输入图像和输出图像都是pBitmap,输出为2值图像
//  改进的OTSU算法，如果结果表现不出两类特性，则认为是一类目标
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SegmentByOTSU_M(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth)
{
	int i,nResultColor,i_from,i_to;
	DWORD dwSize = dwHeight*dwWidth;
	float pHg[256];
	float u,wk,uk,deltk;
	float fMax;
	
	memset(pHg,0,sizeof(float)*256);
	
	for(i=0;i<dwSize; i++)
		pHg[pBitmap[i]]++;
	
	u = 0;
	for(i=0;i<256; i++)
	{
		pHg[i] /= dwSize;
		u += i*pHg[i];
	}
	
	//目的是避免分母为0的情况:
	for(i=0;i<256 && !pHg[i];i++);
	i_from = i;
	for(i=255;i>=0 && !pHg[i];i--);
	i_to = i;
	
	i_from += 2;
	i_to -= 2;
	
	fMax = 0;wk=0; uk=0;
	nResultColor = 0;
	for(i=0;i<i_from;i++)
	{
		wk += pHg[i];
		uk += (float)i * pHg[i];
	}
	//寻找门限;
	for(i=i_from;i<i_to;i++)
	{
		wk += pHg[i];
		uk += (float)i * pHg[i];
		deltk = u*wk-uk;
		deltk *= deltk;
		deltk /= wk*(1-wk);
		if(fMax<deltk) 
		{
			fMax = deltk;
			nResultColor = i;
		}
	}
	
	if( fMax < 20 ) // 最好为20
		nResultColor = 255;
	
	
	for(i=0;i<dwSize;i++)
		pBitmap[i] = pBitmap[i]>nResultColor?255:0;
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
//利用下山法找谷点：
////////////////////////////////////////////////////////////////////////////////////////////////
int CZXDib::SegmentByDownHill(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth)
{
	int i,j,dwSize;
	dwSize = dwWidth*dwHeight;
	DWORD pTemp[256];

	for(i=0;i<256;i++)
		pTemp[i] = 0;

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
		pTemp[pBitmap[i*dwWidth+j]] ++;
	
//	for(i=0;i<13;i++)
		HistogramSmoothBy5Points(pTemp,256);

	//找到最大峰点：
	int max = 0;
	int k = 0;
	for(i=3;i<256;i++)
		if(max<pTemp[i])
		{
			max = pTemp[i];
			k = i;
		}

	for(i=0;i<256;i++)
	{
		pTemp[i] *= 255;
	    pTemp[i] /= max;
	}

	//找谷点： 
	int last,next;
	last = pTemp[k-1]+pTemp[k];
	next = pTemp[k+2]+pTemp[k+3];

	bool bStop = false;
    for(i=k;i<252 && !bStop;i++)
	{
		last += pTemp[i+1];
		next += pTemp[i+4];
		if(last - next <30)
		{
			bStop = true;
			k=i;
		}
		last -= pTemp[i-1];
		next -= pTemp[i+2];
	}
  
	//二值化：
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
		pBitmap[i*dwWidth+j] = pBitmap[i*dwWidth+j]>k?255:0;
	
	return k;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//利用给定的参数将图像进行归一化处理：
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::UnifyByMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float fMeanOutput, float fVarianceOutput,float fMeanInput=-1,float fVarianceInput=-1)
{
	DWORD i,dwSize;
	float temp;
	dwSize = dwHeight*dwWidth;
	
	//求均值：
	if(fMeanInput == -1)
	{
		fMeanInput = 0;
		for(i=0;i<dwSize;i++)
			fMeanInput += pBitmap[i];
		fMeanInput /= dwSize;
	}

	//求方差：
	if(fVarianceInput == -1)
	{
		fVarianceInput = 0;
		for(i=0;i<dwSize;i++)
		{
			temp = pBitmap[i];
			temp *= temp;
			fVarianceInput += temp;
		}
		fVarianceInput = sqrt(fVarianceInput/dwSize-fMeanInput*fMeanInput);
	}

	//规整化
	for(i=0;i<dwSize;i++)
	{
		temp = (pBitmap[i]-fMeanInput)*fVarianceOutput/fVarianceInput+fMeanOutput;
		if(temp<0) pBitmap[i] = 0;
		else
		if(temp>255) pBitmap[i] = 255;
		else
			pBitmap[i] = (BYTE) temp;
	}
	
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//利用给定的参数将图像进行归一化处理：
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::UnifyByMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float pMeanOutput[3], float pVarianceOutput[3],float pMeanInput[3],float pVarianceInput[3])
{
	DWORD i,dwSize;
	float temp;
	bool bDeleteMean = false;
	bool bDeleteVariance = false;
	dwSize = dwHeight*dwWidth;
	
	//求均值：
	if(!pMeanInput)
	{
		bDeleteMean = true;
		pMeanInput = new float[3];
		pMeanInput[0] = 
		pMeanInput[1] = 
		pMeanInput[2] = 0;
		for(i=0;i<dwSize;i++)
		{
			pMeanInput[0] += pBitmap[i*3];
			pMeanInput[1] += pBitmap[i*3+1];
			pMeanInput[2] += pBitmap[i*3+2];
		}
		pMeanInput[0] /= dwSize;
		pMeanInput[1] /= dwSize;
		pMeanInput[2] /= dwSize;
	}

	//求方差：
	if(!pVarianceInput)
	{
		bDeleteVariance = true;
		pVarianceInput = new float[3];
		pVarianceInput[0] = 
		pVarianceInput[1] =
		pVarianceInput[2] =0;
		for(i=0;i<dwSize;i++)
		{
			temp = pBitmap[i*3];
			temp *= temp;
			pVarianceInput[0] += temp;

			temp = pBitmap[i*3+1];
			temp *= temp;
			pVarianceInput[1] += temp;

			temp = pBitmap[i*3+2];
			temp *= temp;
			pVarianceInput[2] += temp;
		}
		pVarianceInput[0] = sqrt(pVarianceInput[0]/dwSize-pMeanInput[0]);
		pVarianceInput[1] = sqrt(pVarianceInput[1]/dwSize-pMeanInput[1]);
		pVarianceInput[2] = sqrt(pVarianceInput[2]/dwSize-pMeanInput[2]);
	}

	//规整化
	for(i=0;i<dwSize;i++)
	{
		temp = (pBitmap[i*3]-pMeanInput[0])*pVarianceOutput[0]/pVarianceInput[0]+pMeanOutput[0];
		if(temp<0) pBitmap[i*3] = 0;
		else
		if(temp>255) pBitmap[i*3] = 255;
		else
			pBitmap[i*3] = (BYTE) temp;
	
		temp = (pBitmap[i*3+1]-pMeanInput[1])*pVarianceOutput[1]/pVarianceInput[1]+pMeanOutput[1];
		if(temp<0) pBitmap[i*3+1] = 0;
		else
		if(temp>255) pBitmap[i*3+1] = 255;
		else
			pBitmap[i*3+1] = (BYTE) temp;

		temp = (pBitmap[i*3+2]-pMeanInput[2])*pVarianceOutput[2]/pVarianceInput[2]+pMeanOutput[2];
		if(temp<0) pBitmap[i*3+2] = 0;
		else
		if(temp>255) pBitmap[i*3+2] = 255;
		else
			pBitmap[i*3+2] = (BYTE) temp;
	}

	if(bDeleteMean) delete pMeanInput;
	if(bDeleteVariance)  delete pVarianceInput;
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//统计图像的均值和方差:
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::StatisticParametersAboutMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float &fMean, float &fVariance)
{
	DWORD i,dwSize;
	float temp;
	dwSize = dwHeight*dwWidth;
	
	//求均值：
	fMean = 0;
	for(i=0;i<dwSize;i++)
		fMean += pBitmap[i];
	fMean /= dwSize;

	//求方差：
	fVariance = 0;
	for(i=0;i<dwSize;i++)
	{
		temp = pBitmap[i];
		temp -= fMean;
		temp *= temp;
		fVariance += temp;
	}
	fVariance = sqrt(fVariance/dwSize);

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//利用给定的参数将图像进行归一化处理：
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::StatisticParametersAboutMeanAndVariance(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, float fMean[3], float fVariance[3])
{
	DWORD i,dwSize;
	float temp;
	dwSize = dwHeight*dwWidth;
	
	//求均值：
	fMean[0] = 
	fMean[1] = 
	fMean[2] = 0;
	for(i=0;i<dwSize;i++)
	{
		fMean[0] += pBitmap[i*3];
		fMean[1] += pBitmap[i*3+1];
		fMean[2] += pBitmap[i*3+2];
	}
	fMean[0] /= dwSize;
	fMean[1] /= dwSize;
	fMean[2] /= dwSize;

	//求方差：
	fVariance[0] = 
	fVariance[1] =
	fVariance[2] =0;
	for(i=0;i<dwSize;i++)
	{
		temp = pBitmap[i*3];
		temp -= fMean[0];
		temp *= temp;
		fVariance[0] += temp;

		temp = pBitmap[i*3+1];
		temp -= fMean[1];
		temp *= temp;
		fVariance[1] += temp;

		temp = pBitmap[i*3+2];
		temp -= fMean[2];
		temp *= temp;
		fVariance[2] += temp;
	}
	fVariance[0] = sqrt(fVariance[0]/dwSize);
	fVariance[1] = sqrt(fVariance[1]/dwSize);
	fVariance[2] = sqrt(fVariance[2]/dwSize);
	
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////
// 生成一个bmp文件头
////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CreateBITMAPFILEHEADER(DWORD dwHeight, DWORD dwWidth, BITMAPFILEHEADER *&pBmi,WORD flag)
{
	if(!pBmi) 
	pBmi =(BITMAPFILEHEADER*) new BYTE[sizeof(BITMAPFILEHEADER)]; 
	if(flag == 24) 
		pBmi->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)-4;
	else 
		pBmi->bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFO)-4+256*sizeof(RGBQUAD); 
	pBmi->bfReserved1 = 0;
	pBmi->bfReserved2 = 0;
	pBmi->bfType = 0x4d42;//'BM'
	DWORD dwWriteWidth = (dwWidth+3)/4*4;
	pBmi->bfSize =  dwHeight*dwWriteWidth + pBmi->bfOffBits;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//求两个向量的互相关:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CorrelationOfTwoVectors(BYTE *pBitmap1, BYTE *pBitmap2, DWORD dwHeight, DWORD dwWidth, float fFlag,float &fResult)
{
	DWORD i,j;
	DWORD dwSize = dwHeight*dwWidth;
	float temp;

	fResult = 0;
	for(i=0;i<dwSize;i++)
	{
		temp  = pBitmap1[i]-pBitmap2[i];
		if(temp<0) temp = 0-temp;
		if(temp>fFlag)
			fResult++;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//求Nagel相似度量:
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::NagelSimilarity(BYTE *pBitmap1, BYTE *pBitmap2, DWORD dwHeight, DWORD dwWidth, double *&pResult)
{
	DWORD Height,Width,dwSize;
	Height = dwHeight-1;
	Width = dwWidth-1;
	dwSize = dwHeight*dwWidth;
	int k,l;
	double variance1,variance2,mean1,mean2;
	double temp;

	if(pResult) delete pResult;
	pResult = (double*) new double [dwSize];

	DWORD i,j;
	for(i=1;i<Height;i++)
	for(j=1; j<Width;j++)
	{
		//3*3临域,均值和方差:
		mean1=0;
		for(k=-1;k<2;k++)
		for(l=-1;l<2;l++)
			mean1 += pBitmap1[(i+k)*dwWidth+j+l];
		mean1 /= 9;
		variance1=0;
		for(k=-1;k<2;k++)
		for(l=-1;l<2;l++)
			variance1+=((double)(pBitmap1[(i+k)*dwWidth+j+l]))*((double)(pBitmap1[(i+k)*dwWidth+j+l]));
		variance1=sqrt(variance1/9-mean1*mean1);

		mean2=0;
		for(k=-1;k<2;k++)
		for(l=-1;l<2;l++)
			mean2 += pBitmap2[(i+k)*dwWidth+j+l];
		mean2 /= 9;
		variance2=0;
		for(k=-1;k<2;k++)
		for(l=-1;l<2;l++)
			variance2+=((double)(pBitmap2[(i+k)*dwWidth+j+l]))*((double)(pBitmap2[(i+k)*dwWidth+j+l]));
		variance2=sqrt(variance2/9-mean2*mean2);

		temp = (variance1+variance2)/2+(mean1-mean2)*(mean1-mean2)/4;
		pResult[i*dwWidth+j] = (temp*temp/(variance1*variance2+1));
	}

	for(i=0;i<dwSize;i+=dwWidth)
	{
		pResult[i]=pResult[i+1];
	    pResult[i+dwWidth-1]=pResult[i+dwWidth-2];
	}
	for(j=0;j<dwWidth;j++)
	{
		pResult[j]=pResult[j+dwWidth];
		pResult[dwSize-j-1]=pResult[dwSize-j-1-dwWidth];
	}
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// 写文件
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::WriteDoubleMatricToFile(CString strFileName, DWORD dwHeight, DWORD dwWidth,double* pMatric)
{
	if(!pMatric)
		return false;
	//打开文件:
	CFile File;
	if(!File.Open(strFileName,CFile::modeCreate|CFile::modeWrite))
	{
		AfxMessageBox("打不开文件"+strFileName);
		return false;
	}
	File.Write(&dwHeight,sizeof(DWORD));
	File.Write(&dwWidth,sizeof(DWORD));
	File.Write(pMatric,dwHeight*dwWidth*sizeof(double));
	File.Close();
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//  从文件中装入浮点型数据    //
////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::LoadDoubleDataFromFile(CString strFileName, DWORD &dwHeight, DWORD &dwWidth, double *&pResult)
{
	CFile File;  
	if(!File.Open(strFileName,CFile::modeRead)){//|CFile::typeBinary
		//AfxMessageBox(FileName+" 打不开！LoadBitmap"+FileName);
		return false;
	}
	File.Read(&dwHeight,sizeof(DWORD));
	File.Read(&dwWidth,sizeof(DWORD));
	if(pResult) delete pResult;
	pResult = new double [dwHeight*dwWidth];
	File.Read(pResult,dwHeight*dwWidth*sizeof(double));
	File.Close();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//  SUSAN算子检测角点
///////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SUSANCornerDetect(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j,k,l;
	BYTE pTemplate[]={ 0,1,1,1,0,
			           1,1,1,1,1,
					   1,1,1,1,1,
					   1,1,1,1,1,
					   0,1,1,1,0};
	DWORD dwTemplateHeight = 5;
	DWORD dwTemplateWidth  = 5;
	int nThreshold = 30;
	BYTE counter;
	double temp1,temp2;

	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);


	for(i=dwTemplateHeight;i<dwHeight-dwTemplateHeight;i++)
	for(j=dwTemplateWidth; j<dwWidth-dwTemplateWidth;  j++)
	{
		temp1 = pBitmap[i*dwWidth+j];
		counter = 0;
		for(k=0;k<dwTemplateHeight;k++)
		for(l=0;l<dwTemplateWidth;l++)
		{
			temp2 = pBitmap[(i+k-dwTemplateHeight/2)*dwWidth+j+l-dwTemplateWidth/2];
			temp2 -= temp1;
			if(temp2<0) temp2 = 0 - temp2;
			
			if(pTemplate[k*dwTemplateWidth+l] && temp2 > nThreshold)
				counter ++;
		}
		pResult[i*dwWidth+j] = counter;
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  中值滤波采用３＊３模板
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::MedianFilter(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	int i,j,k,l;
	BYTE temp1,temp2,temp3,temp4,temp5,temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memcpy(pResult,pBitmap,sizeof(BYTE)*dwHeight*dwWidth);

	for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1; j++)
	{
		temp1 = temp2 = temp3 = temp4 = temp5 = 0;

		for(k=-1;k<2;k++)
		for(l=-1;l<2;l++)
		{
			temp = pBitmap[(i+k)*dwWidth+j+l];
			if(temp>temp1)
			{
				temp2 = temp1;
				temp3 = temp2;
				temp4 = temp3;
				temp5 = temp4;
				temp1 = temp;
			}else if(temp>temp2)
			{
				temp3 = temp2;
				temp4 = temp3;
				temp5 = temp4;
				temp2 = temp;
			}else if(temp>temp3)
			{
				temp4 = temp3;
				temp5 = temp4;
				temp3 = temp;
			}else if(temp>temp4)
			{
				temp5 = temp4;
				temp4 = temp;
			}else if(temp>temp5)
				temp5 = temp;
		}
		pResult[i*dwWidth+j] = temp5;
	}


	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  中值滤波采用1×5模板
////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::MedianFilterH(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, int nFilter, BYTE *&pResult)
{
	int i,j,m,n;
	BYTE bMax,temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memcpy(pResult,pBitmap,sizeof(BYTE)*dwHeight*dwWidth);

	BYTE pWnd[100]={0};
	int nSz2 = nFilter/2;
	nFilter = nSz2 * 2 + 1;
	
	for(i=0;   i<dwHeight;     i++)
	for(j=nSz2;j<dwWidth-nSz2; j++)
	{
		for( m=-nSz2; m<=nSz2; m++ )
			pWnd[m+nSz2] = pBitmap[i*dwWidth+j+m];

		for( m=0; m<nSz2; m++ )
		{
			bMax = pWnd[m];
			for( n=m+1; n<nFilter; n++ )
			{
				if( bMax > pWnd[n] ) continue;
				temp   = pWnd[n];
				pWnd[n] = bMax;
				bMax    = temp;				
		}	}
		pResult[i*dwWidth+j] = bMax;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////////
//   计算罗伯特差分
//////////////////////////////////////////////////////////////////////////////
bool CZXDib::Roberts(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j;
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);
	int temp;

	for(i=0;i<dwHeight-1;i++)
	for(j=0;j<dwWidth-1; j++)
	{
		temp = pBitmap[i*dwWidth+j] ;
		temp -= pBitmap[(i+1)*dwWidth+j+1];
		if(temp<0) temp = 0-temp;
/*
		if(pBitmap[(i+1)*dwWidth+j]<pBitmap[i*dwWidth+j+1])
			temp += pBitmap[i*dwWidth+j+1] - pBitmap[(i+1)*dwWidth+j];
		else 
			temp += pBitmap[(i+1)*dwWidth+j] - pBitmap[i*dwWidth+j+1];
*/
		if(temp>255) 
			temp = 255;
		pResult[i*dwWidth+j] = temp;

	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////
//   计算Laplacian
//////////////////////////////////////////////////////////////////////////////
bool CZXDib::Laplacian(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j,k,l;
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);

	int temp;

	for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1; j++)
	{
		temp = pBitmap[i*dwWidth+j];
		temp *= 8;
		temp = temp - pBitmap[(i+1)*dwWidth+j] - pBitmap[(i-1)*dwWidth+j] - pBitmap[i*dwWidth+j+1] - pBitmap[i*dwWidth+j-1];
		temp = temp - pBitmap[(i+1)*dwWidth+j+1] - pBitmap[(i-1)*dwWidth+j+1] - pBitmap[(i+1)*dwWidth+j-1] - pBitmap[(i-1)*dwWidth+j-1];
		
		if(temp<0) temp = 0-temp;
		if(temp>255) temp = 255;

		pResult[i*dwWidth+j] = temp;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////////
//   计算面积
//////////////////////////////////////////////////////////////////////////////
bool CZXDib::SegmentByArea(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwCount, BYTE *&pResult)
{
	DWORD i,dwSize;
	BYTE  gate;
	long  dCount;

	dCount = dwCount;
	dwSize = dwHeight * dwWidth;
	
	double *pHg = new double [256] ;
	memset(pHg,0,sizeof(double)*256);

	BYTE* pTemp = new BYTE [dwSize];

	for(i=0;i<dwSize;i++)
		pHg[pBitmap[i]]++;

	for(i=0;i<256;i++)
	{
		dCount -= pHg[i];
		if( dCount <= 0 )	break;
	}
	
	gate = min(i,255);

	for(i=0;i<dwSize;i++)
		pTemp[i] = (pBitmap[i]>=gate) * 255;

	delete pResult; 
	pResult = pTemp;

	if(pHg) delete pHg;

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  统计两个方向上的投影图
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ProjectXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag, BYTE *&pHg_X, BYTE *&pHg_Y)
{
	DWORD i,j;
	double x_max,y_max;

	double* p_x = new double[dwWidth];
	double* p_y = new double[dwHeight];
	memset(p_x,0,sizeof(double)*dwWidth);
	memset(p_y,0,sizeof(double)*dwHeight);

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
	{
		if(pBitmap[i*dwWidth+j]==flag)
		{
			(p_x[j])++;
			(p_y[i])++;
		}
	}


	if(pHg_X) delete pHg_X;
	pHg_X = new BYTE [dwWidth];
	
	if(pHg_Y) delete pHg_Y;
	pHg_Y = new BYTE [dwHeight];

	x_max = 0;
	for(i=0;i<dwWidth;i++)
		if(x_max < p_x[i]) x_max = p_x[i]; 
	y_max = 0;
	for(i=0;i<dwHeight;i++)
		if(y_max < p_y[i]) y_max = p_y[i];

	
	for(i=0;i<dwWidth;i++)
		pHg_X[i] = (int)(p_x[i]*255.0/x_max);

	for(i=0;i<dwHeight;i++)
		pHg_Y[i] = (int)(p_y[i]*255.0/y_max);


	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  统计两个方向上的投影图
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ProjectXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag, DWORD *&pHg_X, DWORD *&pHg_Y)
{
	DWORD i,j;

	if(pHg_X) delete pHg_X;
	pHg_X = new DWORD [dwWidth];
	memset(pHg_X,0,sizeof(DWORD)*dwWidth);
	
	if(pHg_Y) delete pHg_Y;
	pHg_Y = new DWORD [dwHeight];
	memset(pHg_Y,0,sizeof(DWORD)*dwHeight);

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
	{
		if(pBitmap[i*dwWidth+j]==flag)
		{
			(pHg_X[j])++;
			(pHg_Y[i])++;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  去掉和图像边缘相交的那些区域,留住中间被背景包围住的那些
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetRidOfOuterArea(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag_rid, BYTE flag_fill, BYTE *&pResult)
{
	DWORD i,j,k,dwSize;
	BYTE rid;

	dwSize = dwHeight*dwWidth;

	if(pResult) delete pResult;
	pResult = new BYTE [dwSize];
	memcpy(pResult,pBitmap,sizeof(BYTE)*dwSize);

	BYTE* pTemp = NULL;
	MarkAreas(pBitmap, dwHeight,dwWidth,flag_rid,pTemp);

	rid = true;
	for(i=0;i<dwHeight;i++)
	{
		if(pResult[i*dwWidth] == flag_rid )
		{
			rid = pTemp[i*dwWidth];
			for(j=0;j<dwSize;j++)
				if(pTemp[j] == rid) pResult[j] = flag_fill;
		}
		if(pResult[i*dwWidth+dwWidth-1] == flag_rid )
		{
			rid = pTemp[i*dwWidth];
			for(j=0;j<dwSize;j++)
				if(pTemp[j] == rid) pResult[j] = flag_fill;
		}
	}

	for(i=0;i<dwWidth;i++)
	{
		if(pResult[i] == flag_rid )
		{
			rid = pTemp[i];
			for(j=0;j<dwSize;j++)
				if(pTemp[j] == rid) pResult[j] = flag_fill;
		}
		if(pResult[dwSize-i-1] == flag_rid )
		{
			rid = pTemp[dwSize-i-1];
			for(j=0;j<dwSize;j++)
				if(pTemp[j] == rid) pResult[j] = flag_fill;
		}
	}

	if(pTemp) delete pTemp;

	return true;
}
////////////////////////////////////////////////////////////////////////////////////
//  标记不同的区域：
////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::MarkAreas(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE flag,BYTE *&pResult)
{
	if(pResult) delete pResult;
	pResult = new BYTE [dwHeight*dwWidth];
	memcpy(pResult,pBitmap,sizeof(BYTE)*dwHeight*dwWidth);

	BYTE colorarray[1000];  //用于合并的标志数组，从第1个(非第0个)开始有效
	WORD color_area = 0;    //标志已经有个多少个大区域
	DWORD i,j,Number;
	WORD  Delete;
	BYTE egg;               //中间颜色值
	
	for(i=0;i<260; i++)
		colorarray[i] = 0;
	
	egg = 1;
	for(i=0; i<dwHeight; i++)
	for(j=0; j<dwWidth; j++)
	{
		if(pResult[i*dwWidth+j]==flag) //对每一个点都要检查是否是目标点
		{
			Number=0;    //设置当前递归迭代层数为0
			Delete = color_area+1; //wang le 
			//使用种子填充法找到改点的所属的整个区域：标志为egg
			SeedGrow(i,j,pResult,dwHeight,dwWidth,Number,flag,egg,Delete,colorarray);
			egg ++;   //设置中间颜色为250
		}
	}

	return true;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   采用小窗口滑动，累加小窗口内灰度总和，整体转换至灰度图
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::SumGrayLevelOfSubArea(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwTemplateHeight,DWORD dwTemplateWidth,BYTE *&pResult)
{
	DWORD i,j,k,l,total,dwTemplateSize;
	dwTemplateSize = dwTemplateHeight*dwTemplateWidth;
	DWORD x_offset = dwTemplateHeight/2;
	DWORD y_offset = dwTemplateWidth/2;

	if(pResult) delete pResult;
	pResult = new BYTE [dwWidth*dwHeight];
	memset(pResult,0,sizeof(BYTE)*dwWidth*dwHeight);
	
	for(i=0;i<dwHeight-dwTemplateHeight;i++)
	for(j=0;j<dwWidth-dwTemplateWidth;j++)
	{
		total = 0;
		for(k=0;k<dwTemplateHeight;k++)
		for(l=0;l<dwTemplateWidth; l++)
			total += pBitmap[(i+k)*dwWidth+j+l];
		pResult[(i+x_offset)*dwWidth+j+y_offset] = (int)(total/dwTemplateSize);
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  将灰度图按照两度加权的形式，进行两个方向上的投影
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ProjectGrayImageXYHistogram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pHg_x,BYTE*& pHg_y)
{
/*	DWORD i,j;
	if(pHg_x) delete pHg_x;
	pHg_x = new BYTE [dwWidth];
	memset(pHg_x,0,sizeof(BYTE)*dwWidth);
	if(pHg_y) delete pHg_y;
	pHg_y = new BYTE [dwHeight];
	memset(pHg_y,0,sizeof(BYTE)*dwHeight);

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
	{
		pBitmpa[i*dwWidth_j];
	}

*/
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    盒子法图像分析
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::BoxesImageAnalysis(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwTemplateHeight, DWORD dwTemplateWidth, int nBoxHeight, int nBoxWidth, double threshold,BYTE *&pResult)
{
	int i,j,counter,k,l,m,n;

	BYTE* pTemp_local = new BYTE [dwTemplateWidth*dwTemplateHeight];
	BYTE* pTemp_bi = NULL;

	DWORD *pTemp_counter = new DWORD [dwHeight*dwWidth];

	memset(pTemp_counter,0,sizeof(DWORD)*dwHeight*dwWidth);
	bool fill;

	for(i=dwTemplateHeight/2; i<dwHeight-dwTemplateHeight/2; i++)
	for(j=dwTemplateWidth/2;  j<dwWidth-dwTemplateWidth/2;   j++)
	{
		for(k=0; k<dwTemplateHeight; k++)
		for(l=0;  l<dwTemplateWidth; l++)
			pTemp_local[k*dwTemplateWidth+l] = pBitmap[(i+k-dwTemplateHeight/2)*dwWidth+j+l-dwTemplateWidth/2];

        SegmentByArea(pTemp_local,dwTemplateHeight,dwTemplateWidth,dwTemplateHeight*dwTemplateWidth*(1-threshold),pTemp_bi);
		
		counter = 0;
		for(k=nBoxHeight/2; k<dwTemplateHeight-nBoxHeight/2; k++)
		for(l=nBoxWidth/2;  l<dwTemplateWidth-nBoxWidth/2;   l++)
		{
			fill = false;
			for(m=-nBoxHeight/2; m<nBoxHeight/2&& !fill; m++)
			for(n=-nBoxWidth/2;  n<nBoxWidth/2 && !fill; n++)
				fill = pTemp_bi[(k+m)*dwTemplateWidth+l+n]==255;
					
			if(!fill) counter ++;
		}
		pTemp_counter[i*dwWidth+j] = counter;
	}
     
	RegularToGrayLevel(pTemp_counter,dwHeight,dwWidth,pResult);
	if(pTemp_counter) delete pTemp_counter;
	if(pTemp_bi) delete pTemp_bi;
	if(pTemp_local) delete pTemp_local;

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//    盒子法图像分析
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::BoxesImageAnalysis(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD dwTemplateHeight, DWORD dwTemplateWidth, int nBoxHeight, int nBoxWidth, BYTE *&pResult)
{
	int i,j,counter,k,l,m,n;

	DWORD *pTemp_counter = new DWORD [dwHeight*dwWidth];
	memset(pTemp_counter,0,sizeof(DWORD)*dwHeight*dwWidth);
	bool fill;

	for(i=0; i<dwHeight-dwTemplateHeight; i++)
	for(j=0;  j<dwWidth-dwTemplateWidth;  j++)
	{
		counter = 0;
		for(k=nBoxHeight/2; k<dwTemplateHeight-nBoxHeight/2; k++)
		for(l=nBoxWidth/2;  l<dwTemplateWidth-nBoxWidth/2;   l++)
		{
			fill = false;
			for(m=-nBoxHeight/2; m<nBoxHeight/2&& !fill; m++)
			for(n=-nBoxWidth/2;  n<nBoxWidth/2 && !fill; n++)
			{
				fill = pBitmap[(i+k+m)*dwWidth+j+l+n]==255;
				if(i+k+m>dwHeight-1 || j+l+n>dwWidth)
				{
					AfxMessageBox("ASDF");	
					return false;
			}	}
			if(!fill) counter ++;
		}
		pTemp_counter[(i+dwTemplateHeight/2)*dwWidth+j+dwTemplateWidth/2] = counter;
	}
     
	RegularToGrayLevel(pTemp_counter,dwHeight,dwWidth,pResult);
	if(pTemp_counter) delete pTemp_counter;

	return true;
}
////////////////////////////////////////////////////////////////////////////////
//
//  测试用将某块区域以图像格式写到硬盘里
//
////////////////////////////////////////////////////////////////////////////////
bool CZXDib::Test(DWORD dwHeight, DWORD dwWidth, BYTE *pInput)
{
	  static CString str="e:\\000.bmp";
	  DWORD dwSize = dwHeight*dwWidth;

	  
	  WriteBMPFileFromVector(str,pInput,dwHeight,dwWidth,8);
	  FindNextFileName(str,1);

	  return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
//  分水岭算法
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ShedSegment(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE *&pResult)
{
	DWORD i,j,dwSize;
	dwSize = dwHeight*dwWidth;

	DWORD pHG[256];
	memset(pHG,0,sizeof(DWORD)*256);

	for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1;j++)
		pHG[pBitmap[i*dwWidth+j]]++;

	DWORD *pArea = new DWORD [dwSize];
	memset(pArea,0,sizeof(DWORD)*dwSize);


	if(pArea) delete pArea;
    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  添加高斯噪声
/////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::AddGaussNoise(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dMean, double dDeviation, BYTE *&pResult)
{
    int i,j,temp;

	dMean -= dDeviation/2;

	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];

	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth;j++)
	{
		temp = (rand()*dDeviation)/RAND_MAX + dMean + pBitmap[i*dwWidth+j];

		if(temp>255) temp = 255;
		else if(temp<0) temp = 0;

		pResult[i*dwWidth+j] =  temp;
	}

	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  添加高斯噪声
/////////////////////////////////////////////////////////////////////////////////////////////////////////
double CZXDib::AddGaussNoise_Fast(double dMean,double dDeviation)
{
	double dNoise;
	static double pSeeds[50]={
			 0.4073,-0.0760, 0.4161, 0.4280, 0.0304,-0.2893,-0.3752, 0.3514,-0.4054,-0.2663,
			 0.2862,-0.0536,-0.0994,-0.4350,-0.3399,-0.1791,-0.4880, 0.0116, 0.2226,-0.0135,
			-0.3895, 0.0574,-0.1197,-0.1780,-0.2882,-0.3286, 0.2864,-0.2684,-0.2939, 0.4238,
			-0.1755, 0.1664,-0.3704,-0.0114,-0.2533,-0.4233,-0.3219,-0.4518, 0.2494,-0.4401,
			-0.3782,-0.4867, 0.3282,-0.3127, 0.1074,-0.4741, 0.4290, 0.2461,-0.1603,-0.1571
	};
	int nNoise;
	static int nPosition1=0;
	static int nPosition2=49;
	
	static int pnSeeds[50];
	
	static int bFirst = 1;
	static double  pSin[101];
	int nSinNumber    = 101;
	static double pLeft[101];
	int nLeftNumber   = 101;
	
	int nScale = 512*(nLeftNumber-1);
	
	static double pU[160001];
	if(bFirst)
	{
		for(int k=0;k<50;k++)
			pnSeeds[k] = (int)(pSeeds[k]*nScale);  //   -256 --- 256;
		
		bFirst = 0;
		int i;
		for(i=0;i<nSinNumber-1;i++)
			pSin[i]=  sin(2*i*3.1415926/(double)((nSinNumber-1)));
		pSin[nSinNumber-1] = pSin[0];
		for(i=0;i<nLeftNumber-1;i++)
			pLeft[i] = sqrt(-2*log(((double)i)/(double)(nLeftNumber-1)));
		pLeft[nLeftNumber-1] = pLeft[0]=sqrt(-2*log((0.0001)/(double)(nLeftNumber-1)));
		
		for(i=0;i<nLeftNumber-1;i++)
			for(int j=0;j<nSinNumber-1;j++)
				pU[i*(nSinNumber-1)+j] = pLeft[i]*pSin[j];
	}
	
	
	//1
	dNoise = pnSeeds[nPosition1]+pnSeeds[nPosition2];
	if(dNoise<-nScale/2)		dNoise +=nScale;
	else if(dNoise>nScale/2)		dNoise -=nScale;
	pnSeeds[nPosition1] = (int)(dNoise);
	
	nPosition1++;	if(nPosition1==50)		nPosition1 = 0;
	nPosition2++;	if(nPosition2==50)		nPosition2 = 0;
	
	//2
	double dNoise2 = pnSeeds[nPosition1]+pnSeeds[nPosition2];
	if(dNoise2<-nScale/2)		dNoise2 +=nScale;
	else if(dNoise2>nScale/2)		dNoise2 -=nScale;
	pnSeeds[nPosition1] = (int)dNoise2;
	
	nPosition1++;	if(nPosition1==50)		nPosition1 = 0;
	nPosition2++;	if(nPosition2==50)		nPosition2 = 0;
	
	
	//sqrt(-2*log(pSeeds[nPosition_out]+0.5))*sin(2*3.1415926*(pSeeds[(nPosition_out+1)%50]+0.5));
	//dNoise = pLeft[(int)((dNoise2+0.5)*(nLeftNumber-1))]*pSin[(int)((dNoise+0.5)*(nSinNumber-1))];
	dNoise = pU[(int)((dNoise+nScale/2)/nScale*(nSinNumber-1))*(nLeftNumber-1)+(int)((dNoise2+nScale/2)/nScale*(nSinNumber-1))];
	dNoise = dNoise * dDeviation + dMean;
	
	
	
	return dNoise;
}
/*
根据所得插值点的个数，以及控制点坐标数组，确定中间插值点坐标数组(包括起始点)
*/
CPoint * CZXDib::Bezier4(CPoint *p, int num)
{
	double mu;
	double mum1,mum13,mu3;
	
    CPoint *point = new CPoint[4];
	CPoint *insertpoint = new CPoint[num+1];
	for(int i =0;i<4;i++)
	{
		point[i] = p[i];
	}
	
	//取0时为起点，取num时为终点，实际中间插值点数为num－1个
	int j; for( j = 0; j<= num; j++) 
	{ 
		mu = 1.0 /(double)num * (double)j;
        mum1 = 1 - mu;
        mum13 = mum1 * mum1 * mum1;
        mu3 = mu * mu * mu;
		
		insertpoint[j].x = mum13*point[0].x + 3*mu*mum1*mum1*point[1].x + 3*mu*mu*mum1*point[2].x + mu3*point[3].x;
		insertpoint[j].y = mum13*point[0].y + 3*mu*mum1*mum1*point[1].y + 3*mu*mu*mum1*point[2].y + mu3*point[3].y;
	}
	delete []point;
	return insertpoint;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  求解两个矩阵的乘积
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::AB(double *pA, double *pB, int nArow, int nAcol, int nBcol,double *pResult)
{
	if(!pA||!pB||!pResult) return false;
	
	int i,j,k;
	double temp;
	
	for(i=0;i<nArow;i++)
	for(j=0;j<nBcol;j++)
	{
		temp = 0;
		for(k=0;k<nAcol;k++)
			temp += pA[i*nAcol+k]*pB[k*nBcol+j];
		pResult[i*nAcol+j] = temp;
	}
	
	return true;
}
//////////////////////////////////////////////////////////////////////////////
//   矩阵转置乘以矩阵 fangzhen
//////////////////////////////////////////////////////////////////////////////
bool CZXDib::ATB(double *pA,double* pB, int raws, double *pResult)
{
	if( !pA || !pB || !pResult ) return false;
	
	double temp; //中间变量
	int i,j,k;
	
	for(i=0; i<raws; i++)
	for(j=0; j<raws; j++)
	{
		temp = 0;
		for(k=0;k<raws;k++)
			temp += pA[k*raws+i]*pB[k*raws+j];
		pResult[i*raws+j] = temp;
	}
	
	return true;
}
///////////////////////////////////////////////////////////////////
//
//  参数列表分别为：
//  图像向量矩阵(大小为图像面积*图像样本个数)，    特征向量矩阵(大小为图像面积*图像面积)，
//  右特征向量矩阵(图像样本个数*图像样本个数)，    每个图像样本大小  ,   图像向量个数(样本个数)
//
//    返回时，第一个参数对角线给出奇异值；第二个参数给出坐奇异值矩阵U；
//            第三个参数给出右奇异值向量矩阵；
///////////////////////////////////////////////////////////////////
bool CZXDib::SVD(double *pMultiImageMatric,
				 double* pEigenSpaceLeft,
				 double* pEigenSpaceRight,
				 long nPerVectorSize,
				 long nNumberOfVector)
{
   DWORD ka = nPerVectorSize>nNumberOfVector ? nPerVectorSize+1:nNumberOfVector+1;     
   double eps = 0.0001;

//接口
    long i,j,k,l,it,ll,kk,ix,iy,mm,nn,iz,m1,ks;
	int p_i,p_j,p_p,p_q;
    double p_d,s_r,s_d;
    double d,t,dd,sm,sm1,em1,sk,ek,b,c,shh,fg[2],cs[2];
    double *s,*e,*w;

    s =(double*) new double [ka];
    e =(double*) new double [ka];
    w =(double*) new double [ka];
	if( !(s && e && w))
	{
		AfxMessageBox("特征分解\n分配内存失败!");
		return FALSE;
	}
 
    it=60; k=nNumberOfVector;
    if (nPerVectorSize-1<nNumberOfVector) k=nPerVectorSize-1;
    l=nPerVectorSize;
    if (nNumberOfVector-2<nPerVectorSize) l=nNumberOfVector-2;
    if (l<0) l=0;
    ll=k;
    if (l>k) ll=l;
    if (ll>=1)
      { for (kk=1; kk<=ll; kk++)
          { if (kk<=k)
              { d=0.0;
                for (i=kk; i<=nPerVectorSize; i++)
                  { ix=(i-1)*nNumberOfVector+kk-1; d=d+pMultiImageMatric[ix]*pMultiImageMatric[ix];}
                s[kk-1]=sqrt(d);
                if (s[kk-1]!=0.0)
                  { ix=(kk-1)*nNumberOfVector+kk-1;
                    if (pMultiImageMatric[ix]!=0.0)
                      { s[kk-1]=fabs(s[kk-1]);
                        if (pMultiImageMatric[ix]<0.0) s[kk-1]=-s[kk-1];
                      }
                    for (i=kk; i<=nPerVectorSize; i++)
                      { iy=(i-1)*nNumberOfVector+kk-1;
                        pMultiImageMatric[iy]=pMultiImageMatric[iy]/s[kk-1];
                      }
                    pMultiImageMatric[ix]=1.0+pMultiImageMatric[ix];
                  }
                s[kk-1]=-s[kk-1];
              }
            if (nNumberOfVector>=kk+1)
              { for (j=kk+1; j<=nNumberOfVector; j++)
                  { if ((kk<=k)&&(s[kk-1]!=0.0))
                      { d=0.0;
                        for (i=kk; i<=nPerVectorSize; i++)
                          { ix=(i-1)*nNumberOfVector+kk-1;
                            iy=(i-1)*nNumberOfVector+j-1;
                            d=d+pMultiImageMatric[ix]*pMultiImageMatric[iy];
                          }
                        d=-d/pMultiImageMatric[(kk-1)*nNumberOfVector+kk-1];
                        for (i=kk; i<=nPerVectorSize; i++)
                          { ix=(i-1)*nNumberOfVector+j-1;
                            iy=(i-1)*nNumberOfVector+kk-1;
                            pMultiImageMatric[ix]=pMultiImageMatric[ix]+d*pMultiImageMatric[iy];
                          }
                      }
                    e[j-1]=pMultiImageMatric[(kk-1)*nNumberOfVector+j-1];
                  }
              }
            if (kk<=k)
              { for (i=kk; i<=nPerVectorSize; i++)
                  { ix=(i-1)*nPerVectorSize+kk-1; iy=(i-1)*nNumberOfVector+kk-1;
                    pEigenSpaceLeft[ix]=pMultiImageMatric[iy];
                  }
              }
            if (kk<=l)
              { d=0.0;
                for (i=kk+1; i<=nNumberOfVector; i++)
                  d=d+e[i-1]*e[i-1];
                e[kk-1]=sqrt(d);
                if (e[kk-1]!=0.0)
                  { if (e[kk]!=0.0)
                      { e[kk-1] = fabs(e[kk-1]);
                        if (e[kk]<0.0) e[kk-1]=-e[kk-1];
                      }
                    for (i=kk+1; i<=nNumberOfVector; i++)
                      e[i-1]=e[i-1]/e[kk-1];
                    e[kk]=1.0+e[kk];
                  }
                e[kk-1]=-e[kk-1];
                if ((kk+1<=nPerVectorSize)&&(e[kk-1]!=0.0))
                  { for (i=kk+1; i<=nPerVectorSize; i++) w[i-1]=0.0;
                    for (j=kk+1; j<=nNumberOfVector; j++)
                      for (i=kk+1; i<=nPerVectorSize; i++)
                        w[i-1]=w[i-1]+e[j-1]*pMultiImageMatric[(i-1)*nNumberOfVector+j-1];
                    for (j=kk+1; j<=nNumberOfVector; j++)
                      for (i=kk+1; i<=nPerVectorSize; i++)
                        { ix=(i-1)*nNumberOfVector+j-1;
                          pMultiImageMatric[ix]=pMultiImageMatric[ix]-w[i-1]*e[j-1]/e[kk];
                        }
                  }
                for (i=kk+1; i<=nNumberOfVector; i++)
                  pEigenSpaceRight[(i-1)*nNumberOfVector+kk-1]=e[i-1];
              }
          }
      }
    mm=nNumberOfVector;
    if (nPerVectorSize+1<nNumberOfVector) mm=nPerVectorSize+1;
    if (k<nNumberOfVector) s[k]=pMultiImageMatric[k*nNumberOfVector+k];
    if (nPerVectorSize<mm) s[mm-1]=0.0;
    if (l+1<mm) e[l]=pMultiImageMatric[l*nNumberOfVector+mm-1];
    e[mm-1]=0.0;
    nn=nPerVectorSize;
    if (nPerVectorSize>nNumberOfVector) nn=nNumberOfVector;
    if (nn>=k+1)
      { for (j=k+1; j<=nn; j++)
          { for (i=1; i<=nPerVectorSize; i++)
              pEigenSpaceLeft[(i-1)*nPerVectorSize+j-1]=0.0;
            pEigenSpaceLeft[(j-1)*nPerVectorSize+j-1]=1.0;
          }
      }
    if (k>=1)
      { for (ll=1; ll<=k; ll++)
          { kk=k-ll+1; iz=(kk-1)*nPerVectorSize+kk-1;
            if (s[kk-1]!=0.0)
              { if (nn>=kk+1)
                  for (j=kk+1; j<=nn; j++)
                    { d=0.0;
                      for (i=kk; i<=nPerVectorSize; i++)
                        { ix=(i-1)*nPerVectorSize+kk-1;
                          iy=(i-1)*nPerVectorSize+j-1;
                          d=d+pEigenSpaceLeft[ix]*pEigenSpaceLeft[iy]/pEigenSpaceLeft[iz];
                        }
                      d=-d;
                      for (i=kk; i<=nPerVectorSize; i++)
                        { ix=(i-1)*nPerVectorSize+j-1;
                          iy=(i-1)*nPerVectorSize+kk-1;
                          pEigenSpaceLeft[ix]=pEigenSpaceLeft[ix]+d*pEigenSpaceLeft[iy];
                        }
                    }
                  for (i=kk; i<=nPerVectorSize; i++)
                    { ix=(i-1)*nPerVectorSize+kk-1; pEigenSpaceLeft[ix]=-pEigenSpaceLeft[ix];}
                  pEigenSpaceLeft[iz]=1.0+pEigenSpaceLeft[iz];
                  if (kk-1>=1)
                    for (i=1; i<=kk-1; i++)
                      pEigenSpaceLeft[(i-1)*nPerVectorSize+kk-1]=0.0;
              }
            else
              { for (i=1; i<=nPerVectorSize; i++)
                  pEigenSpaceLeft[(i-1)*nPerVectorSize+kk-1]=0.0;
                pEigenSpaceLeft[(kk-1)*nPerVectorSize+kk-1]=1.0;
              }
          }
      }
    for (ll=1; ll<=nNumberOfVector; ll++)
      { kk=nNumberOfVector-ll+1; iz=kk*nNumberOfVector+kk-1;
        if ((kk<=l)&&(e[kk-1]!=0.0))
          { for (j=kk+1; j<=nNumberOfVector; j++)
              { d=0.0;
                for (i=kk+1; i<=nNumberOfVector; i++)
                  { ix=(i-1)*nNumberOfVector+kk-1; iy=(i-1)*nNumberOfVector+j-1;
                    d=d+pEigenSpaceRight[ix]*pEigenSpaceRight[iy]/pEigenSpaceRight[iz];
                  }
                d=-d;
                for (i=kk+1; i<=nNumberOfVector; i++)
                  { ix=(i-1)*nNumberOfVector+j-1; iy=(i-1)*nNumberOfVector+kk-1;
                    pEigenSpaceRight[ix]=pEigenSpaceRight[ix]+d*pEigenSpaceRight[iy];
                  }
              }
          }
        for (i=1; i<=nNumberOfVector; i++)
          pEigenSpaceRight[(i-1)*nNumberOfVector+kk-1]=0.0;
        pEigenSpaceRight[iz-nNumberOfVector]=1.0;
      }
    for (i=1; i<=nPerVectorSize; i++)
    for (j=1; j<=nNumberOfVector; j++)
      pMultiImageMatric[(i-1)*nNumberOfVector+j-1]=0.0;
    m1=mm; it=60;
    while (1==1)
      { if (mm==0)
          { //ppp(pMultiImageMatric,e,s,v,m,nNumberOfVector);
		if (nPerVectorSize>=nNumberOfVector) p_i=nNumberOfVector;
		else p_i=nPerVectorSize;
		for (p_j=1; p_j<=p_i-1; p_j++)
		{ pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j-1]=s[p_j-1];
        pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j]=e[p_j-1];
		}
		pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i-1]=s[p_i-1];
		if (nPerVectorSize<nNumberOfVector) pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i]=e[p_i-1];
		for (p_i=1; p_i<=nNumberOfVector-1; p_i++)
			for (p_j=p_i+1; p_j<=nNumberOfVector; p_j++)
			{ p_p=(p_i-1)*nNumberOfVector+p_j-1; p_q=(p_j-1)*nNumberOfVector+p_i-1;
			p_d=pEigenSpaceRight[p_p]; pEigenSpaceRight[p_p]=pEigenSpaceRight[p_q]; pEigenSpaceRight[p_q]=p_d;
			}
	////////////////////////////////////////
            //free(s); free(e); free(w); 
			 goto end;
          }
        if (it==0)
          { //ppp(pMultiImageMatric,e,s,v,nPerVectorSize,nNumberOfVector);
			if (nPerVectorSize>=nNumberOfVector) p_i=nNumberOfVector;
			else p_i=nPerVectorSize;
			for (p_j=1; p_j<=p_i-1; p_j++)
			{ pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j-1]=s[p_j-1];
			pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j]=e[p_j-1];
			}
			pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i-1]=s[p_i-1];
			if (nPerVectorSize<nNumberOfVector) pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i]=e[p_i-1];
			for (p_i=1; p_i<=nNumberOfVector-1; p_i++)
				for (p_j=p_i+1; p_j<=nNumberOfVector; p_j++)
				{ p_p=(p_i-1)*nNumberOfVector+p_j-1; p_q=(p_j-1)*nNumberOfVector+p_i-1;
				p_d=pEigenSpaceRight[p_p]; pEigenSpaceRight[p_p]=pEigenSpaceRight[p_q]; pEigenSpaceRight[p_q]=p_d;
				}
			///////////////////////////////////////
			if (nPerVectorSize>=nNumberOfVector) p_i=nNumberOfVector;
			else p_i=nPerVectorSize;
			for (p_j=1; p_j<=p_i-1; p_j++)
			{ pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j-1]=s[p_j-1];
			pMultiImageMatric[(p_j-1)*nNumberOfVector+p_j]=e[p_j-1];
			}
			pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i-1]=s[p_i-1];
			if (nPerVectorSize<nNumberOfVector) pMultiImageMatric[(p_i-1)*nNumberOfVector+p_i]=e[p_i-1];
			for (p_i=1; p_i<=nNumberOfVector-1; p_i++)
				for (p_j=p_i+1; p_j<=nNumberOfVector; p_j++)
				{ p_p=(p_i-1)*nNumberOfVector+p_j-1; p_q=(p_j-1)*nNumberOfVector+p_i-1;
				p_d=pEigenSpaceRight[p_p]; pEigenSpaceRight[p_p]=pEigenSpaceRight[p_q]; pEigenSpaceRight[p_q]=p_d;
				}  ////////////////////////

            //free(s); free(e); free(w);
			goto enderror;
			 }
        kk=mm-1;
	while ((kk!=0)&&(fabs(e[kk-1])!=0.0))
          { d=fabs(s[kk-1])+fabs(s[kk]);
            dd=fabs(e[kk-1]);
            if (dd>eps*d) kk=kk-1;
            else e[kk-1]=0.0;
          }
        if (kk==mm-1)
          { kk=kk+1;
            if (s[kk-1]<0.0)
              { s[kk-1]=-s[kk-1];
                for (i=1; i<=nNumberOfVector; i++)
                  { ix=(i-1)*nNumberOfVector+kk-1; pEigenSpaceRight[ix]=-pEigenSpaceRight[ix];}
              }
            while ((kk!=m1)&&(s[kk-1]<s[kk]))
              { d=s[kk-1]; s[kk-1]=s[kk]; s[kk]=d;
                if (kk<nNumberOfVector)
                  for (i=1; i<=nNumberOfVector; i++)
                    { ix=(i-1)*nNumberOfVector+kk-1; iy=(i-1)*nNumberOfVector+kk;
                      d=pEigenSpaceRight[ix]; pEigenSpaceRight[ix]=pEigenSpaceRight[iy]; pEigenSpaceRight[iy]=d;
                    }
                if (kk<nPerVectorSize)
                  for (i=1; i<=nPerVectorSize; i++)
                    { ix=(i-1)*nPerVectorSize+kk-1; iy=(i-1)*nPerVectorSize+kk;
                      d=pEigenSpaceLeft[ix]; pEigenSpaceLeft[ix]=pEigenSpaceLeft[iy]; pEigenSpaceLeft[iy]=d;
                    }
                kk=kk+1;
              }
            it=60;
            mm=mm-1;
          }
        else
          { ks=mm;
            while ((ks>kk)&&(fabs(s[ks-1])!=0.0))
              { d=0.0;
                if (ks!=mm) d=d+fabs(e[ks-1]);
                if (ks!=kk+1) d=d+fabs(e[ks-2]);
                dd=fabs(s[ks-1]);
                if (dd>eps*d) ks=ks-1;
                else s[ks-1]=0.0;
              }
            if (ks==kk)
              { kk=kk+1;
                d=fabs(s[mm-1]);
                t=fabs(s[mm-2]);
                if (t>d) d=t;
                t=fabs(e[mm-2]);
                if (t>d) d=t;
                t=fabs(s[kk-1]);
                if (t>d) d=t;
                t=fabs(e[kk-1]);
                if (t>d) d=t;
                sm=s[mm-1]/d; sm1=s[mm-2]/d;
                em1=e[mm-2]/d;
                sk=s[kk-1]/d; ek=e[kk-1]/d;
                b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
                c=sm*em1; c=c*c; shh=0.0;
                if ((b!=0.0)||(c!=0.0))
                  { shh=sqrt(b*b+c);
                    if (b<0.0) shh=-shh;
                    shh=c/(b+shh);
                  }
                fg[0]=(sk+sm)*(sk-sm)-shh;
                fg[1]=sk*ek;
                for (i=kk; i<=mm-1; i++)
                  { //sss(fg,cs);
					if ((fabs(fg[0])+fabs(fg[1]))==0.0)
					{ cs[0]=1.0; cs[1]=0.0; s_d=0.0;}
					else 
					{ s_d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
					if (fabs(fg[0])>fabs(fg[1]))
					{ s_d=fabs(s_d);
					if (fg[0]<0.0) s_d=-s_d;
					}
					if (fabs(fg[1])>=fabs(fg[0]))
					{ s_d=fabs(s_d);
					if (fg[1]<0.0) s_d=-s_d;
					}
					cs[0]=fg[0]/s_d; cs[1]=fg[1]/s_d;
					}
					s_r=1.0;
					if (fabs(fg[0])>fabs(fg[1])) s_r=cs[1];
					else
						if (cs[0]!=0.0) s_r=1.0/cs[0];
						fg[0]=s_d; fg[1]=s_r;
				/////////////////////////////////

                    if (i!=kk) e[i-2]=fg[0];
                    fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
                    e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
                    fg[1]=cs[1]*s[i];
                    s[i]=cs[0]*s[i];
                    if ((cs[0]!=1.0)||(cs[1]!=0.0))
                      for (j=1; j<=nNumberOfVector; j++)
                        { ix=(j-1)*nNumberOfVector+i-1;
                          iy=(j-1)*nNumberOfVector+i;
                          d=cs[0]*pEigenSpaceRight[ix]+cs[1]*pEigenSpaceRight[iy];
                          pEigenSpaceRight[iy]=-cs[1]*pEigenSpaceRight[ix]+cs[0]*pEigenSpaceRight[iy];
                          pEigenSpaceRight[ix]=d;
                        }
                    //sss(fg,cs);
					  if ((fabs(fg[0])+fabs(fg[1]))==0.0)
					  { cs[0]=1.0; cs[1]=0.0; s_d=0.0;}
					  else 
					  { s_d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
					  if (fabs(fg[0])>fabs(fg[1]))
					  { s_d=fabs(s_d);
					  if (fg[0]<0.0) s_d=-s_d;
					  }
					  if (fabs(fg[1])>=fabs(fg[0]))
					  { s_d=fabs(s_d);
					  if (fg[1]<0.0) s_d=-s_d;
					  }
					  cs[0]=fg[0]/s_d; cs[1]=fg[1]/s_d;
					  }
					  s_r=1.0;
					  if (fabs(fg[0])>fabs(fg[1])) s_r=cs[1];
					  else
						  if (cs[0]!=0.0) s_r=1.0/cs[0];
						  fg[0]=s_d; fg[1]=s_r;
				//////////////////////////////////////
                    s[i-1]=fg[0];
                    fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
                    s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
                    fg[1]=cs[1]*e[i];
                    e[i]=cs[0]*e[i];
                    if (i<nPerVectorSize)
                      if ((cs[0]!=1.0)||(cs[1]!=0.0))
                        for (j=1; j<=nPerVectorSize; j++)
                          { ix=(j-1)*nPerVectorSize+i-1;
                            iy=(j-1)*nPerVectorSize+i;
                            d=cs[0]*pEigenSpaceLeft[ix]+cs[1]*pEigenSpaceLeft[iy];
                            pEigenSpaceLeft[iy]=-cs[1]*pEigenSpaceLeft[ix]+cs[0]*pEigenSpaceLeft[iy];
                            pEigenSpaceLeft[ix]=d;
                          }
                  }
                e[mm-2]=fg[0];
                it=it-1;
              }
            else
              { if (ks==mm)
                  { kk=kk+1;
                    fg[1]=e[mm-2]; e[mm-2]=0.0;
                    for (ll=kk; ll<=mm-1; ll++)
                      { i=mm+kk-ll-1;
                        fg[0]=s[i-1];
                        //sss(fg,cs);
						if ((fabs(fg[0])+fabs(fg[1]))==0.0)
						{ cs[0]=1.0; cs[1]=0.0; s_d=0.0;}
						else 
						{ s_d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
						if (fabs(fg[0])>fabs(fg[1]))
						{ s_d=fabs(s_d);
						if (fg[0]<0.0) s_d=-s_d;
						}
						if (fabs(fg[1])>=fabs(fg[0]))
						{ s_d=fabs(s_d);
						if (fg[1]<0.0) s_d=-s_d;
						}
						cs[0]=fg[0]/s_d; cs[1]=fg[1]/s_d;
						}
						s_r=1.0;
						if (fabs(fg[0])>fabs(fg[1])) s_r=cs[1];
						else
							if (cs[0]!=0.0) s_r=1.0/cs[0];
							fg[0]=s_d; fg[1]=s_r;
					///////////////////////////////////////////

                        s[i-1]=fg[0];
                        if (i!=kk)
                          { fg[1]=-cs[1]*e[i-2];
                            e[i-2]=cs[0]*e[i-2];
                          }
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))
                          for (j=1; j<=nNumberOfVector; j++)
                            { ix=(j-1)*nNumberOfVector+i-1;
                              iy=(j-1)*nNumberOfVector+mm-1;
                              d=cs[0]*pEigenSpaceRight[ix]+cs[1]*pEigenSpaceRight[iy];
                              pEigenSpaceRight[iy]=-cs[1]*pEigenSpaceRight[ix]+cs[0]*pEigenSpaceRight[iy];
                              pEigenSpaceRight[ix]=d;
                            }
                      }
                  }
                else
                  { kk=ks+1;
                    fg[1]=e[kk-2];
                    e[kk-2]=0.0;
                    for (i=kk; i<=mm; i++)
                      { fg[0]=s[i-1];
                       // sss(fg,cs);
					if ((fabs(fg[0])+fabs(fg[1]))==0.0)
					{ cs[0]=1.0; cs[1]=0.0; s_d=0.0;}
					else 
					{ s_d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
					if (fabs(fg[0])>fabs(fg[1]))
					{ s_d=fabs(s_d);
					if (fg[0]<0.0) s_d=-s_d;
					}
					if (fabs(fg[1])>=fabs(fg[0]))
					{ s_d=fabs(s_d);
					if (fg[1]<0.0) s_d=-s_d;
					}
					cs[0]=fg[0]/s_d; cs[1]=fg[1]/s_d;
					}
					s_r=1.0;
					if (fabs(fg[0])>fabs(fg[1])) s_r=cs[1];
					else
						if (cs[0]!=0.0) s_r=1.0/cs[0];
						fg[0]=s_d; fg[1]=s_r;
	     //////////////////////////////////////////

                        s[i-1]=fg[0];
                        fg[1]=-cs[1]*e[i-1];
                        e[i-1]=cs[0]*e[i-1];
                        if ((cs[0]!=1.0)||(cs[1]!=0.0))
                          for (j=1; j<=nPerVectorSize; j++)
                            { ix=(j-1)*nPerVectorSize+i-1;
                              iy=(j-1)*nPerVectorSize+kk-2;
                              d=cs[0]*pEigenSpaceLeft[ix]+cs[1]*pEigenSpaceLeft[iy];
                              pEigenSpaceLeft[iy]=-cs[1]*pEigenSpaceLeft[ix]+cs[0]*pEigenSpaceLeft[iy];
                              pEigenSpaceLeft[ix]=d;
                            }
                      }
                  }
              }
          }
      }
end:
	  delete s;
      delete e;
      delete w;
	  return TRUE;
enderror:
	  delete s;
      delete e;
      delete w;
      return FALSE;
}

// pData按列存放一个样本，样本数目nSampleCnt, 样本维数nDim
void CZXDib::PCA(double* pdData, int nSampleCnt, int nDim, double*& pdU, double*& pdSig)
{
	int i,j,m;
	double* pdMean = new double[nDim];
	
	for( j=0; j<nDim; j++ )
	{
		pdMean[j] = 0;
		for( i=0; i<nSampleCnt; i++ )
			pdMean[j] += pdData[j*nSampleCnt+i];
		
		pdMean[j] /= nSampleCnt;
	}

	double* pdCov = new double[nDim * nDim];
	for( i=0; i<nDim; i++ )
	for( j=0; j<nDim; j++ )
	{
		double dTemp = 0;
		for( m=0; m<nSampleCnt; m++ )
			dTemp += (pdData[i*nSampleCnt+m] - pdMean[i])*(pdData[j*nSampleCnt+m] - pdMean[j]);
		pdCov[i*nDim+j] = dTemp / ( nSampleCnt - 1 );
	}

	delete pdU;
	pdU = new double[nDim*nDim];
	
	delete pdSig;
	pdSig = new double[nDim*nDim];

	SVD(pdCov,pdU,pdSig,nDim,nDim);

	delete pdSig;
	pdSig = pdCov;

	delete pdMean;
}

void CZXDib::FussionPCA(DWORD nHeight,DWORD nWidth,BYTE *lpPanBand,
					 BYTE *lpMultiBand,BYTE* pResult)
{
	long i,j,k,nSize;
	nSize = nHeight * nWidth;

	// 先获取均值像素值
	double dMeanR,dMeanG,dMeanB;
	dMeanR = dMeanG = dMeanB = 0;

	for( i=0; i<nSize; i++ )
	{
		dMeanR += lpMultiBand[i*3+2];
		dMeanG += lpMultiBand[i*3+1];
		dMeanB += lpMultiBand[i*3+0];
	}
	dMeanR /= nSize;
	dMeanG /= nSize;
	dMeanB /= nSize;


	// 构造协方差矩阵
	double dCovX[9];
	double dCovX0,dCovX1,dCovX2,dCovX4,dCovX5,dCovX8;
	dCovX0 = dCovX1 = dCovX2 = dCovX4 = dCovX5 = dCovX8 = 0;
//	| RR RG RB |
//	| GR GG GR |
//	| BR BG BB |
	
	memset(dCovX,0,sizeof(double)*9);

	for( i=0; i<nSize; i++ )
	{
		dCovX0 += ( lpMultiBand[i*3+2] - dMeanR ) * ( lpMultiBand[i*3+2] - dMeanR ); // R * R
		dCovX1 += ( lpMultiBand[i*3+2] - dMeanR ) * ( lpMultiBand[i*3+1] - dMeanG ); // R * G
		dCovX2 += ( lpMultiBand[i*3+2] - dMeanR ) * ( lpMultiBand[i*3+0] - dMeanB ); // R * B

		dCovX4 += ( lpMultiBand[i*3+1] - dMeanG ) * ( lpMultiBand[i*3+1] - dMeanG ); // G * G
		dCovX5 += ( lpMultiBand[i*3+1] - dMeanG ) * ( lpMultiBand[i*3+2] - dMeanR ); // G * R

		dCovX8 += ( lpMultiBand[i*3+0] - dMeanB ) * ( lpMultiBand[i*3+0] - dMeanB ); // G * G		
	}

	dCovX[0] = dCovX0 / nSize;
	dCovX[1] = dCovX[3] = dCovX1 / nSize;
	dCovX[2] = dCovX[6] = dCovX2 / nSize;
	dCovX[4] = dCovX4 / nSize;
	dCovX[5] = dCovX[7] = dCovX5 / nSize;
	dCovX[8] = dCovX8 / nSize;


	// 奇异值分解
	double pUI[9],pVO[9];
	SVD(dCovX,pUI,pVO,3,3);

	bool   bReverse = pUI[0] + pUI[3] + pUI[6] < 0 ;	
	double r,g,b;
	double dMean,dDeviation,dMean0,dDeviation0;
	
	dMean = dDeviation = dMean0 = dDeviation0 = 0;

	double *pdTemp = NULL;
	pdTemp = new double[nSize*3];


	// 映射到特征空间
	for( i=0; i<nSize; i++ )
	{
		r = lpMultiBand[i*3+2];
		g = lpMultiBand[i*3+1];
		b = lpMultiBand[i*3+0];

		r-= dMeanR;
		g-= dMeanG;
		b-= dMeanB;

		pdTemp[i*3+2] = r * pUI[0] + g * pUI[3] + b * pUI[6];
		pdTemp[i*3+1] = r * pUI[1] + g * pUI[4] + b * pUI[7];
		pdTemp[i*3+0] = r * pUI[2] + g * pUI[5] + b * pUI[8];

		dMean += pdTemp[i*3+2];
		dDeviation += pdTemp[i*3+2] * pdTemp[i*3+2];

		r = lpPanBand[i];
		if( bReverse ) 
			r = 0 - r;
		dMean0 += r;
		dDeviation0 += r * r;
	}


	// 计算均值方差
	dMean /= nSize;
	dDeviation = sqrt(dDeviation/nSize - dMean*dMean);

	dMean0 /= nSize;
	dDeviation0 = sqrt(dDeviation0/nSize - dMean0*dMean0);

	double dTemp = 1.0 / dDeviation0 * dDeviation; // 简化运算设置中间辅助变量
	double dTemp1= dMean - dMean0 * dTemp;

	// 替换第一主分量
	if( bReverse ) // 如果图像反转，则需要将高分辨率图像求反再替换
		dTemp = -dTemp;
	
	//for( i=0; i<nSize; i++ )
	//	pdTemp[i*3+2] = dTemp1 + lpPanBand[i]*dTemp;


	// 映射回到颜色空间
	for( i=0; i<nSize; i++ )
	{
		r = dTemp1 + lpPanBand[i]*dTemp; //pdTemp[i*3+2];
		g = pdTemp[i*3+1];
		b = pdTemp[i*3+0];
		
		pdTemp[i*3+2] = r* pUI[0] + g* pUI[1] + b* pUI[2] + dMeanR;
		pdTemp[i*3+1] = r* pUI[3] + g* pUI[4] + b* pUI[5] + dMeanG;
		pdTemp[i*3+0] = r* pUI[6] + g* pUI[7] + b* pUI[8] + dMeanB;
	}
	

	// 拷贝结果数据
	for( i=nSize*3-1; i>=0; i-- )
	{
		if( pdTemp[i] < 0 )			pResult[i] = 0;
		else if( pdTemp[i] < 255 ) 	pResult[i] = pdTemp[i];
		else						pResult[i] = 255;
	}


	// 清理中间使用的内存：
	if( pdTemp )
	{
		delete pdTemp;
		pdTemp = NULL;
	}
	
	return;
}
//BOOL CZXDib::Match(StGeoImageBuf& SrcBuf,StGeoImageBuf& DestBuf,double& x,double& y,double& maxr)
BOOL CZXDib::Match(BYTE*pBuf,	  long nHeight,    long nWidth,
				   BYTE* pDestBuf,long nDestHeight,long nDestWidth,
				   double& x,double& y,double& maxr)
{
	long nBitCount;//ight,nWidth;
	//nHeight = SrcBuf.height;
	//nWidth  = SrcBuf.width;
	nBitCount = 8;//SrcBuf.nBitCount;
	long nDestBitCount;//nDestHeight,nDestWidth,;
	//nDestHeight = DestBuf.Height;
	//nDestWidth  = DestBuf.width;
	nDestBitCount = 8;//DestBuf.nBitCount;
	//BYTE* pBuf,*pDestBuf;
	//pDestBuf = DestBuf.buf;
	//pBuf = SrcBuf.buf;
	
    long	os;
    unsigned long	s,ss,o,oo;
    BYTE	*Obuf,*Sbuf,*limit;
    double	num=nHeight*nWidth;
    double	r;
    int		i,j,k,sumi,sumj,ri,rj,nSamplesPerPixel;
	
	if(nBitCount != nDestBitCount)
		return FALSE;
	if(nBitCount != 8 && nBitCount != 24)
		return FALSE;
	
	nSamplesPerPixel=nBitCount/8;
	
	
	// oo：模版中所有像素值平方和
	// o：模版中所有像素值和
	maxr=0;
	oo=o=0;
	for(k=0; k < nHeight; k++)
	{
        Obuf = pBuf + k*nWidth;//Obuf=pBuf[k];
        limit= Obuf + nWidth;
		for(;Obuf < limit;)
		{
			oo+=(long)*Obuf**Obuf;
            o+=*Obuf++;
		}
	}
    sumi = nDestHeight - nHeight;
    sumj = nDestWidth  - nWidth+1;
    for(j=0; j < sumj; j++)
	{
        s=os=ss=0;
        for(k=0; k < nHeight; k++)
		{
            Obuf = pBuf     + k*nWidth;		  // Obuf = pBuf[k];
            Sbuf = pDestBuf + k*nDestWidth +j;// Sbuf = pDestBuf[k]+j;            
			limit= Sbuf + nWidth;
			for(;Sbuf < limit;)
			{
                s+=*Sbuf;
				ss+=(long)*Sbuf**Sbuf;
                os+=*Obuf++**Sbuf++;
			}
		}
        if((r=(oo-o/num*o)*(ss-s/num*s)) > 0) // 均方根
		{
            r=(os-o/num*s)/sqrt(r);
			if(r > maxr)	// 寻找最大相关值
			{
				maxr=r;
				ri=0;
				rj=j;
			}
		}
        for(i=0; i < sumi; i++)
		{
            Sbuf = pDestBuf+i*nDestWidth + j;
			limit= Sbuf + nWidth;
			for(;Sbuf < limit;)
			{
				ss-=(long)*Sbuf**Sbuf;
                s-=*Sbuf++;
			} 
            os=0;
			for(k=0; k < nHeight; k++)
			{
				Obuf = pBuf+k*nWidth;//Obuf = pBuf[k];
				Sbuf = pDestBuf+(k+i+1)*nDestWidth+j;//Sbuf = pDestBuf[k+i+1]+j;
				limit=Sbuf+nWidth;
				for(;Sbuf < limit;)
					os+=(long)*Obuf++**Sbuf++;
			}
            Sbuf=pDestBuf+(nHeight+i)*nDestWidth+j;//Sbuf=pDestBuf[nHeight+i]+j;
			limit=Sbuf+nWidth;
			for(;Sbuf < limit;)
			{
				ss+=(long)*Sbuf**Sbuf;
                s+=*Sbuf++;
			}
            if((r=(oo-o/num*o)*(ss-s/num*s)) > 0)
			{
                r=(os-o/num*s)/sqrt(r);
				if(r > maxr)
				{
					maxr=r;
					ri=i+1;
					rj=j;
				}
			}
		}
	}
	//  y=ri+DestBuf.y+nHeight/2;
	//  x=rj+DestBuf.x+nWidth/2;
	y = ri + nHeight / 2;//-nHeight/2;;// ri + DestBuf.y + nHeight / 2;//-nHeight/2;
    x = rj + nWidth / 2;//= rj + DestBuf.x + nWidth / 2;//-nWidth/2;
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////
//
//  改变图像的大小：
////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::ChangeImageSize(BYTE *pBitmap, DWORD dwHeight,DWORD dwWidth,  BYTE *&pBitmapResult, DWORD dwHeightResult, DWORD dwWidthResult)
{
	if(!(dwWidth && dwHeight && dwWidthResult && dwHeightResult))
	{
		AfxMessageBox("CZXDib::ChangeImageSize  \n 图像大小不能为0!");
		return FALSE;
	}
	if(pBitmapResult) delete []pBitmapResult;
	pBitmapResult = new BYTE[dwWidthResult*dwHeightResult];
    double ratio_x,ratio_y;
	ratio_x = (double) dwWidth;
	ratio_x /= (double) dwWidthResult;
	ratio_y = (double) dwHeight;
	ratio_y /= (double) dwHeightResult;
	DWORD i,j,position,position_l=0;
	double x,y,temp,temp1;
	
	for(j=1; j<dwHeightResult-1; j++)
	for(i=1; i<dwWidthResult-1; i++)
	{
		x = i;
		y = j;
		x *= ratio_x;
		y *= ratio_y;
		
		//抽样：在2*2临域内采用内插法：
		int position = (int)(y) * dwWidth + (int)(x);
		/////采集该点处的灰度值
		temp  = pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x);
		position += dwWidth;
		temp -= (temp-(pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x)))*(y-(int)y);
		
		pBitmapResult[j*dwWidthResult+i] = (BYTE) (temp+0.5);
	}
	
	//最后一行用边界外拓法：
	DWORD dwSize = dwHeightResult*dwWidthResult;
	for(i=0;i<dwSize;i+=dwWidthResult)
	{
		pBitmapResult[i]=pBitmapResult[i+1];
		pBitmapResult[i+dwWidthResult-1] = pBitmapResult[i+dwWidthResult-2];
	}
	for(j=0;j<dwWidthResult;j++)
	{
		pBitmapResult[j]=pBitmapResult[j+dwWidthResult];
		pBitmapResult[dwSize-j-1] = pBitmapResult[dwSize-j-1-dwWidthResult];
	}
	
	return TRUE;
}
void CZXDib::GetRidOfLighting(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dMeanObj,double dDeviationObj,long dwWndWidth)
{
	if( !pBitmap)	
		return;
	
	BYTE* pTemp = new BYTE[dwHeight*dwWidth];
	memcpy(pTemp,pBitmap,sizeof(BYTE)*dwHeight*dwWidth);
	
	int m;
	for( m=0; m<10; m++)
		MeanFilter5(pBitmap,dwHeight,dwWidth);
	
	for( m=0; m<dwHeight*dwWidth; m++ )
	{
		long nTemp = pTemp[m] ;
		nTemp = (nTemp - pBitmap[m])*1.6+120;
		pBitmap[m] = nTemp > 255 ? 255 : (nTemp<0?0:nTemp);
	}
	
	delete pTemp;
	pTemp = NULL;
	return;
}
//////////////////////////////////////////////////////////////////////////
//
//	5*5窗口平滑
//
//////////////////////////////////////////////////////////////////////////
void CZXDib::MeanFilter5(unsigned char *pBitmap, int dwHeight, int dwWidth)
{
    int i,j,k,l,nTemp;
    unsigned char *pResult = 0;
	
    pResult =(unsigned char *) malloc (sizeof(unsigned char)*dwHeight*dwWidth);
    memcpy(pResult,pBitmap,sizeof(unsigned char )*dwHeight*dwWidth);
    
    int bDirection = 1, bContinue;
    
    nTemp = 0;
    for(k=0;k<5;++k)
        for(l=0;l<5;++l)
            nTemp += pResult[k*dwWidth+l];
        
        
        for(i=2;i<dwHeight-2;++i)
        {
            if( bDirection ) 
            {
                j = 3;
                bContinue = j < dwWidth-2;
            }else              
            {
                j = dwWidth-4;
                bContinue = j >= 2;
            }
            
            for(; bContinue ;)
            {
                if( bDirection )
                {
                    for(k=-2;k<=2;++k)
                    {
                        nTemp -= pResult[(i+k)*dwWidth+j-3];
                        nTemp += pResult[(i+k)*dwWidth+j+2];
                    }   }else 
                    {
                        for(k=-2;k<=2;++k)
                        {
                            nTemp += pResult[(i+k)*dwWidth+j-2];
                            nTemp -= pResult[(i+k)*dwWidth+j+3];
                    }   }
                    
                    
                    if( nTemp > 255*25 ) pBitmap[i*dwWidth+j] = 255;
                    else                 pBitmap[i*dwWidth+j] = nTemp / 25;  
                    
                    
                    if( bDirection )
                    {
                        ++j;
                        bContinue = j < dwWidth-2;
                    }else 
                    {
                        --j;
                        bContinue = j >= 2;
                    }
            } // end j
            
            
            if( i < dwHeight-3 )
            {
                if( bDirection)      j = dwWidth-3;
                else                 j = 2;
                
                
                for(k=-2;k<=2;++k)
                {
                    nTemp -= pResult[(i-2)*dwWidth+k+j];
                    nTemp += pResult[(i+3)*dwWidth+k+j];
                } 
            }
            
            bDirection = 1 - bDirection;
            
        }// end i
        
        if(pResult) free(pResult);
        
        return ;
}
//////////////////////////////////////////////////////////////////////////
//
//	窗口平滑
//
//////////////////////////////////////////////////////////////////////////
void CZXDib::MeanFilter(unsigned char *pBitmap, int dwHeight, int dwWidth,long nWndSizeX,long nWndSizeY)
{
    int i,j,k,l,nTemp;
    unsigned char *pResult = 0;

    pResult =(unsigned char *) malloc (sizeof(unsigned char)*dwHeight*dwWidth);
    memcpy(pResult,pBitmap,sizeof(unsigned char )*dwHeight*dwWidth);

    int bDirection = 1, bContinue;

	long nHalfSizeX = nWndSizeX/2;
	long nHalfSizeY = nWndSizeY/2;
	long nSize;

	if( nWndSizeX && nWndSizeY )
		nSize = nWndSizeY*nWndSizeX;
	else if( nWndSizeX )
		nSize = nWndSizeX;
	else if( nWndSizeY )
		nSize = nWndSizeY;


    nTemp = 0;
    for(k=0;k<nWndSizeY;++k)
    for(l=0;l<nWndSizeX;++l)
        nTemp += pResult[k*dwWidth+l];


    for(i=nHalfSizeY;i<dwHeight-nHalfSizeY;++i)
    {
        if( bDirection )
        {
            j = nWndSizeX-nHalfSizeX;//3;
            bContinue = j < dwWidth-nHalfSizeX;
        }else
        {
            j = dwWidth-(nWndSizeX-nHalfSizeX)-1;//3;dwWidth-4;
            bContinue = j >= nHalfSizeX;
        }

        for(; bContinue ;)
        {
            if( bDirection )
            {
                for(k=-nHalfSizeY;k<=nHalfSizeY;++k)
                {
                    nTemp -= pResult[(i+k)*dwWidth+j-nHalfSizeX-1];
                    nTemp += pResult[(i+k)*dwWidth+j+nHalfSizeX];
                }
			}else
            {
                for(k=-nHalfSizeY;k<=nHalfSizeY;++k)
                {
                    nTemp += pResult[(i+k)*dwWidth+j-nHalfSizeX];
                    nTemp -= pResult[(i+k)*dwWidth+j+nHalfSizeX+1];
				}
			}


            if( nTemp > 255*nSize ) pBitmap[i*dwWidth+j] = 255;
            else                 pBitmap[i*dwWidth+j] = nTemp / nSize;


            if( bDirection )
            {
                ++j;
                bContinue = j < dwWidth-nHalfSizeX;
            }else
            {
                --j;
                bContinue = j >= nHalfSizeX;
            }
        } // end j


        if( i < dwHeight-nHalfSizeY-1 )
        {
            if( bDirection)      j = dwWidth-nHalfSizeX-1;
            else                 j = nHalfSizeX;


            for(k=-nHalfSizeX;k<=nHalfSizeX;++k)
            {
                nTemp -= pResult[(i-nHalfSizeY)*dwWidth+k+j];
                nTemp += pResult[(i+nHalfSizeY+1)*dwWidth+k+j];
            }
        }

        bDirection = 1 - bDirection;

    }// end i

    if(pResult) free(pResult);

    return ;
}
/************************************************************************/
/* Gauss Smooth															*/
/* seta: 方差，确定模板大小(3×2×sqrt(2)×seta)						*/
/************************************************************************/
void CZXDib::ImgGaussSmoothH(BYTE* pBitmap, int dwHeight, int dwWidth, float seta)
{
	double PI = 3.1415926;
	int	i, j, x, y, m, n;
	int	nHWndSize = (int)(3.0 * sqrt(2) * seta + 0.5) - 1;	    // 模板半宽 
	int	nWndSize  = (nHWndSize << 1) + 1;						// 模板宽
	
	if( nHWndSize < 1 ) return;
	
	
	double * pFilter = new double [nWndSize];
	double dSum = 0;
	
	// set filter 
	for (i = 0; i < nHWndSize + 1; ++i)
	{
		pFilter[nHWndSize + i] = pFilter[nHWndSize - i] = exp(- i * i / (2.0 * seta * seta)) / (sqrt(2 * PI) * seta);
	}
	for (i = 0; i < nWndSize; ++i)
	{
		dSum += pFilter[i];
	}
	for (i = 0; i < nWndSize; ++i)
	{
		pFilter[i] /= dSum;
	}
	
	
	double* pResult = new double[dwHeight*dwWidth];
	
	/* 两次卷积 */
	for (i = 0; i < dwHeight; ++i)
	for (j = 0; j < dwWidth;  ++j)
	{
		dSum = 0;
		for( m = -nHWndSize; m <= nHWndSize; m++ )
		{
			n = m + j;
			if( n < 0	      ) n = -n; 
			if( n > dwWidth-1 ) n = (dwWidth-1)*2-n;
			dSum += pBitmap[i*dwWidth+ n] * pFilter[m+nHWndSize];
		}
	
		m = dSum+0.5;

		if( m > 255 ) 
			pBitmap[i*dwWidth+j] = 255;
		else if( m< 0 ) 
			pBitmap[i*dwWidth+j] = 0;
		else 
			pBitmap[i*dwWidth+j] = m;
	}

	if( pResult ) delete pResult;
	if( pFilter ) delete pFilter;
	return;
}
/************************************************************************/
/* Gauss Smooth															*/
/* seta: 方差，确定模板大小(3×2×sqrt(2)×seta)						*/
/************************************************************************/
void CZXDib::ImgGaussSmooth1(int* pBitmap, int dwHeight, int dwWidth, float seta)
{
	double PI = 3.1415926;
	int	i, j, x, y, m, n;
	int	nHWndSize = (int)(3.0 * sqrt(2) * seta + 0.5) - 1;	    // 模板半宽 
	int	nWndSize = (nHWndSize << 1) + 1;						// 模板宽

	if (nHWndSize < 1) return;


	double * pFilter = new double[nWndSize];
	double dSum = 0;

	// set filter 
	for (i = 0; i < nHWndSize + 1; ++i)
	{
		pFilter[nHWndSize + i] = pFilter[nHWndSize - i] = exp(-i * i / (2.0 * seta * seta)) / (sqrt(2 * PI) * seta);
	}
	for (i = 0; i < nWndSize; ++i)
	{
		dSum += pFilter[i];
	}
	for (i = 0; i < nWndSize; ++i)
	{
		pFilter[i] /= dSum;
	}


	double* pResult = new double[dwHeight*dwWidth];

	/* 两次卷积 */
	for (i = 0; i < dwHeight; ++i)
	for (j = 0; j < dwWidth; ++j)
	{
		dSum = 0;
		for (m = -nHWndSize; m <= nHWndSize; m++)
		{
			n = m + j;
			if (n < 0) n = -n;
			if (n > dwWidth - 1) n = (dwWidth - 1) * 2 - n;
			dSum += pBitmap[i*dwWidth + n] * pFilter[m + nHWndSize];
		}

		m = dSum + 0.5;
		pBitmap[i*dwWidth + j] = m;
	}

	if (pResult) delete pResult;
	if (pFilter) delete pFilter;
	return;
}
/************************************************************************/
/* Gauss Smooth															*/
/* seta: 方差，确定模板大小(3×2×sqrt(2)×seta)						*/
/************************************************************************/
void CZXDib::ImgGaussSmooth(BYTE* pBitmap, int dwHeight, int dwWidth, float seta)
{
	double PI = 3.1415926;
	int	i, j, x, y, m, n;
	int	nHWndSize = (int)(3.0 * sqrt(2) * seta + 0.5) - 1;	    // 模板半宽 
	int	nWndSize  = (nHWndSize << 1) + 1;						// 模板宽

	if( nHWndSize < 1 ) return;
	
	
	double * pFilter = new double [nWndSize];
	double dSum = 0;

	// set filter 
	for (i = 0; i < nHWndSize + 1; ++i)
	{
		pFilter[nHWndSize + i] = pFilter[nHWndSize - i] = exp(- i * i / (2.0 * seta * seta)) / (sqrt(2 * PI) * seta);
	}
	for (i = 0; i < nWndSize; ++i)
	{
		dSum += pFilter[i];
	}
	for (i = 0; i < nWndSize; ++i)
	{
		pFilter[i] /= dSum;
	}

	
	double* pResult = new double[dwHeight*dwWidth];
	
	/* 两次卷积 */
	for (i = 0; i < dwHeight; ++i)
	for (j = 0; j < dwWidth;  ++j)
	{
		dSum = 0;
		for( m = -nHWndSize; m <= nHWndSize; m++ )
		{
			n = m + j;
			if( n < 0	      ) n = -n; 
			if( n > dwWidth-1 ) n = (dwWidth-1)*2-n;
			dSum += pBitmap[i*dwWidth+ n] * pFilter[m+nHWndSize];
		}
		pResult[i*dwWidth+j] = dSum;
	}
	for (i = 0; i < dwHeight; ++i)
	for (j = 0; j < dwWidth;  ++j)
	{
		dSum = 0;
		for( m = -nHWndSize; m <= nHWndSize; m++ )
		{
			n = m + i;
			if( n < 0	      ) n = -n; 
			if( n > dwHeight-1 ) n = (dwHeight-1)*2 - n;
			dSum += pResult[n*dwWidth+ j] * pFilter[m+nHWndSize];
		}
		m = dSum+0.5;

		if( m > 255 ) 
			pBitmap[i*dwWidth+j] = 255;
		else if( m< 0 ) 
			pBitmap[i*dwWidth+j] = 0;
		else 
			pBitmap[i*dwWidth+j] = m;
	}

	if( pResult ) delete pResult;
	if( pFilter ) delete pFilter;
	return;
}
/************************************************************************/
/************************************************************************/
void CZXDib::ImgMeanSmooth(BYTE* pBitmap, int dwHeight, int dwWidth, long nWndWidth)
{
	double PI = 3.1415926;
	int	i, j, x, y, m, n;
	int	nHWndSize = nWndWidth;// 模板半宽 
	int	nWndSize  = (nHWndSize << 1) + 1;						// 模板宽
	
	if( nHWndSize < 1 ) return;
	
	double dSum;

	
	
	double* pResult = new double[dwHeight*dwWidth];
	
	/* 两次卷积 */
	for (i = 0; i < dwHeight; ++i)
	{
		dSum = 0;
		j=-1; 
		for( m = -nHWndSize; m <= nHWndSize; m++ )
		{
			n = m + j;
			if( n < 0	      ) n = -n; 
			else if( n > dwWidth-1 ) n = (dwWidth-1)*2-n;
			
			dSum += pBitmap[i*dwWidth+ n];
		}

		for (j = 0; j < dwWidth;  ++j)
		{
			n = -nHWndSize + j - 1;
			if( n < 0	      ) n = -n; 
			else if( n > dwWidth-1 ) n = (dwWidth-1)*2-n;

			dSum -= pBitmap[i*dwWidth+n];

			n = nHWndSize + j;
			if( n < 0	      ) n = -n; 
			if( n > dwWidth-1 ) n = (dwWidth-1)*2-n;

			dSum += pBitmap[i*dwWidth+n];

			pResult[i*dwWidth+j] = dSum / nWndSize;
		}		
	}


	for (j = 0; j < dwWidth;  ++j)
	{
		dSum = 0;
		i=-1; 
		for( m = -nHWndSize; m <= nHWndSize; m++ )
		{
			n = m + i;
			if( n < 0	      ) n = -n; 
			if( n > dwHeight-1 ) n = (dwHeight-1)*2-n;
			
			dSum += pResult[n*dwWidth+ j];
		}
		
		for (i = 0; i < dwHeight; ++i)
		{
			n = -nHWndSize + i -1;
			if( n < 0	      ) n = -n; 
			else if( n > dwHeight-1 ) n = (dwHeight-1)*2-n;
			
			dSum -= pResult[n*dwWidth+j];
			
			n = nHWndSize + i;
			if( n < 0	      ) n = -n; 
			else if( n > dwHeight-1 ) n = (dwHeight-1)*2-n;
			
			dSum += pResult[n*dwWidth+j];
			
			pResult[n*dwWidth+j] = dSum / nWndSize;
		}		
	}	
	
	for( i=0; i<dwHeight*dwWidth; i++ )
	{
		m = pResult[i];
		if( m > 255 ) 
			pBitmap[i] = 255;
		else if( m< 0 ) 
			pBitmap[i] = 0;
		else 
			pBitmap[i] = m;
	}

	
	if( pResult ) delete pResult;
	return;
}

void CZXDib::LocalHistgromBanlance(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nWndSize,double dSeta)
{
	long nHWndSize = nWndSize;
	long nTWndSize = (nHWndSize<<1)+1;

	long i,j,m,n,nStep,nTemp,u,v;

	long pHG[256];
	memset( pHG, 0, sizeof(long)*256);
	
	double pdHG[256];
	
	BYTE* pResult = new BYTE[dwHeight*dwWidth];

	//BYTE* pSmooth = new BYTE[dwHeight*dwWidth];
	//memcpy(pSmooth,pBitmap,dwHeight*dwWidth*sizeof(BYTE));
	//ImgGaussSmooth(pSmooth,dwHeight,dwWidth,18);

	i = -1;
	j = -1;
	for( m=-nHWndSize; m<=nHWndSize; m++ )
	for( n=-nHWndSize; n<=nHWndSize; n++)
	{
		u = m+i;
		if( u < 0 ) u = 0;
		else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
		v = n+j;
		if( v < 0 ) v = 0;
		else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;

		pHG[pBitmap[u*dwWidth+v]]++;
	}

	for( i=0; i<dwHeight; i++ )
	{
		// 调整方向：
		if( i % 2 )
		{
			j = dwWidth -1;
			nStep = -1;
		}else 
		{
			j = 0;
			nStep = 1;
		}

		// 把最上面一行减掉，最下面新的一行加上
		for( n =-nHWndSize; n <= nHWndSize; n++ )
		{
			v = n+j;
			if( v < 0 ) v = 0;
			else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
			
			u = i-(1+nHWndSize);
			if( u < 0 ) u = 0;
			else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
			
			pHG[pBitmap[u*dwWidth+v]]--;

			u = i+nHWndSize;
			if( u < 0 ) u = 0;
			else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
			
			pHG[pBitmap[u*dwWidth+v]]++;
		}	
		
		for( ; j<dwWidth && j >=0 ;  j+=nStep )
		{
			// 把最左面一行减掉，最右面新的一行加上，　或者相反			
			for( m =-nHWndSize; m <= nHWndSize; m++ )
			{
				u = m + i;
				if( u < 0 ) u = 0;
				else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;

				v = j + (nHWndSize);
				if( v < 0 ) v = 0;
				else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
				
				pHG[pBitmap[u*dwWidth+v]] += nStep;
				
				v = j - (nHWndSize + 1);
				if( v < 0 ) v = 0;
				else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
				
				pHG[pBitmap[u*dwWidth+v]] -= nStep;
			}
			

			pdHG[0] = pHG[0];
			for( m=0; m<256; m++ )
			{
				pdHG[m] = pdHG[m-1] + pHG[m];
			}

			nTemp = pdHG[pBitmap[i*dwWidth+j]] / nTWndSize / nTWndSize * 255 + 0.5;
			nTemp = nTemp * dSeta + pBitmap[i*dwWidth+j]*(1-dSeta);
			
			if( nTemp > 255 )
				pResult[i*dwWidth+j] = 255;
			else if( nTemp < 0 ) 
				pResult[i*dwWidth+j] = 0;
			else 
				pResult[i*dwWidth+j] = nTemp;
			
		}
	}

	memcpy(pBitmap,pResult,sizeof(BYTE)*dwWidth*dwHeight);
	delete pResult;
	pResult = NULL;

	return;
}
void CZXDib::LocalHistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double pdHGRefer[256],long nWndSize,double dSeta)
{
	long nHWndSize = nWndSize;
	long nTWndSize = (nHWndSize<<1)+1;

	long i,j,m,n,nStep,nTemp,u,v;

	long pHG[256];
	memset( pHG, 0, sizeof(long)*256);
	
	double pdHGCal[256];
	double pdHGReferCal[256];
	long pnMap[256];

	memcpy(pdHGReferCal,pdHGRefer,sizeof(double)*256);
	for( i=1; i<256; i++ )
	{
		pdHGReferCal[i] = pdHGReferCal[i-1] + pdHGReferCal[i];
		pnMap[i] = i;
	}

	for( i=0; i<dwHeight*dwWidth; i++ )
		pHG[pBitmap[i]] ++;

	pdHGCal[0] = pHG[0];
	for( m=1; m<256; m++ )
	{
		pdHGCal[m] = pdHGCal[m-1] + pHG[m];
	}
	
	for( m=0; m<256; m++)
	{
		long   nMinN = m;
		double dMin = 256*dwHeight*dwWidth;
		double dTemp;
		for( n=0; n<256; n++ )
		{
			dTemp = fabs( pdHGReferCal[n]*dwHeight*dwWidth - pdHGCal[m]) ;
			if( dTemp < dMin )
			{
				dMin = dTemp;
				nMinN = n;
			}					
		}
		pnMap[m] = nMinN;
	}
	

	for( i=0; i<dwWidth*dwHeight; i++ )
		pBitmap[i] = pnMap[pBitmap[i]];			
	
/*
	i = -1;
	j = -1;
	for( m=-nHWndSize; m<=nHWndSize; m++ )
	for( n=-nHWndSize; n<=nHWndSize; n++)
	{
		u = m+i;
		if( u < 0 ) u = 0;
		else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
		v = n+j;
		if( v < 0 ) v = 0;
		else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;

		pHG[pBitmap[u*dwWidth+v]]++;
	}

	for( i=0; i<dwHeight; i++ )
	{
		// 调整方向：
		if( i % 2 )
		{
			j = dwWidth -1;
			nStep = -1;
		}else 
		{
			j = 0;
			nStep = 1;
		}

		// 把最上面一行减掉，最下面新的一行加上
		for( n =-nHWndSize; n <= nHWndSize; n++ )
		{
			v = n+j;
			if( v < 0 ) v = 0;
			else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
			
			u = i-(1+nHWndSize);
			if( u < 0 ) u = 0;
			else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
			
			pHG[pBitmap[u*dwWidth+v]]--;

			u = i+nHWndSize;
			if( u < 0 ) u = 0;
			else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;
			
			pHG[pBitmap[u*dwWidth+v]]++;
		}	
		
		for( ; j<dwWidth && j >=0 ;  j+=nStep )
		{
			// 把最左面一行减掉，最右面新的一行加上，　或者相反			
			for( m =-nHWndSize; m <= nHWndSize; m++ )
			{
				u = m + i;
				if( u < 0 ) u = 0;
				else if( u > dwHeight-1 ) u = (dwHeight-1)*2-u;

				v = j + (nHWndSize);
				if( v < 0 ) v = 0;
				else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
				
				pHG[pBitmap[u*dwWidth+v]] += nStep;
				
				v = j - (nHWndSize + 1);
				if( v < 0 ) v = 0;
				else if( v > dwWidth -1 ) v = (dwWidth-1)*2-v;
				
				pHG[pBitmap[u*dwWidth+v]] -= nStep;
			}
			

			pdHGCal[0] = pHG[0];
			for( m=0; m<256; m++ )
			{
				pdHGCal[m] = pdHGCal[m-1] + pHG[m];
			}

			for( m=0; m<256; m++)
			{
				long   nMinN = m;
				double dMin = 256*nTWndSize*nTWndSize;
				double dTemp;
				for( n=0; n<256; n++ )
				{
					dTemp = fabs( pdHGReferCal[n]*nTWndSize*nTWndSize - pdHGCal[m]) ;
					if( dTemp < dMin )
					{
						dMin = dTemp;
						nMinN = n;
					}					
				}
				pnMap[m] = nMinN;
			}

			pResult[i*dwWidth+j] = pnMap[pBitmap[i*dwWidth+j]];			
		}
	}
*/

	return;
}
void CZXDib::HistgromBanlance(long pHG[256],BYTE pnMap[256])
{	
	long i,nTemp;
	double dCount,dTemp;
	
	double pHGCal[256];
	pHGCal[0] = pHG[0];
	
	pnMap[0] = 0;
	
	dCount = pHG[0];

	for( i=1; i<256; i++ )
	{
		pHGCal[i] = pHGCal[i-1] + pHG[i];
		dCount   += pHG[i];
		pnMap[i] = i;
	}	
	
	dCount = 255 / dCount;

	for( i=0; i<256; i++)
	{
		nTemp = pHGCal[i] * dCount + 0.5;
		pnMap[i] = nTemp > 255 ? 255 : (nTemp < 0? 0 : nTemp );
	}
}
void CZXDib::LocalHistgromRegular(const double * pHGOrigin,const double* pHGDestin,DWORD dwLength,long*& pnMap)
{	
	long i,j,m,n,nStep,nTemp,u,v,nMinN;
	double dMin,dTemp;
	
	double *pHGOriginCal = new double[dwLength];
	pHGOriginCal[0] = pHGOrigin[0];
	
	double *pHGDestinCal = new double [dwLength];
	pHGDestinCal[0] = pHGDestin[0];

	if( pnMap ) delete pnMap;
	pnMap = new long [dwLength];
	pnMap[0] = 0;
	
	for( i=1; i<dwLength; i++ )
	{
		pHGDestinCal[i] = pHGDestinCal[i-1] + pHGDestin[i];
		pHGOriginCal[i] = pHGOriginCal[i-1] + pHGOrigin[i];
		pnMap[i] = i;
	}	
	
	for( m=0; m<dwLength; m++)
	{
		nMinN = m;
		dMin = dwLength;
		for( n=0; n<dwLength; n++ )
		{
			dTemp = pHGDestinCal[n] - pHGOriginCal[m];
			if ( dTemp < 0 )
				dTemp = 0- dTemp;

			if( dTemp < dMin )
			{
				dMin = dTemp;
				nMinN = n;
			}					
		}
		pnMap[m] = nMinN;
	}

	if( pHGDestinCal )	delete []pHGDestinCal;
	if( pHGOriginCal )	delete []pHGOriginCal;
}

void CZXDib::LocalHSIRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double pHisH[361],double pHisB[255],double pHisS[100],BYTE* &pReslt)
{
	double pdH[361];
	memset( pdH, 0, sizeof(double)*361);
	
	double pdB[255];
	memset( pdB, 0, sizeof(double)*255);

	double pdS[100];
	memset( pdS, 0, sizeof(double)*100);
	
	DWORD dwSize = dwHeight*dwWidth;
	double H,S,B;
	long i;
	for(i=0; i<dwSize; i++ )
	{
		GetHSBFromRGB(H,S,B,pBitmap[i*3+2],pBitmap[i*3+1],pBitmap[i*3+0]);
		pdH[(long)(H+0.5)] ++;
		pdB[(long)(B*255+0.5)] ++;
		pdS[(long)(S*100+0.5)] ++;
	}	
	
	double dRatio = 1.0 / dwSize;
	for(  i=0;i<361; i++)
		pdH[i] *= dRatio;

	for( i=0; i<255; i++ )
		pdB[i] *= dRatio;
	
	for( i=0; i<100; i++ )
		pdS[i] *= dRatio;
	
	long * pnMapH = NULL;
	long * pnMapB = NULL;
	long * pnMapS = NULL;

	LocalHistgromRegular(pdH,pHisH,360,pnMapH);
	LocalHistgromRegular(pdB,pHisB,255,pnMapB);
	LocalHistgromRegular(pdS,pHisS,100,pnMapS);


	if( pReslt ) delete pReslt;
	pReslt = new BYTE [dwSize*3]; 
	
	for(  i=0; i<dwSize; i++ )
	{
		GetHSBFromRGB(H,S,B,pBitmap[i*3+2],pBitmap[i*3+1],pBitmap[i*3+0]);

		H = pnMapH[(long)(H+0.5)];
		if( H > 360 )	H = 360;
		else if( H < 0 ) H = 0;

		S = pnMapS[(long)(S*100+0.5)] / 100.0;
		if( S < 0 )				S = 0;
		else if( S > 1 )		S = 1;

		B = pnMapB[(long)(B*255+0.5)] / 255.0;
		if( B < 0 )				B = 0;
		else if( B > 255 )		B = 255;

		GetRGBFromHSB(pReslt[i*3+2],pReslt[i*3+1],pReslt[i*3+0],H,S,B);
	}
	
	if( pnMapH ) delete pnMapH;
	pnMapH = NULL;	
	if( pnMapB ) delete pnMapB;
	pnMapB = NULL;
	if( pnMapS ) delete pnMapS;
	pnMapS = NULL;
	
	return;	
}

void CZXDib::GetMeanAndVariant(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double& dMeanR,double& dMeanG,double& dMeanB,double& dDeviationR,double& dDeviationG,double& dDeviationB)
{
	DWORD dwSize = dwHeight*dwWidth;
	
	dMeanR=dMeanB=dMeanG =0 ;
	dDeviationR=dDeviationG=dDeviationB =0 ;
	
	for(long i=0; i<dwSize; i++ )
	{
		dMeanR += pBitmap[i*3+2];
		dMeanG += pBitmap[i*3+1];
		dMeanB += pBitmap[i*3+0];
		dDeviationR += (double)(pBitmap[i*3+2])*(double)(pBitmap[i*3+2]);
		dDeviationG += (double)(pBitmap[i*3+1])*(double)(pBitmap[i*3+1]);
		dDeviationB += (double)(pBitmap[i*3+0])*(double)(pBitmap[i*3+0]);
	}	
	
	double dRatio = 1.0 / dwSize;
	dMeanR *= dRatio;
	dMeanG *= dRatio;
	dMeanB *= dRatio;
	
	dDeviationR = sqrt(dDeviationR*dRatio - dMeanR*dMeanR);
	dDeviationG = sqrt(dDeviationG*dRatio - dMeanG*dMeanG);
	dDeviationB = sqrt(dDeviationB*dRatio - dMeanB*dMeanB);

	return;
}
void CZXDib::GetRectHistgram(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT rect,double dMeanR[256],double dMeanG[256],double dMeanB[256])
{
	DWORD dwSize = (rect.right - rect.left)*(rect.bottom-rect.top);
	long nPos,i,j;	

	memset( dMeanR,0,sizeof(double)*256);
	memset( dMeanG,0,sizeof(double)*256);
	memset( dMeanB,0,sizeof(double)*256);
	
	for( i=rect.top; i<rect.bottom; i++)
	for( j=rect.left; j<rect.right; j++)
	{
		nPos = (i*dwWidth+j)*3;
		dMeanR[pBitmap[nPos+2]]++;
		dMeanG[pBitmap[nPos+1]]++;
		dMeanB[pBitmap[nPos+0]]++;
	}	
	
	double dRatio = 1.0 / dwSize;
	for( i=0; i<256; i++ )
	{
		dMeanR[i] *= dRatio;
		dMeanG[i] *= dRatio;
		dMeanB[i] *= dRatio;
	}
	
	return;
}
void CZXDib::GetRectMeanAndVariant(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,RECT rect,double& dMeanR,double& dMeanG,double& dMeanB,double& dDeviationR,double& dDeviationG,double& dDeviationB)
{
	DWORD dwSize = (rect.right - rect.left)*(rect.bottom-rect.top);
	long nPos;
	
	dMeanR=dMeanB=dMeanG =0 ;
	dDeviationR=dDeviationG=dDeviationB =0 ;
	
	//BYTE* pTemp = new BYTE[dwSize*3];
	//static CString strFileName = "c:\\00000.bmp";
	long i; for( i=rect.top; i<rect.bottom; i++)
	for( long j=rect.left; j<rect.right; j++)
	{
		nPos = (i*dwWidth+j)*3;
		dMeanR += pBitmap[nPos+2];
		dMeanG += pBitmap[nPos+1];
		dMeanB += pBitmap[nPos+0];
		dDeviationR += (double)(pBitmap[nPos+2])*(double)(pBitmap[nPos+2]);
		dDeviationG += (double)(pBitmap[nPos+1])*(double)(pBitmap[nPos+1]);
		dDeviationB += (double)(pBitmap[nPos+0])*(double)(pBitmap[nPos+0]);

		//pTemp[((i-rect.top)*(rect.right-rect.left)+j-rect.left)*3+2] = pBitmap[nPos+2];
		//pTemp[((i-rect.top)*(rect.right-rect.left)+j-rect.left)*3+1] = pBitmap[nPos+1];
		//pTemp[((i-rect.top)*(rect.right-rect.left)+j-rect.left)*3+0] = pBitmap[nPos+0];
	}	
	//WriteBMPFileFromVector(strFileName,pTemp,60,60,24);
	//FindNextFileName(strFileName,1);
	
	double dRatio = 1.0 / dwSize;
	dMeanR *= dRatio;
	dMeanG *= dRatio;
	dMeanB *= dRatio;
	
	dDeviationR = sqrt(dDeviationR*dRatio - dMeanR*dMeanR);
	dDeviationG = sqrt(dDeviationG*dRatio - dMeanG*dMeanG);
	dDeviationB = sqrt(dDeviationB*dRatio - dMeanB*dMeanB);
	
	return;
}

void CZXDib::RGBRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dMeanR,double dMeanG,double dMeanB,double dDeviationR,double dDeviationG,double dDeviationB,BYTE* &pReslt)
{
	double dMeanR0,dMeanB0,dMeanG0;
	double dDeviationR0,dDeviationG0,dDeviationB0;
	double dTemp;

	GetMeanAndVariant(pBitmap,dwHeight,dwWidth,dMeanR0,dMeanG0,dMeanB0,dDeviationR0,dDeviationG0,dDeviationB0);
			
	DWORD dwSize = dwHeight*dwWidth;

	dDeviationR0  = 1.0 / dDeviationR0 * dDeviationR;
	dDeviationG0  = 1.0 / dDeviationG0 * dDeviationG;
	dDeviationB0  = 1.0 / dDeviationB0 * dDeviationB;

	if( pReslt )	delete pReslt;
	pReslt = new BYTE [dwSize*3];

	for(long i=0; i<dwSize; i++ )
	{
		dTemp = ( pBitmap[i*3+2] - dMeanR0 ) * dDeviationR0 + dMeanR;
		pReslt[i*3+2] = dTemp < 0 ? 0:(dTemp>255)? 255 : dTemp;
		dTemp = ( pBitmap[i*3+1] - dMeanG0 ) * dDeviationG0 + dMeanG;
		pReslt[i*3+1] = dTemp < 0 ? 0:(dTemp>255)? 255 : dTemp;
		dTemp = ( pBitmap[i*3+0] - dMeanB0 ) * dDeviationB0 + dMeanB;
		pReslt[i*3+0] = dTemp < 0 ? 0:(dTemp>255)? 255 : dTemp;
	}

	return;	
}
//////////////////////////////////////////////////////////////////////////
//
//	pBitmap 　　为原始图像指针，或者任意指针，但是有效区域只在其右下方：
//	dwHeight 　 pBitmap的高度
//	dwWidth　　 pBitmap的宽度
//	nCellHeight 分块的高度 :  必须为偶数
//	nCellWidth  分块的宽度 :  必须为偶数
//  nHCellCount
//  nVCellCount 水平方向cell数目
//  x,y
//////////////////////////////////////////////////////////////////////////
void CZXDib::RGBRegularCell(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nBandCount,
							long nCellHeight,long nCellWidth,
							long x, long y,
							double dMean[256],double dDeviation[256],
							long nHCellCount,long nVCellCount,
							double dSeta,
							BYTE* &pResult)
{
	// 修改算法，进来之后对不满足条件的进行扩充，方便后续处理
	long i,j,k,m,n,u,v,nNewX,nNewY,nLeft,nTop;
	DWORD dwNewWidth,dwNewHeight;

	RECT rectOrigPos;
	rectOrigPos.left   = 0;
	rectOrigPos.right  = dwWidth;
	rectOrigPos.top    = 0;
	rectOrigPos.bottom = dwHeight;
	
	dwNewWidth = dwWidth;	
	dwNewHeight= dwHeight;
	nNewX	   = x;
	nNewY	   = y;

	if( dwHeight + nCellHeight <= nVCellCount * nCellHeight  || dwWidth + nCellWidth <= nHCellCount * nCellWidth )
		return;

	//if( enCBT & CBT_LEFT )
	{
		k = 0;
		if( x == 0 )
		{
			k = nCellWidth;

		}else if( x < nCellWidth )
		{
			k = nCellWidth - x + nCellWidth;
		}
		rectOrigPos.left   = k;
		rectOrigPos.right += k;
		dwNewWidth		  += k;
		nNewX			  += k;
	}
	//if( enCBT & CBT_RIGHT )
	{
		k = dwWidth - ( x + nHCellCount*nCellWidth );
		if( k >= nCellWidth ) 
		{
			k = 0;

		}else if( k > 0 )
		{			
			k = ( k / nCellWidth + 1 ) * nCellWidth - k;

		}else // k < 0 
		{
			k =  - k + nCellWidth ; 
		}

		dwNewWidth += k;
	}
	//if( enCBT & CBT_TOP )
	{
		k = 0;
		if( y == 0 )
		{
			k = nCellHeight;

		}else if( y < nCellHeight )
		{
			 k = nCellHeight - y + nCellHeight;
		}
		rectOrigPos.top    += k;
		rectOrigPos.bottom += k;
		dwNewHeight        += k;
		nNewY			   += k;
	}
	//if( enCBT & CBT_BOTTOM )
	{
		k = dwHeight - ( y + nVCellCount*nCellHeight );
		if( k >= nCellHeight ) 
		{
			k = 0;
			
		}else if( k > 0 )
		{			
			k = ( k / nCellHeight + 1 ) * nCellHeight - k;
			
		}else // k < 0 
		{
			k =  - k + nCellHeight ; 
		}

		dwNewHeight += k;
	}

	// 拷贝图像生成新的
	BYTE* pTempResult = NULL;
	pTempResult = new BYTE [dwNewHeight*dwNewWidth*nBandCount];

	BYTE* pBitmapNew = new BYTE[dwNewHeight*dwNewWidth*nBandCount];

	for( j=0; j<dwNewHeight; j++ )
	for( i=0; i<dwNewWidth;  i++ )
	{
		u = i - rectOrigPos.left;
		while( u < 0 || u > dwWidth-1 ) 
		{
			if( u < 0 )
				u = -u-1;
			else 
				u = (dwWidth-1)*2 - u +1;
		}
		
		v = j - rectOrigPos.top;
		while( v < 0 || v > dwHeight -1)
		{
			if( v < 0 ) v = -v-1;
			else v = (dwHeight-1)*2 - v +1;
		}
		
		memcpy( pBitmapNew+(j*dwNewWidth+i)*nBandCount, pBitmap+(v*dwWidth+u)*nBandCount, sizeof(BYTE)*nBandCount);
	}

	memcpy(pTempResult,pBitmapNew,sizeof(BYTE)*dwNewHeight*dwNewWidth*nBandCount);
	

	// 统计每一个小区域的信息：
	double dMean9[9][3];
	double dDeviation9[9][3];
	double dObjMean9[9][3];
	double dObjDeviation9[9][3];

	long nCellCount = (nHCellCount+2)*(nVCellCount+2);
	double *pdMean      = new double[nCellCount*3];
	double *pdDeviation = new double[nCellCount*3];

	for( j=0; j<nVCellCount+2; j++ )
	for( i=0; i<nHCellCount+2; i++ )	
	{
		RECT rectCell;
		rectCell.top   = j*nCellHeight;
		rectCell.left  = i*nCellWidth ;
		rectCell.bottom= rectCell.top  + nCellHeight;
		rectCell.right = rectCell.left + nCellWidth;

		GetRectMeanAndVariant(pBitmapNew,dwNewHeight,dwNewWidth,
								rectCell,
								pdMean[(j*(nHCellCount+2)+i)*3+2],
								pdMean[(j*(nHCellCount+2)+i)*3+1],
								pdMean[(j*(nHCellCount+2)+i)*3+0],
								pdDeviation[(j*(nHCellCount+2)+i)*3+2],
								pdDeviation[(j*(nHCellCount+2)+i)*3+1],
								pdDeviation[(j*(nHCellCount+2)+i)*3+0]);
	}


	//重新来平衡每一个小块应该达到的均值方差：
	double *pdObjMean      = new double[nCellCount*3];
	double *pdObjDeviation = new double[nCellCount*3];
	memcpy( pdObjMean,pdMean,sizeof(double)*nCellCount*3);
	memcpy( pdObjDeviation,pdDeviation,sizeof(double)*nCellCount*3);

	double dMean_Mean[3],dMean_Deviation[3];
	dMean_Mean[0] = dMean_Deviation[0] = 0;
	dMean_Mean[1] = dMean_Deviation[1] = 0;
	dMean_Mean[2] = dMean_Deviation[2] = 0;

	double dDeviation_Mean[3],dDeviation_Deviation[3];
	dDeviation_Mean[0] = dDeviation_Deviation[0] = 0;
	dDeviation_Mean[1] = dDeviation_Deviation[1] = 0;
	dDeviation_Mean[2] = dDeviation_Deviation[2] = 0;

	for( k=0;k<3;k++)
	{
		for( i=0; i<nCellCount; i++ )
		{
			dMean_Mean[k]      += pdObjMean[i*3+k];
			dMean_Deviation[k] += pdObjMean[i*3+k]*pdObjMean[i*3+k];

			dDeviation_Mean[k]      += pdObjDeviation[i*3+k];
			dDeviation_Deviation[k] += pdObjDeviation[i*3+k]*pdObjDeviation[i*3+k];
		}
		dMean_Mean[k]     /= nCellCount;
		dMean_Deviation[k] = sqrt(dMean_Deviation[k]/(nCellCount) - dMean_Mean[k]*dMean_Mean[k]);

		dDeviation_Mean[k]     /= nCellCount;
		dDeviation_Deviation[k] = sqrt(dDeviation_Deviation[k]/(nCellCount) - dDeviation_Mean[k]*dDeviation_Mean[k]);
		
		
		for( i=0; i<nCellCount; i++ )
		{
			pdObjMean[i*3+k]      = ( pdObjMean[i*3+k]      - dMean_Mean[k]     ) / (dSeta+0.01) + dMean[k] ;
			pdObjDeviation[i*3+k] = ((pdObjDeviation[i*3+k] - dDeviation_Mean[k]) * (dSeta+0.01) + dDeviation[k]) / (pdDeviation[i*3+k]+ 0.1) ;
		}
	}


	double dRetioCellHeight = 1.0 / ((double)nCellHeight+1);
	double dRetioCellWidth  = 1.0 / ((double)nCellWidth+1);

	// 分块匀色：
	for( j=1; j<nVCellCount+1; j++ )
	for( i=1; i<nHCellCount+1; i++ )	
	{		
		for( m=0; m<3; m++ )		
		for( n=0; n<3; n++ )
		for( k=0; k<3; k++ )
		{
			dMean9        [m*3 + n][k] = pdMean        [((j+m-1)*(nHCellCount+2) + i + n-1)*3+k];
			dDeviation9   [m*3 + n][k] = pdDeviation   [((j+m-1)*(nHCellCount+2) + i + n-1)*3+k];
			dObjMean9     [m*3 + n][k] = pdObjMean     [((j+m-1)*(nHCellCount+2) + i + n-1)*3+k];
			dObjDeviation9[m*3 + n][k] = pdObjDeviation[((j+m-1)*(nHCellCount+2) + i + n-1)*3+k];
		}

		for( m=0; m<nCellHeight; m++ )
		for( n=0; n<nCellWidth;  n++ )
		{
			nTop  = m <= (nCellHeight >> 1) ? 0 : 1;
			nLeft = n <= (nCellWidth  >> 1) ? 0 : 1;						
			
			double dy = (1.0 - 2.0 * nTop  ) * 0.5 + m * dRetioCellHeight;			
			double dx = (1.0 - 2.0 * nLeft ) * 0.5 + n * dRetioCellWidth;
			double dTemp1,dTemp2,dTemp3,dTemp4,dTemp ;

			for( k=0; k<nBandCount; k++)
			{
				dTemp = pBitmapNew[((j*nCellHeight+m)*dwNewWidth + i*nCellWidth+n)*nBandCount+k];
				//dTemp1 = (dTemp - dMean9[nTop * 3     + nLeft  ][k]) / (dDeviation9[nTop * 3     + nLeft  ][k] + 1) * dObjDeviation9[nTop * 3     + nLeft  ][k] + dObjMean9[nTop * 3     + nLeft  ][k];
				//dTemp2 = (dTemp - dMean9[nTop * 3     + nLeft+1][k]) / (dDeviation9[nTop * 3     + nLeft+1][k] + 1) * dObjDeviation9[nTop * 3     + nLeft+1][k] + dObjMean9[nTop * 3     + nLeft+1][k];
				//dTemp3 = (dTemp - dMean9[(nTop+1) * 3 + nLeft  ][k]) / (dDeviation9[(nTop+1) * 3 + nLeft  ][k] + 1) * dObjDeviation9[(nTop+1) * 3 + nLeft  ][k] + dObjMean9[(nTop+1) * 3 + nLeft  ][k];
				//dTemp4 = (dTemp - dMean9[(nTop+1) * 3 + nLeft+1][k]) / (dDeviation9[(nTop+1) * 3 + nLeft+1][k] + 1) * dObjDeviation9[(nTop+1) * 3 + nLeft+1][k] + dObjMean9[(nTop+1) * 3 + nLeft+1][k];

				dTemp1 = (dTemp - dMean9[nTop * 3     + nLeft  ][k]) * dObjDeviation9[nTop * 3     + nLeft  ][k] + dObjMean9[nTop * 3     + nLeft  ][k];
				dTemp2 = (dTemp - dMean9[nTop * 3     + nLeft+1][k]) * dObjDeviation9[nTop * 3     + nLeft+1][k] + dObjMean9[nTop * 3     + nLeft+1][k];
				dTemp3 = (dTemp - dMean9[(nTop+1) * 3 + nLeft  ][k]) * dObjDeviation9[(nTop+1) * 3 + nLeft  ][k] + dObjMean9[(nTop+1) * 3 + nLeft  ][k];
				dTemp4 = (dTemp - dMean9[(nTop+1) * 3 + nLeft+1][k]) * dObjDeviation9[(nTop+1) * 3 + nLeft+1][k] + dObjMean9[(nTop+1) * 3 + nLeft+1][k];

				dTemp  = dTemp1 - (dTemp1 - dTemp2)* (dx-(int)dx);
				dTemp -= (dTemp-(dTemp3 - (dTemp3 - dTemp4)* (dx-(int)dx)))*(dy-(int)dy);
				
				pTempResult[((j*nCellHeight+m)*dwNewWidth + i*nCellWidth+n)*nBandCount+k] = dTemp < 0 ? 0 : (dTemp > 255? 255 : dTemp);
			}
		}		
	}


	for( i=rectOrigPos.top; i<rectOrigPos.bottom;  i++ )
	for( j=rectOrigPos.left; j<rectOrigPos.right; j++ )
	{
		u = j - rectOrigPos.left;		
		v = i - rectOrigPos.top;
		
		memcpy( pResult+(v*dwWidth+u)*nBandCount, pTempResult+(i*dwNewWidth+j)*nBandCount, sizeof(BYTE)*nBandCount);
	}

	delete pBitmapNew;
	pBitmapNew = NULL;

	delete pTempResult;
	pTempResult = NULL;

	if( pdMean )	delete pdMean;
	pdMean = NULL;
	if( pdDeviation)delete pdDeviation;
	pdDeviation = NULL;

	if( pdObjMean ) delete pdObjMean;
	pdObjMean = NULL;
	if( pdObjDeviation) delete pdObjDeviation;
	pdObjDeviation = NULL;
}
//------------------------------------------------------------------
//函数功能：灰值形态学操作，灰值腐蚀
//输入：pImage_Buf：	原图像
//     pImage_Erosion：	腐蚀后图像
//说明：结构元为nWndSize×nWndSize
//     灰度的形态膨胀和形态腐蚀运算的表达式与图像处理中的卷积积分非常相似
//     (即以和、差代替连乘，用最小最大运算代替求总和)，从信号处理的角度来看，
//     灰度形态和差是一种极值滤波，因此灰度形态学也是一种非线性的、不可逆的变换，
//     灰度形态膨胀与腐蚀相当于局部最大和最小滤波运算。
//	modified by zhouxiao 05/12/16
//------------------------------------------------------------------
void CZXDib::GrayErosionFilter(BYTE * pInput, BYTE*& pResult,long nSearchHeight,long nSearchWidth,long nWndSize)
{
	int i,j,k,m;
	short min;
	int nHalfWnd = nWndSize / 2;

	if( pResult ) delete pResult;
	pResult = new BYTE [nSearchHeight*nSearchWidth];
	
	//----求最小值----
	for( i = 0; i < nSearchHeight; i++)
		for( j = 0; j < nSearchWidth; j++)
		{
			if (i < nHalfWnd || i > nSearchHeight-nHalfWnd-1 || j < nHalfWnd || j > nSearchWidth-nHalfWnd-1)
				pResult[i * nSearchWidth + j] = 0;
			else
			{
				min = pInput[i * nSearchWidth + j];
				for( k = i - nHalfWnd; k <= i + nHalfWnd; k++)
					for( m = j - nHalfWnd; m <= j + nHalfWnd; m++)
					{
						if(pInput[k * nSearchWidth + m] < min)
							min = pInput[k * nSearchWidth + m];
					}
					pResult[i * nSearchWidth + j] = min;	//腐蚀后的结果
			}
		}
}
//------------------------------------------------------------------
//函数功能：灰值形态学操作，灰值膨胀
//输入：pImage_Erosion：	腐蚀后图像
//     pImage_Dilation：腐蚀图像经膨胀后的图像
//说明：结构元为nWndSize×nWndSize
//	modified by zhouxiao 05/12/16
//------------------------------------------------------------------
void CZXDib::GrayDilationFilter(BYTE* pInput, BYTE*&pImgResult,long nSearchHeight,long nSearchWidth, long nWndSize)
{
	int i,j,k,m;
	short max;
	long nHalfWnd = nWndSize / 2;
	if( pImgResult ) delete pImgResult;
	pImgResult = new BYTE [nSearchWidth*nSearchHeight];
	
	//----求最大值----
	for( i = 0; i < nSearchHeight; i++)
	for( j = 0; j < nSearchWidth; j++)
	{
		if (i < nHalfWnd || i > nSearchHeight-nHalfWnd-1 || j < nHalfWnd || j > nSearchWidth-nHalfWnd-1)
			pImgResult[i * nSearchWidth + j] = 0;
		else
		{
			max = pInput[i * nSearchWidth + j];
			for( k = i - nHalfWnd; k <= i + nHalfWnd; k++)
				for( m = j - nHalfWnd; m <= j + nHalfWnd; m++)
				{
					if(pInput[k * nSearchWidth + m] > max)
						max = pInput[k * nSearchWidth + m];
				}
				pImgResult[i * nSearchWidth + j] = max;	//膨胀后的结果
		}
	}
		
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//在图像中截取一片区域（8位或24位）
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CutRegionFromSidePicture(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, 
						 double dCenterY, double dCenterX,double dRatio,
						 BYTE *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to)
{
	if(x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from>x_to || y_from>y_to)
		return false;
	
	if(pResult) delete pResult;
	pResult = (BYTE*) new BYTE[(x_to-x_from)*(y_to-y_from)];
	
	DWORD dwOffset = 0;
	long i; for( i=y_from; i<y_to; i++ )
	{
		long j;
		for( j=x_from; j<dCenterX;  j++ )
		{
			double dTemp = dCenterX - (dCenterX-j)*dRatio;
			long x = (long)dTemp;	
			
			pResult[dwOffset++] = (dTemp - x)*(long)(pBitmap[i*dwWidth+x+1]) + (1 - dTemp + x)*(long)(pBitmap[i*dwWidth+x]);
		}
		
		for( ; j<x_to; j++ )
		{
			double dTemp = dCenterX + (j-dCenterX)*(2-dRatio);
			long x = (long)dTemp;	
			
			if( x + 1 > dwWidth-1 )
				pResult[dwOffset++] = 0;
			else 
				pResult[dwOffset++] = (dTemp - x)*(long)(pBitmap[i*dwWidth+x+1]) + (1 - dTemp + x)*(long)(pBitmap[i*dwWidth+x]);			
		}
	}	
	
	return true;
}
void CZXDib::RotationImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dCenterY,double dCenterX,double dAngle,BYTE*& pResult)
{
	DWORD dwSize = dwHeight*dwWidth;
	
	CutRegionFromRotatePicture(pBitmap,dwHeight,dwWidth,dCenterY,dCenterX,dAngle,pResult,0,0,dwWidth,dwHeight);		
	
	double dMinRadius = min(fabs(dCenterY-dwHeight-1),fabs(dCenterY));
	dMinRadius = min(min(fabs(dCenterX-dwWidth -1),fabs(dCenterX)),dMinRadius);
	
	for(long i=0; i<dwHeight; i++ )
	for(long j=0; j<dwWidth;  j++ )
	{
		if( (j - dCenterX )*(j - dCenterX ) + (i - dCenterY ) * (i - dCenterY ) > dMinRadius*dMinRadius )
			pResult[i*dwWidth+j] = 0;
	}
}
void CZXDib::RotationBlur(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,double dCenterY,double dCenterX,double dAngle,BYTE*& pResult)
{
	DWORD dwSize = dwHeight*dwWidth;
	double* pdAcc = new double[dwSize];
	memset(pdAcc,0,sizeof(double)*dwSize);

	double dStep = 0.5;
	
	for( double di=0; di<dAngle; di += dStep )
	{
		BYTE* pTemp = NULL;
		CutRegionFromRotatePicture(pBitmap,dwHeight,dwWidth,dCenterY,dCenterX,di,pTemp,0,0,dwWidth,dwHeight);
		
		for( long j=0; j<dwSize; j++)
			pdAcc[j] += pTemp[j];
		
		delete pTemp;		
	}
		
	delete pResult;
	pResult = new BYTE[dwSize];

	if( dAngle > dStep )
	{
		for( long j=0; j<dwSize; j++)
			pResult[j] = pdAcc[j] / ( dAngle / dStep ) ;
	}else
	{
		for( long j=0; j<dwSize; j++)
			pResult[j] = pBitmap[j];
	}

	double dMinRadius = min(fabs(dCenterY-dwHeight-1),fabs(dCenterY));
		   dMinRadius = min(min(fabs(dCenterX-dwWidth -1),fabs(dCenterX)),dMinRadius);

	for(long i=0; i<dwHeight; i++ )
	for(long j=0; j<dwWidth;  j++ )
	{
		if( (j - dCenterX )*(j - dCenterX ) + (i - dCenterY ) * (i - dCenterY ) > dMinRadius*dMinRadius )
			pResult[i*dwWidth+j] = 0;
	}

	delete pdAcc;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//在图像中截取一片区域（8位或24位）
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::CutRegionFromRotatePicture(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dCenterY, double dCenterX,double dAngel,BYTE *&pResult, DWORD x_from, DWORD y_from, DWORD x_to, DWORD y_to)
{
	if(x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from>x_to || y_from>y_to)
		return false;
	
	if(pResult) delete pResult;
	pResult = (BYTE*) new BYTE[(x_to-x_from)*(y_to-y_from)];
		
	double pi = dAngel* 3.1415926/180.0;
	double dsin = sin(pi);
	double dcos = cos(pi);

	DWORD dwSize = dwHeight * dwWidth;
	
	
	int i,j;
	
	DWORD offset=0;
	for(i=y_from; i<y_to; i++)
	for(j=x_from; j<x_to; j++)
	{
		double x,y,temp;
		x = j - dCenterX;
		y = i - dCenterY;
		temp = dcos*x - dsin*y + dCenterX;
		y    = dsin*x + dcos*y + dCenterY;
		x = temp;
		
		if( x<0 || x>dwWidth-1 || y<0 || y>dwHeight-1 )
			pResult[offset++] = 0;
		else 
		{
			//抽样：在2*2临域内采用内插法：
			int position = (int)(y) * dwWidth + (int)(x);
			/////采集该点处的灰度值
			temp  = pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x);
			position += dwWidth;
			if( position + 1 >= dwSize )
				continue;
			temp -= (temp-(pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x)))*(y-(int)y);
			pResult[offset++] = temp + 0.5;
		}
	}
	
	return true;
}

bool CZXDib::CutRegionFromRotatePictureD(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, double dCenterY, double dCenterX,double dAngel,BYTE *&pResult, double x_from, double y_from, double x_to, double y_to,DWORD &dwResultHegiht,DWORD &dwResultWidth)
{
	if(x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from>x_to || y_from>y_to)
		return false;
	
	if(pResult) delete pResult;
	dwResultWidth = x_to - x_from + 0.5;
	dwResultHegiht= y_to - y_from + 0.5;
	pResult = (BYTE*) new BYTE[dwResultHegiht*dwResultWidth];
		
	double pi = dAngel* 3.1415926/180.0 ;
	double dsin = sin(pi);
	double dcos = cos(pi);
		
	long i,j;	
	double di,dj;
	DWORD offset = 0;
	for( i=0,di=y_from; i<dwResultHegiht; i++,di++)
	for( j=0,dj=x_from; j<dwResultWidth ; j++,dj++)
	{
		double x,y,temp;
		x = dj - dCenterX;
		y = di - dCenterY;
		temp = dcos*x - dsin*y + dCenterX;
		y    = dsin*x + dcos*y + dCenterY;
		x = temp;
		
		if( x<0 || x>dwWidth-1 || y<0 || y>dwHeight-1 )
			pResult[offset++] = 0;
		else 
		{
			//抽样：在2*2临域内采用内插法：
			int position = (int)(y) * dwWidth + (int)(x);
			/////采集该点处的灰度值
			temp  = pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x);
			position += dwWidth;
			temp -= (temp-(pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x)))*(y-(int)y);
			pResult[offset++] = temp + 0.5;
		}
	}
	
	return true;
}
bool CZXDib::CutRegionFromRotatePictureC( BYTE *pBitmap, int dwHeight, int dwWidth, double dCenterY, double dCenterX, double dAngel, BYTE *&pResult, double x_from, double y_from, double x_to, double y_to, int &dwResultHegiht, int &dwResultWidth )
{
	if ( x_from<0 || x_to >dwWidth || y_from<0 || y_to >dwHeight || x_from > x_to || y_from > y_to )
		return false;

	if ( pResult ) delete pResult;
	dwResultWidth  = x_to - x_from + 0.5;
	dwResultHegiht = y_to - y_from + 0.5;
	pResult = ( BYTE* ) new BYTE[dwResultHegiht*dwResultWidth*3];

	double pi = dAngel* 3.1415926 / 180.0;
	double dsin = sin( pi );
	double dcos = cos( pi );

	long i, j;
	double di, dj;
	int offset = 0;
	for ( i = 0, di = y_from; i < dwResultHegiht; i++, di++ )
	for ( j = 0, dj = x_from; j < dwResultWidth; j++, dj++ )
	{
		double x, y, temp;
		x = dj - dCenterX;
		y = di - dCenterY;
		temp = dcos*x - dsin*y + dCenterX;
		y = dsin*x + dcos*y + dCenterY;
		x = temp;

		if ( x<0 || x>dwWidth - 1 || y<0 || y>dwHeight - 1 )
		{
			pResult[offset * 3 + 0] = pResult[offset * 3 + 1] = pResult[offset * 3 + 2] = 0;
		}
		else
		{
			for ( int k = 0; k < 3; k++ )
			{
				//抽样：在2*2临域内采用内插法：
				int position = (int) (y) * dwWidth + (int) ( x );
				/////采集该点处的灰度值
				temp = pBitmap[position*3+k] - ( pBitmap[position * 3 + k] - pBitmap[(position + 1) * 3 + k] )* ( x - (int) x );
				position += dwWidth;
				temp -= ( temp - ( pBitmap[position * 3 + k] - ( pBitmap[position * 3 + k] - pBitmap[(position + 1) * 3 + k] )* ( x - (int) x ) ) )*( y - (int) y );
				pResult[offset*3+k] = temp + 0.5;
			}
		}
		offset++;
	}

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取图像直方图
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetHistgram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, DWORD*& pHis)
{
	if( pHis ) delete pHis;
	pHis = new DWORD [256];
	memset(pHis,0,sizeof(DWORD)*256);

	for( long j=0; j<dwHeight; j++ )
	for( long i=0; i<dwWidth;  i++)	
		pHis[pBitmap[j*dwWidth+i]]++;

	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取图像直方图
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetHistgramImage(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, BYTE*& pHisImg,DWORD &dwHisImgHeight,DWORD &dwHisImgWidth)
{
	if( pHisImg ) delete pHisImg;
	dwHisImgHeight = 256;
	dwHisImgWidth  = 256;

	DWORD *pHis = new DWORD [256];
	memset(pHis,0,sizeof(DWORD)*256);

	DWORD dwSize = dwHeight*dwWidth;
	
	for( long j=0; j<dwSize; j++ )
		pHis[pBitmap[j]]++;

	TranslateHistogramToVector(pHis,pHisImg,false);

	delete pHis;
	pHis = NULL;

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//获取图像直方图
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CZXDib::GetHistgram(BYTE *pBitmap, DWORD dwHeight, DWORD dwWidth, long pHis[256])
{
	memset(pHis,0,sizeof(long)*256);
	
	for( long j=0; j<dwHeight; j++ )
	for( long i=0; i<dwWidth;  i++)	
		pHis[pBitmap[j*dwWidth+i]]++;
		
	return true;
}
//------------------------------------------------------------------
//
//------------------------------------------------------------------
void CZXDib::SubImage(BYTE* pImage1, BYTE* pImage2,DWORD dwHeight,DWORD dwWidth)
{
	for( long i=dwHeight*dwWidth-1; i>=0; i--)
	{
		int nTemp = (int)(pImage1[i])-pImage2[i];
		pImage2[i] = nTemp < 0 ? -nTemp : nTemp;
	}
}
void CZXDib::AppendImage(BYTE* pTemp1,BYTE* pTemp2,DWORD dwHeight,DWORD dwWidth1,DWORD dwWidth2,BYTE*& pResult)
{
	if( pResult ) delete pResult;
	pResult = new BYTE [dwHeight*(dwWidth1+dwWidth2)];
	
	for( long i=0; i<dwHeight; i++ )
	{
		memcpy(pResult+i*(dwWidth1+dwWidth2),pTemp1+i*dwWidth1,sizeof(BYTE)*dwWidth1);
		memcpy(pResult+i*(dwWidth1+dwWidth2)+dwWidth1,pTemp2+i*dwWidth2,dwWidth2);
	}
}
//////////////////////////////////////////////////////////////////////////
//
//
//
//////////////////////////////////////////////////////////////////////////
bool CZXDib::GetRegularHis(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,WORD flag, BYTE*& pResult,DWORD &dwResultHeight,DWORD &dwResultWidth)
{
	BYTE* pTemp1 = NULL;
	BYTE* pTemp2 = NULL;
	
	if( flag == 24 )
	{
		Translate24To8Color(pBitmap,dwHeight,dwWidth,pTemp1);
	}
	else 
	{
		pTemp1 = new BYTE [dwHeight*dwWidth];
		memcpy(pTemp1,pBitmap,dwHeight*dwWidth*sizeof(BYTE));
	}
	
	RegularToGrayLevel(pTemp1,dwHeight,dwWidth,pResult);
	AppendImage(pTemp1,pResult,dwHeight,dwWidth,dwWidth,pTemp2);
	
	delete pResult;	
	pResult = pTemp2;	
	pTemp2 = NULL;
	
	Histogram(pTemp1,dwHeight,dwWidth,pTemp2);
	for( long j=0; j<256; j++ )
		pTemp2[j] = pTemp2[j] > 128 ? 255 : pTemp2[j]*2;
	
	
	TranslateHistogramToVector(pTemp2,pTemp1);
	
	for(long i=0; i<256*256; i++ )
		pTemp1[i] = !pTemp1[i] ? 100 : 200;
	
	ChangeImageSize(pTemp1,256,256, pTemp2,dwHeight,128);
	
	AppendImage(pResult,pTemp2,dwHeight,dwWidth*2,128,pTemp1);
	
	
	dwResultHeight = dwHeight;
	dwResultWidth  = dwWidth * 2 + 128;
	
	if( pResult ) delete pResult;
	pResult = pTemp1;
	pTemp1 = NULL;
	
	
	delete pTemp1;
	delete pTemp2;
	
	return true;
}
//------------------------------------------------------------------
//函数功能：差图像
//输入：无
//说明：形态学滤波后的图像与原图像做差
//	modified by zhouxiao 05/12/16
//------------------------------------------------------------------
void CZXDib::SubImage(BYTE* pImage1,BYTE* pImage2,BYTE* pResult,long nImgHeight,long nImgWidth,long nFilterWidth)
{
	for( long i=nFilterWidth/2; i<nImgHeight-nFilterWidth/2; i++ )
		for( long j=nFilterWidth/2; j<nImgWidth -nFilterWidth/2; j++ )
		{
			//if( pImage1[i*nImgWidth+j] > pImage2[i*nImgWidth+j] )
			pResult[i*nImgWidth+j] = abs((long)pImage1[i*nImgWidth+j] - (long)(pImage2[i*nImgWidth+j]));
			//else 
			//	pResult[i*nImgWidth+j] = 0;
		}
}
/*
void CZXDib::HistgromRegular(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long pnHGRefer[256])
{
	long i,m,n,nTemp,nMinN,dMin;
	long nSize = dwHeight*dwWidth;
	
	long pnHGCal[256];
	long pnHGReferCal[256];
	long pnMap[256];	
	long pHG[256];
	memset( pHG, 0, sizeof(long)*256);
	
	
	memcpy(pnHGReferCal,pnHGRefer,sizeof(long)*256);
	for( i=1; i<256; i++ )
	{
		pnHGReferCal[i] = pnHGReferCal[i-1] + pnHGReferCal[i];
		pnMap[i] = i;
	}
	
	for( i=0; i<nSize; i++ )
		pHG[pBitmap[i]] ++;
	
	pnHGCal[0] = pHG[0];
	for( m=1; m<256; m++ )
	{
		pnHGCal[m] = pnHGCal[m-1] + pHG[m];
	}
	
	for( m=0; m<256; m++)
	{
		nMinN = m;
		dMin  = 256*nSize;
		for( n=0; n<256; n++ )
		{
			nTemp = abs( pnHGReferCal[n] - pnHGCal[m]) ;
			if( nTemp < dMin )
			{
				dMin = nTemp;
				nMinN = n;
			}					
		}
		pnMap[m] = nMinN;
	}	
	
	for( i=0; i<nSize; i++ )
		pBitmap[i] = pnMap[pBitmap[i]];	
}*/
double CZXDib::CNK(long nN,long nK)
{
	long i;
	double CNK=1;
	
	// 计算指数部分
	for( i=0; i<nK; i++ )
		CNK *= (nN-i)/(double)(i+1.0);
	/*for( i=nK; i>0; i--)
		CNK /= i;
*/
	return CNK;
}
double CZXDib::GetPFByGivenSNR_PD(long nBase,long nExp,double dSNR,double dPDTotal)
{
	
	return 0;
}
int CZXDib::LabelImage1(BYTE* pImage,int dwWidth,int dwHeight,unsigned int* pRect)
{
	int i,j,k,nLabelNO,nLineNow,nLineLast;
	int dwSize = dwHeight * dwWidth;          
	int nFrom,nTo;    
	int nCount=0;                     
	
    unsigned int* pLabelMap = new unsigned int[dwSize];

	unsigned int* pLabel[2];
	pLabel[0] = new unsigned int[dwSize];
	pLabel[1] = new unsigned int[dwSize];
	
	memset( pLabel[0], 0, sizeof(unsigned int)*dwSize );
	memset( pLabel[1], 0, sizeof(unsigned int)*dwSize ); 	
	memset( pRect,	   0, sizeof(unsigned int)*dwSize  );
	
	
	for( i=0; i<dwSize; i++ )
		pLabelMap[i] = i;
	
	nLabelNO  = 0;
	nLineLast = 1;
	nLineNow  = 0;
	
	for( i=1; i<dwHeight; i++ )
	{
		memset(pLabel[nLineNow],0,sizeof(unsigned int)*dwWidth);
		
		for( j=1; j<dwWidth;  j++ )
		{			
			if( !pImage[i*dwWidth+j] )
				continue;
			
			if( pImage[i*dwWidth+j-1] == pImage[(i-1)*dwWidth+j] )
			{
				if( !pImage[i*dwWidth+j-1] )
				{
					pRect[nLabelNO*4+0]  = j;
					pRect[nLabelNO*4+1]  = i;
					pLabel[nLineNow][j] = nLabelNO++;
				}
				else 
				{
					nTo   = max(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					nFrom = min(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					
					pLabel[nLineNow][j] = nTo;
					
					if( nFrom == nTo ) continue;
					
					// 下面处理两区域的合并
					pLabelMap[nFrom] = nTo;			// 修改映射表
					
					pRect[nTo*4+0] = min( pRect[nTo*4+0], pRect[nFrom*4+0] );
					pRect[nTo*4+1] = min( pRect[nTo*4+1], pRect[nFrom*4+1] );
					pRect[nTo*4+2] = max( pRect[nTo*4+2], pRect[nFrom*4+2] );
					pRect[nTo*4+3] = max( pRect[nTo*4+3], pRect[nFrom*4+3] );
					
					// 处理合并的区域, 倒序扫描一次即可
					for( k=nLabelNO; k>0; k-- )		
						pLabelMap[k] = pLabelMap[pLabelMap[k]];	
				}
				
			}else if( pImage[i*dwWidth+j-1] )
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineNow][j-1]];
				
			}else //if(pImage[(i-1)*dwWidth+j] )
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineLast][j]];
			}
			
			k = pLabel[nLineNow][j];
			pRect[k*4+0 ] = min(pRect[k*4+0],j);
			pRect[k*4+1 ] = min(pRect[k*4+1],i);
			pRect[k*4+2 ] = max(pRect[k*4+2],j);
			pRect[k*4+3 ] = max(pRect[k*4+3],i);
		}
		
		nLineLast = nLineNow;
		nLineNow  = 1-nLineNow;
	}
	
	nCount=0;
	for( i = 0; i<nLabelNO; i++ )
	{
		if( pLabelMap[i] == i )	
		{
			pRect[nCount*4+0] = pRect[i*4+0];
			pRect[nCount*4+1] = pRect[i*4+1];
			pRect[nCount*4+2] = pRect[i*4+2];
			pRect[nCount*4+3] = pRect[i*4+3];
			nCount++;
		}
	}     
	
	delete pLabelMap;
	delete pLabel[0];
	delete pLabel[1];
	return nCount;
}


void CZXDib::LabelImage(unsigned char* pBitmap,int dwHeight,int dwWidth,unsigned char byTarget,RECT*& pRect,int &nCount,int*& pResult, int*& pnArea)
{
	int i,j,k,nLabelNO,nLineNow,nLineLast;
	int dwSize = dwHeight * dwWidth;

	if( pResult ) delete pResult;
	pResult = new int[dwSize];
	memset(pResult,0,dwSize*sizeof(int));

	if( pnArea ) delete pnArea;
	pnArea = new int[dwSize];
	memset(pnArea,0,dwSize*sizeof(int));

	
	int* pLabel[2] = { NULL, NULL };

	pLabel[0] = new int[dwWidth];
	pLabel[1] = new int[dwWidth];
	memset( pLabel[0], 0, sizeof(int)*dwWidth );
	memset( pLabel[1], 0, sizeof(int)*dwWidth );
	
	if( pRect ) delete pRect;
	pRect = new RECT [dwSize/4];
	memset(pRect,0,sizeof(RECT)*(dwSize/4));
	
	int* pLabelMap = NULL;
	pLabelMap = new int[dwSize];
	for( i=0; i<dwSize; i++ )
		pLabelMap[i] = i;

	memset(pBitmap,255-byTarget,dwWidth);
	memset(pBitmap+dwSize-dwWidth,255-byTarget,dwWidth);
	for( i=1; i<dwHeight-1; i++ )
		pBitmap[i*dwWidth]=pBitmap[i*dwWidth+dwWidth-1]=255-byTarget;
	
	nLabelNO  = 1;
	nLineLast = 1;
	nLineNow  = 0;
	
	for( i=1; i<dwHeight; i++ )
	{
		memset(pLabel[nLineNow],0,sizeof(unsigned char)*dwWidth);
		
		for( j=1; j<dwWidth;  j++ )
		{			
			if( pBitmap[i*dwWidth+j] != byTarget )
				continue;
			
			if( pBitmap[i*dwWidth+j-1] == pBitmap[(i-1)*dwWidth+j] )
			{
				if( pBitmap[i*dwWidth+j-1] != byTarget )
				{
					pLabel[nLineNow][j] = nLabelNO;						
					
					pRect[nLabelNO].left  = j;
					pRect[nLabelNO].top   = i;
					
					nLabelNO++;
				}
				else 
				{
					int nTo   = max(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					int nFrom = min(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					
					pLabel[nLineNow][j] = nFrom;
					pResult[i*dwWidth+j] = nFrom;
					
					if( nFrom == nTo ) continue;
					
					pLabelMap[nTo] = nFrom;		
					
					pRect[nFrom].left  = min(pRect[nTo].left  ,pRect[nFrom].left  );
					pRect[nFrom].right = max(pRect[nTo].right ,pRect[nFrom].right );
					pRect[nFrom].top   = min(pRect[nTo].top   ,pRect[nFrom].top   );
					pRect[nFrom].bottom= max(pRect[nTo].bottom,pRect[nFrom].bottom);					
					
					for( k=nLabelNO; k>0; k-- )		
						pLabelMap[k] = pLabelMap[pLabelMap[k]];	
				}
				
			}else if( pBitmap[i*dwWidth+j-1] == byTarget )
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineNow][j-1]];
				
			}else 
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineLast][j]];
			}

			k = pLabel[nLineNow][j];
			pResult[i*dwWidth+j] = k;

			pRect[k].left  = min(pRect[k].left  ,j);
			pRect[k].right = max(pRect[k].right ,j);
			pRect[k].top   = min(pRect[k].top   ,i);
			pRect[k].bottom= max(pRect[k].bottom,i);
		}
		
		nLineLast = nLineNow;
		nLineNow  = 1-nLineNow;
	}
	
	nCount=0;
	for( i = 0; i<nLabelNO; i++ )
	{
		if( pLabelMap[i] == i )	
			memcpy(pRect+nCount++,pRect+i,sizeof(RECT));
	}

	for( i=0; i<dwSize; i++ )
	{
		if( !pResult[i] ) continue;
		pResult[i] = pLabelMap[pResult[i]];
		pnArea[pResult[i]]++;
	}

	delete pLabel[0];
	delete pLabel[1];
	delete pLabelMap;
}

void CZXDib::LabelImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE byTarget,RECT*& pRect,long &nCount)
{
	long i,j,k,nLabelNO,nLineNow,nLineLast;
	DWORD dwSize = dwHeight * dwWidth;
	
	int* pLabel[2] = { NULL, NULL };
	pLabel[0] = new int[dwWidth];
	pLabel[1] = new int[dwWidth];
	memset( pLabel[0], 0, sizeof(int)*dwWidth );
	memset( pLabel[1], 0, sizeof(int)*dwWidth );
	
	if( pRect ) delete pRect;
	pRect = new RECT [dwSize/4];
	memset(pRect,0,sizeof(RECT)*(dwSize/4));
	
	int* pLabelMap = NULL;
	pLabelMap = new int[dwSize];
	for( i=0; i<dwSize; i++ )
		pLabelMap[i] = i;
	
	nLabelNO  = 1;
	nLineLast = 1;
	nLineNow  = 0;
	
	for( i=1; i<dwHeight; i++ )
	{
		memset(pLabel[nLineNow],0,sizeof(int)*dwWidth);
		
		for( j=1; j<dwWidth;  j++ )
		{			
			if( pBitmap[i*dwWidth+j] != byTarget )
				continue;
			
			if( pBitmap[i*dwWidth+j-1] == pBitmap[(i-1)*dwWidth+j] )
			{
				if( pBitmap[i*dwWidth+j-1] != byTarget )
				{
					pLabel[nLineNow][j] = nLabelNO;
					
					pRect[nLabelNO].left  = j;
					pRect[nLabelNO].top   = i;
					
					nLabelNO++;
				}
				else 
				{
					long nTo   = min(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					long nFrom = max(pLabelMap[pLabel[nLineLast][j]],pLabelMap[pLabel[nLineNow][j-1]]);
					
					pLabel[nLineNow][j] = nTo;
					
					if( nFrom == nTo ) continue;
					
					// 下面处理两区域的合并
					pLabelMap[nFrom] = nTo;			// 修改映射表
					
					pRect[nTo].left  = min(pRect[nTo].left  ,pRect[nFrom].left  );
					pRect[nTo].right = max(pRect[nTo].right ,pRect[nFrom].right );
					pRect[nTo].top   = min(pRect[nTo].top   ,pRect[nFrom].top   );
					pRect[nTo].bottom= max(pRect[nTo].bottom,pRect[nFrom].bottom);					
					
					// 处理合并的区域, 倒序扫描一次即可
					for( k=nLabelNO; k>0; k-- )		
						pLabelMap[k] = pLabelMap[pLabelMap[k]];	
				}
				
			}else if( pBitmap[i*dwWidth+j-1] == byTarget )
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineNow][j-1]];
				
			}else //if(pBitmap[(i-1)*dwWidth+j] == byTarget)
			{
				pLabel[nLineNow][j] = pLabelMap[pLabel[nLineLast][j]];
			}
			
			k = pLabel[nLineNow][j];
			pRect[k].left  = min(pRect[k].left  ,j);
			pRect[k].right = max(pRect[k].right ,j);
			pRect[k].top   = min(pRect[k].top   ,i);
			pRect[k].bottom= max(pRect[k].bottom,i);
		}
		
		nLineLast = nLineNow;
		nLineNow  = 1-nLineNow;
	}
	
	nCount=0;
	for( i = 1; i<nLabelNO; i++ )
	{
		if( pLabelMap[i] == i )	
			memcpy(pRect+nCount++,pRect+i,sizeof(RECT));
	}
	
	delete pLabel[0];
	delete pLabel[1];
	delete pLabelMap;
}
//////////////////////////////////////////////////////////////////////////
void CZXDib::SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int** pResult)
{
    int i,j,temp;
	
    if(*pResult) delete (*pResult);
    (*pResult) = new int[dwHeight*dwWidth];
    memset(*pResult,0,sizeof(int)*dwHeight*dwWidth);    
    
    for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1;j++)
	{
		(*pResult)[i*dwWidth+j]  =  (int)(pBitmap[j-1+(i+1)*dwWidth])
								+ 2*(int)(pBitmap[j  +(i+1)*dwWidth])
								+   (int)(pBitmap[j+1+(i+1)*dwWidth])
								-   (int)(pBitmap[j-1+(i-1)*dwWidth])
								- 2*(int)(pBitmap[j  +(i-1)*dwWidth])
								-   (int)(pBitmap[j+1+(i-1)*dwWidth]);		
	}
	
	return ;
}
//////////////////////////////////////////////////////////////////////////
void CZXDib::SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int nFilterSize, int** pResult)
{
    int i,j,m,n,temp;
	
    if(*pResult) delete (*pResult);
    (*pResult) = new int[dwHeight*dwWidth];
    memset(*pResult,0,sizeof(int)*dwHeight*dwWidth);    
    
    for(i=nFilterSize;i<dwHeight-nFilterSize;i++)
	for(j=1;j<dwWidth-1;j++)
	{
		for( m=1; m<nFilterSize; m++ )
		(*pResult)[i*dwWidth+j]  =  (int)(pBitmap[j+(i+m)*dwWidth])
								-   (int)(pBitmap[j+(i-m)*dwWidth]);			
	}
	
	return ;
}
//////////////////////////////////////////////////////////////////////////
void CZXDib::SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, int nSizeH, int nSizeW, int** pResult)
{
    int i,j,m,n,temp;
	
    if(*pResult) delete (*pResult);
    (*pResult) = new int[dwHeight*dwWidth];
    memset(*pResult,0,sizeof(int)*dwHeight*dwWidth);    
    
    for(i=nSizeH;i<dwHeight-nSizeH;i++)
	for(j=nSizeW;j<dwWidth-nSizeW;j++)
	{
		for( m=1; m<nSizeH; m++ )
		for( n=1; n<nSizeW; n++ )
		(*pResult)[i*dwWidth+j]  =  (int)(pBitmap[j+n+(i+m)*dwWidth])
								 -  (int)(pBitmap[j+n+(i-m)*dwWidth]);			
	}
	
	return ;
}
void CZXDib::GetInteImg(int* pBitmap,int dwHeight,int dwWidth,double*& pIntImg)
{
	int i,j;
	if( pIntImg )	delete pIntImg;
	pIntImg = new double[dwHeight*dwWidth];
	
	// 第一个元素
	pIntImg[0] = pBitmap[0];
	
	// 第一行
	for( j=1; j<dwWidth ; j++ )
		pIntImg[j] = pIntImg[j-1] + pBitmap[j];
	
	// 后续行
	for( i=1; i<dwHeight; i++ )
	{
		pIntImg[i*dwWidth] = pIntImg[(i-1)*dwWidth] + pBitmap[i*dwWidth];
		
		for( j=1; j<dwWidth ; j++ )
			pIntImg[i*dwWidth+j] = pIntImg[(i-1)*dwWidth+j] + pIntImg[i*dwWidth+j-1] - pIntImg[(i-1)*dwWidth+j-1] + pBitmap[i*dwWidth+j];
	}
}
void CZXDib::GetInteImg(BYTE* pBitmap,int dwHeight,int dwWidth,double*& pIntImg)
{
	int i,j;
	if( pIntImg )	delete pIntImg;
	pIntImg = new double[dwHeight*dwWidth];
	
	// 第一个元素
	pIntImg[0] = pBitmap[0];
	
	// 第一行
	for( j=1; j<dwWidth ; j++ )
		pIntImg[j] = pIntImg[j-1] + pBitmap[j];
	
	// 后续行
	for( i=1; i<dwHeight; i++ )
	{
		pIntImg[i*dwWidth] = pIntImg[(i-1)*dwWidth] + pBitmap[i*dwWidth];
		
		for( j=1; j<dwWidth ; j++ )
			pIntImg[i*dwWidth+j] = pIntImg[(i-1)*dwWidth+j] + pIntImg[i*dwWidth+j-1] - pIntImg[(i-1)*dwWidth+j-1] + pBitmap[i*dwWidth+j];
	}
}
void CZXDib::GetInteImg(BYTE* pBitmap,int dwHeight,int dwWidth,int*& pIntImg)
{
	int i,j;
	if( pIntImg )	delete pIntImg;
	pIntImg = new int[dwHeight*dwWidth];
	
	// 第一个元素
	pIntImg[0] = pBitmap[0];
	
	// 第一行
	for( j=1; j<dwWidth ; ++j )
		pIntImg[j] = pIntImg[j-1] + pBitmap[j];
	
	// 后续行
	BYTE * pBit0 = pBitmap + dwWidth;
	int  * pCur  = pIntImg + dwWidth;
	for (i = 1; i < dwHeight; ++i)
	{
		*pCur++ = pCur[-dwWidth] + *pBit0++;
		for (j = 1; j < dwWidth; ++j)
			*pCur++ = pCur[-dwWidth] + pCur[-1] - pCur[-1- dwWidth] + *pBit0++;
	}
}
void CZXDib::GetLocalDeviation(BYTE* pBitmap,int dwHeight,int dwWidth,int nSzH,int nSzW, double*& pDev)
{
	int i,j;
	int nSzH2 = nSzH / 2;
	int nSzW2 = nSzW / 2;

	int nH = dwHeight + nSzH2*2;
	int nW = dwWidth  + nSzW2*2;

	BYTE* pBmp2 = new BYTE[nH*nW];
	for( i=0; i<dwHeight; i++ )
	for( j=0; j<dwWidth; j++ )
		pBmp2[(i+nSzH2)*nW+j+nSzW2] = pBitmap[i*dwWidth+j];
	
	for( i=0; i<dwHeight; i++ )
	for( j=0; j<nSzW2; j++ )
	{
		pBmp2[(i+nSzH2)*nW+     j] = pBitmap[i*dwWidth+dwWidth-1-(nSzW2-1-j)];
		pBmp2[(i+nSzH2)*nW+nW-1-j] = pBitmap[i*dwWidth+(nSzW2-1-j)];
	}
	for( i=0; i<nSzH2; i++ )
	{
		memcpy(pBmp2+i*nW, pBmp2+nSzH2*nW, nW);
		memcpy(pBmp2+(nH-1-i)*nW, pBmp2+(nH-1-nSzH2)*nW, nW);
	}

	double* pdSum = NULL;
	GetInteImg( pBmp2, nH, nW,pdSum );

	double *pdSum2 = NULL;
	GetInteImg2( pBmp2, nH, nW, pdSum2 );

	delete pDev;
	pDev = new double[dwHeight*dwWidth];
	memset(pDev, 0, sizeof(double)*dwHeight*dwWidth);

	
	for( i=nSzH2; i<dwHeight+nSzH2; i++ )
	for( j=nSzW2; j<dwWidth+nSzW2 ; j++ )
	{
		pDev[(i-nSzH2)*dwWidth+j-nSzW2] = pdSum2[(i+nSzH2)*nW+j+nSzW2] 
										- pdSum2[(i-nSzH2)*nW+j+nSzW2] 
										- pdSum2[(i+nSzH2)*nW+j-nSzW2] 
										+ pdSum2[(i-nSzH2)*nW+j-nSzW2];
	}

	delete pdSum;
	delete pdSum2;
}

void CZXDib::GetInteImg2(BYTE* pBitmap,int dwHeight,int dwWidth,double*& pIntImg)
{
	int i,j;
	if( pIntImg )	delete pIntImg;
	pIntImg = new double[dwHeight*dwWidth];
	
	// 第一个元素
	pIntImg[0] = pBitmap[0]*pBitmap[0];
	
	// 第一行
	for( j=1; j<dwWidth ; j++ )
		pIntImg[j] = pIntImg[j-1] + pBitmap[j]*pBitmap[j];
	
	// 后续行
	for( i=1; i<dwHeight; i++ )
	{
		pIntImg[i*dwWidth] = pIntImg[(i-1)*dwWidth] + pBitmap[i*dwWidth]*pBitmap[i*dwWidth];
		
		for( j=1; j<dwWidth ; j++ )
			pIntImg[i*dwWidth+j] = pIntImg[(i-1)*dwWidth+j] + pIntImg[i*dwWidth+j-1] - pIntImg[(i-1)*dwWidth+j-1] + pBitmap[i*dwWidth+j]*pBitmap[i*dwWidth+j];
	}
}
void CZXDib::GetInteImg2(int* pBitmap,int dwHeight,int dwWidth,double*& pIntImg)
{
	int i,j;
	if( pIntImg )	delete pIntImg;
	pIntImg = new double[dwHeight*dwWidth];
	
	// 第一个元素
	pIntImg[0] = pBitmap[0]*pBitmap[0];
	
	// 第一行
	for( j=1; j<dwWidth ; j++ )
		pIntImg[j] = pIntImg[j-1] + pBitmap[j]*pBitmap[j];
	
	// 后续行
	for( i=1; i<dwHeight; i++ )
	{
		pIntImg[i*dwWidth] = pIntImg[(i-1)*dwWidth] + pBitmap[i*dwWidth]*pBitmap[i*dwWidth];
		
		for( j=1; j<dwWidth ; j++ )
			pIntImg[i*dwWidth+j] = pIntImg[(i-1)*dwWidth+j] + pIntImg[i*dwWidth+j-1] - pIntImg[(i-1)*dwWidth+j-1] + pBitmap[i*dwWidth+j]*pBitmap[i*dwWidth+j];
	}
}

void CZXDib::GetInteImg2(BYTE* pBitmap, int dwHeight, int dwWidth, int*& pIntImg)
{
	int i, j;
	if (pIntImg)	delete pIntImg;
	pIntImg = new int[dwHeight*dwWidth];

	// 第一个元素
	pIntImg[0] = pBitmap[0] * pBitmap[0];

	// 第一行
	for (j = 1; j < dwWidth; j++)
		pIntImg[j] = pIntImg[j - 1] + pBitmap[j] * pBitmap[j];

	// 后续行
	for (i = 1; i < dwHeight; i++)
	{
		pIntImg[i*dwWidth] = pIntImg[(i - 1)*dwWidth] + pBitmap[i*dwWidth] * pBitmap[i*dwWidth];

		for (j = 1; j < dwWidth; j++)
			pIntImg[i*dwWidth + j] = pIntImg[(i - 1)*dwWidth + j] + pIntImg[i*dwWidth + j - 1] - pIntImg[(i - 1)*dwWidth + j - 1] + pBitmap[i*dwWidth + j] * pBitmap[i*dwWidth + j];
	}
}

//////////////////////////////////////////////////////////////////////////
void CZXDib::SobelVertical(unsigned char*pBitmap, int dwHeight, int dwWidth, unsigned char** pResult)
{
    int i,j,temp;
	
    if(*pResult) free(*pResult);
    (*pResult) =(unsigned char*) malloc (sizeof(unsigned char)*dwHeight*dwWidth);
    memset(*pResult,0,sizeof(unsigned char)*dwHeight*dwWidth);
    
    
    for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1;j++)
    {
        temp =  (int)(pBitmap[j-1+(i+1)*dwWidth])
            + 2*(int)(pBitmap[j  +(i+1)*dwWidth])
            +   (int)(pBitmap[j+1+(i+1)*dwWidth])
            -   (int)(pBitmap[j-1+(i-1)*dwWidth])
            - 2*(int)(pBitmap[j  +(i-1)*dwWidth])
            -   (int)(pBitmap[j+1+(i-1)*dwWidth]);
        
        temp/=4;
        
        if(temp<0) temp = 0- temp;
        if(temp>255) temp = 255; 
        
        (*pResult)[i*dwWidth+j] = abs(temp);
        
    }
    
	return ;
}
void CZXDib::PriwittH(unsigned char *pBitmap, int dwHeight, int dwWidth,int nSizeX, int nSizeY, unsigned char **pResult)
{
    int i,j,m,n,nTemp,k;
	
    if(!pResult)
    {
        if((*pResult)) delete(*pResult);
        (*pResult) = 0;
        return ;
    }
    if((*pResult)) delete(*pResult);
    (*pResult) = new unsigned char[dwHeight*dwWidth];
    memset(*pResult,0,sizeof(unsigned char)*dwHeight*dwWidth);

	int *pnTemp = new int[dwHeight*dwWidth];
    
	double nMax=-99999999,nMin=99999999;
	for( i=nSizeY; i<dwHeight-nSizeY-1; i++ )
	for( j=nSizeX; j<dwWidth -nSizeX-1; j++ )
	{
		int nH = 0;
		for( n=-nSizeY; n<=nSizeY; n++ )
		for( m=0;       m<=nSizeX; m++ )		
		{
			nH += pBitmap[(i-n)*dwWidth+j+m];
			nH -= pBitmap[(i-n)*dwWidth+j-m];
		}
		
		if( nH < 0 ) nH = 0;
		nH = abs(nH);
		pnTemp[i*dwWidth+j] = nH ;
		nMin = min( nMin, nH);
		nMax = max( nMax, nH);
	}
	for( k=0; k<dwHeight*dwWidth; k++ )
		(*pResult)[k] = ( pnTemp[k] - nMin ) * 255 / ( nMax - nMin );		
    
	delete pnTemp;
	
	return ;
}
void CZXDib::SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, unsigned char **pResult)
{
    int i,j,nTemp;
	
    if(!pResult)
    {
        if((*pResult)) free(*pResult);
        (*pResult) = 0;
        return ;
    }
    if((*pResult)) free(*pResult);
    (*pResult) = (unsigned char *) malloc(sizeof(unsigned char)*dwHeight*dwWidth);
    memset(*pResult,0,sizeof(unsigned char)*dwHeight*dwWidth);
    
    for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1;j++)
	{
		nTemp = (int)(pBitmap[ j+1 + (i-1)*dwWidth])
			+ 2*(int)(pBitmap[ j+1 +     i*dwWidth])
			+   (int)(pBitmap[ j+1 + (i+1)*dwWidth])
			-   (int)(pBitmap[ j-1 + (i-1)*dwWidth])
			- 2*(int)(pBitmap[ j-1 +     i*dwWidth])
			-   (int)(pBitmap[ j-1 + (i+1)*dwWidth]);
		
		nTemp /= 2;
		if (nTemp < 0) nTemp = 0;// -nTemp;
		else if(nTemp>255) nTemp = 255; 
		
		(*pResult)[i*dwWidth+j] = nTemp;            
	}
    
	return ;
}
void CZXDib::SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, int nFilterSize, int **pResult)
{
    int i,j,m,nTemp;
	
    if(!pResult)
        return ;
    
    if(*pResult) delete(*pResult);
    (*pResult) = new int [dwHeight*dwWidth];
	memset(*pResult, 0, sizeof(int)* dwHeight*dwWidth);
    
    for(i=0;i<dwHeight;i++)
	for(j=nFilterSize;j<dwWidth-nFilterSize;j++)
	{
		nTemp = 0;
		for( m=1; m<nFilterSize; m++ )
			nTemp += pBitmap[ j+m + i*dwWidth] - pBitmap[ j-m + i*dwWidth];

		(*pResult)[i*dwWidth+j] = nTemp;            
	}    
}

void CZXDib::SobelHorizon(unsigned char *pBitmap, int dwHeight, int dwWidth, int **pResult)
{
    int i,j,nTemp;

    if((*pResult)) delete(*pResult);
    (*pResult) = (int *) malloc(sizeof(int)*dwHeight*dwWidth);
    memset(*pResult,0,sizeof(int)*dwHeight*dwWidth);
    
    for(i=1;i<dwHeight-1;i++)
	for(j=1;j<dwWidth-1;j++)
	{
		(*pResult)[i*dwWidth+j] = (int)(pBitmap[ j+1 + (i-1)*dwWidth])
								+ 2*(int)(pBitmap[ j+1 +     i*dwWidth])
								+   (int)(pBitmap[ j+1 + (i+1)*dwWidth])
								-   (int)(pBitmap[ j-1 + (i-1)*dwWidth])
								- 2*(int)(pBitmap[ j-1 +     i*dwWidth])
								-   (int)(pBitmap[ j-1 + (i+1)*dwWidth]);
							
	}
    
	return ;
}

void CZXDib::GetHSIFromRGB(BYTE* pColorImg,DWORD dwHeight,DWORD dwWidth,BYTE*& pH,BYTE*& pS,BYTE *& pI )
{
	long nSize = dwHeight*dwWidth;
	double *pdH = new double[nSize];
	double *pdS = new double[nSize];
	double *pdI = new double[nSize];
	
	for( long i=0; i<nSize; i++ )
		GetHSIFromRGB(pColorImg[i*3+2],pColorImg[i*3+1],pColorImg[i*3+0],pdH[i],pdS[i],pdI[i]);
	
	RegularToGrayLevel(pdH,dwHeight,dwWidth,pH);
	RegularToGrayLevel(pdS,dwHeight,dwWidth,pS);
	RegularToGrayLevel(pdI,dwHeight,dwWidth,pI);
	
	delete pdH;
	delete pdS;
	delete pdI;
}
void CZXDib::MorphByMask(BYTE* pImage,int nHeight,int nWidth,BYTE* pMask,int nMskHeight, int nMskWidth)
{
	BYTE* pMin = new BYTE[nHeight*nWidth];
	memset( pMin, 0, nHeight*nWidth );
	
	long nHalfH = nMskHeight / 2;
	long nHalfW = nMskWidth  / 2;
	
	long i, j;
	for( i=nHalfH; i<nHeight-nHalfH; i++ )
	for( j=nHalfW; j<nWidth -nHalfW; j++ )
	{
		long nMin = 255;
		for( long m = 0; m<nMskHeight; m++ )
		for( long n = 0; n<nMskWidth;  n++ )
		{
			if( !pMask[m*nMskWidth+n] )
				continue;

			if( pImage[(i+m-nHalfH)*nWidth+j+n-nHalfW] < nMin )
				nMin = pImage[(i+m-nHalfH)*nWidth+j+n-nHalfW];
		}
		pMin[i*nWidth+j] = nMin;
	}

	for(      i=nHalfH; i<nHeight-nHalfH; i++ )
	for( long j=nHalfW; j<nWidth -nHalfW; j++ )
	{
		long nMax = 0;
		for( long m = 0; m<nMskHeight; m++ )
		for( long n = 0; n<nMskWidth;  n++ )
		{
			if( !pMask[m*nMskWidth+n] )
				continue;
			if( pMin[(i+m-nHalfH)*nWidth+j+n-nHalfW] > nMax )
				nMax = pMin[(i+m-nHalfH)*nWidth+j+n-nHalfW];
		}
		if( pImage[(i)*nWidth+j] < nMax )
			nMax = pImage[(i)*nWidth+j];
		pImage[(i)*nWidth+j] -= nMax;
		//pImage[i*nWidth+j] = pMin[i*nWidth+j];
	}

	// 周边抹0
	for(    i = 0; i<nMskHeight; i++ )
	for(int x = 0; x<nWidth; x++ )
	{
		pImage[(          i)*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for(    i = nMskHeight; i<nHeight-nMskHeight; i++ )
	for(int x = 0;      x<nMskWidth;		  x++ )
	{  
		pImage[i*nWidth         +x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    

	delete pMin;
	pMin = NULL;
}
// 形态学滤波
void CZXDib::MorphH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		//pImage[i] -= pTemp2[i];
		pImage[i] = pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
		for( x = 0; x<nFilterWidth; x++)
		{  
			pImage[i*nWidth+x] = 0;
			pImage[i*nWidth+nWidth-1-x] = 0;
		}    
		
		delete pTemp1;
		delete pTemp2;
}
// 形态学滤波
void CZXDib::MorphH2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		//pImage[i] -= pTemp2[i];
		pImage[i] -= pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}

// 形态学滤波
void CZXDib::MorphH3(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		//pImage[i] -= pTemp2[i];
		pImage[i] = pTemp1[i];
	
	delete pTemp1;
	delete pTemp2;
}


// 形态学滤波    
void CZXDib::MorphV2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];

	BYTE* pTemp2 = new BYTE[nSize];
	memset(pTemp1,0,nSize);
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp2[i];// - pImage[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	delete pTemp2;
	delete pTemp1;
}
void CZXDib::ErosionV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	memset(pTemp1,0,nSize);
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}	
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp1[i];// - pImage[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	delete pTemp1;
}
// 形态学滤波    
void CZXDib::DialtionV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	memset(pTemp1,0,nSize);
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp1[i];// - pImage[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	delete pTemp1;
}


// 形态学滤波    
void CZXDib::MorphV(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp2 = new BYTE[nSize];	
	BYTE* pTemp1 = new BYTE[nSize];
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] -= pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	delete pTemp2;
	delete pTemp1;
}
// 形态学滤波    
void CZXDib::Morph2V(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 周边抹0	
	for( i=0; i<nFilterWidth; i++ )
	{
		memset(pTemp1+i*nWidth,			   0,nWidth);
		memset(pTemp1+(nHeight-1-i)*nWidth,0,nWidth);
	} 
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = min(pTemp2[y*nWidth+x],nTemp);
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}
// 形态学滤波    
void CZXDib::Morph3(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 周边抹0	
	for( i=0; i<nFilterWidth; i++ )
	{
		memset(pTemp1+i*nWidth,			   0,nWidth);
		memset(pTemp1+(nHeight-1-i)*nWidth,0,nWidth);
	} 
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = min(pTemp2[y*nWidth+x],nTemp);
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] -= pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}
// 形态学滤波    
void CZXDib::Morph(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
                                            
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 周边抹0	
	for( i=0; i<nFilterWidth; i++ )
	{
		memset(pTemp1+i*nWidth,			   0,nWidth);
		memset(pTemp1+(nHeight-1-i)*nWidth,0,nWidth);
	} 
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = min(pTemp2[y*nWidth+x],nTemp);
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}
// 形态学滤波    
void CZXDib::Morph4(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 0;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = max(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 周边抹0	
	for( i=0; i<nFilterWidth; i++ )
	{
		memset(pTemp1+i*nWidth,			   0,nWidth);
		memset(pTemp1+(nHeight-1-i)*nWidth,0,nWidth);
	} 
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = min(pTemp2[y*nWidth+x],nTemp);
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp2[i];// - pImage[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}
// 形态学滤波    
void CZXDib::ErosionH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp1[i];// - pImage[i];
	
	delete pTemp1;
}
void CZXDib::Morph(int* pnX, int nW, int nSize)
{
	int i, j;
	int nS2 = nSize * 2;
	int* pnY1 = new int[nW + nS2 * 2];
	int* pnY2 = new int[nW + nS2 * 2];
	for (i = 0; i < nS2; i++)
	{
		pnY1[i] = pnX[nS2 - i];
		pnY1[nW+i+nS2] = pnX[nW - 2 - i];
	}
	memcpy(pnY1 + nS2, pnX, nW * sizeof(int)) ;
	memset(pnY2, 0, sizeof(int)*(nW + nS2 * 2));
	
	
	pnY1 += nS2;
	pnY2 += nS2;
	for (j = -nSize; j < nW+ nSize; j++)
	{
		int nTemp = pnY1[j];
		for (i = -nSize; i <= nSize; i++)
			nTemp = min(nTemp, pnY1[i+j]);
		pnY2[j] = nTemp;
	}

	for (j = 0; j < nW; j++)
	{
		int nTemp = 0;
		for (i = -nSize; i <= nSize; i++)
			nTemp = max(nTemp, pnY2[i + j]);
		pnX[j] -= nTemp;
	}
	pnY1 -= nS2;
	pnY2 -= nS2;
	delete pnY1;
	delete pnY2;
}
// 形态学滤波    
void CZXDib::DialtionH(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 0;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = max(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] = pTemp1[i];// - pImage[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  	
	
	delete pTemp1;
}

CString CZXDib::GetAppDirectory()
{
	CString pathName = _T("");
	HMODULE hModule = ::GetModuleHandle(::AfxGetApp()->m_pszAppName);
	::GetModuleFileName(hModule, pathName.GetBuffer(_MAX_PATH), _MAX_PATH);
	pathName.ReleaseBuffer();
	pathName = pathName.Left(pathName.ReverseFind('\\')) + "\\";

	return pathName;
}
// 形态学滤波    
void CZXDib::Morph2(BYTE* pImage,int nHeight,int nWidth,int nFilterWidth)
{
	int x,y,i;
	BYTE nTemp;
	int nSize = nHeight * nWidth;
	
	BYTE* pTemp1 = new BYTE[nSize];
	BYTE* pTemp2 = new BYTE[nSize];	
	
	memset( pTemp1, 				   0, nFilterWidth );
	memset( pTemp1+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth ; x < nSize-nFilterWidth; x ++ )
	{
		nTemp = 255;
		for( i=-nFilterWidth; i<=nFilterWidth; i++ )
			nTemp = min(nTemp,pImage[x+i]);
		
		pTemp1[x] = nTemp;
	}
	
	memset( pTemp2, 				   0, nFilterWidth );
	memset( pTemp2+nSize-nFilterWidth, 0, nFilterWidth );
	
	for( x = nFilterWidth; x < nSize-nFilterWidth; x ++)
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(nTemp,pTemp1[x+i]);
		
		pTemp2[x] = nTemp;
	}
	
	// 周边抹0	
	for( i=0; i<nFilterWidth; i++ )
	{
		memset(pTemp1+i*nWidth,			   0,nWidth);
		memset(pTemp1+(nHeight-1-i)*nWidth,0,nWidth);
	} 
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++)
	{
		nTemp = 255;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = min(pImage[(y+i)*nWidth+x],nTemp);
		
		pTemp1[y*nWidth+x] = nTemp;
	}				
	
	for( x = 0; x < nWidth; x ++ )
	for( y = nFilterWidth; y < nHeight-nFilterWidth; y ++ )
	{
		nTemp = 0;
		for(i=-nFilterWidth;i<=nFilterWidth;i++)
			nTemp = max(pTemp1[(y+i)*nWidth+x],nTemp);
		
		pTemp2[y*nWidth+x] = max(pTemp2[y*nWidth+x],nTemp);
	}
	
	// 与原图像做减法
	for( i=0; i<nSize; i++ )
		pImage[i] -= pTemp2[i];
	
	// 周边抹0
	for( i=0; i<nFilterWidth; i++ )
	for( x = 0; x<nWidth; x++)
	{
		pImage[i*nWidth+x] = 0;
		pImage[(nHeight-1-i)*nWidth+x] = 0;
	}  
	
	for( i=nFilterWidth; i<nHeight-nFilterWidth; i++ )
	for( x = 0; x<nFilterWidth; x++)
	{  
		pImage[i*nWidth+x] = 0;
		pImage[i*nWidth+nWidth-1-x] = 0;
	}    
	
	delete pTemp1;
	delete pTemp2;
}
void CZXDib::SegmentByCorrelation(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth)
{
	DWORD pH[256];
	memset(pH,0,sizeof(DWORD)*256);

	DWORD dwSize = dwHeight*dwWidth;
	long i; for( i=0; i<dwSize; i++ )
		pH[pBitmap[i]] ++;

	long   nGate = 0 ;
	double dMax = 0;
	for( i=0; i<255; i++ )
	{
		double dMean1 = 0;
		DWORD  dwCount = 0;
		long j;
		for( j=0; j<i; j++ )
		{
			dMean1  += pH[j] * j;
			dwCount += pH[j];
		}
		if( !dwCount ) continue;
		dMean1 /= dwCount;

		double dMean2 = 0;
		dwCount  = 0 ;
		for( j; j<255; j++ )
		{
			dMean2 += pH[j] * j;
			dwCount+= pH[j];
		}
		if( !dwCount ) continue;
		dMean2 /= dwCount;

		double dTemp = dMean1 + dMean2;
		dTemp = (dMean2 - dMean1 ) / dTemp;

		if( dTemp <= dMax ) continue;

		dMax = dTemp;
		nGate = i;
	}

	for( i=0; i<dwSize; i++ )
		pBitmap[i] = pBitmap[i] > nGate ? 255 : 0;
}/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL CZXDib::Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth)
{	
	// 循环变量
	LONG	i,j,nSizeResult;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	long w, h, wp, hp;
	for(wp=0,w=1; w<dwWidth;  wp++ ) w*=2;	
	for(hp=0,h=1; h<dwHeight; hp++ ) h*=2;

	nSizeResult = w * h;
	
	BYTE* pTemp = new BYTE[nSizeResult];
	memset(pTemp,0,sizeof(BYTE)*nSizeResult);
	
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
		pTemp[i*w+j] = lpDIBBits[i*dwWidth+j];

			
	// 分配内存
	complex<double> *TD = new complex<double>[nSizeResult];
	complex<double> *FD = new complex<double>[nSizeResult];
	
	// 行
	for(i = 0; i < nSizeResult; i++)
		TD[i] = complex<double>(pTemp[i], 0);

	// 对y方向进行快速付立叶变换
	for(i = 0; i < h; i++)		
		FFT(&TD[w * i], &FD[w * i], wp);
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
	{
		TD[i + h * j] = FD[j + w * i];
	}

	// 对x方向进行快速付立叶变换
	for(i = 0; i < w; i++)	
		FFT(&TD[i * h], &FD[i * h], hp);
	

	
	/////////=====频谱幅度结果显示: 将保存在FD中的复数转化为实数显示出来===========///
	// 行
	double * pdTemp = new double[w*h];

	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)// 列
	{
		// 计算频谱fabs(FD[j * h + i].real() / FD[j * h + i].imag())*100;////
		double dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				            FD[j * h + i].imag() * FD[j * h + i].imag());

		pdTemp[ w *	((i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2)] = log(1+dTemp);
		
		dTemp /= 1000;

		//dTemp = fabs(FD[j * h + i].real() / 100);
		// 判断是否超过255
		if (dTemp > 255)
		{
			// 对于超过的，直接设置为255
			dTemp = 255;
		}
		
		// 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个象素的指针
		// 此处不直接取i和j，是为了将变换后的原点移到中心
		//lpSrc = (unsigned char*)lpDIBBits + w * (lHeight - 1 - i) + j;

		// 更新源图像
		pTemp[ w *	((i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2)] = (BYTE)(dTemp);
	}
	
	// 删除临时变量
	delete TD;	TD = NULL;
	delete FD;	FD = NULL;

	//计算偏移地址
	int nOffsetX,nOffsetY;
	nOffsetX = (w - dwWidth )/2;
	nOffsetY = (h - dwHeight)/2;

	//RegularToGrayLevel(pdTemp,h,w,pTemp);
	delete pdTemp;

	//将结果再恢复原始大小,拷贝出来：
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
		lpDIBBits[i*dwWidth+j] = pTemp[(i+nOffsetY)*w+j+nOffsetX] ;

	delete pTemp; pTemp = NULL;
	
	// 返回
	return TRUE;
}
/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL CZXDib::Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth,double*& pdResult)
{	
	// 循环变量
	LONG	i,j,nSizeResult;
	
	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	long wp, hp, w, h;
	for( wp=0,w=1; w<dwWidth;  wp++ ) w*=2;	
	for( hp=0,h=1; h<dwHeight; hp++ ) h*=2;

	nSizeResult = w * h;
	
	BYTE* pTemp = new BYTE[nSizeResult];
	memset(pTemp,0,sizeof(BYTE)*nSizeResult);
	
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
		pTemp[i*w+j] = lpDIBBits[i*dwWidth+j];

			
	// 分配内存
	complex<double> *TD = new complex<double>[nSizeResult];
	complex<double> *FD = new complex<double>[nSizeResult];
	
	// 行
	for(i = 0; i < nSizeResult; i++)
		TD[i] = complex<double>(pTemp[i], 0);

	// 对y方向进行快速付立叶变换
	for(i = 0; i < h; i++)		
		FFT(&TD[w * i], &FD[w * i], wp);
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
	{
		TD[i + h * j] = FD[j + w * i];
	}

	// 对x方向进行快速付立叶变换
	for(i = 0; i < w; i++)	
		FFT(&TD[i * h], &FD[i * h], hp);
	

	
	/////////=====频谱幅度结果显示: 将保存在FD中的复数转化为实数显示出来===========///
	// 行
	double* pdTemp = new double[nSizeResult];
	memset(pdTemp,0,sizeof(double)*nSizeResult);
	
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)// 列
	{
		// 计算频谱fabs(FD[j * h + i].real() / FD[j * h + i].imag())*100;////
		double dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() + 
				            FD[j * h + i].imag() * FD[j * h + i].imag());

		pdTemp[ w *	((i<h/2 ? i+h/2 : i-h/2)) + (j<w/2 ? j+w/2 : j-w/2)] = dTemp;
	}
	
	// 删除临时变量
	delete TD;	TD = NULL;
	delete FD;	FD = NULL;

	//计算偏移地址
	int nOffsetX,nOffsetY;
	nOffsetX = (w - dwWidth )/2;
	nOffsetY = (h - dwHeight)/2;

	if( pdResult ) delete pdResult;
	pdResult = new double[dwHeight*dwWidth];

	//将结果再恢复原始大小,拷贝出来：
	for(i=0;i<dwHeight;i++)
	for(j=0;j<dwWidth; j++)
		pdResult[i*dwWidth+j] = pdTemp[(i+nOffsetY)*w+j+nOffsetX] ;

	delete pTemp; pTemp = NULL;
	delete pdTemp;pdTemp= NULL;
	
	// 返回
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL CZXDib::IFourier(complex<double> *TInput, DWORD dwHeight, DWORD dwWidth,BYTE*& pResult,bool bRegular)
{
	DWORD i,j,w,h,wp,hp;
	if( pResult ) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	DWORD dwSize = dwHeight*dwWidth;

	// 计算进行付立叶变换的宽度和高度（2的整数次方）
	for( wp=0,w=1; w<dwWidth;  wp++ ) w*=2;	
	for( hp=0,h=1; h<dwHeight; hp++ ) h*=2;

	if( h != dwHeight || w != dwWidth ) return FALSE;

	complex<double> *TD = new complex<double>[dwSize];
	complex<double> *TF = new complex<double>[dwSize];

	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
		TF[j + w * i] = TInput[i + h * j];

	// 对y方向进行快速付立叶变换
	for( i=0; i<w; i++ )
		IFFT(&TF[i * h], &TD[i * h], hp);
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
		TF[j + w * i] = TD[i + h * j];

	// 对x方向进行快速付立叶变换
	for(i = 0; i < h; i++)		
		IFFT(&TF[i * w], &TD[i * w], wp);

	double dMin = 255;
	double dMax = 0;
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
	{
		// 更新源图像
		double dTemp = TD[i*w+j].real();
		dMin = min(dMin,dTemp);
		dMax = max(dMax,dTemp);
		pResult[i*w+j] = dTemp > 255 ? 255 : (dTemp < 0 ? 0 : dTemp);
	}

	if( bRegular )
	{
		for(i = 0; i < h; i++)
		for(j = 0; j < w; j++)
		{
			double dTemp = TD[i*w+j].real();
			pResult[i*w+j] = (dTemp-dMin)*255/(dMax-dMin);
		}
	}

	delete TF;
	delete TD;
	return TRUE;
}

/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
BOOL CZXDib::Fourier(BYTE* lpDIBBits, DWORD dwHeight, DWORD dwWidth,complex<double> *&TResult,DWORD &dwResultHeight,DWORD& dwResultWidth)
{
	// 循环变量
	LONG	i,j;
	long	nResultSize;

	// 进行付立叶变换的宽度和高度（2的整数次方）	
	int wp, hp, w, h;
	for( wp=0,w=1; w<dwWidth;  wp++ ) w*=2;	
	for( hp=0,h=1; h<dwHeight; hp++ ) h*=2;

	nResultSize = w*h;
	
	BYTE* pTemp = new BYTE[nResultSize];
	memset(pTemp,0,sizeof(BYTE)*nResultSize);
	
	//for(i=0;i<h; i++)
	//for(j=0;j<w; j++)
	for(i=0;i<dwHeight; i++)
	for(j=0;j<dwWidth; j++)
	{
		long nY = i < dwHeight ? i : 2*dwHeight - i - 1;
		long nX = j < dwWidth  ? j : 2*dwWidth  - j - 1;
		pTemp[i*w+j] = lpDIBBits[nY*dwWidth+nX];
	}
			
	// 分配内存
	complex<double> *TD = new complex<double>[nResultSize];
	complex<double> *FD = new complex<double>[nResultSize];
	
	// 行
	for(i = 0; i < nResultSize; i++)
		TD[i] = complex<double>(pTemp[i], 0);
	
	// 对y方向进行快速付立叶变换
	for(i = 0; i < h; i++)		
		FFT(&TD[w * i], &FD[w * i], wp);
	
	// 保存变换结果
	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
		TD[i + h * j] = FD[j + w * i];
	
	// 对x方向进行快速付立叶变换
	for(i = 0; i < w; i++)
		FFT(&TD[i * h], &FD[i * h], hp);
	
	if( TResult ) delete TResult;
	TResult = new complex<double>[w * h];

	for(i = 0; i < h; i++)
	for(j = 0; j < w; j++)
		TResult[i + h * j] = FD[j + w * i];
		
	// 删除临时变量
	delete TD;		TD	= NULL;
	delete FD;		FD	= NULL;
	delete pTemp;	pTemp = NULL;

	dwResultHeight = h;
	dwResultWidth  = w;
	
	// 返回
	return TRUE;
}
/*************************************************************************
 *
 * 函数名称：
 *   FFT()
 *
 * 参数:
 *   complex<double> * TD	- 指向时域数组的指针
 *   complex<double> * FD	- 指向频域数组的指针
 *   r						－2的幂数，即迭代次数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶变换。
 *
 ************************************************************************/
void CZXDib::FFT(complex<double> * TD, complex<double> * FD, int r)
{

	double PI = 3.1415926;
	
	// 付立叶变换点数
	LONG	count;
	
	// 循环变量
	int		i,j,k;
	
	// 中间变量
	int		bfsize,p;
	
	// 角度
	double	angle;
	
	complex<double> *W,*X1,*X2,*X;
	
	// 计算付立叶变换点数
	count = 1 << r;
	
	// 分配运算所需存储器
	W  = new complex<double>[count / 2];
	X1 = new complex<double>[count];
	X2 = new complex<double>[count];
	
	// 计算加权系数
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	
	// 将时域点写入X1
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换
	for(k = 0; k < r; k++)
	{
		for(j = 0; j < 1 << k; j++)
		{
			bfsize = 1 << (r-k);
			for(i = 0; i < bfsize / 2; i++)
			{
				p = j * bfsize;
				X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
				X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1<<k)];
			}
		}
		X  = X1;
		X1 = X2;
		X2 = X;
	}
	
	// 重新排序
	for(j = 0; j < count; j++)
	{
		p = 0;
		for(i = 0; i < r; i++)
		{
			if (j&(1<<i))
			{
				p+=1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}
/*************************************************************************
 *
 * 函数名称：
 *   IFFT()
 *
 * 参数:
 *   complex<double> * FD	- 指向频域值的指针
 *   complex<double> * TD	- 指向时域值的指针
 *   r						－2的幂数
 *
 * 返回值:
 *   无。
 *
 * 说明:
 *   该函数用来实现快速付立叶反变换。
 *
 ************************************************************************/
void CZXDib::IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	double PI = 3.1415926;

	// 付立叶变换点数
	LONG	count;
	
	// 循环变量
	int		i;
	
	complex<double> *X;
	
	// 计算付立叶变换点数
	count = 1 << r;
	
	// 分配运算所需存储器
	X = new complex<double>[count];
	
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(i = 0; i < count; i++)
	{
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	}
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
	{
		TD[i] = complex<double> (TD[i].real() / count, -TD[i].imag() / count);
	}
	
	// 释放内存
	delete X;
}
void CZXDib::AppandImage(BYTE* pBitmap,DWORD dwHeight, DWORD dwWidth,DWORD dwResultHeight,DWORD dwResultWidth,BYTE*& pReslt)
{
	if( pReslt ) delete pReslt;
	pReslt = new BYTE[dwResultHeight*dwResultWidth];
	memset(pReslt,0,sizeof(BYTE)*dwResultHeight*dwResultWidth);

	for( DWORD i=0; i<dwResultHeight && i<dwHeight; i ++ )
	for( DWORD j=0; j<dwResultWidth  && j<dwWidth ; j ++ )
	{
		if( i>=dwHeight || j >=dwWidth ) continue;
		pReslt[(i)*dwResultWidth+j] = pBitmap[i*dwWidth+j];
	}
}
void CZXDib::UnwrapImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nOffsetX,long nOffsetY,BYTE*& pResult)
{
	if( pResult ) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);

	for( long i=0; i<dwHeight; i++ )
	for( long j=0; j<dwWidth ; j++ )
	{
		long nX = j + nOffsetX;
		long nY = i + nOffsetY;
		if( nX < 0 || nX >= dwWidth || nY < 0 || nY >= dwHeight ) continue;

		pResult[i*dwWidth+j] = pBitmap[nY*dwWidth+nX];
	}
}
void CZXDib::SPOMF(complex<double> *pImage,complex<double> *pTemplate,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult)
{
	DWORD dwSize = dwHeight * dwWidth;
	complex<double> *pTemp = new complex<double>[dwSize];

	for( DWORD i=0; i<dwSize; i++ )
	{
		double a = pImage   [i].real();
		double b = pImage   [i].imag();
		double c = pTemplate[i].real();
		double d = pTemplate[i].imag();

		double dReal  = a*c + b*d;
		double dImage = b*c - a*d;

		double dTemp1 = 1.0 / sqrt((c*c+d*d)*(a*a+b*b));		
			   //dTemp1 = 1.0 /  (c*c+d*d);
			   //dTemp1 = 1.0 /  (dReal*dReal+dImage*dImage);

		pTemp[i] = complex<double> (dReal*dTemp1,dImage*dTemp1);
	}

	IFourier(pTemp, dwHeight,dwWidth,pResult,true);

	delete pTemp;	
}

void CZXDib::GetBand(BYTE* pColorImg,DWORD dwHeight,DWORD dwWidth,BYTE*& pBitmap,int nBand)
{
	if( nBand < 0 || nBand > 2 ) return;

	if( pBitmap ) delete pBitmap;
	pBitmap = new BYTE[dwHeight*dwWidth];
	for( long i=0; i<dwHeight*dwWidth; i++ )
		pBitmap[i] = pColorImg[i*3+nBand];

	return;
}
void CZXDib::Polar(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE*& pResult)
{	
	delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	memset(pResult,0,sizeof(BYTE)*dwHeight*dwWidth);
	
	for( long i=0; i<dwHeight; i++ )
	for( long j=0; j<dwWidth;  j++ )
	{
		double dRou   = i * dwHeight / (double)dwHeight;
		double dAngle = j * 2 * 3.1415926 / (double)dwWidth;

		double x = dRou * cos(dAngle) + dwWidth / 2.0;
		double y = dRou * sin(dAngle) + dwHeight/ 2.0;

		if( x<0 || y<0 || x > dwWidth-1 || y > dwHeight-1 )
			continue;
		
		//抽样：在2*2临域内采用内插法：
		int position = (int)(y) * dwWidth + (int)(x);
		/////采集该点处的灰度值
		int temp  = pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x);
		position += dwWidth;
		temp -= (temp-(pBitmap[position] - (pBitmap[position] - pBitmap[position+1])* (x-(int)x)))*(y-(int)y);
		
		pResult[i*dwWidth+j] = (BYTE) (temp+0.5);
	}	
}
/****************************************************************************
功能: 生成高斯噪声方法一
形参: dMean  : 均值
dSigma : 标准差	  
返回: 高斯噪声
****************************************************************************/
double CZXDib::GaussNoise(double dMean, double dSigma)
{
	int u1, u2;
	double v1, v2, sum;
	double dTemp;
	static int nFirstFlag = 1;
	
	if(nFirstFlag)  //第一次运行时先初始化随机种子
	{
		srand((unsigned)time(NULL));
		nFirstFlag = 0;
	}
	
	do
	{
		u1 = rand();
		u2 = rand();
		v1 = 2.0 * u1 / RAND_MAX - 1;
		v2 = 2.0 * u2 / RAND_MAX - 1;
		sum = v1 * v1 + v2 * v2;
	}while(sum >= 1);
	
	//生成标准高斯分布
	dTemp = v1 * sqrt(-2 * log(sum) / sum);
	
	dTemp = dTemp * dSigma + dMean;
	
	return dTemp;
}
double CZXDib::GetHuai(double d)
{
	BOOL bFlag = d > 0;
	
	d = fabs(d*100);
	
	int nd = (int)d;
	double dRatio = d - nd;
	double dResult;
	if( nd > 308 )
	{
		dResult = 1.0;
	}
	else
	{
		dResult = g_pNormal[nd]*(1-dRatio) + g_pNormal[nd+1]*dRatio;
	}
	
	if( !bFlag )
		dResult = 1 - dResult;
	
	return dResult;
}
double CZXDib::SegmentByKSigma(BYTE* pImage, int nHeight, int nWidth, double dK)
{
	int nSize = nHeight * nWidth;
	double dMean,dDeviation;
	dMean = dDeviation = 0;
	long i; for( i=0; i<nSize; i++ )
	{
		dMean += pImage[i];
		dDeviation += pImage[i]*(int)(pImage[i]);
	}
	dMean /= nSize;
	dDeviation = sqrt(dDeviation/nSize-dMean*dMean);

	double dGate = dMean + dK * dDeviation;

	for( i=0; i<nSize; i++ )
		pImage[i] = pImage[i] > dGate ? 255 : 0;
	return dGate;
}

//////////////////////////////////////////////////////////////////////////
//多目标 ，返回各个目标的位置
//输入 nFilterWidth   形态滤波宽度
//输入 areaSize   邻域大小
//输入 num   目标数
//返回 pos   目标位置
//返回 snr   每个目标的信噪比
//////////////////////////////////////////////////////////////////////////
bool CZXDib::MultitargetCode(BYTE *pBitmap,int dwHeight,int dwWidth,int areaSize,int num,POINT*& ptPos)
{
	long x,y;

	//由高到低灰度排序，统计前若干个象素位置
	int *gray = new int[num]; //保存象素值
	memset(gray,0,sizeof(int)*num);

	int * pos = NULL;
	pos = new int[num*2]; //保存象素位置
	memset(pos,0,sizeof(int)*num*2);

	if( ptPos ) delete ptPos;
	ptPos = new POINT[num];
	memset(ptPos,0,sizeof(POINT)*num);

	int *codeOffset = new int[num]; //保存偏移量
	memset(codeOffset,0,sizeof(int)*num);

	int *grayTemp = new int[num]; //保存象素值
	memset(grayTemp,0,sizeof(int)*num);

	int *posTemp = new int[num*2]; //保存象素位置
	memset(posTemp,0,sizeof(int)*num*2);

	int *grayCode = new int[num];
	memset(grayCode,0,sizeof(int)*num);
	int *posCode = new int[num];
	memset(posCode,0,sizeof(int)*num);
	
	int i = 0;
	int j = 0;
	for (y=1; y<dwHeight; y++)
	{
		for (x=1; x<dwWidth; x++)
		{
			int k = pBitmap[y*dwWidth+x];
			//先进行比较得出 码
			for (i=0; i<num; i++)
			{
				if (k > gray[i]) grayCode[i] = 1; //1表示大于，0小于
				else grayCode[i] = 0;
			}
			for (i=0; i<num; i++)
			{
				if (abs(y-pos[i*2])<=areaSize && abs(x-pos[i*2+1])<=areaSize) posCode[i] = 1; //1在邻域，0非
				else posCode[i] = 0;
			} 
			
			//计算偏移量
			for (i=0; i<num; i++)
			{
				codeOffset[i] = 0;
				for (j=0; j<i; j++)
					codeOffset[i] += posCode[j];
			}
			//用灰度码减去偏移量
			for (i=0; i<num; i++)
			{
				codeOffset[i] = grayCode[i] - codeOffset[i];
			}
		
			//grayCode差分，替换为1的那个位置
			if (grayCode[0] == 1)
			{
				grayTemp[0] = k; //replace
				posTemp[0] = y;
				posTemp[1] = x;
			}
			else //grayCode[0] = 0
			{
				if (posCode[0] == 0) 
				{
					for (i=1; i<num; i++)
					{
						if((~grayCode[i]&posCode[i])==1)
							break;
						if (grayCode[i] - grayCode[i-1] == 1)
						{
							grayTemp[i] = k; //replace
							posTemp[i*2] = y;
							posTemp[i*2+1] = x;
							break;
						}
					}
				}
			}
			int flag = 0;
			for (i=0; i<num; i++)
			{
				flag += ~grayCode[i]&posCode[i];
			}
			if (flag < 1)
			//根据偏移量进行移位
			{
				for (i=0; i<num; i++)
				{
					if (grayCode[i] == 0)// && grayTemp[i]==1)
					{
						grayTemp[i] = gray[i];
						posTemp[i*2] = pos[i*2];
						posTemp[i*2+1] = pos[i*2+1];
					}
					
					else if (grayCode[i] == 1)
					{
						if (posCode[i] == 1)
							continue;
						else if (posCode[i] ==0)
						{
							int rrr = i+codeOffset[i];
							if (rrr>=0 && rrr<num)
							{
								grayTemp[i+codeOffset[i]] = gray[i];
								posTemp[(i+codeOffset[i])*2] = pos[i*2];
								posTemp[(i+codeOffset[i])*2+1] = pos[i*2+1];	
							}
						}
					}
				}

				//复制数据
				for (i=0; i<num; i++)
				{
					gray[i] = grayTemp[i];
					pos[i*2] = posTemp[i*2];
					pos[i*2+1] = posTemp[i*2+1];
					grayTemp[i] = 0;
					posTemp[i*2] = 0;
					posTemp[i*2+1] = 0;
				}
			}
		}
	}

	{
		double dMean = 0;
		double dDevaition = 0;
		for(long i = 1; i<num; i++ )
		{
			dMean += gray[i];
			dDevaition += gray[i]*gray[i];
		}
		dMean /= num-1;
		dDevaition = max(10,sqrt(dDevaition/(num-1)-dMean*dMean));
		double dGate = dMean + 3 * dDevaition;
	}

	for (x=0; x<num; x++)
	{
		ptPos[x].x = pos[x*2+1];
		ptPos[x].y = pos[x*2];
	}

	/*/计算信噪比
	for (int aa=0; aa<num; aa++)
	{
		x = pos[2*aa+1];
		y = pos[2*aa];

		long nCount = 0;
		
		CRect rectIn = CRect(x-10,y-10,x+10,y+10);
		CRect rectout= CRect(x-20,y-20,x+20,y+20);

		CRect rectImg = CRect(0,0,dwWidth,dwHeight);

		rectIn.IntersectRect(rectIn,rectImg);
		rectout.IntersectRect(rectout,rectImg);

		double nBackMean = 0;
		double nBackDeviatio = 0;

		for( x = rectout.left; x<rectout.right; x ++ )
		for( y = rectout.top ; y<rectout.bottom;y ++ )
		{
			if( rectIn.PtInRect(CPoint(x,y)) )
				continue;

			nBackMean += pBitmap[y*dwWidth+x];
			nBackDeviatio += ((long)pBitmap[y*dwWidth+x])*((long)pBitmap[y*dwWidth+x]);
			nCount++;
		}
		nBackMean /= nCount;
		nBackDeviatio = sqrt(nBackDeviatio/nCount-nBackMean*nBackMean);

		nCount = 0;
		double nBackMean2=0,nBackDeviatio2=0;
		for( x = rectout.left; x<rectout.right; x ++ )
		for( y = rectout.top ; y<rectout.bottom;y ++ )
		{
			if( rectIn.PtInRect(CPoint(x,y)) )
				continue;

			if( pBitmap[y*dwWidth+x] < nBackDeviatio * 4 + nBackMean )
			{
				//nBackMean2 += pBitmap[y*dwWidth+x];
				//nBackDeviatio2 += ((long)pBitmap[y*dwWidth+x])*((long)pBitmap[y*dwWidth+x]);

				nBackMean2 += pImgOrign[y*dwWidth+x];
				nBackDeviatio2 += ((long)pImgOrign[y*dwWidth+x])*((long)pImgOrign[y*dwWidth+x]);
				
				nCount++;
			}
		}

		if( !nCount ) continue; // 按照3分割没有分割出来，则返回

		nBackMean = nBackMean2;
		nBackDeviatio = nBackDeviatio2;

		nBackMean /= nCount;
		nBackDeviatio = sqrt(nBackDeviatio/nCount-nBackMean*nBackMean);


		nCount=0;
		long nCenterX=0,nCenterY=0;
		long nPower=0;

	
		for( x = rectIn.left; x<rectIn.right; x ++ )
		for( y = rectIn.top ; y<rectIn.bottom;y ++ )
		{
			if( pImgOrign[y*dwWidth+x] > nBackDeviatio * 4 + nBackMean )
			{
				nCenterX += pImgOrign[y*dwWidth+x] * x;
				nCenterY += pImgOrign[y*dwWidth+x] * y;
				nPower += pImgOrign[y*dwWidth+x];
				nCount ++;
	
				pBitmap[y*dwWidth+x] = 255;
			}
		}
		if( !nCount ) 
		{
			continue;
		}
		nCenterX /= nPower;
		nCenterY /= nPower;

		pos[2*aa+1] = nCenterX;
		pos[2*aa]   = nCenterY;

		CRect rectSize = CRect(-5,-5,5,5);
			
		rectSize.OffsetRect(nCenterX,nCenterY);
		prectSize[aa] = rectSize;
		
	}
	*/
	
	delete codeOffset;
	delete grayTemp;
	delete posTemp;
	delete gray;
	delete grayCode;
	delete posCode;
	delete pos;	
	return true;
}

void CZXDib::GetSNR(int* pBitmap,double* pIntImg,double* pIntImg2,DWORD dwHeight,DWORD dwWidth,long nSize,long nSize2,double*& pResult)
{
	long i,j,m,n;
	if( pResult ) delete pResult;
	pResult = new double[dwHeight*dwWidth];
	memset(pResult,0,sizeof(double)*dwHeight*dwWidth);
	
	for( i=nSize2; i<dwHeight-nSize2; i++ )
	for( j=nSize2; j<dwWidth -nSize2; j++ )
	{
		int x1 = j + nSize2;
		int y1 = i + nSize2;
		int x0 = j - nSize2;
		int y0 = i - nSize2;
		
		double dMean1      = pIntImg [y1*dwWidth+x1] + pIntImg [y0*dwWidth+x0] - pIntImg [y0*dwWidth+x1] - pIntImg [y1*dwWidth+x0];
		double dDeviation1 = pIntImg2[y1*dwWidth+x1] + pIntImg2[y0*dwWidth+x0] - pIntImg2[y0*dwWidth+x1] - pIntImg2[y1*dwWidth+x0];
		int    nCount1 = (y1-y0) * ( x1-x0);


		dMean1  /= nCount1;
		dDeviation1 = dDeviation1 / nCount1 - dMean1*dMean1;

		double dSNR = ( pBitmap[i*dwWidth+j] - dMean1 );
		if( dSNR < 0 )
			pResult[i*dwWidth+j] =  0;
		else 
		{
			dSNR = dSNR * dSNR / (dDeviation1+1);
			pResult[i*dwWidth+j] =  dSNR;
		}
	}

}

void CZXDib::GetSNR(BYTE* pBitmap,double* pIntImg,double* pIntImg2,DWORD dwHeight,DWORD dwWidth,long nSize,long nSize2,double*& pResult)
{
	long i,j,m,n;
	if( pResult ) delete pResult;
	pResult = new double[dwHeight*dwWidth];
	memset(pResult,0,sizeof(double)*dwHeight*dwWidth);

	for( i=nSize2; i<dwHeight-nSize2; i++ )
	for( j=nSize2; j<dwWidth -nSize2; j++ )
	{
		int x1 = j + nSize2;
		int y1 = i + nSize2;
		int x0 = j - nSize2;
		int y0 = i - nSize2;
		
		double dMean1  = pIntImg[y1*dwWidth+x1] + pIntImg[y0*dwWidth+x0] - pIntImg[y0*dwWidth+x1] - pIntImg[y1*dwWidth+x0];
		double dDeviation1 = pIntImg2[y1*dwWidth+x1] + pIntImg2[y0*dwWidth+x0] - pIntImg2[y0*dwWidth+x1] - pIntImg2[y1*dwWidth+x0];
		int    nCount1 = (y1-y0) * ( x1-x0);

		x1 = j + nSize;
		y1 = i + nSize;
		x0 = j - nSize;
		y0 = i - nSize;
		
		double dMean2  = pIntImg[y1*dwWidth+x1] + pIntImg[y0*dwWidth+x0] - pIntImg[y0*dwWidth+x1] - pIntImg[y1*dwWidth+x0];
		double dDeviation2 = pIntImg2[y1*dwWidth+x1] + pIntImg2[y0*dwWidth+x0] - pIntImg2[y0*dwWidth+x1] - pIntImg2[y1*dwWidth+x0];
		int    nCount2 = (y1-y0) * ( x1-x0);

		dMean1 -= dMean2;
		dDeviation1 -= dDeviation2;
		nCount1 -= nCount2;	

		dMean1  /= nCount1;
		dDeviation1 = dDeviation1 / nCount1 - dMean1*dMean1;

		double dSNR = ( pBitmap[i*dwWidth+j] - dMean1 )*( pBitmap[i*dwWidth+j] - dMean1 ) / (dDeviation1+1);
		pResult[i*dwWidth+j] = dSNR;
	}

}
int CZXDib::MultitargetCode(BYTE *pBitmap, int dwHeight, int dwWidth, int areaSize, int num, POINT*& pos, double *&pSNR)
{
	long x,y;

	int *gray = new int[num];
	memset(gray,0,sizeof(int)*num);

	if (pos) delete pos;
	pos = new POINT[num];
	memset(pos,0,sizeof(POINT)*num);

	int *codeOffset = new int[num]; 
	memset(codeOffset,0,sizeof(int)*num);

	int *grayTemp = new int[num];
	memset(grayTemp,0,sizeof(int)*num);

	POINT *posTemp = new POINT[num];
	memset(posTemp,0,sizeof(POINT)*num);

	int *grayCode = new int[num];
	memset(grayCode,0,sizeof(int)*num);
	int *posCode = new int[num];
	memset(posCode,0,sizeof(int)*num);

	if (pSNR) delete pSNR;
	pSNR = new double[num];
	
	int i = 0;
	int j = 0;
	for (y=1; y<dwHeight; y++)
	{
		for (x=1; x<dwWidth; x++)
		{
			int k = pBitmap[y*dwWidth+x];
			for (i=0; i<num; i++)
			{
				if (k > gray[i]) grayCode[i] = 1;
				else grayCode[i] = 0;
			}
			for (i=0; i<num; i++)
			{
				if (abs(y-pos[i].y)<=areaSize && abs(x-pos[i].x)<=areaSize) posCode[i] = 1;
				else posCode[i] = 0;
			} 

			for (i=0; i<num; i++)
			{
				codeOffset[i] = 0;
				for (j=0; j<i; j++)
					codeOffset[i] += posCode[j];
			}

			for (i=0; i<num; i++)
			{
				codeOffset[i] = grayCode[i] - codeOffset[i];
			}

			if (grayCode[0] == 1)
			{
				grayTemp[0] = k; //replace
				posTemp[0].x = x;
				posTemp[0].y = y;
			}
			else //grayCode[0] = 0
			{
				if (posCode[0] == 0) 
				{
					for (i=1; i<num; i++)
					{
						if((~grayCode[i]&posCode[i])==1)
							break;
						if (grayCode[i] - grayCode[i-1] == 1)
						{
							grayTemp[i] = k; //replace
							posTemp[i].x = x;
							posTemp[i].y = y;
							break;
						}
					}
				}
			}
			int flag = 0;
			for (i=0; i<num; i++)
			{
				flag += ~grayCode[i]&posCode[i];
			}
			if (flag < 1)
			{
				for (i=0; i<num; i++)
				{
					if (grayCode[i] == 0)// && grayTemp[i]==1)
					{
						grayTemp[i] = gray[i];
						posTemp[i] = pos[i];
					}
					
					else if (grayCode[i] == 1)
					{
						if (posCode[i] == 1)
							continue;
						else if (posCode[i] ==0)
						{
							int rrr = i+codeOffset[i];
							if (rrr>=0 && rrr<num)
							{
								grayTemp[i+codeOffset[i]] = gray[i];
								posTemp[i+codeOffset[i]] = pos[i];
							}
						}
					}
				}

				for (i=0; i<num; i++)
				{
					gray[i] = grayTemp[i];
					pos[i] = posTemp[i];
					grayTemp[i] = 0;
					posTemp[i].x = posTemp[i].y = 0;					
				}
			}
		}
	}
	
	for (int aa=0; aa<num; aa++)
	{
		pSNR[aa] = 0;
		
		x = pos[aa].x;
		y = pos[aa].y;

		long nCount = 0;

		CRect rectIn = CRect(x-4,y-4,x+4,y+4);
		CRect rectout= CRect(x-10,y-10,x+10,y+10);

		CRect rectImg = CRect(0,0,dwWidth,dwHeight);

		rectIn.IntersectRect(rectIn,rectImg);
		rectout.IntersectRect(rectout,rectImg);

		double nBackMean = 0;
		double nBackDeviatio = 0;

		for( x = rectout.left; x<rectout.right; x ++ )
		for( y = rectout.top ; y<rectout.bottom;y ++ )
		{
			if( rectIn.PtInRect(CPoint(x,y)) )
				continue;

			nBackMean += pBitmap[y*dwWidth+x];
			nBackDeviatio += ((long)pBitmap[y*dwWidth+x])*((long)pBitmap[y*dwWidth+x]);
			nCount++;
		}
		nBackMean /= nCount;
		nBackDeviatio = sqrt(nBackDeviatio/nCount-nBackMean*nBackMean)+0.01;

		pSNR[aa] = ( pBitmap[pos[aa].y*dwWidth+pos[aa].x] - nBackMean );// / nBackDeviatio;
	}

	if (codeOffset) delete codeOffset;
	codeOffset = NULL;
	if (grayTemp) delete grayTemp;
	grayTemp = NULL;
	if (posTemp) delete posTemp;
	posTemp = NULL;
	if (gray) delete gray;
	gray = NULL;
	if (grayCode) delete grayCode;
	grayCode = NULL;
	if (posCode) delete posCode;
	posCode = NULL;
	return true;
}
void CZXDib::APlusB(BYTE* pBitmap1,DWORD dwHeight1,DWORD dwWidth1,
					BYTE* pBitmap2,DWORD dwHeight2,DWORD dwWidth2,
					BYTE*& pRes,DWORD &dwHeight, DWORD& dwWidth)
{
	dwHeight = max( dwHeight1, dwHeight2);
	dwWidth  = dwWidth1 + dwWidth2;
 
	if( pRes ) delete pRes;
	pRes = new BYTE[dwHeight*dwWidth];
	memset(pRes,0,sizeof(BYTE)*dwHeight*dwWidth);

	long i; for( i=0; i<dwHeight1; i++ )
	for( long j=0; j<dwWidth1;  j++ )
		pRes[i*dwWidth+j] = pBitmap1[i*dwWidth1+j];	

	for(      i=0; i<dwHeight2; i++ )
	for( long j=0; j<dwWidth2;  j++ )
		pRes[i*dwWidth+j+dwWidth1] = pBitmap2[i*dwWidth2+j];
}
void CZXDib::APlusD(BYTE* pBitmap1,DWORD dwHeight1,DWORD dwWidth1,
					BYTE* pBitmap2,DWORD dwHeight2,DWORD dwWidth2,
					BYTE*& pRes,DWORD &dwHeight, DWORD& dwWidth)
{
	dwWidth = max( dwWidth1, dwWidth2);
	dwHeight= dwHeight1 + dwHeight2;
	
	if( pRes ) delete pRes;
	pRes = new BYTE[dwHeight*dwWidth];
	memset(pRes,0,sizeof(BYTE)*dwHeight*dwWidth);
	
	long i; for( i=0; i<dwHeight1; i++ )
	for( long j=0; j<dwWidth1;  j++ )
		pRes[i*dwWidth+j] = pBitmap1[i*dwWidth1+j];	

	for(      i=0; i<dwHeight2; i++ )
	for( long j=0; j<dwWidth2;  j++ )
		pRes[(i+dwHeight1)*dwWidth+j] = pBitmap2[i*dwWidth2+j];
}
//////////////////////////////////////////////////////////////////////////
//
// 标记灰度图象
//
//////////////////////////////////////////////////////////////////////////
void CZXDib::LabelGrayImage(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,BYTE byGate,RECT*& pRect,long &nCount)
{
	long i,j,k,nLabelNO,nLineNow,nLineLast;
	DWORD dwSize = dwHeight * dwWidth;

	long* pLabel[2] = { NULL, NULL };
	pLabel[0] = new long[dwWidth];
	pLabel[1] = new long[dwWidth];

	for( i=0; i<dwWidth; i++ )
		pLabel[1][i] = pLabel[0][i] = 1;

	if( pRect ) delete pRect;
	pRect = new RECT [dwSize];
	memset( pRect, 0, sizeof(RECT)*dwSize );

	long* pLabelMap = NULL;
	pLabelMap = new long[dwSize];
	for( i=0; i<dwSize; i++ )
		pLabelMap[i] = i;

	nLabelNO  = 2;
	nLineLast = 1;
	nLineNow  = 0;

	for( i=1; i<dwHeight; i++ )
	{
		memset( pLabel[nLineNow], 0, sizeof(long)*dwWidth );

		for( j=1; j<dwWidth;  j++ )
		{
			long nIDLeft = pLabelMap[ pLabel[nLineNow ][j-1] ];
			long nIDUp   = pLabelMap[ pLabel[nLineLast][  j] ];

			bool bLeft = abs(( pBitmap[i*dwWidth+j ]  - (long)(pBitmap[(i  )*dwWidth+j-1]) )) < byGate;// && ( pBitmap[i*dwWidth+j ]  >= (long)(pBitmap[(i  )*dwWidth+j-1]) );
			bool bUp   = abs(( pBitmap[i*dwWidth+j ]  - (long)(pBitmap[(i-1)*dwWidth+  j]) )) < byGate;// && ( pBitmap[i*dwWidth+j ]  >= (long)(pBitmap[(i-1)*dwWidth+  j]) );
			bool bCross= abs(( pBitmap[i*dwWidth+j-1] - (long)(pBitmap[(i-1)*dwWidth+  j]) )) < byGate;// && ( pBitmap[i*dwWidth+j-1] >= (long)(pBitmap[(i-1)*dwWidth+  j]) );


			//
			if( bCross || (bLeft && bUp) )//
			{
				long nTo   = max(nIDLeft,nIDUp);
				long nFrom = min(nIDLeft,nIDUp);

				pLabel[nLineNow][j] = nTo;

				if( nFrom != nTo )
				{
					//
					pLabelMap[nFrom] = nTo;		//

					pRect[nTo].left   = min( pRect[nTo].left  , pRect[nFrom].left   );
					pRect[nTo].top    = min( pRect[nTo].top   , pRect[nFrom].top    );
					pRect[nTo].right  = max( pRect[nTo].right , pRect[nFrom].right  );
					pRect[nTo].bottom = max( pRect[nTo].bottom, pRect[nFrom].bottom );

					//
					memset(&(pRect[nFrom]),0,sizeof(RECT));

					//
					for( k = nLabelNO; k > 0; k-- )
						pLabelMap[k] = pLabelMap[ pLabelMap[k] ];

					nIDUp = nIDLeft = nTo;
				}
			}

			if( !bLeft && !bUp )//
			{
				pRect[nLabelNO].right  = pRect[nLabelNO].left = j;
				pRect[nLabelNO].bottom = pRect[nLabelNO].top  = i;

				pLabel[nLineNow][j] = nLabelNO++;
			}
			else if( bLeft ) //
			{
				pLabel[nLineNow][j] = nIDLeft;
			}
			else if( bUp ) //
			{
				pLabel[nLineNow][j] = nIDUp;
			}

			k = pLabel[nLineNow][j];

			pRect[k].left   = min( pRect[k].left  , j );
			pRect[k].top    = min( pRect[k].top   , i );
			pRect[k].right  = max( pRect[k].right , j );
			pRect[k].bottom = max( pRect[k].bottom, i );
		}

		nLineLast =     nLineNow;
		nLineNow  = 1 - nLineNow;
	}
	nCount = nLabelNO;


	BYTE* pDel = new BYTE[nCount];
	memset(pDel,0,sizeof(BYTE)*nCount);

	// 首先对目标尺度进行过滤
	for( i=0; i<nCount; i++ )
	{
		if( pDel[i] = ( pRect[i].bottom < 1 && pRect[i].right < 1 ) )
			continue;

		pRect[i].right ++;
		pRect[i].bottom++;

		CRect rect = pRect[i];
		if( pDel[i] = ( rect.Width() > 80 || rect.Height() > 80 || rect.Width() > rect.Height() * 10 || rect.Height() > rect.Width() * 10 ) )
			continue;
	}

	// 对包含关系滤波
	for( i=0; i<nCount; i++ )
	{
		if( pDel[i] ) continue;

		for( j=0; j<nCount;	j++ )
		{
			if( i==j || pDel[j] ) continue;

			CRect rectj = pRect[j];
			CRect recti = pRect[i];
			recti.InflateRect(5,5,5,5);
			if( recti.IntersectRect(rectj,recti) )
			{
				recti.UnionRect(CRect(pRect[i]),rectj);
				pRect[i] = recti;
				pDel[j]  = 1;
			}
		}
	}

	long nPos = 0;
	for( i=0; i<nCount; i++ )
	{
		if( pDel[i] ) continue;

		if( nPos != i )
			memcpy(pRect+nPos++,pRect+i,sizeof(RECT));

		//DrawRectInTheMatrix(pBitmap,dwHeight,dwWidth,pRect[i],0);
	}

	nCount = nPos;

	delete pDel;
	pDel = NULL;

	delete pLabel[0];
	delete pLabel[1];
	delete pLabelMap;
}



//针对不同色调颜色
void CZXDib::AdjustHSI(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nHScopeFrom,long nHScopeTo,long nH,long nS,long nI,BYTE* pResult)
{
	
	const long HSI_ADJUST_SCOPENUMBER = 7;
	const long HSI_ADJUST_SCOPEFROM[HSI_ADJUST_SCOPENUMBER] = {
		-180,
			15,
			75,
			135,
			195,
			255,
			315
	};
	const long HSI_ADJUST_SCOPETO[HSI_ADJUST_SCOPENUMBER] = {
		180,
			45,
			105,
			165,
			225,
			285,
			345
	};
	double SCURVEIS[21]={
		1.00,
			.983,
			.965,
			.947,
			.928,
			.909,
			.888,
			.867,
			.845,
			.822,
			.80,
			.772,
			.745,
			.716,
			.684,
			.651,
			.610,
			.565,
			.511,
			.436,
			0};
		double SCURVESS[12]=
		{
			1,
				1.067,
				1.133,
				1.219,
				1.333,
				1.506,
				1.753,
				2.133,
				2.977,
				5.12,
				64,
				100
		};

	// chp 7 - 12
	if (nH == 0 && nS == 0 && nI == 0)
	{
		return;
	}
	
	double dHLeft,dHMidLeft,dHMidRight,dHRight;
	double dDisLeft,dDisRight;

	double pi = 3.14159265;
	CZXDib dib;	
	long i,dwSize;
	double R,G,B,H,S,I;
	double dScale=1;

	double dHLeftTemp,dHMidLeftTemp,dHMidRightTemp,dHRightTemp,dHTemp;
	double dIntervalLeft,dIntervalRight;
	double dTemp = pi / 180;

	dIntervalLeft = dIntervalRight = pi / 6;
	dHLeft = nHScopeFrom >= 60 ? ( nHScopeFrom - 60 ) * dTemp : ( nHScopeFrom - 60 ) * dTemp + 2*pi;
	dHRight= ( nHScopeTo        ) * dTemp;
	dHMidLeft  = nHScopeFrom>=30 ? ( nHScopeFrom-30 ) * dTemp : ( nHScopeFrom-30 ) * dTemp + 2*pi;
	dHMidRight = ( nHScopeFrom) * dTemp;

	dHLeftTemp    = dHLeft;
	dHMidLeftTemp = dHMidLeft > dHLeft ? dHMidLeft : 2*pi + dHMidLeft;
	dHMidRightTemp= dHMidRight> dHLeft ? dHMidRight: 2*pi + dHMidRight;
	dHRightTemp   = dHRight   > dHLeft ? dHRight   : 2*pi + dHRight;

	dDisLeft = dHMidLeftTemp - dHLeftTemp;
	dDisRight= dHRightTemp - dHMidRightTemp;
	ASSERT( dDisRight >= 0 && dDisLeft > 0);

	// chp 7 - 12
	// dTemp <---> nI 以及 S 之间的查找表
	double dLUT1[101*101];
	long j;
	if (nI < 0) // nI 小于0时用到
	{
		memset(dLUT1, 0, sizeof(double)*101*101);
		for (i = 0; i <= 100; i ++) // S
		for (j = 0; j <= 100; j ++) // nI
		{
			S = i;
			S /= 100.0;
				
			// 下面调整S:
			dTemp = (j-j/5*5) / 5 * (SCURVEIS[j/5+1] - SCURVEIS[j/5]) + SCURVEIS[j/5];
			dTemp = pow( 1.0 - pow( 1 - S, dTemp ), 1.0 / dTemp );	
				
			dLUT1[i * 101 + j] = dTemp;
		}		
	}
	// S <---> nS , S之间的查找表
	double dLUT2[101*101];
	if (nS < 0)
	{
		memset(dLUT2, 0, sizeof(double)*101*101);
		for (i = 0; i <= 100; i ++) // S
		for (j = 0; j <= 100; j ++) // nS
		{
			S = i;
			S /= 100.0;

			dTemp = (j - j / 10 * 10) / 10.0;
			dLUT2[i * 101 + j] =  S * (dTemp / SCURVESS[j/10+1] + (1 - dTemp) / SCURVESS[j/10]);
		}
	}
	else if (nS > 0)
	{
		memset(dLUT2, 0, sizeof(double)*101*101);
		for (i = 0; i <= 100; i ++) // S
		for (j = 0; j <= 100; j ++) // nS
		{
			S = i;
			S /= 100.0;
				
			dTemp = (j - j / 10 * 10) / 10.0;
			dLUT2[i * 101 + j] =  S * (dTemp * SCURVESS[j/10+1] + (1 - dTemp) * SCURVESS[j/10]);
		}
	}

	dwSize = dwHeight*dwWidth;
	for( i=0; i<dwSize; i++ )
	{
		// 此处使用最原始图像计算HSI，用于使用最原始的参数，模拟photoshop
		R = pBitmap[i*3+2];
		G = pBitmap[i*3+1];
		B = pBitmap[i*3];

		// 
		dib.GetHSBFromRGB(H,S,I,R,G,B);
		
		H = H * pi / 180.0;
		I *= 255.0;

		dHTemp = H > dHLeft ? H : 2*pi + H;
		// 判断是否在H区间内,算法只对落在这个区间内的进行处理
		if( dHTemp < dHLeftTemp || dHTemp > dHRightTemp  )
		{
			continue;
		}	
		
		// 先计算比例因子，用于计算下面的S,I
		if( dHTemp >= dHMidRightTemp )
			dScale = 1 - ( dHRightTemp - dHTemp ) / dIntervalRight;
		else if( dHTemp <= dHMidLeftTemp )
			dScale = 1 - ( dHTemp - dHLeftTemp ) / dIntervalLeft;
		else 
			dScale = 0;	

		// 重新计算 HSI,
		R = pBitmap[i*3+2];
		G = pBitmap[i*3+1];
		B = pBitmap[i*3];		
		
		dib.GetHSBFromRGB(H,S,I,R,G,B);
		H = H * pi / 180.0;
		I *= 255;
		
		// 计算当前的增量
		
		// 将增量转化为弧度单位
		dTemp = nH * pi / 180.0;
		if( dHTemp < dHMidLeftTemp )		
			H += ( 1 - ( dHMidLeftTemp - dHTemp ) / dDisLeft) * dTemp;
		else if( dHTemp < dHMidRightTemp )
			H += dTemp;
		else 
			H += ( 1 - ( dHTemp - dHMidRightTemp) / dDisRight ) * dTemp;

		// ====== 计算亮度引起的变化

		dTemp = nI * 255.0 / 100.0;
		if( nI < 0 )	// 增量小于0，调整I, 以及S
		{
			dTemp = -nI / 100.0;
			I = ( dTemp*dScale - dTemp ) * S * I + I ;
/*		
			// 下面调整S:
			long m = -nI;
			dTemp = (m-m/5*5) / 5 * (SCURVEIS[m/5+1] - SCURVEIS[m/5]) + SCURVEIS[m/5];
			dTemp = pow( 1.0 - pow( 1 - S, dTemp ), 1.0 / dTemp );*/

			// 调用查找表
			dTemp = dLUT1[(int)(S*100)*101+(-nI)];

			S = (S-dTemp)*(dScale) + dTemp;			
		}
		else if( nI > 0 )//　增量大于0，调整S
		{			
			dTemp = nI / 100.0;
			S = ( dTemp*dScale + 1 - dTemp ) * S;
		}


		// ====== 计算饱和度引起的变化
		double dSBack = S;
		double dIBack = I;		

		if( nS < 0 ) // 若饱和度小于0，则同时调整 明度 和 饱和度
		{			
/*
			long nSTemp = -nS;
			dTemp = nSTemp - nSTemp / 10 * 10;
			dTemp /= 10.0;
			S *= dTemp / SCURVESS[nSTemp/10+1] + (1-dTemp) / SCURVESS[nSTemp/10];*/

			// 利用查找表
			S = dLUT2[(int)(S*100)*101+(-nS)];

			I = ( (S - dSBack)/2 + 1 ) * I;
			I = (dIBack-I)*(dScale) + I;

			S = (dSBack-S)*(dScale) + S;		
		}
		else if (nS > 0)
		{
/*
			dTemp = nS - nS / 10 * 10;
			dTemp /= 10.0;
			S *= dTemp * SCURVESS[(long)nS / 10+1] + (1-dTemp) * SCURVESS[(long)nS/10];*/

			S = dLUT2[(int)(S*100)*101+nS];
			
			I = ((S - dSBack) + 1 ) * I;
			I = (dIBack-I)*(dScale) + I;

			S = (dSBack-S)*(dScale) + S;
		}


		// 规范到有效区间以内
		H = H * 180 / pi;
		if( H > 360 )
			H -= 360;
		else if( H < 0 )
			H += 360;

		I /= 255.0;
		if( I > 1 ) I = 1;
		else if( I < 0 ) I = 0;

		if( S > 1 ) S = 1;
		else if( S < 0 ) S = 0;


		dib.GetRGBFromHSB(
			pResult[i*3+2],
			pResult[i*3+1],
			pResult[i*3],
			H,S,I);
	}
	return;
}

// 针对灰度影象
void CZXDib::AdjustHSI(BYTE* pBitmap,DWORD dwHeight,DWORD dwWidth,long nH,long nS,long nI,BYTE* pResult)
{
	if(!pBitmap||!pResult)
		return;
	
	// chp 7 - 12
	if (nH == 0 && nS == 0 && nI == 0)
	{
		return;
	}
	
	double R,G,B,H,S,I;	
	long i,dwSize;

	dwSize = dwHeight*dwWidth;
	
	for( i=0; i<dwSize; i++ )
	{
		R = pBitmap[i*3+2];
		G = pBitmap[i*3+1];
		B = pBitmap[i*3  ];
		
		{
			// (double &H,double &S,double &Y,BYTE R,BYTE G,BYTE B)
			long nMin    = min(min(R,G),B);
			long nMax    = max(max(R,G),B);
			double delta = nMax - nMin;
			
			H = S = 0;
			I = (R+B+G) / 765.0;

			if( nMax )
			{
				S = delta / nMax;
				I = nMax  / 255.0;
				if( nMax != nMin )
				{
					if( R == nMax )
						H = (G-B)/delta;
					else if( G == nMax )
						H = 2 + ((B-R)/delta);
					else 
						H = 4 + ((R-G)/delta);
			}	}
			
			H = H < 0 ? H * 60 + 360 : H * 60;
		}		
		
		H += nH;
		S += nS / 100.0;
		I += nI / 100.0;	
		
		// 规范到有效区间以内
		H = H > 360 ? H - 360 : ( H < 0 ? H + 360 : H );
		I = min( 1, max( 0, I ) );		
		S = min( 1, max( 0, S ) );
		
		//dib.GetRGBFromHSB(pResult[i*3+2],pResult[i*3+1],pResult[i*3],H,S,I);
		{
			double f,p,q,t,r,g,b;
			
			if( S < 0.001 )
			{
				r = I * 255 + 0.5;
				g = I * 255 + 0.5;
				b = I * 255 + 0.5;
			}else 
			{
				H = H / 60.0;
				f = H - (long)H;
				p = I * (1.0 - S  );
				q = I * (1.0 - S*f);
				t = I * (1.0 - S*(1.0-f));
				
				switch((long)H)
				{
				case 0:		r=I;	g=t;	b=p;	break;
				case 1:		r=q;	g=I;	b=p;	break;
				case 2:		r=p;	g=I;	b=t;	break;
				case 3:		r=p;	b=I;	g=q;	break;
				case 4:		r=t;	b=I;	g=p;	break;
				default:	r=I;	g=p;	b=q;
				}
				r = r * 255 + 0.5;
				g = g * 255 + 0.5;
				b = b * 255 + 0.5;
			}
			
			pResult[i*3+2] = r < 0 ? 0 : ( r > 255 ? 255 : r );
			pResult[i*3+1] = g < 0 ? 0 : ( g > 255 ? 255 : g );
			pResult[i*3  ] = b < 0 ? 0 : ( b > 255 ? 255 : b );
		}
	}
}

void CZXDib::GetPolarImg(BYTE* pBmp, int nH, int nW, int nX, int nY, BYTE*& pRest, int &nResH, int &nResW)
{
	nResH = nH;  // 
	nResW = 360; // 角度

	static double * pdCos = NULL;
	static double * pdSin = NULL;
	if( !pdCos ) 
	{
		pdCos = new double[nResW];
		pdSin = new double[nResW];
		for( int i=0; i<nResW; i++ )
		{
			double d = i * 2 * 3.14159265358 / nResW;
			pdCos[i] = cos(d);
			pdSin[i] = sin(d);
	}	}	
	
	
	delete pRest;
	pRest = new BYTE[nResH*nResW];

	int i,j; 
	for( i=0; i<nResH; i++ )
	for( j=0; j<nResW; j++ )
	{
		int y = i * 0.5 * pdCos[j] + nY + 0.5;
		int x = i * 0.5 * pdSin[j] + nX + 0.5;
		if( x < 0 || y < 0 || x >= nW || y >= nH )
		{
			pRest[i*nResW+j] = 0;
			continue;
		}
		pRest[i*nResW+j] = pBmp[y*nW+x];
	}
}
void CZXDib::GetPolarImgColor(BYTE* pBmp, int nH, int nW, int nX, int nY, BYTE*& pRest, int &nResH, int &nResW)
{
	nResH = nH;  // 
	nResW = 360; // 角度
	
	static double * pdCos = NULL;
	static double * pdSin = NULL;
	if( !pdCos ) 
	{
		pdCos = new double[nResW];
		pdSin = new double[nResW];
		for( int i=0; i<nResW; i++ )
		{
			double d = i * 2 * 3.14159265358 / nResW;
			pdCos[i] = cos(d);
			pdSin[i] = sin(d);
	}	}	
	
	
	delete pRest;
	pRest = new BYTE[nResH*nResW*3];

	int i,j; 
	for( i=0; i<nResH; i++ )
	for( j=0; j<nResW; j++ )
	{
		int y = i * 0.5 * pdCos[j] + nY + 0.5;
		int x = i * 0.5 * pdSin[j] + nX + 0.5;
		if( x < 0 || y < 0 || x >= nW || y >= nH )
		{
			pRest[(i*nResW+j)*3+2] = 0;
			pRest[(i*nResW+j)*3+1] = 0;
			pRest[(i*nResW+j)*3+0] = 0;
			continue;
		}
		pRest[(i*nResW+j)*3+2] = pBmp[(y*nW+x)*3+2];
		pRest[(i*nResW+j)*3+1] = pBmp[(y*nW+x)*3+1];
		pRest[(i*nResW+j)*3+0] = pBmp[(y*nW+x)*3+0];
	}
}
void CZXDib::GetPolarImgSetW(BYTE* pBmp, int nH, int nW, int nX, int nY, int nResW, int nR, BYTE*& pRest, int &nResH)
{
	nResH = nH;  // 
	//nResW = 360; // 角度

	double dStep = 2.0 * 3.14159265358  / nResW;
	
	static double * pdCos = NULL;
	static double * pdSin = NULL;
	if( !pdCos ) 
	{
		pdCos = new double[nResW];
		pdSin = new double[nResW];
		for( int i=0; i<nResW; i++ )
		{
			double d = i * dStep;
			pdCos[i] = cos(d);
			pdSin[i] = sin(d);
	}	}		
	
	delete pRest;
	pRest = new BYTE[nResH*nResW];
	memset(pRest, 0, sizeof(BYTE)*nResH*nResW);
	
	int i,j; 
	for( i=nR; i<nResH; i++ )
	for( j=0; j<nResW; j++ )
	{
		int y = i * 0.5 * pdCos[j] + nY + 0.5;
		int x = i * 0.5 * pdSin[j] + nX + 0.5;
		if( x < 0 || y < 0 || x >= nW || y >= nH )
		{
			pRest[i*nResW+j] = 0;
			continue;
		}
		pRest[i*nResW+j] = pBmp[y*nW+x];
	}
}

/*************************************************************************
 *
 * 函数名称：
 *   Fourier()
 *
 * 参数:
 *   LPSTR lpDIBBits    - 指向源DIB图像指针
 *   LONG  lWidth       - 源图像宽度（象素数）
 *   LONG  lHeight      - 源图像高度（象素数）
 *
 * 返回值:
 *   BOOL               - 成功返回TRUE，否则返回FALSE。
 *
 * 说明:
 *   该函数用来对图像进行付立叶变换。
 *
 ************************************************************************/
void CZXDib::Fourier(BYTE* pData, int nLen, double*& pdR, double*& pdI, int& nResW)
{
	// 循环变量
	int	i, wp;

	// 进行付立叶变换的宽度和高度（2的整数次方）	
	for(wp=0,nResW=1; nResW<nLen;  wp++ ) nResW*=2;	


	double* pTemp = new double[nResW];
	memset(pTemp,0,sizeof(double)*nResW);
	
	for(i=0;i<nResW; i++)
	{
		int nX = i < nResW ? i : 2*nLen - i - 1;
		pTemp[i] = pData[nX];
	}
			
	// 分配内存
	complex<double> *TD = new complex<double>[nResW];
	complex<double> *FD = new complex<double>[nResW];
	
	// 行
	for(i = 0; i < nResW; i++)
		TD[i] = complex<double>(pTemp[i], 0);

	FFT(TD, FD, wp);

	delete pdR; pdR = new double[nResW];
	delete pdI; pdI = new double[nResW];
	
	for( i=0; i<nResW/2; i++ )
	{
		pdR[i] = FD[nResW/2-1-i].real();
		pdI[i] = FD[nResW/2-1-i].imag();
	}
	for( i=nResW/2; i<nResW; i++ )
	{
		pdR[i] = FD[nResW/2 + nResW-1-i].real();
		pdI[i] = FD[nResW/2 + nResW-1-i].imag();
	}
			
	// 删除临时变量
	delete TD;
	delete FD;
	delete pTemp;
}

void CZXDib::FlatterImg(BYTE* pBitmap, int dwHeight, int dwWidth, int nSizeX, int nSizeY, BYTE*& pRest )
{
	int i,j;

	int nH = dwHeight + nSizeY*2;
	int nW = dwWidth  + nSizeX*2;
	
	delete pRest;
	pRest = new BYTE[nH*nW];

	for( i=0; i<dwHeight; i++ )
	for( j=0; j<dwWidth; j++ )
		pRest[(i+nSizeY)*nW+j+nSizeX] = pBitmap[i*dwWidth+j];
	
	for( i=0; i<dwHeight; i++ )
	for( j=0; j<nSizeX; j++ )
	{
		pRest[(i+nSizeY)*nW+     j] = pBitmap[i*dwWidth+dwWidth-1-(nSizeX-1-j)];
		pRest[(i+nSizeY)*nW+nW-1-j] = pBitmap[i*dwWidth+(nSizeX-1-j)];
	}
	for( i=0; i<nSizeY; i++ )
	{
		memcpy(pRest+i*nW, pRest+nSizeY*nW, nW*sizeof(BYTE));
		memcpy(pRest+(nH-1-i)*nW, pRest+(nH-1-nSizeY)*nW, nW*sizeof(BYTE));
	}
}
void CZXDib::AT(BYTE* pBmp, int nHeight, int nWidth)
{
	BYTE* pT = new BYTE[nHeight*nWidth];
	int i, j;
	for( i=0; i<nHeight; i++ )
	for( j=0; j<nWidth;  j++ )
		pT[j*nHeight+i] = pBmp[i*nWidth+j];
	memcpy(pBmp,pT,nHeight*nWidth);
	delete pT;
}

void CZXDib::CreateMasks(int nH, int nW, double dAngleStep, double dWidth, BYTE*& pMask, int &nCnt )
{
	nCnt = 180 / dAngleStep;
	if( pMask ) delete pMask;
	pMask = new BYTE[nH * nW*nCnt];

	memset(pMask, 0, nH * nW*nCnt);
	
	int i; for( i=0; i<nCnt; i++ )
	{
		double dSin = sin(dAngleStep*3.14159265358*i/180);
		double dCos = cos(dAngleStep*3.14159265358*i/180);

		for( int m=0; m<nH; m++ )
		for( int n=0; n<nW; n++ )
		{
			double x = (m-nH/2.0)*dCos-(n-nW/2.0)*dSin;
			//double y = (m-nH/2.0)*dSin+(n-nW/2.0)*dCos;

			if( fabs(x) < dWidth )
				pMask[i*nH*nW+m*nW+n] = 255;
		}
	}
}

/*/===========================================================================  
/// RGB2LAB  
//===========================================================================  
void Saliency::RGB2LAB(
	const vector<unsigned int>&               ubuff,
	vector<double>&                   lvec,
	vector<double>&                   avec,
	vector<double>&                   bvec)
{
	int sz = int(ubuff.size());
	lvec.resize(sz);
	avec.resize(sz);
	bvec.resize(sz);

	for (int j = 0; j < sz; j++)
	{
		int r = (ubuff[j] >> 16) & 0xFF;
		int g = (ubuff[j] >> 8) & 0xFF;
		int b = (ubuff[j]) & 0xFF;

		double xval = 0.412453 * r + 0.357580 * g + 0.180423 * b;
		double yval = 0.212671 * r + 0.715160 * g + 0.072169 * b;
		double zVal = 0.019334 * r + 0.119193 * g + 0.950227 * b;

		xval /= (255.0 * 0.950456);
		yval /= 255.0;
		zVal /= (255.0 * 1.088754);

		double fX, fY, fZ;
		double lval, aval, bval;

		if (yval > 0.008856)
		{
			fY = pow(yval, 1.0 / 3.0);
			lval = 116.0 * fY - 16.0;
		}
		else
		{
			fY = 7.787 * yval + 16.0 / 116.0;
			lval = 903.3 * yval;
		}

		if (xval > 0.008856)
			fX = pow(xval, 1.0 / 3.0);
		else
			fX = 7.787 * xval + 16.0 / 116.0;

		if (zVal > 0.008856)
			fZ = pow(zVal, 1.0 / 3.0);
		else
			fZ = 7.787 * zVal + 16.0 / 116.0;

		aval = 500.0 * (fX - fY) + 128.0;
		bval = 200.0 * (fY - fZ) + 128.0;

		lvec[j] = lval;
		avec[j] = aval;
		bvec[j] = bval;
	}
}
*/

void CZXDib::GetLocalSNR(BYTE* pBitmap, int dwHeight, int dwWidth, int nSzH, int nSzW, double*& pDev)
{
	int i, j;
	int nSzH2 = nSzH / 2;
	int nSzW2 = nSzW / 2;

	int nH = dwHeight + nSzH2 * 2;
	int nW = dwWidth + nSzW2 * 2;

	BYTE* pBmp2 = new BYTE[nH*nW];
	for (i = 0; i < dwHeight; i++)
		for (j = 0; j < dwWidth; j++)
			pBmp2[(i + nSzH2)*nW + j + nSzW2] = pBitmap[i*dwWidth + j];

	for (i = 0; i < dwHeight; i++)
		for (j = 0; j < nSzW2; j++)
		{
			pBmp2[(i + nSzH2)*nW + j] = pBitmap[i*dwWidth + dwWidth - 1 - (nSzW2 - 1 - j)];
			pBmp2[(i + nSzH2)*nW + nW - 1 - j] = pBitmap[i*dwWidth + (nSzW2 - 1 - j)];
		}
	for (i = 0; i < nSzH2; i++)
	{
		memcpy(pBmp2 + i*nW, pBmp2 + nSzH2*nW, nW * sizeof(BYTE));
		memcpy(pBmp2 + (nH - 1 - i)*nW, pBmp2 + (nH - 1 - nSzH2)*nW, nW * sizeof(BYTE));
	}

	double* pdSum = NULL;
	GetInteImg(pBmp2, nH, nW, pdSum);

	double *pdSum2 = NULL;
	GetInteImg2(pBmp2, nH, nW, pdSum2);

	GetSNR(pBmp2, pdSum, pdSum2, nH, nW, 1, nSzW, pDev);

	for (i = nSzH2; i < nH - nSzH2; i++)
		for (j = nSzW2; j < nW - nSzW2; j++)
		{
			pDev[(i - nSzH2)*dwWidth + j - nSzW2] = pDev[(i)*nW + j];
		}


	// 	BYTE* pTemp = NULL;
	// 	RegularToGrayLevel(pDev,dwHeight,dwWidth,pTemp);
	// 	AddInterResultImage(pTemp,dwHeight,dwWidth,8,"GetLocal");
	// 	delete pTemp;

	delete pdSum;
	delete pdSum2;
	delete pBmp2;
}
void CZXDib::RepeatMean(int* pData, int nW, int nCount)
{
	for (int n = 0; n < nCount; ++n)
	for (int m = 1; m < nW - 1; ++m)
		pData[m] = (pData[m - 1] + pData[m] + pData[m + 1]) / 3;
}