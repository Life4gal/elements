#ifndef ELEMENTS_RECT
#define ELEMENTS_RECT

#include <elements/support/point.hpp>

namespace cycfi::elements
{
    ////////////////////////////////////////////////////////////////////////////
    // rect
    ////////////////////////////////////////////////////////////////////////////
    struct rect
    {
        using coordinate_type = point::coordinate_type;

        constexpr         rect()
            :
              left(coordinate_type{}),
              top(coordinate_type{}),
              right(coordinate_type{}),
              bottom(coordinate_type{}) {}
        constexpr         rect(coordinate_type _left, coordinate_type _top, coordinate_type _right, coordinate_type _bottom)
                :
              left(_left),
              top(_top),
              right(_right),
              bottom(_bottom) {}
        constexpr         rect(point origin, coordinate_type right, coordinate_type bottom)
            :
              rect(origin.x, origin.y, right, bottom) {}
        constexpr         rect(coordinate_type left, coordinate_type top, extent size)
            :
              rect(left, top, left + size.width, top + size.height) {}
        constexpr         rect(point origin, extent size)
            :
              rect(origin.x, origin.y, origin.x + size.width, origin.y + size.height) {}

        constexpr bool    operator==(const rect & other) const
        {
            return other.left == left && other.top == top && other.right == right && other.bottom == bottom;
        }
        constexpr bool    operator!=(const rect & other) const
        {
            return !this->operator==(std::forward<const rect&>(other));
        }

        constexpr bool    is_empty() const
        {
            return left == right || top == bottom;
        }
        constexpr bool    includes(point p) const;
        constexpr bool    includes(rect other) const;

        constexpr float   width() const;
        constexpr void    width(coordinate_type width_);
        constexpr float   height() const;
        constexpr void    height(coordinate_type height_);
        constexpr extent  size() const;
        constexpr void    size(extent size_);

        constexpr point   top_left() const;
        constexpr point   bottom_right() const;
        constexpr point   top_right() const;
        constexpr point   bottom_left() const;

        constexpr rect    move(coordinate_type dx, coordinate_type dy) const;
        constexpr rect    move_to(coordinate_type x, coordinate_type y) const;
        constexpr rect    inset(coordinate_type x_inset = 1.0, coordinate_type y_inset = 1.0) const;

        coordinate_type             left;
        coordinate_type             top;
        coordinate_type             right;
        coordinate_type             bottom;
   };

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
   // Inlines
   ////////////////////////////////////////////////////////////////////////////
   constexpr bool rect::includes(point p) const
   {
      return
         (p.x >= left) && (p.x <= right) &&
         (p.y >= top) && (p.y <= bottom)
         ;
   }

   constexpr bool rect::includes(rect other) const
   {
      return
         (other.left >= left) && (other.left <= right) &&
         (other.top >= top) && (other.top <= bottom) &&
         (other.right >= left) && (other.right <= right) &&
         (other.bottom >= top) && (other.bottom <= bottom)
         ;
   }

   constexpr float rect::width() const
   {
      return right - left;
   }

   constexpr void rect::width(float width_)
   {
      right = left + width_;
   }

   constexpr float rect::height() const
   {
      return (bottom - top);
   }

   constexpr void rect::height(float height_)
   {
      bottom = top + height_;
   }

   constexpr extent rect::size() const
   {
      return { width(), height() };
   }

   constexpr void rect::size(extent size_)
   {
      right = left + size_.width;
      bottom = top + size_.height;
   }

   constexpr point rect::top_left() const
   {
      return { left, top };
   }

   constexpr point rect::bottom_right() const
   {
      return { right, bottom };
   }

   constexpr point rect::top_right() const
   {
      return { right, top };
   }

   constexpr point rect::bottom_left() const
   {
      return { left, bottom };
   }

   constexpr rect rect::move(float dx, float dy) const
   {
      rect r = *this;
      r.top += dy;
      r.left += dx;
      r.bottom += dy;
      r.right += dx;
      return r;
   }

   constexpr rect rect::move_to(float x, float y) const
   {
      return move(x-left, y-top);
   }

   constexpr rect rect::inset(float x_inset, float y_inset) const
   {
      rect r = *this;
      r.top += y_inset;
      r.left += x_inset;
      r.bottom -= y_inset;
      r.right -= x_inset;

      if (!is_valid(r))
         clear(r);

      return r;
   }

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
