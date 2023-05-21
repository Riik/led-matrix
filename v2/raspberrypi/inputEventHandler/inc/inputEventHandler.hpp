#pragma once

class InputEventHandler {
    public:
        virtual bool eventWaiting() = 0;

        virtual void flushWaitingEvents() = 0;

        virtual ~InputEventHandler() {};
};
