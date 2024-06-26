/*
 * IC: WS2812B
 * SPI Baud Rate: 6.25MHz
 * send 1bit time = 1S / 6.25MHz = 160ns
 * send 1byte time = 160 * 8 = 1280ns
 * send 1Pixel time = 1280 * 3(RGB) = 3840ns
 * So: 384 Pixel * 3840ns =  1,474,560ns = 1474.56µs = 1.47456ms
 * 280µs = 280000ns
 *
 *  code 0 : T0H:220ns~380ns T0L:580ns~1µs
 *  _____T0H
 *  |   |_________|T0L
 *  T0H: 2*160ns = 320ns
 *  T0L: 6*160ns = 960ns
 *
 *  code 1 : T0H:580ns~1µs   T0L:580ns~1µs
 *  ________T1H
 *  |      |______|T1L
 *  T1H: 4*160ns = 640ns
 *  T1L: 4*160ns = 640ns
 *
 *  ReSet code, Frame unit, low voltage time >280µs (datasheet: 50µs type val)
 *  |_____________|
 *  280µs / 1280ns = 218.75 byte
 *  50µs / 1280ns = 39 byte
 *
 *  Time conversion:
 *  1s  = 1000ms
 *  1ms = 1000μs
 *  1μs = 1000ns
 *  1ns = 1000ps
 *
 *  FirstBit : LSB
 * */
#include "ws2812b.h"

#if 1

#include "spi.h"

// SPI speed 5.7M ~ 8MHz
enum : uint8_t {
    CODE_0 = 0b11100000,
    CODE_1 = 0b11110000
};
static const uint16_t pixel_snum = PIXEL_SNUM;
static color_t frame_cache[pixel_snum];
static uint8_t frame_cache_decoded[pixel_snum * 24];

extern "C" void ws2812_sync_frame()
{
    HAL_SPI_Transmit(&hspi2, frame_cache_decoded, sizeof(frame_cache_decoded), 100);
    uint8_t end_frame = 0;
    for (uint8_t i = 0; i < 40; ++i)
        HAL_SPI_Transmit(&hspi2, &end_frame, 1, 100);
}

extern "C" void ws2812_init()
{
    MX_SPI2_Init();
    // DeInit SPI_SCK pin,  only use MOSI pin
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
}

static void set_frame_buffer(uint16_t index, uint8_t r, uint8_t g, uint8_t b)
{
    uint8_t *pR = &frame_cache_decoded[(index) * 24 + 0];
    uint8_t *pG = &frame_cache_decoded[(index) * 24 + 8];
    uint8_t *pB = &frame_cache_decoded[(index) * 24 + 16];
    for (uint8_t i = 0; i < 8; i++) {
        *pR++ = ((r << i) & 0x80) ? CODE_1 : CODE_0;
        *pG++ = ((g << i) & 0x80) ? CODE_1 : CODE_0;
        *pB++ = ((b << i) & 0x80) ? CODE_1 : CODE_0;
    }
}

static void decode_pixel(uint16_t index, color_t color)
{
    for (uint8_t bit_index = 0; bit_index < 24; ++bit_index) {
        frame_cache_decoded[index * 24 + bit_index] = ((color.rgb << bit_index) & (1 << 23)) ? CODE_1 : CODE_0;
    }
}

static void decode_frame()
{
    for (uint16_t pixel_index = 0; pixel_index < pixel_snum; ++pixel_index) {
        for (uint8_t bit_index = 0; bit_index < 24; ++bit_index) {
            frame_cache_decoded[pixel_index * 24 + bit_index] = ((frame_cache[pixel_index].rgb << bit_index) & (1 << 23)) ? CODE_1 : CODE_0;
        }
    }
}

extern "C" void ws2812_fill(uint32_t color)
{
//    for (uint8_t i = 0; i < pixel_snum; ++i) {
//        frame_cache[i].rgb = color;
//    }
    for (auto & item : frame_cache) {
        item.rgb = color;
    }
    decode_frame();
}

extern "C" void ws2812_set_pixel(uint16_t index, uint32_t color)
{
    frame_cache[index].rgb = color;
    decode_pixel(index, frame_cache[index]);
}

#endif
