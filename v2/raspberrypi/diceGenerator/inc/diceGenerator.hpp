#pragma once
#include <chrono>
#include <vector>
#include <random>
#include <cstdint>

#include "subProgram.hpp"
#include "canvas2d.hpp"
#include "argumentParser.hpp"
#include "texturedTriangle2d.hpp"
#include "inputEventHandler.hpp"

class DiceGenerator : public SubProgram {
    public:
        DiceGenerator(const MatrixScreen& refenceScreen, const ParsedArguments& pArgs);
        MatrixScreen getScreen() final;
    private:
        Gfx2D::Canvas canvas;
        std::uniform_int_distribution<uint_fast8_t> dist;
        std::random_device urandom;
        std::chrono::time_point<std::chrono::steady_clock> lastRollTime;
        std::unique_ptr<InputEventHandler> eventHandler;
        bool waitingForInput;
        float stoppingRollTimeout = 0.75f;
        float startRollTimeout = 0.005f;
        float currentRollTimeout;

        MatrixScreen currentScreen;

        MatrixScreen screenFromNumber(uint_fast8_t num);
};
