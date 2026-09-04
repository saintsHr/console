#pragma once

#include <cstdint>
#include "st7735.hpp"

namespace console {

using Pixel = uint16_t;

constexpr uint16_t RENDERER_WIDTH = 128;
constexpr uint16_t RENDERER_HEIGHT = 128;

class Renderer {
public:
	Renderer(drivers::St7735* display);

	void beginFrame(uint16_t color);
	void endFrame(uint8_t fps);

	void drawPixel(
	    uint16_t color,
	    int16_t x,
	    int16_t y
	);

	void drawLine(
		uint16_t color,
		uint16_t x0, uint16_t y0,
		uint16_t x1, uint16_t y1
	);

	void drawFillQuad(
    	uint16_t color,
    	uint16_t x,
    	uint16_t y,
    	uint16_t width,
    	uint16_t height
	);

	void drawHollowQuad(
    	uint16_t color,
    	uint16_t x,
    	uint16_t y,
    	uint16_t width,
    	uint16_t height
	);
private:
	Pixel framebuffer_[RENDERER_WIDTH * RENDERER_HEIGHT] = {0};
	drivers::St7735* display_ = nullptr;
};

}