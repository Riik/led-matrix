#include "textScroller.hpp"
#include "fontToTexture2d.hpp"

TextScroller::TextScroller(MatrixScreen templateScreen) : canvas(templateScreen, PixelColor::off) {
    std::string text = "Hoi, Rik!";
    // Translate the text into a bunch of textures
    for (const char &ch : text) {
        this->textTextures.emplace_back(Gfx2D::fontToTexture(ch));
    }
    float xCoordinate = -1.0f;
    for (const Gfx2D::Texture& texture : this->textTextures) {
        this->textTriangles.push_back(Gfx2D::TexturedTriangle({xCoordinate, -1}, {xCoordinate, 1}, {xCoordinate + 2.0f, -1},
                texture, {{0,0}, {0,1}, {1,0}}));
        this->textTriangles.push_back(Gfx2D::TexturedTriangle({xCoordinate + 2.0f, 1}, {xCoordinate, 1}, {xCoordinate + 2.0f, -1},
                texture, {{1,1}, {0,1}, {1,0}}));
        xCoordinate += 2.0f;
    }
    Gfx2D::BoundaryBox canvasBoundaryBox = this->canvas.getBoundaryBox();
    this->endXCoordinate = xCoordinate + canvasBoundaryBox.getPointHigh().x();
    this->startXCoordinate = 2*canvasBoundaryBox.getPointLow().x();
    this->currentXCoordinate = canvasBoundaryBox.getPointLow().x();
    this->lastWakeTime = std::chrono::steady_clock::now();
}

MatrixScreen TextScroller::getScreen() {
    std::chrono::time_point<std::chrono::steady_clock> curTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> diffInSec = curTime - lastWakeTime;
    this->lastWakeTime = curTime;
    this->currentXCoordinate += diffInSec.count() * this->textSpeedPerSec;
    if (this->currentXCoordinate > this->endXCoordinate) {
       this->currentXCoordinate = this->startXCoordinate;
    }
    Gfx2D::TransformationMatrix translation = Gfx2D::createTranslationMatrix(-this->currentXCoordinate, 0.0f);
    std::vector<Gfx2D::TexturedTriangle> transformedTriangles;

    for (const Gfx2D::TexturedTriangle& triangle : this->textTriangles) {
        transformedTriangles.emplace_back(triangle.createTransformedTriangle(translation));
    }

    for (const Gfx2D::TexturedTriangle& triangle : transformedTriangles) {
        this->canvas.addToFrame(triangle);
    }

    return this->canvas.generateFrame();
}
