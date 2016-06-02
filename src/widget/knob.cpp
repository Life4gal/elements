/*=================================================================================================
   Copyright (c) 2016 Joel de Guzman

   Licensed under a Creative Commons Attribution-ShareAlike 4.0 International.
   http://creativecommons.org/licenses/by-sa/4.0/
=================================================================================================*/
#include <photon/widget/knob.hpp>
#include <photon/theme.hpp>
#include <photon/support.hpp>
#include <photon/app.hpp>
#include <photon/window.hpp>
#include <cmath>

namespace photon
{
   namespace
   {
      float const travel = 0.83;
      float const rng = (2 * M_PI) * travel;
      float const offs = (2 * M_PI) * (1-travel)/2;

      // size of knob, inset with gauge
      float const knob_size = 0.72;

      void draw_knob(canvas& canvas_, circle cp, color fill_color)
      {
         float radius = cp.radius;
         float shadow = radius/8;

         // Knob Shadow
         paint shc
            = canvas_.radial_gradient(
                  point{ cp.cx, cp.cy }, radius, radius+shadow,
                  color{ 0, 0, 0, 64 }, color{ 0, 0, 0, 0 }
               );

         canvas_.begin_path();
         canvas_.circle({ cp.cx, cp.cy, radius+shadow });
         canvas_.circle(cp);
         canvas_.path_winding(canvas::hole);
         canvas_.fill_paint(shc);
         canvas_.fill();

         // Knob
         float    bevel = radius/5;

         paint knob
            = canvas_.linear_gradient(
                  point{ cp.cx, cp.cy-(radius-bevel) },
                  point{ cp.cx, cp.cy+(radius-bevel) },
                  color{ 255, 255, 255, 16 }, color{ 0, 0, 0, 16 }
               );

         canvas_.begin_path();
         canvas_.circle({ cp.cx, cp.cy, radius });
         canvas_.fill_color(fill_color);
         canvas_.fill();
         canvas_.fill_paint(knob);

         // Knob bevel
         paint bvc
            = canvas_.radial_gradient(
                  point{ cp.cx, cp.cy }, radius-bevel, radius,
                  color{ 0, 0, 0, 0 }, color{ 0, 0, 0, 64 }
               );

         canvas_.begin_path();
         canvas_.circle(cp);
         canvas_.circle({ cp.cx, cp.cy, radius-bevel });
         canvas_.path_winding(canvas::hole);
         canvas_.fill_paint(bvc);
         canvas_.fill();

         // Knob Outline
         canvas_.begin_path();
         canvas_.circle(circle{ cp.cx, cp.cy, cp.radius-1.0f });
         canvas_.stroke_width(1);
         canvas_.stroke_color(fill_color.level(0.6));
         canvas_.stroke();
      }

      point draw_knob_indicator(
         canvas& canvas_, circle cp, float pos, color indicator_color)
      {
         auto state = canvas_.new_state();

         canvas_.translate({ cp.cx, cp.cy });
         canvas_.rotate(offs + (pos * rng));

         float r = cp.radius;
         float ind_w = r/8;
         float ind_h = r/2.5;
         rect  ind_r = { -ind_w/2, -ind_h/2, ind_w/2, ind_h/2 };
         ind_r = ind_r.move(0, r*0.7);

         // Fill
         paint gr
            = canvas_.linear_gradient(
                  ind_r.top_right(), ind_r.bottom_right(),
                  color{ 0, 0, 0, 90 }, color{ 127, 127, 127, 90 }
               );

         canvas_.begin_path();
         canvas_.round_rect(ind_r, ind_w * 0.3);
         canvas_.fill_color(indicator_color);
         canvas_.fill();
         canvas_.fill_paint(gr);

         // Glow
         float ind_glow = ind_w * 0.7;
         paint glow_paint
            = canvas_.box_gradient(ind_r.inset(-ind_glow * 0.25, -ind_glow * 0.25)
             , ind_glow, ind_glow, indicator_color, color(0, 0, 0, 0)
            );

         canvas_.begin_path();
         canvas_.rect(ind_r.inset(-ind_glow, -ind_glow));
         canvas_.round_rect(ind_r, ind_w * 0.3);
         canvas_.path_winding(canvas::hole);
         canvas_.fill_paint(glow_paint);
         canvas_.fill();

         return canvas_.transform_point(center_point(ind_r));
      }

      void draw_knob_gauge(
         canvas& canvas_, float pos, circle cp
       , color controls_color, color indicator_color, bool pan)
      {
         auto  state = canvas_.new_state();
         float start = (M_PI / 2) + ((2 * M_PI) * (1-travel)/2);
         float ind_start = start;
         float pos_ = start + (pos * rng);
         float end = start +  rng;
         point grad_start;
         point grad_end;

         if (pan)
         {
            if (pos > 0.5)
            {
               ind_start += (0.5 * rng);
               grad_start = point{ cp.cx, cp.cy };
               grad_end = point{ cp.cx+cp.radius, cp.cy };
            }
            else
            {
               float new_pos = start + (0.5 * rng);
               ind_start = pos_;
               pos_ = new_pos;
               grad_start = point{ cp.cx, cp.cy };
               grad_end = point{ cp.cx-cp.radius, cp.cy };
            }
         }
         else
         {
            grad_start = point{ cp.cx-cp.radius, cp.cy };
            grad_end = point{ cp.cx+cp.radius, cp.cy };
         }

         paint grad
            = canvas_.linear_gradient(
                  grad_start, grad_end,
                  controls_color, indicator_color.level(1.5)
               );

         canvas_.begin_path();
         canvas_.arc({ cp.cx, cp.cy, cp.radius*1.25f }, start, end);
         canvas_.stroke_width(cp.radius/5);
         canvas_.stroke_color(controls_color.opacity(0.6));
         canvas_.stroke();

         canvas_.begin_path();
         canvas_.arc({ cp.cx, cp.cy, cp.radius*1.25f }, ind_start, pos_);
         canvas_.stroke_width(cp.radius/5);
         canvas_.stroke_paint(grad);
         canvas_.stroke();
      }
   }

   void knob::draw_knob(theme& thm, rect bounds, bool hilite)
   {
      auto     c = center_point(bounds);
      auto     r = std::min(bounds.width(), bounds.height())/2;
      circle   cp = { c.x, c.y, r };
      auto     controls_color = thm.controls_color;

      if (hilite)
         controls_color = controls_color.opacity(0.5).level(1.5);

      photon::draw_knob(thm.canvas(), cp, controls_color);
   }

   point knob::draw_indicator(theme& thm, rect bounds, bool hilite)
   {
      auto     c = center_point(bounds);
      auto     r = std::min(bounds.width(), bounds.height())/2;
      circle   cp = { c.x, c.y, r };
      auto     indicator_color = thm.indicator_color.level(1.5);

      if (hilite)
         indicator_color = indicator_color.opacity(1).level(2.0);

      return draw_knob_indicator(thm.canvas(), cp, _pos, indicator_color);
   }

   void knob::draw_gauge(theme& thm, rect bounds, bool hilite)
   {
      auto  c = center_point(bounds);
      auto  r = std::min(bounds.width(), bounds.height())/2;
      auto  cp = circle{ c.x, c.y, r };

      auto  indicator_color = thm.indicator_color.level(1.2);
      auto  controls_color = thm.controls_color.opacity(1.0).level(1.5);

      if (hilite)
      {
         controls_color = controls_color.level(1.5);
         indicator_color = indicator_color.level(1.3);
      }

      draw_knob_gauge(
         thm.canvas(), _pos, cp, controls_color, indicator_color, _pan
      );
   }

   rect knob::limits(basic_context const& ctx) const
   {
      return { 16, 16, full_extent, full_extent };
   }

   widget* knob::hit_test(context const& ctx, point p)
   {
      auto&    canvas_ = ctx.canvas();
      auto     c = center_point(ctx.bounds);
      auto     r = std::min(ctx.bounds.width(), ctx.bounds.height())/2;

      canvas_.begin_path();
      canvas_.circle(circle{ c.x, c.y, r });
      canvas_.fill_color({ 0, 0, 0, 0 });
      canvas_.fill();

      if (canvas_.on_fill(p))
         return this;
      return 0;
   }

   void knob::draw(context const& ctx)
   {
      auto&    thm = ctx.theme();
      auto     mp = ctx.cursor_pos();
      bool     hilite = _tracking || hit_test(ctx, mp);

      // make the bounds smaller to accomodate the gauge
      rect  b = ctx.bounds;
      b.width(b.width() * knob_size);
      b.height(b.height() * knob_size);
      b = center(b, ctx.bounds);

      draw_knob(thm, b, hilite);
      draw_gauge(thm, b, hilite);
      _indicator_pos = draw_indicator(thm, b, hilite);
   }

   widget* knob::click(context const& ctx, mouse_button btn)
   {
      _tracking = btn.is_pressed;
      if (_tracking)
      {
         point mp = ctx.cursor_pos();
         _offset = point{ mp.x-_indicator_pos.x, mp.y-_indicator_pos.y };
         reposition(ctx);
      }
      ctx.window.draw();

      return this;
   }

   void knob::drag(context const& ctx, mouse_button btn)
   {
      if (_tracking)
         reposition(ctx);
   }

   void knob::reposition(context const& ctx)
   {
      point mp = ctx.cursor_pos();
      mp = mp.move(-_offset.x, -_offset.y);
      _offset.x *= 0.95;
      _offset.y *= 0.95;

      point center = center_point(ctx.bounds);
      float angle = -std::atan2(mp.x-center.x, mp.y-center.y);
      if (angle < 0.0f)
         angle += 2 * M_PI;

      float val = (angle-offs) / rng;
      if (std::abs(val - _pos) < 0.6)
         limit(_pos = val, 0.0, 1.0);
      ctx.window.draw();
   }

   bool knob::is_control() const
   {
      return true;
   }

   bool knob::scroll(context const& ctx, point p)
   {
      _pos += p.y * 0.01;
      limit(_pos, 0.0, 1.0);
      ctx.window.draw();
      return true;
   }

   image_knob::image_knob(
      image_ptr img_
    , float size_
    , std::size_t num_images_
    , bool pan_)
    : knob(pan_)
    , _img(img_)
    , _size(size_)
    , _num_images(num_images_)
   {}

   rect image_knob::limits(basic_context const& ctx) const
   {
      return { 32, 32, _size, _size };
   }

   void image_knob::draw(context const& ctx)
   {
      auto&    thm = ctx.theme();
      auto     mp = ctx.cursor_pos();
      bool     hilite = tracking() || hit_test(ctx, mp);
      rect     bounds = ctx.bounds;
      auto     dia = std::min(bounds.width(), bounds.height());

      bounds.width(dia);
      bounds.height(dia);
      bounds = center(bounds, ctx.bounds);

      draw_knob(thm, bounds, hilite);

      float    radius = bounds.width();
      float    inset = (radius - (radius * knob_size)) / 2;
      draw_gauge(thm, bounds.inset(inset, inset), hilite);

      indicator_pos(draw_indicator(thm, bounds, hilite));
   }

   void image_knob::draw_knob(theme& thm, rect bounds, bool hilite)
   {
      float scale = bounds.width() / _size;
      _img->draw(bounds, { 0.0f, float(_size * int(_num_images * position())) }, scale);
   }

   point image_knob::draw_indicator(theme& thm, rect bounds, bool hilite)
   {
      auto&    canvas_ = thm.canvas();
      auto     state = canvas_.new_state();
      auto     c = center_point(bounds);
      circle   cp = { c.x, c.y, bounds.width()/2 };

      canvas_.translate({ cp.cx, cp.cy });
      canvas_.rotate(offs + (position() * rng));

      float r = cp.radius;
      float ind_w = r/8;
      float ind_h = r/2.5;
      rect  ind_r = { -ind_w/2, -ind_h/2, ind_w/2, ind_h/2 };
      ind_r = ind_r.move(0, r*0.7);

      return canvas_.transform_point(center_point(ind_r));
   }
}