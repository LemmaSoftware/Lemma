/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/18/2012
 **/

#include "KernelEM1DReflSpec.h"

namespace Lemma {

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INAIR>::ComputeReflectionCoeffs(const Real& lambda) {

        rams = lambda*lambda;
        u = (rams-kk.array()).sqrt(); // CRITICAL

        uk = u(0);
        um = u(0);

        Zyu(1) = -u(0)/yh(0);
        Zyi    =  u.array() / yh.array();

        Zyd.tail<1>() = Zyi.tail<1>();

        // Vectorise, performance benchmarks approx. the same as loop w gcc 9 -TI
        cf.segment(1,nlay-2) = (-2.*u.segment(1, nlay-2).array() * LayerThickness.segment(1, nlay-2).array()).exp();
        th.segment(1,nlay-2) = (1.-cf.segment(1, nlay-2).array()) / (1.+cf.segment(1, nlay-2).array());

//         for (int ilay=1; ilay<nlay-1; ++ilay) {
//             cf(ilay) = std::exp(-(Real)(2.)*u(ilay)*LayerThickness(ilay));
//             th(ilay) = ((Real)(1.)-cf(ilay)) / ((Real)(1.)+cf(ilay));
//         }

        // recursive, can't vectorize
        for (int N=nlay-2; N >= 1; --N) {
            Zyd(N) = Zyi(N)*(Zyd(N+1)+Zyi(N)*th(N)) / (Zyi(N)+Zyd(N+1)*th(N)) ;
        }

        //rtd(nlay-1) = 0;
        rtd(0) = (Zyu(1)+Zyd(1)) / (Zyu(1)-Zyd(1)) ;

        return;
    }

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INAIR>::ComputeReflectionCoeffs(const Real& lambda) {

        rams = lambda*lambda;
        u = (rams-kk.array()).sqrt(); // CRITICAL

        uk = u(0);
        um = u(0);

        Zyu(1) = -u(0)/zh(0);
        Zyi    =  u.array() / zh.array();

        Zyd.tail<1>() = Zyi.tail<1>();

        // Vectorise, performance benchmarks approx. the same as loop w/ gcc 9 -TI
        cf.segment(1,nlay-2) = (-2.*u.segment(1, nlay-2).array() * LayerThickness.segment(1, nlay-2).array()).exp();
        th.segment(1,nlay-2) = (1.-cf.segment(1, nlay-2).array()) / (1.+cf.segment(1, nlay-2).array());

//         for (int ilay=1; ilay<nlay-1; ++ilay) {
//             cf(ilay) = std::exp(-(Real)(2.)*u(ilay)*LayerThickness(ilay));
//             th(ilay) = ((Real)(1.)-cf(ilay)) / ((Real)(1.)+cf(ilay));
//         }

        // recursive, can't vectorize
        for (int N=nlay-2; N >=1; --N) {
            Zyd(N) = Zyi(N)*(Zyd(N+1)+Zyi(N)*th(N)) / (Zyi(N)+Zyd(N+1)*th(N)) ;
        }

        //rtd(nlay-1) = 0;
        rtd(0) = (Zyu(1)+Zyd(1)) / (Zyu(1)-Zyd(1)) ;

        return;
    }

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INGROUND>::ComputeReflectionCoeffs(const Real& lambda) {

        rams = lambda*lambda;
        u = (rams-kk.array()).sqrt();
        uk = u(0);
        um = u(layr);

        Zyu(1) = -u(0)/yh(0);
        Zyi    =  u.array() / yh.array();

        Zyd.tail<1>() = Zyi.tail<1>();

        for (int ilay=1; ilay<nlay-1; ++ilay) {
            cf(ilay) = std::exp(-(Real)(2.)*u(ilay)*LayerThickness(ilay));
            th(ilay) = ((Real)(1.)-cf(ilay)) / ((Real)(1.)+cf(ilay));
        }

        // Can't use blocks, b/c recursive
        for (int N=1; N<lays; ++N){
            Zyu(N+1)=Zyi(N)*(Zyu(N)-Zyi(N)*th(N)) /
                            (Zyi(N)-Zyu(N)*th(N)) ;
        }

        int ne = nlay-2;
        for (int N=ne; N >=lays+1; --N) {
            Zyd(N) = Zyi(N)*(Zyd(N+1)+Zyi(N)*th(N)) /
                            (Zyi(N)+Zyd(N+1)*th(N)) ;
        }

        rtd(nlay-1) = 0;
        rtd(0) = (Zyu(1)+Zyd(1)) / (Zyu(1)-Zyd(1)) ;

        int le = layr;
        if (le == nlay-1) --le;
        for (int N=1; N<=le; ++N) {
            rtd(N) = (Zyi(N)-Zyd(N+1)) / (Zyi(N)+Zyd(N+1)) ;
        }
        return;
    }

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INGROUND>::ComputeReflectionCoeffs(const Real& lambda) {

        rams = lambda*lambda;
        u = (rams-kk.array()).sqrt();
        uk = u(0);
        um = u(layr);

        Zyu(1) = -u(0)/zh(0);
        Zyi    =  u.array() / zh.array();

        Zyd.tail<1>() = Zyi.tail<1>();

        for (int ilay=1; ilay<nlay-1; ++ilay) {
            cf(ilay) = std::exp(-(Real)(2.)*u(ilay)*LayerThickness(ilay));
            th(ilay) = ((Real)(1.)-cf(ilay)) / ((Real)(1.)+cf(ilay));
        }

        // Can't use blocks, b/c recursive
        for (int N=1; N<lays; ++N){
            Zyu(N+1)=Zyi(N)*(Zyu(N)-Zyi(N)*th(N)) /
                            (Zyi(N)-Zyu(N)*th(N)) ;
        }

        int ne = nlay-2;
        for (int N=ne; N >=lays+1; --N) {
            Zyd(N) = Zyi(N)*(Zyd(N+1)+Zyi(N)*th(N)) /
                            (Zyi(N)+Zyd(N+1)*th(N)) ;
        }

        rtd(nlay-1) = 0;
        rtd(0) = (Zyu(1)+Zyd(1)) / (Zyu(1)-Zyd(1)) ;

        int le = layr;
        if (le == nlay-1) --le;
        for (int N=1; N<=le; ++N) {
            rtd(N) = (Zyi(N)-Zyd(N+1)) / (Zyi(N)+Zyd(N+1)) ;
        }
        return;
    }

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INAIR>::PreComputePotentialTerms( ) {
        relIud = 0;
        if (rx_z<= tx_z) relIud=1;
        relCon = rtd(0)*std::exp(u(0)*(rx_z+tx_z));
        Real adz = std::abs(rx_z - tx_z);
        relenukadz = std::exp(-uk*adz); // Singular source term
    }

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INAIR>::PreComputePotentialTerms( ) {
        relIud = 0;
        if (rx_z<= tx_z) relIud=1;
        relCon = rtd(0)*std::exp(u(0)*(rx_z+tx_z));
        Real adz = std::abs(rx_z - tx_z);
        relenukadz = std::exp(-uk*adz); // Singular source term
    }

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INGROUND>::PreComputePotentialTerms( ) {
        Complex dd =  ((Real)(1.)+rtd(1)*cf(1));
        rel_a = ((Real)(1.) + rtd(0)) / dd;
        if (layr >= 2) {
            for (int n=2; n<=layr; ++n) {
                rel_a *= ((Real)(1.)+rtd(n-1));
                if (n < nlay-1) {
                    rel_a /= ((Real)(1.)+rtd(n)*cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=layr; ++n) {
            Complex ut = u(0);
            if (n>1) {
                ut = u(n-1);
            }
            p += (u(n)-ut) * LayerDepth(n-1);
        }
        relexp_pbs1 = std::exp(uk*tx_z-um*rx_z+ p);

        if (layr < Earth->GetNumberOfLayers()-1) {
            relexp_pbs2 = std::exp(uk*tx_z-um*((Real)(2.)*LayerDepth(layr)-rx_z)+p);
        }
    }

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INGROUND>::PreComputePotentialTerms( ) {
        Complex dd =  ((Real)(1.)+rtd(1)*cf(1));
        rel_a = ((Real)(1.) + rtd(0)) / dd;
        if (layr >= 2) {
            for (int n=2; n<=layr; ++n) {
                rel_a *= ((Real)(1.)+rtd(n-1));
                if (n < nlay-1) {
                    rel_a /= ((Real)(1.)+rtd(n)*cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=1; n<=layr; ++n) {
            Complex ut = u(0);
            if (n>1) {
                ut = u(n-1);
            }
            p += (u(n)-ut) * LayerDepth(n-1);
        }
        relexp_pbs1 = std::exp(uk*tx_z-um*rx_z+ p);
        if (layr < Earth->GetNumberOfLayers()-1) {
            relexp_pbs2 = std::exp(uk*tx_z-um*((Real)(2.)*LayerDepth(layr)-rx_z)+p);
        }
    }

}		// -----  end of Lemma  name  -----
