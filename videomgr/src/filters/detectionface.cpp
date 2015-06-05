#include "filters/detectionface.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/gpu/gpu.hpp>

DetectionFace::DetectionFace(bool useCPU/* = true*/)
    : _useCPU(useCPU)
    , _cascadeFileName("resources/haarcascade_frontalface_alt_tree.xml")
    , _cpu_cascade(nullptr)
    , _gpu_cuda_cascade(nullptr)
    , _gpu_ocl_cascade(nullptr)
{
    if(!_useCPU)
    {
        int canCuda = cv::gpu::getCudaEnabledDeviceCount();
        if(canCuda > 0)
        {
            //use cuda
            _gpu_cuda_cascade = new cv::gpu::CascadeClassifier_GPU();
            if(!_gpu_cuda_cascade->load(_cascadeFileName))
            {
                std::cout << "ERROR: Could not load classifier cascade by GPU(CUDA): " << _cascadeFileName << std::endl;
            }
            else return;
        }
        //use ocl
        _gpu_ocl_cascade = new cv::ocl::OclCascadeClassifier();
        if(!_gpu_ocl_cascade->load(_cascadeFileName))
        {
            std::cout << "ERROR: Could not load classifier cascade by GPU(OpenCL): " << _cascadeFileName << std::endl;
            _useCPU = true;
        }
        else return;
    }
    //use cpu
    _cpu_cascade = new cv::CascadeClassifier();
    if(!_cpu_cascade->load(_cascadeFileName))
        std::cout << "ERROR: Could not load classifier cascade by CPU: " << _cascadeFileName << std::endl;
}

DetectionFace::~DetectionFace()
{
    if(_cpu_cascade) delete _cpu_cascade;
    if(_gpu_cuda_cascade) delete _gpu_cuda_cascade;
    if(_gpu_ocl_cascade) delete _gpu_ocl_cascade;
}

bool DetectionFace::detect( cv::Mat& img, std::vector<cv::Rect>& faces )
{
    if(_useCPU)
        return detectCPU(img, faces);
    else
    {
        if(_gpu_cuda_cascade)
            return detectGPUCuda(img, faces);
        else if(_gpu_ocl_cascade)
            return detectGPUOcl(img, faces);
    }
}

bool DetectionFace::detectCPU(cv::Mat& img, std::vector<cv::Rect>& faces)
{
    if(nullptr == _cpu_cascade || _cpu_cascade->empty()) return false;
    cv::Mat gray_img;
    cvtColor(img, gray_img, CV_BGR2GRAY);
    equalizeHist(gray_img, gray_img);
    _cpu_cascade->detectMultiScale(img, faces, 1.1,
        3, 0 | CV_HAAR_SCALE_IMAGE,
		cv::Size(30, 30), cv::Size(0, 0));
    return true;
}

bool DetectionFace::detectGPUCuda(cv::Mat& img, std::vector<cv::Rect>& faces)
{
    if(nullptr == _gpu_cuda_cascade || _gpu_cuda_cascade->empty()) return false;
    cv::gpu::GpuMat ocl_img(img), ocl_gray_img, obj_img;
    cv::gpu::cvtColor(ocl_img, ocl_gray_img, CV_BGR2GRAY);
    cv::gpu::equalizeHist( ocl_gray_img, ocl_gray_img );
    _gpu_cuda_cascade->detectMultiScale( ocl_gray_img, obj_img
		, 1.1, 3, cv::Size(30, 30));
    return true;
}

bool DetectionFace::detectGPUOcl(cv::Mat& img, std::vector<cv::Rect>& faces)
{
    if(nullptr == _gpu_ocl_cascade || _gpu_ocl_cascade->empty()) return false;
    cv::ocl::oclMat ocl_img(img), ocl_gray_img;
    cv::ocl::cvtColor(ocl_img, ocl_gray_img, CV_BGR2GRAY);
    cv::ocl::equalizeHist( ocl_gray_img, ocl_gray_img );
    _gpu_ocl_cascade->detectMultiScale( ocl_gray_img, faces
        , 1.1, 3, 0|CV_HAAR_SCALE_IMAGE
		, cv::Size(30, 30), cv::Size(0, 0));
    return true;
}
