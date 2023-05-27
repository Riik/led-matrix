#include <getopt.h>
#include <stdexcept>
#include <sstream>
#include <climits>

#include "argumentParser.hpp"

static int64_t str2number(const char *s)
{
    char *end;
    errno = 0;
    int64_t l = std::strtoll(s, &end, 10);
    if (errno == ERANGE && l == LLONG_MAX) {
        std::stringstream ss;
        ss << "Overflow while converting " << s;
        throw std::invalid_argument(ss.str());
    }
    if (errno == ERANGE && l == LLONG_MIN) {
        std::stringstream ss;
        ss << "Underflow while converting " << s;
        throw std::invalid_argument(ss.str());
    }
    if (*s == '\0' || *end != '\0') {
        std::stringstream ss;
        ss << s << " is not a number";
        throw std::invalid_argument(ss.str());
    }
    return l;
}

static float str2float(const char* s)
{
    std::string str(s);
    float ret;
    std::stringstream stream(str);
    stream >> ret;
    if (stream.fail() || !stream.eof()) {
        std::stringstream ss;
        ss << str << " cannot be parsed to float";
        throw std::invalid_argument(ss.str());
    }
    return ret;
}

static uint_fast8_t parseBrightness(const char* arg)
{
    int64_t num = str2number(arg);
    if (num < 0 || num > 15) {
        std::stringstream ss;
        ss << "Brightness must be in [0, 15]";
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast8_t>(num);
}

static uint_fast8_t parseNSides(const char* arg)
{
    long long num = str2number(arg);
    if (num < 1 || num > 99) {
        std::stringstream ss;
        ss << "Brightness must be in [1, 99]";
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast8_t>(num);
}

static uint_fast32_t parseMaxFramesPerSecond(const char* arg)
{
    int64_t num = str2number(arg);
    if (num < 0) {
        throw std::invalid_argument("Fps limit cannot be a negative number");
    }
    if (INT64_MAX > UINT_FAST32_MAX && num > static_cast<int64_t>(UINT_FAST32_MAX)) {
        std::stringstream ss;
        ss << "Given fps limit out of range. Max: " << UINT_FAST32_MAX << " given: " << arg;
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast32_t>(num);
}

static uint_fast16_t parseWidth(const char* arg)
{
    int64_t num = str2number(arg);
    if (num < 0) {
        throw std::invalid_argument("Width must be > 0");
    }
    if (INT64_MAX > UINT_FAST16_MAX && num > static_cast<int64_t>(UINT_FAST16_MAX)) {
        std::stringstream ss;
        ss << "Given width out of range. Max: " << UINT_FAST16_MAX << " given: " << arg;
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast16_t>(num);
}

static uint_fast16_t parseHeight(const char* arg)
{
    int64_t num = str2number(arg);
    if (num < 0) {
        throw std::invalid_argument("Height must be > 0");
    }
    if (INT64_MAX > UINT_FAST16_MAX && num > static_cast<int64_t>(UINT_FAST16_MAX)) {
        std::stringstream ss;
        ss << "Given height out of range. Max: " << UINT_FAST16_MAX << " given: " << arg;
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast16_t>(num);
}

static float parseTextSpeed(const char* arg) {
    float speed = str2float(arg);
    if (speed <= 0) {
        throw std::invalid_argument("Text speed must be > 0!");
    }
    return speed;
}

ParsedArguments parseArguments(int argc, char * const argv[]) {
    const ParsedArguments defaultArguments = {
        .brightness = 7,
        .maxFramesPerSecond = 200,
#if defined(__arm__) || defined(__aarch64__)
        .matrixDriver = SelectedMatrixDriver::spi,
#else
        .matrixDriver = SelectedMatrixDriver::ncurses,
#endif //defined(__arm__) || defined(__aarch64__)
        .ledMatrixWidth = 1,
        .ledMatrixHeight = 1,
        .textScrollerText = "Hello, world!",
        .textScrollerSpeed = 2.0f,
        .diceGeneratorSides = 20,
        .subProgram = SelectedSubProgram::diceGenerator,
        .inputEvent = SelectedInputEvent::terminal
    };
    ParsedArguments ret = defaultArguments;
    // This makes sure that getopt does not print errors
    opterr = 0;
    const struct option longopts[] = {
        {"brightness", required_argument, nullptr, 1},
        {"fpsLimit", required_argument, nullptr, 2},
        {"sides", required_argument, nullptr, 3},
        {"spiDriver", no_argument, nullptr, 4},
        {"ncursesDriver", no_argument, nullptr, 5},
        {"width", required_argument, nullptr, 6},
        {"height", required_argument, nullptr, 7},
        {"text", required_argument, nullptr, 8},
        {"textSpeed", required_argument, nullptr, 9},
        {"textScroller", no_argument, nullptr, 10},
        {"diceGenerator", no_argument, nullptr, 11},
        {"terminalInput", no_argument, nullptr, 12},
        {"gpioInput", no_argument, nullptr, 13},
        {0, 0, nullptr, 0}
    };

    while (true) {
        int longindex;
        int c = getopt_long(argc, argv, ":", longopts, &longindex);
        if (c == -1) {
            break;
        }
        switch(c) {
            case 1:
                ret.brightness = parseBrightness(optarg);
                break;
            case 2:
                ret.maxFramesPerSecond = parseMaxFramesPerSecond(optarg);
                break;
            case 3:
                ret.diceGeneratorSides = parseNSides(optarg);
                break;
            case 4:
                ret.matrixDriver = SelectedMatrixDriver::spi;
                break;
            case 5:
                ret.matrixDriver = SelectedMatrixDriver::ncurses;
                break;
            case 6:
                ret.ledMatrixWidth = parseWidth(optarg);
                break;
            case 7:
                ret.ledMatrixHeight = parseHeight(optarg);
                break;
            case 8:
                ret.textScrollerText = std::string(optarg);
                break;
            case 9:
                ret.textScrollerSpeed = parseTextSpeed(optarg);
                break;
            case 10:
                ret.subProgram = SelectedSubProgram::textScroller;
                break;
            case 11:
                ret.subProgram = SelectedSubProgram::diceGenerator;
                break;
            case 12:
                ret.inputEvent = SelectedInputEvent::terminal;
                break;
            case 13:
                ret.inputEvent = SelectedInputEvent::gpiod;
                break;
            case ':':
                {
                    std::stringstream ss;
                    ss << argv[optind - 1] << " requires an argument";
                    throw std::invalid_argument(ss.str());
                    break;
                }
            case '?':
                {
                    std::stringstream ss;
                    ss << "Unkown argument " << argv[optind - 1];
                    throw std::invalid_argument(ss.str());
                    break;
                }
            default:
                {
                    std::stringstream ss;
                    ss << "Getopt returned unknown character code " << c << " on input " << argv[optind - 1];
                    throw std::invalid_argument(ss.str());
                    break;
                }
        }
    }
    return ret;
}
