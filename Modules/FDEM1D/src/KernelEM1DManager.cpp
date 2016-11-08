/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/26/2012
 **/

#include "KernelEM1DManager.h"

namespace Lemma {

    std::ostream &operator<<(std::ostream &stream, const KernelEM1DManager &ob) {
        stream << ob.Serialize() << "\n---\n";
        return stream;
    }


    // ====================  LIFECYCLE     =======================

    KernelEM1DManager::KernelEM1DManager( const ctor_key& ) : LemmaObject( ), TEReflBase(nullptr),
        TMReflBase(nullptr), Earth(nullptr), Dipole(nullptr) {
    }

    KernelEM1DManager::~KernelEM1DManager() {

    }

    std::shared_ptr<KernelEM1DManager> KernelEM1DManager::NewSP() {
        return std::make_shared<KernelEM1DManager>( ctor_key() );
    }


    // ====================  ACCESS        =======================

    // A race condition can develop with these. It's OK to not attach, since this is an internal class, and
    // we know what is going on.
    void KernelEM1DManager::SetEarth( std::shared_ptr<LayeredEarthEM> Earthin) {
        Earth = Earthin;
    }

    void KernelEM1DManager::SetDipoleSource( std::shared_ptr<DipoleSource> DipoleIn, const int& ifreqin,
            const Real& rx_zin) {
        Dipole = DipoleIn;

        ifreq = ifreqin;
        rx_z = rx_zin;
    }

    std::shared_ptr<KernelEM1DBase> KernelEM1DManager::GetKernel(const unsigned int& ik) {
        return KernelVec[ik];
    }

    KernelEM1DBase* KernelEM1DManager::GetRAWKernel(const unsigned int& ik) {
        return KernelVec[ik].get();
    }

    std::shared_ptr<DipoleSource> KernelEM1DManager::GetDipole( ) {
        return Dipole;
    }

    // ====================  OPERATIONS    =======================

    void KernelEM1DManager::ComputeReflectionCoeffs(const Real& lambda, const int& idx, const Real& rho0) {

        if (TEReflBase != nullptr) {
            TEReflBase->ComputeReflectionCoeffs(lambda);
            TEReflBase->PreComputePotentialTerms( );
        }

        if (TMReflBase != nullptr) {
            TMReflBase->ComputeReflectionCoeffs(lambda);
            TMReflBase->PreComputePotentialTerms( );
        }

    }

    void KernelEM1DManager::ResetSource(const int& ifreq) {

        if (TEReflBase != nullptr) {
            TEReflBase->SetUpSource(Dipole, ifreq);
        }

        if (TMReflBase != nullptr) {
            TMReflBase->SetUpSource(Dipole, ifreq);

        }
    }

}		// -----  end of Lemma  name  -----
