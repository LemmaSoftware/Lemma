/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
  @version  $Id: emearth1d.cpp 270 2015-08-24 15:45:41Z tirons $
 **/

#include "emearth1d.h"

#ifdef LEMMAUSEOMP
#include "omp.h"
#endif

namespace Lemma {

#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const EMEarth1D &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
	std::ostream &operator<<(std::ostream &stream, const
		EMEarth1D &ob) {

		stream << *(LemmaObject*)(&ob);
		stream << "Dipole source address: " << ob.Dipole << std::endl;
		stream << "Wire antenna address: " << ob.Antenna << std::endl;
		stream << *ob.Earth << std::endl;
		stream << *ob.Receivers;
		return stream;
	}
#endif

#ifdef KIHALEE_EM1D
    // Wrapper function for Fortran subroutine Em1D bi kihand
    // Returns E or H fields (SLOW)
    extern "C" { void em1dcall_(int &itype,   // source
                            int     &ipol,    // source
                            int     &nlay,    // Earth
                            int     &nfreq,   // source
                            int     &nfield,  // Calculator
                            int     &nres,    // Receivers
                            int     &jtype,   // N/A
                            int     &jgamma,  // Controller
                            double  &acc,     // Controller
                            double  *dep,     // Earth
                            std::complex<double> *sig,     // Earth
                            double  *susl,    // Earth
                            double  *sush,    // Earth
                            double  *sustau,  // Earth
                            double  *susalp,  // Earth
                            double  *eprl,    // Earth
                            double  *eprh,    // Earth
                            double  *eprtau,  // Earth
                            double  *epralp,  // Earth
                            double  &finit,   // N/A
                            double  &flimit,  // N/A
                            double  &dlimit,  // N/A
                            double  &lfinc,   // N/A
                            double  &tx,      // Source
                            double  &ty,      // Source
                            double  &tz,      // Source
                            double  *rxx,     // Receivers
                            double  *rxy,     // Receivers
                            double  *rxz,     // Receivers
                            std::complex<double> *ex,      // Receivers
                            std::complex<double> *ey,      //    |
                            std::complex<double> *ez,      //    |
                            std::complex<double> *hx,      //    |
                            std::complex<double> *hy,      //    V
                            std::complex<double> *hz );    //   ___
    }
#endif

    // ====================  LIFECYCLE     ===================================

    // TODO init large arrays here.
    EMEarth1D::EMEarth1D(const std::string& name) : LemmaObject(name),
            Dipole(nullptr), Earth(nullptr), Receivers(nullptr), Antenna(nullptr),
            FieldsToCalculate(BOTH), HankelType(ANDERSON801), icalcinner(0), icalc(0)
        //#ifdef HAVEBOOSTPROGRESS
        //    , disp(0)
        //#endif
        {
    }

    EMEarth1D::~EMEarth1D() {
        if (this->NumberOfReferences > 0)
            throw DeleteObjectWithReferences( this );
        DetachAll();
    }

    EMEarth1D* EMEarth1D::New() {
        EMEarth1D * Obj = new EMEarth1D("EmEarth1D");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void EMEarth1D::Delete() {
        this->DetachFrom(this);
    }

    void EMEarth1D::Release() {
        DetachAll();
        delete this;
    }

    #ifdef HAVE_YAMLCPP
    YAML::Node EMEarth1D::Serialize() const {
        YAML::Node node = LemmaObject::Serialize();

        node["FieldsToCalculate"] = enum2String(FieldsToCalculate);
        node["HankelType"] = enum2String(HankelType);

        //if (Dipole != NULL) node["Dipole"] = Dipole->Serialize();
        if (Earth != NULL)     node["Earth"] = Earth->Serialize();
        //if (Receivers != NULL) node["Receivers"] = Receivers->Serialize(); Can be huge?
        if (Antenna != NULL)   node["Antenna"] = Antenna->Serialize();

        node.SetTag( this->Name );

        return node;
    }
    #endif

    // ====================  ACCESS        ===================================
    void EMEarth1D::AttachDipoleSource(DipoleSource *dipoleptr) {
        if (this->Dipole != NULL) {
            this->Dipole->DetachFrom(this);
        }
        dipoleptr->AttachTo(this);
        this->Dipole = dipoleptr;
    }

    void EMEarth1D::AttachLayeredEarthEM(LayeredEarthEM *earthptr) {
        if (this->Earth != NULL)
            this->Earth->DetachFrom(this);
        earthptr->AttachTo(this);
        this->Earth = earthptr;
    }

    void EMEarth1D::AttachReceiverPoints(ReceiverPoints *recptr) {
        if (this->Receivers != NULL) {
            this->Receivers->DetachFrom(this);
        }

        recptr->AttachTo(this);
        this->Receivers = recptr;

        if (Receivers == NULL) {
            std::cout << "NULL Receivers in emearth1d.cpp " << std::endl;
            return;
        }

        if (Dipole != NULL) {
            switch (FieldsToCalculate) {
                case E:
                    Receivers->SetNumberOfBinsE(Dipole->GetNumberOfFrequencies());
                    break;
                case H:
                    Receivers->SetNumberOfBinsH(Dipole->GetNumberOfFrequencies());
                    break;
                case BOTH:
                    Receivers->SetNumberOfBinsE(Dipole->GetNumberOfFrequencies());
                    Receivers->SetNumberOfBinsH(Dipole->GetNumberOfFrequencies());
                    break;
            }
        } else if (Antenna != NULL) {
            switch (FieldsToCalculate) {
                case E:
                    Receivers->SetNumberOfBinsE(Antenna->GetNumberOfFrequencies());
                    break;
                case H:
                    Receivers->SetNumberOfBinsH(Antenna->GetNumberOfFrequencies());
                    break;
                case BOTH:
                    Receivers->SetNumberOfBinsE(Antenna->GetNumberOfFrequencies());
                    Receivers->SetNumberOfBinsH(Antenna->GetNumberOfFrequencies());
                    break;
            }

        }

    }

    void EMEarth1D::AttachWireAntenna(WireAntenna *antennae) {
        if (this->Antenna != NULL) {
            this->Antenna->DetachFrom(this);
        }
        antennae->AttachTo(this);
        this->Antenna = antennae;
    }

    void EMEarth1D::SetFieldsToCalculate(const FIELDCALCULATIONS &calc) {
        FieldsToCalculate = calc;
    }

    void EMEarth1D::SetHankelTransformMethod( const HANKELTRANSFORMTYPE &type) {
        HankelType = type;
    }

    void EMEarth1D::Query() {
        std::cout << "EmEarth1D::Query()" << std::endl;

        std::cout << "Dipole " << Dipole;
        if (Dipole) std::cout << *Dipole << std::endl;

        std::cout << "Earth " << Earth;
        if (Earth) std::cout << *Earth << std::endl;

        std::cout << "Receivers " << Earth;
        if (Earth) std::cout << *Receivers << std::endl;

        std::cout << "Antenna " << Earth;
        if (Antenna) std::cout << *Antenna << std::endl;

        std::cout << "icalc " << icalc << std::endl;

        std::cout << "icalcinner " << icalcinner << std::endl;
    }

    // ====================  OPERATIONS    ===================================
    void EMEarth1D::DetachAll() {

        if (this->Dipole != NULL){
            this->Dipole->DetachFrom(this);
        }
        Dipole = NULL;

        if (this->Receivers != NULL){
            this->Receivers->DetachFrom(this);
        }
        Receivers = NULL;

        if (this->Earth != NULL){
            this->Earth->DetachFrom(this);
        }
        Earth = NULL;

        if (this->Antenna != NULL){
            this->Antenna->DetachFrom(this);
        }
        Antenna = NULL;
    }

    void EMEarth1D::CalculateWireAntennaFields(bool progressbar) {

        #ifdef HAVEBOOSTPROGRESS
        boost::progress_display *disp;
        #endif

        if (Earth == NULL) {
            throw NullEarth();
        }
        if (Receivers == NULL) {
            throw NullReceivers();
        }
        if (Antenna == NULL) {
            throw NullAntenna();
        }
        if (Dipole != NULL) {
            throw DipoleSourceSpecifiedForWireAntennaCalc();
        }

        Receivers->ClearFields();

        // Check to make sure Receivers are set up for all calculations
        switch(FieldsToCalculate) {
            case E:
                if (Receivers->NumberOfBinsE != Antenna->GetNumberOfFrequencies())
                    Receivers->SetNumberOfBinsE(Antenna->GetNumberOfFrequencies());
                break;
            case H:
                if (Receivers->NumberOfBinsH != Antenna->GetNumberOfFrequencies())
                    Receivers->SetNumberOfBinsH(Antenna->GetNumberOfFrequencies());
                break;
            case BOTH:
                if (Receivers->NumberOfBinsH != Antenna->GetNumberOfFrequencies())
                    Receivers->SetNumberOfBinsH(Antenna->GetNumberOfFrequencies());
                if (Receivers->NumberOfBinsE != Antenna->GetNumberOfFrequencies())
                    Receivers->SetNumberOfBinsE(Antenna->GetNumberOfFrequencies());
                break;
        }

        if (Antenna->GetName() == std::string("PolygonalWireAntenna") || Antenna->GetName() == std::string("TEMTransmitter") ) {

            icalc += 1;

            // Check to see if they are all on a plane? If so we can do this fast
            /* TODO FIX THIS ISSUES */
            if (Antenna->IsHorizontallyPlanar() && HankelType == ANDERSON801) {
                //std::cout << "Lag baby lag" << std::endl;
                for (int ifreq=0; ifreq<Antenna->GetNumberOfFrequencies();++ifreq) {
                    //std::cout << "Num Recs" <<  Receivers->GetNumberOfReceivers() << std::endl;
                    Real wavef = 2.*PI* Antenna->GetFrequency(ifreq);
                    #ifdef LEMMAUSEOMP
                    #pragma omp parallel
                    {
                    #endif
                    Hankel2* Hankel = Hankel2::New();
                    #ifdef LEMMAUSEOMP
                    #pragma omp for schedule(static, 1)
                    #endif
                    for (int irec=0; irec<Receivers->GetNumberOfReceivers(); ++irec) {
                    //for (int irec=0; irec<2; ++irec) { // TODO FIXME BELO
                        PolygonalWireAntenna *AntCopy = static_cast<PolygonalWireAntenna*>(this->Antenna)->Clone();
                        SolveLaggedTxRxPair(irec, Hankel, wavef, ifreq, AntCopy);
                        AntCopy->Delete();
                        //exit(0);
                    }
                    //Receivers->ClearFields(); // FIXME DEBUG TODO
                    Hankel->Delete();
                    #ifdef LEMMAUSEOMP
                    }
                    #endif
                }
            } else
            if (Receivers->GetNumberOfReceivers() > Antenna->GetNumberOfFrequencies()) {

                //std::cout << "freq parallel #1" << std::endl;
                //** Progress display bar for long calculations */
                #ifdef HAVEBOOSTPROGRESS
                if (progressbar) {
                    disp = new boost::progress_display( Receivers->GetNumberOfReceivers()*Antenna->GetNumberOfFrequencies() );
                }
                #endif

                // parallelise across receivers
                #ifdef LEMMAUSEOMP
                #pragma omp parallel
                #endif
                { // OpenMP Parallel Block
                    // Since these antennas change we need a local copy for each
                    // thread.
                    PolygonalWireAntenna *AntCopy =
                            static_cast<PolygonalWireAntenna*>(this->Antenna)->Clone();

                    HankelTransform* Hankel;
                    switch (HankelType) {
                        case ANDERSON801:
                            Hankel = Hankel2::New();
                            break;
                        case CHAVE:
                            Hankel = HankelTransformGaussianQuadrature::New();
                            break;
                        case FHTKEY201:
                            Hankel = FHTKey::New();
                            break;
                        case FHTKEY101:
                            Hankel = FHTKey101::New();
                            break;
                        case FHTKEY51:
                            Hankel = FHTKey51::New();
                            break;
                        case QWEKEY:
                            Hankel = QWEKey::New();
                            break;
                        default:
                            std::cerr << "Hankel transform cannot be created\n";
                            exit(EXIT_FAILURE);
                    }

                    //for (int irec=tid; irec<Receivers->GetNumberOfReceivers(); irec+=nthreads) {
                    #ifdef LEMMAUSEOMP
                    #pragma omp for schedule(static, 1) //nowait
                    #endif
                    for (int irec=0; irec<Receivers->GetNumberOfReceivers(); ++irec) {
                        if (!Receivers->GetMask(irec)) {
                            AntCopy->ApproximateWithElectricDipoles(Receivers->GetLocation(irec));
                            for (int idip=0; idip<AntCopy->GetNumberOfDipoles(); ++idip) {
                                DipoleSource* tDipole = AntCopy->GetDipoleSource(idip);
                                //#ifdef LEMMAUSEOMP
                                //#pragma omp for schedule(static, 1)
                                //#endif
                                for (int ifreq=0; ifreq<tDipole->GetNumberOfFrequencies();
                                        ++ifreq) {
                                    // Propogation constant in free space
                                    Real wavef   = tDipole->GetAngularFrequency(ifreq) *
                                          std::sqrt(MU0*EPSILON0);
                                    SolveSingleTxRxPair(irec, Hankel, wavef, ifreq, tDipole);
                                } // freq loop
                            } // dipole loop
                        } // mask
                        //std::cout << "Normal Path\n";
                        //std::cout << Receivers->GetHfield(0, irec) << std::endl;
                        //if (irec == 1) exit(0);
                        #ifdef HAVEBOOSTPROGRESS
                        if (progressbar) ++(*disp);
                        #endif
                    } // receiver loop
                    Hankel->Delete();
                    AntCopy->Delete();
                } // OMP_PARALLEL BLOCK
            } else if (Antenna->GetNumberOfFrequencies() > 8) {
                // parallel across frequencies
                //std::cout << "freq parallel #2" << std::endl;
                for (int irec=0; irec<Receivers->GetNumberOfReceivers(); ++irec) {
                    if (!Receivers->GetMask(irec)) {
                        static_cast<PolygonalWireAntenna*>(Antenna)->
                            ApproximateWithElectricDipoles(Receivers->GetLocation(irec));
                        #ifdef LEMMAUSEOMP
                        #pragma omp parallel
                        #endif
                        { // OpenMP Parallel Block

                            HankelTransform* Hankel;
                            switch (HankelType) {
                                case ANDERSON801:
                                    Hankel = Hankel2::New();
                                    break;
                                case CHAVE:
                                    Hankel = HankelTransformGaussianQuadrature::New();
                                    break;
                                case FHTKEY201:
                                    Hankel = FHTKey::New();
                                    break;
                                case FHTKEY101:
                                    Hankel = FHTKey101::New();
                                    break;
                                case FHTKEY51:
                                    Hankel = FHTKey51::New();
                                    break;
                                case QWEKEY:
                                    Hankel = QWEKey::New();
                                    break;
                                default:
                                    std::cerr << "Hankel transform cannot be created\n";
                                    exit(EXIT_FAILURE);
                            }
                            #ifdef LEMMAUSEOMP
                            #pragma omp for schedule(static, 1)
                            #endif
                            for (int ifreq=0; ifreq<Antenna->GetNumberOfFrequencies(); ++ifreq) {
                                for (int idip=0; idip<Antenna->GetNumberOfDipoles(); ++idip) {
                                    DipoleSource* tDipole = Antenna->GetDipoleSource(idip);
                                    // Propogation constant in free space
                                    Real wavef   = tDipole->GetAngularFrequency(ifreq) *
                                          std::sqrt(MU0*EPSILON0);
                                    SolveSingleTxRxPair(irec, Hankel, wavef,
                                                    ifreq, tDipole);
                                } // dipole loop
                            } // frequency loop
                            Hankel->Delete();
                        } // OMP_PARALLEL BLOCK
                    } // mask loop
                    #ifdef HAVEBOOSTPROGRESS
                    //if (Receivers->GetNumberOfReceivers() > 100) {
                    //    ++ disp;
                    //}
                    #endif
                } // receiver loop
                //std::cout << "End freq parallel " << std::endl;
            } // Frequency Parallel
              else {
                //std::cout << "parallel across #3 " << std::endl;
                for (int irec=0; irec<Receivers->GetNumberOfReceivers(); ++irec) {
                    if (!Receivers->GetMask(irec)) {

                        static_cast<PolygonalWireAntenna*>(Antenna)->
                            ApproximateWithElectricDipoles(Receivers->GetLocation(irec));
//                         std::cout << "Not Masked " << std::endl;
//                         std::cout << "n Freqs " << Antenna->GetNumberOfFrequencies() << std::endl;
//                         std::cout << "n Dipoles " << Antenna->GetNumberOfDipoles() << std::endl;
//                         if ( !Antenna->GetNumberOfDipoles() ) {
//                             std::cout << "NO DIPOLES!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
// //                            std::cout << "rec location " << Receivers->GetLocation(irec) << std::endl;
// //                        }

                        #ifdef LEMMAUSEOMP
                        #pragma omp parallel
                        #endif
                        { // OpenMP Parallel Block
                            HankelTransform* Hankel;
                            switch (HankelType) {
                                case ANDERSON801:
                                    Hankel = Hankel2::New();
                                    break;
                                case CHAVE:
                                    Hankel = HankelTransformGaussianQuadrature::New();
                                    break;
                                case FHTKEY201:
                                    Hankel = FHTKey::New();
                                    break;
                                case FHTKEY101:
                                    Hankel = FHTKey101::New();
                                    break;
                                case FHTKEY51:
                                    Hankel = FHTKey51::New();
                                    break;
                                case QWEKEY:
                                    Hankel = QWEKey::New();
                                    break;
                                default:
                                    std::cerr << "Hankel transform cannot be created\n";
                                    exit(EXIT_FAILURE);
                            }
                            for (int ifreq=0; ifreq<Antenna->GetNumberOfFrequencies(); ++ifreq) {
                                #ifdef LEMMAUSEOMP
                                #pragma omp for schedule(static, 1)
                                #endif
                                for (int idip=0; idip<Antenna->GetNumberOfDipoles(); ++idip) {
                                    //#pragma omp critical
                                    //{
                                    //cout << "idip=" << idip << "\tthread num=" << omp_get_thread_num() << '\n';
                                    //}
                                    DipoleSource* tDipole = Antenna->GetDipoleSource(idip);
                                    // Propogation constant in free space
                                    Real wavef   = tDipole->GetAngularFrequency(ifreq) *
                                          std::sqrt(MU0*EPSILON0);
                                    SolveSingleTxRxPair(irec, Hankel, wavef, ifreq, tDipole);
                                } // dipole loop
                            } // frequency loop
                            Hankel->Delete();
                        } // OMP_PARALLEL BLOCK
                    } // mask loop
                    #ifdef HAVEBOOSTPROGRESS
                    //if (Receivers->GetNumberOfReceivers() > 100) {
                    //    ++ disp;
                    //}
                    #endif
                } // receiver loop
           } // Polygonal parallel logic
        } else {
             std::cerr << "Lemma with WireAntenna class is currently broken"
                  << " fix or use PolygonalWireAntenna\n" << std::endl;
             exit(EXIT_FAILURE);
            // TODO, getting wrong answer, curiously worKernel->GetKs() with MakeCalc, maybe
            // a threading issue, use SolveSingleTxRxPair maype instead of call
            // to MakeCalc3? !!!
            for (int idip=0; idip<Antenna->GetNumberOfDipoles(); ++idip) {
                this->Dipole = Antenna->GetDipoleSource(idip);
                MakeCalc3();
                //++disp;
            }
            this->Dipole = NULL;
        }

        #ifdef HAVEBOOSTPROGRESS
        if (progressbar) {
            delete disp;
        }
        #endif
    }

    #ifdef KIHALEE_EM1D
    void EMEarth1D::MakeCalc() {

        int itype;       // 1 = elec, 2 = mag
        switch (this->Dipole->GetDipoleSourceType()) {
            case (GROUNDEDELECTRICDIPOLE) :
                itype = 1;
                break;
            case (MAGNETICDIPOLE) :
                itype = 2;
                break;
            case (UNGROUNDEDELECTRICDIPOLE) :
                std::cerr << "Fortran routine cannot calculate ungrounded"
                             "electric dipole\n";
            default:
                throw NonValidDipoleType();
        }

        int ipol ;
        Vector3r Pol = this->Dipole->GetPolarisation();
        if (std::abs(Pol[0]-1) < 1e-5) {
            ipol = 1;
        } else if (std::abs(Pol[1]-1) < 1e-5) {
            ipol = 2;
        } else if (std::abs(Pol[2]-1) < 1e-5) {
            ipol = 3;
        } else {
            std::cerr << "Fortran routine cannot calculate arbitrary "
                             "dipole polarisation, set to x, y, or z\n";
        }

        int nlay   =  Earth->GetNumberOfNonAirLayers();

        if (nlay > MAXLAYERS) {
            std::cerr << "FORTRAN CODE CAN ONLY HANDLE " << MAXLAYERS
                      << " LAYERS\n";
            throw  EarthModelWithMoreThanMaxLayers();
        }

        int nfreq  = 1;     // number of freqs

        int nfield;     // field output 1 = elec, 2 = mag, 3 = both
        switch (FieldsToCalculate) {
            case E:
                nfield = 1;
                break;
            case H:
                nfield = 2;
                break;
            case BOTH:
                nfield = 3;
                break;
            default:
                throw 7;
        }

        int nres   = Receivers->GetNumberOfReceivers();
        int jtype  = 3;     // form ouf output,
                            // 1 = horizontal,
                            // 2 = down hole,
                            // 3 = freq sounding
                            // 4 = down hole logging

        int jgamma = 0;     // Units 0 = MKS (H->A/m and E->V/m)
                            //       1 = h->Gammas   E->V/m

        double acc = 0.;    // Tolerance

        // TODO, fix FORTRAN calls so these arrays can be nlay long, not
        // MAXLAYERS.

        // Model Parameters
        double *dep    = new double[MAXLAYERS];
        dep[0] = 0.;          // We always say air starts at 0
        for (int ilay=1; ilay<Earth->GetNumberOfLayers(); ++ilay) {
            dep[ilay] = dep[ilay-1] + Earth->GetLayerThickness(ilay);
            //std::cout << "Depth " << dep[ilay] << std::endl;
        }

        std::complex<double> *sig = new std::complex<double> [MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            sig[ilay-1] = (std::complex<double>)(Earth->GetLayerConductivity(ilay));
        }

        // TODO, pass these into Fortran call, and return Cole-Cole model
        // parameters. Right now this does nothing
        //std::complex<double> *sus   = new std::complex<double>[MAXLAYERS];
        //std::complex<double> *epr   = new std::complex<double>[MAXLAYERS];

        // Cole-Cole model stuff
        double *susl   = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            susl[ilay-1] = Earth->GetLayerLowFreqSusceptibility(ilay);
        }

        double *sush   = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            sush[ilay-1] = Earth->GetLayerHighFreqSusceptibility(ilay);
        }

        double *sustau = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            sustau[ilay-1] = Earth->GetLayerTauSusceptibility(ilay);
        }

        double *susalp = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            susalp[ilay-1] = Earth->GetLayerBreathSusceptibility(ilay);
        }

        double *eprl   = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            eprl[ilay-1] = Earth->GetLayerLowFreqPermitivity(ilay);
        }

        double *eprh   = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            eprh[ilay-1] = Earth->GetLayerHighFreqPermitivity(ilay);
        }

        double *eprtau = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            eprtau[ilay-1] = Earth->GetLayerTauPermitivity(ilay);
        }

        double *epralp = new double[MAXLAYERS];
        for (int ilay=1; ilay<=nlay; ++ilay) {
            epralp[ilay-1] = Earth->GetLayerBreathPermitivity(ilay);
        }

        // Freq stuff
        double finit  = Dipole->GetFrequency(0); //(1000);   // Starting freq
        double flimit = Dipole->GetFrequency(0); //(1000);   // max freq
        double dlimit = Dipole->GetFrequency(0); //(1000);   // difusion limit
        double lfinc(1);       // no. freq per decade

        // tx location jtype != 4
        double txx = Dipole->GetLocation(0); // (0.);
        double txy = Dipole->GetLocation(1); // (0.);
        double txz = Dipole->GetLocation(2); // (0.);

        // rx position
        // TODO, fix Fortran program to not waste this memory
        // maybe
        const int MAXREC = 15;
        double *rxx = new double [MAXREC];
        double *rxy = new double [MAXREC];
        double *rxz = new double [MAXREC];

        std::complex<double> *ex = new std::complex<double>[MAXREC];
        std::complex<double> *ey = new std::complex<double>[MAXREC];
        std::complex<double> *ez = new std::complex<double>[MAXREC];

        std::complex<double> *hx = new std::complex<double>[MAXREC];
        std::complex<double> *hy = new std::complex<double>[MAXREC];
        std::complex<double> *hz = new std::complex<double>[MAXREC];

        int nres2 = MAXREC;
        int ii=0;

        for (ii=0; ii<nres-MAXREC; ii+=MAXREC) {

            for (int ir=0; ir<MAXREC; ++ir) {
                //Vector3r pos = Receivers->GetLocation(ii+ir);
                rxx[ir] = Receivers->GetLocation(ii+ir)[0];
                rxy[ir] = Receivers->GetLocation(ii+ir)[1];
                rxz[ir] = Receivers->GetLocation(ii+ir)[2];
            }

            em1dcall_(itype, ipol, nlay, nfreq, nfield, nres2, jtype,
                  jgamma, acc, dep, sig, susl, sush, sustau, susalp,
                  eprl, eprh, eprtau, epralp, finit, flimit, dlimit,
                  lfinc, txx, txy, txz, rxx, rxy, rxz, ex, ey, ez,
                  hx, hy, hz);

            // Scale By Moment
            for (int ir=0; ir<MAXREC; ++ir) {

                ex[ir] *= Dipole->GetMoment();
                ey[ir] *= Dipole->GetMoment();
                ez[ir] *= Dipole->GetMoment();

                hx[ir] *= Dipole->GetMoment();
                hy[ir] *= Dipole->GetMoment();
                hz[ir] *= Dipole->GetMoment();

                // Append values instead of setting them
                this->Receivers->AppendEfield(0, ii+ir, (Complex)(ex[ir]),
                                                        (Complex)(ey[ir]),
                                                        (Complex)(ez[ir]) );
                this->Receivers->AppendHfield(0, ii+ir, (Complex)(hx[ir]),
                                                        (Complex)(hy[ir]),
                                                        (Complex)(hz[ir]) );
            }
        }

        //ii += MAXREC;
        nres2 = 0;
        // Perform last positions
        for (int ir=0; ir<nres-ii; ++ir) {
            rxx[ir] = Receivers->GetLocation(ii+ir)[0];
            rxy[ir] = Receivers->GetLocation(ii+ir)[1];
            rxz[ir] = Receivers->GetLocation(ii+ir)[2];
            ++nres2;
        }

        em1dcall_(itype, ipol, nlay, nfreq, nfield, nres2, jtype,
              jgamma, acc, dep, sig, susl, sush, sustau, susalp,
              eprl, eprh, eprtau, epralp, finit, flimit, dlimit,
              lfinc, txx, txy, txz, rxx, rxy, rxz, ex, ey, ez,
              hx, hy, hz);

        // Scale By Moment
        for (int ir=0; ir<nres-ii; ++ir) {

            ex[ir] *= Dipole->GetMoment();
            ey[ir] *= Dipole->GetMoment();
            ez[ir] *= Dipole->GetMoment();

            hx[ir] *= Dipole->GetMoment();
            hy[ir] *= Dipole->GetMoment();
            hz[ir] *= Dipole->GetMoment();

            // Append values instead of setting them
            this->Receivers->AppendEfield(0, ii+ir, (Complex)(ex[ir]),
                                                    (Complex)(ey[ir]),
                                                    (Complex)(ez[ir]) );
            this->Receivers->AppendHfield(0, ii+ir, (Complex)(hx[ir]),
                                                    (Complex)(hy[ir]),
                                                    (Complex)(hz[ir]) );

        }

        delete [] sig;
        delete [] dep;

        //delete [] sus;
        //delete [] epr;

        delete [] susl;
        delete [] sush;
        delete [] susalp;
        delete [] sustau;

        delete [] eprl;
        delete [] eprh;
        delete [] epralp;
        delete [] eprtau;

        delete [] rxx;
        delete [] rxy;
        delete [] rxz;

        delete [] ex;
        delete [] ey;
        delete [] ez;

        delete [] hx;
        delete [] hy;
        delete [] hz;

    }
#endif


    void EMEarth1D::SolveSingleTxRxPair (const int &irec,
                   HankelTransform *Hankel, const Real &wavef, const int &ifreq,
                   DipoleSource *tDipole) {
        ++icalcinner;

        Real rho = (Receivers->GetLocation(irec).head<2>() - tDipole->GetLocation().head<2>()).norm();

        tDipole->SetKernels(ifreq, FieldsToCalculate, Receivers, irec, Earth);
        Hankel->ComputeRelated( rho, tDipole->GetKernelManager() );
        tDipole->UpdateFields( ifreq,  Hankel, wavef );
    }

    void EMEarth1D::SolveLaggedTxRxPair(const int &irec, Hankel2* Hankel,
                    const Real &wavef, const int &ifreq, PolygonalWireAntenna* antenna) {

        antenna->ApproximateWithElectricDipoles(Receivers->GetLocation(irec));

        // Determine the min and max arguments
        Real rhomin = 1e9;
        Real rhomax = 1e-9;
        for (int idip=0; idip<antenna->GetNumberOfDipoles(); ++idip) {
            DipoleSource* tDipole = antenna->GetDipoleSource(idip);
            Real rho = (Receivers->GetLocation(irec).head<2>() - tDipole->GetLocation().head<2>()).norm();
            rhomin = std::min(rhomin, rho);
            rhomax = std::max(rhomax, rho);
        }
        //std::cout << "rhomin\t" << rhomin << "\trhomax" << rhomax << std::endl;

        // Determine number of lagged convolutions to do
        // TODO, can Hankel2 adjust the lagg spacing safely?
        int nlag = 1; // We need an extra for some reason for stability
        Real lrho ( 1.01* rhomax );
        while ( lrho > rhomin ) {
            nlag += 1;
            lrho *= Hankel->GetABSER();
        }

        //int nlag = rhomin
        DipoleSource* tDipole = antenna->GetDipoleSource(0);
        tDipole->SetKernels(ifreq, FieldsToCalculate, Receivers, irec, Earth);

        // Instead we should pass the antenna into this so that Hankel hass all the rho arguments...
        Hankel->ComputeLaggedRelated( 1.01* rhomax, nlag, tDipole->GetKernelManager() );

        //std::cout << Hankel->GetAnswer() << std::endl;
        //std::cout << Hankel->GetArg() << std::endl;


        // Sort the dipoles by rho

        for (int idip=0; idip<antenna->GetNumberOfDipoles(); ++idip) {
        //for (int idip=0; idip<1; ++idip) {
            DipoleSource* tDipole = antenna->GetDipoleSource(idip);
            tDipole->SetKernels(ifreq, FieldsToCalculate, Receivers, irec, Earth);
            // Pass Hankel2 a message here so it knows which one to return in Zgauss!
            Real rho = (Receivers->GetLocation(irec).head<2>() - tDipole->GetLocation().head<2>()).norm();
            //std::cout << " in Lagged " <<  rho << "\t" << rhomin << "\t" << rhomax << std::endl;
            Hankel->SetLaggedArg( rho );
            //std::cout << "out Lagged" << std::endl;
            tDipole->UpdateFields( ifreq,  Hankel, wavef );
        }
        //std::cout << "Spline\n";
        //std::cout << Receivers->GetHfield(0, irec) << std::endl;
    }

    //////////////////////////////////////////////////////////
    // Thread safe OO Reimplimentation of KiHand's
    // EM1DNEW.for programme
    void EMEarth1D::MakeCalc3() {

        if ( Dipole == NULL ) throw NullDipoleSource();

        if (Earth == NULL) throw NullEarth();

        if (Receivers == NULL) throw NullReceivers();

        #ifdef LEMMAUSEOMP
        #pragma omp parallel
        #endif
        { // OpenMP Parallel Block

            #ifdef LEMMAUSEOMP
            int tid = omp_get_thread_num();
            int nthreads = omp_get_num_threads();
            #else
            int tid=0;
            int nthreads=1;
            #endif

            DipoleSource* tDipole = Dipole->Clone();

            HankelTransform* Hankel;
            switch (HankelType) {
                case ANDERSON801:
                    Hankel = Hankel2::New();
                    break;
                case CHAVE:
                    Hankel = HankelTransformGaussianQuadrature::New();
                    break;
                case FHTKEY201:
                    Hankel = FHTKey::New();
                    break;
                case FHTKEY101:
                    Hankel = FHTKey101::New();
                    break;
                case FHTKEY51:
                    Hankel = FHTKey51::New();
                    break;
                case QWEKEY:
                    Hankel = QWEKey::New();
                    break;
                default:
                    std::cerr << "Hankel transform cannot be created\n";
                    exit(EXIT_FAILURE);
            }

            if ( tDipole->GetNumberOfFrequencies() < Receivers->GetNumberOfReceivers() ) {
                for (int ifreq=0; ifreq<tDipole->GetNumberOfFrequencies(); ++ifreq) {
                    // Propogation constant in free space being input to Hankel
                    Real wavef   = tDipole->GetAngularFrequency(ifreq) * std::sqrt(MU0*EPSILON0);
                    for (int irec=tid; irec<Receivers->GetNumberOfReceivers(); irec+=nthreads) {
                        SolveSingleTxRxPair(irec, Hankel, wavef, ifreq, tDipole);
                    }
                }
            } else {
                for (int irec=0; irec<Receivers->GetNumberOfReceivers(); ++irec) {
                    for (int ifreq=tid; ifreq<tDipole->GetNumberOfFrequencies(); ifreq+=nthreads) {
                        // Propogation constant in free space being input to Hankel
                        Real wavef   = tDipole->GetAngularFrequency(ifreq) * std::sqrt(MU0*EPSILON0);
                        SolveSingleTxRxPair(irec, Hankel, wavef, ifreq, tDipole);
                    }
                }
            }

            tDipole->Delete();
            Hankel->Delete();

        } // OpenMP Parallel Block
    }

    NullReceivers::NullReceivers() :
        runtime_error("NULL RECEIVERS") {}

    NullAntenna::NullAntenna() :
        runtime_error("NULL ANTENNA") {}

    NullInstrument::NullInstrument(LemmaObject* ptr) :
        runtime_error("NULL INSTRUMENT") {
        std::cout << "Thrown by instance of "
                  << ptr->GetName() << std::endl;
    }

    DipoleSourceSpecifiedForWireAntennaCalc::
        DipoleSourceSpecifiedForWireAntennaCalc() :
        runtime_error("DIPOLE SOURCE SPECIFIED FOR WIRE ANTENNA CALC"){}

} // end of Lemma Namespace
