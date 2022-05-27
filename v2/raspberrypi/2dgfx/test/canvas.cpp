#include <vector>

#include <gtest/gtest.h>

#include "canvas.hpp"

class SolidColorSquareDrawable: public Gfx2D::CanvasDrawable {
    private:
        const float xStart, xEnd, yStart, yEnd;
        const Gfx2D::CanvasDrawable::Color color;
    public:
        SolidColorSquareDrawable(const float xStart, const float xEnd, const float yStart, const float yEnd,
                const Gfx2D::CanvasDrawable::Color color) : xStart(xStart), xEnd(xEnd), yStart(yStart), yEnd(yEnd),
                color(color) { }
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
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(0, 1, 0, 1, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(7, 8, 0, 1, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(0, 1, 7, 8, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(7, 8, 7, 8, Gfx2D::CanvasDrawable::Color::on)
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
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(2, 6, 2, 6, Gfx2D::CanvasDrawable::Color::off),
        SolidColorSquareDrawable(1, 7, 1, 7, Gfx2D::CanvasDrawable::Color::on)
    };
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
    std::vector<SolidColorSquareDrawable> vec = {
        SolidColorSquareDrawable(1, 7, 1, 7, Gfx2D::CanvasDrawable::Color::on),
        SolidColorSquareDrawable(2, 6, 2, 6, Gfx2D::CanvasDrawable::Color::off)
    };
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
