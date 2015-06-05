#pragma once

#include <opencv2/core/core.hpp>

namespace VideoMgr
{
	class Filter
	{
	public:
		Filter(){};
		virtual ~Filter(){};

		virtual void process_frame(cv::Mat& frame) = 0;
	};

}
