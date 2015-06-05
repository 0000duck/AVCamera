
#include "camera.hpp"
#include "h264writer.hpp"
#include "filter.hpp"
#include "util.hpp"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <functional>
#include <memory>


namespace VideoMgr
{
	Camera::Camera()
		: _status(CREATED)
	{
		av_register_all();

		_video = std::make_shared<cv::VideoCapture>(0);
		_camera_thread = std::thread(std::bind(&Camera::thread_task, this));
	}

	Camera::~Camera()
	{
		_camera_thread.join();
		_video->release();
		_h264.reset();
		_filters.clear();
	}


	void Camera::thread_task()
	{
		cv::Mat frame;
		unsigned long timer, curr_time;
		timer = curr_time = get_now_time();
		unsigned long duration = 0;
		while (true)
		{
			if( _status == CREATED)
			{
				timer = curr_time = get_now_time();
				if (!_video->isOpened())
				{
					custom_sleep(1800); continue;
				}
				*_video >> frame;
				// Filter
				for (auto& theFilter : _filters)
				{
					theFilter->process_frame(frame);
				}

				curr_time = get_now_time();
				if (_last_status == PAUSED)
				{
					timer = curr_time - 40;
					_last_status = _status;
				}
				duration = curr_time - timer;
				frame.copyTo(_last_frame);//backup to last_frame everyone in recording

				if (refresh_callback) refresh_callback();

			}
			else if(_status == PAUSED)
			{
				curr_time = get_now_time();
				custom_sleep(1800); continue;
			}
			else if(_status == STOPPED)
			{
				if(_last_status == RECORDING
					|| _last_status == PAUSED)
				{
					_h264->close();
					_h264.reset();
					_last_status = _status;
				}
				timer = curr_time = get_now_time();
				custom_sleep(1800); continue;
			}
			else if(_status == RECORDING)
			{
				*_video >> frame;
				// Filter
				//_filter->show_datetime(frame);
				//

				curr_time = get_now_time();
				//cv::imshow("video", frame); for test
				if(_last_status == PAUSED)
				{
					timer = curr_time - 40;
					_last_status = _status;
				}
				duration = curr_time - timer;

				_h264->write(frame, duration > 40 ? duration : 40);
				if(duration < 40) custom_sleep(40 - duration);
				frame.copyTo(_last_frame);//backup to last_frame everyone in recording

				timer = curr_time;
			}
			else if(_status == EXITED)
			{
				if(_last_status == STOPPED
					|| _last_status == CREATED)
				{
					;
				}
				break;
			}

		}
	}


	int Camera::start( std::string file, int width, int height, int bit_rate )
	{
		//create h264writer
		_h264.reset(new H264Writer());
		if(!_h264->create(file, width, height, bit_rate))
			return static_cast<int>(CREATED);

		//create last frame mat
		_last_frame.create(height, width, CV_8UC3);

		_last_status = _status;
		if(_status == CREATED)
		{
			_status = RECORDING;
		}
		else if(_status == PAUSED)
		{
			_status = RECORDING;
		}
		else if(_status == STOPPED)
		{
			_status = RECORDING;
		}
		return static_cast<int>(_status);
	}

	int Camera::pause()
	{
		if(!_video->isOpened()) return 0;
		_last_status = _status;
		if(_status == RECORDING)
		{
			_status = PAUSED;
		}
		return static_cast<int>(_status);
	}

	int Camera::stop()
	{
		if(!_video->isOpened()) return 0;
		_last_status = _status;
		_last_frame.release();
		if(_status == CREATED)
		{
			_status = STOPPED;
		}
		else if(_status == RECORDING)
		{
			_status = STOPPED;
		}
		else if(_status == PAUSED)
		{
			_status = STOPPED;
		}
		return static_cast<int>(_status);
	}

	int Camera::exit()
	{
		refresh_callback = nullptr;
		stop();
		_status = EXITED;
		return static_cast<int>(_status);
	}

	void Camera::get_curr_frame( cv::Mat& frame )
	{
		frame = _last_frame;//it is not copy memory
	}

	std::vector<std::shared_ptr<Filter>>& Camera::Filters()
	{
		return _filters;
	}

}
