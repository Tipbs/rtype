#include <mutex>
#ifndef SERVER
#include <raylib.h>
#endif

#ifdef SERVER
static auto time_since_last_tick =
    std::chrono::high_resolution_clock::now(); // voir si raylib utilise m�me
                                               // chose
float GetFrameTime();
void ResetFrameTime();
#endif // !SERVER

std::chrono::steady_clock::time_point GetTimePoint();
