#include <vector>

#include <gtest/gtest.h>

#include "canvas.hpp"
#include "matrix2d.hpp"

class SolidColorSquareDrawable: public Gfx2D::CanvasDrawable {
    private:
        const float xStart, xEnd, yStart, yEnd;
        const Gfx2D::CanvasDrawable::Color color;
    public:
        SolidColorSquareDrawable(const float xStart, const float xEnd, const float yStart, const float yEnd,
                const Gfx2D::CanvasDrawable::Color color) : xStart(xStart), xEnd(xEnd), yStart(yStart), yEnd(yEnd),
                color(color) { }
        SolidColorSquareDrawable(const Gfx2D::Point& bottomLeft, const Gfx2D::Point& topRight,
                const Gfx2D::CanvasDrawable::Color color) : xStart(bottomLeft.x()), xEnd(topRight.x()), yStart(bottomLeft.y()),
                yEnd(topRight.y()), color(color) {}
        bool pointIsInDrawable(const Gfx2D::Point& p) const override final
        {
            return p.x() >= this->xStart && p.x() <= this->xEnd &&
                p.y() >= this->yStart && p.y() <= this->yEnd;
        }

        Gfx2D::CanvasDrawable::Color colorAtPoint(const Gfx2D::Point& /*unused*/) const override final
        {
            return this->color;
        }
};

TEST(canvas, transparentBackground)
{
    MatrixScreen screen(1,1);
    EXPECT_ANY_THROW(Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::transparent));
}

TEST(canvas, emptyCanvasOff)
{
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::off);
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(MatrixScreen::PixelColor::off);
    EXPECT_EQ(screen, output);
    screen.resetScreen(MatrixScreen::PixelColor::on);
    EXPECT_NE(screen, output);
}

TEST(canvas, emptyCanvasOn)
{
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::on);
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(MatrixScreen::PixelColor::on);
    EXPECT_EQ(screen, output);
    screen.resetScreen(MatrixScreen::PixelColor::off);
    EXPECT_NE(screen, output);
}

TEST(canvas, nonOverlappingSquares)
{
    float pixelDelta = 1.0f/8.0f;
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(-1.0, -1.0 + pixelDelta, -1.0, -1.0 + pixelDelta, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(1.0 - pixelDelta, 1.0, -1.0, -1.0 + pixelDelta, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(-1.0, -1.0 + pixelDelta, 1.0 - pixelDelta, 1.0, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(1.0 - pixelDelta, 1.0, 1.0 - pixelDelta, 1.0, Gfx2D::CanvasDrawable::Color::on)
    };
    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(MatrixScreen::PixelColor::off);
    screen(0,0) = MatrixScreen::PixelColor::on;
    screen(7,0) = MatrixScreen::PixelColor::on;
    screen(7,7) = MatrixScreen::PixelColor::on;
    screen(0,7) = MatrixScreen::PixelColor::on;

    EXPECT_EQ(screen, output);
}

TEST(canvas, invisibleOverlappingSquares)
{
    Gfx2D::TransformationMatrix translationMat = Gfx2D::createTranslationMatrix(-4, -4);
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(0.25f, 0.25f);
    Gfx2D::TransformationMatrix pixelTransformation = scaleMat*translationMat;
    std::vector<SolidColorSquareDrawable> vec;

    Gfx2D::Point bottomLeft = pixelTransformation * Gfx2D::Point(2, 2);
    Gfx2D::Point topRight = pixelTransformation * Gfx2D::Point(6,6);
    vec.emplace_back(bottomLeft, topRight, Gfx2D::CanvasDrawable::Color::off);

    bottomLeft = pixelTransformation * Gfx2D::Point(1,1);
    topRight = pixelTransformation * Gfx2D::Point(7,7);
    vec.emplace_back(bottomLeft, topRight, Gfx2D::CanvasDrawable::Color::on);

    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(MatrixScreen::PixelColor::off);
    for (size_t x = 1; x < 7; ++x) {
        for (size_t y = 1; y < 7; ++y) {
            screen(x, y) = MatrixScreen::PixelColor::on;
        }
    }

    EXPECT_EQ(screen, output);
}

TEST(canvas, visibleOverlappingSquares)
{
    Gfx2D::TransformationMatrix translationMat = Gfx2D::createTranslationMatrix(-4, -4);
    Gfx2D::TransformationMatrix scaleMat = Gfx2D::createScaleMatrix(0.25f, 0.25f);
    Gfx2D::TransformationMatrix pixelTransformation = scaleMat*translationMat;
    std::vector<SolidColorSquareDrawable> vec;

    Gfx2D::Point bottomLeft = pixelTransformation * Gfx2D::Point(1,1);
    Gfx2D::Point topRight = pixelTransformation * Gfx2D::Point(7,7);
    vec.emplace_back(bottomLeft, topRight, Gfx2D::CanvasDrawable::Color::on);

    bottomLeft = pixelTransformation * Gfx2D::Point(2, 2);
    topRight = pixelTransformation * Gfx2D::Point(6,6);
    vec.emplace_back(bottomLeft, topRight, Gfx2D::CanvasDrawable::Color::off);

    MatrixScreen screen(1,1);
    Gfx2D::Canvas canvas = Gfx2D::Canvas(screen, Gfx2D::CanvasDrawable::Color::off);

    for (auto& drawable : vec) {
        canvas.addToFrame(drawable);
    }
    MatrixScreen output = canvas.generateFrame();
    screen.resetScreen(MatrixScreen::PixelColor::off);
    for (size_t x = 1; x < 7; ++x) {
        for (size_t y = 1; y < 7; ++y) {
            screen(x, y) = MatrixScreen::PixelColor::on;
        }
    }
    for (size_t x = 2; x < 6; ++x) {
        for (size_t y = 2; y < 6; ++y) {
            screen(x, y) = MatrixScreen::PixelColor::off;
        }
    }

    EXPECT_EQ(screen, output);
}
