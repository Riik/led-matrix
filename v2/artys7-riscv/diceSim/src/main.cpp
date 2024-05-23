#include <iostream>

#include "hal/inc/uart.h"
#include "common/inc/matrixScreen.hpp"
#include "matrixDriver.hpp"

#include "2dgfx/inc/canvas2d.hpp"
#include "2dgfx/inc/texturedTriangle2d.hpp"
#include "2dgfx/inc/fontToTexture2d.hpp"

static MatrixScreen screenFromNumber(Gfx2D::Canvas& canvas, uint_fast8_t num) {
    if (num >= 100) {
        num = 99;
    }
    std::string text = std::to_string(num);

    std::vector<Gfx2D::Texture> textTextures;
    std::vector<Gfx2D::TexturedTriangle> textTriangles;

    Gfx2D::FontColorMap colorMap = {PixelColor::off, PixelColor::transparent};

    // Translate the text into a bunch of textures
    for (const char &ch : text) {
        textTextures.emplace_back(Gfx2D::fontToTexture(ch, colorMap));
    }
    // Now we need a bunch of texturedTriangles to draw the text on
    float currentXCoordinate = -1.0f;
    for (const Gfx2D::Texture& texture : textTextures) {
        textTriangles.push_back(
                Gfx2D::TexturedTriangle(
                    {currentXCoordinate, -1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1}, texture,
                    {{0,0}, {0,1}, {1,0}}));
        textTriangles.push_back(
                Gfx2D::TexturedTriangle(
                    {currentXCoordinate + 2.0f, 1}, {currentXCoordinate, 1}, {currentXCoordinate + 2.0f, -1}, texture,
                    {{1,1}, {0,1}, {1,0}}));
        currentXCoordinate += 2.0f;
    }
    float translationX = num < 10 ? 0.2f : -0.75f;
    Gfx2D::TransformationMatrix translationMatrix = Gfx2D::createTranslationMatrix(translationX, 0.0f);
    Gfx2D::TransformationMatrix rotationMatrix = Gfx2D::createRotationMatrix(M_PI/2);
    Gfx2D::TransformationMatrix scaleMatrix = Gfx2D::createScaleMatrix(0.5f, 0.5f);
    Gfx2D::TransformationMatrix transformation = rotationMatrix * scaleMatrix * translationMatrix;
    std::vector<Gfx2D::TexturedTriangle> transformedTriangles;

    for (const Gfx2D::TexturedTriangle& triangle : textTriangles) {
        transformedTriangles.emplace_back(triangle.createTransformedTriangle(transformation));
    }

    for (const Gfx2D::TexturedTriangle& triangle : transformedTriangles) {
        canvas.addToFrame(triangle);
    }

    return canvas.generateFrame();
}

int main() {
    uart_init(115200);
    MatrixScreen screen(2, 2);
    MatrixDriver driver(screen.getMatrixCountHeight() * screen.getMatrixCountWidth());
    driver.setBrightness(0);
    Gfx2D::Canvas canvas(screen, PixelColor::on);

    while(true) {
        for (uint_fast8_t i = 0; i < 100; ++i) {
            driver.setScreen(screenFromNumber(canvas, i));
        }
        for (uint_fast8_t i = 1; i < 99; ++i) {
            driver.setScreen(screenFromNumber(canvas, 99 - i));
        }
    }
    return 0;
}
