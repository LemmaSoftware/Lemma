/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/18/2012
  @version  $Id: kernelem1dreflspec.h 123 2014-02-05 23:47:20Z tirons $
 **/

#ifndef  KERNELEM1DREFLSPEC_INC
#define  KERNELEM1DREFLSPEC_INC

#include "dipolesource.h"
#include "kernelem1dreflbase.h"
#include "layeredearthem.h"

// #include<unordered_map> // for caching results

namespace Lemma {

    // forward declare
    //struct cache;
    // Simple container to hold reflection results
    struct  cache {
        Real                       rho0;
        Real                       lambda[805];
        Real                       rams[805];
        Complex                    uk[805];
        Complex                    um[805];
        Real                       zh0i[805];
        VectorXcr                  Rtd[805];
        VectorXcr                  Rtu[805];
        VectorXcr                  u[805];
        VectorXcr                  cf[805];
        VectorXcr                  kk[805];
        const Real                 epsilon;
        //bool                       nc;
        cache( ) : epsilon (std::numeric_limits<Real>::epsilon() ) { // TODO reset to precision of Real
            //Rtd = VectorXcr::Zero(805);
            //Rtu = VectorXcr::Zero(805);
        }
    };

    // forward declaration for friend
    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    class KernelEm1DSpec;

    // ===================================================================
    //  Class:  KernelEM1DReflSpec
    /**
      @class
      \brief   Specialized version of KernelEM1DReflBase
      \details Through use of template specialisations, this KernelEm1D
               class delivers much better performance.
     */
    // ===================================================================
    template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    class KernelEM1DReflSpec : public KernelEM1DReflBase {

        public:

            template<EMMODE Mode2, int Ikernel2, DIPOLE_LOCATION Isource2, DIPOLE_LOCATION Irecv2>
            friend class KernelEm1DSpec;
            friend class KernelEM1DManager;

            // ====================  LIFECYCLE     =======================

            static KernelEM1DReflSpec* New() {
                KernelEM1DReflSpec<Mode, Isource, Irecv>*  Obj =
                    new KernelEM1DReflSpec<Mode, Isource, Irecv>("KernelEM1DReflSpec<>");
                Obj->AttachTo(Obj);
                return Obj;
            }

            void Delete() {
                this->DetachFrom(this);
            }

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            // ====================  ACCESS        =======================

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            KernelEM1DReflSpec (const std::string& name) : KernelEM1DReflBase(name)
            {
            }

            /// Default protected constructor.
            ~KernelEM1DReflSpec () {
                if (this->NumberOfReferences > 0)
                    throw DeleteObjectWithReferences( this );
            }

            void Release() {
                delete this;
            }

            // ====================  OPERATIONS    =======================

            /** Computes reflection coefficients. Depending on the
             * specialisation, this will either be TM or TE mode.
             */
            void ComputeReflectionCoeffs(const Real &lambda);

            /* Computes reflection coefficients. Depending on the
             * specialisation, this will either be TM or TE mode. This method
             * stores previous results in a struct. For a given index, and
             * lambda, the result will be the same. Turned out to be of limited utility.
             */
            //void ComputeReflectionCoeffs(const Real &lambda, const int& idx);

            /** Precomputes expensive calculations that are reused by insances
             * of KernelEM1DSpec in the calculation of Related potentials. This
             * method is specialised based on template parameters
             */
            void PreComputePotentialTerms();

            /*
             * Sets the cache in CACHE to use. Somewhat expensive, avoid calling in tight loops
             */
            //void SetTCache(const Real& rho0);

            // ====================  DATA MEMBERS  =========================

        private:

            //** Storage container for reused results */
            //static std::unordered_map<Real, cache> CACHE;

            //** Currenly used cache */
            //cache* tcache;
            //#pragma omp threadprivate(tcache)

    }; // -----  end of class  KernelEM1DReflSpec  -----

    //template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    //std::unordered_map<Real, cache> KernelEM1DReflSpec<Mode, Isource, Irecv>::CACHE;

    //template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    //cache*  KernelEM1DReflSpec<Mode, Isource, Irecv>::tcache;

    ///////////////////////////////////////////////
    // Declarations of specialisations

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INAIR>::ComputeReflectionCoeffs(const Real& lambda);

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INAIR>::ComputeReflectionCoeffs(const Real& lambda);

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INGROUND>::ComputeReflectionCoeffs(const Real& lambda);

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INGROUND>::ComputeReflectionCoeffs(const Real& lambda);

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INAIR>::PreComputePotentialTerms( );

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INAIR>::PreComputePotentialTerms( );

    template<>
    void KernelEM1DReflSpec<TM, INAIR, INGROUND>::PreComputePotentialTerms( );

    template<>
    void KernelEM1DReflSpec<TE, INAIR, INGROUND>::PreComputePotentialTerms( );

    ///////////////////////////////////////////////
    // Default mode definitions

/*
    template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    void KernelEM1DReflSpec<Mode, Isource, Irecv>::SetTCache(const Real& rho0) {
       #ifdef LEMMAUSEOMP
        #pragma omp critical
        #endif
        {
        this->tcache = &this->CACHE[rho0];
        }
    }
*/

/*
    template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    void KernelEM1DReflSpec<Mode, Isource, Irecv>::ComputeReflectionCoeffs(const Real& lambda, const int& idx) {

        if ( (std::abs(this->tcache->lambda[idx]-lambda) <= this->tcache->epsilon) &&
            this->tcache->u[idx].size() > 0  && std::abs(this->kk(0) - this->tcache->kk[idx](0)) <= this->tcache->epsilon ) {
            //std::cout << "USING CACHED RESULTS !!!!!!" << std::endl;
            // load all the values we need
            this->u   = this->tcache->u[idx];
            this->rams = this->tcache->rams[idx];
            this->cf = this->tcache->cf[idx];
            //this->kk = this->tcache->kk[idx];
            this->uk = this->tcache->uk[idx];
            this->um = this->tcache->um[idx];
            this->rtd = this->tcache->Rtd[idx];
            this->rtu = this->tcache->Rtu[idx];

        } else { // else do the work
        }

        ComputeReflectionCoeffs(lambda);

        //#pragma omp critical
        //{
        //std::cout << idx << "\t" << lambda << "\t" << rtd.transpose() << std::endl;
        //}
        // store the results
        this->tcache->u[idx]      = this->u;
        this->tcache->cf[idx]     = this->cf;
        this->tcache->kk[idx]     = this->kk;
        this->tcache->uk[idx]     = this->uk;
        this->tcache->um[idx]     = this->um;
        this->tcache->Rtd[idx]    = this->rtd;
        this->tcache->Rtu[idx]    = this->rtu;
        this->tcache->zh0i[idx]    = std::imag(this->zh[0]);
        this->tcache->lambda[idx] = lambda;
        this->tcache->rams[idx]   = rams;

        }

        return;
    }
*/

    template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    void KernelEM1DReflSpec<Mode, Isource, Irecv>::ComputeReflectionCoeffs(const Real& lambda) {
        static bool called = false;
        if (!called) {
            std::cout << "unspecialised Reflection function KernelEM1DReflSpec<"
                      << Mode << ", " << Isource << ", "
                      << Irecv << " >::ComputeReflectionCoeffs( const Real& lambda ) --> SLOW PERFORMANCE EXPECTED\n";
            called = true;
        }

        rams = lambda*lambda;
        //////////////////////////////////////////
        // Compute TEM stuff

        // This call to sqrt takes ~ 15% of execution time
        u  = (rams-kk.array()).sqrt();
        uk = u(lays);
        um = u(layr);

        switch (Mode) {

            // TM mode
            case (TM):
                Zyu(1) = -u(0)/yh(0);
                Zyi    =  u.array() / yh.array();
                break;

            // TE mode
            case (TE):
                Zyu(1) = -u(0)/zh(0);
                Zyi    =  u.array() / zh.array();
                break;

            default:
                throw 11; //IllegalMode(this);
        }

        Zyd.tail<1>() = Zyi.tail<1>();

        for (int ilay=1; ilay<nlay-1; ++ilay) {
            cf(ilay) =
                std::exp(-(Real)(2.)*u(ilay)*LayerThickness(ilay));
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

        if (layr < lays) {
            // Receiver above source layer
            int ls = layr;
            if (ls == 0) {
                ls = layr+1;
            }
            for (int N=ls; N<=lays; ++N) {
                rtu(N)= (Zyi(N)+Zyu(N)) /
                        (Zyi(N)-Zyu(N)) ;
            }
            if (lays < nlay-1) {
                rtd(lays) = (Zyi(lays)-Zyd(lays+1)) /
                            (Zyi(lays)+Zyd(lays+1)) ;
            }
        } else {
            // RECEIVER IN OR BELOW THE SOURCE LAYER
            if (lays == layr) {  // Rx In source Layer
                if (layr == 0) {
                    rtd(0) = (Zyu(1)+Zyd(1)) /
                             (Zyu(1)-Zyd(1)) ;
                } else if (layr == nlay-1) {
                    rtu(nlay-1) = (Zyi(nlay-1)+Zyu(nlay-1)) /
                                  (Zyi(nlay-1)-Zyu(nlay-1)) ;
                } else {
                    rtu(layr) = (Zyi(layr)+Zyu(layr)) /
                                (Zyi(layr)-Zyu(layr)) ;
                    rtd(layr) = (Zyi(layr)-Zyd(layr+1)) /
                                (Zyi(layr)+Zyd(layr+1)) ;
                }
            } else { // receiver below source layer
                if (lays == 0) {
                    rtd(0) = (Zyu(1)+Zyd(1)) /
                             (Zyu(1)-Zyd(1)) ;
                } else {
                    rtu(lays) = (Zyi(lays)+Zyu(lays)) /
                                (Zyi(lays)-Zyu(lays)) ;
                }
                int le = layr;
                if (le == nlay-1) --le;
                int ls = lays;
                if (lays == 0   ) ++ls;

                // TODO use blocks to vectorize maybe?
                // This works but gives same to slightly worse
                // performance as loop.
//                 int nn = le-ls+1;
//                 rtd.segment(ls, nn) =
//                      (Zyi.segment(ls  , nn).array() -
//                       Zyd.segment(ls+1, nn).array()).array() /
//                      (Zyi.segment(ls  , nn).array() +
//                       Zyd.segment(ls+1, nn).array()).array() ;
                for (int N=ls; N<=le; ++N) {
                    rtd(N) = (Zyi(N)-Zyd(N+1)) /
                             (Zyi(N)+Zyd(N+1)) ;
                }
            }
        } // End in or below source layer
        return;
    }

    template<EMMODE Mode, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    void KernelEM1DReflSpec<Mode, Isource, Irecv>::PreComputePotentialTerms( ) {
        static bool called = false;
        if (!called) {
            std::cerr << "unspecialised function KernelEM1DReflSpec<"
                      << Mode << ", " << Isource << ", "
                      << Irecv << " >::PreComputePotentialTerms\n";
            called = true;
        }
    }


}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef KERNELEM1DREFLSPEC_INC  -----
