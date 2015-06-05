#pragma once
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/ocl/ocl.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <opencv2/objdetect/objdetect.hpp>

class DetectionFace
{
public:
    DetectionFace( bool useCPU = true );
    ~DetectionFace();

	bool detect(cv::Mat& img, std::vector<cv::Rect>& faces);

protected:
	bool detectCPU(cv::Mat& img, std::vector<cv::Rect>& faces);
	bool detectGPUOcl(cv::Mat& img, std::vector<cv::Rect>& faces);
	bool detectGPUCuda(cv::Mat& img, std::vector<cv::Rect>& faces);

private:
	cv::CascadeClassifier* _cpu_cascade;
    cv::gpu::CascadeClassifier_GPU* _gpu_cuda_cascade;
	cv::ocl::OclCascadeClassifier* _gpu_ocl_cascade;
    bool _useCPU;
    const std::string _cascadeFileName;
};