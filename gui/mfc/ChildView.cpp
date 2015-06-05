#include "stdafx.h"
#include "Camera.h"
#include "ChildView.h"

#include <opencv2/core/core.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

VideoMgr::Camera& CChildView::Camera()
{
	return _camera;
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::UpdateVideoFrame()
{
	if (_camera._status == VideoMgr::CameraSatus::EXITED) return;
	int width = 640, height = 480, channel = 3;
	if (_img.IsNull())
	{
		_img.Create(width, height, channel * 8);
	}
	else
	{
		if (_img.GetWidth() != width || _img.GetHeight() != height || _img.GetBPP() == channel)
		{
			_img.Destroy();
			_img.Create(width, height, channel * 8);
		}
	}

	cv::Mat mat;
	_camera.get_curr_frame( mat );
	if( mat.data == nullptr || _img.IsNull() ) return;

	//convert Mat to CImage
	uchar* ps;
	uchar* pimg = (uchar*)_img.GetBits();
	int step = _img.GetPitch();

	for( int i = 0; i < mat.rows; ++i )
	{
		ps = ( mat.ptr<uchar>( i ) );
		for( int j = 0; j < mat.cols; ++j )
		{
			memcpy( pimg + i*step, ps, mat.cols * 3 );
		}
	}
	
	if (_img.IsNull()) return;
	CClientDC dc(this);
	_img.Draw(dc.GetSafeHdc(), _dst_rect, Gdiplus::InterpolationModeBilinear);
}

void CChildView::UpdateStatus(VideoMgr::CameraSatus status)
{
	switch (status)
	{
	case VideoMgr::CREATED:
	case VideoMgr::STOPPED:

		break;
	case VideoMgr::PAUSED:

		break;
	case VideoMgr::RECORDING:

		break;
	}
}


void CChildView::OnDestroy()
{
	_camera.exit();
	Sleep(1000);
	// TODO: Add your message handler code here
	CWnd::OnDestroy();
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	GetClientRect(&_dst_rect);
}
