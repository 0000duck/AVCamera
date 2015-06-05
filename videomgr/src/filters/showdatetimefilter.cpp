#include "filters/showdatetimefilter.hpp"
#include <time.h>

VideoMgr::ShowDateTimeFilter::ShowDateTimeFilter(double x /*= 0.1*/, double y /*= 0.1*/)
	: _x(x), _y(y)
{
}

void VideoMgr::ShowDateTimeFilter::process_frame(cv::Mat& frame)
{
	time_t tt = time(NULL);
	tm* t = localtime(&tt);
	char timestr[30];
	sprintf(timestr, "%d-%02d-%02d %02d:%02d:%02d",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);
	cv::putText(frame, std::string(timestr), cv::Point((int)(_x*frame.cols), (int)(_y*frame.rows)), cv::FONT_HERSHEY_PLAIN, 1.5, cv::Scalar(0, 255, 0));
}
