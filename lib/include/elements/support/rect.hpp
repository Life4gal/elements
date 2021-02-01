#ifndef ELEMENTS_RECT
#define ELEMENTS_RECT

#include <elements/support/point.hpp>

namespace cycfi::elements
{
    template<typename CoordinateType, typename = std::enable_if_t<std::is_arithmetic_v<CoordinateType>>>
    struct basic_rect;
    using rect = basic_rect<point::coordinate_type>;

    ////////////////////////////////////////////////////////////////////////////
    // Free Functions
    ////////////////////////////////////////////////////////////////////////////
    constexpr bool       is_valid(rect r);
    constexpr bool       is_same_size(rect a, rect b);
    bool                 intersects(rect a, rect b);

    constexpr point      center_point(rect r);
    constexpr float      area(rect r);
    rect                 max(rect a, rect b);
    rect                 min(rect a, rect b);

    constexpr void       clear(rect& r);
    rect                 center(rect r, rect encl);
    rect                 center_h(rect r, rect encl);
    rect                 center_v(rect r, rect encl);
    rect                 align(rect r, rect encl, float x_align, float y_align);
    rect                 align_h(rect r, rect encl, float x_align);
    rect                 align_v(rect r, rect encl, float y_align);
    rect                 clip(rect r, rect encl);

    ////////////////////////////////////////////////////////////////////////////
    // rect
    ////////////////////////////////////////////////////////////////////////////
    template<typename CoordinateType, typename>
    struct basic_rect
    {
        using coordinate_type = point::coordinate_type;

        constexpr basic_rect()
            :
              left(coordinate_type{}),
              top(coordinate_type{}),
              right(coordinate_type{}),
              bottom(coordinate_type{}) {}

        constexpr basic_rect(coordinate_type _left, coordinate_type _top, coordinate_type _right, coordinate_type _bottom)
                : left(_left), top(_top), right(_right), bottom(_bottom) {}
        constexpr basic_rect(point origin, coordinate_type right, coordinate_type bottom)
            :
                basic_rect(origin.x, origin.y, right, bottom) {}
        constexpr basic_rect(coordinate_type left, coordinate_type top, extent size)
            :
                basic_rect(left, top, left + size.width, top + size.height) {}
        constexpr basic_rect(point origin, extent size)
            :
        basic_rect(origin.x, origin.y, origin.x + size.width, origin.y + size.height) {}

        constexpr bool operator==(const rect & other) const
        {
            return other.left == left && other.top == top && other.right == right && other.bottom == bottom;
        }
        constexpr bool operator!=(const rect & other) const
        {
            return !this->operator==(std::forward<const rect&>(other));
        }

        constexpr bool is_empty() const
        {
            return left == right || top == bottom;
        }

        constexpr bool includes(point p) const
        {
            return (p.x >= left) && (p.x <= right) &&
                   (p.y >= top) && (p.y <= bottom);
        }

        constexpr bool includes(rect other) const
        {
            return (other.left >= left) && (other.left <= right) &&
                   (other.top >= top) && (other.top <= bottom) &&
                   (other.right >= left) && (other.right <= right) &&
                   (other.bottom >= top) && (other.bottom <= bottom);
        }

        constexpr float width() const
        {
            return right - left;
        }

        constexpr void width(coordinate_type width_)
        {
            right = left + width_;
        }

        constexpr float height() const
        {
            return bottom - top;
        }

        constexpr void height(coordinate_type height_)
        {
            bottom = top + height_;
        }

        constexpr extent size() const
        {
            return {width(), height()};
        }

        constexpr void size(extent size_)
        {
            right = left + size_.width;
            bottom = top + size_.height;
        }

        constexpr point top_left() const
        {
            return {left, top};
        }

        constexpr point bottom_right() const
        {
            return {right, bottom};
        }

        constexpr point top_right() const
        {
            return {right, top};
        }

        constexpr point bottom_left() const
        {
            return {left, bottom};
        }

        constexpr rect move(coordinate_type dx, coordinate_type dy) const
        {
            auto rect = *this;
            rect.top += dy;
            rect.left += dx;
            rect.bottom += dy;
            rect.right += dx;
            return rect;
        }

        constexpr rect move_to(coordinate_type x, coordinate_type y) const
        {
            return move(x-left, y-top);
        }

        constexpr rect inset(coordinate_type x_inset = 1.0, coordinate_type y_inset = 1.0) const
        {
            auto r = *this;
            r.top += y_inset;
            r.left += x_inset;
            r.bottom -= y_inset;
            r.right -= x_inset;

            if (!is_valid(r))
                clear(r);

            return r;
        }

        coordinate_type left;
        coordinate_type top;
        coordinate_type right;
        coordinate_type bottom;
   };

   constexpr bool is_valid(rect r)
   {
      return (r.left <= r.right) && (r.top <= r.bottom);
   }

   constexpr bool is_same_size(rect a, rect b)
   {
      return (a.width() == b.width()) && (a.height() == b.height());
   }

   constexpr point center_point(rect r)
   {
      return { r.left + (r.width() / 2.0f), r.top + (r.height() / 2.0f) };
   }

   constexpr float area(rect r)
   {
      return r.width() * r.height();
   }

   constexpr void clear(rect& r)
   {
      r.left = 0.0;
      r.top  = 0.0;
      r.right = 0.0;
      r.bottom = 0.0;
   }
}

#endif
