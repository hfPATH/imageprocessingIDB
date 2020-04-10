#include "stdafx.h"

////////////////////////////////////////////////////
//
//  将向量数据转换为ＢＭＰ图像向量:
//
////////////////////////////////////////////////////
bool TranslateVectorToBitmap(int dwHeight,int dwWidth,BYTE* pVector,BYTE*& pBitmapData,WORD flag)
{
	int i,j;
	if(flag==24) dwWidth *= 3;
	else if(flag!=8) return false;
	int dwWriteWidth = (dwWidth+3)/4*4;
		
	if(pBitmapData) delete []pBitmapData;
	pBitmapData = (BYTE*) new BYTE[dwHeight*dwWriteWidth];
		
	for(i=0; i<dwHeight; i++)
		for(j=0; j<dwWidth; j++)
			pBitmapData[(i)*dwWriteWidth + j] = pVector[i*dwWidth + j];
			//pBitmapData[(dwHeight-1-i)*dwWriteWidth+j]   = pVector[i*dwWidth+j];
			
		return true;
}
////////////////////////////////////////////////////
//
//  将ＢＭＰ图像向量转换为向量数据:
//
////////////////////////////////////////////////////
bool TranslateBitmapToVector(int dwHeight,int dwWidth,BYTE* pBitmapData,BYTE*& pVector,WORD flag)
{
	int i,j;
	if(flag==24) dwWidth *= 3;
	else 
		if(flag!=8) return false;
		int dwWriteWidth = (dwWidth+3)/4*4;
		
		if(pVector) delete []pVector;
		pVector = (BYTE*) new BYTE[dwHeight*dwWriteWidth];
		
		for(i=0; i<dwHeight; i++)
			for(j=0; j<dwWidth; j++)
				pVector[i*dwWidth+j] = pBitmapData[(dwHeight-1-i)*dwWriteWidth+j];
			
			return true;
}


bool CreateBITMAPINFO(BITMAPINFO *&pbi,int dwHeight, int dwWidth,WORD flag)
{
	if(flag==24) dwWidth *= 3;
	int dwWriteSize  = dwHeight * ((dwWidth+3)/4*4);
	
	if(pbi) delete pbi;	
	if(flag == 24) 
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)];
	else 
		pbi = (BITMAPINFO*) new BYTE[sizeof(BITMAPINFO)+255*sizeof(RGBQUAD)];
	pbi->bmiHeader.biSize = sizeof(BITMAPINFO)-4;//无论是黑白图还是彩图都是40
	pbi->bmiHeader.biBitCount = flag;
	pbi->bmiHeader.biHeight =-dwHeight;
	
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

////////////////////////////////////////////////////////////////////
//将一维向量图像数据写入文件
// FileName 文件名
// pBitmapData 一维向量指针
// dwWidth,dwHeight 图像的宽度和长度
// flag : 图像的类型
////////////////////////////////////////////////////////////////////
bool WriteBMPFileFromVector(CString &FileName, BYTE *pBitmapData,int dwHeight,int dwWidth,WORD flag)
{
	//参数有效性检测：
	if(!pBitmapData) 
		return FALSE;
	if((flag!=8)&&(flag!=24))
		return FALSE; 
	
	if(flag == 24)
		dwWidth *= 3;
	
	int dwWriteWidth = (dwWidth+3)/4*4;
	int dwWriteSize = dwHeight * dwWriteWidth;
	
	//打开文件:
	CFile File;
//	File.Open("2.bmp",);
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
	int position = dwWidth*dwHeight - dwWidth;
	int skip = dwWriteWidth - dwWidth;
	
	File.Seek(BitmapFileHeader.bfOffBits,0);
	for(int i=0; i<dwHeight; i++)
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



///////////////////////////////////////////////////////////////////////////////////////////
//
//  归一化到灰度图像的范围内
//
///////////////////////////////////////////////////////////////////////////////////////////
bool RegularToGrayLevel(double *pBitmap, int dwHeight, int dwWidth,BYTE*& pResult)
{
	int i,j;
	double max= 0;
	double min=-3;
	double temp;
	
	if(pResult) delete pResult;
	pResult = new BYTE[dwHeight*dwWidth];
	
	max -= min;

	for(i=0; i<dwHeight;i++)
	for(j=0; j<dwWidth; j++)
	{
		temp = (pBitmap[i*dwWidth+j]-min)*255.0/max;
		pResult[i*dwWidth+j] = (BYTE) ( temp>255 ? 255:(temp<0 ? 0:temp) );
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
bool LoadVectorFromBMPFile(CString &FileName, BYTE *&pBitmapData, int &dwHeight, int &dwWidth,WORD &flag)
{
	int i,skip,position;

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
	dwWidth *= flag/8; //若是真彩色，则把宽度变为三倍
	int dwSize = dwHeight * dwWidth; //位图矩阵大小
	int dReadWidth = (dwWidth+3)/4*4; //实际存储的宽度
    
	//重新申请空间：
//	if(pBitmapData) delete pBitmapData;
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
	
	dwWidth /= flag / 8; //恢复图像的宽度:

	return true;
}

