#ifndef WS2812B_H
#define WS2812B_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define PIXEL_SNUM        6
typedef union {
    uint32_t rgb: 24;
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
} color_t;
void ws2812_init();
void ws2812_fill(uint32_t color);
void ws2812_set_pixel(uint16_t index, uint32_t color);
void ws2812_sync_frame();
#ifdef __cplusplus
}
#endif
#endif //WS2812B_H
