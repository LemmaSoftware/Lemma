/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     08/09/2010
  @version  $Id: quasinewtonbfgs.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "quasinewtonbfgs.h"


namespace Lemma {

    // ====================  STREAMS       =======================

    std::ostream &operator<<(std::ostream &stream,
            const QuasiNewtonBFGS &ob) {
        stream << *(InverseSolver*)(&ob);
        return stream;
    }


    // ====================  LIFECYCLE     =======================

    QuasiNewtonBFGS* QuasiNewtonBFGS::New() {
        QuasiNewtonBFGS* Object = new QuasiNewtonBFGS("QuasiNewtonBFGS");
        Object->AttachTo(Object);
        return Object;
    }

    QuasiNewtonBFGS::QuasiNewtonBFGS(const std::string &name) :
        InverseSolver(name) {
    }

    void QuasiNewtonBFGS::Delete() {
        this->DetachFrom(this);
    }

    void QuasiNewtonBFGS::Release() {
        delete this;
    }

    QuasiNewtonBFGS::~QuasiNewtonBFGS() {
        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences( this );
    }

    // ====================  INQUIRY       =======================

    int QuasiNewtonBFGS::NumberOfIterations() {
        return 0;
    }

    bool QuasiNewtonBFGS::Success () {
        return false;
    }

    void QuasiNewtonBFGS::PrintNorm() {
        std::cout << "Norm " << std::endl;
        PredictedData->Zero();
        std::cout << ObservedData->Norm(PredictedData) << std::endl;
    }

    VectorXr QuasiNewtonBFGS::GetPhiMVector() {
        VectorXr NewVec(2);
        return NewVec;
    }

    VectorXr QuasiNewtonBFGS::GetPhiDVector() {
        VectorXr NewVec(2);
        return NewVec;
    }

    void QuasiNewtonBFGS::FillInG(const Vector3r& pos, const Vector3r& step) {

    }

    // ====================  ACCESS        =======================

}		// -----  end of Lemma  name  -----
