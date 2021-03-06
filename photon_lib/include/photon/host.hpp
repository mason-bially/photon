/*=============================================================================
   Copyright (c) 2016-2019 Cycfi Research. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]

   Key mapping ported to C++ from GLFW3

   Copyright (c) 2009-2016 Camilla Berglund <elmindreda@glfw.org>

   This software is provided 'as-is', without any express or implied
   warranty. In no event will the authors be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
      claim that you wrote the original software. If you use this software
      in a product, an acknowledgment in the product documentation would
      be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not
      be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
      distribution.
=============================================================================*/
#if !defined(CYCFI_PHOTON_HOST_AUGUST_20_2016)
#define CYCFI_PHOTON_HOST_AUGUST_20_2016

#include <utility>
#include <memory>
#include <string>
#include <cstdint>
#include <functional>
#include <cairo.h>

#include <infra/support.hpp>
#include <photon/support/point.hpp>
#include <photon/support/rect.hpp>
#include <photon/window.hpp>

namespace cycfi { namespace photon
{
   ////////////////////////////////////////////////////////////////////////////
   // This header is self-contained, and should always be self contained. The
   // facilities in this header file contain all that is necessary to
   // implement platform specific behavior for a host platform application's
   // most basic functionalities, including support for 1) The main application,
   // 2) Creation and management of the main window, 3) Keyboard and mouse
   // event handling, 4) The clipboard.
   //
   // It is crucual, by design, to keep this header file always self contained
   // with no dependencies(*). The objective is the make it as easy as possible
   // to implement and maintain different hosts for various platforms and
   // applications. A platform (OS) may have multiple hosts for diverse needs.
   // For example, OSX may have a "Desktop App" host as well as an "AU"
   // (Audio Units) host that itself is typically a plugin guest, hosted by
   // another application.
   //
   // (* The only exception to this 'no-dependencies' rule is the necessary
   // inclusion of rect.hpp and point.hpp. Both have no dependencies.)
   //
   // Another crucial design strategy is the keep this header file free from
   // any platform specific code (public types, public functions, etc.) that
   // may pollute the clean platform independent API.
   ////////////////////////////////////////////////////////////////////////////


   ////////////////////////////////////////////////////////////////////////////
   // Mouse Button
   ////////////////////////////////////////////////////////////////////////////
   struct mouse_button
   {
      enum what { left, middle, right };

      bool     down;
      int      num_clicks;
      what     state;
      int      modifiers;
      point    pos;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Cursor tracking
   ////////////////////////////////////////////////////////////////////////////
   enum class cursor_tracking
   {
      entering,   // Sent when the cursor is entering the view
      hovering,   // Sent when the cursor is hovering over the view
      leaving     // Sent when the cursor is leaving the view
   };

   ////////////////////////////////////////////////////////////////////////////
   // View Limits
   ////////////////////////////////////////////////////////////////////////////
   struct view_limits
   {
      point    min;
      point    max;
   };

   constexpr float         full_extent    = 1E6;
   constexpr view_limits   full_limits    = { { 0.0, 0.0 }, { full_extent, full_extent } };

   ////////////////////////////////////////////////////////////////////////////
   // Text info
   ////////////////////////////////////////////////////////////////////////////
   struct text_info
   {
      uint32_t codepoint;
      int      modifiers;
   };

   ////////////////////////////////////////////////////////////////////////////
   // Focus request
   ////////////////////////////////////////////////////////////////////////////
   enum class focus_request
   {
      wants_focus,
      begin_focus,
      end_focus
   };

   ////////////////////////////////////////////////////////////////////////////
   // Keyboard information
   ////////////////////////////////////////////////////////////////////////////
   enum class key_action
   {
      unknown           = -1,
      release           = 0,
      press             = 1,
      repeat            = 2,
   };

   enum : uint16_t
   {
      mod_shift         = 0x0001,
      mod_control       = 0x0002,
      mod_alt           = 0x0004,
      mod_super         = 0x0008,
   };

   enum class key_code : int16_t
   {
      unknown           = -1,

      // Printable keys
      space             = 32,
      apostrophe        = 39,    // '
      comma             = 44,    // ,
      minus             = 45,    // -
      period            = 46,    // .
      slash             = 47,    // /
      _0                = 48,
      _1                = 49,
      _2                = 50,
      _3                = 51,
      _4                = 52,
      _5                = 53,
      _6                = 54,
      _7                = 55,
      _8                = 56,
      _9                = 57,
      semicolon         = 59,    // ;
      equal             = 61,    // =
      a                 = 65,
      b                 = 66,
      c                 = 67,
      d                 = 68,
      e                 = 69,
      f                 = 70,
      g                 = 71,
      h                 = 72,
      i                 = 73,
      j                 = 74,
      k                 = 75,
      l                 = 76,
      m                 = 77,
      n                 = 78,
      o                 = 79,
      p                 = 80,
      q                 = 81,
      r                 = 82,
      s                 = 83,
      t                 = 84,
      u                 = 85,
      v                 = 86,
      w                 = 87,
      x                 = 88,
      y                 = 89,
      z                 = 90,
      left_bracket      = 91,    // [
      backslash         = 92,    // \ (back-slash)
      right_bracket     = 93,    // ]
      grave_accent      = 96,    // `
      world_1           = 161,   // non-US #1
      world_2           = 162,   // non-US #2

      // Function keys
      escape            = 256,
      enter             = 257,
      tab               = 258,
      backspace         = 259,
      insert            = 260,
      _delete           = 261,
      right             = 262,
      left              = 263,
      down              = 264,
      up                = 265,
      page_up           = 266,
      page_down         = 267,
      home              = 268,
      end               = 269,
      caps_lock         = 280,
      scroll_lock       = 281,
      num_lock          = 282,
      print_screen      = 283,
      pause             = 284,
      f1                = 290,
      f2                = 291,
      f3                = 292,
      f4                = 293,
      f5                = 294,
      f6                = 295,
      f7                = 296,
      f8                = 297,
      f9                = 298,
      f10               = 299,
      f11               = 300,
      f12               = 301,
      f13               = 302,
      f14               = 303,
      f15               = 304,
      f16               = 305,
      f17               = 306,
      f18               = 307,
      f19               = 308,
      f20               = 309,
      f21               = 310,
      f22               = 311,
      f23               = 312,
      f24               = 313,
      f25               = 314,
      kp_0              = 320,
      kp_1              = 321,
      kp_2              = 322,
      kp_3              = 323,
      kp_4              = 324,
      kp_5              = 325,
      kp_6              = 326,
      kp_7              = 327,
      kp_8              = 328,
      kp_9              = 329,
      kp_decimal        = 330,
      kp_divide         = 331,
      kp_multiply       = 332,
      kp_subtract       = 333,
      kp_add            = 334,
      kp_enter          = 335,
      kp_equal          = 336,
      left_shift        = 340,
      left_control      = 341,
      left_alt          = 342,
      left_super        = 343,
      right_shift       = 344,
      right_control     = 345,
      right_alt         = 346,
      right_super       = 347,
      menu              = 348,

      last              = menu,
   };

   struct key_info
   {
      key_code          key;
      key_action        action;
      int               modifiers;
   };

   ////////////////////////////////////////////////////////////////////////////
   // The base view base class
   ////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
   using host_view = void*;
#elif defined(_WIN32)
   using host_view = HWND;
#elif defined(__linux__)
   using host_view = GtkWidget*;
#endif

   class base_view : non_copyable
   {
   public:
                     base_view(host_window h);
      virtual        ~base_view();

      virtual void   draw(cairo_t* ctx, rect area) {};
      virtual void   click(mouse_button btn) {}
      virtual void   drag(mouse_button btn) {}
      virtual void   cursor(point p, cursor_tracking status) {}
      virtual void   scroll(point dir, point p) {}
      virtual void   key(key_info const& k) {}
      virtual void   text(text_info const& info) {}
      virtual void   focus(focus_request r) {}

      void           refresh();
      void           refresh(rect area);
      void           limits(view_limits limits_);

      point          cursor_pos() const;
      point          size() const;
      void           size(point p);
      bool           is_focus() const;
      host_view      host() const { return _view; }

   private:

      host_view      _view;
   };



   // ////////////////////////////////////////////////////////////////////////////
   // // The view base class
   // ////////////////////////////////////////////////////////////////////////////
   // class base_view
   // {
   // public:
   //                   base_view(host_view* h) : h(h) {}
   //                   base_view(base_view const&) = delete;
   //    virtual        ~base_view() {}
   //    base_view&     operator=(base_view const&) = delete;

   //    virtual void   draw(cairo_t* ctx, rect area) {};
   //    virtual void   click(mouse_button btn) {}
   //    virtual void   drag(mouse_button btn) {}
   //    virtual void   cursor(point p, cursor_tracking status) {}
   //    virtual void   scroll(point dir, point p) {}
   //    virtual void   key(key_info const& k) {}
   //    virtual void   text(text_info const& info) {}
   //    virtual void   focus(focus_request r) {}

   //    void           refresh();
   //    void           refresh(rect area);
   //    void           limits(view_limits limits_);

   //    point          cursor_pos() const;
   //    point          size() const;
   //    void           size(point p);
   //    bool           is_focus() const;

   // private:

   //    friend struct platform_access;
   //    host_view*     h;
   // };

   // ////////////////////////////////////////////////////////////////////////////
   // // Application event loop entry
   // int app_main(int argc, const char* argv[]);

   // ////////////////////////////////////////////////////////////////////////////
   // // View creation callback
   // extern std::function<std::unique_ptr<base_view>(host_view* h)> new_view;

   ////////////////////////////////////////////////////////////////////////////
   // The clipboard
   std::string clipboard();
   void clipboard(std::string const& text);

   ////////////////////////////////////////////////////////////////////////////
   // The Cursor
   enum class cursor_type
   {
      arrow,
      ibeam,
      cross_hair,
      hand,
      h_resize,
      v_resize
   };

   void set_cursor(cursor_type type);
}}

#endif
