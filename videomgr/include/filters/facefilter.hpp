#pragma once

#include "filter.hpp"
#include "detectionface.hpp"

#include <opencv2\core\core.hpp>
#include <opencv2\objdetect\objdetect.hpp>

namespace VideoMgr
{
	class FaceFilter : public Filter
	{
	public:
		FaceFilter();
		virtual ~FaceFilter(){}

		virtual void process_frame(cv::Mat& frame) override;

	private:
		DetectionFace _detection_face;
	};
}
