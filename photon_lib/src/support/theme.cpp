/*=============================================================================
   Copyright (c) 2016-2019 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#include <photon/support/theme.hpp>
#include <photon/view.hpp>

namespace cycfi { namespace photon
{
   // The global theme
   theme _theme;

   theme const& get_theme()
   {
      return _theme;
   }

   void set_theme(theme const& thm)
   {
      _theme = thm;
   }
}}
