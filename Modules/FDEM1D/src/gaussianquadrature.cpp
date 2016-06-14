/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     10/07/2010
  @version  $Id: gaussianquadrature.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "gaussianquadrature.h"

namespace Lemma {

//	std::ostream &operator<<(std::ostream &stream,
//				const gaussianquadrature &ob) {
//
//	}

	////////////////////////////////////////////////
	gaussianquadrature::gaussianquadrature() {
	}
	gaussianquadrature::~gaussianquadrature() {
	}
	gaussianquadrature* gaussianquadrature::New() {
		return new gaussianquadrature;
	}
	void gaussianquadrature::Delete() {
		delete this;
	}

	////////////////////////////////////////////////
	void gaussianquadrature::Integrate() {

		this->intres=(this->funcx.array()*this->wgq.array()).sum();

	}

	////////////////////////////////////////////////
	void gaussianquadrature::CalcAW() {

		while ((this->y-this->y0).array().abs().maxCoeff()>
				this->machineeps) {
			this->L.col(0).setOnes();
			this->Ltemp.col(0).setZero();
			this->L.col(1)=this->y;
			this->Ltemp.col(1).setOnes();

			for (int kk=2;kk<this->N2;++kk) {
				this->ytemp=this->L.col(kk-1).array()*this->y.array();
				this->ytemp=this->ytemp.array()*(2.0*Real(kk)-1.0);
				this->L.col(kk)=this->ytemp.array()-((Real(kk)-1.0)*
					this->L.col(kk-2).array());
				this->L.col(kk)=this->L.col(kk).array()/Real(kk);
			}
			this->ytemp=this->y.array()*this->L.col(this->N1).array();
			this->Lp=this->L.col(this->N1-1)-this->ytemp;
			this->Lp=this->Lp.array()/(1.0-(this->y.array()*this->y.array()));
			this->Lp=this->Lp.array()*this->N2;
			this->y0=this->y;
			this->ytemp=this->L.col(this->N1);
			this->y=this->y0.array()-(this->ytemp.array()/this->Lp.array());
		}

		for (int ii=0;ii<this->N1;++ii) {
			this->xu(ii)=((this->b-this->a)*((this->y(ii)+1.0)/2.0))+this->a;
			this->wgq(ii)=((this->b-this->a)/((1.0-(this->y(ii)*this->y(ii)))*
				(this->Lp(ii)*this->Lp(ii))))*((Real(this->N2)/Real(this->N1))*
				(Real(this->N2)/Real(this->N1)));
		}
	}

	////////////////////////////////////////////////
	void gaussianquadrature::SetFreqs(const int& nfreq,
			const Real& a, const Real& b) {
		this->N = nfreq-1;
		this->N1 = nfreq;
		this->N2 = nfreq+1;
		// \TODO Use pointers here instead
		this->a = a;
		this->b = b;

		//Debugging only
		//std::cout << N << "   " << a << "   " << b << std::endl;

		this->dx = (2.0)/Real(nfreq);
		this->machineeps = std::numeric_limits<Real>::epsilon();

		this->ytemp.resize(this->N1);
		this->xu.resize(this->N1);
		this->y.resize(this->N1);
		this->y0.resize(this->N1);
		this->Lp.resize(this->N1);
		this->wgq.resize(this->N1);
		// \TODO Instead of copying the variable, use pointers
		this->funcx.resize(nfreq);

		this->L.resize(this->N1,this->N2);
		this->Ltemp.resize(this->N1,this->N2);

		for (int ii=0;ii<this->N1;++ii) {
			this->xu(ii)=(dx*Real(ii))-1.0;
			this->y(ii)=std::cos((2.0*Real(ii)+1.0)*PI/(2.0*
				Real(this->N)+2.0))+(0.27/(Real(this->N1)))*
				std::sin(PI*xu(ii)*Real(this->N)/(Real(this->N2)));
			this->y0(ii)=2.0;
			this->ytemp(ii)=0.0;
		}
	}

	////////////////////////////////////////////////
	void gaussianquadrature::SetFunc(const VectorXr& fx) {
		this->funcx = fx;
	}

	////////////////////////////////////////////////
	VectorXr gaussianquadrature::GetAbscissae() {

		return this->xu;
	}

	////////////////////////////////////////////////
	VectorXr gaussianquadrature::GetWeights() {

		return this->wgq;
	}

	////////////////////////////////////////////////
	Real gaussianquadrature::GetResult() {

		return intres;
	}

} //end of namespace Lemma

