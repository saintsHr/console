#include "renderer.hpp"
#include "freertos/idf_additions.h"
#include "portmacro.h"
#include <cstdint>

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