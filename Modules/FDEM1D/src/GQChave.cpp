/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/02/2010
  @version  $Id: hankeltransformgaussianquadrature.cpp 200 2014-12-29 21:11:55Z tirons $
 **/

//    Description: Port of Alan Chave's gaussian quadrature algorithm, which
//                 is public domain, code listed in published article:
//          Chave, A. D., 1983, Numerical integration of related Hankel transforms by
//          quadrature and continued fraction expansion: Geophysics, 48
//          1671--1686  doi: 10.1190/1.1441448

#include "GQChave.h"

namespace Lemma{

    std::ostream &operator<<(std::ostream &stream, const GQChave &ob) {
        stream << ob.Serialize()  << "\n";
        return stream;
    }

    // Initialise static members
    const VectorXr GQChave::WT =
        (VectorXr(254) << //  (WT(I),I=1,20)
        0.55555555555555555556e+00,0.88888888888888888889e+00,
        0.26848808986833344073e+00,0.10465622602646726519e+00,
        0.40139741477596222291e+00,0.45091653865847414235e+00,
        0.13441525524378422036e+00,0.51603282997079739697e-01,
        0.20062852937698902103e+00,0.17001719629940260339e-01,
        0.92927195315124537686e-01,0.17151190913639138079e+00,
        0.21915685840158749640e+00,0.22551049979820668739e+00,
        0.67207754295990703540e-01,0.25807598096176653565e-01,
        0.10031427861179557877e+00,0.84345657393211062463e-02,
        0.46462893261757986541e-01,0.85755920049990351154e-01,
        //  (WT(I),I=21,40)
        0.10957842105592463824e+00,0.25447807915618744154e-02,
        0.16446049854387810934e-01,0.35957103307129322097e-01,
        0.56979509494123357412e-01,0.76879620499003531043e-01,
        0.93627109981264473617e-01,0.10566989358023480974e+00,
        0.11195687302095345688e+00,0.11275525672076869161e+00,
        0.33603877148207730542e-01,0.12903800100351265626e-01,
        0.50157139305899537414e-01,0.42176304415588548391e-02,
        0.23231446639910269443e-01,0.42877960025007734493e-01,
        0.54789210527962865032e-01,0.12651565562300680114e-02,
        0.82230079572359296693e-02,0.17978551568128270333e-01,
        //  (WT(I),I=41,60)
        0.28489754745833548613e-01,0.38439810249455532039e-01,
        0.46813554990628012403e-01,0.52834946790116519862e-01,
        0.55978436510476319408e-01,0.36322148184553065969e-03,
        0.25790497946856882724e-02,0.61155068221172463397e-02,
        0.10498246909621321898e-01,0.15406750466559497802e-01,
        0.20594233915912711149e-01,0.25869679327214746911e-01,
        0.31073551111687964880e-01,0.36064432780782572640e-01,
        0.40715510116944318934e-01,0.44914531653632197414e-01,
        0.48564330406673198716e-01,0.51583253952048458777e-01,
        0.53905499335266063927e-01,0.55481404356559363988e-01,
        //  (WT(I),I=61,80)
        0.56277699831254301273e-01,0.56377628360384717388e-01,
        0.16801938574103865271e-01,0.64519000501757369228e-02,
        0.25078569652949768707e-01,0.21088152457266328793e-02,
        0.11615723319955134727e-01,0.21438980012503867246e-01,
        0.27394605263981432516e-01,0.63260731936263354422e-03,
        0.41115039786546930472e-02,0.89892757840641357233e-02,
        0.14244877372916774306e-01,0.19219905124727766019e-01,
        0.23406777495314006201e-01,0.26417473395058259931e-01,
        0.27989218255238159704e-01,0.18073956444538835782e-03,
        0.12895240826104173921e-02,0.30577534101755311361e-02,
        //  (WT(I),I=81,100)
        0.52491234548088591251e-02,0.77033752332797418482e-02,
        0.10297116957956355524e-01,0.12934839663607373455e-01,
        0.15536775555843982440e-01,0.18032216390391286320e-01,
        0.20357755058472159467e-01,0.22457265826816098707e-01,
        0.24282165203336599358e-01,0.25791626976024229388e-01,
        0.26952749667633031963e-01,0.27740702178279681994e-01,
        0.28138849915627150636e-01,0.50536095207862517625e-04,
        0.37774664632698466027e-03,0.93836984854238150079e-03,
        0.16811428654214699063e-02,0.25687649437940203731e-02,
        0.35728927835172996494e-02,0.46710503721143217474e-02,
        //  (WT(I),I=101,120)
        0.58434498758356395076e-02,0.70724899954335554680e-02,
        0.83428387539681577056e-02,0.96411777297025366953e-02,
        0.10955733387837901648e-01,0.12275830560082770087e-01,
        0.13591571009765546790e-01,0.14893641664815182035e-01,
        0.16173218729577719942e-01,0.17421930159464173747e-01,
        0.18631848256138790186e-01,0.19795495048097499488e-01,
        0.20905851445812023852e-01,0.21956366305317824939e-01,
        0.22940964229387748761e-01,0.23854052106038540080e-01,
        0.24690524744487676909e-01,0.25445769965464765813e-01,
        0.26115673376706097680e-01,0.26696622927450359906e-01,
        //  (WT(I),I=121,140)
        0.27185513229624791819e-01,0.27579749566481873035e-01,
        0.27877251476613701609e-01,0.28076455793817246607e-01,
        0.28176319033016602131e-01,0.28188814180192358694e-01,
        0.84009692870519326354e-02,0.32259500250878684614e-02,
        0.12539284826474884353e-01,0.10544076228633167722e-02,
        0.58078616599775673635e-02,0.10719490006251933623e-01,
        0.13697302631990716258e-01,0.31630366082226447689e-03,
        0.20557519893273465236e-02,0.44946378920320678616e-02,
        0.71224386864583871532e-02,0.96099525623638830097e-02,
        0.11703388747657003101e-01,0.13208736697529129966e-01,
        //  (WT(I),I=141,160)
        0.13994609127619079852e-01,0.90372734658751149261e-04,
        0.64476204130572477933e-03,0.15288767050877655684e-02,
        0.26245617274044295626e-02,0.38516876166398709241e-02,
        0.51485584789781777618e-02,0.64674198318036867274e-02,
        0.77683877779219912200e-02,0.90161081951956431600e-02,
        0.10178877529236079733e-01,0.11228632913408049354e-01,
        0.12141082601668299679e-01,0.12895813488012114694e-01,
        0.13476374833816515982e-01,0.13870351089139840997e-01,
        0.14069424957813575318e-01,0.25157870384280661489e-04,
        0.18887326450650491366e-03,0.46918492424785040975e-03,
        //  (WT(I),I=161,180)
        0.84057143271072246365e-03,0.12843824718970101768e-02,
        0.17864463917586498247e-02,0.23355251860571608737e-02,
        0.29217249379178197538e-02,0.35362449977167777340e-02,
        0.41714193769840788528e-02,0.48205888648512683476e-02,
        0.54778666939189508240e-02,0.61379152800413850435e-02,
        0.67957855048827733948e-02,0.74468208324075910174e-02,
        0.80866093647888599710e-02,0.87109650797320868736e-02,
        0.93159241280693950932e-02,0.98977475240487497440e-02,
        0.10452925722906011926e-01,0.10978183152658912470e-01,
        0.11470482114693874380e-01,0.11927026053019270040e-01,
        //  (WT(I),I=181,200)
        0.12345262372243838455e-01,0.12722884982732382906e-01,
        0.13057836688353048840e-01,0.13348311463725179953e-01,
        0.13592756614812395910e-01,0.13789874783240936517e-01,
        0.13938625738306850804e-01,0.14038227896908623303e-01,
        0.14088159516508301065e-01,0.69379364324108267170e-05,
        0.53275293669780613125e-04,0.13575491094922871973e-03,
        0.24921240048299729402e-03,0.38974528447328229322e-03,
        0.55429531493037471492e-03,0.74028280424450333046e-03,
        0.94536151685852538246e-03,0.11674841174299594077e-02,
        0.14049079956551446427e-02,0.16561127281544526052e-02,
        //  (WT(I),I=201,220)
        0.19197129710138724125e-02,0.21944069253638388388e-02,
        0.24789582266575679307e-02,0.27721957645934509940e-02,
        0.30730184347025783234e-02,0.33803979910869203823e-02,
        0.36933779170256508183e-02,0.40110687240750233989e-02,
        0.43326409680929828545e-02,0.46573172997568547773e-02,
        0.49843645647655386012e-02,0.53130866051870565663e-02,
        0.56428181013844441585e-02,0.59729195655081658049e-02,
        0.63027734490857587172e-02,0.66317812429018878941e-02,
        0.69593614093904229394e-02,0.72849479805538070639e-02,
        0.76079896657190565832e-02,0.79279493342948491103e-02,
        //  (WT(I),I=221,240)
        0.82443037630328680306e-02,0.85565435613076896192e-02,
        0.88641732094824942641e-02,0.91667111635607884067e-02,
        0.94636899938300652943e-02,0.97546565363174114611e-02,
        0.10039172044056840798e-01,0.10316812330947621682e-01,
        0.10587167904885197931e-01,0.10849844089337314099e-01,
        0.11104461134006926537e-01,0.11350654315980596602e-01,
        0.11588074033043952568e-01,0.11816385890830235763e-01,
        0.12035270785279562630e-01,0.12244424981611985899e-01,
        0.12443560190714035263e-01,0.12632403643542078765e-01,
        0.12810698163877361967e-01,0.12978202239537399286e-01,
        //  (WT(I),I=241,254)
        0.13134690091960152836e-01,0.13279951743930530650e-01,
        0.13413793085110098513e-01,0.13536035934956213614e-01,
        0.13646518102571291428e-01,0.13745093443001896632e-01,
        0.13831631909506428676e-01,0.13906019601325461264e-01,
        0.13968158806516938516e-01,0.14017968039456608810e-01,
        0.14055382072649964277e-01,0.14080351962553661325e-01,
        0.14092845069160408355e-01,0.14094407090096179347e-01).finished();

        const VectorXr GQChave::WA =
        (VectorXr(127) << //  (WT(I),I=1,20)
        //  (WA(I),I=1,20)
        0.77459666924148337704e+00,0.96049126870802028342e+00,
        0.43424374934680255800e+00,0.99383196321275502221e+00,
        0.88845923287225699889e+00,0.62110294673722640294e+00,
        0.22338668642896688163e+00,0.99909812496766759766e+00,
        0.98153114955374010687e+00,0.92965485742974005667e+00,
        0.83672593816886873550e+00,0.70249620649152707861e+00,
        0.53131974364437562397e+00,0.33113539325797683309e+00,
        0.11248894313318662575e+00,0.99987288812035761194e+00,
        0.99720625937222195908e+00,0.98868475754742947994e+00,
        0.97218287474858179658e+00,0.94634285837340290515e+00,
        //  (WA(I),I=21,40)
        0.91037115695700429250e+00,0.86390793819369047715e+00,
        0.80694053195021761186e+00,0.73975604435269475868e+00,
        0.66290966002478059546e+00,0.57719571005204581484e+00,
        0.48361802694584102756e+00,0.38335932419873034692e+00,
        0.27774982202182431507e+00,0.16823525155220746498e+00,
        0.56344313046592789972e-01,0.99998243035489159858e+00,
        0.99959879967191068325e+00,0.99831663531840739253e+00,
        0.99572410469840718851e+00,0.99149572117810613240e+00,
        0.98537149959852037111e+00,0.97714151463970571416e+00,
        0.96663785155841656709e+00,0.95373000642576113641e+00,
        //  (WA(I),I=41,60)
        0.93832039777959288365e+00,0.92034002547001242073e+00,
        0.89974489977694003664e+00,0.87651341448470526974e+00,
        0.85064449476835027976e+00,0.82215625436498040737e+00,
        0.79108493379984836143e+00,0.75748396638051363793e+00,
        0.72142308537009891548e+00,0.68298743109107922809e+00,
        0.64227664250975951377e+00,0.59940393024224289297e+00,
        0.55449513263193254887e+00,0.50768775753371660215e+00,
        0.45913001198983233287e+00,0.40897982122988867241e+00,
        0.35740383783153215238e+00,0.30457644155671404334e+00,
        0.25067873030348317661e+00,0.19589750271110015392e+00,
        //  (WA(I),I=61,80)
        0.14042423315256017459e+00,0.84454040083710883710e-01,
        0.28184648949745694339e-01,0.99999759637974846462e+00,
        0.99994399620705437576e+00,0.99976049092443204733e+00,
        0.99938033802502358193e+00,0.99874561446809511470e+00,
        0.99780535449595727456e+00,0.99651414591489027385e+00,
        0.99483150280062100052e+00,0.99272134428278861533e+00,
        0.99015137040077015918e+00,0.98709252795403406719e+00,
        0.98351865757863272876e+00,0.97940628167086268381e+00,
        0.97473445975240266776e+00,0.96948465950245923177e+00,
        0.96364062156981213252e+00,0.95718821610986096274e+00,
        //  (WA(I),I=81,100)
        0.95011529752129487656e+00,0.94241156519108305981e+00,
        0.93406843615772578800e+00,0.92507893290707565236e+00,
        0.91543758715576504064e+00,0.90514035881326159519e+00,
        0.89418456833555902286e+00,0.88256884024734190684e+00,
        0.87029305554811390585e+00,0.85735831088623215653e+00,
        0.84376688267270860104e+00,0.82952219463740140018e+00,
        0.81462878765513741344e+00,0.79909229096084140180e+00,
        0.78291939411828301639e+00,0.76611781930376009072e+00,
        0.74869629361693660282e+00,0.73066452124218126133e+00,
        0.71203315536225203459e+00,0.69281376977911470289e+00,
        //  (WA(I),I=101,120)
        0.67301883023041847920e+00,0.65266166541001749610e+00,
        0.63175643771119423041e+00,0.61031811371518640016e+00,
        0.58836243444766254143e+00,0.56590588542365442262e+00,
        0.54296566649831149049e+00,0.51955966153745702199e+00,
        0.49570640791876146017e+00,0.47142506587165887693e+00,
        0.44673538766202847374e+00,0.42165768662616330006e+00,
        0.39621280605761593918e+00,0.37042208795007823014e+00,
        0.34430734159943802278e+00,0.31789081206847668318e+00,
        0.29119514851824668196e+00,0.26424337241092676194e+00,
        0.23705884558982972721e+00,0.20966523824318119477e+00,
        //  (WA(I),I=121,127)
        0.18208649675925219825e+00,0.15434681148137810869e+00,
        0.12647058437230196685e+00,0.98482396598119202090e-01,
        0.70406976042855179063e-01,0.42269164765363603212e-01,
        0.14093886410782462614e-01).finished();

/*
    const Real PI2    = 0.6366197723675813;
    const Real X01P   = 0.4809651115391545e01;
    const Real XMAX   = std::numeric_limits<Real>::max();
    const Real XSMALL = 0.9094947017729281e-12;
    const Real J0_X01 = 0.2404825557695772e01;
    const Real J0_X02 = 0.1043754397719454e-15;
    const Real J0_X11 = 0.5520078110286310e01;
    const Real J0_X12 = 0.8088597146146419e-16;
    const Real FUDGE  = 6.071532166000000e-18;
    const Real FUDGEX = 1.734723476000000e-18;
    const Real TWOPI1 = 0.6283185005187988e01;
    const Real TWOPI2 = 0.3019915981956752e-06;
    const Real RTPI2  = 0.7978845608028652e0;
    const Real XMIN   = std::numeric_limits<Real>::min();
    const Real J1_X01 = 0.3831705970207512e1;
    const Real J1_X02 = -0.5967810507509414e-15;
    const Real J1_X11 = 0.7015586669815619e1;
    const Real J1_X12 = -0.5382308663841630e-15;
*/

    GQChave::GQChave( const ctor_key& key ) : HankelTransform( key ) {
        karg.resize(255, 100);
        kern.resize(510, 100);
    }

    GQChave::GQChave( const YAML::Node& node, const ctor_key& key ) : HankelTransform(node, key) {

    }

    /////////////////////////////////////////////////////////////
    GQChave::~GQChave() {
    }

    /////////////////////////////////////////////////////////////
    std::shared_ptr<GQChave> GQChave::NewSP() {
        return std::make_shared<GQChave>( ctor_key() );
    }


    //--------------------------------------------------------------------------------------
    //       Class:  GQChave
    //      Method:  DeSerialize
    // Description:  Factory method, converts YAML node into object
    //--------------------------------------------------------------------------------------
    std::shared_ptr<GQChave> GQChave::DeSerialize( const YAML::Node& node ) {
        if (node.Tag() != "GQChave") {
            throw  DeSerializeTypeMismatch( "GQChave", node.Tag());
        }
        return std::make_shared<GQChave> ( node, ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  GQChave
    //      Method:  Serialize
    // Description:  Converts object into Serialized version
    //--------------------------------------------------------------------------------------
    YAML::Node GQChave::Serialize() const {
        YAML::Node node = HankelTransform::Serialize();
        node.SetTag( GetName() );
        //node["LayerConductivity"] = LayerConductivity;
        return node;
    }

    /////////////////////////////////////////////////////////////

    //--------------------------------------------------------------------------------------
    //       Class:  GQChave
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string GQChave::GetName (  ) const {
        return CName;
    }		// -----  end of method GQChave::GetName  -----

    Complex GQChave::
            Zgauss(const int &ikk, const EMMODE &mode,
                    const int &itype, const Real &rho, const Real &wavef,
                    KernelEM1DBase* Kernel){

        // TI, TODO, change calls to Zgauss to reflect this, go and fix so we
        // dont subract 1 from this everywhere
        //Kernel->SetIk(ikk+1);
        //Kernel->SetMode(mode);
        //ik   = ikk+1;
        //mode = imode;

        Real Besr(0);
        Real Besi(0);

        // Parameters
        int nl(1);           // Lower limit for gauss order to start comp
        int nu(7);           // upper limit for gauss order

#ifdef  LEMMA_SINGLE_PRECISION
        Real rerr = 1e-5;   // Error, for double Kihand set to .1e-10, .1e-11
        Real aerr = 1e-6;
#else      // -----  not LEMMA_SINGLE_PRECISION  -----
        Real rerr = 1e-11;  // Error, for double Kihand set to .1e-10, .1e-11
        Real aerr = 1e-12;
#endif     // -----  not LEMMA_SINGLE_PRECISION  -----

        int npcs(1);
        int inew(0);

        //const int NTERM = 100;
        //BESINT.karg.resize(255, NTERM);
        //BESINT.kern.resize(510, NTERM);
        //this->karg.setZero();
        //this->kern.setZero();

        Besautn(Besr, Besi, itype, nl, nu, rho, rerr, aerr, npcs, inew, wavef, Kernel);

        return Complex(Besr, Besi);
    }

    //////////////////////////////////////////////////////////////////
    void GQChave::
        Besautn(Real &besr, Real &besi,
                    const int &besselOrder,
                    const int &lowerGaussLimit,
                    const int &upperGaussLimit,
                    const Real &rho,
                    const Real &relativeError,
                    const Real &absError,
                    const int& numPieces,
                    int &inew,
                    const Real &aorb,
                    KernelEM1DBase* Kernel) {

        HighestGaussOrder      = 0;
        NumberPartialIntegrals = 0;
        NumberFunctionEvals    = 0;

        inew = 0;

        if (lowerGaussLimit > upperGaussLimit) {
            besr = 0;
            besi = 0;
            throw LowerGaussLimitGreaterThanUpperGaussLimit();
        }

        int ncntrl = 0;
        int nw = std::max(inew, 1);

        // temps
        Real besr_1(0);
        Real besi_1(0);
        int ierr(0);
        int ierr1(0);
        int ierr2(0);
        VectorXi xsum(1);
        //xsum.setZero(); // TODO xsum doesn't do a god damn thing
        int nsum(0);

        // Check for Rtud
        Bestrn(besr_1, besi_1, besselOrder, lowerGaussLimit, rho,
                        .1*relativeError, .1*absError,
                        numPieces, xsum, nsum, nw, ierr, ncntrl, aorb, Kernel);

        if (ierr != 0 && lowerGaussLimit == 7) {
            HighestGaussOrder = lowerGaussLimit;
            return;

        } else {

            Real oldr = besr_1;
            Real oldi = besi_1;

            for (int n=lowerGaussLimit+1; n<=upperGaussLimit; ++n) {
                int nw2 = 2;
                Bestrn(besr_1, besi_1, besselOrder, n, rho, .1*relativeError,
                        .1*absError, numPieces, xsum, nsum, nw2, ierr,
                        ncntrl, aorb, Kernel);
                if (ierr != 0 && n==7) {
                    besr_1 = oldr;
                    besi_1 = oldi;
                    HighestGaussOrder = n;
                    std::cerr << "CONVERGENCE FAILED AT SMALL ARGUMNENT!\n";
                    ierr1 = ierr + 10;
                    break;
                } else if (std::abs(besr_1-oldr) <=
                                relativeError*std::abs(besr_1)+absError &&
                           std::abs(besi_1-oldi) <=
                                relativeError*std::abs(besi_1)+absError) {
                    HighestGaussOrder = n;
                    break;
                } else {
                    oldr = besr_1;
                    oldi = besi_1;
                }
            }
        }

        inew   = 0;
        ncntrl = 1;
        nw=std::max(inew, 1);
        Real besr_2, besi_2;

        //karg.setZero();
        //kern.setZero();

        HighestGaussOrder      = 0;
        NumberPartialIntegrals = 0;
        NumberFunctionEvals    = 0;

        Bestrn(besr_2, besi_2, besselOrder, lowerGaussLimit, rho,
                        .1*relativeError, .1*absError,
                        numPieces, xsum, nsum, nw, ierr, ncntrl, aorb, Kernel);

        if (ierr != 0 && lowerGaussLimit == 7) {
            HighestGaussOrder = lowerGaussLimit;
            return;
        } else {

            Real oldr = besr_2;
            Real oldi = besi_2;

            for (int n=lowerGaussLimit+1; n<=upperGaussLimit; ++n) {

                int nw2 = 2;
                Bestrn(besr_2, besi_2, besselOrder, n, rho, .1*relativeError,
                        .1*absError, numPieces, xsum, nsum, nw2, ierr,
                        ncntrl, aorb, Kernel);

                if (ierr != 0 && n==7) {
                    besr_2 = oldr;
                    besi_2 = oldi;
                    HighestGaussOrder = n;
                    std::cerr << "CONVERGENCE FAILED AT SMALL ARGUMNENT!\n";
                    ierr2 = ierr + 20;
                    break;
                } else if (std::abs(besr_2-oldr) <=
                                relativeError*std::abs(besr_2)+absError &&
                           std::abs(besi_2-oldi) <=
                                relativeError*std::abs(besi_2)+absError) {
                    HighestGaussOrder = n;
                    break;
                } else {
                    oldr = besr_2;
                    oldi = besi_2;
                }
            }
        }

        besr = besr_1 + besr_2;
        besi = besi_1 + besi_2;
        ierr = ierr1 + ierr2;
        return;
    }

    /////////////////////////////////////////////////////////////
    void GQChave::
        Bestrn(Real &BESR, Real &BESI, const int &IORDER,
            const int &NG, const Real &rho,
            const Real &RERR, const Real &AERR, const int &NPCS,
            VectorXi &XSUM, int &NSUM, int &NEW,
            int &IERR, int &NCNTRL, const Real &AORB, KernelEM1DBase* Kernel) {


        Xr.setZero();
        Xi.setZero();
        Dr.setZero();
        Di.setZero();
        Sr.setZero();
        Si.setZero();
        Cfcor.setZero();
        Cfcoi.setZero();
        Dr.setZero();
        Di.setZero();

        Dr(0) = (Real)(-1);

        const int NTERM = 100;
        const int NSTOP = 100;

        int NPO;

        //std::cout << "Bestrn NEW " << NEW << std::endl;

        if (NEW == 2) {
            NPO = nps;
        } else {
            Nk.setZero();
            nps = 0;
            NPO=NTERM;
        }

        // Trivial?
        if (IORDER != 0 && rho == 0) {
            BESR = 0;
            BESI = 0;
            IERR = 0;
            return;
        }

        NumberPartialIntegrals=0;
        int  NW    = NEW;
        np  = 0; // TI, zero based indexing

        int  NPB   = 1; // 0?
        int  L     = 0; // TODO, should be 0?
        Real B     = 0.;
        Real A     = 0.;
        Real SUMR  = 0.;
        Real SUMI  = 0.;
        Real XSUMR = 0.;
        Real XSUMI = 0.;
        Real TERMR(0), TERMI(0);

        // COMPUTE BESSEL TRANSFORM EXPLICITLY ON (0,XSUM(NSUM))
        if (NSUM > 0) {
            std::cerr << "NSUM GREATER THAN ZERO UNTESTED" << std::endl;
            Real LASTR=0.0;
            Real LASTI=0.0;
            for (int N=1; N<=NSUM; ++N) {
                if (NW == 2 && np > NPO) NW=1;
                if (np > NTERM) NW=0;
                A=B;
                B=XSUM(N);
                Besqud(A, B, TERMR, TERMI, NG, NW, IORDER, rho, Kernel);

                XSUMR += TERMR;
                XSUMI += TERMI;
                if ( (std::abs(XSUMR-LASTR) <= RERR*std::abs(XSUMR)+AERR) &&
                     (std::abs(XSUMI-LASTI) <= RERR*std::abs(XSUMI)+AERR)) {
                    BESR=XSUMR;
                    BESI=XSUMI;
                    IERR=0;
                    nps = std::max(np, nps);
                    return;
                } else {
                    ++np;
                    LASTR=XSUMR;
                    LASTI=XSUMI;
                }
            }
            while (ZeroJ(NPB,IORDER) > XSUM(NSUM*rho)) {
                ++NPB;
            }
        }

        // ENTRY POINT FOR PADE SUMMATION OF PARTIAL INTEGRANDS
        Real LASTR=0.e0;
        Real LASTI=0.e0;
        if (NCNTRL == 0) {
            A = 0.;
            B = ZeroJ(NPB,IORDER) / rho;
            if (B > AORB) {
                B = AORB;
            }
        } else {
            A = AORB;
            B = ZeroJ(NPB,IORDER)/rho;
            while (B <= A) {
                ++NPB;
                B = ZeroJ(NPB,IORDER)/rho;
            }
        }

        // CALCULATE TERMS AND SUM WITH PADECF, QUITTING WHEN CONVERGENCE IS
        // OBTAINED
        if (NCNTRL != 0) {

            for (int N=1; N<=NSTOP; ++N) {

                if (NPCS == 1) {

                    if ((NW==2) && (np > NPO)) {
                        NW=1;
                    }

                    if (np > NTERM) {
                        NW=0;
                    }

                    Besqud(A,B,TERMR,TERMI,NG,NW,IORDER,rho,Kernel);

                    ++np;

                } else {
                    std::cout << "In the else conditional\n";
                    TERMR=0.;
                    TERMI=0.;
                    Real XINC=(B-A)/NPCS;
                    Real AA=A;
                    Real BB=A+XINC;
                    for (int I=1; I<=NPCS; ++I) {
                        if ((NW == 2) && (np > NPO)) NW=1;
                        if (np > NTERM) NW=0;
                        Real TR, TI;
                        Besqud(AA, BB, TR, TI, NG, NW, IORDER, rho, Kernel);
                        TERMR+=TR;
                        TERMI+=TI;
                        AA=BB;
                        BB=BB+XINC;
                        ++np;
                    }
                }

                ++NumberPartialIntegrals;
                Sr(L) = TERMR;
                Si(L) = TERMI;

                Padecf(SUMR,SUMI,L);

                if ((std::abs(SUMR-LASTR) <= RERR*std::abs(SUMR)+AERR) &&
                    (std::abs(SUMI-LASTI) <= RERR*std::abs(SUMI)+AERR)) {
                    BESR=XSUMR+SUMR;
                    BESI=XSUMI+SUMI;
                    IERR=0;
                    nps = std::max(np-1, nps);
                    return;
                } else {
                    LASTR=SUMR;
                    LASTI=SUMI;
                    ++NPB;
                    A=B;
                    B=ZeroJ(NPB,IORDER)/rho;
                    ++L;
                }
            }

            BESR=XSUMR+SUMR;
            BESI=XSUMI+SUMI;
            IERR=1;
            nps = std::max(np, nps);
            return;
        }

        // GUSSIAN QUADRATURE ONLY IN THE INTERVAL IN WHICH K_0 IS LESS THAN
        // LAMBDA
        //std::cout << "NCNTRL " << NCNTRL << " NPCS " << NPCS << std::endl;
        for (int N=1; N<=NSTOP; ++N) {

            //std::cout << "NSTOP" << NSTOP << std::endl;

            if (NPCS ==  1) {

                if ((NW==2) && (np > NPO)) {
                    NW=1;
                }

                if (np > NTERM) {
                    NW=0;
                }

                // Problem here, A not getting initialised
                //std::cout << "A " << A << " B " << B << std::endl;
                Besqud(A, B, TERMR, TERMI, NG, NW, IORDER, rho, Kernel);

                ++np;

            } else {

                TERMR=0.;
                TERMI=0.;
                Real XINC=(B-A)/NPCS;
                Real AA=A;
                Real BB=A+XINC;
                Real TR(0), TI(0);
                for (int I=1; I<NPCS; ++I) {

                    if ((NW == 2) && (np > NPO)) {
                        NW=1;
                    }

                    if (np > NTERM) {
                        NW=0;
                    }
                    //std::cout << "AA " << AA << " BB " << BB << std::endl;
                    Besqud(AA, BB, TR, TI, NG, NW, IORDER, rho, Kernel);
                    TERMR+=TR;
                    TERMI+=TI;
                    AA=BB;
                    BB=BB+XINC;
                    ++np;
                }
            }

            if (B >= AORB) {
                BESR=XSUMR+TERMR;
                BESI=XSUMI+TERMI;
                IERR=0;
                nps = std::max(np, nps); // TI np is zero based
                return;
            } else {
                XSUMR = XSUMR + TERMR;
                XSUMI = XSUMI + TERMI;
                ++NPB;
                A = B;
                B = ZeroJ(NPB,IORDER)/rho;
                if (B >= AORB) B = AORB;
            }
        }
        return;
    }


    /////////////////////////////////////////////////////////////
    void GQChave::
            Besqud(const Real &LowerLimit, const Real &UpperLimit,
                    Real &Besr, Real &Besi, const int &npoints,
                    const int &NEW,
                    const int &besselOrder, const Real &rho,
                    KernelEM1DBase* Kernel) {

        const int NTERM = 100;

        // Filter Weights
        Eigen::Matrix<int ,   7, 1>    NWA;
        Eigen::Matrix<int ,   7, 1>    NWT;
        Eigen::Matrix<Real, 254, 1>    FUNCT;
        Eigen::Matrix<Real,  64, 1>    FR1;
        Eigen::Matrix<Real,  64, 1>    FI1;
        Eigen::Matrix<Real,  64, 1>    FR2;
        Eigen::Matrix<Real,  64, 1>    FI2;
        Eigen::Matrix<Real,  64, 1>    BES1;
        Eigen::Matrix<Real,  64, 1>    BES2;

        // Init Vals
        NWT << 1, 3, 7, 15, 31, 63, 127;
        NWA << 1, 2, 4,  8, 16, 32,  64;

        // CHECK FOR TRIVIAL CASE
        if (LowerLimit >= UpperLimit) {
            Besr = 0.;
            Besi = 0.;
            //std::cout << "Lower limit > Upper limit " << NEW << std::endl;
            //std::cout << "Lower limit " << LowerLimit << "Upper Limit " << UpperLimit <<std::endl;
            return;
        }

        // Temp vars for function generation
        Real diff(0);
        Real FZEROR(0);
        Real FZEROI(0);
        Real SUM(0);
        int  KN(0);
        int  LA(0);
        int  LK(0);
        int  N(0);

        int K;
        int NW;
        Real ACUMR;
        Real ACUMI;
        Real BESF;

        switch (NEW) {

            // CONSTRUCT FUNCT FROM SAVED KERNELS
            case (2):

                diff = (UpperLimit-LowerLimit)/2.;
                FZEROR = kern(0, np);
                FZEROI = kern(1, np);

                K = std::min(Nk(np)-1, npoints-1);
                NW=NWT(K);

                LK=2;

                for (N=0; N<2*NW; N+=2) {
                    FUNCT(N)   = kern(LK  , np) +
                                 kern(LK+2, np) ;

                    FUNCT(N+1) = kern(LK+1, np) +
                                 kern(LK+3, np) ;
                    LK=LK+4;
                }

                if (Nk(np) >= npoints) {
                    ACUMR = _dot(NW, WT.tail(255-NW), 1, FUNCT,             2);
                    ACUMI = _dot(NW, WT.tail(255-NW), 1, FUNCT.tail<253>(), 2);
                    Besr  = (ACUMR+WT(2*NW)*FZEROR)*diff;
                    Besi  = (ACUMI+WT(2*NW)*FZEROI)*diff;
                    return;

                } else {

                    // COMPUTE ADDITIONAL ORDERS BEFORE TAKING DOT PRODUCT
                    SUM= (UpperLimit+LowerLimit)/2.;
                    KN  = K +1;
                    LA  = LK/2;

                }

                break;

            default:

                // SCALE FACTORS
                SUM  = (UpperLimit+LowerLimit) / 2.;
                diff = (UpperLimit-LowerLimit) / 2.;

                // ONE POINT GAUSS
                //Kernel(SUM,FZEROR,FZEROI,TEM,GEOM,PARA);
                Complex tba = Kernel->BesselArg(SUM);
                //cout.precision(16);
                //std::cout << "SUM " << "\t" << SUM << "\t" << tba << "\n";
                ++NumberFunctionEvals;

                BESF = Jbess(SUM*rho,besselOrder);
                FZEROR = BESF*std::real(tba);//FZEROR;
                FZEROI = BESF*std::imag(tba);//FZEROI;

                if (NEW == 1) {
                    karg(0, np) = SUM;
                    kern(0, np) = FZEROR;
                    kern(1, np) = FZEROI;
                    LA=1;
                    LK=2;
                }
                N  = 0; // TI was 1, 0 based index
                KN = 0;

        }  // End of new switch

        // STEP THROUGH GAUSS ORDERS (NG = Number Gauss)
        for (int K=KN; K<npoints; ++K) {

            // COMPUTE NEW FUNCTION VALUES
            int NA = NWA(K) - 1; // NWA assumes 1 based indexing

            //std::cout << "Besqud NA=" << NA << std::endl;
            for (int J=0; J<NWA(K); ++J) {

                Real X = WA(NA)*diff;

                Real SUMP = SUM+X;
                Real SUMM = SUM-X;

                ++NA;

                //std::cout << "Calling from here\n";
                //Kernel(SUMP, FR1(J), FI1(J), TEM, GEOM, PARA);
                //Kernel(SUMM, FR2(J), FI2(J), TEM, GEOM, PARA);
                Complex bes1 = Kernel->BesselArg(SUMP);
                //std::cout << "SUMP" << "\t" << SUMP << "\t" << bes1 << "\n";
                Complex bes2 = Kernel->BesselArg(SUMM);
                //std::cout << "SUMM" << "\t" << SUMM << "\t" << bes1 << "\n";
                FR1(J) = std::real(bes1);
                FI1(J) = std::imag(bes1);
                FR2(J) = std::real(bes2);
                FI2(J) = std::imag(bes2);

                NumberFunctionEvals +=2;
                BES1(J)=Jbess(SUMP*rho, besselOrder);
                BES2(J)=Jbess(SUMM*rho, besselOrder);

                if (NEW >= 1) {
                    karg(LA  , np) = SUMP;
                    karg(LA+1, np) = SUMM;
                    LA += 2;
                }
            }

            // COMPUTE PRODUCTS OF KERNELS AND BESSEL FUNCTIONS
            // TODO vectorize
            for (int J=0; J<NWA(K); ++J) {
                FR1(J    ) = BES1(J)*FR1(J);
                FI1(J    ) = BES1(J)*FI1(J);
                FR2(J    ) = BES2(J)*FR2(J);
                FI2(J    ) = BES2(J)*FI2(J);
                FUNCT(N  ) = FR1(J)+FR2(J);
                FUNCT(N+1) = FI1(J)+FI2(J);
                N+=2;
            }

            if (NEW >= 1) {
                for (int J=0; J<NWA(K); ++J) {
                    kern(LK  , np) = FR1(J);
                    kern(LK+1, np) = FI1(J);
                    kern(LK+2, np) = FR2(J);
                    kern(LK+3, np) = FI2(J);
                    LK=LK+4;
                }
            }

        }

        // COMPUTE DOT PRODUCT OF WEIGHTS WITH INTEGRAND VALUES
        NW=NWT(npoints-1);
        ACUMR = _dot(NW, WT.tail(255-NW), 1, FUNCT            , 2);
        ACUMI = _dot(NW, WT.tail(255-NW), 1, FUNCT.tail<253>(), 2);
        Besr = (ACUMR+WT(2*NW-1)*FZEROR)*diff;
        Besi = (ACUMI+WT(2*NW-1)*FZEROI)*diff;
        if (np <= NTERM) Nk(np) = npoints;
        return;
    }


    /////////////////////////////////////////////////////////////
    void GQChave::
            Padecf(Real &SUMR, Real &SUMI, const int &N) {

        if (N < 2) {

            // INITIALIZE FOR RECURSIVE CALCULATIONS
            //if (N == 0) {
            //    Xr.setZero();
            //    Xi.setZero();
                //Xr = Eigen::Matrix<Real, Eigen::Dynamic, 1>::Zero(100);
                //Xi = Eigen::Matrix<Real, Eigen::Dynamic, 1>::Zero(100);
            //}

            Dr(N) = Sr(N);
            Di(N) = Si(N);

            Real DENOM;
            switch (N) {

                case 0:
                    DENOM = 1;
                    Cfcor(N) = -(-1.*Dr(N)) / DENOM;
                    Cfcoi(N) = -(-1.*Di(N)) / DENOM;
                    break;

                default:
                    DENOM = Dr(N-1)*Dr(N-1) +
                            Di(N-1)*Di(N-1);

                    Cfcor(N) = -(Dr(N-1)*Dr(N)   +
                                        Di(N-1)*Di(N)  )  / DENOM;
                    Cfcoi(N) = -(Dr(N-1)*Di(N  ) -
                                        Dr(N  )*Di(N-1) ) / DENOM;
            }

            CF(SUMR, SUMI, Cfcor, Cfcoi, N);
            return;

        } else {
            int L = 2*(int)((N)/2) - 1;//+1;// - 1; // - 1; // - 1;

            // UPDATE X VECTORS FOR RECURSIVE CALCULATION OF CF COEFFICIENTS
            for (int K = L; K >= 3; K-= 2) {
                Xr(K) = Xr(K-1)+Cfcor(N-1)*Xr(K-2) -
                               Cfcoi(N-1)*Xi(K-2);

                Xi(K) = Xi(K-1)+Cfcor(N-1)*Xi(K-2) +
                               Cfcoi(N-1)*Xr(K-2);
            }

            Xr(1) = Xr(0)+Cfcor(N-1);
            Xi(1) = Xi(0)+Cfcoi(N-1);

            // INTERCHANGE ODD AND EVEN PARTS
            for (int K=0; K<L; K+=2) {
                Real T1 = Xr(K);
                Real T2 = Xi(K);
                Xr(K) = Xr(K+1);
                Xi(K) = Xi(K+1);
                Xr(K+1) = T1;
                Xi(K+1) = T2;
            }

            // COMPUTE FIRST COEFFICIENTS
            Dr(N) = Sr(N);
            Di(N) = Si(N);
            // Dr getting fucked up here
            for (int K=0; K<std::max(1, L/2+1); ++K) {
                Dr(N) += Sr(N-K-1)*Xr(2*K) -
                                Si(N-K-1)*Xi(2*K);
                Di(N) += Si(N-K-1)*Xr(2*K) +
                                Sr(N-K-1)*Xi(2*K);
            }

            // COMPUTE NEW CF COEFFICIENT
            Real DENOM = Dr(N-1)*Dr(N-1) +
                         Di(N-1)*Di(N-1);

            //std::cout << "DENOM " << DENOM << std::endl;
            Cfcor(N)=-(Dr(N  )*Dr(N-1) +
                              Di(N)*Di(N-1))/DENOM;
            Cfcoi(N)=-(Dr(N-1)*Di(N  ) -
                              Dr(N)*Di(N-1))/DENOM;

            // EVALUATE CONTINUED FRACTION
            CF(SUMR,SUMI,Cfcor,Cfcoi,N);

            return;
        }
    }

    /////////////////////////////////////////////////////////////
    void GQChave::CF(Real& RESR, Real &RESI,
                    Eigen::Matrix<Real, 100, 1> &CFCOR,
                    Eigen::Matrix<Real, 100, 1> &CFCOI,
                    const int &N) {

        ////////////////////////////////////////////////
        // ONE Seems sort of stupid, maybe use ? instead
        // TODO benchmark difference

        //RESR = ONE(N)+CFCOR(N);
        RESR = (!N ? 0:1) + CFCOR(N);
        RESI = CFCOI(N);

        for (int K=N-1; K>=0; --K) {
            Real DENOM=RESR*RESR + RESI*RESI;
            Real RESRO=RESR;
            //RESR=ONE(K)+(RESR*CFCOR(K)+RESI*CFCOI(K))/DENOM;
            RESR = (!K ? 0:1) + (RESR*CFCOR(K)+RESI*CFCOI(K))/DENOM;
            RESI=(RESRO*CFCOI(K)-RESI*CFCOR(K))/DENOM;
        }

        return;
    }

    /////////////////////////////////////////////////////////////
    Real GQChave::
        ZeroJ(const int &nzero, const int &besselOrder) {

        Real ZT1 = -1.e0/8.e0;
        Real ZT2 =  124.e0/1536.e0;
        Real ZT3 = -120928.e0/491520.e0;
        Real ZT4 =  401743168.e0/220200960.e0;
        Real OT1 =  3.e0/8.e0;
        Real OT2 = -36.e0/1536.e0;
        Real OT3 =  113184.e0/491520.e0;
        Real OT4 = -1951209.e0/220200960.e0;

        Real BETA(0);
        switch (besselOrder) {

            case 0:
                BETA=(nzero-.25e0)*PI;
                return BETA-ZT1/BETA-ZT2/std::pow(BETA,3) -
                         ZT3/std::pow(BETA,5)-ZT4/std::pow(BETA,7);
            case 1:
                BETA=(nzero+.25e0)*PI;
                return BETA-OT1/BETA-OT2/std::pow(BETA,3) -
                        OT3/std::pow(BETA,5)-OT4/std::pow(BETA,7);
            default:
                throw 77;
        }
        return 0.;
    }


    /////////////////////////////////////////////////////////////
    // Dot product allowing non 1 based incrementing
    Real GQChave::_dot(const int&n,
        const Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> &X1,
                const int &inc1,
        const Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic> &X2,
        const int &inc2) {

        int k;
        if (inc2 > 0) {
            k=0;
        } else {
            k = n*std::abs(inc2);
        }

        Real dot=0.0;
        if (inc1 > 0) {
            for (int i=0; i<n; i += inc1) {
                dot += X1(i)*X2(k);
                k += inc2;
            }
        } else {
            for (int i=n-1; i>=0; i += inc1) {
                dot += X1(i)*X2(k);
                k += inc2;
            }
        }

        return dot;
    }

    /////////////////////////////////////////////////////////////
    //
#ifdef HAVE_BOOST_SPECIAL_FUNCTIONS
    Real GQChave::Jbess(const Real &x, const int &IORDER) {
        switch (IORDER) {
            case 0:
                //return boost::math::detail::bessel_j0(X);
                return boost::math::cyl_bessel_j(0, x);
                break;
            case 1:
                //return boost::math::detail::bessel_j1(x);
                return boost::math::cyl_bessel_j(1, x);
                break;
            default:
                throw std::runtime_error("Non 0 or 1 Bessel argument specified in GQChave");
        }
    }
#else
    Real GQChave::Jbess(const Real &x, const int &IORDER) {
        std::cerr << "GQChave requires boost special functions module";
    }
#endif
    //////////////////////////////////////////////////////
    // Exception classes

    LowerGaussLimitGreaterThanUpperGaussLimit::
        LowerGaussLimitGreaterThanUpperGaussLimit() :
        runtime_error("LOWER GAUSS LIMIT > UPPER GAUSS LIMIT") {}

}       // -----  end of Lemma  name  -----

