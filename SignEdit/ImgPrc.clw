; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imgprc.h"
LastPage=0

ClassCount=8
Class1=CChildFrame
Class2=CDlgFiles
Class3=hist_draw
Class4=CImgPrcApp
Class5=CAboutDlg
Class6=CImgPrcDoc
Class7=CImgPrcView
Class8=CMainFrame

ResourceCount=5
Resource1=IDD_DIALOGFILES
Resource2=IDR_MAINFRAME
Resource3=IDR_IMGPRCTYPE
Resource4=IDD_ABOUTBOX
Resource5=IDD_Hist

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CDlgFiles]
Type=0
BaseClass=CDialog
HeaderFile=DlgFiles.h
ImplementationFile=DlgFiles.cpp

[CLS:hist_draw]
Type=0
BaseClass=CDialog
HeaderFile=hist_draw.h
ImplementationFile=hist_draw.cpp

[CLS:CImgPrcApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImgPrc.h
ImplementationFile=ImgPrc.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImgPrc.cpp
ImplementationFile=ImgPrc.cpp
LastObject=ID_FILE_OPEN

[CLS:CImgPrcDoc]
Type=0
BaseClass=CDocument
HeaderFile=ImgPrcDoc.h
ImplementationFile=ImgPrcDoc.cpp

[CLS:CImgPrcView]
Type=0
BaseClass=CScrollView
HeaderFile=ImgPrcView.h
ImplementationFile=ImgPrcView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_DIALOGFILES]
Type=1
Class=CDlgFiles
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_BUTTON1,button,1342242816
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT2,edit,1350631552
Control8=IDC_BUTTON2,button,1342242816

[DLG:IDD_Hist]
Type=1
Class=hist_draw
ControlCount=0

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IMGPRCTYPE]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_OPEN2
Command4=ID_FILE_CLOSE
Command5=ID_FILE_SAVE
Command6=ID_FILE_SAVE_AS
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_VIEW_TOOLBAR
Command17=ID_VIEW_STATUS_BAR
Command18=ID_WINDOW_NEW
Command19=ID_WINDOW_CASCADE
Command20=ID_WINDOW_TILE_HORZ
Command21=ID_WINDOW_ARRANGE
Command22=ID_APP_ABOUT
Command23=ID_PROCESS
Command24=ID_LHD123
Command25=ID_1WEIFEN
Command26=ID_ZHI
Command27=ID_LAP
Command28=ID_JUNHENG
Command29=ID_HU
Command30=ID_TRANS
Command31=ID_Test
Command32=ID_Morph
Command33=ID_LinearFilter
Command34=ID_32799
Command35=ID_double_mean
Command36=ID_Robinson
Command37=ID_Fusion
Command38=ID_DFT
Command39=ID_MENUITEM32793
Command40=ID_SHARPEN_GRADIENT
Command41=ID_Gaus_Segment
Command42=ID_Canny
CommandCount=42

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

