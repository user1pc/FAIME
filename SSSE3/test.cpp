
#include <stdio.h>
#include <stdint.h>

int RegionSquareIntersection(
    int top_left_x, int top_left_y,
    int top_right_x, int top_right_y,
    int bottom_left_x, int bottom_left_y,
    int bottom_right_x, int bottom_right_y,
    int x, int y, int width)
{
    // Top 2d "ray" is defined as a point at top left, direction towards top right.
    int top_line_dx = top_right_x - top_left_x;
    int top_line_dy = top_right_y - top_left_y;

    // Right 2d "ray" is defined as a point at top right, direction towards bottom right.
    int right_line_dx = bottom_right_x - top_right_x;
    int right_line_dy = bottom_right_y - top_right_y;

    // Bottom 2d "ray" is defined as a point at bottom right, direction towards bottom left.
    int bottom_line_dx = bottom_left_x - bottom_right_x;
    int bottom_line_dy = bottom_left_y - bottom_right_y;

    // Left 2d "ray" is defined as a point at bottom left, direction towards top left.
    int left_line_dx = top_left_x - bottom_left_x;
    int left_line_dy = top_left_y - bottom_left_y;


    // Calculate intersection of Top ray with the test region
    int top_intersection_t1_numer = ((top_line_dx >= 0) ? x : (x + width)) - top_left_x;
    int top_intersection_t1_denom = top_line_dx;
    int top_intersection_t2_numer = ((top_line_dy >= 0) ? y : (y + width)) - top_left_y;
    int top_intersection_t2_denom = top_line_dy;

    // Calculate intersection of Right ray with the test region
    int right_intersection_t1_numer = ((right_line_dx >= 0) ? x : (x + width)) - top_right_x;
    int right_intersection_t1_denom = right_line_dx;
    int right_intersection_t2_numer = ((right_line_dy >= 0) ? y : (y + width)) - top_right_y;
    int right_intersection_t2_denom = right_line_dy;

    // Calculate intersection of Bottom ray with the test region
    int bottom_intersection_t1_numer = ((bottom_line_dx >= 0) ? x : (x + width)) - bottom_right_x;
    int bottom_intersection_t1_denom = bottom_line_dx;
    int bottom_intersection_t2_numer = ((bottom_line_dy >= 0) ? y : (y + width)) - bottom_right_y;
    int bottom_intersection_t2_denom = bottom_line_dy;

    // Calculate intersection of Left ray with the test region
    int left_intersection_t1_numer = ((left_line_dx >= 0) ? x : (x + width)) - bottom_left_x;
    int left_intersection_t1_denom = left_line_dx;
    int left_intersection_t2_numer = ((left_line_dy >= 0) ? y : (y + width)) - bottom_left_y;
    int left_intersection_t2_denom = left_line_dy;

}

class MapTraversal
{
    public:
    int image_width;
    int top_left_a, top_right_a, bottom_left_a, bottom_right_a;
    int top_left_b, top_right_b, bottom_left_b, bottom_right_b;
    int top_left_c, top_right_c, bottom_left_c, bottom_right_c;
    int top_left_d, top_right_d, bottom_left_d, bottom_right_d;
    int top_left_e, top_right_e, bottom_left_e, bottom_right_e;
    int top_left_f, top_right_f, bottom_left_f, bottom_right_f;

    int top_left_t, top_right_t, bottom_left_t, bottom_right_t;
    int top_left_u, top_right_u, bottom_left_u, bottom_right_u;
    int top_left_v, top_right_v, bottom_left_v, bottom_right_v;

    int mapped_top_left_x, mapped_top_right_x, mapped_bottom_left_x, mapped_bottom_right_x;
    int mapped_top_left_y, mapped_top_right_y, mapped_bottom_left_y, mapped_bottom_right_y;
    int mapped_top_left_div, mapped_top_right_div, mapped_bottom_left_div, mapped_bottom_right_div;

    void UpdateMappedValues(int x_, int y_)
    {
        mapped_top_left_x = top_left_a * x_ + top_left_b * y_ + top_left_c;
        mapped_top_right_x = top_right_a * x_ + top_right_b * y_ + top_right_c;
        mapped_bottom_left_x = bottom_left_a * x_ + bottom_left_b * y_ + bottom_left_c;
        mapped_bottom_right_x = bottom_right_a * x_ + bottom_right_b * y_ + bottom_right_c;

        mapped_top_left_y = top_left_d * x_ + top_left_e * y_ + top_left_f;
        mapped_top_right_y = top_right_d * x_ + top_right_e * y_ + top_right_f;
        mapped_bottom_left_y = bottom_left_d * x_ + bottom_left_e * y_ + bottom_left_f;
        mapped_bottom_right_y = bottom_right_d * x_ + bottom_right_e * y_ + bottom_right_f;

        mapped_top_left_div = top_left_t * x_ + top_left_u * y_ + top_left_v;
        mapped_top_right_div = top_right_t * x_ + top_right_u * y_ + top_right_v;
        mapped_bottom_left_div = bottom_left_t * x_ + bottom_left_u * y_ + bottom_left_v;
        mapped_bottom_right_div = bottom_right_t * x_ + bottom_right_u * y_ + bottom_right_v;
    }

    MapTraversal(
        int tl_a, int tr_a, int bl_a, int br_a,
        int tl_b, int tr_b, int bl_b, int br_b,
        int tl_c, int tr_c, int bl_c, int br_c,
        int tl_d, int tr_d, int bl_d, int br_d,
        int tl_e, int tr_e, int bl_e, int br_e,
        int tl_f, int tr_f, int bl_f, int br_f,
        int tl_t, int tr_t, int bl_t, int br_t,
        int tl_u, int tr_u, int bl_u, int br_u,
        int tl_v, int tr_v, int bl_v, int br_v,
        int init_x, int init_y,
        int image_width_
    )
    {
        top_left_a = tl_a; top_right_a = tr_a; bottom_left_a = bl_a; bottom_right_a = br_a;
        top_left_b = tl_b; top_right_b = tr_b; bottom_left_b = bl_b; bottom_right_b = br_b;
        top_left_c = tl_c; top_right_c = tr_c; bottom_left_c = bl_c; bottom_right_c = br_c;
        top_left_d = tl_d; top_right_d = tr_d; bottom_left_d = bl_d; bottom_right_d = br_d;
        top_left_e = tl_e; top_right_e = tr_e; bottom_left_e = bl_e; bottom_right_e = br_e;
        top_left_f = tl_f; top_right_f = tr_f; bottom_left_f = bl_f; bottom_right_f = br_f;
        top_left_t = tl_t; top_right_t = tr_t; bottom_left_t = bl_t; bottom_right_t = br_t;
        top_left_u = tl_u; top_right_u = tr_u; bottom_left_u = bl_u; bottom_right_u = br_u;
        top_left_v = tl_v; top_right_v = tr_v; bottom_left_v = bl_v; bottom_right_v = br_v;
        image_width = image_width_;
    }

    void TraverseRegion(int current_x, int current_y, int half_width,
    int top_left_dx, int top_left_dy,
    int top_right_dx, int top_right_dy,
    int bottom_left_dx, int bottom_left_dy,
    int bottom_right_dx, int bottom_right_dy)
    {
        while (true)
        {
            if ()
        }
    }

    void TraverseTilNoLongerAPoint(int x, int y)
    {
        UpdateMappedValues(x, y);
        int half_width = image_width / 2;
        int current_x = 0;
        int current_y = 0;
        while (true)
        {
            // We are at some level, with 4 quadrants.
            // Want to see which of these 4 quadrants each of the 4 points belong in.
            // If all points belong in the same quadrant, keep zooming into that quadrant.
            // Otherwise we have traversed til the region is no longer a "point"

            int middle_x = current_x + half_width;
            int middle_y = current_y + half_width;
            
            int top_left_x = (mapped_top_left_x >= middle_x * mapped_top_left_div) ? 1 : 0;
            int top_left_y = (mapped_top_left_y >= middle_y * mapped_top_left_div) ? 1 : 0;
            int top_right_x = (mapped_top_right_x >= middle_x * mapped_top_right_div) ? 1 : 0;
            int top_right_y = (mapped_top_right_y >= middle_y * mapped_top_right_div) ? 1 : 0;
            int bottom_left_x = (mapped_bottom_left_x >= middle_x * mapped_bottom_left_div) ? 1 : 0;
            int bottom_left_y = (mapped_bottom_left_y >= middle_y * mapped_bottom_left_div) ? 1 : 0;
            int bottom_right_x = (mapped_bottom_right_x >= middle_x * mapped_bottom_right_div) ? 1 : 0;
            int bottom_right_y = (mapped_bottom_right_y >= middle_y * mapped_bottom_right_div) ? 1 : 0;

            if (top_left_x != top_right_x    || top_left_y != top_right_y    ||
                top_left_x != bottom_left_x  || top_left_y != bottom_left_y  ||
                top_left_x != bottom_right_x || top_left_y != bottom_right_y)
            {
                // Reached termination condition where not all 4 corners go to the same quadrant.
                
            }

            // All 4 points are the same, so zoom into that point.
            current_x += half_width * top_left_x;
            current_y += half_width * top_left_y;
            half_width /= 2;
            if (half_width == 0)
            {
                // TODO: this shouldn't happen, idk
            }
        }
    }

    void TraverseAll(int output_width, int output_height)
    {
        for (int y = 0; y < output_height; y++)
        {
            for (int x = 0; x < output_width; x++)
            {
                UpdateMappedValues(x, y);
            }
        }
    }
};

int main()
{
    for (int i = INT8_MIN; i <= (int)INT8_MAX; i++)
    {
        for (int j = INT8_MIN; j <= (int)INT8_MAX; j++)
        {
            int8_t a = (int8_t)i;
            int8_t b = (int8_t)j;
            uint8_t desired_a = (uint8_t)a;
            uint8_t desired_b = (uint8_t)b;
            bool correct = desired_a < desired_b;
            bool test = cmplt(a, b);
            if (correct != test)
            {
                printf("Wrong result.(unsigned) %i < %i ||| (signed) %i < %i\n",
                (int)desired_a, (int)desired_b, (int)a, (int)b);
                return 0;
            }
        }   
    }
}