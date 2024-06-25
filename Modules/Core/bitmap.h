#ifndef GF_RTOS_BITMAP_H
#define GF_RTOS_BITMAP_H

#include <stdint.h>

typedef struct {
    uint32_t bitmap;
} bitmap_t;

//inline void bitmap_init(bitmap_t *bitmap) { bitmap->bitmap = 0; }
//inline void bitmap_set(bitmap_t *bitmap, const uint32_t pos) { bitmap->bitmap |= (1 << pos); }
//inline void bitmap_clear(bitmap_t *bitmap, const uint32_t pos) { bitmap->bitmap &= ~(1 << pos); }
//inline uint32_t bitmapPosCount(void) { return sizeof(bitmap_t) * 8; }
void bitmap_init(bitmap_t *bitmap);
void bitmap_set(bitmap_t *bitmap, uint32_t pos);
void bitmap_clear(bitmap_t *bitmap, uint32_t pos);
uint32_t bitmapPosCount(void);
uint32_t bitmap_getFirstSet(bitmap_t *bitmap);

#endif //GF_RTOS_BITMAP_H
