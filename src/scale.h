#include <Arduino.h>

void scaleImage(const uint16_t *src, uint16_t *dst, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
    float x_ratio = srcWidth / (float)dstWidth;
    float y_ratio = srcHeight / (float)dstHeight;
    for (int y = 0; y < dstHeight; y++)
    {
        for (int x = 0; x < dstWidth; x++)
        {
            int src_x = (int)(x * x_ratio);
            int src_y = (int)(y * y_ratio);
            dst[y * dstWidth + x] = src[src_y * srcWidth + src_x];
        }
    }
}

void scaleRGB565Image(uint16_t *originalImage, uint16_t *scaledImage, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
{
    // 计算水平和垂直缩放比例（放大倍数的倒数）
    int xScale = (srcWidth << 16) / dstWidth;
    int yScale = (srcHeight << 16) / dstHeight;

    // 循环遍历缩放后图像的每个像素
    for (int y = 0; y < dstHeight; ++y)
    {
        for (int x = 0; x < dstWidth; ++x)
        {
            // 计算在原始图像中对应的像素位置
            int nearestX = (x * xScale) >> 16;
            int nearestY = (y * yScale) >> 16;

            // 确保计算得到的位置在原始图像范围内
            nearestX = nearestX < 0 ? 0 : nearestX;
            nearestX = nearestX >= srcWidth ? srcWidth - 1 : nearestX;
            nearestY = nearestY < 0 ? 0 : nearestY;
            nearestY = nearestY >= srcHeight ? srcHeight - 1 : nearestY;

            // 获取在原始图像中对应位置的像素值并存储到缩放后图像数组中
            scaledImage[y * dstWidth + x] = originalImage[nearestY * srcWidth + nearestX];
        }
    }
}
