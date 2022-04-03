#include <chrono>

class FrameLimiter {

    /* zero is a special case, in that case there will be no sleeping. */
    FrameLimiter(const unsigned int framesPerSecond);

    /* Puts the calling thread to sleep until the next frame. */
    void waitForNextFrame();

private:
    std::chrono::time_point<std::chrono::steady_clock> lastWakeTime;
    std::chrono::nanoseconds timePerFrame;
};


