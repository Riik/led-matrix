#include <vector>

#include <gtest/gtest.h>

#include "canvas2d.hpp"
#include "matrix2d.hpp"

class SolidColorSquareDrawable: public Gfx2D::CanvasDrawable {
    private:
        const float xStart, xEnd, yStart, yEnd;
        const PixelColor color;
    public:
        SolidColorSquareDrawable(const float xStart, const float xEnd, const float yStart, const float yEnd,
                const PixelColor color) : xStart(xStart), xEnd(xEnd), yStart(yStart), yEnd(yEnd),
                color(color) { }
        SolidColorSquareDrawable(const Gfx2D::Point& bottomLeft, const Gfx2D::Point& topRight,
                const PixelColor color) : xStart(bottomLeft.x()), xEnd(topRight.x()), yStart(bottomLeft.y()),
                yEnd(topRight.y()), color(color) {}
        bool pointIsInDrawable(const Gfx2D::Point& p) const override final
        {
            return p.x() >= this->xStart && p.x() <= this->xEnd &&
                p.y() >= this->yStart && p.y() <= this->yEnd;
        }

        PixelColor colorAtPoint(const Gfx2D::Point& /*unused*/) const override final
        {
            return this->color;
        }
};

TEST(canvas2d, transparentBackground)
{
    MatrixScreen screen(1,1);
    EXPECT_ANY_THROW(Gfx2D::Canvas(screen, PixelColor::transparent));
}

TEST(canvas2d, emptyCanvasOff)
{
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::off);
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    EXPECT_EQ(screen, output);
    screen.resetScreen(PixelColor::on);
    EXPECT_NE(screen, output);
}

TEST(canvas2d, emptyCanvasOn)
{
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::on);
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::on);
    EXPECT_EQ(screen, output);
    screen.resetScreen(PixelColor::off);
    EXPECT_NE(screen, output);
}

TEST(canvas2d, nonOverlappingSquares)
{
    float pixelDelta = 1.0f/8.0f;
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(-1.0, -1.0 + pixelDelta, -1.0, -1.0 + pixelDelta, PixelColor::on),
        SolidColorSquareDrawable(1.0 - pixelDelta, 1.0, -1.0, -1.0 + pixelDelta, PixelColor::on),
        SolidColorSquareDrawable(-1.0, -1.0 + pixelDelta, 1.0 - pixelDelta, 1.0, PixelColor::on),
        SolidColorSquareDrawable(1.0 - pixelDelta, 1.0, 1.0 - pixelDelta, 1.0, PixelColor::on)
    };
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    screen(0,0) = PixelColor::on;
    screen(7,0) = PixelColor::on;
    screen(7,7) = PixelColor::on;
    screen(0,7) = PixelColor::on;

    EXPECT_EQ(screen, output);
}

TEST(canvas2d, invisibleOverlappingSquares)
{
    Gfx2D::TransformationMatrix translationMat = Gfx2D::createTranslationMatrix(-4, -4);
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(0.25f, 0.25f);
    Gfx2D::TransformationMatrix pixelTransformation = scaleMat*translationMat;
    std::vector<SolidColorSquareDrawable> vec;

    Gfx2D::Point bottomLeft = pixelTransformation * Gfx2D::Point(2, 2);
    Gfx2D::Point topRight = pixelTransformation * Gfx2D::Point(6,6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::off);

    bottomLeft = pixelTransformation * Gfx2D::Point(1,1);
    topRight = pixelTransformation * Gfx2D::Point(7,7);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);

    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    for (size_t x = 1; x < 7; ++x) {
        for (size_t y = 1; y < 7; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }

    EXPECT_EQ(screen, output);
}

TEST(canvas2d, visibleOverlappingSquares)
{
    Gfx2D::TransformationMatrix translationMat = Gfx2D::createTranslationMatrix(-4, -4);
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(0.25f, 0.25f);
    Gfx2D::TransformationMatrix pixelTransformation = scaleMat*translationMat;
    std::vector<SolidColorSquareDrawable> vec;

    Gfx2D::Point bottomLeft = pixelTransformation * Gfx2D::Point(1,1);
    Gfx2D::Point topRight = pixelTransformation * Gfx2D::Point(7,7);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);

    bottomLeft = pixelTransformation * Gfx2D::Point(2, 2);
    topRight = pixelTransformation * Gfx2D::Point(6,6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::off);

    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    for (size_t x = 1; x < 7; ++x) {
        for (size_t y = 1; y < 7; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = PixelColor::off;
        }
    }

    EXPECT_EQ(screen, output);
}

TEST(canvas2d, canvasShouldEmptyFrame)
{
    float pixelDelta = 1.0f/8.0f;
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(-1.0, -1.0 + pixelDelta, -1.0, -1.0 + pixelDelta, PixelColor::on)
    };
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, PixelColor::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    screen(0,0) = PixelColor::on;

    EXPECT_EQ(screen, output);
    output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    EXPECT_EQ(screen, output);
}

TEST(canvas2d, differentMatrixLayouts)
{
    // This transformation matrix is good enough for our purposes, but we
    // do need to keep in mind that it is mostly incorrect.
    // Since we want the canvas to preserve aspect ratio, we can do this as long as
    // either width or height is exactly 1 matrix (8 pixels).
    // In practice, we define a mapping function where (4,4) maps to (0,0) and (1,1) maps to (0.25, 0.25).
    // The clipping space is then a subsection of this mapping, as long as either width or height is
    // 8 pixels.
    Gfx2D::TransformationMatrix translationMat = Gfx2D::createTranslationMatrix(-4, -4);
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(0.25f, 0.25f);
    Gfx2D::TransformationMatrix pixelTransformation = scaleMat*translationMat;
    std::vector<SolidColorSquareDrawable> vec;
    Gfx2D::Point bottomLeft, topRight;

    // 5 cubes, one in the center and half a cube for every ledmatrix away from the center.
    // First, the center cube
    bottomLeft = pixelTransformation * Gfx2D::Point(2, 2);
    topRight = pixelTransformation * Gfx2D::Point(6,6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);
    // Top cube
    bottomLeft = pixelTransformation * Gfx2D::Point(2, 14);
    topRight = pixelTransformation * Gfx2D::Point(6, 18);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);
    // Bottom cube
    bottomLeft = pixelTransformation * Gfx2D::Point(2, -10);
    topRight = pixelTransformation * Gfx2D::Point(6, -6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);
    // Left cube
    bottomLeft = pixelTransformation * Gfx2D::Point(-10, 2);
    topRight = pixelTransformation * Gfx2D::Point(-6, 6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);
    // Right cube
    bottomLeft = pixelTransformation * Gfx2D::Point(14, 2);
    topRight = pixelTransformation * Gfx2D::Point(18, 6);
    vec.emplace_back(bottomLeft, topRight, PixelColor::on);

    // First of all, a 1x1 ledscreen
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas(screen, PixelColor::off);
    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    EXPECT_EQ(screen, output);

    // Next, 3 wide, 1 high.
    screen = MatrixScreen(3, 1);
    canvas = Gfx2D::Canvas(screen, PixelColor::off);
    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    // Draw the cube on the left
    for (size_t x = 0; x < 2; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    // Draw the center cube
    for (size_t x = 10; x < 14; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    // Draw the right cube
    for (size_t x = 22; x < 24; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    EXPECT_EQ(screen, output);

    // Then, 1 wide, 3 high
    screen = MatrixScreen(1, 3);
    canvas = Gfx2D::Canvas(screen, PixelColor::off);
    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    // Draw the cube on the bottom
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 0; y < 2; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    // Draw the center cube
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 10; y < 14; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    // Draw the top cube
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 22; y < 24; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
    EXPECT_EQ(screen, output);

    // Finally, a 3x3 screen. Since aspect ratio is preserved, we expect a bigger 1x1,
    // so only the center cube visible.
    screen = MatrixScreen(3, 3);
    canvas = Gfx2D::Canvas(screen, PixelColor::off);
    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    output = canvas.generateFrame();
    screen.resetScreen(PixelColor::off);
    for (size_t x = 6; x < 18; ++x) {
        for (size_t y = 6; y < 18; ++y) {
            screen(x, y) = PixelColor::on;
        }
    }
}
