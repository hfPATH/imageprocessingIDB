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
	// 构造函数
	CFRect(double l, double t, double r, double b);
	CFRect(CRect rct);

	virtual ~CFRect();	

	// 拷贝构造函数
	void operator = ( CFRect& rect);
	// 获取宽度
	double Width ();
	// 获取高度
	double Height();
	// 偏移
	void   OffsetRect(double dXOffset,double dYOffset);
	// 缩小
	void   DeflateRect(double l,double t, double r, double b);
	// 放大
	void   EnflateRect(double l,double t, double r, double b);
	// 交集
	BOOL   InterSectRect(CFRect* rect1,CFRect* rect2);
	// 判断点是否在矩形内
	BOOL   PtInRect(double x, double y);
	// 并集
	void   UnionRect(CFRect* rect1,CFRect* rect2);
	void   UnionRect(CFRect& rect1,CFRect& rect2);

	// 数据
	double left;
	double right;
	double top;
	double bottom;
};

#endif // !defined(AFX_FRECT_H__3C612F60_5C0E_428B_9DF4_C61380A8D641__INCLUDED_)
