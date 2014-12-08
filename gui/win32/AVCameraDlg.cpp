
// AVCameraDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "win32.h"
#include "AVCameraDlg.h"
#include "afxdialogex.h"

#include <opencv2/core/core.hpp>

#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAVCameraDlg �Ի���

CAVCameraDlg::CAVCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAVCameraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAVCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAVCameraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CAVCameraDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CAVCameraDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CAVCameraDlg::OnBnClickedButtonStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAVCameraDlg ��Ϣ�������

BOOL CAVCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	_dc = GetDlgItem(IDC_VIDEO_PLAYBACK)->GetDC();

	_camera.reset(new VideoMgr::Camera());
	_camera->refresh_sign.connect(boost::bind(&CAVCameraDlg::UpdateVideoFrame, this));
	UpdateStatus(VideoMgr::CREATED);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAVCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAVCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CAVCameraDlg::UpdateStatus(VideoMgr::CameraSatus status)
{
	switch (status)
	{
	case VideoMgr::CREATED:
	case VideoMgr::STOPPED:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
		break;
	case VideoMgr::PAUSED:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
		break;
	case VideoMgr::RECORDING:
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PAUSE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(true);
		break;
	}
}

void CAVCameraDlg::OnBnClickedButtonStart()
{
	UpdateStatus(VideoMgr::RECORDING);
	int width = 640, height = 480, channel = 3, bit_rate = 1815484;
	if(_img.IsNull())
	{
		_img.Create(width, height, channel * 8);
	}
	else
	{
		if(_img.GetWidth() != width || _img.GetHeight() != height || _img.GetBPP() == channel)
		{
			_img.Destroy();
			_img.Create(width, height, channel * 8);
		}
	}
	_camera->start("d:\\test.mp4", 640, 480, 1815484);
}

void CAVCameraDlg::OnBnClickedButtonPause()
{
	UpdateStatus(VideoMgr::PAUSED);
	_camera->pause();
}

void CAVCameraDlg::OnBnClickedButtonStop()
{
	UpdateStatus(VideoMgr::STOPPED);
	_camera->stop();
}

void CAVCameraDlg::UpdateVideoFrame()
{
	cv::Mat mat;
	_camera->get_curr_frame(mat);
	if(mat.data == nullptr || _img.IsNull()) return;
	
	//convert Mat to CImage
	uchar* ps;
	uchar* pimg = (uchar*)_img.GetBits();
	int step = _img.GetPitch();
	for (int i = 0; i < mat.rows; ++i)
	{
		ps = (mat.ptr<uchar>(i));
		for ( int j = 0; j < mat.cols; ++j )
		{
			if ( mat.channels() == 3 ) //3 channels
			{
				for (int k = 0 ; k < 3; ++k )
				{
					*(pimg + i*step + j*3 + k ) = ps[j*3 + k];
				}
			}
		}
	}
	RECT pic_rect,dst_rect;
	GetDlgItem(IDC_VIDEO_PLAYBACK)->GetWindowRect(&pic_rect);
	dst_rect.left = dst_rect.top = 0;
	dst_rect.right = pic_rect.right - pic_rect.left;
	dst_rect.bottom = pic_rect.bottom - pic_rect.top;
	_img.Draw(_dc->GetSafeHdc(), dst_rect, Gdiplus::InterpolationModeBilinear);

}

void CAVCameraDlg::OnClose()
{
	_camera->exit();
	Sleep(500);
	ReleaseDC(_dc);
	CDialogEx::OnClose();
}
