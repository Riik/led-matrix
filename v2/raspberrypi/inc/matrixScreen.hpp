#include <vector>

/**
 * The bottom-left is coordinate 0,0.
 */
class MatrixScreen {

    public:
        static constexpr std::size_t matrixPixelCountWidth = 8;
        static constexpr std::size_t matrixPixelCountHeight = 8;

        enum class PixelColor {
            on,
            off,
            transparent
        };

        explicit MatrixScreen(const std::size_t& matrixCountWidth, const std::size_t& matrixCountHeight);

        std::size_t getMatrixCountWidth() const;
        std::size_t getMatrixCountHeight() const;
        std::size_t getPixelCountWidth() const;
        std::size_t getPixelCountHeight() const;

        void resetScreen(MatrixScreen::PixelColor color);

        MatrixScreen::PixelColor operator()(std::size_t width, std::size_t height) const;
        MatrixScreen::PixelColor& operator()(std::size_t width, std::size_t height);

    private:
        std::vector<MatrixScreen::PixelColor> screen;
        const std::size_t matrixCountWidth;
        const std::size_t matrixCountHeight;
        const std::size_t pixelCountWidth;
        const std::size_t pixelCountHeight;
};
