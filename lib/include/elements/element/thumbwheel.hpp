/*=============================================================================
   Copyright (c) 2016-2020 Joel de Guzman

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(ELEMENTS_THUMBWHEEL_SEPTEMBER_22_2020)
#define ELEMENTS_THUMBWHEEL_SEPTEMBER_22_2020

#include <elements/element/dial.hpp> // $$$ for now $$$

#include <elements/element/proxy.hpp>
#include <elements/element/tracker.hpp>
#include <elements/element/port.hpp>
#include <elements/support.hpp>
#include <infra/support.hpp>
#include <functional>

namespace cycfi { namespace elements
{
   ////////////////////////////////////////////////////////////////////////////
   // Thumbwheels
   ////////////////////////////////////////////////////////////////////////////
   class thumbwheel_base : public dial_base
   {
   public:

      using dial_base::dial_base;

   protected:

       double              compute_value(context const& ctx, tracker_info& track_info) override;
   };

   template <typename Subject>
   inline proxy<remove_cvref_t<Subject>, thumbwheel_base>
   thumbwheel(Subject&& subject, double init_value = 0.0)
   {
      return { std::forward<Subject>(subject), init_value };
   }

   ////////////////////////////////////////////////////////////////////////////
   // Basic (vertical and horizontal) thumbwheels (You can use these as the
   // subject of thumbwheel)
   //
   //    A thumbwheel subject can be constructed using a vport or hport (see
   //    port.hpp) and a receiver<double> which recieves a 0.0 to 1.0 value
   //    that determines the vertical position of the content.
   //
   //    The thumbwheel, like the dial, is a continuous device. If you need
   //    the value to be quantized, you can pass a non-zero value to the
   //    constructor. For example, a quantize value of 0.25 will quantize the
   //    possible values to 0.0, 0.25, 0.5, 0.75 and 1.0.
   ////////////////////////////////////////////////////////////////////////////
   class basic_thumbwheel_element : public basic_receiver<double>
   {
   public:

      basic_thumbwheel_element(float quantize_ = 0.0)
       : _quantize(quantize_)
      {}

      virtual void         align(double val) = 0;
      virtual double       align() const = 0;
      void                 value(double val) override;

   protected:

      float                quantize() const { return _quantize; }
      void                 make_aligner(context const& ctx);
      void                 do_align(view& view_, rect const& bounds, double val);

   private:

      using align_function = std::function<void(double val)>;

      float                _quantize;
      align_function       _aligner;
   };

   struct basic_vthumbwheel_element : vport_element, basic_thumbwheel_element
   {
      using basic_thumbwheel_element::basic_thumbwheel_element;

      view_limits          limits(basic_context const& ctx) const override;
      void                 draw(context const& ctx) override;
      void                 align(double val) override;
      double               align() const override;
   };

   struct basic_hthumbwheel_element : hport_element, basic_thumbwheel_element
   {
      using basic_thumbwheel_element::basic_thumbwheel_element;

      view_limits          limits(basic_context const& ctx) const override;
      void                 draw(context const& ctx) override;
      void                 align(double val) override;
      double               align() const override;
   };

   template <typename Subject>
   inline proxy<remove_cvref_t<Subject>, basic_vthumbwheel_element>
   basic_vthumbwheel(Subject&& subject, double quantize_ = 0.0)
   {
      return { std::forward<Subject>(subject), quantize_ };
   }

   template <typename Subject>
   inline proxy<remove_cvref_t<Subject>, basic_hthumbwheel_element>
   basic_hthumbwheel(Subject&& subject, double quantize_ = 0.0)
   {
      return { std::forward<Subject>(subject), quantize_ };
   }
}}

#endif
