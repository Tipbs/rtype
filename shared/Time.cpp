#include <mutex>

#ifdef SERVER
	std::mutex mutex;
	static auto time_since_last_tick = std::chrono::high_resolution_clock::now(); // voir si raylib utilise m�me chose

	float GetFrameTime()
	{
		std::scoped_lock lock(mutex);
		const auto now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<double>(now - time_since_last_tick)
			.count();
	}

float GetFrameTime() { return 1; }

void ResetFrameTime()
{
    std::scoped_lock lock(mutex);
    time_since_last_tick = std::chrono::high_resolution_clock::now();
}
#else
#include <raylib.h>
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint()
{
    return std::chrono::steady_clock::now();
}
