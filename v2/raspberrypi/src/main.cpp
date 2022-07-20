#include <atomic>
#include <csignal>
#include <vector>
#include <chrono>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"

#include "canvas2d.hpp"
#include "solidColorTriangle2d.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

const float rotationSpeedRadPerSec = 3.0f;
const float middleRotationRadPerSec = 1.0f;

int main(void) {
    signal(SIGINT, sigintHandler);

    MatrixScreen screen(4,4);
    MatrixDriver matrixDriver(spidev, screen);
    Gfx2D::Canvas canvas(screen, PixelColor::off);

    std::chrono::time_point<std::chrono::steady_clock> lastWakeTime = std::chrono::steady_clock::now();

    std::vector<Gfx2D::SolidColorTriangle> vec {
        Gfx2D::SolidColorTriangle({-0.75, -0.75}, {-0.75, 0.75}, {0.75, -0.75}, PixelColor::on),
        Gfx2D::SolidColorTriangle({0.75, 0.75}, {-0.75, 0.75}, {0.75, -0.75}, PixelColor::off)
    };

    std::vector<Gfx2D::SolidColorTriangle> vec2 {
        Gfx2D::SolidColorTriangle({-0.50, -0.50}, {-0.50, 0.50}, {0.50, -0.50}, PixelColor::off),
        Gfx2D::SolidColorTriangle({0.50, 0.50}, {-0.50, 0.50}, {0.50, -0.50}, PixelColor::on)
    };

    std::vector<Gfx2D::SolidColorTriangle> vec3 {
        Gfx2D::SolidColorTriangle({-0.25, -0.25}, {-0.25, 0.25}, {0.25, -0.25}, PixelColor::on),
        Gfx2D::SolidColorTriangle({0.25, 0.25}, {-0.25, 0.25}, {0.25, -0.25}, PixelColor::off)
    };

    float totalRotationRad = 0.0f;

    while(!halt) {
        std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> diffInSec = curTime - lastWakeTime;
        lastWakeTime = curTime;
        float rotationRad = rotationSpeedRadPerSec * diffInSec.count();
        totalRotationRad += rotationRad;
        if (totalRotationRad >= 2*M_PI) {
            totalRotationRad -= 2*M_PI;
        }
        Gfx2D::TransformationMatrix rotation = Gfx2D::createRotationMatrix(totalRotationRad);
        std::vector<Gfx2D::SolidColorTriangle> transformedVec;
        // Weird two step thing here, but we are kindof stuck.
        // We transform the original objects into rotated ones. Someone has to hold on to those
        // Canvas cannot do it, since canvas only holds references to some interface and knows
        // nothing about the actual object. Therefore, store it in some new vector, transformedVec.
        // Butttt, push_back could invalidate old references if a resize triggers.
        // Therefore, two step: first push them all to some vector, then store references to the contents
        // of that vector in canvas. No memory issues, but very ugly code.
        for (auto& v : vec) {
            transformedVec.push_back(v.createTransformedTriangle(rotation));
        }
        for (auto& v : vec2) {
            transformedVec.push_back(v.createTransformedTriangle(rotation));
        }
        for (auto& v : vec3) {
            transformedVec.push_back(v.createTransformedTriangle(rotation));
        }
        for (auto& v : transformedVec) {
            canvas.addToFrame(v);
        }
        matrixDriver.setScreen(canvas.generateFrame());
    }
    screen.resetScreen(PixelColor::off);
    matrixDriver.setScreen(screen);
    return EXIT_SUCCESS;
}
