// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#include <string>
#include <vector>

#ifdef _DEBUG
#pragma comment (lib, "opencv_core249d.lib")
#pragma comment (lib, "opencv_highgui249d.lib")
#pragma comment (lib, "opencv_objdetect249d.lib")
#pragma comment (lib, "opencv_video249d.lib")
#endif

#ifdef NDEBUG
#pragma comment (lib, "opencv_core249.lib")
#pragma comment (lib, "opencv_highgui249.lib")
#pragma comment (lib, "opencv_objdetect249.lib")
#pragma comment (lib, "opencv_video249.lib")
#endif