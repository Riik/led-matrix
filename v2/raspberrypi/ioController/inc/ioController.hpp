#pragma once

class IoController {
    public:
        virtual void waitForButtonPress() = 0;

        virtual ~IoController() {};
};
