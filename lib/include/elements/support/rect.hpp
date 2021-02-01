#ifndef ELEMENTS_RECT
#define ELEMENTS_RECT

#include <elements/support/point.hpp>

namespace cycfi::elements
{
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_rect;
    using rect = basic_rect<point::coordinate_type>;

    template<typename CoordinateType, typename>
    struct basic_rect
    {
        using coordinate_type = CoordinateType;

        constexpr basic_rect()
            :
              left(coordinate_type{}),
              top(coordinate_type{}),
              right(coordinate_type{}),
              bottom(coordinate_type{}) {}

        constexpr basic_rect(coordinate_type _left, coordinate_type _top, coordinate_type _right, coordinate_type _bottom)
                : left(_left), top(_top), right(_right), bottom(_bottom) {}

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect(const basic_point<T>& origin, coordinate_type right, coordinate_type bottom)
            : basic_rect(origin.x, origin.y, right, bottom) {}

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr basic_rect(coordinate_type left, coordinate_type top, const basic_extent<T>& size)
            : basic_rect(left, top, left + size.width, top + size.height) {}

        template <typename T1, typename T2, typename T = std::common_type_t<T1, T2>, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, T2>>>
        constexpr basic_rect(const basic_point<T1>& origin, const basic_extent<T2>& size)
            : basic_rect<T>(origin.x, origin.y, origin.x + size.width, origin.y + size.height) {}

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator==(const basic_rect<T> & other) const
        {
//            return basic_point<coordinate_type>{left, top} == basic_point<coordinate_type>{other.left, other.top}
//                   && basic_point<coordinate_type>{right, bottom} == basic_point<coordinate_type>{other.right, other.bottom};

          return other.left == left && other.top == top && other.right == right && other.bottom == bottom;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool operator!=(const basic_rect<T> & other) const
        {
            return !this->operator==(std::forward<const basic_rect<T> &>(other));
        }

        [[nodiscard]] constexpr bool is_empty() const
        {
            return left == right || top == bottom;
        }

        [[nodiscard]] constexpr bool is_valid() const
        {
            return left <= right && top <= bottom;
        }

        // [x, y)
        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        [[nodiscard]] constexpr bool includes(const basic_point<T>& p) const
        {
            return p.template horizontal_between(left, right) && p.template vertical_between(top, bottom);
        }

        // [x, y)
        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        [[nodiscard]] constexpr bool includes(const basic_rect<T>& other) const
        {
            return (other.left >= left) && (other.left < right) &&
                   (other.top >= top) && (other.top < bottom) &&
                   (other.right >= left) && (other.right < right) &&
                   (other.bottom >= top) && (other.bottom < bottom);
        }

        [[nodiscard]] constexpr coordinate_type width() const
        {
            return right - left;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void widen(T width)
        {
            right = left + width;
        }

        [[nodiscard]] constexpr coordinate_type height() const
        {
            return bottom - top;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void heighten(T height)
        {
            bottom = top + height;
        }

        [[nodiscard]] constexpr basic_extent<coordinate_type> size() const
        {
            return {width(), height()};
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void resize_right(const basic_extent<T>& size)
        {
            right = left + size.width;
            bottom = top + size.height;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr void resize_left(const basic_extent<T>& size)
        {
            left = right - size.width;
            top = bottom - size.height;
        }

        constexpr void clear()
        {
            *this = basic_rect<coordinate_type>();
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> left_top() const
        {
            return {left, top};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> right_bottom() const
        {
            return {right, bottom};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> right_top() const
        {
            return {right, top};
        }

        [[nodiscard]] constexpr basic_point<coordinate_type> left_bottom() const
        {
            return {left, bottom};
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr basic_rect<coordinate_type> move(T1 dx, T2 dy) const
        {
            auto rect = *this;

            rect.left += dx;
            rect.right += dx;

            rect.top += dy;
            rect.bottom += dy;

            return rect;
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        constexpr void reposition(T1 x, T2 y)
        {
            right -= (left - x);
            left = x;

            bottom -= (top - y);
            top = y;
        }

        template <typename T1, typename T2, typename = std::enable_if_t<std::is_arithmetic_v<T1> && std::is_arithmetic_v<T2> && std::is_convertible_v<T1, coordinate_type> && std::is_convertible_v<T2, coordinate_type>>>
        [[nodiscard]] constexpr rect inset(T1 horizontal = 1, T2 vertical = 1) const
        {
            auto r = *this;

            r.left += horizontal;
            r.right -= horizontal;

            r.top += vertical;
            r.bottom -= vertical;

            if (!r.is_valid())
            {
                r.clear();
            }

            return r;
        }

        template <typename T, typename = std::enable_if_t<std::is_arithmetic_v<T> && std::is_convertible_v<T, coordinate_type>>>
        constexpr bool is_intersects(const basic_rect<T>& other) const
        {
            if(other.left >= right || other.right <= left || other.top >= bottom || other.bottom <= top)
            {
                return false;
            }
            return true;
        }

        coordinate_type left;
        coordinate_type top;
        coordinate_type right;
        coordinate_type bottom;
   };

    ////////////////////////////////////////////////////////////////////////////
    // Free Functions
    ////////////////////////////////////////////////////////////////////////////

    constexpr point      center_point(rect r);
    constexpr float      area(rect r);
    rect                 max(rect a, rect b);
    rect                 min(rect a, rect b);

    rect                 center(rect r, rect encl);
    rect                 center_h(rect r, rect encl);
    rect                 center_v(rect r, rect encl);
    rect                 align(rect r, rect encl, float x_align, float y_align);
    rect                 align_h(rect r, rect encl, float x_align);
    rect                 align_v(rect r, rect encl, float y_align);
    rect                 clip(rect r, rect encl);

   constexpr point center_point(rect r)
   {
      return { r.left + (r.width() / 2.0f), r.top + (r.height() / 2.0f) };
   }

   constexpr float area(rect r)
   {
      return r.width() * r.height();
   }
}

#endif
