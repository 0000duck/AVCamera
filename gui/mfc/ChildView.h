#pragma once

// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:
	VideoMgr::Camera& Camera();

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();
	void UpdateVideoFrame();
	void UpdateStatus(VideoMgr::CameraSatus status);

	// Generated message map functions
protected:
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	VideoMgr::Camera _camera;
	CImage _img;
	RECT _dst_rect;
	
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

