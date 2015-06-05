#include "filters/blurfilter.hpp"
#include "opencv2/imgproc/imgproc.hpp"

VideoMgr::BlurFilter::BlurFilter(double ksize /*= 30*/)
	: _ksize(ksize)
{
}

void VideoMgr::BlurFilter::process_frame(cv::Mat& frame)
{
	cv::blur(frame, frame, cv::Size((int)_ksize, (int)_ksize), cv::Point(-1, -1));
}
