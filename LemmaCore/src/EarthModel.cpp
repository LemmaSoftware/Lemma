/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/23/2010
  @version  $Id: earthmodel.cpp 210 2015-02-25 02:57:03Z tirons $
 **/

#include "EarthModel.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
				const EarthModel &ob) {

		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	// ====================  LIFECYCLE     =======================

	EarthModel::EarthModel(const std::string&name) :
			LemmaObject(name), BField(0,0,0), BFieldUnit(0,0,0), BInc(0), BDec(0), BMag(0) {
	}

    #ifdef HAVE_YAMLCPP
    EarthModel::EarthModel(const YAML::Node& node) : LemmaObject(node) {
        BInc = node["BInc"].as<double>();
        BDec = node["BDec"].as<double>();
        BMag = node["BMag"].as<double>();
        BField = node["BField"].as<Vector3r>();
    }


    YAML::Node EarthModel::Serialize() const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( this->GetName() );
        node["BField"] = BField;
        node["BInc"] = BInc;
        node["BDec"] = BDec;
        node["BMag"] = BMag;
        return node;
    }
    #endif

	EarthModel::~EarthModel() {
	}

 	// ====================  ACCESS        ===================================
	void EarthModel::SetMagneticFieldComponents(const Vector3r &bfield,
					const MAGUNITS &unit) {

 		this->BField = bfield;

		switch (unit) {
			case TESLA:
				break;
			case NANOTESLA:
				BField *= 1e-9;
				break;
		  	case GAUSS:
				BField *= 1e-4;
				break;
			default:
				throw "MAGUNITS UNDEFINED\n";
		}

		BMag = bfield.norm(  );
	    BInc = std::acos (bfield.dot(Vector3r(0,0,1)) / BMag) ;
	    BDec = std::acos (bfield.dot(Vector3r(1,0,0)) / BMag) ;
		BFieldUnit = BField.array() / BMag;
 	}

	void EarthModel::SetMagneticFieldIncDecMag(const Real &inc,
			const Real &dec, const Real &mag, const MAGUNITS &unit) {
		BMag = mag;
		BInc = inc;
		BDec = dec;

		switch (unit) {
			case TESLA:
				break;
			case NANOTESLA:
				BMag *= 1e-9;
				break;
		  	case GAUSS:
				BMag *= 1e-4;
				break;
			default:
				throw "MAGUNITS UNDEFINED\n";
		}

		BField(0) = BMag * cos(BInc*(PI/180.)) * cos(BDec*(PI/180.));
		BField(1) = BMag * cos(BInc*(PI/180.)) * sin(BDec*(PI/180.));
		BField(2) = BMag * sin(BInc*(PI/180.))  ;
		BFieldUnit = BField.array() / BMag;
	}

	// ====================  INQUIRY       =======================

	Vector3r EarthModel::GetMagneticField( ) {
		return this->BField;
	}

    Vector3r EarthModel::GetMagneticFieldInGauss( ) {
		return this->BField*1e4;
	}

	Vector3r EarthModel::GetMagneticFieldUnitVector() {
		return this->BFieldUnit;
	}

	Real  EarthModel::GetMagneticFieldMagnitude() {
		return this->BMag;
	}

	Real  EarthModel::GetMagneticFieldMagnitudeInGauss() {
		return this->BMag*1e4;
	}

    ////////////////////////////////////////////////////////
    // Error Classes

    NullEarth::NullEarth() :
        runtime_error( "NULL VALUED LAYERED EARTH MODEL") {}


}		// -----  end of Lemma  name  -----
