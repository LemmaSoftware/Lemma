/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/10/2011
  @version  $Id: temintegrationkernel.cpp 266 2015-04-01 03:24:00Z tirons $
 **/

#include "temintegrationkernel.h"

namespace Lemma {

	//std::ostream &operator<<(std::ostream &stream,
	//	const TemIntegrationKernel &ob) {
	//stream << *(IntegrationKernel<Real>*)(&ob);
	//return stream;
	//}

	// =======================  Lifecycle  ==================================

	TemIntegrationKernel::TemIntegrationKernel(const std::string &name) :
		IntegrationKernel<Real>(name), component(ZCOMPONENT), EmEarthInt(NULL), Trans(NULL),
		DipoleS(NULL), Receivers(NULL) {

	}

	TemIntegrationKernel::~TemIntegrationKernel() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	TemIntegrationKernel* TemIntegrationKernel::New() {
		TemIntegrationKernel* Obj = new TemIntegrationKernel("TemIntegrationKernel");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void TemIntegrationKernel::Delete() {
		this->DetachFrom(this);
        if ( EmEarthInt != NULL ) {
            EmEarthInt->DetachFrom(this);
        }
        if (Trans != NULL) {
            Trans->DetachFrom(this);
        }
        if (DipoleS != NULL) {
            DipoleS->DetachFrom(this);
        }
        if (Receivers != NULL) {
            Receivers->DetachFrom(this);
        }
	}

	void TemIntegrationKernel::Release() {
		delete this;
	}

	Real TemIntegrationKernel::Argument(const Real& w, const int& iRelated) {
		// Finally, the meat of things!
		// You can't beat the meat.
		// Use this to define the Argument of the cosine transform, sans
		// cosine.
        //std::cout << "In Argument" << std::endl;
		Real nu =  w / (2.*PI);

        Receivers->ClearFields();
		if (this->Trans != NULL && this->DipoleS == NULL) {
			this->Trans->SetFrequency(0, nu);
			this->EmEarthInt->CalculateWireAntennaFields();
		} else if (this->DipoleS != NULL && this->Trans==NULL) {
			this->DipoleS->SetFrequency(0, nu);
			this->EmEarthInt->MakeCalc3();
		}

        /*
        if ( std::abs(std::imag(Receivers->GetHfield(0)(component))) <= 0.) {
            std::cout << "DOOM! " << Receivers->GetHfield(0).transpose() << "\t" << nu <<  std::endl;
            std::cout << "this " << this << std::endl;
            std::cout << "trans: " << this->Trans << std::endl;
            std::cout << "EmEarthInt: " << this->EmEarthInt << std::endl;
            std::cout << "TEMIntegrationKernel.cpp" << std::endl;
            EmEarthInt->Query();
            exit(EXIT_FAILURE);
            //return Argument(w, iRelated);
        }
        */

		// TODO return a MatrixXr instead so we can have multiple receivers/components
		return std::imag(Receivers->GetHfield(0)(component)) / w;
		//answer = std::imag(temp);         // dH/dt step response,    H impulse response
		//answer = std::imag(temp) / ( w ); // dH/dt impulse response, H step response   (TI)
	}

	void TemIntegrationKernel::SetEMEarth1D(EMEarth1D* earth) {
		if (this->EmEarthInt != NULL) {
			this->EmEarthInt->DetachFrom(this);
		}
		earth->AttachTo(this);
		this->EmEarthInt=earth;

	}

	void TemIntegrationKernel::SetTransmitter(WireAntenna *antennae) {
		if (this->Trans != NULL) {
			this->Trans->DetachFrom(this);
		}
		antennae->AttachTo(this);
		this->Trans=antennae;
		if (this->DipoleS != NULL) {
			this->DipoleS->DetachFrom(this);
			DipoleS = NULL;
		}

	}

	void TemIntegrationKernel::SetDipole(DipoleSource* dipolesource) {
		if (this->DipoleS != NULL) {
			this->DipoleS->DetachFrom(this);
		}
		dipolesource->AttachTo(this);
		this->DipoleS = dipolesource;
		if (this->Trans != NULL) {
			this->Trans->DetachFrom(this);
            Trans = NULL;
		}

	}

	void TemIntegrationKernel::SetReceiver(ReceiverPoints* receiver) {
		if (this->Receivers != NULL) {
			this->Receivers->DetachFrom(this);
		}
		receiver->AttachTo(this);
		this->Receivers=receiver;

	}

	int TemIntegrationKernel::GetNumRel() {
		//int temp;
		return 1;
	}


    void TemIntegrationKernel::SetComponent(const FIELDCOMPONENT& comp) {
        component = comp;
    }


} // end namespace Lemma

