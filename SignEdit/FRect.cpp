// FRect.cpp: implementation of the CFRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFRect::CFRect()
{
	left = right = top = bottom = 0;
}

CFRect::~CFRect()
{

}
// ���캯��
CFRect::CFRect(CRect rct)
{
	left = rct.left;
	right = rct.right;
	top = rct.top;
	bottom = rct.bottom;
}
// ���캯��
CFRect::CFRect(double l, double t, double r, double b)
{
	left   = l;
	right  = r;
	top    = t;
	bottom = b;
}
// �������캯��
void CFRect::operator = ( CFRect& rect)
{
	left   = rect.left;
	right  = rect.right;
	top    = rect.top;
	bottom = rect.bottom;
}
// ��ȡ���
double CFRect::Width ()
{
	return right - left;
}
// ��ȡ�߶�
double CFRect::Height()
{
	return bottom - top;
}
// ƫ��
void CFRect::OffsetRect(double dXOffset,double dYOffset)
{
	left   += dXOffset;
	right  += dXOffset;

	top    += dYOffset;
	bottom += dYOffset;
}
// �Ŵ�
void CFRect::EnflateRect(double l,double t, double r, double b)
{
	left   -= l;
	right  += r;
	top    -= t;
	bottom += b;
}
// ��С
void CFRect::DeflateRect(double l,double t, double r, double b)
{
	left   += l;
	right  -= r;
	top    += t;
	bottom -= b;
}
// ����
BOOL   CFRect::InterSectRect(CFRect* rect1,CFRect* rect2)
{
	left   = max(  rect1->left,    rect2->left   );
	right  = min(  rect1->right,   rect2->right  );
	top    = max(  rect1->top,     rect2->top    );
	bottom = min(  rect1->bottom,  rect2->bottom );

	return (left <= right && top <= bottom );
}
// �жϵ��Ƿ��ھ�����
BOOL   CFRect::PtInRect(double x, double y)
{
	return ( x >= left && x <= right && y >= top && y <= bottom ); 
}
// ����
void   CFRect::UnionRect(CFRect* rect1,CFRect* rect2)
{
	left   = min(  rect1->left,    rect2->left   );
	right  = max(  rect1->right,   rect2->right  );
	top    = min(  rect1->top,     rect2->top    );
	bottom = max(  rect1->bottom,  rect2->bottom );
}
// ����
void   CFRect::UnionRect(CFRect& rect1,CFRect& rect2)
{
	left   = min(  rect1.left,    rect2.left   );
	right  = max(  rect1.right,   rect2.right  );
	top    = min(  rect1.top,     rect2.top    );
	bottom = max(  rect1.bottom,  rect2.bottom );
}