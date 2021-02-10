
#ifndef USER_DEFINED_H
#define USER_DEFINED_H

#include <stdint.h>
#include <tmmintrin.h>

#include <string.h> // memset

typedef struct
{
    __m128i *buffer;
    uint32_t bytes_per_pixel;
    uint32_t num_vectors;
    uint32_t max_num_accum_log2;
} UserAccumBuff;


size_t UserAccumBuff_calculate_size(uint32_t num_vectors, uint32_t max_num_accum_log2)
{
    uint32_t num_bytes_overflow_per_pixel = (1 << max_num_accum_log2) / 8;
    if (num_bytes_overflow_per_pixel == 0)
        num_bytes_overflow_per_pixel = 1;
    return (size_t)num_vectors * (size_t)(1 + num_bytes_overflow_per_pixel) * sizeof(__m128i);
}

/// Initializes a rectangle accumulation region.
/// @param buffer A buffer to be used internally. All memory needed in a UserAccumBuff should use this buffer
///               as it is guaranteed to be in cached memory. This buffer will be allocated as many bytes as
///               specified by a call to UserAccumBuff_calculate_size().
///               If there are rare cases, like unexpected overflow or error handling, the user may use their
///               own buffer, however this will mess with cache efficency.
/// @param buffer_size Number of bytes allocated for buffer. This size will be exactly the size returned by
///                    the call to UserAccumBuff_calculate_size().
/// @param num_vectors Number of pixel vectors (__m128i's) to be stored in this accumulation buffer.
/// @param max_num_accum_log2 Worst-case log2 of the number of pixels that will be accumulated with this call.
///                           This may be useful for knowing how many integer overflows will occur during accumulation.
UserAccumBuff UserAccumBuff_init(__m128i *buffer, size_t buffer_size, uint32_t num_vectors, uint32_t max_num_accum_log2)
{
    UserAccumBuff ret;
    ret.buffer = buffer;
    ret.num_vectors = num_vectors;
    ret.max_num_accum_log2 = max_num_accum_log2;
    uint32_t num_bytes_overflow_per_pixel = (1 << max_num_accum_log2) / 8;
    if (num_bytes_overflow_per_pixel == 0)
        num_bytes_overflow_per_pixel = 1;
    ret.bytes_per_pixel = num_bytes_overflow_per_pixel + 1;
    memset(buffer, 0, buffer_size);
    return ret;
}

__m128i cmplt_epu8(__m128i x, __m128i y)
{
    __m128i msb_per_byte = _mm_set1_epi8(0b10000000); // TODO: I want constexpr :(
    x = _mm_xor_si128(x, msb_per_byte);
    y = _mm_xor_si128(y, msb_per_byte);
    return _mm_cmplt_epi8(x, y);
}

void UserAccumBuff_accum_pixel(UserAccumBuff *accum, uint32_t index, __m128i new_pixels)
{
    uint32_t accum_ptr_delta = accum->num_vectors;
    uint32_t bytes_per_pixel = accum->bytes_per_pixel;
    __m128i *old_accum_ptr = accum->buffer + index;

    // Add the low bytes
    __m128i old_value = _mm_load_si128(old_accum_ptr);
    __m128i added = _mm_add_epi8(old_value, new_pixels);
    _mm_store_si128(old_accum_ptr, added);
    __m128i overflow = cmplt_epu8(added, new_pixels);

    // Expect low bytes to overflow, so add the overflow to the 2nd bytes.
    old_accum_ptr += accum_ptr_delta;
    old_value = _mm_load_si128(old_accum_ptr);
    added = _mm_sub_epi8(old_value, overflow);
    overflow = cmplt_epu8(added, overflow);
    uint16_t has_overflow = _mm_movemask_epi8(overflow);
    _mm_store_si128(old_accum_ptr, added);

    // If the 2nd overflow byte overflows, go in a loop. Expect not taken!
    for (uint32_t i = 2; has_overflow && i < bytes_per_pixel; i++)
    {
        old_accum_ptr += accum_ptr_delta;
        old_value = _mm_load_si128(old_accum_ptr);
        added = _mm_sub_epi8(old_value, overflow);
        overflow = cmplt_epu8(added, overflow);
        has_overflow = _mm_movemask_epi8(overflow);
        _mm_store_si128(old_accum_ptr, added);
    }
}

// borrow is basically a carry, except negative values for overflow.
__m128i add_with_borrow(__m128i a, __m128i b, __m128i *borrow)
{
    __m128i sum1 = _mm_add_epi8(a, b);
    __m128i overflow1 = cmplt_epu8(sum1, b);
    __m128i sum2 = _mm_sub_epi8(sum1, *borrow);
    __m128i overflow2 = cmplt_epu8(sum2, *borrow);
    *borrow = _mm_add_epi8(overflow1, overflow2);
    return sum2;
}

void UserAccumBuff_accum_internal(UserAccumBuff *accum, uint32_t src_index, __m128i shuffle_mask, uint32_t dest_index)
{
    uint32_t accum_ptr_delta = accum->num_vectors;
    uint32_t bytes_per_pixel = accum->bytes_per_pixel;
    __m128i *src_ptr = accum->buffer + src_index;
    __m128i *dest_ptr = accum->buffer + dest_index;

    __m128i overflow = _mm_setzero_si128();
    for (uint32_t i = 0; i < bytes_per_pixel; i++)
    {
        __m128i src = _mm_load_si128(src_ptr);
        src = _mm_shuffle_epi8(src, shuffle_mask);
        __m128i dest = _mm_load_si128(dest_ptr);
        dest = add_with_borrow(src, dest, &overflow);
        _mm_store_si128(dest_ptr, dest);
        src_ptr += accum_ptr_delta;
        dest_ptr += accum_ptr_delta;
    }
}

typedef struct
{
    void *pixels;
    uint32_t width, height;
} Bitmap32BPPARGB;


void UserAccumBuff_retire(UserAccumBuff *accum, uint32_t index, uint32_t global_x, uint32_t global_y, void *user_data)
{
    
}

#endif // USER_DEFINED_H
