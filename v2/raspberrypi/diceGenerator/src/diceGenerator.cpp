#include <cassert>
#include <sstream>
#include <memory>

#include "diceGenerator.hpp"
#include "fontToTexture2d.hpp"
#include "gpiodInputEventHandler.hpp"
#include "stdinEventHandler.hpp"

DiceGenerator::DiceGenerator(const MatrixScreen& referenceScreen, const ParsedArguments& pArgs) :
    canvas(referenceScreen, PixelColor::off), dist(1, pArgs.nSides), urandom("/dev/urandom"), currentScreen(referenceScreen) {
   this->lastRollTime = std::chrono::steady_clock::now();
   this->currentScreen = this->screenFromNumber(pArgs.nSides);

   this->eventHandler = std::make_unique<StdinEventHandler>();
   this->waitingForInput = true;
   this->stoppingRollTimeout = 0.75;
   this->startRollTimeout = 0.005;
   this->currentRollTimeout = this->startRollTimeout;
}

MatrixScreen DiceGenerator::getScreen() {
    if (this->waitingForInput && !this->eventHandler->eventWaiting()) {
        return this->currentScreen;
    }

    this->waitingForInput = false;
    std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> diffInSec = curTime - this->lastRollTime;
    if (diffInSec.count() > this->currentRollTimeout) {
        this->currentRollTimeout *= 1.23f;
        this->lastRollTime = curTime;
        this->currentScreen = this->screenFromNumber(this->dist(this->urandom));
        if (this->currentRollTimeout > this->stoppingRollTimeout) {
            this->eventHandler->flushWaitingEvents();
            this->waitingForInput = true;
            this->currentRollTimeout = this->startRollTimeout;
        }
    }
    return this->currentScreen;
}

MatrixScreen DiceGenerator::screenFromNumber(uint_fast8_t num) {
    assert(num < 100);

    std::string text = std::to_string(num);

    std::vector<Gfx2D::Texture> textTextures;
    std::vector<Gfx2D::TexturedTriangle> textTriangles;
    // Translate the text into a bunch of textures
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
    float translationX = num < 10 ? 0.2f : -0.75f;
    Gfx2D::TransformationMatrix transformation =
        (Gfx2D::createScaleMatrix(0.5f, 0.5f) * Gfx2D::createTranslationMatrix(translationX, 0.0f));
    std::vector<Gfx2D::TexturedTriangle> transformedTriangles;

    for (const Gfx2D::TexturedTriangle& triangle : textTriangles) {
        transformedTriangles.emplace_back(triangle.createTransformedTriangle(transformation));
    }

    for (const Gfx2D::TexturedTriangle& triangle : transformedTriangles) {
        canvas.addToFrame(triangle);
    }
    return canvas.generateFrame();
}
