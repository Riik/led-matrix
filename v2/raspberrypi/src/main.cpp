#include <atomic>
#include <csignal>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <iostream>
#include <memory>
#include <random>

#include "matrixScreen.hpp"
#include "matrixDriver.hpp"
#include "matrixDriverSpi.hpp"
#include "matrixDriverNcurses.hpp"
#include "gpiodInputEventHandler.hpp"
#include "stdinEventHandler.hpp"
#include "frameLimiter.hpp"
#include "fontToTexture2d.hpp"
#include "texturedTriangle2d.hpp"
#include "canvas2d.hpp"
#include "solidColorTriangle2d.hpp"
#include "argumentParser.hpp"

const std::string spidev = "/dev/spidev0.0";
static std::atomic_bool halt = false;

static void sigintHandler(int signum)
{
    halt = true;
}

std::vector<Gfx2D::TexturedTriangle> createTextTriangles(std::vector<Gfx2D::Texture>& textTextures, const std::string& text) {
    std::vector<Gfx2D::TexturedTriangle> textTriangles;
    // Translate the text into a bunch of textures
    textTextures.clear();
    for (const char &ch : text) {
        textTextures.emplace_back(Gfx2D::fontToTexture(ch));
    }
    // Now we need a bunch of texturedTriangles to draw the text on
    float currentXCoordinate = -1.0f;
    for (const Gfx2D::Texture& texture : textTextures) {
        textTriangles.push_back(Gfx2D::TexturedTriangle({currentXCoordinate, -1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1},
                texture, {{0,0}, {0,1}, {1,0}}));
        textTriangles.push_back(Gfx2D::TexturedTriangle({currentXCoordinate + 2.0f, 1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1},
                texture, {{1,1}, {0,1}, {1,0}}));
        currentXCoordinate += 2.0f;
    }
    return textTriangles;
}

int main(int argc, char * const argv[]) {
    signal(SIGINT, sigintHandler);

    ParsedArguments pArgs;
    try {
        pArgs = parseArguments(argc, argv);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    MatrixScreen referenceScreen(pArgs.ledMatrixWidth, pArgs.ledMatrixHeight);
    FrameLimiter frameLimiter(pArgs.maxFramesPerSecond);
    std::unique_ptr<MatrixDriver> matrixDriver;
#if !defined(__APPLE__)
    if (pArgs.matrixDriver == SelectedMatrixDriver::spi) {
        matrixDriver = std::make_unique<MatrixDriverSpi>(spidev, referenceScreen, pArgs.brightness);
    } else {
#endif //!defined(__APPLE__)
        matrixDriver = std::make_unique<MatrixDriverNcurses>();
#if !defined(__APPLE__)
    }
#endif //!defined(__APPLE__)
    Gfx2D::Canvas canvas(referenceScreen, PixelColor::off);

    std::uniform_int_distribution<unsigned int> dist(1, pArgs.nSides);
    std::uniform_int_distribution<unsigned int> sideEffectDist(0, 100);
    std::random_device urandom("/dev/urandom");

    std::chrono::time_point<std::chrono::steady_clock> lastRollTime = std::chrono::steady_clock::now();

    std::vector<Gfx2D::Texture> textTextures;
    std::vector<Gfx2D::TexturedTriangle> textTriangles;

    std::unique_ptr<InputEventHandler> eventHandler(new StdinEventHandler());

    unsigned int diceDigit = 1;
    float translationX = 0.0f;
    float stoppingRollTime = 0.75f;
    float rollTime = 0.005f;

    while(!halt) {
        if (rollTime > stoppingRollTime) {
            if (eventHandler->eventWaiting()) {
                rollTime = 0.005f;
            } else {
                frameLimiter.waitForNextFrame();
                continue;
            }
        }

        std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
        std::chrono::duration<float> diffInSec = curTime - lastRollTime;

        if (diffInSec.count() > rollTime) {
            rollTime *= 1.23f;

            lastRollTime = curTime;
            diceDigit = dist(urandom);

            std::string text = std::to_string(diceDigit);
            translationX = diceDigit < 10 ? 0.2f : -0.75f;

            if (sideEffectDist(urandom) > 98) {
                text = std::string(1, static_cast<char>(1));
                translationX = 0.0f;
            }

            textTriangles = createTextTriangles(textTextures, text);
        }


        Gfx2D::TransformationMatrix transformation =
            (Gfx2D::createScaleMatrix(0.5f, 0.5f) * Gfx2D::createTranslationMatrix(translationX, 0.0f));

        std::vector<Gfx2D::TexturedTriangle> transformedTriangles;

        for (const Gfx2D::TexturedTriangle& triangle : textTriangles) {
            transformedTriangles.emplace_back(triangle.createTransformedTriangle(transformation));
        }

        for (const Gfx2D::TexturedTriangle& triangle : transformedTriangles) {
            canvas.addToFrame(triangle);
        }
        matrixDriver->setScreen(canvas.generateFrame());
        frameLimiter.waitForNextFrame();

        if (rollTime > stoppingRollTime) {
            eventHandler->flushWaitingEvents();
        }

    }
    return EXIT_SUCCESS;
}
