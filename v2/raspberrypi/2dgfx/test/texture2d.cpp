#include <cmath>
#include <gtest/gtest.h>

#include "texture2d.hpp"

TEST(texture2d, sizeMustMatch)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::off, PixelColor::on});
    EXPECT_ANY_THROW(Gfx2D::Texture(texels, 3, 3));
    EXPECT_ANY_THROW(Gfx2D::Texture(texels, 1, 3));
}

TEST(texture2d, sizeCannotBeZero)
{
    std::vector<PixelColor> texels;
    EXPECT_ANY_THROW(Gfx2D::Texture(texels, 0, 0));
}

static void testInboundResults(const Gfx2D::Texture& texture)
{
    constexpr float epsilon = std::numeric_limits<float>::epsilon();
    EXPECT_EQ(texture.getColorAt(0.0f, 0.0f), PixelColor::on);

    EXPECT_EQ(texture.getColorAt(0.5f - epsilon, 0.0f), PixelColor::on);
    EXPECT_EQ(texture.getColorAt(0.5f, 0.0f), PixelColor::off);
    EXPECT_EQ(texture.getColorAt(1.0f - epsilon, 0.0f), PixelColor::off);

    EXPECT_EQ(texture.getColorAt(0.0f, 0.5f - epsilon), PixelColor::on);
    EXPECT_EQ(texture.getColorAt(0.0f, 0.5f), PixelColor::off);
    EXPECT_EQ(texture.getColorAt(0.0f, 1.0f- epsilon), PixelColor::off);

    EXPECT_EQ(texture.getColorAt(0.5f - epsilon, 0.5f - epsilon), PixelColor::on);
    EXPECT_EQ(texture.getColorAt(0.5f, 0.5f), PixelColor::on);
    EXPECT_EQ(texture.getColorAt(1.0f - epsilon, 1.0f - epsilon), PixelColor::on);
}

TEST(texture2d, inboundResults)
{
    const std::vector<Gfx2D::Texture::WrapMode> wrapModes = {Gfx2D::Texture::WrapMode::Repeat,
        Gfx2D::Texture::WrapMode::MirroredRepeat,
        Gfx2D::Texture::WrapMode::ClampToEdge,
        Gfx2D::Texture::WrapMode::ClampToBorder};
    for (const auto& sMode : wrapModes) {
        for (const auto& tMode : wrapModes) {
            const Gfx2D::Texture texture({PixelColor::on, PixelColor::off, PixelColor::off, PixelColor::on}, 2, 2,
                    sMode, tMode);
            testInboundResults(texture);
        }
    }
}

TEST(texture2d, wrapModeRepeatRepeat)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::Repeat, Gfx2D::Texture::WrapMode::Repeat);
    std::vector<float> numList = {-4.0f, -3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    constexpr float epsilon = 0.1;

    for (float sBase : numList) {
        for (float tBase : numList) {
            float s = sBase;
            float t = tBase;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);

            s = sBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            s = sBase + 0.5f;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
            s = sBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);

            s = sBase;
            t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            t = tBase + 0.5f;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);

            s = sBase + 0.5f - epsilon;
            t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            s = sBase + 0.5f;
            t = tBase + 0.5f;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            s = sBase + 1.0f - epsilon;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        }
    }
}

TEST(texture2d, wrapModeRepeatRepeatEdgeCase)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    // Set the edge of the texture to off and make sure we do not hit the texture in the edge case
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::Repeat, Gfx2D::Texture::WrapMode::Repeat, PixelColor::off);
    constexpr float epsilon = std::numeric_limits<float>::epsilon() / 8.0f;

    float s = -epsilon;
    float t = -epsilon;
    EXPECT_NE(s, 0.0f);
    EXPECT_EQ(s - std::floor(s), 1.0f);
    EXPECT_NE(t, 0.0f);
    EXPECT_EQ(t - std::floor(t), 1.0f);
    EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
}

TEST(texture2d, wrapModeMirroredRepeatMirroredRepeat)
{
    // For mirrored repeat, every even case is just like repeat, while every odd case actually mirrors
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::MirroredRepeat, Gfx2D::Texture::WrapMode::MirroredRepeat);
    float epsilon = 0.1;
    // Keep the y axis at zero, the x axis should be properly mirrored
    // All even cases should be the same as repeat
    for (float sBase : {-4.0f, -2.0f, 0.0f, 2.0f, 4.0f}) {
        float tBase = 0.0f;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }
    // All odd cases should be mirrored over the y axis
    for (float sBase : {-3.0f, -1.0f, 1.0f, 3.0f}) {
        float tBase = 0.0f;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
    }
    // Now keep the x axis at zero while moving the y
    for (float tBase : {-4.0f, -2.0f, 0.0f, 2.0f, 4.0f}) {
        float sBase = 0.0f;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }

    // All odd cases should be mirrored over the x axis
    for (float tBase : {-3.0f, -1.0f, 1.0f, 3.0f}) {
        float sBase = 0.0f;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
    }
    // The bottom-left to top right diagonal, odd
    for (float sBase : {-3.0f, -1.0f, 1.0f, 3.0f}) {
        float tBase = sBase;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }
    // The bottom-left to top right diagonal, even
    for (float sBase : {-4.0f, -2.0f, 2.0f, 4.0f}) {
        float tBase = sBase;
        // bottom left
        float s = sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }
    // The bottom-right to top-left diagonal, odd
    for (float sBase : {-3.0f, -1.0f, 1.0f, 3.0f}) {
        float tBase = sBase;
        // bottom left
        float s = -sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }
    // The bottom-right to top-left diagonal, even
    for (float sBase : {-4.0f, -2.0f, 2.0f, 4.0f}) {
        float tBase = sBase;
        // bottom left
        float s = -sBase;
        float t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        s = sBase + 0.5f - epsilon;
        t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
    }
}

TEST(texture2d, wrapModeMirroredRepeatMirroredRepeatEdgeCase)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    // Set the edge of the texture to off and make sure we do not hit the texture in the edge case
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::Repeat, Gfx2D::Texture::WrapMode::Repeat, PixelColor::off);
    constexpr float epsilon = std::numeric_limits<float>::epsilon() / 8.0f;

    float s = -epsilon;
    float t = -epsilon;
    EXPECT_NE(s, 0.0f);
    EXPECT_EQ(s - std::floor(s), 1.0f);
    EXPECT_NE(t, 0.0f);
    EXPECT_EQ(t - std::floor(t), 1.0f);
    EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
}

TEST(texture2d, wrapModeClampToEdgeClampToEdge)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::ClampToEdge, Gfx2D::Texture::WrapMode::ClampToEdge);
    constexpr float epsilon = 0.1;

    // Everything below zero, the lower left quadrant
    std::vector<float> numList = {-4.0f, -3.0f, -2.0f, -1.0f};
    for (float sBase : numList) {
        for (float tBase : numList) {
            // bottom left
            float s = sBase + 0.5f - epsilon;
            float t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // bottom right
            s = sBase + 1.0f - epsilon;
            t = tBase;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // top left
            s = sBase;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // top right
            s = sBase + 1.0f - epsilon;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        }
    }
    // Only s below zero: the upper left quadrant
    for (float sBase : numList) {
        for (float tBase : numList) {
            tBase *= -1;
            // bottom left
            float s = sBase + 0.5f - epsilon;
            float t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
            // bottom right
            s = sBase + 1.0f - epsilon;
            t = tBase;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
            // top left
            s = sBase;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
            // top right
            s = sBase + 1.0f - epsilon;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::transparent);
        }
    }
    // Only t below zero: the lower right quadrant
    for (float sBase : numList) {
        sBase *= -1;
        for (float tBase : numList) {
            // bottom left
            float s = sBase + 0.5f - epsilon;
            float t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
            // bottom right
            s = sBase + 1.0f - epsilon;
            t = tBase;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
            // top left
            s = sBase;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
            // top right
            s = sBase + 1.0f - epsilon;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::off);
        }
    }
    // s and t above zero, the upper right quadrant
    for (float sBase : numList) {
        sBase *= -1;
        for (float tBase : numList) {
            tBase *= -1;
            // bottom left
            float s = sBase + 0.5f - epsilon;
            float t = tBase + 0.5f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // bottom right
            s = sBase + 1.0f - epsilon;
            t = tBase;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // top left
            s = sBase;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
            // top right
            s = sBase + 1.0f - epsilon;
            t = tBase + 1.0f - epsilon;
            EXPECT_EQ(texture.getColorAt(s, t), PixelColor::on);
        }
    }
    // Now the two straight lines where one of the two is at zero.
    // First, keep s at zero
    numList = {-4.0f, -3.0f, -2.0f, -1.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    for (float tBase : numList) {
        float sBase = 0.0f;
        PixelColor leftCase, rightCase;
        if (tBase < 0) {
            leftCase = PixelColor::on;
            rightCase = PixelColor::off;
        } else {
            leftCase = PixelColor::transparent;
            rightCase = PixelColor::on;
        }
        // bottom left
        float s = sBase + 0.5f - epsilon;
        float t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), leftCase);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), rightCase);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), leftCase);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), rightCase);
    }
    // Then, keep the t at zero
    for (float sBase : numList) {
        float tBase = 0.0f;
        PixelColor topCase, bottomCase;
        if (sBase < 0) {
            topCase = PixelColor::transparent;
            bottomCase = PixelColor::on;
        } else {
            topCase = PixelColor::on;
            bottomCase = PixelColor::off;
        }
        // bottom left
        float s = sBase + 0.5f - epsilon;
        float t = tBase + 0.5f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), bottomCase);
        // bottom right
        s = sBase + 1.0f - epsilon;
        t = tBase;
        EXPECT_EQ(texture.getColorAt(s, t), bottomCase);
        // top left
        s = sBase;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), topCase);
        // top right
        s = sBase + 1.0f - epsilon;
        t = tBase + 1.0f - epsilon;
        EXPECT_EQ(texture.getColorAt(s, t), topCase);
    }

}

TEST(texture2d, wrapModeClampToBorderClampToBorder)
{
    // The easy thing here is that whatever the coordinate is, as long as its out of bounds it should
    // return the edge color.
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    constexpr float epsilon = 0.1;
    std::vector<float> numList = {-4.0f, -3.0f, -2.0f, -1.0f, 0.0f, 1.0f, 2.0f, 3.0f, 4.0f};
    for (PixelColor borderColor : {PixelColor::on, PixelColor::off, PixelColor::transparent}) {
        const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::ClampToBorder, Gfx2D::Texture::WrapMode::ClampToBorder,
                borderColor);
        for (float sBase : numList) {
            for (float tBase : numList) {
                if (sBase == 0.0f && tBase == 0.0f) {
                    continue;
                }
                // bottom left
                float s = sBase + 0.5f - epsilon;
                float t = tBase + 0.5f - epsilon;
                EXPECT_EQ(texture.getColorAt(s, t), borderColor);
                // bottom right
                s = sBase + 1.0f - epsilon;
                t = tBase;
                EXPECT_EQ(texture.getColorAt(s, t), borderColor);
                // top left
                s = sBase;
                t = tBase + 1.0f - epsilon;
                EXPECT_EQ(texture.getColorAt(s, t), borderColor);
                // top right
                s = sBase + 1.0f - epsilon;
                t = tBase + 1.0f - epsilon;
                EXPECT_EQ(texture.getColorAt(s, t), borderColor);
            }
        }
    }
    }
