#pragma once
#include "stdafx.h"
#include "ImgPrc.h"
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <Memory>
#include "Motion.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

typedef std::shared_ptr<FeatureTrajectory> FeatureTrajectoryPtr;

struct FeaturePointMatch {
	FeatureTrajectoryPtr feature;
	int pointNum;

	FeaturePointMatch( FeatureTrajectoryPtr _feature, const int& _pointNum ):
		feature( _feature ), pointNum( _pointNum ) {}
};

class CGroup
{
public:
	CGroup( double dConDis, double dSegDis );
	~CGroup();

	void Clear();

	void AddTraj( const FeatureTrajectoryPtr &ptr);


	void GetObjBox( std::vector<RECT>& rects, unsigned int nFrameID);

	int GetDirection( std::vector<FeatureTrajectoryPtr> &ptr)
	{
		return 1; // 0 : up-down;  1 :  left-right
	};

	std::vector<std::vector<FeatureTrajectoryPtr>> m_objects;
	double connectionDistance, segmentationDistance;
};

