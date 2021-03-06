/*=============================================================================
   Copyright (c) 2016-2019 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_PHOTON_GUI_LIB_WIDGET_BUTTON_APRIL_21_2016)
#define CYCFI_PHOTON_GUI_LIB_WIDGET_BUTTON_APRIL_21_2016

#include <photon/element/layer.hpp>
#include <functional>

namespace cycfi { namespace photon
{
   ////////////////////////////////////////////////////////////////////////////
   // Momentary Button
   ////////////////////////////////////////////////////////////////////////////
   class basic_button : public array_composite<2, deck_element>
   {
   public:

      using base_type = array_composite<2, deck_element>;
      using button_function = std::function<void(bool)>;
      using base_type::value;

                        template <typename W1, typename W2>
                        basic_button(W1&& off, W2&& on);

      virtual element*  hit_test(context const& ctx, point p);
      virtual element*  click(context const& ctx, mouse_button btn);
      virtual void      drag(context const& ctx, mouse_button btn);
      virtual bool      is_control() const;

      virtual void      value(int new_state);
      virtual void      value(bool new_state);
      bool              value() const;

      button_function   on_click;

   protected:

      bool              state(bool new_state);

   private:

      bool              _state;
   };

   template <typename W1, typename W2>
   inline basic_button::basic_button(W1&& off, W2&& on)
    : _state(false)
   {
      (*this)[0] = share(std::forward<W1>(off));
      (*this)[1] = share(std::forward<W2>(on));
   }

   ////////////////////////////////////////////////////////////////////////////
   // Toggle Button
   ////////////////////////////////////////////////////////////////////////////
   class basic_toggle_button : public basic_button
   {
   public:
                        template <typename W1, typename W2>
                        basic_toggle_button(W1&& off, W2&& on);

      virtual element*  click(context const& ctx, mouse_button btn);
      virtual void      drag(context const& ctx, mouse_button btn);

   private:

      bool              _current_state;
   };

   template <typename W1, typename W2>
   inline basic_toggle_button::basic_toggle_button(W1&& off, W2&& on)
    : basic_button(std::forward<W1>(off), std::forward<W2>(on))
    , _current_state(false)
   {}

   ////////////////////////////////////////////////////////////////////////////
   // Latching Button
   ////////////////////////////////////////////////////////////////////////////
   class basic_latching_button : public basic_button
   {
   public:
                        template <typename W1, typename W2>
                        basic_latching_button(W1&& off, W2&& on);

      virtual element*  click(context const& ctx, mouse_button btn);
   };

   template <typename W1, typename W2>
   inline basic_latching_button::basic_latching_button(W1&& off, W2&& on)
    : basic_button(std::forward<W1>(off), std::forward<W2>(on))
   {}
}}

#endif