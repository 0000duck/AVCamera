#pragma once

#include "h264writer.hpp"
#include "filter.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <thread>
#include <memory>
#include <functional>
#include <vector>
#include <atomic>

namespace VideoMgr
{
	enum CameraSatus
	{
		CREATED   = 0,
		RECORDING = 1,
		PAUSED    = 2,
		STOPPED   = 4,
		EXITED    = 8,
	};
	class Camera
	{
	public:
		Camera();
		~Camera();
		
		int start(std::string file, int width, int height, int bit_rate); //start recording
		int pause(); //pause recording
		int stop();  //stop recording
		int exit();
		void get_curr_frame(cv::Mat& frame);
		std::vector<std::shared_ptr<Filter>>& Filters();
		
	private:
		void thread_task();	//do not use
	public:
		std::function<void()>                refresh_callback;
		CameraSatus                          _status;
	private:
		cv::Mat                              _last_frame;
		std::thread                          _camera_thread;
		std::shared_ptr<cv::VideoCapture>    _video;
		std::shared_ptr<H264Writer>          _h264;
		std::vector<std::shared_ptr<Filter>> _filters;
		std::atomic<CameraSatus>             _last_status;
		std::string                          _file;
	};

}
