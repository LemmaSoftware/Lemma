/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/14/2013
  @version  $Id$
 **/

#include "instrumentfem.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const InstrumentFem &ob) {
		stream << *(LemmaObject*)(&ob);
		return stream;
	}

	InstrumentFem::InstrumentFem(const std::string &name) : Instrument (name),
		EarthModel(NULL), Dipole(NULL), Receiver(NULL), InputData(NULL),
	    OutputData(NULL)	{
		}

	InstrumentFem::~InstrumentFem() {
		if (NumberOfReferences !=0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	InstrumentFem* InstrumentFem::New() {
		InstrumentFem* Obj = new InstrumentFem("InstrumentFem");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void InstrumentFem::Delete() {
		this->DetachFrom(this);
	}

	void InstrumentFem::Release() {
		delete this;
	}

	void InstrumentFem::MakeCalculation() {

		EMEarth1D *EMEarthLocal=EMEarth1D::New();
		EMEarthLocal->AttachLayeredEarthEM(this->EarthModel);
		EMEarthLocal->AttachDipoleSource(this->Dipole);
		EMEarthLocal->SetFieldsToCalculate(H);
		//EMEarthLocal->SetHankelTransformMethod(DIGITALFILTERING);
		EMEarthLocal->SetHankelTransformMethod(CHAVE);
		EMEarthLocal->AttachReceiverPoints(this->Receiver);
		EMEarthLocal->MakeCalc3();
		//EMEarthLocal->MakeCalc();

		EMEarthLocal->Delete();

		// GetHfield(freq_idx, loc_idx)(comp)

	}

	void InstrumentFem::ForwardModelProfile() {


	}

	void InstrumentFem::EMEarthModel(LayeredEarthEM* Earth) {
		if (this->EarthModel != NULL) {
			this->EarthModel->DetachFrom(this);
		}
		Earth->AttachTo(this);
		this->EarthModel = Earth;
	}

	void InstrumentFem::SetDipoleSource(DipoleSource* dipolesource) {
		if (this->Dipole != NULL) {
			this->Dipole->DetachFrom(this);
		}
		dipolesource->AttachTo(this);
		this->Dipole = dipolesource;
	}

	void InstrumentFem::SetReceiverPoints(ReceiverPoints* receiver) {
		if (this->Receiver != NULL) {
			this->Receiver->DetachFrom(this);
		}
		receiver->AttachTo(this);
		this->Receiver = receiver;
	}

	void InstrumentFem::AlignWithData(DataFEM* inpdata) {
		this->nFreq = inpdata->GetnFreq();
		this->nObs = inpdata->GetnObs();
		this->freq = inpdata->GetFreqs();
		this->xyz = inpdata->GetXYZ();
		this->TxOrientation = inpdata->GetTxOrien();
		this->RxOrientation = inpdata->GetRxOrien();
		this->TxMom = inpdata->GetTxMom();
		this->TxRxSep = inpdata->GetTxRxSep();
		this->ScaleFac = inpdata->GetScaleFac();

		// Check into boost for the use of pointer arrays
		// array of nFreq DipoleSource objects
			//first get rid of existing dipoles
		for (unsigned int id=0;id<this->Transmitters.size();++id) {
			this->Transmitters[id]->Delete();
		}
		this->Transmitters.clear();
		//this->Transmitters.resize(this->nFreq);

		for (unsigned int id=0;id<this->Receivers.size();++id) {
			this->Receivers[id]->Delete();
		}
		this->Receivers.clear();


		for (int ii=0;ii<this->nFreq;ii++) {
			/*
			this->Transmitters.push_back(DipoleSource::New());
			std::cout << &(this->Transmitters[ii]) << std::endl;
			*/
			DipoleSource *tt = DipoleSource::New();
			ReceiverPoints *tr = ReceiverPoints::New();
			Real ftemp;
			Real momtemp;
			ftemp = this->freq(ii);
			momtemp = this->TxMom(ii);
			tt->SetType(MAGNETICDIPOLE);
			tt->SetLocation(0,0,-1.e-3);
			// This is really hoopty but will do for now
			if (this->TxOrientation(ii) == X) {
				tt->SetPolarisation(XPOLARISATION);
			}
			else if (this->TxOrientation(ii) == Y) {
				tt->SetPolarisation(YPOLARISATION);
			}
			else if (this->TxOrientation(ii) == Z) {
				tt->SetPolarisation(ZPOLARISATION);
			}
			else {
				std::cout << "DANGER! DANGER! UNHELPFUL ERROR"
					 << std::endl;
				tt->SetPolarisation(ZPOLARISATION);
			}
			tt->SetNumberOfFrequencies(1);
			tt->SetFrequency(0,ftemp);
			tt->SetMoment(momtemp);
			this->Transmitters.push_back(tt);
			tr->SetNumberOfReceivers(1);
			tr->SetLocation(0,this->TxRxSep.block(0,ii,3,1));
			this->Receivers.push_back(tr);
		}

		// Now populate the attached output data object?
		// Or, should I do this later after the computation?

	}

	void InstrumentFem::SetOutputData(DataFEM* outputdata) {
		if (this->OutputData != NULL) {
			this->OutputData->DetachFrom(this);
		}
		outputdata->AttachTo(this);
		this->OutputData = outputdata;
	}



}// end of namespace lemma
