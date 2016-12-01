/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/24/2009
  @version  $Id: layeredearth.cpp 210 2015-02-25 02:57:03Z tirons $
 **/

#include "LayeredEarth.h"

namespace Lemma {

	// ====================    FRIENDS     ======================

    std::ostream &operator << (std::ostream &stream, const LayeredEarth &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc
        return stream;
    }

	// ====================  LIFECYCLE     ===================================

	LayeredEarth::LayeredEarth( ) : EarthModel( ),
		NumberOfLayers(0), 	NumberOfInterfaces(0) {
	}

	LayeredEarth::~LayeredEarth() {
	}

    LayeredEarth::LayeredEarth(const YAML::Node& node) : EarthModel(node) {
        std::cout << "LayeredEarth(YAML::Node )" << std::endl;
        NumberOfLayers = node["NumberOfLayers"].as<int>();
        NumberOfInterfaces = node["NumberOfInterfaces"].as<int>();
        LayerThickness = node["LayerThickness"].as<VectorXr>();
	}

    YAML::Node LayeredEarth::Serialize() const {
        YAML::Node node = EarthModel::Serialize();
        node["NumberOfLayers"] = NumberOfLayers;
        node["NumberOfInterfaces"] = NumberOfInterfaces;
        node["LayerThickness"] = LayerThickness;
        node.SetTag( this->GetName() );
        return node;
    }

	// ====================  OPERATIONS    ===================================

	// ====================  ACCESS        ===================================

    //--------------------------------------------------------------------------------------
    //       Class:  LayeredEarth
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string LayeredEarth::GetName (  ) const {
        return CName;
    }		// -----  end of method LayeredEarth::get_GetName  -----

	void LayeredEarth::SetLayerThickness(const VectorXr &thick) {
		if (thick.size() != this->NumberOfLayers - 2)
			throw EarthModelParametersDoNotMatchNumberOfLayers( );
		LayerThickness = thick;
	}

	// ====================  INQUIRY       ===================================

	int LayeredEarth::GetNumberOfLayers () {
		return this->NumberOfLayers;
	}

	int LayeredEarth::GetNumberOfNonAirLayers () {
		return this->NumberOfLayers - 1;
	}

    VectorXr LayeredEarth::GetLayerThickness( ) {
        return LayerThickness;
    }

	Real LayeredEarth::GetLayerThickness(const int &ilay) {
		// Take into account infinite top and bottom layers
		// estimate infinity by 1000 m
		if (ilay < 0 || ilay > NumberOfLayers - 1) {
			throw RequestForNonValidEarthModelParameter( );
		} else if (ilay == 0) {
			return 1000.;
		} else if (ilay == NumberOfLayers - 1) {
			return 1000.;
		} else {
			return this->LayerThickness(ilay-1);
		}
	}

	Real LayeredEarth::GetLayerDepth(const int &ilay) {
		Real depth = 0;
		if (ilay == 0) {
			return depth;
		} else {
			for (int i=1; i<=ilay; ++i) {
				depth += GetLayerThickness(i);
			}
		}
		return depth;
	}

	int LayeredEarth::GetLayerAtThisDepth(const Real& depth) {
		if (depth <= 0 || NumberOfLayers < 2) {
			return 0;
		}
		Real laydep = 0;
		for (int ilay=0; ilay<NumberOfLayers-2; ++ilay) {
			laydep += LayerThickness[ilay];
			if (laydep >= depth) { return ilay+1; }
		}
		return NumberOfLayers-1;
	}

    EarthModelWithLessThanTwoLayers::EarthModelWithLessThanTwoLayers() :
        runtime_error( "EARTH MODEL WITH LESS THAN TWO LAYERS") { }

    EarthModelWithMoreThanMaxLayers::EarthModelWithMoreThanMaxLayers() :
        runtime_error( "EARTH MODEL WITH MORE THAN MAX LAYERS") { }

    EarthModelParametersDoNotMatchNumberOfLayers::
        EarthModelParametersDoNotMatchNumberOfLayers( ) :
        runtime_error( "EARTH MODEL PARAMETERS DO NOT MATCH NUMBER OF LAYERS")
        {}

    RequestForNonValidEarthModelParameter::
        RequestForNonValidEarthModelParameter() :
        runtime_error( "REQUEST FOR NON VALID EARTH MODEL PARAMETER") {}
}
