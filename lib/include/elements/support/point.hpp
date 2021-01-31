#ifndef ELEMENTS_POINT
#define ELEMENTS_POINT

#include <type_traits>

namespace cycfi::elements
{
    ////////////////////////////////////////////////////////////////////////////
    // Points
    ////////////////////////////////////////////////////////////////////////////
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_point
    {
        using coordinate_type = CoordinateType;

        constexpr         basic_point() : x(coordinate_type{}), y(coordinate_type{}) {}
        constexpr         basic_point(coordinate_type _x, coordinate_type _y) : x(_x), y(_y) {}

        constexpr bool    operator==(const basic_point & other) const
        {
            return other.x == x && other.y == y;
        }
        constexpr bool    operator!=(const basic_point & other) const
        {
            return !this->operator==(other);
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void move_to(T dx, T dy)
        {
            x += dx;
            y += dy;
        }

        coordinate_type             x;
        coordinate_type             y;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Sizes
    ////////////////////////////////////////////////////////////////////////////
    template<typename SizeType, typename = std::enable_if_t<std::is_arithmetic_v<SizeType>>>
    struct basic_extent : basic_point<SizeType>
    {
        using size_type = SizeType;

        using basic_point<size_type>::basic_point;

        // extent will use this ctor, but the IDE always warning me that these ctor maybe not be used
        [[maybe_unused]] explicit basic_extent(basic_point<size_type> p) : basic_point<size_type>(p) {}

    private:
        // do not use these
        using basic_point<size_type>::move_to;
   };

    using point = basic_point<float>;
    using extent = basic_extent<float>;
}

#endif
