/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/20/2010
 **/

#include "WindowFilter.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator << (std::ostream &stream, const WindowFilter &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  WindowFilter
    // Description:  constructor (locked with ctor_key)
    //--------------------------------------------------------------------------------------
    WindowFilter::WindowFilter( const ctor_key& key ) : Filter( key ),
        Width(0), SamplingRate(0), Bandwidth(0), Coefficients(), Type(RECTANGULAR)
    {
    }   // -----  end of method WindowFilter::WindowFilter  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  WindowFilter
    // Description:  DeSerializing constructor (locked with ctor_key)
    //--------------------------------------------------------------------------------------
    WindowFilter::WindowFilter( const YAML::Node& node, const ctor_key& key) : Filter( node, key ),
        Width(0), SamplingRate(0), Bandwidth(0), Coefficients(), Type(RECTANGULAR)
    {
        Width = node["Width"].as<int>( );
        Nt = node["Nt"].as<int>( );
        Nw = node["Nw"].as<int>( );
        SamplingRate = node["SamplingRate"].as<Real>();
        Bandwidth = node["Bandwidth"].as<Real>();
        Coefficients = node["Coefficients"].as<VectorXr>();
        Type = string2Enum<WINDOWTYPE>( node["WINDOWTYPE"].as<std::string>() );
    } // -----  end of method WindowFilter::WindowFilter  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  NewSP()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr< WindowFilter > WindowFilter::NewSP() {
        return std::make_shared< WindowFilter >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  ~WindowFilter
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    WindowFilter::~WindowFilter() {
    }

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  WindowFilter::Serialize (  ) const {
        YAML::Node node = Filter::Serialize();;
        node.SetTag( GetName() );
        // FILL IN CLASS SPECIFICS HERE
        node["Width"] = Width;
        node["Nt"] = Nt;
        node["Nw"] = Nw;
        node["SamplingRate"] = SamplingRate;
        node["Bandwidth"] = Bandwidth;
        node["Coefficients"] = Coefficients;
        node["WINDOWTYPE"] = enum2String(Type);
        return node;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<WindowFilter> WindowFilter::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() != "WindowFilter") {
            throw  DeSerializeTypeMismatch( "WindowFilter", node.Tag());
        }
        return std::make_shared<WindowFilter>( node, ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  WindowFilter
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string WindowFilter::GetName (  ) const {
        return CName;
    }		// -----  end of method WindowFilter::GetName  -----

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
        Width         = (int)(std::round(Bandwidth/df));
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
