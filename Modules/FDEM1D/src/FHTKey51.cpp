/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      02/11/2014 03:42:53 PM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "FHTKey51.h"

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const FHTKey51 &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  STATIC CONST MEMBERS     ============

    const Eigen::Matrix<Real, 51, 3>  FHTKey51::WT51 =
        ( Eigen::Matrix<Real, 51, 3>()   <<
        // Base                   J0                      J1
           4.9915939069102170e-03,   6.5314496156480717e-02,   3.8409924166118657e-05,
           6.1703482511978082e-03,  -2.2265895125268051e-01,  -9.4382633059055269e-05,
           7.6274629409160176e-03,   4.2794928430042073e-01,   1.6377612356675232e-04,
           9.4286721829279947e-03,  -5.8890658905204007e-01,  -1.7853487610334151e-04,
           1.1655233178024955e-02,   6.7897324897038758e-01,   1.9759569268356316e-04,
           1.4407591843112350e-02,  -6.8296549495756353e-01,  -1.2796247718553291e-04,
           1.7809914185937643e-02,   6.4702210424566342e-01,   1.3163370416771999e-04,
           2.2015687754376449e-02,  -5.6843090153083187e-01,   1.7066927559149181e-05,
           2.7214645856121149e-02,   4.9960500362966542e-01,   4.1368007808198310e-05,
           3.3641326918204623e-02,  -4.0923517268678855e-01,   2.3582259002758631e-04,
           4.1585655121173182e-02,   3.5487831784012780e-01,  -1.9929745662753875e-05,
           5.1406019627642255e-02,  -2.7408257835871558e-01,   5.8196110748926486e-04,
           6.3545442443014022e-02,   2.4668368971888163e-01,   1.1164151202379049e-05,
           7.8551564282309541e-02,  -1.7348161774657314e-01,   1.2053942009238238e-03,
           9.7101350057185065e-02,   1.7509173908347012e-01,   2.8328713800714110e-04,
           1.2003162851145673e-01,  -1.0007796678766585e-01,   2.4375075610646724e-03,
           1.4837684372696586e-01,   1.3297302593795621e-01,   1.1709289200267103e-03,
           1.8341572156771246e-01,  -4.3822390117083297e-02,   5.0080095645252103e-03,
           2.2672895630607542e-01,   1.1415804492335258e-01,   3.5856849047131551e-03,
           2.8027051982381157e-01,   4.2584271963990363e-03,   1.0536604059335859e-02,
           3.4645581033005746e-01,   1.1496183925974515e-01,   9.6674718436839564e-03,
           4.2827061721265969e-01,   5.1313336759457658e-02,   2.2583365267100426e-02,
           5.2940581770869455e-01,   1.3312389010285514e-01,   2.4201911801947001e-02,
           6.5442388190885858e-01,   1.0111673764393908e-01,   4.8602551164692191e-02,
           8.0896469756649991e-01,   1.6331446488042209e-01,   5.6856974938076565e-02,
           1.0000000000000000e+00,   1.4704406144951229e-01,   1.0202078096072606e-01,
           1.2361478850785035e+00,   1.8341391219815678e-01,   1.2202639829343022e-01,
           1.5280615937840571e+00,   1.5073655145087875e-01,   1.9561985197118459e-01,
           1.8889101074258496e+00,   1.2356364992866918e-01,   2.1570853920674185e-01,
           2.3349722343978727e+00,   1.1529465836107913e-02,   2.8552388758852060e-01,
           2.8863709892679585e+00,  -1.2852626053530605e-01,   2.0559964796530625e-01,
           3.5679813939355345e+00,  -2.9874216118407571e-01,   1.1666011451374350e-01,
           4.4105526541128617e+00,  -3.1330553036684633e-01,  -2.3636714743601936e-01,
           5.4520953354089947e+00,  -3.2332453606385231e-02,  -3.4036030266164258e-01,
           6.7395961181122024e+00,   4.2056051544727674e-01,  -1.9475364255836261e-01,
           8.3311374876876929e+00,   1.3883637369942961e-01,   5.7608115965662166e-01,
           1.0298517985703377e+01,  -5.0223684329542873e-01,  -2.1471464584834585e-01,
           1.2730491227470161e+01,   3.7996073748731690e-01,  -1.2611070492338947e-01,
           1.5736769806847677e+01,  -1.7984404170109178e-01,   2.0019703515125831e-01,
           1.9452974714702009e+01,   7.0557286965350657e-02,  -1.6147664664730754e-01,
           2.4046753552064487e+01,  -2.7522277139686569e-02,   1.1398023016900673e-01,
           2.9725343546388515e+01,   1.1909130661901874e-02,  -7.9924455203352035e-02,
           3.6744920558100112e+01,  -5.8977428045126450e-03,   5.7024556382799692e-02,
           4.5422155835273074e+01,   3.2648420034561555e-03,  -4.1006636508593382e-02,
           5.6148501871478999e+01,  -1.9304039780209286e-03,   2.9192063091319696e-02,
           6.9407851838755207e+01,   1.1594138315987755e-03,  -2.0089573135329029e-02,
           8.5798369258319340e+01,  -6.7050941490060716e-04,   1.2965400526524978e-02,
           1.0605947270185592e+02,   3.5089803807606258e-04,  -7.5154910983038946e-03,
           1.3110519287294045e+02,  -1.5287051174029577e-04,   3.6269079130258285e-03,
           1.6206540689269471e+02,   4.8238483411813232e-05,  -1.2668720233377250e-03,
           2.0033680997479166e+02,  -8.0433917146487977e-06,   2.3403502580547994e-04).finished();

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  FHTKey51
    // Description:  constructor (locked)
    //--------------------------------------------------------------------------------------
    FHTKey51::FHTKey51 (const ctor_key& ) : HankelTransform( ) {

    }  // -----  end of method FHTKey51::FHTKey51  (constructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  FHTKey51
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    FHTKey51::FHTKey51( const YAML::Node& node, const ctor_key& ) : HankelTransform(node) {

    }

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    std::shared_ptr<FHTKey51> FHTKey51::NewSP() {
        return std::make_shared< FHTKey51 >( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  ~FHTKey51
    // Description:  destructor
    //--------------------------------------------------------------------------------------
    FHTKey51::~FHTKey51 () {

    }  // -----  end of method FHTKey51::~FHTKey51  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  DeSerialize
    // Description:  Factory method, converts YAML node into object
    //--------------------------------------------------------------------------------------
    std::shared_ptr<FHTKey51> FHTKey51::DeSerialize( const YAML::Node& node ) {
        if (node.Tag() != "FHTKey51") {
            throw  DeSerializeTypeMismatch( "FHTKey51", node.Tag());
        }
        return std::make_shared<FHTKey51> ( node, ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  Serialize
    // Description:  Converts object into Serialized version
    //--------------------------------------------------------------------------------------
    YAML::Node FHTKey51::Serialize() const {
        YAML::Node node = HankelTransform::Serialize();
        node.SetTag( GetName() );
        //node["LayerConductivity"] = LayerConductivity;
        return node;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string FHTKey51::GetName (  ) const {
        return CName;
    }		// -----  end of method FHTKey51::GetName  -----

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  Zgauss
    //--------------------------------------------------------------------------------------
    Complex FHTKey51::Zgauss ( const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, KernelEM1DBase* Kernel ) {
 		return Zans(0, Kernel->GetManagerIndex());
    }		// -----  end of method FHTKey51::ComputeRelated  -----


    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void FHTKey51::ComputeRelated ( const Real& rho, std::shared_ptr<KernelEM1DBase> Kernel ) {
        return ;
    }		// -----  end of method FHTKey51::ComputeRelated  -----

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void FHTKey51::ComputeRelated ( const Real& rho, std::vector< std::shared_ptr<KernelEM1DBase> > KernelVec ) {
        return ;
    }		// -----  end of method FHTKey51::ComputeRelated  -----

    //--------------------------------------------------------------------------------------
    //       Class:  FHTKey51
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void FHTKey51::ComputeRelated ( const Real& rho, std::shared_ptr<KernelEM1DManager> KernelManager ) {

        //kernelVec = KernelManager->GetSTLVector();
        int nrel = (int)(KernelManager->GetSTLVector().size());
        Eigen::Matrix<Complex, 51, Eigen::Dynamic > Zwork;
        // TODO, if we want to allow lagged, then 1 below should be nlag
        Zans= Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(1, nrel);
        Zwork.resize(51, nrel);
        VectorXr lambda = WT51.col(0).array()/rho;
        int NumFun = 0;
        int idx = 0;

        // Get Kernel values
        for (int ir=0; ir<lambda.size(); ++ir) {
            // irelated loop
            ++NumFun;
            KernelManager->ComputeReflectionCoeffs(lambda(ir), idx, rho);
            for (int ir2=0; ir2<nrel; ++ir2) {
                // Zwork* needed due to sign convention of filter weights
 			    Zwork(ir, ir2) = std::conj(KernelManager->GetSTLVector()[ir2]->RelBesselArg(lambda(ir)));
            }

        }

        // We diverge slightly from Key here, each kernel is evaluated seperately, whereby instead
        // they prefer to sum them. The reason is that all those terms have been removed from the kernels
        // in the interests of making them as generic and reusable as possible. This approach requires slightly
        // more multiplies, but the same number of kernel evaluations, which is the expensive part.
        // Inner product and scale
        for (int ir2=0; ir2<nrel; ++ir2) {
            Zans(0, ir2) = Zwork.col(ir2).dot(WT51.col(KernelManager->GetSTLVector()[ir2]->GetBesselOrder() + 1))/rho;
        }

        return ;
    }		// -----  end of method FHTKey51::ComputeRelated  -----

}		// -----  end of Lemma  name  -----
