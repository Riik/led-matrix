#include <getopt.h>
#include <stdexcept>
#include <sstream>
#include <climits>

#include "argumentParser.hpp"

static long long str2number(const char *s)
{
    char *end;
    errno = 0;
    long long l = std::strtoll(s, &end, 10);
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

static uint_fast8_t parseBrightness(const char* arg)
{
    long long num = str2number(arg);
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
    long long num = str2number(arg);
    if (num < 0) {
        throw std::invalid_argument("Fps limit cannot be a negative number");
    }
    if (INT64_MAX > UINT_FAST32_MAX && num > static_cast<long long>(UINT_FAST32_MAX)) {
        std::stringstream ss;
        ss << "Given fps limit out of range. Max: " << UINT_FAST32_MAX << " given: " << arg;
        throw std::invalid_argument(ss.str());
    }
    return static_cast<uint_fast32_t>(num);
}

ParsedArguments parseArguments(int argc, char * const argv[]) {
    ParsedArguments ret = defaultArguments;
    // This makes sure that getopt does not print errors
    opterr = 0;
    const struct option longopts[] = {
        {"brightness", required_argument, nullptr, 'b'},
        {"fpsLimit", required_argument, nullptr, 'f'},
        {"sides", required_argument, nullptr, 'd'},
        {"spiDriver", no_argument, nullptr, 's'},
        {"ncursesDriver", no_argument, nullptr, 'n'},
        {0, 0, nullptr, 0}
    };

    while (true) {
        int longindex;
        int c = getopt_long(argc, argv, ":", longopts, &longindex);
        if (c == -1) {
            break;
        }
        switch(c) {
            case 'b':
                ret.brightness = parseBrightness(optarg);
                break;
            case 'f':
                ret.maxFramesPerSecond = parseMaxFramesPerSecond(optarg);
                break;
            case 'd':
                ret.nSides = parseNSides(optarg);
            case 's':
                ret.matrixDriver = SelectedMatrixDriver::spi;
                break;
            case 'n':
                ret.matrixDriver = SelectedMatrixDriver::ncurses;
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
