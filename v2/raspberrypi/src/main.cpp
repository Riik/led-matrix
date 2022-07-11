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

    const std::string text = "Hoi, Rik!";
    // Translate the text into a bunch of textures
    std::vector<Gfx2D::Texture> textTextures;
    for (const char &ch : text) {
        textTextures.emplace_back(Gfx2D::fontToTexture(ch));
    }
    // Now we need a bunch of texturedTriangles to draw the text on
    float currentXCoordinate = -1.0f;
    std::vector<Gfx2D::TexturedTriangle> textTriangles;
    for (const Gfx2D::Texture& texture : textTextures) {
        textTriangles.push_back(Gfx2D::TexturedTriangle({currentXCoordinate, -1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1},
                texture, {{0,0}, {0,1}, {1,0}}));
        textTriangles.push_back(Gfx2D::TexturedTriangle({currentXCoordinate + 2.0f, 1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1},
                texture, {{1,1}, {0,1}, {1,0}}));
        currentXCoordinate += 2.0f;
    }
    currentXCoordinate += 1.0f;

    std::chrono::time_point<std::chrono::steady_clock> lastWakeTime = std::chrono::steady_clock::now();
    constexpr float textSpeedPerSec = 2.0f;
    constexpr float startPosition = -2.0f;
    float totalMovement = startPosition;

    while(!halt) {
        std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> diffInSec = curTime - lastWakeTime;
        lastWakeTime = curTime;
        totalMovement += diffInSec.count() * textSpeedPerSec;
        if (totalMovement > currentXCoordinate) {
            totalMovement = startPosition;
        }
        Gfx2D::TransformationMatrix translation = Gfx2D::createTranslationMatrix(-totalMovement, 0.0f);

        std::vector<Gfx2D::TexturedTriangle> transformedTriangles;

        for (const Gfx2D::TexturedTriangle& triangle : textTriangles) {
            transformedTriangles.emplace_back(triangle.createTransformedTriangle(translation));
        }

        for (const Gfx2D::TexturedTriangle& triangle : transformedTriangles) {
            canvas.addToFrame(triangle);
        }
        matrixDriver.setScreen(canvas.generateFrame());
    }
    screen.resetScreen(PixelColor::off);
    matrixDriver.setScreen(screen);
    return EXIT_SUCCESS;
}
