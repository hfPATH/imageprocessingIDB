// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0AD300AB_BD68_44B0_AE10_5024819AF478__INCLUDED_)
#define AFX_STDAFX_H__0AD300AB_BD68_44B0_AE10_5024819AF478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0AD300AB_BD68_44B0_AE10_5024819AF478__INCLUDED_)
#include <afxcontrolbars.h>
#include <map>
//map
typedef std::map<CString, int> CBorderTypeMap;

#define WM_UDDATE_BITMAP (WM_USER + 1)		//���´���ͼƬ
#define WM_MORPHOLOGY (WM_USER + 2)			//��̬ѧ
#define WM_HISTOGRAM (WM_USER + 3)			//ֱ��ͼ����
#define WM_BINARYREGION (WM_USER + 4)		//��ͨ��ɸѡ
#define WM_POLYGON (WM_USER + 5)			//��Ӷ����
#define WM_CONERDETECTOR (WM_USER + 6)		//�ǵ�����
#define WM_MSER	(WM_USER + 7)				//mser
#define WM_SIFT (WM_USER + 8)				//SIFT����
#define WM_SMOOTH (WM_USER + 9)      // Smooth
#define WM_LOGANDDOG (WM_USER + 10)   //LOG and DOG
#define WM_COLORCVT (WM_USER+11)		//ɫ��ת��
#define WM_THRESHOLDSEG (WM_USER+12)			//��ֵ�ָ�
#define WM_ADAPTIVETHRESHOLD (WM_USER+13)	//����Ӧ��ֵ
#define WM_ITERATIVETHRESHOLD (WM_USER+14)	//������ֵ�ָ�
#define WM_STANDARDHOFFTRAN (WM_USER+15)		//��׼���任
#define WM_PPHT (WM_USER+16)					//�ۼƸ��ʻ��任
#define WM_HOUGHCIRCLETRAN (WM_USER+17)		//���Բ�任

//edge
#define WM_OPENCVSOBEL (WM_USER + 18)
#define WM_ROBERT (WM_USER + 19)
#define WM_OPENCVCANNY (WM_USER + 20)
#define WM_CANCELOPERA (WM_USER + 21)
#define WM_OPENCVLAPLACIAN (WM_USER + 22)
#define WM_GABOROPERATOR (WM_USER + 23)
#define WM_FEATURETRACK (WM_USER + 24)

