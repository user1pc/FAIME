
#ifndef FAIME2D_H
#define FAIME2D_H

#include <stdint.h>
#include <tmmintrin.h>


__m128i user_defined_accum(__m128i old_val, __m128i add_me)
{
    return _mm_add_epi8(old_val, add_me);
}

typedef struct
{
    __m128i *buffer;
    uint32_t width, height;
} AccumBuffer;

typedef struct
{
    const __m128i *image_data;
    uint32_t bytes_per_pixel;
    uint32_t image_x, image_y;
    uint32_t level_size;
    uint32_t width, height;

} Faime2DCachedLevelIterator;


Faime2DCachedLevelIterator Faime2DCachedLevelIterator_init(
    const __m128i *image_data,
    uint32_t bytes_per_pixel,
    uint32_t level_size,
    uint32_t initial_image_x,
    uint32_t initial_image_y,
    uint32_t initial_width,
    uint32_t initial_height)
{
    Faime2DCachedLevelIterator ret;
    ret.image_data = image_data;
    ret.bytes_per_pixel = bytes_per_pixel;
    ret.level_size = level_size;
    ret.image_x = initial_image_x;
    ret.image_y - initial_image_y;
    ret.width = initial_width;
    ret.height = initial_height;
    return ret;
}

void Faime2DCachedLevelIterator_apply(Faime2DCachedLevelIterator *iter, AccumBuffer *accum)
{

}

typedef struct
{
    const void *image_data[32];
    uint32_t stride, height;
    uint32_t bytes_per_pixel;
    uint32_t in_x, in_y;
    uint32_t out_x, out_y;
    const uint32_t target_load_chunk_size = 4 * 1024; // number of bytes for ideal linear access. maybe min(non-temporal prefetch buffer size, RAM row size)?
    const uint32_t l2_cache_size = 128 * 1024; // approximately the L2 cache size per core. Maybe with room for pollution.
    uint32_t output_buffer_width, output_buffer_height;
    __m128i *output_buffer;
} Faime2D;

#endif // FAIME2D_H
