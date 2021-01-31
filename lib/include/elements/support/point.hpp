#ifndef ELEMENTS_POINT
#define ELEMENTS_POINT

#include <type_traits>
#include <cmath>

namespace cycfi::elements
{
    template<typename SizeType, typename = std::enable_if_t<std::is_arithmetic_v<SizeType>>>
    struct basic_extent;
    ////////////////////////////////////////////////////////////////////////////
    // Points
    ////////////////////////////////////////////////////////////////////////////
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_point
    {
        using coordinate_type = CoordinateType;

        constexpr basic_point() : x(coordinate_type{}), y(coordinate_type{}) {}

        constexpr basic_point(coordinate_type _x, coordinate_type _y) : x(_x), y(_y) {}

        // cast a extent to point, todo: delete it
        constexpr explicit operator basic_extent<coordinate_type>()
        {
            return basic_extent<coordinate_type>(x, y);
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator==(const basic_point<T>& other) const noexcept
        {
            return other.x == x && other.y == y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator!=(const basic_point<T>& other) const noexcept
        {
            return !this->operator==(std::forward<const basic_point<T>&>(other));
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator>(const basic_point<T>& other) const noexcept
        {
            return x > other.x && y > other.y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator>=(const basic_point<T>& other) const noexcept
        {
            return this->template operator>(std::forward<const basic_point<T>&>(other)) || this->template operator==(std::forward<const basic_point<T>&>(other));
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator<(const basic_point<T>& other) const noexcept
        {
            return !(this->template operator>=(std::forward<const basic_point<T>&>(other)));
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool operator<=(const basic_point<T>& other) const noexcept
        {
            return !(this->template operator>(std::forward<const basic_point<T>&>(other)));
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point operator+(const basic_point<T>& other) noexcept
        {
            return {other.x + x, other.y + y};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point operator+(T distance) noexcept
        {
            return {x + distance, y + distance};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point& operator+=(const basic_point<T>& other) noexcept
        {
            x += other.x;
            y += other.y;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point& operator+=(T distance) noexcept
        {
            x += distance;
            y += distance;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point operator-(const basic_point<T>& other) noexcept
        {
            return {other.x - x, other.y - y};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point operator-(T distance) noexcept
        {
            return {x - distance, y - distance};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point& operator-=(const basic_point<T>& other) noexcept
        {
            x -= other.x;
            y -= other.y;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr basic_point& operator-=(T distance) noexcept
        {
            x -= distance;
            y -= distance;
            return *this;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool horizontal_greater_than(T _x) const noexcept
        {
            return x > _x;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool horizontal_equal(T _x) const noexcept
        {
            return x == _x;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool horizontal_greater_equal(T _x) const noexcept
        {
            return horizontal_greater_than(_x) || horizontal_equal(_x);
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool vertical_greater_than(T _y) const noexcept
        {
            return y > _y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool vertical_equal(T _y) const noexcept
        {
            return y == _y;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr bool vertical_greater_equal(T _y) const noexcept
        {
            return vertical_greater_than(_y) || vertical_equal(_y);
        }

        // horizontal distance from other to this, if other in this' right side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType horizontal_distance(const basic_point<T>& other) const noexcept
        {
            return other.x - x + 1;
        }

        // horizontal distance from other to this, if other in this' right side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType horizontal_distance(T _x) const noexcept
        {
            return _x - x + 1;
        }

        // vertical distance from other to this, if other in this' bottom side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType vertical_distance(const basic_point<T>& other) const noexcept
        {
            return other.y - y + x;
        }

        // vertical distance from other to this, if other in this' bottom side, return value should greater than 0
        template <typename T, typename RetType = std::common_type_t<T, coordinate_type>, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr RetType vertical_distance(T _y) const noexcept
        {
            return _y - y + x;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, CoordinateType>>>
        constexpr auto get_distance(const basic_point<T>& other) noexcept
        {
            return std::sqrt(std::pow(horizontal_distance(std::forward<const basic_point<T>&>(other)), 2) + std::pow(vertical_distance(std::forward<const basic_point<T>&>(other)), 2));
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void reposition(T _x, T _y) noexcept
        {
            x = _x;
            y = _y;
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void horizontal_move(T dx) noexcept
        {
            x += dx;
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void vertical_move(T dy) noexcept
        {
            y += dy;
        }

        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void move_to(T dx, T dy) noexcept
        {
            horizontal_move(dx);
            vertical_move(dy);
        }

        // clamp a point in the specified area (not allowed on the edge of the area)
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void clamp_upper(T _x, T _y) noexcept
        {
            x = (x >= _x) ? (_x - 1) : x;
            y = (y >= _y) ? (_y - 1) : y;
        }

        // clamp a point in the specified area (allowed on the edge of the area)
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void clamp_lower(T _x, T _y) noexcept
        {
            x = (x < _x) ? _x : x;
            y = (y < _y) ? _y : y;
        }

        // [x1, x2)
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool horizontal_between(T x1, T x2) const noexcept
        {
            return horizontal_greater_equal(x1) && !horizontal_greater_equal(x2);
        }

        // [y1, y2)
        template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool vertical_between(T y1, T y2) const noexcept
        {
            return vertical_greater_equal(y1) && !vertical_greater_equal(y2);
        }

        coordinate_type             x;
        coordinate_type             y;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Sizes
    ////////////////////////////////////////////////////////////////////////////
    template<typename SizeType, typename>
    struct basic_extent
    {
        using size_type = SizeType;

        constexpr basic_extent() : width(size_type{}), height(size_type{}) {}

        constexpr basic_extent(size_type _width, size_type _height) : width(_width), height(_height) {}

        // cast a extent to point, todo: delete it
        constexpr explicit operator basic_point<size_type>()
        {
            return basic_point<size_type>(width, height);
        }

        size_type width;
        size_type height;
   };

    using point = basic_point<float>;
    using extent = basic_extent<float>;
}

#endif
