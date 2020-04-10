//////////////////////////////////////////////////////////////////////////////////////////
//   TrackerParameters.h
//     此头文件中定义了和跟踪其有关的一些参数，用于共享
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

enum TRACK_TYPE   //设置跟踪类型
{ 
	TT_KL_AFFINE,  //卡洛特征空间和基于仿射模型跟踪
	TT_SVD_AFFINE, //奇异值分解和基于仿射模型跟踪
	TT_AFFINE      //基于仿射模型跟踪
};

enum TRACK_TEPLATE_UPDATE_TYPE //设置模板更新类型
{
     TTUT_PWOER,                       //加权累加
	 TTUT_MOVING_TARGET_SEGMENT,       //运动目标检测
	 TTUT_EIGENSPACE_TRAINNING_OFFLINE,//特征空间重构，脱机训练
	 TTUT_EIGENSPACE_TRAINNING_ONLINE  //特征空间重构，在线训练
};

enum TRACK_SOURCE_TYPE    //跟踪数据源类型
{
	TST_USB_CAMERA,       //USB摄像机
	TST_HARDDISK_FILE     //硬盘文件序列
};

#define CAPTION "特征跟踪系统USB显示窗口"


