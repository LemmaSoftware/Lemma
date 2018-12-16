/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/15/2011
  @version  $Id: digitalfilterintegrator.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "DigitalFilterIntegratorAnderson.h"

namespace Lemma {

    // Specialisations to templated class are found here. This means that
    // DigitalFilterIntegratorAnderson is not strictly templated, and must be linked
    // with lemma for functionality.

    template < >
    inline Complex DigitalFilterIntegratorAnderson<Complex>::AbsMax(const Complex& C,
        const Complex& Cmax) {
		return Complex(std::max(std::abs(real(C)), std::real(Cmax)),
					   std::max(std::abs(imag(C)), std::imag(Cmax)) );
    }

    template < >
    Real DigitalFilterIntegratorAnderson<Real>::AbsMax(const Real& C,
        const Real& Cmax) {
        return std::max(C, Cmax);
    }

    ///////////////////////////////////////////
	// Computes the transform y

    template < >
    void DigitalFilterIntegratorAnderson<Real>::Compute(const Real& rho,
        const int& ntol, const Real& tol) {

		Real y1 = this->ABSCISSA/rho;
		this->Key.setZero();

		// Check to make sure everything is set
		if (rho<=0) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson Argument rho < 0.");
		}

		if (this->NumConv<1) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson NumConv is less than 1.");
		}

		if (this->IntKernel == NULL) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson Unset Kernel Calculator");
		}

		Arg = VectorXr::Zero(this->NumConv);
		Real y = std::pow(rho*ABSER, this->NumConv-1);

		if (y <= 0) {
			std::cerr << "Exponent Underflow Error";
			throw std::underflow_error("Exponent underflow");
		}

		this->Work.resize(Eigen::NoChange, this->IntKernel->GetNumRel());

        // Main Loop
		int itol = 0;
		int none = 0;
		this->NumFun = 0;
		int idx = 0;
		int istore = 0;
		Real Sum(0.);
		Real Cmax(0.);
		Real C(0.);
		Ans.resize(this->NumConv, this->IntKernel->GetNumRel());
		Ans.setZero();
 		// 1010 Loop
 		for (int ilag=0; ilag < this->NumConv; ++ilag) {
  			istore = this->NumConv - 1 - ilag;
  			if (ilag > 0) y1 *= ABSE;
  			Arg(istore) = ABSCISSA/y1;

 			// 1000 Loop
 			for (int irel=0; irel < this->IntKernel->GetNumRel(); ++irel) {
				//this->SetBesselOrder(this->Ckernel->GetBesselOrder(irel));
  				none = 0;
  				itol = ntol;
  				Sum = Real(0);
  				Cmax = Real(0);
  				y = y1;
				// Begin right side convolution at weight 298
				// counting from 0
  				idx = ilow;
  				y *= ABSE;
				// Code Block 20 in Anderson
				do {
 					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
                    Cmax = AbsMax(C, Cmax);
					++idx;
					y *= ABSE;
				} while (idx <= ihi);
				//if (real(Cmax) == 0 && imag(Cmax) == 0) none = 1;
				if (Cmax == Real(0)) none = 1;
				Cmax *= tol;
				// Code Block 30 in Anderson
				do {
 					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
					if (std::abs(C) <= Cmax) {
						--itol;
						if (itol < 0 || idx > FilterWeights.size()-1) break;
					} else {
						itol = ntol;
					}
					++idx;
					y *= ABSE;
				} while (idx < FilterWeights.size());
				itol = ntol;
				y = y1;
				// Code Block 60 in Anderson
				idx = ilow-1;
				do {
 					this->StoreRetreive(idx, ilag, y, Sum, irel, C);
					if (std::abs(C) <= Cmax &&  none == 0) {
						--itol;
						if (itol < 0 || idx < 0) break;
					} else {
						itol = ntol;
					}
					--idx;
					y *= ABSER;
				} while (idx>=0);
 				Ans(istore, irel) = Sum/Arg(istore);
  			}  // End of 1000 loop
  		}      // End of 1010 loop
	}


    template < >
	void DigitalFilterIntegratorAnderson<Complex>::Compute(const Real &rho,
            const int& ntol, const Real &tol) {

		Real y1 = this->ABSCISSA/rho;

		this->Key.setZero();

		// Check to make sure everything is set
		if (rho<=0) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson Argument rho < 0.");
		}

		if (this->NumConv<1) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson NumConv is less than 1.");
		}

		if (this->IntKernel == NULL) {
			throw std::runtime_error("In DigitalFilterIntegratorAnderson Unset Kernel Calculator");
		}

		Arg = VectorXr::Zero(this->NumConv);
		Real y = std::pow(rho*ABSER, this->NumConv-1);

		if (y <= 0) {
			std::cerr << "Exponent Underflow Error";
			throw std::underflow_error("Exponent underflow");
		}

		this->Work.resize(Eigen::NoChange, this->IntKernel->GetNumRel());

        // Main Loop
		int itol = 0;
		int none = 0;
		this->NumFun = 0;
		int idx = 0;
		int istore = 0;
		Complex Zsum(0.);
		Complex Cmax(0.);
		Complex C(0.);
		Ans.resize(this->NumConv, this->IntKernel->GetNumRel());
		Ans.setZero();
 		// 1010 Loop
 		for (int ilag=0; ilag < this->NumConv; ++ilag) {
  			istore = this->NumConv - 1 - ilag;
  			if (ilag > 0) y1 *= ABSE;
  			Arg(istore) = ABSCISSA/y1;
 			// 1000 Loop
 			for (int irel=0; irel < this->IntKernel->GetNumRel(); ++irel) {
				//this->SetBesselOrder(this->Ckernel->GetBesselOrder(irel));
  				none = 0;
  				itol = ntol;
  				Zsum = Complex(0);
  				Cmax = Complex(0);
  				y = y1;
				// Begin right side convolution at weight 298
				// counting from 0
  				idx = ilow;
  				y *= ABSE;
				// Code Block 20 in Anderson
				do {
 					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
                    Cmax = AbsMax(C, Cmax);
					++idx;
					y *= ABSE;
				} while (idx <= ihi);
				//if (real(Cmax) == 0 && imag(Cmax) == 0) none = 1;
				if (Cmax == Complex(0)) none = 1;
				Cmax *= tol;
				// Code Block 30 in Anderson
				do {
 					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
					if ( std::abs(real(C)) <= real(Cmax) &&
						 std::abs(imag(C)) <= imag(Cmax) ) {
						--itol;
						if (itol < 0 || idx > FilterWeights.size()-1) break;
					} else {
						itol = ntol;
					}
					++idx;
					y *= ABSE;
				} while (idx < FilterWeights.size());
				itol = ntol;
				y = y1;
				// Code Block 60 in Anderson
				idx = ilow-1;
				do {
 					this->StoreRetreive(idx, ilag, y, Zsum, irel, C);
					if ( std::abs(real(C)) <= real(Cmax) &&
						 std::abs(imag(C)) <= imag(Cmax) &&
						 none == 0 ) {
						--itol;
						if (itol < 0 || idx < 0) break;
					} else {
						itol = ntol;
					}
					--idx;
					y *= ABSER;
				} while (idx>=0);
 				Ans(istore, irel) = Zsum/Arg(istore);
  			}  // End of 1000 loop
  		}      // End of 1010 loop
	}

}		// -----  end of Lemma  name  -----
