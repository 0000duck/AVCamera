#pragma once

#include "filter.hpp"
#include <opencv2\core\core.hpp>

namespace VideoMgr
{
	class ShowDateTimeFilter : public Filter
	{
	public:
		ShowDateTimeFilter(double x = 0.001, double y = 0.001);
		virtual ~ShowDateTimeFilter(){}

		virtual void process_frame(cv::Mat& frame) override;

		void set_x(double x){ _x = x; }
		double get_x(){ return _x; }
		void set_y(double y){ _y = y; }
		double get_y(){ return _y; }

	private:
		double _x;
		double _y;
	};
}