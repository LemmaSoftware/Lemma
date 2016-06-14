/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/17/2010
  @version  $Id: receivercubes.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "receivercubes.h"

namespace Lemma{

	// ====================    FRIENDS     ======================
	std::ostream &operator<<(std::ostream &stream,
				const ReceiverCubes &ob) {
		stream << *(ReceiverPoints*)(&ob);
  		return stream;
	}

	// ====================  LIFECYCLE     ===================================

	ReceiverCubes * ReceiverCubes::New() {
		ReceiverCubes* Obj = new ReceiverCubes("ReceiverCubes");
        Obj->AttachTo(Obj);
        return Obj;
	}

	void ReceiverCubes::Delete() {
		this->DetachFrom(this);
	}

    void ReceiverCubes::Release() {
		delete this;
	}

	ReceiverCubes::ReceiverCubes(const std::string &name) :
			ReceiverPoints(name) {
	}

	ReceiverCubes::~ReceiverCubes() {
        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences( this );
    }

	// ====================  OPERATIONS    =======================

	void ReceiverCubes::SetNumberOfReceivers(const int &nrec){

        ReceiverPoints::SetNumberOfReceivers(nrec);

		this->Locations.resize(Eigen::NoChange, nrec);
		this->Locations.setZero();

		this->Lengths.resize(Eigen::NoChange, nrec);
		this->Lengths.setZero();

	}

	void ReceiverCubes::SetLengthX(const int &ir, const  Real& lx) {
		this->Lengths.col(ir)[0] = lx;
	}

	void ReceiverCubes::SetLengthY(const int &ir, const  Real& ly) {
		this->Lengths.col(ir)[1] = ly;
	}

	void ReceiverCubes::SetLengthZ(const int &ir, const  Real& lz) {
		this->Lengths.col(ir)[2] = lz;
	}

	void ReceiverCubes::SetLength(const int &ir, const  Vector3r& l) {
		this->Lengths.col(ir) = l;
	}

	void ReceiverCubes::SetLength(const int &ir,const  Real& lx,
					const Real& ly, const Real& lz) {
		this->Lengths.col(ir) << lx, ly, lz;
	}

	Real ReceiverCubes::GetLengthX(const int &ir) {
		return this->Lengths.col(ir)[0];
	}

	Real ReceiverCubes::GetLengthY(const int &ir) {
		return this->Lengths.col(ir)[1];
	}

	Real ReceiverCubes::GetLengthZ(const int &ir) {
		return this->Lengths.col(ir)[2];
	}

	Vector3r ReceiverCubes::GetLength(const int& ir) {
		return this->Lengths.col(ir);
	}

	Real ReceiverCubes::GetVolume(const int& ir) {
		return this->Lengths.col(ir)[0] *
			   this->Lengths.col(ir)[1] *
		       this->Lengths.col(ir)[2] ;
	}

	Real ReceiverCubes::GetVolumeSum() {
		Real Vol = 0.;
		for (int i=0; i<NumberOfReceivers; ++i) {
			Vol += GetVolume(i);
		}
		return Vol;
	}

}		// -----  end of Lemma  name  -----
