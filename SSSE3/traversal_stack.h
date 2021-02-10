
#ifndef TRAVERSAL_STACK_H
#define TRAVERSAL_STACK_H

#include <stdint.h>
#include <tmmintrin.h>

typedef struct
{
    __m128i x_offset, y_offset;
    uint32_t x_base, y_base;
    uint32_t level;
} TraversalStack;

__m128i TraversalStack_push(__m128i dx, __m128i dy, __m128i )

#endif // TRAVERSAL_STACK_H
