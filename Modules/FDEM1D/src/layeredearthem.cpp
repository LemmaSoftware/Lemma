/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/24/2009
  @version  $Id: layeredearthem.cpp 216 2015-03-09 02:26:49Z tirons $
 **/

#include "layeredearthem.h"

namespace Lemma {

#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const LayeredEarthEM &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream,
                const LayeredEarthEM &ob) {
        stream << *(LayeredEarth*)(&ob);
        //stream << "Class Name : "<< ob.Name  << "\n";
        stream << std::setw(15) << "LayerNumber"         << "\t"
               << std::setw(15) << "Thickness"           << "\t"
               << std::setw(15) << "BottomInterface"     << "\t"
               << std::setw(15) << "Conductivity"        << "\t"
               << std::setw(15) << "Susceptibility"      << "\t"
               << std::setw(15) << "Permitivity"         << "\n";
        stream << std::setw(15) << " "                   << "\t"
               << std::setw(15) << "[m]"                 << "\t"
               << std::setw(15) << "[m]"                 << "\t"
               << std::setw(15) << "[Sm^-1]"             << "\t"
               << std::setw(15) << "[]"                  << "\t"
               << std::setw(15) << "[F/m]"               << "\n";
        for (int i=0; i< ob.NumberOfLayers; ++i) {
            stream << std::setw(15) <<  i << "\t"
                   << std::setw(15) << ((LayeredEarth*)(&ob))->GetLayerThickness(i) << "\t"
                   << std::setw(15) << ((LayeredEarth*)(&ob))->GetLayerDepth(i) << "\t"
                   << std::setw(15) << ob.LayerConductivity(i)     << "\t"
                   << std::setw(15) << ob.LayerSusceptibility(i)   << "\t"
                   << std::setw(15) << ob.LayerPermitivity(i)      << "\n";
        }

        return stream;
    }
#endif

    // ====================  LIFECYCLE     ===================================

    LayeredEarthEM::LayeredEarthEM(const std::string &name) :
        LayeredEarth(name) {
    }

    LayeredEarthEM::~LayeredEarthEM() {
        if (NumberOfReferences != 0) {
            throw DeleteObjectWithReferences(this);
        }
    }

    LayeredEarthEM* LayeredEarthEM::New() {
        LayeredEarthEM* Obj = new LayeredEarthEM("LayeredEarthEM");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void LayeredEarthEM::Delete() {
        this->DetachFrom(this);
    }

    void LayeredEarthEM::Release() {
        delete this;
    }

    #ifdef HAVE_YAMLCPP
    YAML::Node LayeredEarthEM::Serialize() const {
        YAML::Node node = LayeredEarth::Serialize();
        node.SetTag( this->Name );
        node["LayerConductivity"] = LayerConductivity;
        node["LayerSusceptibility"] = LayerSusceptibility;
        node["LayerLowFreqSusceptibility"] = LayerLowFreqSusceptibility;
        node["LayerHighFreqSusceptibility"] = LayerHighFreqSusceptibility;
        node["LayerTauSusceptibility"] = LayerTauSusceptibility;
        node["LayerBreathSusceptibility"] = LayerBreathSusceptibility;
        node["LayerPermitivity"] = LayerPermitivity;
        node["LayerLowFreqPermitivity"] = LayerLowFreqPermitivity;
        node["LayerHighFreqPermitivity"] = LayerHighFreqPermitivity;
        node["LayerTauPermitivity"] = LayerTauPermitivity;
        node["LayerBreathPermitivity"] = LayerBreathPermitivity;
        return node;
    }
    #endif

    // ====================  OPERATIONS    ===================================
    void LayeredEarthEM::EvaluateColeColeModel(const Real& omega) {

        for (int ilay=0; ilay<NumberOfLayers; ++ilay) {
            if ( LayerTauSusceptibility(ilay) > 1e-10) {
                LayerSusceptibility(ilay) = LayerHighFreqSusceptibility(ilay) + (LayerLowFreqSusceptibility(ilay) -
                     LayerHighFreqSusceptibility(ilay)) /
                    ((Real)(1.) + std::pow(Complex(0, omega*
                                    LayerTauSusceptibility(ilay)),
                             LayerBreathSusceptibility(ilay)));
            } else {
                LayerSusceptibility(ilay) = LayerLowFreqSusceptibility(ilay);
            }

            if ( LayerTauPermitivity(ilay) > 1e-10) {
                LayerPermitivity(ilay) = LayerHighFreqPermitivity(ilay) +
                    (LayerLowFreqPermitivity(ilay) -
                     LayerHighFreqPermitivity(ilay)) /
                    ((Real)(1.) + std::pow(Complex(0,
                                    omega*LayerTauPermitivity(ilay)),
                            LayerBreathPermitivity(ilay)));
            } else {
                LayerPermitivity(ilay) = LayerLowFreqPermitivity(ilay);
            }
        }
    }

	LayeredEarthEM* LayeredEarthEM::Clone() {
		LayeredEarthEM* copy = LayeredEarthEM::New();

		copy->LayerConductivity = this->LayerConductivity;
		copy->LayerSusceptibility = this->LayerSusceptibility;
		copy->LayerLowFreqSusceptibility = this->LayerLowFreqSusceptibility;
		copy->LayerHighFreqSusceptibility = this->LayerHighFreqSusceptibility;
		copy->LayerTauSusceptibility = this->LayerTauSusceptibility;
		copy->LayerBreathSusceptibility = this->LayerBreathSusceptibility;
		copy->LayerPermitivity = this->LayerPermitivity;
		copy->LayerLowFreqPermitivity = this->LayerLowFreqPermitivity;
		copy->LayerHighFreqPermitivity = this->LayerHighFreqPermitivity;
		copy->LayerTauPermitivity = this->LayerTauPermitivity;
		copy->LayerBreathPermitivity = this->LayerBreathPermitivity;
		copy->NumberOfLayers = this->NumberOfLayers;
		copy->NumberOfInterfaces = this->NumberOfInterfaces;
		copy->LayerThickness = this->LayerThickness;

		return copy;

	}

    // ====================  ACCESS        ==================================

    void LayeredEarthEM::SetLayerConductivity(const VectorXcr &sig) {
        if (sig.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity = sig;
    }

    void LayeredEarthEM::SetLayerConductivity(const int& ilay, const Complex &sig) {
        if (ilay > this->NumberOfLayers || ilay < 1 )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity[ilay] = sig;
    }

/*
    void LayeredEarthEM::SetLayerResistivity(const VectorXcr &rhos) {
        if (rhos.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity = 1./rhos.array();
    }
*/

    void LayeredEarthEM::SetLayerHighFreqSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerHighFreqSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerLowFreqSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerLowFreqSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerBreathSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerBreathSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerTauSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerTauSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerHighFreqPermitivity(const VectorXr &per) {
        if (per.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerHighFreqPermitivity = per;
    }

    void LayeredEarthEM::SetLayerLowFreqPermitivity(const VectorXr &per) {
        if (per.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerLowFreqPermitivity = per;
    }

    void LayeredEarthEM::SetLayerBreathPermitivity(const VectorXr &per) {
        if (per.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerBreathPermitivity = per;
    }

    void LayeredEarthEM::SetLayerTauPermitivity(const VectorXr &per) {
        if (per.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerTauPermitivity = per;
    }

    void LayeredEarthEM::SetLayerThickness(const VectorXr &thick) {
        if (thick.size() != this->NumberOfLayers - 2)
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerThickness = thick;
    }

    void LayeredEarthEM::SetNumberOfLayers(const int&nlay) {

        // Check for validity
        if (nlay < 2) {
            throw EarthModelWithLessThanTwoLayers();
        }
        //else if (nlay > MAXLAYERS)
        //    throw EarthModelWithMoreThanMaxLayers();

        // Otherwise
        this->NumberOfLayers = nlay;


        // Resize all layers

        //////////////////////////////////
        // Thicknesses set to zero
        LayerThickness = VectorXr::Zero(NumberOfLayers);


        ///////////////////////////////////
        // Conducitivy set to zero
        LayerConductivity = VectorXcr::Zero(NumberOfLayers);


        ////////////////////////////////////
        // Susceptibility set to One (free space)
        LayerSusceptibility = VectorXcr::Ones(NumberOfLayers);

        // workaround for Valgrind on AMD which compains about assignment to 1 of VectorXr
        LayerLowFreqSusceptibility = LayerSusceptibility.real();  // 1
        LayerHighFreqSusceptibility = LayerSusceptibility.real(); // 1
        LayerTauSusceptibility = LayerSusceptibility.imag();      // 0
        LayerBreathSusceptibility = LayerSusceptibility.imag();   // 0

        //////////////////////////////////////
        // Permitivity set to 1 (free space)
        //this->LayerPermitivity.resize(NumberOfLayers);
        //this->LayerPermitivity.setOnes();
        LayerPermitivity = VectorXcr::Ones(NumberOfLayers);

        // workaround for Valgrind on AMD which compains about assignment to 1 of VectorXr
        LayerLowFreqPermitivity = LayerPermitivity.real();      // 1
        LayerHighFreqPermitivity = LayerPermitivity.real();     // 1
        LayerTauPermitivity = LayerPermitivity.imag();          // 0
        LayerBreathPermitivity = LayerPermitivity.imag();       // 0

    }

    // ====================  INQUIRY       ===================================

    VectorXcr LayeredEarthEM::GetLayerConductivity() {
        return this->LayerConductivity;
    }

    Complex LayeredEarthEM::GetLayerConductivity(const int &ilay) {
        return this->LayerConductivity(ilay);
    }

    Complex LayeredEarthEM::GetLayerSusceptibility(const int &ilay) {
        return this->LayerSusceptibility(ilay);
    }

    VectorXcr LayeredEarthEM::GetLayerSusceptibility( ) {
        return this->LayerSusceptibility;
    }

    Complex LayeredEarthEM::GetLayerPermitivity(const int &ilay) {
        return this->LayerPermitivity(ilay);
    }

    VectorXcr LayeredEarthEM::GetLayerPermitivity( ) {
        return this->LayerPermitivity;
    }

    Real LayeredEarthEM::GetLayerLowFreqSusceptibility(const int &ilay) {
        return this->LayerLowFreqSusceptibility(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerLowFreqSusceptibility( ) {
        return this->LayerLowFreqSusceptibility;
    }

    Real LayeredEarthEM::GetLayerHighFreqSusceptibility(const int &ilay) {
        return this->LayerHighFreqSusceptibility(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerHighFreqSusceptibility( ) {
        return this->LayerHighFreqSusceptibility;
    }

    Real LayeredEarthEM::GetLayerTauSusceptibility(const int &ilay) {
        return this->LayerTauSusceptibility(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerTauSusceptibility( ) {
        return this->LayerTauSusceptibility;
    }

    Real LayeredEarthEM::GetLayerBreathSusceptibility(const int &ilay) {
        return this->LayerBreathSusceptibility(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerBreathSusceptibility( ) {
        return this->LayerBreathSusceptibility;
    }

    Real LayeredEarthEM::GetLayerLowFreqPermitivity(const int &ilay) {
        return this->LayerLowFreqPermitivity(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerLowFreqPermitivity( ) {
        return this->LayerLowFreqPermitivity;
    }

    Real LayeredEarthEM::GetLayerHighFreqPermitivity(const int &ilay) {
        return this->LayerHighFreqPermitivity(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerHighFreqPermitivity( ) {
        return this->LayerHighFreqPermitivity;
    }

    Real LayeredEarthEM::GetLayerTauPermitivity(const int &ilay) {
        return this->LayerTauPermitivity(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerTauPermitivity( ) {
        return this->LayerTauPermitivity;
    }

    Real LayeredEarthEM::GetLayerBreathPermitivity(const int &ilay) {
        return this->LayerBreathPermitivity(ilay);
    }

    VectorXr LayeredEarthEM::GetLayerBreathPermitivity( ) {
        return this->LayerBreathPermitivity;
    }
}
