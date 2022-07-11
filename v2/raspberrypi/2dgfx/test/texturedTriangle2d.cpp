#include <gtest/gtest.h>

#include "texturedTriangle2d.hpp"

TEST(texturedTriangle2d, sanity)
{
    std::vector<PixelColor> texels({PixelColor::on, PixelColor::off, PixelColor::transparent, PixelColor::on});
    const Gfx2D::Texture texture(texels, 2, 2, Gfx2D::Texture::WrapMode::ClampToBorder, Gfx2D::Texture::WrapMode::ClampToBorder,
            PixelColor::on);
    Gfx2D::TexturedTriangle texTri({0,0}, {1,0}, {0,1}, texture, {{0,0}, {1,0}, {0,1}});

    EXPECT_TRUE(texTri.pointIsInDrawable({0.25, 0.25}));
    EXPECT_EQ(texTri.colorAtPoint({0.25, 0.25}), PixelColor::on);
    EXPECT_TRUE(texTri.pointIsInDrawable({0.75, 0.1}));
    EXPECT_EQ(texTri.colorAtPoint({0.75, 0.1}), PixelColor::off);
    EXPECT_TRUE(texTri.pointIsInDrawable({0.1, 0.75}));
    EXPECT_EQ(texTri.colorAtPoint({0.1, 0.75}), PixelColor::transparent);

    Gfx2D::TransformationMatrix move = Gfx2D::createTranslationMatrix(2, 1);
    texTri = texTri.createTransformedTriangle(move);

    EXPECT_FALSE(texTri.pointIsInDrawable({0.25, 0.25}));
    EXPECT_FALSE(texTri.pointIsInDrawable({0.75, 0.1}));
    EXPECT_FALSE(texTri.pointIsInDrawable({0.1, 0.75}));

    EXPECT_TRUE(texTri.pointIsInDrawable({2.25, 1.25}));
    EXPECT_EQ(texTri.colorAtPoint({2.25, 1.25}), PixelColor::on);
    EXPECT_TRUE(texTri.pointIsInDrawable({2.75, 1.1}));
    EXPECT_EQ(texTri.colorAtPoint({2.75, 1.1}), PixelColor::off);
    EXPECT_TRUE(texTri.pointIsInDrawable({2.1, 1.75}));
    EXPECT_EQ(texTri.colorAtPoint({2.1, 1.75}), PixelColor::transparent);
}
