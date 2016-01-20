/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     03/23/2011
  @version  $Id: inversesolvertem1d.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "inversesolvertem1d.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const InverseSolverTEM1D &ob) {
		stream << *(LemmaObject*)(&ob);
		stream << "Wire antenna source address: " << ob.Antenna << std::endl;
		stream << "Dipole source address: " << ob.Dipole << std::endl;
		stream << *ob.Receiver << std::endl;
		stream << *ob.StartingMod << std::endl;
		stream << *ob.RecoveredMod << std::endl;
		stream << *ob.InstrumentFor << std::endl;
		return stream;
	}

    // ====================  LIFECYCLE     =======================

	InverseSolverTEM1D::InverseSolverTEM1D(const std::string &name) :
		InverseSolver(name),Antenna(NULL), Dipole(NULL),
		Receiver(NULL), StartingMod(NULL), RecoveredMod(NULL),
		InstrumentFor(NULL) {

		this->InstrumentFor = InstrumentTem::New();
		//this->SCondIndex.resize(42);
		//this->SThickIndex.resize(42);
		//this->SCondIndex(0) = 42;
		//this->SThickIndex(0) = 42;
	}

	InverseSolverTEM1D::~InverseSolverTEM1D() {
		if (NumberOfReferences !=0) throw DeleteObjectWithReferences(this);

		if (this->Antenna != NULL) this->Antenna->DetachFrom(this);

		if (this->Dipole != NULL) this->Dipole->DetachFrom(this);

		if (this->Receiver != NULL) this->Receiver->DetachFrom(this);

		if (this->StartingMod != NULL) this->StartingMod->DetachFrom(this);

		if (this->RecoveredMod != NULL) this->RecoveredMod->DetachFrom(this);

		this->InstrumentFor->Delete();

	}

    InverseSolverTEM1D* InverseSolverTEM1D::New() {
        InverseSolverTEM1D*  Obj =
            new InverseSolverTEM1D( "InverseSolverTEM1D" );
        Obj->AttachTo(Obj);
        return Obj;

    }

    void InverseSolverTEM1D::Delete() {
        this->DetachFrom(this);
    }

    void InverseSolverTEM1D::Release() {
        delete this;
    }

    // ====================  OPERATIONS    =======================

	void InverseSolverTEM1D::Calculate() {
		MatrixXr jac;
		VectorXr newthickness;
		VectorXcr newconductivity;
		VectorXr oldthickness;
		VectorXcr oldconductivity;
		MatrixXr olddata;
		MatrixXr newdata;
		VectorXr deltathick;
		VectorXcr deltacond;
		MatrixXr H;
		VectorXr g;
		VectorXr p;
		VectorXr s;
		VectorXr y;

		//int nrec=42;
		int nlayers=42;

		this->InstrumentFor->EMEarthModel(this->RecoveredMod);
		this->InstrumentFor->SetTransmitLoop(this->Antenna);
		this->InstrumentFor->SetReceiver(this->Receiver);
		this->InstrumentFor->SetTimes(this->Times);

		nlayers = this->RecoveredMod->GetNumberOfLayers();
		//nrec = this->Receiver->GetNumberOfReceivers();
		//check for parameters set
		if (this->SCondIndex.size() == 0 /*&& SCondIndex(0) == 42*/) {
			//std::cout << "Cond Index is null" << std::endl;
			this->SCondIndex.resize(nlayers-1);
			for (int ii=1;ii<nlayers;ii++) {
				this->SCondIndex(ii-1)=ii;
			}
		}
		if (this->SThickIndex.size()==0 /*&& SThickIndex(0)==42*/) {
			//std::cout << "Thickness Index is null" << std::endl;
			this->SThickIndex.resize(nlayers-2);
			for (int ii=0;ii<nlayers-2;ii++) {
				this->SThickIndex(ii)=ii+1;
			}
		}

		std::cout << "Thickness indices to solve for: " << this->SThickIndex
			<< std::endl;
		//std::cout << "Conductivity indices to solve for: " << this->SCondIndex
		//	<< std::endl;


		//Real alpha;

		// while \nabla g > \epsilon
		// Forward model from starting
		this->InstrumentFor->MakeLaggedCalculation(FHTKEY201);
		olddata = this->InstrumentFor->GetMeasurements();
		// Get A Jacobian Matrix
		jac = this->CalcJac(this->RecoveredMod);

		// Output jacobian matrix
		std::ofstream outfile2;
		outfile2.open("jacobian.out");
		for (int ii=0; ii<jac.rows();ii++) {
			for (int jj=0;jj<jac.cols();jj++) {
				outfile2 << jac(ii,jj) << "    ";
			}
			outfile2 << std::endl;
		}
		outfile2.close();

		H = jac.transpose()*jac;
		std::cout << "H: " << std::endl;
		std::cout << H << std::endl;

		//Temporary solution for only one linear step
		//delta m = (JTJ)^-1JTd
		//VectorXr x0;
		//VectorXr md;
		//x0.resize(jac.cols());
		//x0.setZero();
		//VectorXr jtd;
		//md = this->MeasuredData.col(0);
		//std::cout << md << std::endl;
		//jtd = jac.transpose()*md;
		//std::cout << jtd.size() << std::endl;
		//Real tol;
		//int iter;
		//tol = 1.e-8;
		//iter = 1000;
		//this->RecModStep = CG(H,x0,jtd,iter,tol);
		//std::cout << this->RecModStep << std::endl;

		// Gauss-Newton Solution
		//


		// Quasi newton solution
		// while ||\nabla g|| > \epsilon
			// g = jac.transpose()*(olddata(:,1)-this->MeasuredData(:,1));
			// p = H*g*(-1);
			// newmod = oldmod + alpha * p
			// s = alpha * p
			// y = g(k+1) - g(k)
			// H(k+1) = H - (H*s*s.transpose()*H)/(s.T*H*s) + (y*y.T)/(y.T*s)
		// Solve for model perturbation
		// end while


	}

	MatrixXr InverseSolverTEM1D::CalcJac(LayeredEarthEM* m0) {
		MatrixXr jac;
		int nrec;
		int nlayers;
		int nparams;
		int nfsigs;
		//int nflays;
		Real step = 0.05;
		VectorXr currentsigma;
		VectorXr currentthick;
		VectorXr newsigma;
		VectorXr newthick;
		Complex ctemp;
		VectorXcr vctemp;
		VectorXr params;
		MatrixXr currentmeasurements;
		MatrixXr perturbedmeasurements;
		MatrixXr diffs;

		nrec = this->Receiver->GetNumberOfReceivers();
		nlayers = this->StartingMod->GetNumberOfLayers();
		nfsigs = this->SCondIndex.size();
		//nflays = this->SThickIndex.size();
		nparams = this->SCondIndex.size() + this->SThickIndex.size();
		jac.resize(nrec*this->Times.size(),nparams); /// TODO memory leak
		params.resize(nparams);
		std::cout << this->SThickIndex << std::endl;

		//Need to set currentsigma and currentthick
		//
		currentsigma.resize(nlayers);
		currentthick.resize(nlayers);
		currentsigma = m0->GetLayerConductivity().array().real();
		for (int ii=0;ii<nlayers;ii++) {
			currentthick(ii) = m0->GetLayerThickness(ii);
		}
		newsigma = currentsigma;
		newthick = currentthick;

		currentmeasurements = this->InstrumentFor->GetMeasurements();

		// Concatenate thicknesses and conductivities into one vector (params)
		// Here is where I write over the original parameters
		for (int ii=0;ii<SCondIndex.size();ii++) {
			params(ii)=currentsigma(this->SCondIndex(ii));
		}
		int jj = 0;
		for (int ii=SCondIndex.size();ii<nparams;ii++) {
			params(ii) = currentthick(this->SThickIndex(jj)); //this was +1
			jj++;
		}
		std::cout << "Size Params: " << params.size() << std::endl;
		std::cout << "Size newsigma: " << newsigma.size() << std::endl;
		std::cout << "Size newthick: " << newthick.size() << std::endl;
		std::cout << "Size currentsigma: " << currentsigma.size() << std::endl;
		std::cout << "Size currentthick: " << currentthick.size() << std::endl;
		std::cout << "Thickness vector : " << currentthick << std::endl;
		std::cout << "Sigma vector: " << currentsigma << std::endl;



		//std::cout << "Jacobian size: " << jac.rows() << " x " <<
		//	jac.cols() << std::endl;

		Real tparam;
		for (int jj=0;jj<nparams;jj++) {
			tparam = params(jj)*step;
			params(jj) = params(jj) + tparam;
			newsigma = currentsigma;
			newthick = currentthick;
			std::cout << "New sigma, in loop: " << newsigma << std::endl;
			std::cout << "New thick, in loop: " << newthick << std::endl;
			//plop params back into newsigma and newthick
			for (int kk=0;kk<nfsigs;kk++) {
				std::cout << "Sigma index: " << this->SCondIndex(kk) << std::endl;
				newsigma(this->SCondIndex(kk)) = params(kk);
			}
			int ii = 0;
			std::cout << "Params, in loop: " << params << std::endl;
			for (int kk=nfsigs;kk<nparams;kk++) {
				std::cout << "Thick index: " << this->SThickIndex(ii) << std::endl;
				newthick(this->SThickIndex(ii)) = params(kk); //here is the problem
				//params is loaded incorrectly
				ii++;
			}
			std::cout << "New thickness: " << newthick << std::endl;
			std::cout << "New cond: " << newsigma << std::endl;
			m0->SetLayerThickness(newthick.segment(1,newthick.size()-2));
			m0->SetLayerConductivity(newsigma.cast<Complex>());
			std::cout << *m0 << std::endl;

			this->InstrumentFor->MakeLaggedCalculation(FHTKEY201);
			perturbedmeasurements = this->InstrumentFor->GetMeasurements();
			diffs = currentmeasurements - perturbedmeasurements;
			diffs = diffs / tparam;
			//Put the values back
			params(jj) = params(jj) - tparam;
			for (int ii=0;ii<jac.rows();ii++) {
				jac(ii,jj)=diffs(ii,1);
			}
			std::cout << "Current Jacobian: " << std::endl;
			std::cout << jac << std::endl;

		}

		return jac;

	}

	void InverseSolverTEM1D::AttachStartMod(LayeredEarthEM* inpstartmod) {
		if (this->StartingMod != NULL) {
			this->StartingMod->DetachFrom(this);
		}
		inpstartmod->AttachTo(this);
		this->StartingMod = inpstartmod;
	}

	void InverseSolverTEM1D::AttachRecMod(LayeredEarthEM* inprecmod) {
		if (this->RecoveredMod != NULL) {
			this->RecoveredMod->DetachFrom(this);
		}
		inprecmod->AttachTo(this);
		this->RecoveredMod = inprecmod;
	}

	void InverseSolverTEM1D::AttachWireAntenna(WireAntenna* inpwireantenna) {
		if (this->Antenna != NULL) {
			this->Antenna->DetachFrom(this);
		}
		inpwireantenna->AttachTo(this);
		this->Antenna = inpwireantenna;
	}

	void InverseSolverTEM1D::AttachDipoleSource(DipoleSource* inpdipsource) {
		if (this->Dipole != NULL) {
			this->Dipole->DetachFrom(this);
		}
		inpdipsource->AttachTo(this);
		this->Dipole = inpdipsource;
	}

	void InverseSolverTEM1D::AttachReceiver(ReceiverPoints* inpreceiver) {
		if (this->Receiver != NULL) {
			this->Receiver->DetachFrom(this);
		}
		inpreceiver->AttachTo(this);
		this->Receiver = inpreceiver;
	}

	void InverseSolverTEM1D::SetTimes(VectorXr &inptimes) {
		this->Times = inptimes;

	}

	void InverseSolverTEM1D::ShowSoln() {
		int ntimes;
		ntimes = this->InstrumentFor->GetNumberOfTimes();

		for (int ii=0;ii<ntimes;ii++) {
			std::cout<<"  "<<this->InstrumentFor->GetMeasurements()(ii,0)
			<<"  "<<this->InstrumentFor->GetMeasurements()(ii,1)<<std::endl;
		}
	}

	void InverseSolverTEM1D::WriteSoln(const std::string &filename) {
		int nlayers;
		VectorXcr conductivities;
		VectorXr thicknesses;

		nlayers = this->RecoveredMod->GetNumberOfLayers();
		conductivities.resize(nlayers);
		thicknesses.resize(nlayers);
		conductivities = this->RecoveredMod->GetLayerConductivity();
		for (int ii=0;ii<nlayers;ii++) {
			thicknesses(ii) = this->RecoveredMod->GetLayerThickness(ii);
		}

		std::ofstream outfile1;
		outfile1.open(filename.c_str());
		// Model file has format:
		// thickness (m) 	conductivity (S/m)
		for (int ii=0;ii<nlayers;ii++) {
			outfile1 << thicknesses(ii) << "    " << conductivities(ii) <<
				"    " << std::endl;
		}
		outfile1.close();

	}

	void InverseSolverTEM1D::SetFreeParams(const VectorXi &layers,
		const VectorXi &cond) {

		this->SCondIndex = cond;
		this->SThickIndex = layers;
	}

	void InverseSolverTEM1D::AttachMeasuredData(const MatrixXr &data) {
		this->MeasuredData = data;

	}

	int InverseSolverTEM1D::NumberOfIterations() {

		return 42;
	}

	bool InverseSolverTEM1D::Success() {

		return false;
	}

	VectorXr InverseSolverTEM1D::GetPhiMVector() {
		VectorXr temp;
		temp.resize(1);

		temp(0)=42;
		return temp;
	}

	VectorXr InverseSolverTEM1D::GetPhiDVector() {
		VectorXr temp;
		temp.resize(1);
		temp(0) = 42;
		return temp;
	}

	void InverseSolverTEM1D::FillInG(const Vector3r& pos,
		const Vector3r& step) {

	}

}

