#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "subProgram.hpp"
#include "canvas2d.hpp"
#include "texturedTriangle2d.hpp"
#include "argumentParser.hpp"

class TextScroller : public SubProgram {
    public:
        TextScroller(MatrixScreen referenceScreen, const ParsedArguments& pArgs);

        MatrixScreen getScreen() final;
    private:
        std::chrono::time_point<std::chrono::steady_clock> lastWakeTime;
        float textSpeedPerSec = 2.0f;
        Gfx2D::Canvas canvas;
        std::vector<Gfx2D::Texture> textTextures;
        std::vector<Gfx2D::TexturedTriangle> textTriangles;
        float currentXCoordinate;
        float endXCoordinate;
        float startXCoordinate;
};
