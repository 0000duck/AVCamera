#include "filters/facefilter.hpp"
#include <vector>

#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core/types_c.h>
#include <opencv2/imgproc/imgproc.hpp>

VideoMgr::FaceFilter::FaceFilter()
/*: _face_cascade_path("resources/haarcascade_frontalface_alt2.xml"),
_eye_cascade_path("resources/haarcascade_eye_tree_eyeglasses.xml")*/
{
	/*bool ret = _face_cascade.load(_face_cascade_path);
	ret = _eyes_cascade.load(_eye_cascade_path);*/
}

void VideoMgr::FaceFilter::process_frame(cv::Mat& frame)
{
	/*std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cv::cvtColor(frame, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	_face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(30, 30));

	for (std::size_t i = 0; i < faces.size(); i++)
	{
	cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
	ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);

	cv::Mat faceROI = frame_gray(faces[i]);
	std::vector<cv::Rect> eyes;

	_eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0);

	for (std::size_t j = 0; j < (eyes.size() >= 2 ? 2 : eyes.size()); j++)
	{
	cv::Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
	int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
	cv::circle(frame, center, radius, cv::Scalar(255, 0, 0), 4, 8, 0);
	}
	}*/
	if (frame.empty()) return;
	std::vector<cv::Rect> faces;
	_detection_face.detect(frame, faces);
	for (std::size_t i = 0; i < faces.size(); i++)
	{
		cv::Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, cv::Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar(255, 0, 255), 4, 8, 0);
	}

}
