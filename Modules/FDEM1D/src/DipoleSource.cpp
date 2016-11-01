/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
  @version  $Id: dipolesource.cpp 203 2015-01-09 21:19:04Z tirons $
 **/

#include "dipolesource.h"
#include "kernelem1dmanager.h"

#include "GroundedElectricDipole.h"
#include "UngroundedElectricDipole.h"
#include "MagneticDipole.h"
#include "receiverpoints.h"
#include "hankeltransform.h"

namespace Lemma {


    // ====================    FRIENDS     ======================
    std::ostream &operator<<(std::ostream &stream,
                const DipoleSource &ob) {

        stream << *(LemmaObject*)(&ob);
        switch (ob.Type) {
            case (NOSOURCETYPE):
                stream << "\tType=" << ob.Type << " (Uninitialized dipole)"
                << "\n";
                break;
            case (GROUNDEDELECTRICDIPOLE):
                stream << "\tType=" << ob.Type << " (Grounded Electric dipole)"
                << "\n";
                break;
            case (UNGROUNDEDELECTRICDIPOLE):
                stream << "\tType=" << ob.Type << " (Ungrounded Electric dipole)"
                << "\n";
                break;
            case (MAGNETICDIPOLE):
                stream << "\tType=" << ob.Type << " (Magnetic dipole)"
                << "\n";
                break;
            default:
                throw NonValidDipoleType( );
        }

        /*
        switch (ob.Polarisation) {

            case(NOPOLARISATION):
                stream << "Polarisation=" << ob.Polarisation <<
                        " (Unset polarisation)" << "\n";
                break;

            case(XPOLARISATION):
                stream << "Polarisation=" << ob.Polarisation <<
                        " (X polarised)" << "\n";
                break;

            case(YPOLARISATION):
                stream << "Polarisation=" << ob.Polarisation <<
                        " (Y polarised)" << "\n";
                break;

            case(ZPOLARISATION):
                stream << "Polarisation=" << ob.Polarisation <<
                        " (Z polarised)" << "\n";
                break;
        }
        */
        stream << "\tPolarisation " << ob.Phat.transpose() << "\n";
        stream << "\tLocation= " << ob.Location.transpose()
                << " [metres] \n";
        stream << "\tFrequencies= " << ob.Freqs.transpose() << " [Hz] \n";
        stream << "\tPhase= " << ob.Phase << "\n";
        stream << "\tMoment= " << ob.Moment << "\n";
        return stream;
    }


    // ====================  LIFECYCLE     ======================

    DipoleSource::DipoleSource(const std::string &name) :
                    LemmaObject(name),
                    Type(NOSOURCETYPE),
                    Phase(0),
                    Moment(1),
                    KernelManager(NULL),
                    Receivers(NULL),
                    Earth(NULL)
    {
        this->Location.setZero();
        this->Phat.setZero();
    }

    DipoleSource::~DipoleSource() {
        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences(this);
        /* These are not attched as they are connected for such a short time that the reference counting
           was getting tricked and calling delete
        if (Receivers != NULL) {
            Receivers->DetachFrom(this);
        }

        if (Earth != NULL) {
            Earth->DetachFrom(this);
        }
        */
        if (KernelManager != NULL) {
            KernelManager->Delete();
        }

    }

    DipoleSource* DipoleSource::New() {
        DipoleSource* Obj = new DipoleSource("DipoleSource");
        Obj->AttachTo(Obj);
        return Obj;
    }

    DipoleSource* DipoleSource::Clone() {

        DipoleSource* Obj = new DipoleSource("DipoleSource");
        Obj->AttachTo(Obj);

        // copy
        Obj->Type = Type;

        Obj->irec = irec;
        Obj->lays = lays;
        Obj->layr = layr;

        Obj->Phase = Phase;
        Obj->Moment = Moment;

        Obj->xxp = xxp;
        Obj->yyp = yyp;
        Obj->rho = rho;
        Obj->sp = sp;
        Obj->cp = cp;
        Obj->scp = scp;
        Obj->sps = sps;
        Obj->cps = cps;
        Obj->c2p = c2p;

        Obj->FieldsToCalculate = FieldsToCalculate;
        Obj->f = f;
        Obj->ik = ik;

        Obj->Location = Location;
        Obj->Phat = Phat;
        Obj->Freqs = Freqs;

        return Obj;
    }

    void DipoleSource::Delete() {
        this->DetachFrom(this);
    }

    void DipoleSource::Release() {
        delete this;
    }

    // ====================  ACCESS        ======================
    void DipoleSource::SetLocation(const Vector3r &posin) {
        this->Location = posin;
    }

    void DipoleSource::SetLocation(const Real &xp, const Real &yp,
                    const Real &zp) {
        this->Location = Vector3r(xp, yp, zp);
    }

    void DipoleSource::SetPhase(const Real &phase) {
        this->Phase = phase;
    }

    void DipoleSource::SetPolarity(const DipoleSourcePolarity &pol) {
        static bool called = false;
        if (!called) {
            std::cerr << "\n\n=================================================================\n"
                      << "WARNING: Use of deprecated method DipoleSource::SetPolarity(pol)\n"
                      << "Use more general SetPolarisation( Vector3r ) or SetPolarisation( x, y, z );\n"
                      << "This method will be removed in future versions of Lemma"
                      << "\n=================================================================\n";
            called = true;
        }
//         Polarity = pol;
//         switch (Polarity) {
//             case POSITIVE:
//                 Moment =  std::abs(Moment);
//                 break;
//             case NEGATIVE:
//                 Moment = -std::abs(Moment);
//                 break;
//             default:
//                 throw NonValidDipolePolarity();
//         };
    }

    void DipoleSource::SetType(const DipoleSourceType & stype) {

        switch (stype) {
            case (GROUNDEDELECTRICDIPOLE):
                this->Type = stype;
                break;
            case (UNGROUNDEDELECTRICDIPOLE):
                this->Type = stype;
                break;
            case (MAGNETICDIPOLE):
                this->Type = stype;
                break;
            default:
                throw NonValidDipoleTypeAssignment();
        }
    }

    void DipoleSource::SetPolarisation(const Vector3r& pol) {
        this->Phat = pol / pol.norm();
    }

    void DipoleSource::SetPolarisation(const Real& x, const Real& y, const Real& z) {
        Vector3r pol = (VectorXr(3) << x, y, z).finished();
        this->Phat = pol / pol.norm();
    }

    Vector3r DipoleSource::GetPolarisation() {
        return Phat;
    }

    DipoleSourceType DipoleSource::GetType() {
        return Type;
    }

    void DipoleSource::SetPolarisation(const
                    DipoleSourcePolarisation &pol) {

        static bool called = false;
        if (!called) {
            std::cout << "\n\n========================================================================================\n"
                      << "WARNING: Use of deprecated method DipoleSource::SetPolarisation(DipleSourcePolarisation)\n"
                      << "Use more general SetPolarisation( Vector3r ) or SetPolarisation( x, y, z );\n"
                      << "This method will be removed in future versions of Lemma"
                      << "\n========================================================================================\n";
            called = true;
        }

        switch (pol) {
            case (XPOLARISATION):
                //this->Polarisation = pol;
                this->Phat = (VectorXr(3) << 1, 0, 0).finished();
                break;
            case (YPOLARISATION):
                //this->Polarisation = pol;
                this->Phat = (VectorXr(3) << 0, 1, 0).finished();
                break;
            case (ZPOLARISATION):
                //this->Polarisation = pol;
                this->Phat = (VectorXr(3) << 0, 0, 1).finished();
                break;
            default:
                throw NonValidDipolePolarisationAssignment();
        }
    }

    void DipoleSource::SetMoment(const Real &moment) {
        this->Moment = moment;
    }

    // ====================  OPERATIONS     =====================

    void DipoleSource::SetKernels(const int& ifreq, const FIELDCALCULATIONS&  Fields , ReceiverPoints* ReceiversIn, const int& irecin,
            LayeredEarthEM* EarthIn  ) {

        // These are not properly attached because in tight parallel loops reference counting is unreliable. This is safe as this
        // method is not exposed to the public API and we know what we are doing here -TI
        if (Receivers != ReceiversIn) {
            //if (Receivers != NULL) Receivers->DetachFrom(this);
            Receivers = ReceiversIn;
            //ReceiversIn->AttachTo(this);
        }

        if (Earth != EarthIn) {
            //if (Earth != NULL) Earth->DetachFrom(this);
            Earth = EarthIn;
            //EarthIn->AttachTo(this);
        }

        if (irecin != irec) {
            irec = irecin;
        }

        if (FieldsToCalculate != Fields) {
            FieldsToCalculate = Fields;
        }

        xxp = Receivers->GetLocation(irec)[0] - Location[0];
        yyp = Receivers->GetLocation(irec)[1] - Location[1];
        rho = (Receivers->GetLocation(irec).head<2>() - Location.head<2>()).norm();

        sp = yyp/rho;
        cp = xxp/rho;
        scp = sp*cp;
        sps = sp*sp;
        cps = cp*cp;
        c2p = cps-sps;

        f = VectorXcr::Zero(13);
        ik = VectorXi::Zero(13);

        lays = Earth->GetLayerAtThisDepth(Location[2]);
        layr = Earth->GetLayerAtThisDepth(Receivers->GetLocation(irec)[2]);

        if (KernelManager != NULL) KernelManager->Delete();
        KernelManager = KernelEM1DManager::New();
            KernelManager->SetEarth(Earth);
            KernelManager->SetDipoleSource(this, ifreq, Receivers->GetLocation(irec)[2]);
        kernelFreq = Freqs(ifreq); // this is never used
        ReSetKernels( ifreq, Fields, Receivers, irec, Earth );

        return;
    }

    // TODO we could make the dipoles template specializations avoiding this rats nest of switch statements. Probably
    //      not the most critical piece though
    void DipoleSource::ReSetKernels(const int& ifreq, const FIELDCALCULATIONS&  Fields , ReceiverPoints* Receivers, const int& irec,
            LayeredEarthEM* Earth  ) {

        Vector3r Pol = Phat;

        switch (Type) {

            case (GROUNDEDELECTRICDIPOLE):

                if (std::abs(Pol[2]) > 0) { // z dipole

                    switch(FieldsToCalculate) {

                        case E:
                            if (lays == 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INAIR, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INAIR, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INGROUND, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INAIR>( );
                            } else {
                                ik[10] = KernelManager->AddKernel<TM, 10, INGROUND, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:
                            if (lays == 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INAIR>( );
                            } else {
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INGROUND>( );
                            }
                            break;


                        case BOTH:
                            if ( lays == 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INAIR, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INAIR>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INAIR, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INGROUND>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TM, 10, INGROUND, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INAIR>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INAIR>( );
                            } else {
                                ik[10] = KernelManager->AddKernel<TM, 10, INGROUND, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INGROUND>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INGROUND>( );
                            }
                        }
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // x or y grounded HED dipole

                    switch(FieldsToCalculate) {

                        case E:
                            if ( lays == 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INAIR, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INAIR, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INAIR, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INAIR, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INAIR, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INAIR, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INGROUND, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INGROUND, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INGROUND, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INAIR>( );
                            } else {
                                ik[0] = KernelManager->AddKernel<TM, 0, INGROUND, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INGROUND, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INGROUND, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:
                            if (lays == 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INAIR>( );
                            } else {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INGROUND>( );
                            }
                            break;

                        case BOTH:
                            if (lays == 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INAIR, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INAIR, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INAIR, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INAIR>( );
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INAIR, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INAIR, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INAIR, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INGROUND>( );
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TM, 0, INGROUND, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INGROUND, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INGROUND, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INAIR>( );
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INAIR>( );
                            } else {
                                ik[0] = KernelManager->AddKernel<TM, 0, INGROUND, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TM, 1, INGROUND, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TM, 4, INGROUND, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INGROUND>( );
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INGROUND>( );
                            }
                            break;
                        }
                    }
                break;

            case (UNGROUNDEDELECTRICDIPOLE):

                if (std::abs(Pol[2]) > 0) { // z dipole

                    switch(FieldsToCalculate) {

                        case E:
                            if (lays == 0 && layr == 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INAIR>( );
                            } else {
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:
                            if (lays == 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INAIR>( );
                            } else {
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INGROUND>( );
                            }
                            break;


                        case BOTH:
                            if ( lays == 0 && layr == 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INAIR>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INAIR, INGROUND>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INAIR>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INAIR>( );
                            } else {
                                ik[11] = KernelManager->AddKernel<TM, 11, INGROUND, INGROUND>( );
                                ik[12] = KernelManager->AddKernel<TM, 12, INGROUND, INGROUND>( );
                            }
                        }
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // x or y grounded HED dipole

                    switch(FieldsToCalculate) {

                        case E:
                            if ( lays == 0 && layr == 0) {
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INAIR>( );
                            } else {
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:
                            if (lays == 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INAIR>( );
                            } else {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INGROUND>( );
                            }
                            break;

                        case BOTH:
                            if (lays == 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INAIR, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INAIR>( );
                            } else {
                                ik[5] = KernelManager->AddKernel<TM, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TM, 6, INGROUND, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TE, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TE, 3, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TE, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TE, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TE, 9, INGROUND, INGROUND>( );
                            }
                            break;
                        }
                    }


                break;

            case (MAGNETICDIPOLE):

                if (std::abs(Pol[2]) > 0) { // z dipole

                    switch (FieldsToCalculate) {

                        case E:

                            if (lays == 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INGROUND, INAIR>( );
                            } else {
                                ik[12] = KernelManager->AddKernel<TE, 12, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:

                            if (lays == 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TE, 10, INAIR, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[10] = KernelManager->AddKernel<TE, 10, INAIR, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[10] = KernelManager->AddKernel<TE, 10, INGROUND, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INGROUND, INAIR>( );
                            } else {
                                ik[10] = KernelManager->AddKernel<TE, 10, INGROUND, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INGROUND, INGROUND>( );
                            }
                            break;


                        case BOTH:

                            if (lays == 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INAIR, INAIR>( );
                                ik[10] = KernelManager->AddKernel<TE, 10, INAIR, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INAIR, INGROUND>( );
                                ik[10] = KernelManager->AddKernel<TE, 10, INAIR, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[12] = KernelManager->AddKernel<TE, 12, INGROUND, INAIR>( );
                                ik[10] = KernelManager->AddKernel<TE, 10, INGROUND, INAIR>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INGROUND, INAIR>( );
                            } else {
                                ik[12] = KernelManager->AddKernel<TE, 12, INGROUND, INGROUND>( );
                                ik[10] = KernelManager->AddKernel<TE, 10, INGROUND, INGROUND>( );
                                ik[11] = KernelManager->AddKernel<TE, 11, INGROUND, INGROUND>( );
                            }
                    }
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // x or y grounded HED dipole

                    switch (FieldsToCalculate) {

                        case E:
                            if ( lays == 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TE, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[5] = KernelManager->AddKernel<TE, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TE, 5, INGROUND, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INGROUND, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INGROUND, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INGROUND, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INGROUND, INAIR>( );
                            } else {
                                ik[5] = KernelManager->AddKernel<TE, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INGROUND, INGROUND>( );
                            }
                            break;

                        case H:

                            if ( lays == 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TE, 0, INAIR, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INAIR, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INAIR, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[0] = KernelManager->AddKernel<TE, 0, INAIR, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INAIR, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INAIR, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INAIR, INGROUND>( );
                            } else if (lays > 0 && layr == 0) {
                                ik[0] = KernelManager->AddKernel<TE, 0, INGROUND, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INGROUND, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INGROUND, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INGROUND, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INGROUND, INAIR>( );
                            } else {
                                ik[0] = KernelManager->AddKernel<TE, 0, INGROUND, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INGROUND, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INGROUND, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INGROUND, INGROUND>( );
                            }
                            break;

                        case BOTH:

                            if ( lays == 0 && layr == 0) {
                                ik[5] = KernelManager->AddKernel<TE, 5, INAIR, INAIR>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INAIR, INAIR>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INAIR, INAIR>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INAIR, INAIR>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INAIR, INAIR>( );
                                ik[0] = KernelManager->AddKernel<TE, 0, INAIR, INAIR>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INAIR, INAIR>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INAIR, INAIR>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INAIR, INAIR>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INAIR, INAIR>( );
                            } else if (lays == 0 && layr > 0) {
                                ik[5] = KernelManager->AddKernel<TE, 5, INAIR, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INAIR, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INAIR, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INAIR, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INAIR, INGROUND>( );
                                ik[0] = KernelManager->AddKernel<TE, 0, INAIR, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INAIR, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INAIR, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INAIR, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INAIR, INGROUND>( );
                            } else {
                                ik[5] = KernelManager->AddKernel<TE, 5, INGROUND, INGROUND>( );
                                ik[6] = KernelManager->AddKernel<TE, 6, INGROUND, INGROUND>( );
                                ik[7] = KernelManager->AddKernel<TM, 7, INGROUND, INGROUND>( );
                                ik[8] = KernelManager->AddKernel<TM, 8, INGROUND, INGROUND>( );
                                ik[9] = KernelManager->AddKernel<TM, 9, INGROUND, INGROUND>( );
                                ik[0] = KernelManager->AddKernel<TE, 0, INGROUND, INGROUND>( );
                                ik[1] = KernelManager->AddKernel<TE, 1, INGROUND, INGROUND>( );
                                ik[4] = KernelManager->AddKernel<TE, 4, INGROUND, INGROUND>( );
                                ik[2] = KernelManager->AddKernel<TM, 2, INGROUND, INGROUND>( );
                                ik[3] = KernelManager->AddKernel<TM, 3, INGROUND, INGROUND>( );
                           }
                           break;
                        }
                }
                break;

            default:
                std::cerr << "Dipole type incorrect, in dipolesource.cpp";
                exit(EXIT_FAILURE);

        }


    }

    void DipoleSource::UpdateFields( const int& ifreq, HankelTransform* Hankel, const Real& wavef) {

        Vector3r Pol = Phat;

        switch (Type) {

            case (GROUNDEDELECTRICDIPOLE):

                //Hankel->ComputeRelated(rho, KernelManager);

                if (std::abs(Pol[2]) > 0) { // z dipole
                    switch(FieldsToCalculate) {
                        case E:
                            f(10) = Hankel->Zgauss(10, TM, 1, rho, wavef, KernelManager->GetKernel(ik[10])) / KernelManager->GetKernel(ik[10])->GetYm();
                            f(11) = Hankel->Zgauss(11, TM, 0, rho, wavef, KernelManager->GetKernel(ik[11])) / KernelManager->GetKernel(ik[11])->GetYm();
                            std::cout.precision(12);
                            this->Receivers->AppendEfield(ifreq, irec,
                                -Pol[2]*QPI*cp*f(10)*Moment,
                                -Pol[2]*QPI*sp*f(10)*Moment,
                                 Pol[2]*QPI*f(11)*Moment);
                            break;

                        case H:
                            f(12) = Hankel->Zgauss(12, TM, 1, rho, wavef, KernelManager->GetKernel(ik[12]));
                            this->Receivers->AppendHfield(ifreq, irec,
                                -Pol[2]*QPI*sp*f(12)*Moment,
                                 Pol[2]*QPI*cp*f(12)*Moment,
                                 0. );
                            break;

                        case BOTH:
                            f(10) = Hankel->Zgauss(10, TM, 1, rho, wavef, KernelManager->GetKernel(ik[10])) / KernelManager->GetKernel(ik[10])->GetYm();
                            f(11) = Hankel->Zgauss(11, TM, 0, rho, wavef, KernelManager->GetKernel(ik[11])) / KernelManager->GetKernel(ik[11])->GetYm();
                            this->Receivers->AppendEfield(ifreq, irec,
                                    -Pol[2]*QPI*cp*f(10)*Moment,
                                    -Pol[2]*QPI*sp*f(10)*Moment,
                                     Pol[2]*QPI*f(11)*Moment   );

                            f(12) = Hankel->Zgauss(12, TM, 1, rho, wavef, KernelManager->GetKernel(ik[12]));
                            this->Receivers->AppendHfield(ifreq, irec,
                                    -Pol[2]*QPI*sp*f(12)*Moment,
                                     Pol[2]*QPI*cp*f(12)*Moment,
                                     0. );
                    } // Fields to calculate Z polarity Electric dipole
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // y dipole
                    switch(FieldsToCalculate) {
                        case E:
                            f(2) = Hankel->Zgauss(2, TE, 0, rho, wavef, KernelManager->GetKernel(ik[2])) * KernelManager->GetKernel(ik[2])->GetZs();
                            f(3) = Hankel->Zgauss(3, TE, 1, rho, wavef, KernelManager->GetKernel(ik[3])) * KernelManager->GetKernel(ik[3])->GetZs();
                            f(0) = Hankel->Zgauss(0, TM, 0, rho, wavef, KernelManager->GetKernel(ik[0])) / KernelManager->GetKernel(ik[0])->GetYm();
                            f(1) = Hankel->Zgauss(1, TM, 1, rho, wavef, KernelManager->GetKernel(ik[1])) / KernelManager->GetKernel(ik[1])->GetYm();
                            f(4) = Hankel->Zgauss(4, TM, 1, rho, wavef, KernelManager->GetKernel(ik[4])) / KernelManager->GetKernel(ik[4])->GetYm();
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[1]*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho))*Moment,
                                    Pol[1]*QPI*((sps*f(0)+c2p*f(1)/rho)-(cps*f(2)-c2p*f(3)/rho))*Moment,
                                    Pol[1]*QPI*sp*f(4)*Moment);
                            }
                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*((cps*f(0)-c2p*f(1)/rho)-(sps*f(2)+c2p*f(3)/rho)),
                                    Pol[0]*Moment*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[0]*Moment*QPI*cp*f(4) );
                            }
                            break;
                        case H:
                            f(5) = Hankel->Zgauss(5, TM, 0, rho, wavef, KernelManager->GetKernel(ik[5]));
                            f(6) = Hankel->Zgauss(6, TM, 1, rho, wavef, KernelManager->GetKernel(ik[6]));
                            f(7) = Hankel->Zgauss(7, TE, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetZs()/KernelManager->GetKernel(ik[7])->GetZm();
                            f(8) = Hankel->Zgauss(8, TE, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetZs()/KernelManager->GetKernel(ik[8])->GetZm();
                            f(9) = Hankel->Zgauss(9, TE, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetZs()/KernelManager->GetKernel(ik[9])->GetZm();
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                        Pol[1]*QPI*(sps*f(5)+c2p*f(6)/rho-cps*f(7)+c2p*f(8)/rho)*Moment,
                                        Pol[1]*QPI*scp*(-f(5)+(Real)(2.)*f(6)/rho-f(7)+(Real)(2.)*f(8)/rho)*Moment,
                                        -Pol[1]*QPI*cp*f(9)*Moment );
                            }
                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*scp*(f(5)-(Real)(2.)*f(6)/rho+f(7)-(Real)(2.)*f(8)/rho),
                                    Pol[0]*Moment*QPI*(-cps*f(5)+c2p*f(6)/rho+sps*f(7)+c2p*f(8)/rho),
                                    Pol[0]*Moment*QPI*sp*f(9) );
                            }
                            break;
                        case BOTH:
                            f(0) = Hankel->Zgauss(0, TM, 0, rho, wavef, KernelManager->GetKernel(ik[0])) / KernelManager->GetKernel(ik[0])->GetYm();
                            f(1) = Hankel->Zgauss(1, TM, 1, rho, wavef, KernelManager->GetKernel(ik[1])) / KernelManager->GetKernel(ik[1])->GetYm();
                            f(4) = Hankel->Zgauss(4, TM, 1, rho, wavef, KernelManager->GetKernel(ik[4])) / KernelManager->GetKernel(ik[4])->GetYm();
                            f(2) = Hankel->Zgauss(2, TE, 0, rho, wavef, KernelManager->GetKernel(ik[2])) * KernelManager->GetKernel(ik[2])->GetZs();
                            f(3) = Hankel->Zgauss(3, TE, 1, rho, wavef, KernelManager->GetKernel(ik[3])) * KernelManager->GetKernel(ik[3])->GetZs();
                            f(5) = Hankel->Zgauss(5, TM, 0, rho, wavef, KernelManager->GetKernel(ik[5]));
                            f(6) = Hankel->Zgauss(6, TM, 1, rho, wavef, KernelManager->GetKernel(ik[6]));
                            f(7) = Hankel->Zgauss(7, TE, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetZs()/KernelManager->GetKernel(ik[7])->GetZm();
                            f(8) = Hankel->Zgauss(8, TE, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetZs()/KernelManager->GetKernel(ik[8])->GetZm();
                            f(9) = Hankel->Zgauss(9, TE, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetZs()/KernelManager->GetKernel(ik[9])->GetZm();

                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                        Pol[1]*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho))*Moment ,
                                        Pol[1]*QPI*((sps*f(0)+c2p*f(1)/rho)-(cps*f(2)-c2p*f(3)/rho))*Moment,
                                        Pol[1]*QPI*sp*f(4)*Moment);

                                this->Receivers->AppendHfield(ifreq, irec,
                                        Pol[1]*QPI*(sps*f(5)+c2p*f(6)/rho-cps*f(7)+c2p*f(8)/rho)*Moment,
                                        Pol[1]*QPI*scp*(-f(5)+(Real)(2.)*f(6)/rho-f(7)+(Real)(2.)*f(8)/rho)*Moment,
                                       -Pol[1]*QPI*cp*f(9)*Moment );
                            }
                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*((cps*f(0)-c2p*f(1)/rho)-(sps*f(2)+c2p*f(3)/rho)),
                                    Pol[0]*Moment*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[0]*Moment*QPI*cp*f(4) );

                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*scp*(f(5)-(Real)(2.)*f(6)/rho+f(7)-(Real)(2.)*f(8)/rho),
                                    Pol[0]*Moment*QPI*(-cps*f(5)+c2p*f(6)/rho+sps*f(7)+c2p*f(8)/rho),
                                    Pol[0]*Moment*QPI*sp*f(9) );
                            }
                            break;
                    }
                }
                break; // GROUNDEDELECTRICDIPOLE


            case UNGROUNDEDELECTRICDIPOLE:

                //Hankel->ComputeRelated(rho, KernelManager);

                if (std::abs(Pol[2]) > 0) { // z dipole
                    switch(FieldsToCalculate) {
                        case E:
                            f(10) = 0;
                            f(11) = Hankel->Zgauss(11, TM, 0, rho, wavef, KernelManager->GetKernel(ik[11])) / KernelManager->GetKernel(ik[11])->GetYm();

                            this->Receivers->AppendEfield(ifreq, irec,
                                -Pol[2]*QPI*cp*f(10)*Moment,
                                -Pol[2]*QPI*sp*f(10)*Moment,
                                 Pol[2]*QPI*f(11)*Moment);
                            break;

                        case H:
                            f(12) = Hankel->Zgauss(12, TM, 1, rho, wavef, KernelManager->GetKernel(ik[12]));
                            this->Receivers->AppendHfield(ifreq, irec,
                                -Pol[2]*QPI*sp*f(12)*Moment,
                                 Pol[2]*QPI*cp*f(12)*Moment,
                                 0. );
                            break;

                        case BOTH:
                            f(10) = 0;
                            f(11) = Hankel->Zgauss(11, TM, 0, rho, wavef, KernelManager->GetKernel(ik[11])) / KernelManager->GetKernel(ik[11])->GetYm();
                            this->Receivers->AppendEfield(ifreq, irec,
                                    -Pol[2]*QPI*cp*f(10)*Moment,
                                    -Pol[2]*QPI*sp*f(10)*Moment,
                                     Pol[2]*QPI*f(11)*Moment   );

                            f(12) = Hankel->Zgauss(12, TM, 1, rho, wavef, KernelManager->GetKernel(ik[12]));
                            this->Receivers->AppendHfield(ifreq, irec,
                                    -Pol[2]*QPI*sp*f(12)*Moment,
                                     Pol[2]*QPI*cp*f(12)*Moment,
                                     0. );
                    } // Fields to calculate Z polarity Electric dipole
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // y dipole
                    switch(FieldsToCalculate) {
                        case E:
                            f(0) = 0;
                            f(1) = 0;
                            f(2) = Hankel->Zgauss(2, TE, 0, rho, wavef, KernelManager->GetKernel(ik[2])) * KernelManager->GetKernel(ik[2])->GetZs();
                            f(3) = Hankel->Zgauss(3, TE, 1, rho, wavef, KernelManager->GetKernel(ik[3])) * KernelManager->GetKernel(ik[3])->GetZs();
                            f(4) = 0;
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[1]*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho))*Moment,
                                    Pol[1]*QPI*((sps*f(0)+c2p*f(1)/rho)-(cps*f(2)-c2p*f(3)/rho))*Moment,
                                    Pol[1]*QPI*sp*f(4)*Moment);
                            }
                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*((cps*f(0)-c2p*f(1)/rho)-(sps*f(2)+c2p*f(3)/rho)),
                                    Pol[0]*Moment*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[0]*Moment*QPI*cp*f(4) );
                            }
                            break;
                        case H:
                            f(5) = Hankel->Zgauss(5, TM, 0, rho, wavef, KernelManager->GetKernel(ik[5]));
                            f(6) = Hankel->Zgauss(6, TM, 1, rho, wavef, KernelManager->GetKernel(ik[6]));
                            f(7) = Hankel->Zgauss(7, TE, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetZs()/KernelManager->GetKernel(ik[7])->GetZm();
                            f(8) = Hankel->Zgauss(8, TE, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetZs()/KernelManager->GetKernel(ik[8])->GetZm();
                            f(9) = Hankel->Zgauss(9, TE, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetZs()/KernelManager->GetKernel(ik[9])->GetZm();
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                        Pol[1]*QPI*(sps*f(5)+c2p*f(6)/rho-cps*f(7)+c2p*f(8)/rho)*Moment,
                                        Pol[1]*QPI*scp*(-f(5)+(Real)(2.)*f(6)/rho-f(7)+(Real)(2.)*f(8)/rho)*Moment,
                                        -Pol[1]*QPI*cp*f(9)*Moment );
                                // Analytic whole space solution could go here
                            }
                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*scp*(f(5)-(Real)(2.)*f(6)/rho+f(7)-(Real)(2.)*f(8)/rho),
                                    Pol[0]*Moment*QPI*(-cps*f(5)+c2p*f(6)/rho+sps*f(7)+c2p*f(8)/rho),
                                    Pol[0]*Moment*QPI*sp*f(9) );
                                // Analytic whole space solution
                            }
                            break;

                        case BOTH:
                            f(0) = 0;
                            f(1) = 0;
                            f(4) = 0;
                            f(2) = Hankel->Zgauss(2, TE, 0, rho, wavef, KernelManager->GetKernel(ik[2])) * KernelManager->GetKernel(0)->GetZs();
                            f(3) = Hankel->Zgauss(3, TE, 1, rho, wavef, KernelManager->GetKernel(ik[3])) * KernelManager->GetKernel(1)->GetZs();
                            f(5) = Hankel->Zgauss(5, TM, 0, rho, wavef, KernelManager->GetKernel(ik[5]));
                            f(6) = Hankel->Zgauss(6, TM, 1, rho, wavef, KernelManager->GetKernel(ik[6]));
                            f(7) = Hankel->Zgauss(7, TE, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetZs()/KernelManager->GetKernel(ik[7])->GetZm();
                            f(8) = Hankel->Zgauss(8, TE, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetZs()/KernelManager->GetKernel(ik[8])->GetZm();
                            f(9) = Hankel->Zgauss(9, TE, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetZs()/KernelManager->GetKernel(ik[9])->GetZm();

                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                        Pol[1]*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho))*Moment ,
                                        Pol[1]*QPI*((sps*f(0)+c2p*f(1)/rho)-(cps*f(2)-c2p*f(3)/rho))*Moment,
                                        Pol[1]*QPI*sp*f(4)*Moment);

                                this->Receivers->AppendHfield(ifreq, irec,
                                        Pol[1]*QPI*(sps*f(5)+c2p*f(6)/rho-cps*f(7)+c2p*f(8)/rho)*Moment,
                                        Pol[1]*QPI*scp*(-f(5)+(Real)(2.)*f(6)/rho-f(7)+(Real)(2.)*f(8)/rho)*Moment,
                                       -Pol[1]*QPI*cp*f(9)*Moment );
                            }

                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*((cps*f(0)-c2p*f(1)/rho)-(sps*f(2)+c2p*f(3)/rho)),
                                    Pol[0]*Moment*QPI*scp*((f(0)-(Real)(2.)*f(1)/rho)+(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[0]*Moment*QPI*cp*f(4) );

                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*scp*(f(5)-(Real)(2.)*f(6)/rho+f(7)-(Real)(2.)*f(8)/rho),
                                    Pol[0]*Moment*QPI*(-cps*f(5)+c2p*f(6)/rho+sps*f(7)+c2p*f(8)/rho),
                                    Pol[0]*Moment*QPI*sp*f(9) );
                            }
                            break;
                    }
                }

                break; // UNGROUNDEDELECTRICDIPOLE


            case MAGNETICDIPOLE:


                //Hankel->ComputeRelated(rho, KernelManager);
                if (std::abs(Pol[2]) > 0) { // z dipole
                    switch(FieldsToCalculate) {
                        case E:
                            f(12)=Hankel->Zgauss(12, TE, 1, rho, wavef, KernelManager->GetKernel(ik[12]))*KernelManager->GetKernel(ik[12])->GetZs();
                            this->Receivers->AppendEfield(ifreq, irec,
                                 Pol[2]*Moment*QPI*sp*f(12),
                                -Pol[2]*Moment*QPI*cp*f(12),
                                 0);
                            break;
                        case H:
                            f(10)=Hankel->Zgauss(10, TE, 1, rho, wavef, KernelManager->GetKernel(ik[10]))*KernelManager->GetKernel(ik[10])->GetZs()/KernelManager->GetKernel(ik[10])->GetZm();
                            f(11)=Hankel->Zgauss(11, TE, 0, rho, wavef, KernelManager->GetKernel(ik[11]))*KernelManager->GetKernel(ik[11])->GetZs()/KernelManager->GetKernel(ik[11])->GetZm();
                            this->Receivers->AppendHfield(ifreq, irec,
                                -Pol[2]*Moment*QPI*cp*f(10),
                                -Pol[2]*Moment*QPI*sp*f(10),
                                 Pol[2]*Moment*QPI*f(11) );
                            break;

                        case BOTH:
                            f(12)=Hankel->Zgauss(12, TE, 1, rho, wavef, KernelManager->GetKernel(ik[12]))*KernelManager->GetKernel(ik[12])->GetZs();
                            f(10)=Hankel->Zgauss(10, TE, 1, rho, wavef, KernelManager->GetKernel(ik[10]))*KernelManager->GetKernel(ik[10])->GetZs()/KernelManager->GetKernel(ik[10])->GetZm();
                            f(11)=Hankel->Zgauss(11, TE, 0, rho, wavef, KernelManager->GetKernel(ik[11]))*KernelManager->GetKernel(ik[11])->GetZs()/KernelManager->GetKernel(ik[11])->GetZm();

                            this->Receivers->AppendEfield(ifreq, irec,
                                 Pol[2]*Moment*QPI*sp*f(12),
                                -Pol[2]*Moment*QPI*cp*f(12),
                                 0);

                            this->Receivers->AppendHfield(ifreq, irec,
                                -Pol[2]*Moment*QPI*cp*f(10),
                                -Pol[2]*Moment*QPI*sp*f(10),
                                 Pol[2]*Moment*QPI*f(11) );
                            break;
                    }
                }
                if (std::abs(Pol[1]) > 0 || std::abs(Pol[0]) > 0) { // x or y grounded HED dipole

                    switch (FieldsToCalculate) {

                        case E:

                            f(5) = Hankel->Zgauss(5, TE, 0, rho, wavef, KernelManager->GetKernel(ik[5]))*KernelManager->GetKernel(ik[5])->GetZs();
                            f(6) = Hankel->Zgauss(6, TE, 1, rho, wavef, KernelManager->GetKernel(ik[6]))*KernelManager->GetKernel(ik[6])->GetZs();
                            f(7) = Hankel->Zgauss(7, TM, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetKs()/KernelManager->GetKernel(ik[7])->GetYm();
                            f(8) = Hankel->Zgauss(8, TM, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetKs()/KernelManager->GetKernel(ik[8])->GetYm();
                            f(9) = Hankel->Zgauss(9, TM, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetKs()/KernelManager->GetKernel(ik[9])->GetYm();

                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                Pol[0]*Moment*QPI*scp*((-f(5)+(Real)(2.)*f(6)/rho)+(f(7)-(Real)(2.)*f(8)/rho)),
                                Pol[0]*Moment*QPI*((cps*f(5)-c2p*f(6)/rho)+(sps*f(7)+c2p*f(8)/rho)),
                                Pol[0]*Moment*QPI*sp*f(9));
                            }
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[1]*Moment*QPI*(-(sps*f(5)+c2p*f(6)/rho)-(cps*f(7)-c2p*f(8)/rho)),
                                    Pol[1]*Moment*QPI*scp*((f(5)-(Real)(2.)*f(6)/rho)-(f(7)-(Real)(2.)*f(8)/rho)),
                                    -Pol[1]*Moment*QPI*cp*f(9) );
                            }
                            break;

                        case H:
                            f(0) = Hankel->Zgauss(0, TE, 0, rho, wavef, KernelManager->GetKernel(ik[0]))*KernelManager->GetKernel(ik[0])->GetZs()/KernelManager->GetKernel(ik[0])->GetZm();
                            f(1) = Hankel->Zgauss(1, TE, 1, rho, wavef, KernelManager->GetKernel(ik[1]))*KernelManager->GetKernel(ik[1])->GetZs()/KernelManager->GetKernel(ik[1])->GetZm();
                            f(4) = Hankel->Zgauss(4, TE, 1, rho, wavef, KernelManager->GetKernel(ik[4]))*KernelManager->GetKernel(ik[4])->GetZs()/KernelManager->GetKernel(ik[4])->GetZm();
                            f(2) = Hankel->Zgauss(2, TM, 0, rho, wavef, KernelManager->GetKernel(ik[2]))*KernelManager->GetKernel(ik[2])->GetKs();
                            f(3) = Hankel->Zgauss(3, TM, 1, rho, wavef, KernelManager->GetKernel(ik[3]))*KernelManager->GetKernel(ik[3])->GetKs();

                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[0]*Moment*QPI*(cps*f(0)-c2p*f(1)/rho+(sps*f(2)+c2p*f(3)/rho)),
                                    Pol[0]*Moment*QPI*scp*(f(0)-(Real)(2.)*f(1)/rho-(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[0]*Moment*QPI*cp*f(4) );
                            }
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[1]*Moment*QPI*scp*(f(0)-(Real)(2.)*f(1)/rho-(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[1]*Moment*QPI*(sps*f(0)+c2p*f(1)/rho+(cps*f(2)-c2p*f(3)/rho)),
                                    Pol[1]*Moment*QPI*sp*f(4));
                            }
                            break;

                        case BOTH:
                            f(5) = Hankel->Zgauss(5, TE, 0, rho, wavef, KernelManager->GetKernel(ik[5]))*KernelManager->GetKernel(ik[5])->GetZs();
                            f(6) = Hankel->Zgauss(6, TE, 1, rho, wavef, KernelManager->GetKernel(ik[6]))*KernelManager->GetKernel(ik[6])->GetZs();
                            f(7) = Hankel->Zgauss(7, TM, 0, rho, wavef, KernelManager->GetKernel(ik[7]))*KernelManager->GetKernel(ik[7])->GetKs()/KernelManager->GetKernel(ik[7])->GetYm();
                            f(8) = Hankel->Zgauss(8, TM, 1, rho, wavef, KernelManager->GetKernel(ik[8]))*KernelManager->GetKernel(ik[8])->GetKs()/KernelManager->GetKernel(ik[8])->GetYm();
                            f(9) = Hankel->Zgauss(9, TM, 1, rho, wavef, KernelManager->GetKernel(ik[9]))*KernelManager->GetKernel(ik[9])->GetKs()/KernelManager->GetKernel(ik[9])->GetYm();
                            f(0) = Hankel->Zgauss(0, TE, 0, rho, wavef, KernelManager->GetKernel(ik[0]))*KernelManager->GetKernel(ik[0])->GetZs()/KernelManager->GetKernel(ik[0])->GetZm();
                            f(1) = Hankel->Zgauss(1, TE, 1, rho, wavef, KernelManager->GetKernel(ik[1]))*KernelManager->GetKernel(ik[1])->GetZs()/KernelManager->GetKernel(ik[1])->GetZm();
                            f(4) = Hankel->Zgauss(4, TE, 1, rho, wavef, KernelManager->GetKernel(ik[4]))*KernelManager->GetKernel(ik[4])->GetZs()/KernelManager->GetKernel(ik[4])->GetZm();
                            f(2) = Hankel->Zgauss(2, TM, 0, rho, wavef, KernelManager->GetKernel(ik[2]))*KernelManager->GetKernel(ik[2])->GetKs();
                            f(3) = Hankel->Zgauss(3, TM, 1, rho, wavef, KernelManager->GetKernel(ik[3]))*KernelManager->GetKernel(ik[3])->GetKs();

                            if (std::abs(Pol[0]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                Pol[0]*Moment*QPI*scp*((-f(5)+(Real)(2.)*f(6)/rho)+(f(7)-(Real)(2.)*f(8)/rho)),
                                Pol[0]*Moment*QPI*((cps*f(5)-c2p*f(6)/rho)+(sps*f(7)+c2p*f(8)/rho)),
                                Pol[0]*Moment*QPI*sp*f(9));

                                this->Receivers->AppendHfield(ifreq, irec,
                                Pol[0]*Moment*QPI*(cps*f(0)-c2p*f(1)/rho+(sps*f(2)+c2p*f(3)/rho)),
                                Pol[0]*Moment*QPI*scp*(f(0)-(Real)(2.)*f(1)/rho-(f(2)-(Real)(2.)*f(3)/rho)),
                                Pol[0]*Moment*QPI*cp*f(4) );
                            }
                            if (std::abs(Pol[1]) > 0) {
                                this->Receivers->AppendEfield(ifreq, irec,
                                    Pol[1]*Moment*QPI*(-(sps*f(5)+c2p*f(6)/rho)-(cps*f(7)-c2p*f(8)/rho)),
                                    Pol[1]*Moment*QPI*scp*((f(5)-(Real)(2.)*f(6)/rho)-(f(7)-(Real)(2.)*f(8)/rho)),
                                    -Pol[1]*Moment*QPI*cp*f(9) );
                                this->Receivers->AppendHfield(ifreq, irec,
                                    Pol[1]*Moment*QPI*scp*(f(0)-(Real)(2.)*f(1)/rho-(f(2)-(Real)(2.)*f(3)/rho)),
                                    Pol[1]*Moment*QPI*(sps*f(0)+c2p*f(1)/rho+(cps*f(2)-c2p*f(3)/rho)),
                                    Pol[1]*Moment*QPI*sp*f(4));
                            }

                            break;
                    }
                }
                break;
            case NOSOURCETYPE:
                throw NonValidDipoleType(this);

        } // Source Type Switch

    }

    // ====================  INQUIRY       ======================

    KernelEM1DManager*  DipoleSource::GetKernelManager() {
        return KernelManager;
    }

    Vector3r DipoleSource::GetLocation() {
        return this->Location;
    }

    #ifdef LEMMAUSEVTK
    vtkActor* DipoleSource::GetVtkActor() {

        vtkActor*                    vActor;

        vtkLineSource*               vLineSource;
        vtkTubeFilter*               vTube;
        vtkPolyDataMapper*           vMapper;
        vtkRegularPolygonSource*     vCircleSource;

        vLineSource = vtkLineSource::New();
        vTube = vtkTubeFilter::New();
        vMapper = vtkPolyDataMapper::New();
        vCircleSource = vtkRegularPolygonSource::New();

        VectorXr M0 = Location - .5*Moment*Phat;
        VectorXr M1 = Location + .5*Moment*Phat;

        vActor = vtkActor::New();

        switch (Type) {

            case GROUNDEDELECTRICDIPOLE:

                vLineSource->SetPoint1( M0(0), M0(1), M0(2));
                vLineSource->SetPoint2( M1(0), M1(1), M1(2));

                vTube->SetInputConnection(vLineSource->GetOutputPort());
                vTube->SetRadius(.1 * std::abs(Moment));
                vTube->SetNumberOfSides(6);
                vTube->SetCapping(1);
                vMapper->SetInputConnection(vTube->GetOutputPort());
                vActor->SetMapper(vMapper);
                vActor->GetProperty()->SetColor(Phat[0], Phat[1], Phat[2]);
                break;

            case UNGROUNDEDELECTRICDIPOLE:

                vLineSource->SetPoint1( M0(0), M0(1), M0(2));
                vLineSource->SetPoint2( M1(0), M1(1), M1(2));
                vTube->SetInputConnection(vLineSource->GetOutputPort());
                vTube->SetRadius(.1 * std::abs(Moment));
                vTube->SetNumberOfSides(6);
                vTube->SetCapping(1);
                vMapper->SetInputConnection(vTube->GetOutputPort());
                vActor->SetMapper(vMapper);
                //vActor->GetProperty()->SetColor(Phat[0], Phat[1], Phat[2]);
                vActor->GetProperty()->SetColor(rand()/(Real)(RAND_MAX), rand()/(Real)(RAND_MAX), rand()/(Real)(RAND_MAX));
                vActor->GetProperty()->SetOpacity(1.);
                break;

            case MAGNETICDIPOLE:

                vCircleSource->SetCenter(Location(0), Location(1),
                                Location(2));
                vCircleSource->SetNumberOfSides(360);

                vCircleSource->SetNormal(Phat[0], Phat[1], Phat[2]);
                vCircleSource->SetRadius(0.2);  // .2 m radius
                vCircleSource->SetGeneratePolygon(false);
                vCircleSource->SetGeneratePolyline(true);
                vCircleSource->Update();

                vTube->SetInputConnection(vCircleSource->GetOutputPort());
                //vTube->SetRadius( max((float)(*xCoords->GetTuple(nx)),
                //                    (float)(*yCoords->GetTuple(ny))) / 100);
                vTube->SetRadius(.1*std::abs(Moment));
                vTube->SetNumberOfSides(6);
                vTube->SetCapping(1);
                vMapper->SetInputConnection(vTube->GetOutputPort());
                vActor->SetMapper(vMapper);
                vActor->GetProperty()->SetColor(.9,.2,.9);
                break;

            default:
                throw NonValidDipoleType();
        }

        vLineSource->Delete();
        vCircleSource->Delete();
        vTube->Delete();
        vMapper->Delete();

        return vActor;

    }
    #endif

    Real DipoleSource::GetLocation(const int& coordinate) {
        switch (coordinate) {
            case (0):
                return this->Location.x();
                break;
            case (1):
                return this->Location.y();
                break;
            case (2):
                return this->Location.z();
                break;
            default:
                throw  NonValidLocationCoordinate( );
        }
    }

    DipoleSourceType DipoleSource::GetDipoleSourceType() {
        return this->Type;
    }

    //DipoleSourcePolarisation DipoleSource::GetDipoleSourcePolarisation() {
    //    return this->Polarisation;
    //}

    Real DipoleSource::GetAngularFrequency(const int& ifreq) {
        return 2.*PI*this->Freqs(ifreq);
    }

    Real DipoleSource::GetFrequency(const int& ifreq) {
        return this->Freqs(ifreq);
    }

    VectorXr DipoleSource::GetFrequencies( ) {
        return this->Freqs;
    }

    Real DipoleSource::GetPhase() {
        return this->Phase;
    }

    Real DipoleSource::GetMoment() {
        return this->Moment;
    }

    int DipoleSource::GetNumberOfFrequencies() {
        return this->Freqs.size();
    }

    void DipoleSource::SetNumberOfFrequencies(const int &nfreq){
        Freqs.resize(nfreq);
        Freqs.setZero();
    }

    void DipoleSource::SetFrequency(const int &ifreq, const Real &freq){
        Freqs(ifreq) = freq;
    }

    void DipoleSource::SetFrequencies(const VectorXr &freqs){
        Freqs = freqs;
    }

    /////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////
    // Error classes
    NullDipoleSource::NullDipoleSource() :
        runtime_error( "NULL VALUED DIPOLE SOURCE") {}

    NonValidDipoleTypeAssignment::NonValidDipoleTypeAssignment( ) :
        runtime_error( "NON VALID DIPOLE TYPE ASSIGNMENT") { }

    NonValidDipoleType::NonValidDipoleType( LemmaObject* ptr ) :
        runtime_error( "NON VALID DIPOLE TYPE") {
            std::cout << "Thrown by instance of "
                      << ptr->GetName() << std::endl;
    }

    NonValidDipoleType::NonValidDipoleType( ) :
        runtime_error( "NON VALID DIPOLE TYPE") { }

    NonValidDipolePolarity::NonValidDipolePolarity () :
        runtime_error( "NON VALID DIPOLE POLARITY") { }

    NonValidDipolePolarisation::NonValidDipolePolarisation( ) :
        runtime_error( "NON VALID DIPOLE TYPE") { }

    NonValidDipolePolarisationAssignment::
            NonValidDipolePolarisationAssignment( ) :
        runtime_error( "NON VALID DIPOLE POLARISATION ASSIGNMENT") { }

    NonValidLocationCoordinate::NonValidLocationCoordinate( ) :
        runtime_error( "NON VALID LOCATION COORDINATE REQUESTED") { }

}
