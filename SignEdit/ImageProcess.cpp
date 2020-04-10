#include "stdafx.h"

////////////////////////////////////////////////////
//
//  ����������ת��Ϊ�£ͣ�ͼ������:
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
//  ���£ͣ�ͼ������ת��Ϊ��������:
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
	pbi->bmiHeader.biSize = sizeof(BITMAPINFO)-4;//�����Ǻڰ�ͼ���ǲ�ͼ����40
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
	
	//����256ɫͼ����д��ɫ��
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
//��һά����ͼ������д���ļ�
// FileName �ļ���
// pBitmapData һά����ָ��
// dwWidth,dwHeight ͼ��Ŀ�Ⱥͳ���
// flag : ͼ�������
////////////////////////////////////////////////////////////////////
bool WriteBMPFileFromVector(CString &FileName, BYTE *pBitmapData,int dwHeight,int dwWidth,WORD flag)
{
	//������Ч�Լ�⣺
	if(!pBitmapData) 
		return FALSE;
	if((flag!=8)&&(flag!=24))
		return FALSE; 
	
	if(flag == 24)
		dwWidth *= 3;
	
	int dwWriteWidth = (dwWidth+3)/4*4;
	int dwWriteSize = dwHeight * dwWriteWidth;
	
	//���ļ�:
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
	BitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFO)-4;//�����Ǻڰ�ͼ���ǲ�ͼ����40
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
	
	//����256ɫͼ����д��ɫ��
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
	
	//д�ļ���
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
//  ��һ�����Ҷ�ͼ��ķ�Χ��
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
// ���ڴ���װ��ͼ�����ݵ�ָ���ľ���: ��ֻ��256ɫ��24λ���ɫ��
// FileName ��ͼ������
// ppBitmapData �Ǵ��洢��ͼ��һά������ָ���ַ
// dwWidth , dwHeight �ֱ���ͼ��Ŀ�Ⱥͳ���
// flag ����ͼ��λ����8����24��
////////////////////////////////////////////////////////////////////////////
bool LoadVectorFromBMPFile(CString &FileName, BYTE *&pBitmapData, int &dwHeight, int &dwWidth,WORD &flag)
{
	int i,skip,position;

	//���ƶ�ͼ���ļ�FileName��
	CFile File;  
	if(!File.Open(FileName,CFile::modeRead|CFile::typeBinary)){
		//AfxMessageBox(" �򲻿���LoadBitmap"+FileName);
		return FALSE;
	}
	//���ļ�ͷ��
	BITMAPFILEHEADER bfh;
	File.Read(&bfh,sizeof(BITMAPFILEHEADER));
	if(bfh.bfType!=0x4d42)  //'BM' ����bmpλͼ������
		return FALSE;

	int ii= sizeof(BITMAPFILEHEADER);
	//��λͼ��Ϣͷ��
	BITMAPINFO bmi;
	File.Read(&bmi,sizeof(BITMAPINFO));
	dwHeight = bmi.bmiHeader.biHeight;
	dwWidth = bmi.bmiHeader.biWidth;
	//1��4��8��24 �ֱ��Ӧ2��16��256�����ɫ
	flag = bmi.bmiHeader.biBitCount; 
	dwWidth *= flag/8; //�������ɫ����ѿ�ȱ�Ϊ����
	int dwSize = dwHeight * dwWidth; //λͼ�����С
	int dReadWidth = (dwWidth+3)/4*4; //ʵ�ʴ洢�Ŀ��
    
	//��������ռ䣺
//	if(pBitmapData) delete pBitmapData;
	pBitmapData = (BYTE*) new BYTE[dwSize];
	if(!pBitmapData) return FALSE;

	//����ÿ����ת���ֽ���
	skip = dReadWidth - dwWidth; 
	//���ó�ʼ�ľ�����λ�ã�
	
	position = dwSize - dwWidth;
	//���ļ���
	File.Seek(bfh.bfOffBits,0);
	for(i=0;i<dwHeight;i++)
	{
		File.Read(pBitmapData+position,dwWidth);	
		File.Seek(skip,1);
		position -= dwWidth; //����ָ��
	}

	File.Close();
	
	dwWidth /= flag / 8; //�ָ�ͼ��Ŀ��:

	return true;
}

