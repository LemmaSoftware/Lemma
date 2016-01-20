/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/26/2012
  @version  $Id: kernelem1dmanager.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "kernelem1dmanager.h"


namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
                const KernelEM1DManager &ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }


    // ====================  LIFECYCLE     =======================

    KernelEM1DManager::KernelEM1DManager(const std::string& name) :
                    LemmaObject(name), TEReflBase(NULL), TMReflBase(NULL), Earth(NULL), Dipole(NULL) {
    }

    KernelEM1DManager::~KernelEM1DManager() {

        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences(this);

//         if (Earth != NULL) {
//             Earth->DetachFrom(this);
//         }
//
//         if (Dipole != NULL) {
//             Dipole->DetachFrom(this);
//         }

        for (unsigned int ik=0; ik<KernelVec.size(); ++ik) {
            KernelVec[ik]->Delete();
        }

        if (TEReflBase != NULL) TEReflBase->Delete();
        if (TMReflBase != NULL) TMReflBase->Delete();

    }

    KernelEM1DManager* KernelEM1DManager::New() {
        KernelEM1DManager* Obj = new KernelEM1DManager("KernelEM1DManager");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void KernelEM1DManager::Delete() {
        this->DetachFrom(this);
    }

    void KernelEM1DManager::Release() {
        delete this;
    }

    // ====================  ACCESS        =======================

    // A race condition can develop with these. It's OK to not attach, since this is an internal class, and
    // we know what is going on.
    void KernelEM1DManager::SetEarth(LayeredEarthEM* Earthin) {
        //if (Earth != NULL) {
        //    Earth->DetachFrom(this);
        //}
        //Earthin->AttachTo(this);
        Earth = Earthin;
    }

    void KernelEM1DManager::SetDipoleSource(DipoleSource* DipoleIn, const int& ifreqin,
            const Real& rx_zin) {
        //if (Dipole != NULL) {
        //    Dipole->DetachFrom(this);
        //}
        //DipoleIn->AttachTo(this);
        Dipole = DipoleIn;

        ifreq = ifreqin;
        rx_z = rx_zin;
    }

    KernelEm1DBase* KernelEM1DManager::GetKernel(const unsigned int& ik) {
        return KernelVec[ik];
    }

    DipoleSource* KernelEM1DManager::GetDipole( ) {
        return Dipole;
    }

    // ====================  OPERATIONS    =======================

    void KernelEM1DManager::ComputeReflectionCoeffs(const Real& lambda, const int& idx, const Real& rho0) {

        if (TEReflBase != NULL) {
            TEReflBase->ComputeReflectionCoeffs(lambda);
            TEReflBase->PreComputePotentialTerms( );
        }

        if (TMReflBase != NULL) {
            TMReflBase->ComputeReflectionCoeffs(lambda);
            TMReflBase->PreComputePotentialTerms( );
        }

    }

    void KernelEM1DManager::ResetSource(const int& ifreq) {

        if (TEReflBase != NULL) {
            TEReflBase->SetUpSource(Dipole, ifreq);
        }

        if (TMReflBase != NULL) {
            TMReflBase->SetUpSource(Dipole, ifreq);

        }
    }

}		// -----  end of Lemma  name  -----
