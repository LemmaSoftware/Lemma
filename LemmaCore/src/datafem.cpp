/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     01/03/2013
  @version  $Id$
 **/

#include "datafem.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const DataFEM &ob) {
		stream << *(Data*)(&ob);
		stream << "Number of Soundings: " << ob.nObs << std::endl;
		stream << "Number of Frequencies: " << ob.nFreq << std::endl;
		stream << std::setw(10) << "Number" << "\t"
				<< std::setw(10) << "Frequency" << "\t"
				<< std::setw(10) << "Tx Moment" << "\t"
				<< std::setw(10) << "Tx Coil O." << "\t"
				<< std::setw(10) << "Rx Coil O." << "\n";
		stream << std::setw(10) << " " << "\t"
				<< std::setw(10) << "[Hz]" << "\t"
				<< std::setw(10) << "[Am^2]" << "\t"
				<< std::setw(10) << "[]" << "\t"
				<< std::setw(10) << "[]" << "\n";
		for (int ii=0;ii<ob.nFreq;ii++) {
			stream << std::setw(10) << ii << "\t"
					<< std::setw(10) << ob.freq(ii) << "\t"
					<< std::setw(10) << ob.TxMom(ii) << "\t"
					<< std::setw(10) << ob.TxOrientation(ii) << "\t"
					<< std::setw(10) << ob.RxOrientation(ii) << "\n";
		}
		stream << " " << "\n";
		stream << std::setw(10) << "Number" << "\t"
				<< std::setw(10) << "Coil Sep X" << "\t"
				<< std::setw(10) << "Coil Sep Y" << "\t"
				<< std::setw(10) << "Coil Sep Z" << "\t"
				<< std::setw(10) << "Coil Scale" << "\n";
		stream << std::setw(10) << " " << "\t"
				<< std::setw(10) << "[m]" << "\t"
				<< std::setw(10) << "[m]" << "\t"
				<< std::setw(10) << "[m]" << "\t"
				<< std::setw(10) << "[]" << "\n";
		for (int ii=0;ii<ob.nFreq;ii++) {
			stream << std::setw(10) << ii << "\t"
					<< std::setw(10) << ob.TxRxSep(0,ii) << "\t"
					<< std::setw(10) << ob.TxRxSep(1,ii) << "\t"
					<< std::setw(10) << ob.TxRxSep(2,ii) << "\t"
					<< std::setw(10) << ob.ScaleFac(ii) << "\n";
		}

		return stream;
	}


	void DataFEM::Delete() {
		this->DetachFrom(this);
	}

	void DataFEM::Release() {
		delete this;
	}

	DataFEM* DataFEM::New() {
		DataFEM* Obj = new DataFEM("DataFEM");
		Obj->AttachTo(Obj);
		return Obj;
	}

	DataFEM* DataFEM::Clone() {
		DataFEM* Obj = new DataFEM("DataFEM");
		Obj->AttachTo(Obj);
		//TODO Copy
		return Obj;
	}

	DataFEM::DataFEM(const std::string &name) : Data(name) {
	}

	DataFEM::~DataFEM() {
		if (this->NumberOfReferences != 0)
			throw DeleteObjectWithReferences(this);
	}

	void DataFEM::Zero() {
		//TODO Zero out all the data members
	}

	Real DataFEM::Norm(Data* Data2) {
		return 0;
		//TODO Compute the norm
	}

	void DataFEM::SetSize(const int &nobs,const int &nfreq) {
		this->nObs = nobs;
		this->nFreq = nfreq;
	}

	void DataFEM::SetData(const MatrixXr &inputdata) {
		this->FEMDataCube = inputdata;
	}

	void DataFEM::SetUncertainties(const MatrixXr &uncertain) {
		this->FEMUncertainCube = uncertain;
	}

	void DataFEM::SetPositions(const Vector3Xr &positions) {
		this->xyz = positions;
	}

	void DataFEM::SetFreq(const VectorXr &freqs) {
		this->freq = freqs;
	}

	void DataFEM::SetTxOrientation(const Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> &txorientation) {
		this->TxOrientation = txorientation;
	}

	void DataFEM::SetRxOrientation(const Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> &rxorientation) {
		this->RxOrientation = rxorientation;
	}

	void DataFEM::SetTxMom(const VectorXr &txmom) {
		this->TxMom = txmom;
	}

	void DataFEM::SetTxRxSep(const Vector3Xr &txrxsep) {
		this->TxRxSep = txrxsep;
	}

	void DataFEM::SetScaleFac(const VectorXr &scalefac) {
		this->ScaleFac = scalefac;
	}

	int DataFEM::GetnFreq() {
		return this->nFreq;
	}

	int DataFEM::GetnObs() {
		return this->nObs;
	}

	MatrixXr DataFEM::GetFEMDataCube() {
		return this->FEMDataCube;
	}

	MatrixXr DataFEM::GetFEMUncertainCube() {
		return this->FEMUncertainCube;
	}

	Real DataFEM::GetDataCoeff(const int &x, const int &y) {
		return this->FEMDataCube.coeff(x,y);
	}

	VectorXr DataFEM::GetFreqs() {
		return this->freq;
	}

	Vector3Xr DataFEM::GetXYZ() {
		return this->xyz;
	}

	VectorXr DataFEM::GetSingleXYZ(const int &x) {
		return this->xyz.block(x,0,1,3);
	}

	Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> DataFEM::GetTxOrien() {
		return this->TxOrientation;
	}

	Eigen::Matrix<ORIENTATION, Eigen::Dynamic, 1> DataFEM::GetRxOrien() {
		return this->RxOrientation;
	}

	VectorXr DataFEM::GetTxMom() {
		return this->TxMom;
	}

	Vector3Xr DataFEM::GetTxRxSep() {
		return this->TxRxSep;
	}

	VectorXr DataFEM::GetScaleFac() {
		return this->ScaleFac;
	}

} // end of namespace Lemma
