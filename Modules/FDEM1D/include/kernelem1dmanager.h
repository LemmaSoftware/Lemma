/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     06/26/2012
  @version   0.0
 **/

#ifndef  KERNELEM1DMANAGER_INC
#define  KERNELEM1DMANAGER_INC

//#include "dipolesource.h"
//#include "kernelem1dreflbase.h"
#include "kernelem1dbase.h"
#include "kernelem1dspec.h"

namespace Lemma {

    class DipoleSource;
    class KernelEM1DReflBase;

    // ===================================================================
    //  Class:  KernelEM1DManager
    /**
      @class
      \brief
      \details
     */
    // ===================================================================
    class KernelEM1DManager : public LemmaObject {

        public:

            friend std::ostream &operator<<(std::ostream &stream,
                        const KernelEM1DManager &ob);

            // ====================  LIFECYCLE     =======================

            /** Returns a pointer to a new object of type KernelEM1DManager.
             * It allocates all necessary memory.
             */
            static KernelEM1DManager* New();

            /** Deletes this object. Delete also disconnects any
             * attachments to this object.
             */
            void Delete();

            // ====================  OPERATORS     =======================

            // ====================  OPERATIONS    =======================

            template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
            int AddKernel( );
            /*
            {

                KernelEm1DSpec<Mode, Ikernel, Isource, Irecv>* NewKern =
                    KernelEm1DSpec<Mode, Ikernel, Isource, Irecv>::New();
                KernelVec.push_back( NewKern );
                NewKern->managerIdx = KernelVec.size()-1;
                switch (Mode) {
                    case TE:
                        if (TEReflBase == NULL) {
                            TEReflBase = KernelEM1DReflSpec<TE, Isource, Irecv>::New();
                            TEReflBase->Initialise(Earth);
                            TEReflBase->SetUpSource(Dipole, ifreq);
                            TEReflBase->SetUpReceiver( rx_z );
                        }
                        NewKern->SetReflBase(TEReflBase);
                        break;
                    case TM:
                        if (TMReflBase == NULL) {
                            TMReflBase = KernelEM1DReflSpec<TM, Isource, Irecv>::New();
                            TMReflBase->Initialise(Earth);
                            TMReflBase->SetUpSource(Dipole, ifreq);
                            TMReflBase->SetUpReceiver( rx_z );
                        }
                        NewKern->SetReflBase(TMReflBase);
                        break;
                }
                return static_cast<int>(KernelVec.size()-1);
            }
            */

            void ResetSource(const int& ifreq);

            /** For use in related Kernel calculations. This function calles
             * ComputeReflectionCoeffs on TEReflBase and TMReflBase, if they
             * exist. After this has been called, KernelEM1DBase::RelBesselArg() may be safely
             * called. This method stores solutions of the same idx. rho0 is the intial lambda argument
             */
            void ComputeReflectionCoeffs(const Real& lambda, const int& idx, const Real& rho0);

            /** Clears the vector of kernels */
            void ClearVec() {
                for (unsigned int ik=0; ik<this->KernelVec.size(); ++ik) {
                    this->KernelVec[ik]->Delete();
                }
                KernelVec.clear();
            }

            // ====================  ACCESS        =======================

            /** Sets the LayeredEarthEM class that will be used by the kernels.
             */
            void SetEarth( LayeredEarthEM*   Earth);

            /** Sets the source of the kernels */
            void SetDipoleSource( DipoleSource*   Dipole, const int& ifreq, const Real& rx_zin);

            /** Returns pointer to specified kernel indice. Indices are assigned in the same
                order as they are created by AddKernel.
             */
            KernelEm1DBase*    GetKernel(const unsigned int& ik);

            /** Returns pointer to connected dipole.
             */
            DipoleSource*    GetDipole( );

            inline std::vector<KernelEm1DBase*>  GetSTLVector() {
                return KernelVec;
            }

            // ====================  INQUIRY       =======================

        protected:

            // ====================  LIFECYCLE     =======================

            /** Default protected constructor. */
            KernelEM1DManager (const std::string& name);

            /** Default protected constructor. */
            ~KernelEM1DManager ();

            /**
             * @copybrief LemmaObject::Release()
             * @copydetails LemmaObject::Release()
             */
            void Release();

            // ====================  DATA MEMBERS  =========================

            /** List of KernelEm1D instances */
            std::vector<KernelEm1DBase*>           KernelVec;

            /** Reflection base used for TE mode */
            KernelEM1DReflBase*                    TEReflBase;

            /** Reflection base used for TM mode */
            KernelEM1DReflBase*                    TMReflBase;

            /** EmEarth Class */
            LayeredEarthEM*                        Earth;

            /** EM dipole souce */
            DipoleSource*                          Dipole;

            /** Frequency index for the sources */
            int                                    ifreq;

            /** Receiver height */
            Real                                   rx_z;

        private:

    }; // -----  end of class  KernelEM1DManager  -----

    // template methods
    template<EMMODE Mode, int Ikernel, DIPOLE_LOCATION Isource, DIPOLE_LOCATION Irecv>
    int KernelEM1DManager::AddKernel( ) {

        KernelEm1DSpec<Mode, Ikernel, Isource, Irecv>* NewKern =
                    KernelEm1DSpec<Mode, Ikernel, Isource, Irecv>::New();
        KernelVec.push_back( NewKern );
        NewKern->managerIdx = KernelVec.size()-1;
        switch (Mode) {
            case TE:
                if (TEReflBase == NULL) {
                    TEReflBase = KernelEM1DReflSpec<TE, Isource, Irecv>::New();
                    TEReflBase->Initialise(Earth);
                    TEReflBase->SetUpSource(Dipole, ifreq);
                    TEReflBase->SetUpReceiver( rx_z );
                }
                NewKern->SetReflBase(TEReflBase);
                break;
            case TM:
                if (TMReflBase == NULL) {
                    TMReflBase = KernelEM1DReflSpec<TM, Isource, Irecv>::New();
                    TMReflBase->Initialise(Earth);
                    TMReflBase->SetUpSource(Dipole, ifreq);
                    TMReflBase->SetUpReceiver( rx_z );
                }
                NewKern->SetReflBase(TMReflBase);
                break;
        }
        return static_cast<int>(KernelVec.size()-1);
     }


//     /** Clears the vector of kernels */
//     void ClearVec() {
//         for (unsigned int ik=0; ik<this->KernelVec.size(); ++ik) {
//             this->KernelVec[ik]->Delete();
//         }
//         KernelVec.clear();
//     }

}		// -----  end of Lemma  name  -----

#endif   // ----- #ifndef KERNELEM1DMANAGER_INC  -----
