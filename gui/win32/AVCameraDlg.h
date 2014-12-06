
// AVCameraDlg.h : ͷ�ļ�
//

#pragma once
#include "Camera.h"


// CAVCameraDlg �Ի���
class CAVCameraDlg : public CDialogEx
{
// ����
public:
	CAVCameraDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_AVCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	DECLARE_MESSAGE_MAP()
	
protected:
	void UpdateStatus(VideoMgr::CameraSatus status);

private:
	std::shared_ptr<VideoMgr::Camera> _camera;

};
