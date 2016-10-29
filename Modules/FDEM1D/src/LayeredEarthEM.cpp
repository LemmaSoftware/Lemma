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

#include "LayeredEarthEM.h"

namespace Lemma {

    std::ostream &operator << (std::ostream &stream, const LayeredEarthEM &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     ===================================

    LayeredEarthEM::LayeredEarthEM( const ctor_key& ) : LayeredEarth() {
    }

    LayeredEarthEM::LayeredEarthEM( const YAML::Node& node, const ctor_key& ) : LayeredEarth(node) {

        LayerConductivity = node["LayerConductivity"].as<VectorXcr>();

        LayerSusceptibility = node["LayerSusceptibility"].as<VectorXcr>();
        LayerLowFreqSusceptibility = node["LayerLowFreqSusceptibility"].as<VectorXr>();
        LayerHighFreqSusceptibility = node["LayerHighFreqSusceptibility"].as<VectorXr>();
        LayerTauSusceptibility = node["LayerTauSusceptibility"].as<VectorXr>();
        LayerBreathSusceptibility = node["LayerBreathSusceptibility"].as<VectorXr>();

        LayerPermitivity = node["LayerPermitivity"].as<VectorXcr>();
        LayerLowFreqPermitivity = node["LayerLowFreqPermitivity"].as<VectorXr>();
        LayerHighFreqPermitivity = node["LayerHighFreqPermitivity"].as<VectorXr>();
        LayerTauPermitivity = node["LayerTauPermitivity"].as<VectorXr>();
        LayerBreathPermitivity = node["LayerBreathPermitivity"].as<VectorXr>();

    }

    LayeredEarthEM::~LayeredEarthEM() {
    }

    std::shared_ptr<LayeredEarthEM> LayeredEarthEM::NewSP() {
        return std::make_shared<LayeredEarthEM> ( ctor_key() );
    }

    std::shared_ptr<LayeredEarthEM> LayeredEarthEM::DeSerialize( const YAML::Node& node ) {
        if (node.Tag() != "LayeredEarthEM") {
            throw  DeSerializeTypeMismatch( "LayeredEarthEM", node.Tag());
        }
        return std::make_shared<LayeredEarthEM> ( node, ctor_key() );
    }



    YAML::Node LayeredEarthEM::Serialize() const {
        YAML::Node node = LayeredEarth::Serialize();
        node.SetTag( GetName() );
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

    // ====================  OPERATIONS    ===================================
    void LayeredEarthEM::EvaluateColeColeModel(const Real& omega) {

        for (int ilay=0; ilay<GetNumberOfLayers(); ++ilay) {
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

	std::shared_ptr<LayeredEarthEM> LayeredEarthEM::Clone() {
		auto copy = LayeredEarthEM::NewSP();
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
		copy->SetNumberOfLayers( this->GetNumberOfLayers() );
		copy->NumberOfInterfaces = this->NumberOfInterfaces;
		copy->LayerThickness = this->LayerThickness;
		return copy;
	}

    // ====================  ACCESS        ==================================

    void LayeredEarthEM::SetLayerConductivity(const VectorXcr &sig) {
        if (sig.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity = sig;
    }

    void LayeredEarthEM::SetLayerConductivity(const int& ilay, const Complex &sig) {
        if (ilay > this->GetNumberOfLayers() || ilay < 1 )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity[ilay] = sig;
    }

/*  // TODO fix layer 0 problem, 1/0 --> infty, plus layer 0 is ignored anyway
    void LayeredEarthEM::SetLayerResistivity(const VectorXcr &rhos) {
        if (rhos.size() != this->NumberOfLayers )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerConductivity = 1./rhos.array();
    }
*/

    void LayeredEarthEM::SetLayerHighFreqSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerHighFreqSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerLowFreqSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerLowFreqSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerBreathSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerBreathSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerTauSusceptibility(const VectorXr &sus) {
        if (sus.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerTauSusceptibility = sus;
    }

    void LayeredEarthEM::SetLayerHighFreqPermitivity(const VectorXr &per) {
        if (per.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerHighFreqPermitivity = per;
    }

    void LayeredEarthEM::SetLayerLowFreqPermitivity(const VectorXr &per) {
        if (per.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerLowFreqPermitivity = per;
    }

    void LayeredEarthEM::SetLayerBreathPermitivity(const VectorXr &per) {
        if (per.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerBreathPermitivity = per;
    }

    void LayeredEarthEM::SetLayerTauPermitivity(const VectorXr &per) {
        if (per.size() != this->GetNumberOfLayers() )
            throw EarthModelParametersDoNotMatchNumberOfLayers( );
        LayerTauPermitivity = per;
    }

    void LayeredEarthEM::SetLayerThickness(const VectorXr &thick) {
        if (thick.size() != this->GetNumberOfLayers() - 2)
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
        this->NumberOfInterfaces = nlay-1;

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

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: */
