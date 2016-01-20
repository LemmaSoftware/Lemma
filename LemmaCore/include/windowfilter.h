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
#include "filter.h"

namespace Lemma {
    /** Types of filter window that are supported.
     */
    enum WINDOWTYPE { HAMMING, /*!< A hamming window */
                      HANNING, /*!< A hanning window */
                      RECTANGULAR /*!< Rectangular window */
                    };

    // ===================================================================
    //  Class:  WindowFilter
    /**
      @class   WindowFilter
      \brief   Fourier domain window filter.
      \details Current types are Hamming and Hanning. Others to be added.
     */
    // ===================================================================
    class WindowFilter : public Filter {

        public:

            // ====================  LIFECYCLE     =======================

            /**
             * Returns a pointer to a new object of type WindowFilter.
             * It allocates all necessary memory.
             */
            static WindowFilter* New();

            /**
             * @copybrief LemmaObject::Delete()
             * @copydetails LemmaObject::Delete()
             */
            void Delete();

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

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            WindowFilter (const std::string& name);

            /// Default protected constructor.
            ~WindowFilter ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

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

        private:

    }; // -----  end of class  WindowFilter  -----

}       // -----  end of Lemma  name  -----

#endif   // ----- #ifndef WINDOWFILTER_INC  -----
