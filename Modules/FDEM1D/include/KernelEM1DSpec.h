/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/16/2012
 **/

#ifndef  KERNELEM1DSPEC_INC
#define  KERNELEM1DSPEC_INC

#include "KernelEM1DBase.h"
#include "KernelEM1DReflSpec.h"

#include "KernelEM1DReflBase.h"
#include "LayeredEarthEM.h"
#include "DipoleSource.h"

namespace Lemma {

    //class KernelEM1DReflBase;

    // ===================================================================
    //  Class:  KernelEM1DSpec
    /**
      \ingroup FDEM1D
      \brief   Optimized version of KernelEm1D
      \details Through use of template specialisations, this KernelEm1D
               class delivers much better performance.
      \note    This class is internal and cannot be serialized.
     */
    // ===================================================================
    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    class KernelEM1DSpec : public KernelEM1DBase {

        public:

            // ====================  LIFECYCLE     =======================

            /// Default locked constructor.
            explicit KernelEM1DSpec (const ctor_key& key ) : KernelEM1DBase( key ), ReflCalc(nullptr) {
            }

            /// Default destructor.
            ~KernelEM1DSpec () {
            }

            /** Returns a pointer to a new object of type KernelEM1DSpec.
             * It allocates all necessary memory.
             */
            static std::shared_ptr<KernelEM1DSpec> NewSP() {
                return std::make_shared< KernelEM1DSpec > ( ctor_key() );
            }

            /* Management has shifted from this approach, this will be removed
            static std::shared_ptr<KernelEM1DSpec> NewSP2(LayeredEarthEM* Earth, std::shared_ptr<DipoleSource> Dipole,
                                                         const int& ifreq, const Real& rz) {
                auto Obj = std::make_shared< KernelEM1DSpec > ( ctor_key() );

                // under this scenario KernelEM1DSpec manages its own Refl Base
                Obj->ReflCalc = KernelEM1DReflSpec<Mode, Isource, Irecv>::NewSP();
                Obj->ReflCalc->Initialise(Earth);
                Obj->ReflCalc->SetUpSource(Dipole, ifreq);
                Obj->ReflCalc->SetUpReceiver( rz );

                return Obj;
            }
            */

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

			/** Returns the Complex bessel argument to be evaluated for a given
			 * lambda value. This is specialised for each kernel type.
             */
            Complex BesselArg(const Real& lambda);

            /** Returns the Complex bessel argument to be evaluated for a given
			 * lambda value. This is used in the calculation of related kernels.
			 * This function does not call
			 * KernelEM1DReflBase->ComputeReflectionCoeffs() Which saves
			 * significant cost. This is specialised for each kernel type.
             */
            Complex RelBesselArg(const Real& lambda);

            // ====================  ACCESS        =======================

            void SetIk(const int& ikin) {
                std::cerr << "deprecated (SetIk in KernelEm1dSpec.h)\n";
                exit(EXIT_FAILURE);
            }

            void SetMode(const EMMODE& modein) {
                //ReflCalc->mode = modein;
                std::cerr << "deprecated (SetMode in KernelEM1DSpec.h)\n";
                exit(EXIT_FAILURE);
            }

            void SetReflBase( std::shared_ptr<KernelEM1DReflBase> Base ) {
                ReflCalc = Base;
            }

            // ====================  INQUIRY       =======================

            int GetNumRel() {
                std::cerr << "deprecated GetNumRel() (in KernelEM1DSpec.h) ";
                return -1;
            }

            int GetBesselOrder();


            Complex GetZm() {
                return ReflCalc->GetZm();
            }

            Complex GetYm() {
                return ReflCalc->GetYm();
            }

            Complex GetZs() {
                return ReflCalc->GetZs();
            }

            Complex GetKs() {
                return ReflCalc->GetKs();
            }

            /** Returns the name of the underlying class, similiar to Python's type */
            virtual std::string GetName() const;

        protected:

            // ====================  OPERATIONS    =======================

            /// Calculates the potential when the receiver is above the source
			/// layer
			Complex PotentialAboveSourceLayer(const Real &ra);

			/// Calculates the potential when the receiver is below the source
			/// layer
            Complex PotentialBelowSourceLayer(const Real &ra);

            /// Calculates the potential when the receiver is below the source
			/// layer
            Complex RelPotentialBelowSourceLayer(const Real &ra);

            /// Used for related kernels. Stores expensive exp(Complex) values
            Complex PotentialInSourceLayer(const Real &ra);

            /// Used for related kernels. Stores expensive exp(Complex) values
            Complex RelPotentialInSourceLayer(const Real &ra);

            // ====================  DATA MEMBERS  =========================

            /// Make these static consts
			static const Eigen::Matrix<int, 13, 1>      JD;

			static const Eigen::Matrix<Real, 4, 4>      SS_SN;

			static const Eigen::Matrix<Real, 4, 4>      SR_SN;

			static const Eigen::Matrix<Real, 4, 4>      RS_SN;

			static const Eigen::Matrix<Real, 4, 2>      SS_SL;

            std::shared_ptr<KernelEM1DReflBase>         ReflCalc;

        private:

            static constexpr auto CName = "KernelEM1DSpec";

    }; // -----  end of class  KernelEM1DSpec  -----

    // ================  INITIALISE STATIC CONSTS ==========================

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    const Eigen::Matrix<int, 13, 1>  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::JD =
    ( Eigen::Matrix<int, 13, 1>()  <<  4, 4, 1, 1, 3, 3, 3, 2, 2, 1, 2, 1, 1 ).finished();

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    const Eigen::Matrix<Real, 4, 4>  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::SS_SN =
    ( Eigen::Matrix<Real, 4, 4>()  <<
                 1.e0,  1.e0, 1.e0,  1.e0,
                -1.e0, -1.e0, 1.e0,  1.e0,
                -1.e0,  1.e0, 1.e0, -1.e0,
                 1.e0, -1.e0, 1.e0, -1.e0 ).finished();

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    const Eigen::Matrix<Real, 4, 4>  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::SR_SN =
    ( Eigen::Matrix<Real, 4, 4>()  <<
                 1.e0,  1.e0, 1.e0,  1.e0,
                -1.e0, -1.e0, 1.e0,  1.e0,
                 1.e0, -1.e0, 1.e0, -1.e0,
                -1.e0,  1.e0, 1.e0, -1.e0 ).finished();

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    const Eigen::Matrix<Real, 4, 4>  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::RS_SN =
    ( Eigen::Matrix<Real, 4, 4>()  <<
                 1.e0,  1.e0,  1.e0, 1.e0,
                -1.e0, -1.e0,  1.e0, 1.e0,
                -1.e0,  1.e0, -1.e0, 1.e0,
                 1.e0, -1.e0, -1.e0, 1.e0 ).finished();

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    const Eigen::Matrix<Real, 4, 2>  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::SS_SL =
    ( Eigen::Matrix<Real, 4, 2>()  <<
                1.e0,  1.e0,
               -1.e0,  1.e0,
                1.e0, -1.e0,
               -1.e0, -1.e0 ).finished();

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    std::string KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::GetName() const {
        return CName;
    }

    ///////////////////////////////////////////////
    // Declarations of specialisations for private fuctions
    // (some compilers seem to need these, or defaults are fallen back on)

    /* Bessel Args */

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::BesselArg( const Real& lambda );

    template <>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::RelBesselArg( const Real& lambda );

    /* Get Bessel Order */

    template <>
    int KernelEM1DSpec<TM, 0, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 1, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 2, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 3, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 4, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 5, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 6, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 7, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 8, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 9, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 10, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 11, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 12, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 0, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 1, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 2, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 3, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 4, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 5, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 6, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 7, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 8, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 9, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 10, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 11, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 12, INAIR, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 0, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 1, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 2, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 3, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 4, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 5, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 6, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 7, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 8, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 9, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 10, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 11, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 12, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 0, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 1, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 2, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 3, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 4, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 5, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 6, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 7, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 8, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 9, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 10, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 11, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 12, INAIR, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 0, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 1, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 2, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 3, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 4, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 5, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 6, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 7, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 8, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 9, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 10, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 11, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 12, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 0, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 1, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 2, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 3, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 4, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 5, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 6, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 7, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 8, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 9, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 10, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 11, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 12, INGROUND, INGROUND>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 0, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 1, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 2, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 3, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 4, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 5, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 6, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 7, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 8, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 9, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 10, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 11, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TM, 12, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 0, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 1, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 2, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 3, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 4, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 5, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 6, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 7, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 8, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 9, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 10, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 11, INGROUND, INAIR>::GetBesselOrder( );

    template <>
    int KernelEM1DSpec<TE, 12, INGROUND, INAIR>::GetBesselOrder( );

    /* POTENTIAL CALCULATIONS */

    /* Potential in air source layer*/
    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    /* Potential in air source layer TE*/
    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::PotentialInSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INAIR>::RelPotentialInSourceLayer(const Real &ra);

    /* Potential below source in air*/

    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 0, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 1, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 2, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 3, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 4, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 5, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 6, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 7, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 8, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 9, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 10, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 11, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TM, 12, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 0, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 1, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 2, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 3, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 4, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 5, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 6, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 7, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 8, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 9, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 10, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 11, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::PotentialBelowSourceLayer(const Real &ra);

    template<>
    Complex KernelEM1DSpec<TE, 12, INAIR, INGROUND>::RelPotentialBelowSourceLayer(const Real &ra);

    /////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////
    // Default mode definitions

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::BesselArg(const Real& lambda) {
        static bool called = false;
        if (!called) {
            std::cout << "Unspecialised KernelEM1DSpec::BesselArg() <" << Mode << " " << Ikernel << " "
                      << Isource << " " << Irecv << ">...slow" << std::endl;
            called = true;
        }

        ///////////////////////////////////////////////
        // Compute reflection coeffs (4) ways
        // 1) Member function pointer
        // 2) Fast Delegates way
        // 3) Boost::function and boost::bind way
        // 4) No function pointers, all logic @ each step.
        // For (4) about 40% of time is spent in this loop.
        // However, none of the other options are producing
        // apreciably faster code.

        // Function pointers used to specilized cases
        //(this->*ComputeReflectionCoeffs)( );

        // Boost::bind way and fast delegates way
        //ComputeReflectionCoeffs();
        //ComputeReflectionCoeffsTempl<TE>();


        // Don't use function pointers, ~ same speed right now :(
        ReflCalc->ComputeReflectionCoeffs(lambda);

        ReflCalc->id = this->JD(Ikernel);

        // If these params are equal they share reflection coeffs
        //  + layr
        // ++ ifirst;

        Real ra=1.0;
        int i1=(Ikernel  )*(Ikernel- 2)*(Ikernel- 5)*(Ikernel- 7);
        int i2=(Ikernel-4)*(Ikernel- 9)*(Ikernel-10)*(Ikernel-12);

        if(i1 == 0)  ra=lambda;
        if(i2 == 0)  ra=ReflCalc->rams;
        if(Ikernel == 11) ra=lambda*ReflCalc->rams;
        Complex pot;

        if (ReflCalc->lays == ReflCalc->layr) {
            pot = PotentialInSourceLayer(ra);
        } else if (ReflCalc->layr > ReflCalc->lays) {
            pot = PotentialBelowSourceLayer(ra);
        } else {
            pot = PotentialAboveSourceLayer(ra);
        }

        return pot/ReflCalc->uk;

    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    int KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::GetBesselOrder( ) {
        std::cerr << "Calling base GetBesselOrder in KernelEM1DSpec < " << Mode
        << "\t" << Ikernel << "\t" << Isource << "\t" << Irecv <<  "\n";
        exit(EXIT_FAILURE);
        return 1;
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::RelBesselArg(const Real& lambda) {
        static bool called = false;
        if (!called) {
            std::cout << "Unspecialised KernelEM1DSpec::RelBesselArg <" << Mode << " " << Ikernel << " "
                      << Isource << " " << Irecv << ">...slow" << std::endl;
            called = true;
        }
        return this->BesselArg(lambda);
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex  KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::PotentialAboveSourceLayer(const Real &ra) {
        static bool called = false;
        if (!called) {
            std::cout << "WARNING: ";
            std::cout << "Unspecialised PotentialAboveSourceLayer <" << Mode << " " << Ikernel << " "
                  << Isource << " " << Irecv << ">...this function will be slow\n\n";
            called = true;
        }
        Complex  ud;
        switch (ReflCalc->id) {
            case (2):
                ud = ReflCalc->um;
                break;
            case (3):
                ud = ReflCalc->uk;
                break;
            case (4):
                ud = ReflCalc->um*ReflCalc->uk;
                break;
            default:
                ud = Complex(1,0);
        }
        int I1 = (Ikernel   )*(Ikernel- 1)*(Ikernel- 4)*(Ikernel- 7)*
                 (Ikernel- 8)*(Ikernel- 9)*(Ikernel-10)*(Ikernel-11);
        Complex CC, DD(1,0);
        if (Mode==TM && ReflCalc->layr==0 && I1==0) {
            CC = (Real)(2.) * ReflCalc->rtu(ReflCalc->lays)*ReflCalc->u(1)*ReflCalc->yh(0) /
                    (ReflCalc->yh(0)*ReflCalc->u(1)-ReflCalc->yh(1)*ReflCalc->u(0));
        } else {
            CC = ReflCalc->rtu(ReflCalc->lays)+ReflCalc->rtu(ReflCalc->lays)/ReflCalc->rtu(ReflCalc->layr+1);
        }
        if (ReflCalc->lays < ReflCalc->nlay) {
             DD -= ReflCalc->rtu(ReflCalc->lays) * ReflCalc->rtd(ReflCalc->lays) * ReflCalc->cf(ReflCalc->lays)  ;
        }
        if (ReflCalc->layr > 0) {
            DD *= ( (Real)(1.) + ReflCalc->rtu(ReflCalc->layr)*ReflCalc->cf(ReflCalc->layr));
        }
        Complex A = CC/DD;
        int LS = ReflCalc->layr + 1;
        int LE = ReflCalc->lays - 1;
        if (LE >= LS) {
            for (int N=LS; N<=LE; ++N) {
                A *= (ReflCalc->rtu(N)+ReflCalc->rtu(N)/ReflCalc->rtu(N+1)) /
                        ( (Real)(1.)+ReflCalc->rtu(N)*ReflCalc->cf(N));
            }
        }
        Complex P(0);
        Complex CON(0);

        if (ReflCalc->layr == 0) {
            P = - ReflCalc->u(0)*ReflCalc->rx_z;
            if (LE > 0) {
                for (int N=1; N<=LE; ++N) {
                    P += (Real)(2.)*ReflCalc->u(N)*ReflCalc->LayerThickness(N)+(ReflCalc->u(N+1)-ReflCalc->u(N))*ReflCalc->LayerDepth(N);
                }
            }
            CON = RS_SN(ReflCalc->id-1, 2) * std::exp(-P-ReflCalc->uk*(ReflCalc->tx_z-(Real)(2.)*ReflCalc->LayerDepth(ReflCalc->lays-1)));
            if (ReflCalc->lays < ReflCalc->nlay-1) {
                CON += RS_SN(ReflCalc->id-1, 3)*ReflCalc->rtd(ReflCalc->lays) *
                        std::exp(-P-ReflCalc->uk*((Real)(2.)*ReflCalc->LayerThickness(ReflCalc->lays)-ReflCalc->tx_z));
            }
        } else {
            for (int N=ReflCalc->layr; N<=LE; ++N) {
                P += (Real)(2.)*ReflCalc->u(N)*ReflCalc->LayerThickness(N)+(ReflCalc->u(N+1)-ReflCalc->u(N))*ReflCalc->LayerDepth(N);
            }
            CON = RS_SN(ReflCalc->id-1, 0)*ReflCalc->rtu(ReflCalc->layr) *
                std::exp(-P-ReflCalc->uk*(ReflCalc->tx_z-(Real)(2.)*ReflCalc->LayerDepth(ReflCalc->lays-1))-ReflCalc->um*ReflCalc->rx_z) +
                RS_SN(ReflCalc->id-1, 2) *
                std::exp(-P-ReflCalc->uk*(ReflCalc->tx_z-(Real)(2.)*ReflCalc->LayerDepth(ReflCalc->lays-1))-
                    ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr-1)-ReflCalc->rx_z));
            if (ReflCalc->layr < ReflCalc->nlay-1) {
                CON += ReflCalc->rtd(ReflCalc->lays)*(RS_SN(ReflCalc->id-1,1)*ReflCalc->rtu(ReflCalc->layr) *
                    std::exp(-P-ReflCalc->uk*( (Real)(2.)*ReflCalc->LayerThickness(ReflCalc->lays)-ReflCalc->tx_z)-
                            ReflCalc->um*ReflCalc->rx_z) + RS_SN(ReflCalc->id-1,3)*
                    std::exp(-P-ReflCalc->uk*( (Real)(2.)*ReflCalc->LayerThickness(ReflCalc->lays)-ReflCalc->tx_z)-
                            ReflCalc->um*( (Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr-1)-ReflCalc->rx_z)));
            }
        }

        return ra*A*CON*ud;
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::RelPotentialBelowSourceLayer(const Real &ra) {
        static bool called = false;
        if (!called) {
            std::cout << "WARNING\n";
            std::cout << "Unspecialised PotentialBelowSourceLayer <" << Mode << " " << Ikernel << " "
                  << Isource << " " << Irecv << ">...this function will be slow\n\n";
            called = true;
        }
        return PotentialBelowSourceLayer(ra);
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::PotentialBelowSourceLayer(const Real &ra) {
        static bool called = false;
        if (!called) {
            std::cout << "WARNING\n";
            std::cout << "Unspecialised PotentialBelowSourceLayer <" << Mode << " " << Ikernel << " "
                  << Isource << " " << Irecv << ">...this function will be slow\n\n";
            called = true;
        }
        Complex ud;
        switch (ReflCalc->id) {
            case 2:
                ud = ReflCalc->um;
                break;
            case 3:
                ud = ReflCalc->uk;
                break;
            case 4:
                ud = ReflCalc->um*ReflCalc->uk;
                break;
            default:
                ud = 1.;
        }
        Complex dd(1.,0);
        if (ReflCalc->lays > 0) {
            dd -= ReflCalc->rtu(ReflCalc->lays)*ReflCalc->rtd(ReflCalc->lays)*ReflCalc->cf(ReflCalc->lays);
        }
        if (ReflCalc->lays < ReflCalc->nlay) {
            dd *= ((Real)(1.)+ReflCalc->rtd(ReflCalc->lays+1)*ReflCalc->cf(ReflCalc->lays+1));
        }
        Complex a = ((Real)(1.) + ReflCalc->rtd(ReflCalc->lays)) / dd;
        if (ReflCalc->layr >= ReflCalc->lays+2) {
            for (int n=ReflCalc->lays+2; n<=ReflCalc->layr; ++n) {
                a *= ((Real)(1.)+ReflCalc->rtd(n-1));
                if (n < ReflCalc->nlay-1) {
                    a /= ((Real)(1.)+ReflCalc->rtd(n)*ReflCalc->cf(n));
                }
            }
        }
        Complex p(0,0);
        for (int n=ReflCalc->lays+1; n<=ReflCalc->layr; ++n) {
            Complex ut = ReflCalc->u(0);
            if (n>1) {
                ut = ReflCalc->u(n-1);
            }
            p += (ReflCalc->u(n)-ut) * ReflCalc->LayerDepth(n-1);
        }
        Complex con = SR_SN(ReflCalc->id-1, 0) * std::exp(ReflCalc->uk*ReflCalc->tx_z - ReflCalc->um*ReflCalc->rx_z + p);
        if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
            con += SR_SN(ReflCalc->id-1, 2) * ReflCalc->rtd(ReflCalc->layr) * std::exp(ReflCalc->uk*ReflCalc->tx_z-
                        ReflCalc->um*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
        }
        if (ReflCalc->lays > 0) {
            con += SR_SN(ReflCalc->id-1, 1) * ReflCalc->rtu(ReflCalc->lays)*std::exp(ReflCalc->uk*( (Real)(2.)*
                        ReflCalc->LayerDepth(ReflCalc->lays-1)-ReflCalc->tx_z)-ReflCalc->um*ReflCalc->rx_z+p);
            if (ReflCalc->layr < ReflCalc->Earth->GetNumberOfLayers()-1) {
                con += SR_SN(ReflCalc->id-1, 3) * ReflCalc->rtu(ReflCalc->lays)*ReflCalc->rtd(ReflCalc->layr) *
                    std::exp(ReflCalc->uk*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->lays-1)-ReflCalc->tx_z)-ReflCalc->um*
                    ((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->layr)-ReflCalc->rx_z)+p);
            }
        }
        return ra*a*con*ud;
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::RelPotentialInSourceLayer(const Real &ra) {
        static bool called = false;
        if (!called) {
            std::cout << "Unspecialised KernelEM1DSpec::RelPotentialInSourceLayer() <" << Mode << " " << Ikernel << " "
                      << Isource << " " << Irecv << ">...slow" << std::endl;
            called = true;
        }
        return PotentialInSourceLayer(ra);
    }

    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    Complex KernelEM1DSpec<Mode, Ikernel, Isource, Irecv>::PotentialInSourceLayer(const Real &ra) {
        static bool called = false;
        if (!called) {
            std::cout << "WARNING\n";
            std::cout << "Unspecialised PotentialInSourceLayer <" << Mode << " " << Ikernel << " "
                  << Isource << " " << Irecv << ">...this function will be slow\n\n";
            called = true;
        }
        int iud(0);
        if (ReflCalc->rx_z <= ReflCalc->tx_z) iud=1;
        Real adz = std::abs(ReflCalc->rx_z - ReflCalc->tx_z);
        Complex con;
        Complex ud;
        switch (ReflCalc->id) {
            case (1):
                ud = 1.;
                break;
            case (4):
                ud = ReflCalc->uk*ReflCalc->uk;
                break;
            default:
                ud = ReflCalc->uk;
        }
        if (ReflCalc->lays == 0) {
            con = ReflCalc->rtd(0)*std::exp(ReflCalc->u(0)*(ReflCalc->rx_z+ReflCalc->tx_z));
        } else {
            if (ReflCalc->lays == ReflCalc->Earth->GetNumberOfLayers() - 1) {
                con = SS_SN(ReflCalc->id-1, 0)*ReflCalc->rtu(ReflCalc->nlay-1)*std::exp(ReflCalc->u(ReflCalc->nlay-1) *
                        ((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->nlay-2)-ReflCalc->rx_z-ReflCalc->tx_z));
            } else {
                con = ReflCalc->rtu(ReflCalc->lays)*(SS_SN(ReflCalc->id-1,0)*
                    std::exp(ReflCalc->uk*((Real)(2.)*ReflCalc->LayerDepth(ReflCalc->lays-1)-ReflCalc->rx_z-ReflCalc->tx_z))
                    + SS_SN(ReflCalc->id-1, 1)*ReflCalc->rtd(ReflCalc->lays)
                    * std::exp(ReflCalc->uk*(ReflCalc->tx_z-ReflCalc->rx_z-(Real)(2.)* ReflCalc->LayerThickness(ReflCalc->lays))))
                    + ReflCalc->rtd(ReflCalc->lays)
                    * ( SS_SN(ReflCalc->id-1, 2)
                        * std::exp(ReflCalc->uk*(ReflCalc->rx_z+ReflCalc->tx_z-(Real)(2.) * ReflCalc->LayerDepth(ReflCalc->lays)))
                        + SS_SN(ReflCalc->id-1, 3)*ReflCalc->rtu(ReflCalc->lays)
                        * std::exp(ReflCalc->uk*(ReflCalc->rx_z-ReflCalc->tx_z-(Real)(2.) * ReflCalc->LayerThickness(ReflCalc->lays)))
                      ) ;
                con /= ((Real)(1.)-ReflCalc->rtu(ReflCalc->lays)*ReflCalc->rtd(ReflCalc->lays)*ReflCalc->cf(ReflCalc->lays)) ;
            }
        }
        // Add singular term (source term)
        con += SS_SL(ReflCalc->id-1, iud)*std::exp(-ReflCalc->uk*adz);
        return ra*ud*con;
    }

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef KERNELEM1DSPEC_INC  -----
