/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/20/2010
  @version  $Id: windowfilter.h 193 2014-11-10 23:51:41Z tirons $
 **/

#ifndef  WINDOWFILTER_INC
#define  WINDOWFILTER_INC
#include "Filter.h"

namespace Lemma {

    // ===================================================================
    //  Class:  WindowFilter
    /**
      @class   WindowFilter
      \ingroup LemmaCore
      \brief   Fourier domain window filter.
      \details Current types are Hamming and Hanning. Others to be added.
     */
    // ===================================================================
    class WindowFilter : public Filter {

        friend std::ostream &operator<<(std::ostream &stream, const WindowFilter& ob);

        struct ctor_key {};

        public:

            // ====================  LIFECYCLE     =======================

            /** Default constructor, inaccessible use NewSP  */
            explicit WindowFilter ( const ctor_key& );

            /** DeSerializing constructor, usees factory DeSerialize  method, inaccessible use DeSerialize*/
            WindowFilter ( const YAML::Node& node, const ctor_key& );

            /** Default constructor. */
            virtual ~WindowFilter ( );

            /**
             *  Uses YAML to serialize this object.
             *  @return a YAML::Node
             */
            YAML::Node Serialize() const;

            /**
             *   Constructs an object from a YAML::Node.
             */
            static std::shared_ptr< WindowFilter > DeSerialize(const YAML::Node& node);

            /**
             *  Factory method for generating concrete class.
             *  @return a std::shared_ptr of type WindowFilter
             */
            static std::shared_ptr< WindowFilter > NewSP();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            /// Sets the bandwith of the window.
            /// @param[in] width Bandwidth in Hz
            void SetBandwidth(const Real& width);

            /// Sets the sampling rate of an input time domain signal.
            /// @param[in] dt is the sampling rate, in seconds.
            void SetSamplingInterval(const Real& dt);

            /// Specifies the length of the time series to apply filter to.
            void SetNumberOfSamples(const int& nt);

            /// Sets the type of window to compute.
            void SetWindowType(const WINDOWTYPE& type);

            /** Returns the number of frequency bins for whole data record.
             *  this is different then the length of the filter. This is
             *  simply the length of the real to complex FFT, or
             *  Nw = Nt%2 ? (Nt-1)/2+1 : (Nt)/2+1;
             *  @return the number of frequency bins.
             */
            int GetNumberOfFrequencyBins();

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

            /// Returns a Vector of the filter coefficients.
            VectorXr GetFilterCoefficients( );

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const ;

        protected:

            // ====================  LIFECYCLE     =======================


        private:
            // ====================  DATA MEMBERS  =========================

            /// Width of the window
            int         Width;

            /// Length of time series
            int         Nt;

            /// Length of the frequency series representation
            int         Nw;

            /// Sampling rate of a time domain signal
            Real        SamplingRate;

            /// Bandwith of filter
            Real        Bandwidth;

            /// The filter coefficients
            VectorXr    Coefficients;

            /// The type of filter to use
            WINDOWTYPE  Type;

            /** ASCII string representation of the class name */
            static constexpr auto CName = "WindowFilter";

            WindowFilter ( const WindowFilter& ) = delete;

    }; // -----  end of class  WindowFilter  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef WINDOWFILTER_INC  -----
