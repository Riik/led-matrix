#include <atomic>
#include <csignal>
#include <vector>
#include <chrono>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"
#include "fontToTexture2d.hpp"
#include "texturedTriangle2d.hpp"

#include "canvas2d.hpp"
#include "solidColorTriangle2d.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

int main(void) {
    signal(SIGINT, sigintHandler);

    MatrixScreen screen(4,4);
    MatrixDriver matrixDriver(spidev, screen, 60);
    Gfx2D::Canvas canvas(screen, PixelColor::off);

    std::chrono::time_point<std::chrono::steady_clock> lastWakeTime = std::chrono::steady_clock::now();

    std::size_t fontIndex = 0;
    Gfx2D::Texture texture = Gfx2D::fontToTexture(fontIndex);
    std::vector<Gfx2D::TexturedTriangle> vec4 {
        Gfx2D::TexturedTriangle({-0.75, -0.75}, {-0.75, 0.75}, {0.75, -0.75}, texture, {{0,0}, {1,0}, {0,1}}),
        Gfx2D::TexturedTriangle({0.75, 0.75}, {-0.75, 0.75}, {0.75, -0.75}, texture, {{1,1}, {1,0}, {0,1}})
    };

    float totalTimePassed = 0.0f;

    while(!halt) {
        std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> diffInSec = curTime - lastWakeTime;
        lastWakeTime = curTime;
        totalTimePassed += diffInSec.count();

        if (totalTimePassed > 1.0f) {
            totalTimePassed -= 1.0f;
            fontIndex++;
            if (fontIndex > Gfx2D::getMaxFontIndex()) {
                fontIndex = 0;
            }
            texture = Gfx2D::fontToTexture(fontIndex);
            vec4 = {
                Gfx2D::TexturedTriangle({-0.75, -0.75}, {-0.75, 0.75}, {0.75, -0.75}, texture, {{0,0}, {1,0}, {0,1}}),
                Gfx2D::TexturedTriangle({0.75, 0.75}, {-0.75, 0.75}, {0.75, -0.75}, texture, {{1,1}, {1,0}, {0,1}})
            };

        }
        for (auto& v : vec4) {
            canvas.addToFrame(v);
        }
        matrixDriver.setScreen(canvas.generateFrame());
    }
    screen.resetScreen(PixelColor::off);
    matrixDriver.setScreen(screen);
    return EXIT_SUCCESS;
}
