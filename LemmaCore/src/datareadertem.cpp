/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     12/28/2011
  @version  $Id: datareadertem.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "datareadertem.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const DataReaderTem &ob) {
		stream << *(DataReaderTem*)(&ob);
		return stream;
	}

	DataReaderTem::DataReaderTem(const std::string &name) :
		DataReader(name), InputData(NULL) {

	}

	DataReaderTem::~DataReaderTem() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	DataReaderTem* DataReaderTem::New() {
		DataReaderTem* Obj = new DataReaderTem("DataReaderTem");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void DataReaderTem::Delete() {
		this->DetachFrom(this);
	}

	void DataReaderTem::Release() {
		delete this;
	}

	DataTEM* DataReaderTem::GetData() {

		return this->InputData;
	}

	void DataReaderTem::SetDataTEM(DataTEM* inputtemp) {
		if (this->InputData != NULL) {
			this->InputData->DetachFrom(this);
		}
		inputtemp->AttachTo(this);
		this->InputData=inputtemp;
	}

	void DataReaderTem::ReadData(const std::string &datafile) {
		std::string temp;
		std::string temp2;
		int obs;
		int gates;
		MatrixXr datamatrix;
		Vector3Xr positions;

		std::fstream infile(datafile.c_str(),std::ios::in);
		if (infile.fail()) {
			std::cout << "Data file I/O error." << std::endl;
		/// \TODO thow some kind of stoppage error
		}

		// file format will be:
		// nObs nGates
		// x1 y1 d1 d2 d3 etc

		infile >> obs;
		infile >> gates;
		this->InputData->SetSize(obs,gates);

		//set dimensions and finish reading in data
		datamatrix.resize(obs,gates);
		positions.resize(Eigen::NoChange,obs);

		for (int ii=0;ii<obs;ii++) {
			infile >> positions(0,ii);
			infile >> positions(1,ii);
			infile >> positions(2,ii);
			for (int jj=0;jj<gates;jj++) {
				infile >> datamatrix(ii,jj);
			}
		}

		this->InputData->SetData(datamatrix);
		this->InputData->SetPositions(positions);

		infile.close();

	}


} // end namespace Lemma
