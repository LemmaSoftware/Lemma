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

#include "datareaderfemubc.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const DataReaderFemUBC &ob) {
		stream << *(DataReader*)(&ob);
		return stream;
	}

	DataReaderFemUBC::DataReaderFemUBC(const std::string &name) :
		DataReader(name), InputData(NULL) {

	}

	DataReaderFemUBC::~DataReaderFemUBC() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	DataReaderFemUBC* DataReaderFemUBC::New() {
		DataReaderFemUBC* Obj = new DataReaderFemUBC("DataReaderFemUBC");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void DataReaderFemUBC::Delete() {
		this->DetachFrom(this);
	}

	void DataReaderFemUBC::Release() {
		delete this;
	}

	DataFEM* DataReaderFemUBC::GetData() {
		return this->InputData;
	}

	void DataReaderFemUBC::SetDataFEM(DataFEM* inputtemp) {
		if (this->InputData != NULL) {
			this->InputData->DetachFrom(this);
		}
		inputtemp->AttachTo(this);
		this->InputData = inputtemp;
	}

	void DataReaderFemUBC::ReadData(const std::string &datafile,int vbs) {
		if (vbs != 0) {
			std::cout << std::endl << "   Reading datafile: " << datafile <<
				std::endl;
		}

		std::string temp;

		Real rtemp;
		//MatrixXr datamatrix;
		//MatrixXr uncertainties
		//Vector3Xr positions;
		//VectorXr freqs;
		//VectorXr txmom;
		Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> txorien;
		Eigen::Matrix<ORIENTATION,Eigen::Dynamic,1> rxorien;
		//VectorXr scalefactor;
		//Vector3Xr txrxsep;
		int ontype;

		std::fstream infile(datafile.c_str(),std::ios::in);
		if (infile.fail()) {
			throw GenericFileIOError(this,datafile);
		}
///*
		// Get down to brass tacks and read the file
		infile >> this->nObs;

		this->PositionVec.resize(Eigen::NoChange,this->nObs);


		for (int ii=0;ii<this->nObs;ii++) {
			infile >> this->PositionVec(0,ii);
			infile >> this->PositionVec(1,ii);
			infile >> this->nFreq;
			if (ii==0) {
				this->DataMatrix.resize(this->nObs,this->nFreq*2);
				this->Uncertainties.resize(this->nObs,this->nFreq*2);
				this->Freqs.resize(this->nFreq,Eigen::NoChange);
				txorien.resize(this->nFreq,Eigen::NoChange);
				rxorien.resize(this->nFreq,Eigen::NoChange);
				this->ScaleFac.resize(this->nFreq);
				this->TxRxSep.resize(Eigen::NoChange,this->nFreq);
				this->TxMom.resize(this->nFreq,Eigen::NoChange);
			}
			for (int jj=0;jj<this->nFreq;jj++) {
				infile >> this->Freqs(jj);
				infile >> temp;
				infile >> this->TxMom(jj);
				infile >> this->PositionVec(2,ii);
				this->PositionVec(2,ii) = this->PositionVec(2,ii) * -1;
				infile >> temp;
				if (temp.compare("x")==0 || temp.compare("X")==0) {
					txorien(jj) = X;
				}
				else if (temp.compare("y")==0 || temp.compare("Y")==0) {
					txorien(jj) = Y;
				}
				else if (temp.compare("z")==0 || temp.compare("Z")==0) {
					txorien(jj) = Z;
				}
				else {
					std::cout << "Error reading transmitter orientation.  " <<
						"Assuming z." << std::endl;
					txorien(jj) = Z;
				}
				infile >> temp;
				infile >> this->ScaleFac(jj);
				infile >> this->TxRxSep(0,jj);
				infile >> this->TxRxSep(1,jj);
				infile >> rtemp;
				this->TxRxSep(2,jj) = this->PositionVec(2,ii) + rtemp;
				infile >> temp;
				if (temp.compare("x")==0 || temp.compare("X")==0) {
					rxorien(jj) = X;
				}
				else if (temp.compare("y")==0 || temp.compare("Y")==0) {
					rxorien(jj) = Y;
				}
				else if (temp.compare("z")==0 || temp.compare("Z")==0) {
					rxorien(jj) = Z;
				}
				else {
					std::cout << "Error reading receiver orientation.  " <<
						"Assuming z." << std::endl;
					rxorien(jj) = Z;
				}

				infile >> ontype;
				//ASSUMING THAT ONLY INPHASE AND QUADRATURE ARE PRESENT
				infile >> temp;
				infile >> this->DataMatrix(ii,(2*jj));
				infile >> this->DataMatrix(ii,(2*jj)+1);
				//uncertainty type
				infile >> temp;
				//pair of uncertainties
				if (temp.compare("v") == 0 || temp.compare("V") == 0) {
					infile >> this->Uncertainties(ii,(2*jj));
					infile >> this->Uncertainties(ii,(2*jj)+1);
				}
				else if (temp.compare("p")==0 || temp.compare("P")==0) {
					infile >> rtemp;
					this->Uncertainties(ii,(2*jj)) = this->DataMatrix(ii,(2*jj))*rtemp;
					infile >> rtemp;
					this->Uncertainties(ii,(2*jj)+1)=this->DataMatrix(ii,(2*jj)+1)*rtemp;
				}
				else {
					std::cout << "Error reading data file.  Unknown uncertainty parameter." << std::endl;
				}
			}
		}
		infile.close();

		this->TxOrientation = txorien;
		this->RxOrientation = rxorien;


		// Populate the data object now

		this->SendData();
	}

	void DataReaderFemUBC::SendData() {
		this->InputData->SetSize(this->nObs,this->nFreq);
		this->InputData->SetData(this->DataMatrix);
		this->InputData->SetUncertainties(this->Uncertainties);
		this->InputData->SetPositions(this->PositionVec);
		this->InputData->SetFreq(this->Freqs);
		this->InputData->SetTxMom(this->TxMom);
		this->InputData->SetTxRxSep(this->TxRxSep);
		this->InputData->SetTxOrientation(this->TxOrientation);
		this->InputData->SetRxOrientation(this->RxOrientation);
		this->InputData->SetScaleFac(this->ScaleFac);


	}

} // end of namespace Lemma
