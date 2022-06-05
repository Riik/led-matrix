#include <thread>

#include "frameLimiter.hpp"

FrameLimiter::FrameLimiter(const uint32_t framesPerSecond)
{
    if (framesPerSecond == 0) {
        timePerFrame = std::chrono::nanoseconds{0};
    } else {
        timePerFrame = std::chrono::nanoseconds{1000000000/framesPerSecond};
    }
    lastWakeTime = std::chrono::steady_clock::now();

}

void FrameLimiter::waitForNextFrame()
{
    if (timePerFrame == std::chrono::nanoseconds{0}) {
        return;
    }

    this->lastWakeTime += timePerFrame;
    if (this->lastWakeTime < std::chrono::steady_clock::now()) {
        this->lastWakeTime = std::chrono::steady_clock::now();
        return;
    } else {
        std::this_thread::sleep_until(this->lastWakeTime);
    }
}
