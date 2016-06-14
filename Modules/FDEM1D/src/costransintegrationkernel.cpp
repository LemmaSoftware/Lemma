/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     02/08/2011
  @version  $Id: costransintegrationkernel.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "costransintegrationkernel.h"

namespace Lemma {

    // ====================  FRIENDS       =======================

//     std::ostream &operator<<(std::ostream &stream,
// 			const DigitalFilterCosTrans &ob) {
// 		stream << *(DigitalFilterIntegrator<Real>*)(&ob);
// 		return stream;
// 	}

    // ====================  LIFECYCLE     =======================

    CosTransIntegrationKernel::
        CosTransIntegrationKernel(const std::string& name) :
        IntegrationKernel<Real>(name) {
    }

    CosTransIntegrationKernel::~CosTransIntegrationKernel() {}


    CosTransIntegrationKernel* CosTransIntegrationKernel::New() {
        CosTransIntegrationKernel* Obj =
            new CosTransIntegrationKernel("CosTransIntegrationKernel");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void CosTransIntegrationKernel::Delete() {
        this->DetachFrom(this);
    }

    void CosTransIntegrationKernel::Release() {
        delete this;
    }

    // ====================  OPERATIONS    =======================

    void CosTransIntegrationKernel::SetA(const Real& Ain) {
        A = Ain;
    }

    void CosTransIntegrationKernel::SetIntegral(const int& i) {
        Integral = i;
    }

    Real CosTransIntegrationKernel::GetAnalytical (const Real& B) {
        Real SQPI = std::sqrt(PI);
        switch (Integral) {
            case 1:
                return SQPI*std::exp(-B*B/(4.*A*A))/(2.*A);
                break;
            case 2:
                return (PI/2.)*std::exp(-A*B)/A;
                break;
            case 3:
                return A/(A*A+B*B);
                break;
            default:
                std::cerr << "Test integral is not allowed\n";
                exit(EXIT_FAILURE);
        }
    }

    Real CosTransIntegrationKernel::Argument(const Real&x, const int& iRelated) {

//     1      DEXP(-A*A*G*G)    SQPI*DEXP(-B*B/(4.*A*A))/(2.*A), WHERE
//                               SQPI="SQUARE ROOT OF PI"
//     2      1/(A*A+G*G)       (PI/2)*DEXP(-A*B)/A
//     3      DEXP(-A*G)        A/(A*A+B*B)

        switch (Integral) {
            case 1:
                return std::exp( -A*A*x*x );
            case 2:
                return 1./(A*A+x*x);
            case 3:
                return std::exp(-A*x);
            default:
                std::cerr << "Test integral is not allowed\n";
                exit(EXIT_FAILURE);
        }
    }

    int CosTransIntegrationKernel::GetNumRel( ) {
        return 1;
    }

}		// -----  end of Lemma  name  -----
