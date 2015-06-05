
#if defined(_WIN32) || defined(_WIN64)
#include <thread>
#include <ctime>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

unsigned long get_now_time()
{
#if defined(_WIN32) || defined(_WIN64)
    return static_cast<unsigned long>(std::time(nullptr));
#else
	timeval now;
	gettimeofday(&now, NULL);
	return now.tv_usec/1000 + now.tv_sec*1000;
#endif
}

void custom_sleep(unsigned long millisecond)
{
#if defined(_WIN32) || defined(_WIN64)
    std::this_thread::sleep_for( std::chrono::milliseconds( millisecond ) );
#else
	usleep(millisecond*1000);
#endif
}
