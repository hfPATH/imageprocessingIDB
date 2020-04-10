// FRect.h: interface for the CFRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRECT_H__3C612F60_5C0E_428B_9DF4_C61380A8D641__INCLUDED_)
#define AFX_FRECT_H__3C612F60_5C0E_428B_9DF4_C61380A8D641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFRect  
{
public:

	CFRect();
	// ���캯��
	CFRect(double l, double t, double r, double b);
	CFRect(CRect rct);

	virtual ~CFRect();	

	// �������캯��
	void operator = ( CFRect& rect);
	// ��ȡ���
	double Width ();
	// ��ȡ�߶�
	double Height();
	// ƫ��
	void   OffsetRect(double dXOffset,double dYOffset);
	// ��С
	void   DeflateRect(double l,double t, double r, double b);
	// �Ŵ�
	void   EnflateRect(double l,double t, double r, double b);
	// ����
	BOOL   InterSectRect(CFRect* rect1,CFRect* rect2);
	// �жϵ��Ƿ��ھ�����
	BOOL   PtInRect(double x, double y);
	// ����
	void   UnionRect(CFRect* rect1,CFRect* rect2);
	void   UnionRect(CFRect& rect1,CFRect& rect2);

	// ����
	double left;
	double right;
	double top;
	double bottom;
};

#endif // !defined(AFX_FRECT_H__3C612F60_5C0E_428B_9DF4_C61380A8D641__INCLUDED_)
