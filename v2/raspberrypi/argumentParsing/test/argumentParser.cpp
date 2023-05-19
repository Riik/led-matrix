#include <getopt.h>
#include <gtest/gtest.h>

#include "argumentParser.hpp"

class argumentParserTest : public ::testing::Test {
    void SetUp() override {
        // Resetting optind makes sure we start each argv parsing from the beginning
        optind = 1;
    }
};

TEST_F(argumentParserTest, incorrectArgument)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--SurelyInvalidArgument";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };

    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, brightnessTest)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--brightness=5";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };

    ParsedArguments pArgs = parseArguments(argc, argv);
    EXPECT_EQ(pArgs.brightness, 5);
}

TEST_F(argumentParserTest, brightnessOutOfRangeLow)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--brightness=-1";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, brightnessOutOfRangeHigh)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--brightness=16";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, brightnessNoArgument)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--brightness";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, brightnessInvalidArgument)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--brightness=henk";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, textSpeedValid)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--textSpeed=0.5";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    ParsedArguments pArgs = parseArguments(argc, argv);
    EXPECT_EQ(pArgs.textScrollerSpeed, 0.5f);
}

TEST_F(argumentParserTest, textSpeedNotANumber)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--textSpeed=henk";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}


TEST_F(argumentParserTest, textSpeedNegative)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--textSpeed=-1.0";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}

TEST_F(argumentParserTest, textSpeedPartialNumber)
{
    int argc = 2;
    char arg0[] = "path";
    char arg1[] = "--textSpeed=0.5.5";
    char * const argv[] = {
        arg0,
        arg1,
        nullptr
    };
    EXPECT_ANY_THROW(parseArguments(argc, argv));
}
