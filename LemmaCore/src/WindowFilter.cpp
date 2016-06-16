/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/20/2010
  @version  $Id: windowfilter.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "WindowFilter.h"

namespace Lemma {

    // ====================  LIFECYCLE     =======================

    WindowFilter::WindowFilter( ) : Filter( ),
        Width(0), SamplingRate(0), Bandwidth(0), Coefficients(),
        Type(RECTANGULAR) {}

    std::shared_ptr< WindowFilter > WindowFilter::NewSP() {
        std::shared_ptr<WindowFilter> sp(new  WindowFilter( ), LemmaObjectDeleter() );
        return sp;
    }

    WindowFilter::~WindowFilter() {
    }


    // ====================  OPERATIONS    =======================

    void WindowFilter::SetWindowType(const WINDOWTYPE &type) {
        Type = type;
    }

    void WindowFilter::SetBandwidth(const Real& width) {
        Bandwidth = width;
    }

    void WindowFilter::SetSamplingInterval(const Real& rate) {
        SamplingRate = rate;
    }

    int WindowFilter::GetNumberOfFrequencyBins() {
        return this->Nw;
    }

    void WindowFilter::SetNumberOfSamples(const int& nt) {
        Nt = nt;
        Nw = Nt%2 ? (Nt-1)/2+1 : (Nt)/2+1;  // odd , even
    }

    VectorXr WindowFilter::GetFilterCoefficients() {
        Real  Nyquist = .5/SamplingRate;
        std::cout << "Window nyquist " << Nyquist << std::endl;
        std::cout << "Window Nw  " << Nw << std::endl;
        Real  df      = (Nyquist)/((Real)(Nw)); // df Hz per bin
        Width         = Bandwidth/df;
        Coefficients.resize(Width);
        switch (Type) {
            case HAMMING:
           	    for (int n=0; n<Width; ++n) {
		            Coefficients(n) = 0.5 * (1.- std::cos( (2.*PI*(Real)(n)) /
                                       ((Real)(Width)-1.) ) );
	            }
                break;
            case HANNING:
	            for (int n=0; n<Width; ++n) {
		            Coefficients(n) = 0.54 - 0.46 *
                                std::cos( (2.*PI*(Real)(n)) /
                                    ((Real)(Width)-1.) ) ;
	            }
                break;
            case RECTANGULAR:
                Coefficients.setOnes();
                break;
        }
        return Coefficients;
    }
}		// -----  end of Lemma  name  -----
