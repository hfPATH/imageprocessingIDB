//////////////////////////////////////////////////////////////////////////////////////////
//   TrackerParameters.h
//     ��ͷ�ļ��ж����˺͸������йص�һЩ���������ڹ���
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

enum TRACK_TYPE   //���ø�������
{ 
	TT_KL_AFFINE,  //���������ռ�ͻ��ڷ���ģ�͸���
	TT_SVD_AFFINE, //����ֵ�ֽ�ͻ��ڷ���ģ�͸���
	TT_AFFINE      //���ڷ���ģ�͸���
};

enum TRACK_TEPLATE_UPDATE_TYPE //����ģ���������
{
     TTUT_PWOER,                       //��Ȩ�ۼ�
	 TTUT_MOVING_TARGET_SEGMENT,       //�˶�Ŀ����
	 TTUT_EIGENSPACE_TRAINNING_OFFLINE,//�����ռ��ع����ѻ�ѵ��
	 TTUT_EIGENSPACE_TRAINNING_ONLINE  //�����ռ��ع�������ѵ��
};

enum TRACK_SOURCE_TYPE    //��������Դ����
{
	TST_USB_CAMERA,       //USB�����
	TST_HARDDISK_FILE     //Ӳ���ļ�����
};

#define CAPTION "��������ϵͳUSB��ʾ����"


