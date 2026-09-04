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
		int16_t x0, int16_t y0,
		int16_t x1, int16_t y1
	);

	void drawFillQuad(
    	uint16_t color,
    	int16_t x,
    	int16_t y,
    	uint16_t width,
    	uint16_t height
	);

	void drawHollowQuad(
    	uint16_t color,
    	int16_t x,
    	int16_t y,
    	uint16_t width,
    	uint16_t height
	);

	void drawHollowCircle(
		uint16_t color,
		int16_t cx, int16_t cy,
		uint16_t r
	);
	
	void drawFillCircle(
		uint16_t color,
		int16_t cx, int16_t cy,
		uint16_t r
	);

	void drawHollowTriangle(
	    uint16_t color,
	    int16_t x0, int16_t y0,
	    int16_t x1, int16_t y1,
	    int16_t x2, int16_t y2
	);

	void drawFillTriangle(
	    uint16_t color,
	    int16_t x0, int16_t y0,
	    int16_t x1, int16_t y1,
	    int16_t x2, int16_t y2
	);

	void drawChar(
		uint16_t color,
		int16_t x, int16_t y,
		char c, uint8_t scale = 1
	);

	void drawText(
		uint16_t color,
		int16_t x, int16_t y,
		const char* text, uint8_t scale = 1
	);
private:
	Pixel framebuffer_[RENDERER_WIDTH * RENDERER_HEIGHT] = {0};
	drivers::St7735* display_ = nullptr;
};

}