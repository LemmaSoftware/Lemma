/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     05/18/2012
 **/

#ifndef  KERNELEM1DREFLBASE_INC
#define  KERNELEM1DREFLBASE_INC

#include "KernelEM1DBase.h"
#include "DipoleSource.h"
#include "LayeredEarthEM.h"

namespace Lemma {

    enum DIPOLE_LOCATION { INAIR, INGROUND };

    // forward declaration for friend
    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    class KernelEM1DSpec;


    // ===================================================================
    //  Class:  KernelEM1DReflBase
    /**
      @class
      \brief   Abstract class defining EM1DRefl class.
      \details Derived classes are template specialized for optimal performance.
     */
    // ===================================================================
    class KernelEM1DReflBase : public LemmaObject {

        template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
        friend class KernelEM1DSpec;
        friend class KernelEM1DManager;
        //friend class DipoleSource;

        public:

            // ====================  LIFECYCLE     =======================

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            void Initialise( std::shared_ptr<LayeredEarthEM> EarthIn) {

                nlay = EarthIn->GetNumberOfLayers();
                zh = VectorXcr::Zero(nlay);
                yh = VectorXcr::Zero(nlay);
                u = VectorXcr::Zero(nlay);
                cf = VectorXcr::Zero(nlay);    // nlay -1 (lay 0 empty)
                rtu = VectorXcr::Zero(nlay);   // nlay -1 Interfaces only
                rtd = VectorXcr::Zero(nlay);   // nlay -1 Interfaces only
                kk = VectorXcr::Zero(nlay);
                Zyu = VectorXcr::Zero(nlay);
                Zyd = VectorXcr::Zero(nlay);
                Zyi = VectorXcr::Zero(nlay);
                th = VectorXcr::Zero(nlay);

                // Don't attach Earth, because this is performance critical.
                // Everying is internal, it's OK relax Frankie
                //if (Earth != NULL) {
                //    Earth->DetachFrom(this);
                //    Earth = NULL;
                //}
                //EarthIn->AttachTo(this);
                Earth = EarthIn;

                LayerThickness.resize(nlay);
                for (int ilay=0; ilay<nlay; ++ilay) {
                    LayerThickness(ilay) =  Earth->GetLayerThickness(ilay);
                }

                LayerDepth.resize(nlay);
                for (int ilay=0; ilay<nlay; ++ilay) {
                    LayerDepth(ilay) =  Earth->GetLayerDepth(ilay);
                }

            }

            void SetUpSource( DipoleSource* Dipole, const int &ifreq ) {

                zh(0) = Complex(0, Dipole->GetAngularFrequency(ifreq)*MU0);
                yh(0) = Complex(0, Dipole->GetAngularFrequency(ifreq)*EPSILON0);
                kk(0) = -zh(0) * yh(0);

                Earth->EvaluateColeColeModel(Dipole->GetAngularFrequency(ifreq));

                for (int ilay=1; ilay<Earth->GetNumberOfLayers(); ++ilay) {
                    zh(ilay) = zh(0) * Earth->GetLayerSusceptibility(ilay);
                    yh(ilay) = Earth->GetLayerConductivity(ilay) +
                            yh(0)*Earth->GetLayerPermitivity(ilay);
                    kk(ilay)   = -zh(ilay)*yh(ilay);
                }

                tx_z = Dipole->GetLocation(2);
                lays = 0;
                Real Depth(0);
                for (int ilay=1; ilay<nlay; ++ilay) {
                    if (tx_z > Depth) {
                        lays = ilay;
                    }
                    Depth += LayerThickness(ilay);
                }
            }

            void SetUpReceiver(const Real &rz) {
                rx_z = rz;
                Real Depth(0.);
                layr = 0;
                for (int ilay=1; ilay<Earth->GetNumberOfLayers(); ++ilay) {
                    if (rx_z > Depth) {
                        layr = ilay;
                    }
                    Depth += LayerThickness(ilay);
                }
            }

            // ====================  ACCESS        =======================
            Complex GetYm() {
                return yh(layr);
            }

            Complex GetZm() {
                return zh(layr);
            }

            Complex GetZs() {
                return zh(lays);
            }

            Complex GetKs() {
                return kk(lays);
            }

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /// Default protected constructor.
            KernelEM1DReflBase ( ) : LemmaObject( )
            {
            }

            /// Default protected constructor.
            ~KernelEM1DReflBase () {
            }

            // ====================  OPERATIONS    =======================

            /** Computes reflection coefficients. Depending on the
             * specialisation, this will either be TM or TE mode.
             */
            virtual void ComputeReflectionCoeffs(const Real& lambda)=0;

            /** Precomputes expensive calculations that are reused by insances
             * of KernelEM1DSpec in the calculation of Related potentials.
             */
            virtual void PreComputePotentialTerms()=0;

            // ====================  DATA MEMBERS  =========================

			/// Bessel order, only 0 or 1 supported
			int id; // Needs to be dim nRel, or separate

            /// Layer the source is in
			int lays;

			/// Layer the receiver is in
			int layr;

			/// Number of Layers
			int nlay;

            /// Number of Related kernels to be computed
            int nRelated;

            /// Used in related kernel precompute calls
            int relIud;

			/// Receiver z position
			Real rx_z;

			/// Transmitter z position
			Real tx_z;

            /// bessel arg squared
            Real rams;

            /** Related part of con term */
            Complex  relCon;

            Complex  relenukadz;

            Complex  relexp_pbs1;

            Complex  relexp_pbs2;

            Complex  rel_a;

			/// TM or TE mode
			EMMODE mode;

            /// Pointer to layered earth
            std::shared_ptr<LayeredEarthEM> Earth = nullptr;

			Complex       uk;
			Complex       um;
			VectorXcr     cf;  // nlay
			VectorXcr      u;  // nlay
			VectorXcr     yh;  // nlay
			VectorXcr     zh;  // nlay
			VectorXcr     kk;  // nlay
			VectorXcr    Zyu;  //(nlay); //Zyu.setZero();
			VectorXcr    Zyd;  //(nlay); //Zyd.setZero();
			VectorXcr    Zyi;  //(nlay); //Zyi.setZero();
			VectorXcr     th;  //(nlay);
			VectorXr     LayerThickness;
			VectorXr     LayerDepth;

			/// Reflection/Transmission coeffients for upgoing waves in a
			/// layered earth model
			VectorXcr    rtu;

			/// Reflection/Transmission coeffients for downgoing waves in
			/// a layered earth model
			VectorXcr    rtd;

        private:

    }; // -----  end of class  KernelEM1DReflBase  -----

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef KERNELEM1DREFLBASE_INC  -----
