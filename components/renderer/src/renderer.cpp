#include "renderer.hpp"
#include "freertos/idf_additions.h"
#include "portmacro.h"
#include <cstdint>
#include <algorithm>

static void swapInt16(int16_t &a, int16_t &b) {
    int16_t tmp = a;
    a = b;
    b = tmp;
}

namespace console {

Renderer::Renderer(drivers::St7735* display) : display_(display) {}

void Renderer::beginFrame(uint16_t color) {
	for (uint16_t y = 0; y < RENDERER_HEIGHT; y++) {
        for (uint16_t x = 0; x < RENDERER_WIDTH; x++) {
        	drawPixel(color, x, y);
    	}
    }
}

void Renderer::drawPixel(
    uint16_t color,
    int16_t x,
    int16_t y
) {
    if (x < 0 || y < 0) return;
    if (x >= RENDERER_WIDTH || y >= RENDERER_HEIGHT) return;
    framebuffer_[y * RENDERER_WIDTH + x] = color;
}

void Renderer::drawLine(
	uint16_t color,
	uint16_t x0, uint16_t y0,
	uint16_t x1, uint16_t y1
) {
	int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;

    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (true) {
        drawPixel(color, x0, y0);

        if (x0 == x1 && y0 == y1) break;

        int e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Renderer::drawFillQuad(
    uint16_t color,
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height
) {
    for (uint16_t py = y; py < y + height; py++) {
        for (uint16_t px = x; px < x + width; px++) {
            drawPixel(color, px, py);
        }
    }
}

void Renderer::drawHollowQuad(
    uint16_t color,
    uint16_t x,
    uint16_t y,
    uint16_t width,
    uint16_t height
) {
	drawLine(color, x, y, x + width, y);
    drawLine(color, x + width, y, x + width, y + height);
    drawLine(color, x + width, y + height, x, y + height);
    drawLine(color, x, y + height, x, y);
}

void Renderer::drawHollowCircle(
    uint16_t color,
    int16_t cx, int16_t cy,
    int16_t r
) {
    int16_t x = r;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        drawPixel(color, cx + x, cy + y);
        drawPixel(color, cx + y, cy + x);
        drawPixel(color, cx - y, cy + x);
        drawPixel(color, cx - x, cy + y);
        drawPixel(color, cx - x, cy - y);
        drawPixel(color, cx - y, cy - x);
        drawPixel(color, cx + y, cy - x);
        drawPixel(color, cx + x, cy - y);

        y += 1;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void Renderer::drawFillCircle(
    uint16_t color,
    int16_t cx, int16_t cy,
    int16_t r
) {
    int16_t x = r;
    int16_t y = 0;
    int16_t err = 0;

    while (x >= y) {
        drawLine(color, cx - x, cy + y, cx + x, cy + y);
        drawLine(color, cx - x, cy - y, cx + x, cy - y);
        drawLine(color, cx - y, cy + x, cx + y, cy + x);
        drawLine(color, cx - y, cy - x, cx + y, cy - x);

        y += 1;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

void Renderer::drawHollowTriangle(
    uint16_t color,
    int16_t x0, int16_t y0,
    int16_t x1, int16_t y1,
    int16_t x2, int16_t y2
) {
    drawLine(color, x0, y0, x1, y1);
    drawLine(color, x1, y1, x2, y2);
    drawLine(color, x2, y2, x0, y0);
}

void Renderer::drawFillTriangle(
    uint16_t color,
    int16_t x0, int16_t y0,
    int16_t x1, int16_t y1,
    int16_t x2, int16_t y2
) {
    if (y0 > y1) { swapInt16(y0, y1); swapInt16(x0, x1); }
    if (y1 > y2) { swapInt16(y1, y2); swapInt16(x1, x2); }
    if (y0 > y1) { swapInt16(y0, y1); swapInt16(x0, x1); }

    if (y0 == y2) {
        int16_t minX = std::min({x0, x1, x2});
        int16_t maxX = std::max({x0, x1, x2});
        drawLine(color, minX, y0, maxX, y0);
        return;
    }

    auto interpX = [](int16_t xa, int16_t ya, int16_t xb, int16_t yb, int16_t y) -> int16_t {
        if (yb == ya) return xa;
        return xa + (int32_t)(xb - xa) * (y - ya) / (yb - ya);
    };

    bool secondHalf = false;
    for (int16_t y = y0; y <= y2; y++) {
        if (y > y1) secondHalf = true;

        int16_t xLong = interpX(x0, y0, x2, y2, y);
        int16_t xShort;

        if (!secondHalf) {
            xShort = (y1 != y0) ? interpX(x0, y0, x1, y1, y) : x1;
        } else {
            xShort = (y2 != y1) ? interpX(x1, y1, x2, y2, y) : x1;
        }

        if (xShort > xLong) swapInt16(xShort, xLong);
        drawLine(color, xShort, y, xLong, y);
    }
}

void Renderer::endFrame(uint8_t fps) {
	if (!display_) return;

	display_->draw(framebuffer_);

	if (fps == 0) return;

    static TickType_t lastFrame = xTaskGetTickCount();
    TickType_t frameTicks = pdMS_TO_TICKS(1000 / fps);

    vTaskDelayUntil(
        &lastFrame,
        frameTicks
    );
}

}