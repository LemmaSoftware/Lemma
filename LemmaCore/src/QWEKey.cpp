/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*      Original code is port of algorithm published by Key2011
        %------------------------------------------------------------------%
        % Copyright (c) 2012 by the Society of Exploration Geophysicists.  %
        % For more information, go to http://software.seg.org/2012/0003 .  %
        % You must read and accept usage terms at:                         %
        % http://software.seg.org/disclaimer.txt before use.               %
        %------------------------------------------------------------------%
*/

/**
 * @file
 * @date      02/12/2014 10:28:15 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2014, XRI Geophysics, LLC
 * @copyright Copyright (c) 2014, Trevor Irons
 */

#include "QWEKey.h"

//#include <Eigen/Eigenvalues>

namespace Lemma {

    // ====================  FRIEND METHODS  =====================

    std::ostream &operator<<(std::ostream &stream, const QWEKey &ob) {

        stream << *(HankelTransform*)(&ob);

        return stream;
    }

    // ====================  LIFECYCLE     =======================

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  QWEKey
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    //
    QWEKey::QWEKey (const std::string& name) : HankelTransform(name), RelTol(1e-12), AbsTol(1e-32), nQuad(61), nDelay(1),
    //QWEKey::QWEKey (const std::string& name) : HankelTransform(name), RelTol(1e-38), AbsTol(1e-48), nQuad(39), nDelay(5),
        nIntervalsMax(40) {
        BesselWeights( J0 ); // TODO experiment with zero weight (J0, J1) options, should be static one time method
    }  // -----  end of method QWEKey::QWEKey  (constructor)  -----


    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  New()
    // Description:  public constructor
    //--------------------------------------------------------------------------------------
    QWEKey* QWEKey::New() {
        QWEKey*  Obj = new QWEKey("QWEKey");
        Obj->AttachTo(Obj);
        return Obj;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  ~QWEKey
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    QWEKey::~QWEKey () {

    }  // -----  end of method QWEKey::~QWEKey  (destructor)  -----

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  Delete
    // Description:  public destructor
    //--------------------------------------------------------------------------------------
    void QWEKey::Delete() {
        this->DetachFrom(this);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  Release
    // Description:  destructor (protected)
    //--------------------------------------------------------------------------------------
    void QWEKey::Release() {
        delete this;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  Zgauss
    //--------------------------------------------------------------------------------------
    Complex QWEKey::Zgauss ( const int &ikk, const EMMODE &imode,
                            const int &itype, const Real &rho,
                            const Real &wavef, KernelEm1DBase *Kernel ) {
        return Textrap(Kernel->GetManagerIndex(), Tn(Kernel->GetManagerIndex())) ;
    }		// -----  end of method QWEKey::Zgauss  -----

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void QWEKey::ComputeRelated ( const Real& rho, KernelEm1DBase* Kernel ) {
        return ;
    }		// -----  end of method QWEKey::ComputeRelated  -----

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void QWEKey::ComputeRelated ( const Real& rho, std::vector< KernelEm1DBase* > KernelVec ) {
        return ;
    }		// -----  end of method QWEKey::ComputeRelated  -----

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  ComputeRelated
    //--------------------------------------------------------------------------------------
    void QWEKey::ComputeRelated ( const Real& rho, KernelEM1DManager* KernelManagerIn ) {
        KernelManager = KernelManagerIn;  // OK becauase this is internal and we know what we are doing

        Lambda = Bx.array()/rho;
        Intervals = xInt.array()/rho;
        int nrel = (int)(KernelManager->GetSTLVector().size());
        Zans = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(1, nrel);
        QWE(rho);
        return ;
    }		// -----  end of method QWEKey::ComputeRelated  -----

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  GaussQuadWeights
    //--------------------------------------------------------------------------------------
    void QWEKey::GaussQuadWeights(const int& N) {
        std::cerr<<"QWEKey needs work to remove Boost, etc." << std::endl;
        // Below works with older Eigen, need to find problem
//         VectorXr Nv = VectorXr::LinSpaced(N-1, 1, N-1);
//         VectorXr beta  = 0.5 / (1.-(2.*Nv.array()).pow(-2)).sqrt();
//         MatrixXr T = MatrixXr::Zero(N,N);
//         //std::cerr << "Eigen ERROR BELOW, QWEKey.cpp  QWEKey::GaussQuadWeights, COMMENTED OUT ";
//         T.bottomLeftCorner(N-1, N-1) = beta.asDiagonal();
//         Eigen::SelfAdjointEigenSolver<MatrixXr> eig( T.selfadjointView< Eigen::Lower >() ); // PROBLEM LINE
//             GaussAbscissa = eig.eigenvalues();
//             GaussWeights = 2.*eig.eigenvectors().row(0).array().pow(2);
    }

    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  BesselWeights
    //--------------------------------------------------------------------------------------
    void QWEKey::BesselWeights ( const sZeroType& sType ) {
    #ifdef HAVEBOOSTSPECIALFUNCTIONS
        GaussQuadWeights(nQuad); // TODO should this be moved out of initializer?
        std::vector<Real> bz;
        xInt = VectorXr(nIntervalsMax+1);
        xInt(0) = 1e-20;
        switch (sType) {
            case J0:
                boost::math::cyl_bessel_j_zero(0.0, 1, nIntervalsMax, std::back_inserter(bz));
                xInt.tail(nIntervalsMax) = VectorXr::Map(&bz[0], nIntervalsMax);
                break;
            case J1:
                boost::math::cyl_bessel_j_zero(1.0, 1, nIntervalsMax, std::back_inserter(bz));
                xInt.tail(nIntervalsMax) = VectorXr::Map(&bz[0], nIntervalsMax);
                break;
            case NPI:
                xInt << 1e-20, VectorXr::LinSpaced(nIntervalsMax, 1, nIntervalsMax).array() * PI;
                break;
        }
        VectorXr dx = ( xInt.tail(nIntervalsMax) - xInt.head(nIntervalsMax) ).array() / 2.;
        //  x = GaussAbscissa
        //        dx in every row        GaussWeights+1 rows, cols = n
        //     dx[0] dx[1] ... dx[N]       Gw[0]  Gw[0]  ... ndX
        //     dx[0] dx[1] ... dx[N]       Gw[1]
        MatrixXr Bxm = (dx.transpose().replicate(GaussAbscissa.size(), 1)).eval().array() *
                 ((GaussAbscissa.replicate(1, dx.size()).array() + 1.));
        Bxm.array() += xInt.head(Bxm.cols()).transpose().replicate( Bxm.rows(), 1 ).array();
        Bx =  VectorXr::Map( &Bxm(0,0), Bxm.size() );
        BJ0 = VectorXr(Bx.size());
        BJ1 = VectorXr(Bx.size());
        int iw = 0;
        for (int ii=0; ii<Bx.size(); ++ii) {
            BJ0(ii) = boost::math::cyl_bessel_j(0, Bx(ii)) * GaussWeights(iw);
            BJ1(ii) = boost::math::cyl_bessel_j(1, Bx(ii)) * GaussWeights(iw);
            ++iw;
            if (iw == GaussWeights.size()) iw = 0;
        }
        return ;
    # else
        std::cerr  << "QWEKey requires Boost functionalility that is missing\n";
    #endif
    }		// -----  end of method QWEKey::BesselWeights  -----


    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  QWE
    //--------------------------------------------------------------------------------------
    void QWEKey::QWE ( const Real& rho ) {

        // TODO, is -1 needed here?
        int nTerms = nIntervalsMax - nDelay;// - 1;
        int nrel = (int)(KernelManager->GetSTLVector().size());

        // TODO GREMLINS LIVE IN HERE
        MatrixXcr prev = Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(1, nrel);
        for (int i=0; i<nDelay; ++i) {
            getEyKernel(i, 0, rho);
            prev += Zans;
        }

        // Some of these are complex
        TS      = MatrixXcr::Zero(nrel, nTerms);
        Tn      = VectorXi::Zero(nrel);
        Textrap = MatrixXcr::Zero(nrel, nTerms);
        TrelErr = MatrixXr::Zero(nrel, nTerms);
        TabsErr = MatrixXr::Zero(nrel, nTerms);
        VectorXi Converged = VectorXi::Zero(nrel);

        // is nTerms right, 1 array shifting
        for (int i=nDelay; i<nTerms; ++i) {

            int n = i-nDelay;
            getEyKernel(i, 0, rho);

            for (int j=0; j<nrel; ++j) {

                if (!Converged(j)) { //continue;

                    Tn(j) = n;                            // order of the expansion
                    TS(j,n+1) = TS(j, n) + Zans(0, j);    // working array for transformation

                    /* Compute the Shanks transform using the Epsilon algorithm:
                        Structured after Weniger (1989, p26) */
                    /* TI - some kind ob bug here, shanks extrapolation doesn't buy much for TEM at least */
                    Complex aux2(0);
                    for (int k=n+1; k>0; --k) {
                        Complex aux1 = aux2;
                        aux2 = TS(j,k-1);
                        Complex ddff = TS(j,k) - aux2;
                        if (std::abs(ddff) < std::numeric_limits<Real>::min() ) {
                            TS(j,k-1) = std::numeric_limits<Real>::max() ;
                        } else {
                            TS(j,k-1) = aux1 + 1./ddff;
                        }

                    }

                    // The extrapolated result plus the prev integration term:
                    Textrap(j,n) = TS(j, (n-1)%2)+prev(0, j);
                    //Textrap(j,n) = TS(j, n%2 + 1)+prev(0, j);

                    // Step 3: Analyze for convergence:
                    if (n > 1) {
                        TabsErr(j,n) = std::abs( Textrap(j, n) - Textrap(j, n-1));
                        TrelErr(j,n) = TabsErr(j, n) / std::abs(Textrap(j, n)) ;
                        Converged(j) = TrelErr(j,n) < RelTol + AbsTol/std::abs(Textrap(j,n));
                    }
                }
            }
            if ( Converged.all() == 1 ) break;
        }

        // Trim up results
        // Clean up the T structure arrays? We can't really do this
        // because they are fixed size, maybe see how they are used and
        // init to zero. If they are only summed we are OK.
        /*
        for (int j = 0; j<nrel; ++j) {:nKernels
            n = Tn(j);
            T(j).extrap = T(j).extrap(1:n);
            T(j).relErr = T(j).relErr(1:n);
            T(j).absErr = T(j).absErr(1:n);
        }
        */
        return ;
    }		// -----  end of method QWEKey::QWE  -----


    //--------------------------------------------------------------------------------------
    //       Class:  QWEKey
    //      Method:  getEyKernel
    //--------------------------------------------------------------------------------------
    void QWEKey::getEyKernel ( const int& i, const int& idx, const Real& rho ) {
        int bidx = i*nQuad;
        int nrel = (int)(KernelManager->GetSTLVector().size());
        Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic > Zwork =
            Eigen::Matrix<Complex, Eigen::Dynamic, Eigen::Dynamic>::Zero(nQuad, nrel);
        for (int ik=0; ik<nQuad; ++ik) {
            KernelManager->ComputeReflectionCoeffs( Lambda(bidx+ik), idx, rho );
            for (int ir2=0; ir2<nrel; ++ir2) {
                // Zwork* needed due to sign convention (e^-jwt) of FT in filter weights
 			    Zwork(ik, ir2) =    std::conj(KernelManager->GetSTLVector()[ir2]->RelBesselArg(Lambda(bidx+ik)));
 			}
        }

        Real bma  = (Intervals(i+1)-Intervals(i))/2;
        for (int ir2=0; ir2<nrel; ++ir2) {
            if (KernelManager->GetSTLVector()[ir2]->GetBesselOrder() == 0) {
                Zans(0, ir2) = bma * Zwork.col(ir2).dot( BJ0.segment(bidx, nQuad) ); // / rho;
            } else {
                Zans(0, ir2) = bma * Zwork.col(ir2).dot( BJ1.segment(bidx, nQuad) ); // / rho;
            }
        }
        // fcount += nQuad
        return ;
    }		// -----  end of method QWEKey::getEyKernel  -----

    void QWEKey::TestPrivate(const int& N) {

        //GaussQuadWeights(N);
        //std::cout << "abscissa\n" << GaussAbscissa << std::endl;
        //std::cout << "weights\n" << GaussWeights << std::endl;
        BesselWeights( J1 );
        //BesselZeros(0, N);
        std::cout << std::scientific;
        std::cout << "BJ0" << BJ0 << std::endl;
        std::cout << "BJ1" << BJ1 << std::endl;
        //std::cout << "Bess Zero\n" << xInt << std::endl;

    }

}		// -----  end of Lemma  name  -----

