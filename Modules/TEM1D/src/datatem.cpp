/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     03/23/2010
  @version  $Id: datatem.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "datatem.h"

namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
        const DataTEM &ob) {
        stream << *(Data*)(&ob);
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    void DataTEM::Delete() {
        this->DetachFrom(this);
    }

    void DataTEM::Release() {
        delete this;
    }

    DataTEM * DataTEM::New() {
        DataTEM* Obj = new DataTEM("DataTEM");
        Obj->AttachTo(Obj);
        return Obj;
    }

    DataTEM * DataTEM::Clone() {
        DataTEM* Obj = new DataTEM("DataTEM");
        Obj->AttachTo(Obj);
        // TODO Copy
        return Obj;
    }

    DataTEM::DataTEM(const std::string &name) : Data(name) {
    }

    DataTEM::~DataTEM() {
        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences( this );
    }

    // ====================  OPERATIONS    =======================
    void DataTEM::Zero() {
    }

    Real DataTEM::Norm(Data* Data2) {
        return 0;
    }

	void DataTEM::SetSize(const int &nObs,const int &nGates) {
		this->nObs = nObs;
		this->nGates = nGates;

	}

	void DataTEM::SetData(const MatrixXr &inputdata) {
		this->TEMDataCube = inputdata;
	}

	void DataTEM::SetPositions(const Vector3Xr &positions) {
		this->locations = positions;
	}

	int DataTEM::GetnGates() {
		return this->nGates;
	}

	int DataTEM::GetnObs() {
		return this->nObs;
	}

	MatrixXr DataTEM::GetData() {
		return this->TEMDataCube;
	}

	Real DataTEM::GetDataCoeff(const int &x, const int &y) {
		return this->TEMDataCube.coeff(x,y);
	}

	VectorXr DataTEM::GetTimes() {
		return this->times;
	}

	Vector3Xr DataTEM::GetPositions() {
		return this->locations;
	}



}       // -----  end of Lemma  name  -----
