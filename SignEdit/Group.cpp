#include "stdafx.h"
#include "Group.h"
#include "Motion.hpp"
#include "cvutils.hpp"


#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))

CGroup::CGroup(double dConDis, double dSegDis)
{
	connectionDistance = dConDis;
	segmentationDistance = dSegDis;
}


CGroup::~CGroup()
{
}


void CGroup::Clear()
{
	for ( auto iter = m_objects.begin(); iter != m_objects.end(); iter++ )
		iter->clear();
	m_objects.clear();
}

void CGroup::AddTraj( const FeatureTrajectoryPtr &ptr )
{
	std::vector<std::vector<FeatureTrajectoryPtr>>::iterator objIter0;
	std::vector<std::vector<FeatureTrajectoryPtr>>::iterator objIter = m_objects.begin();

	int nSize = ptr->positions->size();
	cv::Point2f pt0 = ( *( ptr->positions ) )[0];
	cv::Point2f pt1 = ( *( ptr->positions ) )[nSize - 1];
	if (  norm( pt0 - pt1 )  <  8  )
		return;

	float dAngle0 = ptr->getMainDirc();

	bool bAlready = false;
	while ( objIter != m_objects.end() )
	{
		bool bConnect = false;
		for ( std::vector<FeatureTrajectoryPtr>::iterator ptIter = objIter->begin(); ptIter != objIter->end(); ptIter++ )
		{
//			nSize = ( *ptIter )->positions->size();
//			cv::Point2f pt2 = ( *( ( *ptIter )->positions ) )[0];
//			cv::Point2f pt3 = ( *( ( *ptIter )->positions ) )[nSize - 1];
//
//			if ( ( fabs( pt0.x - 778.0 ) < 2 && fabs( pt0.y - 245.0 ) < 2 &&
//				fabs( pt2.x - 784.0 ) < 2 && fabs( pt2.y - 258.0 ) < 2 ) ||
//				( fabs( pt2.x - 778.0 ) < 2 && fabs( pt2.y - 245.0 ) < 2 &&
//				fabs( pt0.x - 784.0 ) < 2 && fabs( pt0.y - 258.0 ) < 2 ) )
//			{
//				nSize = nSize;
//			}

			int firstInstant = max( ptr->getFirstInstant(), ( *ptIter )->getFirstInstant());
			int lastInstant  = min( ptr->getLastInstant(),  ( *ptIter )->getLastInstant());
			if ( firstInstant >= lastInstant )
				continue;

			if ( !( *ptIter )->minMaxSimilarity( *ptr, firstInstant, lastInstant, connectionDistance, segmentationDistance ) )
				continue;

			float dAngle1 = ( *ptIter )->getMainDirc();
			
			if ( fabs( dAngle1 - dAngle0 ) > 15 )
				continue;

			bConnect = true;
			break;
		}

		if ( !bConnect )
		{
			objIter++;
			continue;
		}

		if ( !bAlready )
		{
			objIter->push_back( ptr );
			objIter0 = objIter;
			bAlready = true;
			objIter++;
		}
		else // combine
		{
			objIter0->insert( objIter0->end(), objIter->begin(), objIter->end() );
			objIter = m_objects.erase( objIter );
		}
	}
	if ( !bAlready )
	{
		std::vector<FeatureTrajectoryPtr> pt;
		pt.push_back( ptr );
		m_objects.push_back( pt );		
	}
}


void CGroup::GetObjBox( std::vector<RECT>& rects, unsigned int nFrameID )
{
	rects.clear();

	std::vector<std::vector<FeatureTrajectoryPtr>>::iterator objIter = m_objects.begin();
	for ( ; objIter != m_objects.end(); objIter++ )
	{
		int nl, nr, nt, nb;
		nl = nt = 9999;
		nr = nb = -1;

		bool bFound = false;
		for ( std::vector<FeatureTrajectoryPtr>::iterator ptIter = objIter->begin(); ptIter != objIter->end(); ptIter++ )
		{
			cv::Point2f pt;
			if ( !( *ptIter )->getPts( pt, nFrameID ) )
				continue;
			nl = nl > pt.x ? pt.x : nl;
			nr = nr < pt.x ? pt.x : nr;
			nt = nt > pt.y ? pt.y : nt;
			nb = nb < pt.y ? pt.y : nb;
			bFound = true;
		}

		if ( !bFound )
			continue;

		RECT rct;
		rct.left = nl;
		rct.right = nr;
		rct.top = nt;
		rct.bottom = nb;
		rects.push_back( rct );
	}
}