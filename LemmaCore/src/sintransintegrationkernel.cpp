/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/08/2011
  @version  $Id: sintransintegrationkernel.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "sintransintegrationkernel.h"

namespace Lemma {

    // ====================  FRIENDS       =======================

//     std::ostream &operator<<(std::ostream &stream,
// 			const DigitalFilterSinTrans &ob) {
// 		stream << *(DigitalFilterIntegrator<Real>*)(&ob);
// 		return stream;
// 	}

    // ====================  LIFECYCLE     =======================

    SinTransIntegrationKernel::
        SinTransIntegrationKernel(const std::string& name) :
        IntegrationKernel<Real>(name) {
    }

    SinTransIntegrationKernel::~SinTransIntegrationKernel() {}


    SinTransIntegrationKernel* SinTransIntegrationKernel::New() {
        SinTransIntegrationKernel* Obj =
            new SinTransIntegrationKernel("SinTransIntegrationKernel");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void SinTransIntegrationKernel::Delete() {
        this->DetachFrom(this);
    }

    void SinTransIntegrationKernel::Release() {
        delete this;
    }

    // ====================  OPERATIONS    =======================

    void SinTransIntegrationKernel::SetA(const Real& Ain) {
        A = Ain;
    }

    void SinTransIntegrationKernel::SetIntegral(const int& i) {
        Integral = i;
    }

    Real SinTransIntegrationKernel::GetAnalytical (const Real& B) {
        Real SQPI = std::sqrt(PI);
        switch (Integral) {
            case 1:
                return SQPI*B*std::exp(-B*B/(4.*A*A))/(4.*A*A*A);
                break;
            case 2:
                return (PI/2.)*std::exp(-A*B);
                break;
            case 3:
                return B/(A*A+B*B);
                break;
            default:
                std::cerr << "Test integral is not allowed\n";
                exit(EXIT_FAILURE);
        }
    }

    Real SinTransIntegrationKernel::Argument(const Real&x, const int& iRelated) {

//     1      DEXP(-A*A*G*G)    SQPI*DEXP(-B*B/(4.*A*A))/(2.*A), WHERE
//                               SQPI="SQUARE ROOT OF PI"
//     2      1/(A*A+G*G)       (PI/2)*DEXP(-A*B)/A
//     3      DEXP(-A*G)        A/(A*A+B*B)

        switch (Integral) {
            case 1:
                return x*std::exp( -A*A*x*x );
            case 2:
                return x/(A*A+x*x);
            case 3:
                return std::exp(-A*x);
            default:
                std::cerr << "Test integral is not allowed\n";
                exit(EXIT_FAILURE);
        }
    }

    int SinTransIntegrationKernel::GetNumRel( ) {
        return 1;
    }

}		// -----  end of Lemma  name  -----
