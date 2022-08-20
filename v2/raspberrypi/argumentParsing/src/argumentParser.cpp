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

ParsedArguments parseArguments(int argc, char * const argv[]) {
    ParsedArguments ret = defaultArguments;
    // This makes sure that getopt does not print errors
    opterr = 0;
    const struct option longopts[] = {
        {"brightness", required_argument, nullptr, 'b'},
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
