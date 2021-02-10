
#ifndef USER_DEFINED_MAPPING_H
#define USER_DEFINED_MAPPING_H

#include <stdint.h>
#include <tmmintrin.h>

// This mapping is a 3D plane mapping.
// The vector-value function is defined as:
// F_x(x,y) = (ax+by)/(tx+uy)
// F_y(x,y) = (cx+dy)/(tx+uy)
// There are 4 corners of each pixel, so 4 different
// coeffecients for each corner.
// The region between these 4 pixels will be considered inside
// the mapped region.
typedef struct 
{
    int32_t top_left_a, top_right_a, bottom_left_a, bottom_right_a;
    int32_t top_left_b, top_right_b, bottom_left_b, bottom_right_b;
    int32_t top_left_c, top_right_c, bottom_left_c, bottom_right_c;
    int32_t top_left_d, top_right_d, bottom_left_d, bottom_right_d;
    int32_t top_left_t, top_right_t, bottom_left_t, bottom_right_t;
    int32_t top_left_u, top_right_u, bottom_left_u, bottom_right_u;
    
} UserDefinedMapping;

UserDefinedMapping UserDefinedMapping_init(int32_t x_base, int32_t y_base, uint32_t depth,
__m128i x_offset, __m128i y_offset)
{
    UserDefinedMapping ret;
    
}

__m128i GeometryIntersectionTest(uint32_t input_x, uint32_t input_y, uint32_t mip_level,
uint32_t output_x_base, uint32_t output_y_base, __m128i output_x_offset, __m128i output_y_offset)
{

}

#endif // USER_DEFINED_MAPPING_H
