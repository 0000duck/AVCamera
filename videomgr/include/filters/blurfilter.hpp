#pragma once

#include "filter.hpp"
#include <opencv2\core\core.hpp>

namespace VideoMgr
{
	class BlurFilter : public Filter
	{
	public:
		BlurFilter(double ksize = 30);
		virtual ~BlurFilter(){}

		virtual void process_frame(cv::Mat& frame) override;

		void set_ksize(double ksize){ _ksize = ksize; }
		double get_ksize(){ return _ksize; }
	private:
		double _ksize;
	};
}
