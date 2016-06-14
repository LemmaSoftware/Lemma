/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/19/2011
  @version  $Id: modelreadertem1dubc.cpp 153 2014-03-21 23:54:38Z tirons $
 **/

#include "modelreadertem1dubc.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const ModelReaderTem1DUBC &ob) {
		stream << *(ModelReaderTem1DUBC*)(&ob);
		return stream;
	}

	ModelReaderTem1DUBC::ModelReaderTem1DUBC(const std::string &name) :
		ModelReaderTem1D(name), Trans(NULL), Receivers(NULL),
		EarthMod(NULL) {
	}

	ModelReaderTem1DUBC::~ModelReaderTem1DUBC() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	ModelReaderTem1DUBC* ModelReaderTem1DUBC::New() {
		ModelReaderTem1DUBC* Obj = new
			ModelReaderTem1DUBC("ModelReaderTem1DUBC");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void ModelReaderTem1DUBC::Delete() {
		this->DetachFrom(this);
	}

	void ModelReaderTem1DUBC::Release() {
		delete this;
	}

	void ModelReaderTem1DUBC::SetEMEarth1D(LayeredEarthEM* earth) {
		if (this->EarthMod != NULL) {
			this->EarthMod->DetachFrom(this);
		}
		earth->AttachTo(this);
		this->EarthMod=earth;
	}

	void ModelReaderTem1DUBC::SetTransmitter(WireAntenna* antennae) {
		if (this->Trans != NULL) {
			this->Trans->DetachFrom(this);
		}
		antennae->AttachTo(this);
		this->Trans=antennae;
	}

	void ModelReaderTem1DUBC::SetReceiver(ReceiverPoints* receiver) {
		if (this->Receivers != NULL) {
			this->Receivers->DetachFrom(this);
		}
		receiver->AttachTo(this);
		this->Receivers=receiver;
	}

	void ModelReaderTem1DUBC::ReadParameters(const std::string &params) {
		std::string temp;
		std::string temp2;
		char* commentBuffer = new char[200];
		int bufsize;

		bufsize = 200;
		std::fstream infile(params.c_str(),std::ios::in);
		if (infile.fail()) {
			std::cout << "Parameter file I/O error." << std::endl;
		}

		infile.getline(commentBuffer,bufsize);
		temp = commentBuffer;
		std::istringstream in(temp);
		in >> this->instrumentfile;
		in.clear();
		//testing only
		//std::cout << this->instrumentfile << std::endl;

		infile.getline(commentBuffer,bufsize);
		temp = " ";
		temp = commentBuffer;
		std::istringstream in2(temp);
		in2 >> this->modelfile;
		in2.clear();
		//testing only
		//std::cout << this->modelfile << std::endl;

		infile.getline(commentBuffer,bufsize);
		infile.getline(commentBuffer,bufsize);
		infile.getline(commentBuffer,bufsize);
		temp = " ";
		temp = commentBuffer;
		std::istringstream in3(temp);
		in3 >> temp2;
		if (temp2 == "y" || temp2 == "Y" || temp2 == "yes" || temp2 == "Yes" ||
			temp2 == "YES" || temp2 == "true" || temp2 == "True" ||
			temp2 == "TRUE") {
			this->addednoise = true;
		}
		else {
			this->addednoise = false;
		}
		in3.clear();
		//testing only
		//std::cout << this->addednoise << std::endl;

		if (this->addednoise == true) {
			infile.getline(commentBuffer,bufsize);
			temp = " ";
			temp = commentBuffer;
			std::istringstream in4(temp);
			in4 >> this->noisepercent;
			in4 >> this->noisethresh;
			in4 >> this->noiseseed;
			in4.clear();
			//std::cout << this->noisepercent << "  " << this->noisethresh <<
			//	"  " << this->noiseseed << std::endl;
		}

		infile.close();
		delete [] commentBuffer;

		// Call the other readers from here.
		this->ReadModel();
		this->ReadInstrument();
	}

	void ModelReaderTem1DUBC::ReadModel() {
		// Model File cannot have comments!

		int numlayers; //includes both halfspace and air layers
		VectorXr thicknesses;
		VectorXr conductivities;
		VectorXcr complexconduct;
		std::string temp;
		Real temp2;

		std::fstream infile(this->modelfile.c_str(),std::ios::in);
		if (infile.fail()) {
			std::cout << "Model File I/O error." << std::endl;
		}
		infile >> numlayers;
		numlayers = numlayers + 1;
		thicknesses.resize(numlayers-2);
		conductivities.resize(numlayers);
		conductivities(0) = 0.;
		//Testing
		//std::cout << "Numlayers  " << numlayers << std::endl;
		//std::cout << conductivities(0) << std::endl;
		for (int ii=0;ii<numlayers-2;ii++) {
			infile >> thicknesses(ii);
			infile >> conductivities(ii+1);
			//Testing
			//std::cout<<ii<<"  "<<thicknesses(ii)<<"  "<<conductivities(ii+1)
			//	<< std::endl;
		}
		infile >> temp2;
		//Testing
		//std::cout << temp2 << std::endl;
		infile >> conductivities(numlayers-1);
		//Testing
		//std::cout << conductivities(numlayers-1) << std::endl;
		//std::cout << "Thicknesses: " << thicknesses << std::endl;
		//std::cout << "Conductivities: " << conductivities << std::endl;

		//Didn't read the function declaration.  Conductivities needs
		//to be complex...
		complexconduct.resize(conductivities.size());
        complexconduct.setZero();
		complexconduct.real() = conductivities.array();
		//Testing
		//std::cout << complexconduct << std::endl;

		// Attach all to layered earth class
		this->EarthMod->SetNumberOfLayers(numlayers);
		this->EarthMod->SetLayerConductivity(complexconduct);
		this->EarthMod->SetLayerThickness(thicknesses);

		infile.close();

	}

	void ModelReaderTem1DUBC::ReadInstrument() {
		// No comments allowed in this data file either for now.
		int tempint;
		Real tempreal;
		VectorXr transpoints;
		MatrixXr receiverpoints;
		std::string tempstring;
		VectorXr tempvec;
		int timeunits;
		int ntimes;
		int ntimestemp;
		int numsounds;
		VectorXr orientation;
		MatrixXr timestemp;

		//Testing
		//std::cout << "ModelReaderTem1DUBC::ReadInstrument()" << std::endl;

		std::fstream infile(this->instrumentfile.c_str(),std::ios::in);
		if (infile.fail()) {
			std::cout << "Observations File I/O error." << std::endl;
		}
		infile >> tempint;
		this->Trans->SetNumberOfPoints(tempint+1);
		//Testing
		//std::cout << "Number of Points: " << tempint << std::endl;

		transpoints.resize(tempint*2+2);
		//Testing
		//std::cout << "Transmitter corners:  ";
		for (int ii=0;ii<(tempint*2);ii++) {
			infile >> transpoints(ii);
			//Testing
			//std::cout << transpoints(ii) << "  ";
		}
		transpoints(tempint*2) = transpoints(0);
		transpoints(tempint*2+1) = transpoints(1);
		//Testing
		//std::cout << transpoints(tempint*2) << "  " <<
		//	transpoints(tempint*2+1) << std::endl;
		infile >> tempreal;
		//Testing
		//std::cout << "Transmitter height: " << tempreal << std::endl;
		int jj;
		jj = 0;
		for (int ii=0;ii<(tempint+1);ii++) {
			this->Trans->SetPoint(ii,Vector3r(transpoints(jj),
				transpoints(jj+1),tempreal));
			//Testing
			//std::cout<< transpoints(jj) << transpoints(jj+1) << std::endl;
			jj = jj+2;
		}

		//Testing


		infile >> this->waveformfile;
		//Testing
		//std::cout << "Waveform file: " << this->waveformfile << std::endl;

		infile >> numsounds; // number of soundings
		//Testing
		//std::cout << "Number of soundings: " << numsounds << std::endl;
		this->Trans->SetCurrent(1);
		this->Trans->SetNumberOfTurns(1);
		infile >> timeunits; // flag for units of time
		// 1 = microseconds, 2 = milliseconds, 3 = seconds
		//Testing
		//std::cout << "Time units: " << timeunits << std::endl;

		receiverpoints.resize(numsounds,3);
		//Testing
		//std::cout << "Receiverpoints resized" << std::endl;
		this->receivermoment.resize(numsounds);
		//Testing
		//std::cout << "Receivermoment resized" << std::endl;
		orientation.resize(numsounds);
		//Testing
		//std::cout << "Orientation resized" << std::endl;
		ntimestemp = 30;
		timestemp.resize(numsounds,ntimestemp);
		//Testing
		//std::cout << "timestemp resized" << std::endl;
		this->Receivers->SetNumberOfReceivers(numsounds);
		//Testing
		//std::cout << "Reading receiver points" << std::endl;
		for (int ii=0;ii<numsounds;ii++) {
			//Testing
			//std::cout << "Beginning iteration: " << ii << std::endl;
			infile >> this->receivermoment(ii); //receiver moment
			//Testing
			//std::cout << "Receiver moment:  " << this->receivermoment(ii)
			//	<< std::endl;
			infile >> receiverpoints(ii,0);
			infile >> receiverpoints(ii,1);
			infile >> receiverpoints(ii,2);
			//Testing
			//std::cout << "Receiverpoints:  " << receiverpoints.row(ii)
			//	<< std::endl;
			this->Receivers->SetLocation(ii,receiverpoints.row(ii));
			infile >> tempstring; //receiver orientation
			//Testing
			//std::cout << "Receiver orientation: " << tempstring << std::endl;
			if (tempstring == "z") {
				orientation(ii) = 0;
			}
			if (tempstring == "x") {
				orientation(ii) = 1;
			}
			if (tempstring == "y") {
				orientation(ii) = 2;
			}
			else {
				orientation(ii) = 0;
			}
			//Testing
			//std::cout << "Modified orientation: " << orientation(ii)
			//	<< std::endl;

			infile >> ntimes;
			//Testing
			//std::cout << "Number of gates: " << ntimes << std::endl;
			if (ntimes != ntimestemp) {
				timestemp.resize(Eigen::NoChange,ntimes);
				ntimestemp = ntimes;
				//Testing
			//	std::cout << "Better not be here!" << std::endl;
			}
			//Testing
			//std::cout << "Actual size: " << timestemp.size() << std::endl;
			infile >> tempint;
			//Testing
			//std::cout << "Unknown variable: " << tempint << std::endl;
			//std::cout << "Times: ";
			for (int jj=0;jj<ntimes;jj++) {
				infile >> timestemp(ii,jj);
				//Testing
				std::cout << "time read? " << jj << ". " << timestemp(ii,jj) << "\t " << tempint << std::endl;
				infile >> tempint;
			}
			//Testing
			//std::cout << std::endl;
		}

		infile.close();
		//Testing
		//std::cout << "Left the loop" << std::endl;
		// Make sure times are in seconds
		if (timeunits == 1) {
			timestemp = timestemp.array()/1e6;
			timeunits = 3;
		}
		if (timeunits == 2) {
			timestemp = timestemp.array()/1e3;
			timeunits = 3;
		}
		//Testing
		//std::cout << "Scaled times: " << timestemp << std::endl;

		this->modtimes.resize(ntimes);
		//Testing
		//std::cout << "Modtimes size: " << this->modtimes.size() << std::endl;
		//std::cout << "timestemp size: " << timestemp.rows() << " x " <<
		//	timestemp.cols() << std::endl;
		tempvec = timestemp.row(0);
		this->modtimes = tempvec.array();
		//Testing
		//std::cout << "Finished with loading modtimes" << std::endl;
		// This code has been written so far to allow for the general case
		// of soundings with different vectors of times.  However, the
		// instrument class has not been defined that way yet.  So right now
		// only one vector of times is returned via an accessor.  This will
		// be changed later.


	}

	void ModelReaderTem1DUBC::ReadWaveform() {

	}

	VectorXr ModelReaderTem1DUBC::GetTimes() {

		//Testing
		//std::cout << this->modtimes;
		return this->modtimes;
	}

	//Datamembers:
	// modelfile, instrumentfile, waveformfile

} // namespace Lemma
