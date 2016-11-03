/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/17/2012
  @version  $Id: kernelem1dspec.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "kernelem1dspec.h"

namespace Lemma {

    // ====================  SPECIALISATIONS  ===============================

    ////////////////////////////////////////////////////////////////////////
    // GetBesselOrder
    /* TM INAIR INAIR */
    template <>
    int KernelEM1DSpec<TM, 0, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 1, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 2, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 3, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 4, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 5, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 6, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 7, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 8, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 9, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 10, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 11, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 12, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    /* TE INAIR INAIR */
    template <>
    int KernelEM1DSpec<TE, 0, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 1, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 2, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 3, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 4, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 5, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 6, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 7, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 8, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 9, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 10, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 11, INAIR, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 12, INAIR, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    /* TM INAIR INGROUND */
    template <>
    int KernelEM1DSpec<TM, 0, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 1, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 2, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 3, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 4, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 5, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 6, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 7, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 8, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 9, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 10, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 11, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 12, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    /* TE INAIR INGROUND */
    template <>
    int KernelEM1DSpec<TE, 0, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 1, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 2, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 3, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 4, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 5, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 6, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 7, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 8, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 9, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 10, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 11, INAIR, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 12, INAIR, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    /* TM INGROUND INAIR */
    template <>
    int KernelEM1DSpec<TM, 0, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 1, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 2, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 3, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 4, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 5, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 6, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 7, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 8, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 9, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 10, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 11, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 12, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    /* TE INGROUND INAIR */
    template <>
    int KernelEM1DSpec<TE, 0, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 1, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 2, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 3, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 4, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 5, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 6, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 7, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 8, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 9, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 10, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 11, INGROUND, INAIR>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 12, INGROUND, INAIR>::GetBesselOrder( ) {
        return 1;
    }

    /* TM INGROUND INGROUND */
    template <>
    int KernelEM1DSpec<TM, 0, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 1, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 2, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 3, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 4, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 5, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 6, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 7, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 8, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 9, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 10, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TM, 11, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TM, 12, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    /* TE INGROUND INGROUND */
    template <>
    int KernelEM1DSpec<TE, 0, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 1, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 2, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 3, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 4, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 5, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 6, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 7, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 8, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 9, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 10, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }

    template <>
    int KernelEM1DSpec<TE, 11, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 0;
    }

    template <>
    int KernelEM1DSpec<TE, 12, INGROUND, INGROUND>::GetBesselOrder( ) {
        return 1;
    }


    ////////////////////////////////////////////////////////////////////////
    // BesselArg specialisations
    // TODO eradicate all ReflCalc->id terms, these should be hard coded

    /* TM INAIR INAIR */
    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::BesselArg( const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::RelBesselArg( const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::BesselArg( const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::RelBesselArg( const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    /* TE INAIR INAIR */

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialInSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    /* TM INAIR INGROUND */

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    /* TE INAIR INGROUND */

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 4;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 4;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 3;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 3;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(lambda)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(1)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 2;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 2;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::RelBesselArg(const Real& lambda ) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(lambda*ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::BesselArg(const Real& lambda ) {
        ReflCalc->ComputeReflectionCoeffs(lambda);
        ReflCalc->id = 1;
        return PotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::RelBesselArg(const Real& lambda) {
        ReflCalc->id = 1;
        return RelPotentialBelowSourceLayer(ReflCalc->rams)/ReflCalc->uk;
    }

    ////////////////////////////////////////////////////////////////////////
    // Potential terms
    // TODO the following could be precomputed in PotentialInSourceLayer: adz,
    // iud, and con *before* source term.

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(3, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(3, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(3, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(3, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(3, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(3, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(3, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(3, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(2, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(2, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(1, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*ReflCalc->uk*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra) {
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z-ReflCalc->tx_z);
        Complex con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        // Add singular term (source term)
        con += SS_SL(0, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*con;
    }

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra) {
        Complex con = ReflCalc->relCon +  SS_SL(0, ReflCalc->relIud)*ReflCalc->relenukadz;
        return ra*con;
    }

    // TODO in PotentialBelowSourceLayer:

    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(3, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(3, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(3, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(3, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {

        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;//*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;//*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(3, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(3, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(3, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(3, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(3, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {

        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {

        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(2, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(2, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(2, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->uk;
    }

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(0, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(0, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(0, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con*ReflCalc->um;
    }

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra) {
        Complex dd =  ((Real)(1.)+ReflCalc->rtd(1)*ReflCalc->cf(1));
        Complex a = ((Real)(1.) + ReflCalc->rtd(0)) / dd;
        if (ReflCalc->layr >= 2) {
            for (int n=2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z+ p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        return ra*a*con;
    }

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra) {
        Complex con = SR_SN(ReflCalc->id-1, 0) * ReflCalc->relexp_pbs1;
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * ReflCalc->relexp_pbs2;
        }
        return ra*ReflCalc->rel_a*con;
    }

}		// -----  end of :Lemma  name  -----
