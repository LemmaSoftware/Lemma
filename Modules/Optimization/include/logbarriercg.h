/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/11/2010
  @version  $Id: logbarriercg.h 265 2015-03-27 16:05:21Z tirons $
 **/

#ifndef  LOGBARRIERCG_INC
#define  LOGBARRIERCG_INC

#include <Eigen/IterativeLinearSolvers>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <Eigen/Eigen>
#include "cg.h"
#include "bicgstab.h"
#include "lemma.h"


namespace Lemma {

    template < typename  cgScalar >
    cgScalar PhiB (const cgScalar& mux, const cgScalar& muy, const cgScalar& minVal,
                 const cgScalar& maxVal, const VectorXr x) {
        cgScalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
                // phib += std::abs((maxVal - x.array()).log().sum()*muy);
        return phib;
    }

    // PhiB for block log barrier

    template < typename  cgScalar >
    cgScalar PhiB2 (const cgScalar& mux, const cgScalar& muy, const cgScalar& minVal,
                const cgScalar& maxVal, const VectorXr x, const int& block,
                const int &nblocks) {
        cgScalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
        //phib      += std::abs((maxVal - x.array()).log().sum()*muy);
        for (int ib=0; ib<nblocks; ++ib) {
            //HiSegments(ib) = x.segment(ib*block, block).sum();
            phib += cgScalar(block)*std::log(maxVal - x.segment(ib*block, block).sum())*muy;
        }
        return phib;
    }

    template < typename  cgScalar >
    cgScalar PhiB2 (const cgScalar& minVal, const cgScalar& maxVal, const VectorXr x,
                  const int& block, const int &nblocks) {
        cgScalar phib  = std::abs((x.array() - minVal).log().sum());
        //phib      += std::abs((maxVal - x.array()).log().sum()*muy);
        for (int ib=0; ib<nblocks; ++ib) {
            //HiSegments(ib) = x.segment(ib*block, block).sum();
            phib += cgScalar(block)*std::log(maxVal - x.segment(ib*block, block).sum());
        }
        return phib;
    }

    template < typename  cgScalar >
    cgScalar PhiB2_NN (const cgScalar& mux, const cgScalar& minVal, const VectorXr x) {
        cgScalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
        return phib;
    }


    /** Impliments a logarithmic barrier CG solution of a Real linear system of
     *  the form \f[ \mathbf{A} \mathbf{x} = \mathbf{b} \f] s.t. \f$ x \in
     *  (minVal, maxVal) \f$. Note that this method optimized the complete
     *  solution, using the large matrix ATA. If you have a system with a huge
     *  number of columns, see the implicit version of this routine. Solution of
     *  the dual problem (interior-point) follows "Tikhonov Regularization with
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  @param[in] A is a Real Matrix.
     *  @param[in] b is a Real vector
     *  @param[in] x0 is current value.
     *  @param[in] xr is reference model
     *  @param[in] Wd is a Sparse Real matrix, that specifies data objective
     *  function.
     *  @param[in] Wm is a Sparse Real matrix, that sepcifies the model
     *  objective function.
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     */
    template < typename  cgScalar >
    VectorXr LogBarrierCG_4(const MatrixXr &A,
                           const VectorXr  &b,
                           const VectorXr  &xr,
                           const VectorXr  &x0,
                           const Eigen::SparseMatrix<cgScalar>& WdTWd,
                           const Eigen::SparseMatrix<cgScalar>& WmTWm,
                           const cgScalar &minVal,
                           const cgScalar &maxVal,
                           const Real& lambda) {

        // TODO pass correct phid and phim into this so that we are working with that.

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            exit(1);
        }

//         std::cout << "A\n" << A << std::endl;
//         std::cout << "WdTWd\n" << WdTWd << std::endl;
//         std::cout << "WmTWm\n" << WmTWm << std::endl;

        MatrixXr ATWdTWdA  = A.transpose()*WdTWd*A;
                 ATWdTWdA += lambda*WmTWm;

        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        cgScalar SIGMA  = .25;         //1e-2;            // 1e-1;

        cgScalar epsilon = 1e-55;       // Ratio between phib and phim+phid
        Real delta = minVal + 1e-8;
        // initial guess, start with reference model
        VectorXr x = VectorXr::Zero(N).array() + epsilon;
        VectorXr b_pre = VectorXr::Zero(M);

        cgScalar phid = (b_pre - b).norm();
        cgScalar phim = x.norm();
        cgScalar mux  = (phid + lambda*phim) * 1e4;
        cgScalar muy  = 1e-30;
        cgScalar phib = PhiB(mux, muy, minVal, maxVal, x);


        int iloop(0);
        do {

            ////////////////////////////////////////////////////////////
            // Log barrier terms
            VectorXr X1 = VectorXr::Ones(N).array() / ((x0+x).array()-minVal) ;
            VectorXr Y1 = VectorXr::Ones(N).array() / (maxVal-(x0+x).array()) ;

            VectorXr X2 = VectorXr::Ones(N).array() / (((x0+x).array()-minVal)*((x0+x).array()-minVal));
            VectorXr Y2 = VectorXr::Ones(N).array() / ((maxVal-(x0+x).array())*(maxVal-(x0+x).array()));

            /////////////////////////////////////////////////////////////
            // Solve system

            /////////////////////////////////////////////////////////////
            // CG solution of complete system
            VectorXr b2 = (A.transpose()*WdTWd*(b)).array()   // b_pre-b_obs
                           //- (lambda*WmTWm*(x-xr)).array()
                           + 2.*mux*X1.array() + 2.*muy*Y1.array();

            // Eigen requires these temporaries :(
            MatrixXr A2 = ATWdTWdA;
                     A2 += lambda*WmTWm;
                     A2.diagonal().array() += mux*X2.array() + muy*Y2.array();

            //std::cout << "all set up" << std::endl;

            Eigen::ConjugateGradient<MatrixXr> CG;
            //CG.setTolerance(1e-30);
            CG.compute(A2);

            VectorXr ztilde = CG.solve(b2);  // full soln

            // update x
            VectorXr  h = ztilde - x;
            cgScalar d(.25);
            for (int ix=0; ix<x.size(); ++ix) {
                d = std::min(d, (cgScalar).925*((x0(ix)+x[ix]-minVal)/std::abs(h[ix])));
                d = std::min(d, (cgScalar).925*(std::abs(x0(ix)+x[ix]-maxVal)/std::abs(h[ix])));
            }
            // fix overstepping, use x0+x
            x += d*h;
                std::cout << "\titerations " << CG.iterations() << std::endl;
                std::cout << "\ttolerance " << CG.tolerance() << std::endl;
                std::cout << "\td " << d << std::endl;

            for (int i=0; i<x.size(); ++i) {
                if (x(i)+x0(i) < minVal) {
                    std::cerr << "overstepp\t" << x(i) << "\t" << x0(i) << "\t" << x(i)+x0(i) << "\t" << minVal << std::endl;
                    //x(i) = minVal - x0(i) + delta;
                    x(i) = minVal + delta;
                    exit(1);
                } else if (x(i)+x0(i) > maxVal) {
                    std::cout << "overstepp BIG\t" << x(i) << "\t" << x0(i) << "\t" << maxVal<< std::endl;
                    x(i) = maxVal - x0(i) - delta;
                    exit(1);
                }
            }
            //std::cout << "|h|=" << h.norm() << " |x|=" << x.norm() << std::endl;

            // Determine mu steps to take
            VectorXr s1 = mux * (X2.asDiagonal()*h - 2.*X1);
            VectorXr s2 = muy * (Y2.asDiagonal()*h - 2.*Y1);
            //std::cout << "s1 = " << s1.transpose() << "\ns2 = " << s2.transpose() << std::endl;
            //std::cout << "ztilde = " << ztilde.transpose() << std::endl;
            //std::cout << "x = " << x.transpose() << std::endl;
            //std::cout << "h = " << h.transpose() << std::endl;
            // determine mu for next step
            mux = SIGMA/((cgScalar)(N)) * std::abs( s1.dot(x) ) ;
            muy = SIGMA/((cgScalar)(N)) * std::abs( s2.dot(x) ) ;

            b_pre = A*x;
            phid = (b_pre - b).norm(); //std::sqrt( (WdTWd*(b_pre-b)).norm() ) ; //(b - (b_pre)).norm();
            //Real phid_n = (b_pre-b).norm() ; //(b - (b_pre)).norm();
            phim = (WmTWm*(x-xr)).norm();
            phib = PhiB(mux, muy, minVal, maxVal, x0+x);
            ++iloop;

            std::cout.precision(14);
            std::cout << iloop << "\t" << phid << "\t" << phim << "\t" << mux << "\t" << muy << "\t" << phib<< std::endl;

        //} while (std::abs(phib) > epsilon && phid > 1e2);
        } while (std::abs(phib / (phid+lambda*phim)) > epsilon);

        std::cout << "final x\t" << x.array().exp().transpose() << std::endl;
        return x;

    }


    /** Impliments a logarithmic barrier CG solution of a Real linear system of
     *  the form \f[ \mathbf{A} \mathbf{x} = \mathbf{b} \f] s.t. \f$ x \in
     *  (minVal, maxVal) \f$. Note that this method optimized the complete
     *  solution, using the large matrix ATA. If you have a system with a huge
     *  number of columns, see the implicit version of this routine. Solution of
     *  the dual problem (interior-point) follows "Tikhonov Regularization with
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  @param[in] A is a Real Matrix.
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     */
    template < typename cgScalar >
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &b, const cgScalar &minVal,
        const cgScalar &maxVal) {

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            exit(1);
        }

        // TODO make ATA implicit
        MatrixXr ATA = A.transpose()*A;
        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        int MAXITER = 100;             // M*N;
        cgScalar SIGMA  = 1e-2;            //1e-1;
        cgScalar delta  = 1e-4;

        // Cholesky preconditioner
        //Eigen::FullPivLU <MatrixXr> Pre;
        //Eigen::ColPivHouseholderQR <MatrixXr> Pre;
        //Pre.compute(ATA);

        // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
        cgScalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATA*Spike).array().abs().maxCoeff());
        }
        cgScalar lambda  = 1e3*limsup;//e-1;//limsup;
        cgScalar mux0    = 1e-1*lambda;
        cgScalar muy0    = 1e-1*lambda;
        cgScalar epsilon = 1e-20;          // Ratio between phib and phim+phid

        /////////////////////////////
        // logX spacing
        //cgScalar MinLam = 1e-24;
        //cgScalar MaxLam = 1e-4;
        //VectorXr Lambdas;
        //cgScalar LS = 5000;
        //cgScalar dl10 = std::log(LS*MaxLam+1.)/(cgScalar)MAXITER;
        //Lambdas = 1./LS*(dl10*VectorXr::LinSpaced(MAXITER+1, 0, MAXITER)).array().exp()
        // + MinLam - 1./LS;

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        cgScalar phid = (b - (b_pre)).norm();
        cgScalar phim = x.norm();
        cgScalar phib = PhiB(mux0, muy0, minVal, maxVal, x);
        cgScalar mux = mux0;
        cgScalar muy = muy0;
        cgScalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);
        std::cout << "Starting CG iterations 7" << std::endl;

        /// @todo add stopping criteria.
        //int iLambda = MAXITER - 1;
        for (int ii=0; ii<MAXITER; ++ii) {

            //lambda = Lambdas[iLambda];
            //iLambda -= 1;
            // Phi_m  is just L2Norm right now. Maybe add s, alpha_T2, and
            // alpha_z terms
            VectorXr WmT_Wm = VectorXr::Ones(N).array()*lambda;

            VectorXr Xm1 = x;
            int iter = N*M;
            mux = mux0;
            muy = muy0;
            int iloop(0);

            do {

                VectorXr X1(x.size());
                VectorXr Y1(x.size());
                VectorXr X2(x.size());
                VectorXr Y2(x.size());
                VectorXr b2;
                //VectorXr HiSegments = VectorXr::Zero(nblocks);
                MatrixXr A2;

                ///////////////////////////////////
                // setup
                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    ///////////////////////////////////////////////////////////
                    // Log barrier terms
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        X1 = VectorXr::Ones(N).array() / (x.array()-minVal) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        Y1 = VectorXr::Ones(N).array() / (maxVal-x.array()) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        Y2 = VectorXr::Ones(N).array() / ((maxVal-x.array())*(maxVal-x.array()));
                    }
                #ifdef LEMMAUSEOMP
                } // parallel sections
                #endif

                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        b2 = -(A.transpose()*(b_pre-b)).array() -
                              (WmT_Wm.array()*x.array()).array() +
                              (2.*mux)*X1.array() + (2.*muy)*Y1.array();
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        A2 = ATA;
                        A2.diagonal().array() += WmT_Wm.array() + mux*X2.array() +
                                                          muy*Y2.array();
                    }
                #ifdef LEMMAUSEOMP
                } // parallel sections
                #endif

//                 // Jacobi Preconditioner
//                 Eigen::SparseMatrix<cgScalar> MM =
//                     Eigen::SparseMatrix<cgScalar>(A2.rows(), A2.cols());
//                 for (int i=0; i<ATA.rows(); ++i) {
//                     MM.insert(i,i) = 1./ATA(i,i);
//                 }
//                 MM.finalize();

                /////////////////////////////////////////////////////////////
                // Solve system,
                // CG solution of complete system
                // TODO add reference model
                tol = 1e-5*phid+mux+muy;// 1e-14;
                iter = N*M;
                //std::cout << "Call cg" << std::endl;

                // Decomposition preconditioner
                //Pre.setThreshold(1e1*tol);
                //Pre.compute(A2);

                // Jacobi Preconditioner
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, MM, iter, tol);

                // Decomposition preconditioner
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, Pre, iter, tol);

                // No preconditioner
                VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, iter, tol);
                //std::cout << "out cg" << std::endl;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                //VectorXr  h = ztilde; // - x;
                VectorXr s1, s2;

                // update x
                //VectorXr  h = ztilde; // - x;
                //cgScalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
                //x += d*h;
                cgScalar d = std::min(1.,0.995*(x.array()/ztilde.array().abs()).minCoeff());
                x += d*ztilde;

//                 // Fix any overstepping
//                 for (int ib=0; ib<nblocks; ++ib) {
//                     while (x.segment(ib*block, block).sum() > maxVal) {
//                         x.segment(ib*block, block).array() *= (.9);
//                     }
//                 }
//                 for (int i=0; i<x.size(); ++i) {
//                     if (x(i) < minVal) {
//                         x(i) = minVal + delta;
//                     }
//                 }
                b_pre = A*x;
                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phib = PhiB(mux, muy, minVal, maxVal, x);
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phid = (b - (b_pre)).norm();
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phim = x.norm();
                    }
                #ifdef LEMMAUSEOMP
                }
                #endif

                // Determine mu steps to take
                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s1 = mux * (X2.asDiagonal()*(x+ztilde) - 2.*X1);
                        mux = SIGMA/((cgScalar)(N)) * std::abs( s1.dot(x) ) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s2 = muy * (Y2.asDiagonal()*(x+ztilde) - 2.*Y1);
                        muy = SIGMA/((cgScalar)(N)) * std::abs( s2.dot(x) ) ;
                    }
                #ifdef LEMMAUSEOMP
                }
                #endif
                ++iloop;
            } while (std::abs(phib / (phid+lambda*phim)) > epsilon);

            // report
            phireport.precision(12);
            std::cout << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                      << "\t" << lambda << "\t" << mux << "\t" << muy << "\t"
                      << iter << "\t" << iloop <<  std::endl;
            phireport << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                      << "\t" << lambda << "\t" << mux << "\t" << muy << "\t"
                      << iter << "\t" << iloop <<  std::endl;

            // write model file
            std::fstream modfile;
            std::string fname = "iteration" + to_string(ii) + ".dat";
            modfile.open( fname.c_str(), std::ios::out);
            modfile << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                  << "\t" << lambda << "\t" << mux << "\t" << muy << "\t" << iter <<  "\n";
            modfile << x << "\n";
            modfile.close();

            // write predicted data file
            std::fstream predata;
            fname = "iteration" + to_string(ii) + "pre.dat";
            predata.open(fname.c_str(), std::ios::out);
            predata << b_pre << std::endl;
            predata.close();

            // update lambda
            // @todo smarter lambda change
            lambda *= .92;

        }
        phireport.close();
        // TODO, determine optimal solution
        return x;
    }

    /** Impliments a logarithmic barrier CG solution of a Real linear system of
     *  the form \f[ \mathbf{A} \mathbf{x} = \mathbf{b} \f] s.t. \f$ x \in
     *  (minVal, maxVal) \f$. Note that this method optimized the complete
     *  solution, using the large matrix ATA. If you have a system with a huge
     *  number of columns, see the implicit version of this routine. Solution of
     *  the dual problem (interior-point) follows "Tikhonov Regularization with
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  @param[in] A is a Real Matrix.
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] block is the number of parameters to sum together for the
     *  upper barrier term. So block block number parameters are kept under maxVal.
     *  as such A.rows() / block must be evenly divisible.
     */
    template <typename cgScalar>
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &b, const cgScalar &minVal,
        const cgScalar &maxVal, const int& block) {

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            exit(1);
        }

        // write predicted data file
        std::fstream obsdata;
        std::string fname = "obsdata.dat";
        obsdata.open(fname.c_str(), std::ios::out);
        obsdata << b << std::endl;
        obsdata.close();

//         #ifdef LEMMAUSEVTK
//         double blue[3] = {0.0,0.0,1.0};
//         double red[3] = {1.0,0.0,0.0};
//         VectorXr ind = VectorXr::LinSpaced(b.size(), 1, b.size());
//         #endif

        // TODO make ATA implicit
        MatrixXr ATA = A.transpose()*A;
        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        int MAXITER = 100;             // M*N;
        cgScalar SIGMA  = 1e-2;            //1e-1;
        cgScalar delta  = 1e-4;

        // Cholesky preconditioner
        //Eigen::FullPivLU <MatrixXr> Pre;
        //Eigen::ColPivHouseholderQR <MatrixXr> Pre;
        //Pre.compute(ATA);

        // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
        cgScalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATA*Spike).array().abs().maxCoeff());
        }
        cgScalar lambda  = 1e-6;//*limsup;//e-1;//limsup;
        cgScalar mux0    = 1e-1*lambda;
        cgScalar muy0    = 1e-1*lambda;
        cgScalar epsilon = 1e-25;          // Ratio between phib and phim+phid

        /////////////////////////////
        // logX spacing
        //cgScalar MinLam = 1e-24;
        //cgScalar MaxLam = 1e-4;
        //VectorXr Lambdas;
        //cgScalar LS = 5000;
        //cgScalar dl10 = std::log(LS*MaxLam+1.)/(cgScalar)MAXITER;
        //Lambdas = 1./LS*(dl10*VectorXr::LinSpaced(MAXITER+1, 0, MAXITER)).array().exp()
        // + MinLam - 1./LS;

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        int nblocks = x.size()/block;
        cgScalar phid = (b - (b_pre)).norm();
        cgScalar phim = x.norm();
        cgScalar phib = PhiB2(mux0, muy0, minVal, maxVal, x, block, nblocks);
        cgScalar mux = mux0;
        cgScalar muy = muy0;
        cgScalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);
        std::cout << "Starting CG iterations 6 " << std::endl;

        /// @todo add stopping criteria.
        //int iLambda = MAXITER - 1;
        for (int ii=0; ii<MAXITER; ++ii) {

            //lambda = Lambdas[iLambda];
            //iLambda -= 1;
            // Phi_m  is just L2Norm right now. Maybe add s, alpha_T2, and
            // alpha_z terms
            VectorXr WmT_Wm = VectorXr::Ones(N).array()*lambda;

            VectorXr Xm1 = x;
            int iter = N*M;
            mux = mux0;
            muy = muy0;
            int iloop(0);

//             #ifdef LEMMAUSEVTK
//             matplot::Plot2D_VTK p_2d("x(t)","y(t)",800,600);
//             p_2d.plot(ind, b, blue, "-");
//             p_2d.plot(ind, b_pre, red, "-");
//             p_2d.show();
//             #endif

            do {

                VectorXr X1(x.size());
                VectorXr Y1(x.size());
                VectorXr X2(x.size());
                VectorXr Y2(x.size());
                VectorXr b2;
                //VectorXr HiSegments = VectorXr::Zero(nblocks);
                MatrixXr A2;

                ///////////////////////////////////
                // setup
                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    ///////////////////////////////////////////////////////////
                    // Log barrier terms
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        X1 = VectorXr::Ones(N).array() / (x.array()-minVal) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        for (int ib=0; ib<nblocks; ++ib) {
                            //HiSegments(ib) = x.segment(ib*block, block).sum();
                            Y1.segment(ib*block, block) = VectorXr::Ones(block).array() /
                            (maxVal - x.segment(ib*block, block).sum());
                        }
                        //for (int ix=0; ix<x.size(); ++ix) {
                        //    Y1(ix) = 1./( (maxVal-HiSegments(ib)) );
                        //}
                        //Y1 = VectorXr::Ones(N).array() / (maxVal-x.array()) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        for (int ib=0; ib<nblocks; ++ib) {
                            //HiSegments(ib) = x.segment( ib*block, block ).sum();
                            Y2.segment(ib*block, block) = VectorXr::Ones(block).array() /
                            ( (maxVal-x.segment(ib*block, block).sum()) *
                              (maxVal-x.segment(ib*block, block).sum()) );
                        }
                        //Y2 = VectorXr::Ones(N).array() / ((maxVal-x.array())*(maxVal-x.array()));
                    //std::cout << Y1.transpose() << std::endl << std::endl;
                    //std::cout << Y2.transpose() << std::endl;
                    }
                #ifdef LEMMAUSEOMP
                } // parallel sections
                #endif

                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        b2 = -(A.transpose()*(b_pre-b)).array() -
                              (WmT_Wm.array()*x.array()).array() +
                              (2.*mux)*X1.array() + (2.*muy)*Y1.array();
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        A2 = ATA;
                        A2.diagonal().array() += WmT_Wm.array() + mux*X2.array() +
                                                          muy*Y2.array();
                    }
                #ifdef LEMMAUSEOMP
                } // parallel sections
                #endif

//                 // Jacobi Preconditioner
//                 Eigen::SparseMatrix<cgScalar> MM =
//                     Eigen::SparseMatrix<cgScalar>(A2.rows(), A2.cols());
//                 for (int i=0; i<ATA.rows(); ++i) {
//                     MM.insert(i,i) = 1./ATA(i,i);
//                 }
//                 MM.finalize();

                /////////////////////////////////////////////////////////////
                // Solve system,
                // CG solution of complete system
                // TODO add reference model
                tol = 1e-5*phid+mux+muy;// 1e-14;
                iter = N*M;
                //std::cout << "Call cg" << std::endl;

                // Decomposition preconditioner
                //Pre.setThreshold(1e1*tol);
                //Pre.compute(A2);

                // Jacobi Preconditioner
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, MM, iter, tol);

                // Decomposition preconditioner
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, Pre, iter, tol);

                // No preconditioner
                VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, iter, tol);
                //std::cout << "out cg" << std::endl;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                //VectorXr  h = ztilde; // - x;
                VectorXr s1, s2;

                // update x
                //VectorXr  h = ztilde; // - x;
                //cgScalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
                //x += d*h;
                cgScalar d = std::min(1.,0.995*(x.array()/ztilde.array().abs()).minCoeff());
                x += d*ztilde;

//                 // Fix any overstepping
//                 for (int ib=0; ib<nblocks; ++ib) {
//                     while (x.segment(ib*block, block).sum() > maxVal) {
//                         x.segment(ib*block, block).array() *= (.9);
//                     }
//                 }
//                 for (int i=0; i<x.size(); ++i) {
//                     if (x(i) < minVal) {
//                         x(i) = minVal + delta;
//                     }
//                 }
                b_pre = A*x;

                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phib = PhiB2(mux, muy, minVal, maxVal, x, block, nblocks);
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phid = (b - (b_pre)).norm();
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        phim = x.norm();
                    }
                #ifdef LEMMAUSEOMP
                }
                #endif

                // Determine mu steps to take
                #ifdef LEMMAUSEOMP
                #pragma omp parallel sections
                {
                #endif
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s1 = mux * (X2.asDiagonal()*(x+ztilde) - 2.*X1);
                        mux = SIGMA/((cgScalar)(N)) * std::abs( s1.dot(x) ) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s2 = muy * (Y2.asDiagonal()*(x+ztilde) - 2.*Y1);
                        muy = SIGMA/((cgScalar)(N)) * std::abs( s2.dot(x) ) ;
                    }
                #ifdef LEMMAUSEOMP
                }
                #endif
                ++iloop;
            } while (std::abs(phib / (phid+lambda*phim)) > epsilon);

            // report
            phireport.precision(12);
            std::cout << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                      << "\t" << lambda << "\t" << mux << "\t" << muy << "\t"
                      << iter << "\t" << iloop <<  std::endl;
            phireport << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                      << "\t" << lambda << "\t" << mux << "\t" << muy << "\t"
                      << iter << "\t" << iloop <<  std::endl;

            std::fstream modfile;
            std::string fname = "iteration" + to_string(ii) + ".dat";
            modfile.open( fname.c_str(), std::ios::out);
            modfile << ii << "\t" << x.norm() << "\t" <<  (b-(A*x)).norm()
                  << "\t" << lambda << "\t" << mux << "\t" << muy << "\t" << iter <<  "\n";
            modfile << x << "\n";
            modfile.close();

            // write predicted data file
            std::fstream predata;
            fname = "iteration" + to_string(ii) + "pre.dat";
            predata.open(fname.c_str(), std::ios::out);
            predata << b_pre << std::endl;
            predata.close();

            // update lambda
            // @todo smarter lambda change
            lambda *= .85;

        }
        phireport.close();
        // TODO, determine optimal solution
        return x;
    }

    /** Impliments a logarithmic barrier CG solution of a Real linear system of
     *  the form \f[ \mathbf{A} \mathbf{x} = \mathbf{b} \f] s.t. \f$ x \in
     *  (minVal, maxVal) \f$. Note that this method optimized the complete
     *  solution, using the large matrix ATA. If you have a system with a huge
     *  number of columns, see the implicit version of this routine. Solution of
     *  the dual problem (interior-point) follows "Tikhonov Regularization with
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  @param[in] A is a Real Matrix.
     *  @param[in] xref is a reference model
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] block is the number of parameters to sum together for the
     *  upper barrier term. So block block number parameters are kept under maxVal.
     *  as such A.rows() / block must be evenly divisible.
     *  @param[in] WdTWd is the data objective function
     *  @param[in] WmTWm is the model objective function
     */
    template <typename cgScalar>
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &xr,
        const VectorXr &b,
        const cgScalar &minVal,
        const cgScalar &maxVal, const int& block,
        const Eigen::SparseMatrix<cgScalar>& WdTWd,
        const Eigen::SparseMatrix<cgScalar>& WmTWm, Real lambda0=1e1) {

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            std::cerr << "A.rows() " << A.rows() << "\n";
            std::cerr << "A.cols() " << A.cols() << "\n";
            std::cerr << "b.size() " << b.size() << "\n";
            exit(1);
        }

        // write predicted data file
        std::fstream obsdata;
        std::string fname = "obsdata.dat";
        obsdata.open(fname.c_str(), std::ios::out);
        obsdata << b << std::endl;
        obsdata.close();

        // TODO make ATA implicit
        MatrixXr ATWdTWdA = A.transpose()*WdTWd*A;
        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        int MAXITER = 175;             // M*N;
        cgScalar SIGMA  = .25;//5.85; //1e-2; // .25; //1e-2;   // 1e-1;
        cgScalar delta  = 1e-4;

//         // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
//         cgScalar limsup = 1e10;
//         for (int i=0; i<N; ++i) {
//             VectorXr Spike = VectorXr::Zero(N);
//             Spike(i) = (minVal + maxVal) / 2.;
//             limsup = std::min(limsup, (ATWdTWdA*Spike).array().abs().maxCoeff());
//         }

        cgScalar lambda  = lambda0;        //*limsup;//e-1;//limsup;
        cgScalar epsilon = 1e-15;          // Ratio between phib and phim+phid

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        int nblocks = x.size()/block;
        cgScalar phid = (WdTWd*(b - b_pre)).norm();
        cgScalar phim = (WmTWm*(x - xr)).norm();
        cgScalar phib = PhiB2(minVal, maxVal, x, block, nblocks);
        cgScalar mux  = (phid + lambda*phim) / phib;
        cgScalar muy  = mux;
        //cgScalar tol;//  = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);
        std::cout << "Starting CG iterations 5" << std::endl;

        Eigen::ConjugateGradient< MatrixXr, Eigen::Upper > cg;

        /// @todo add stopping criteria.
        for (int ii=0; ii<MAXITER; ++ii) {

            int iter = N*M;
            mux = (phid + lambda*phim) / phib;
            muy = mux;
            int iloop(0);
            int itertot(0);
            VectorXr h;
            bool cont(true);
            do {

                //restart:

                VectorXr X1(x.size());
                VectorXr Y1(x.size());
                VectorXr X2(x.size());
                VectorXr Y2(x.size());
                VectorXr b2;
                MatrixXr A2;

                ///////////////////////////////////
                // setup

                ///////////////////////////////////////////////////////////
                // Log barrier terms

                X1 = VectorXr::Ones(N).array() / (x.array()-minVal) ;

                for (int ib=0; ib<nblocks; ++ib) {
                    Y1.segment(ib*block, block) = VectorXr::Ones(block).array() /
                    (maxVal - x.segment(ib*block, block).sum());
                }

                X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));

                for (int ib=0; ib<nblocks; ++ib) {
                    Y2.segment(ib*block, block) = VectorXr::Ones(block).array() /
                        ( (maxVal-x.segment(ib*block, block).sum()) *
                          (maxVal-x.segment(ib*block, block).sum()) );
                }

                // Newton step
                //b2 = - (A.transpose()*WdTWd*(b_pre-b)).array()
                //     - lambda*(WmTWm*(x-xr)).array()
                //     + (2.*mux)*X1.array() + (2.*muy)*Y1.array();

                // Full
                b2 =  (A.transpose()*WdTWd*(b)).array()
                     //- lambda*(WmTWm*(x-xr)).array()
                     + (2.*mux)*X1.array() + (2.*muy)*Y1.array();

                A2 = ATWdTWdA;
                A2 += lambda*WmTWm;
                A2.diagonal().array() +=  mux*X2.array() + muy*Y2.array();

//                 // Jacobi Preconditioner
//                 Eigen::SparseMatrix<cgScalar> MM =
//                     Eigen::SparseMatrix<cgScalar>(A2.rows(), A2.cols());
//                 for (int i=0; i<ATWdTWdA.rows(); ++i) {
//                     MM.insert(i,i) = 1./ATWdTWdA(i,i);
//                 }
//                 MM.finalize();

                /////////////////////////////////////////////////////////////
                // Solve system,
                // CG solution of complete system
                // TODO add reference model
                //tol = 1e-5*phid+mux+muy;// 1e-14;
                iter = N*M;
                //std::cout << "Call cg" << std::endl;

                // Decomposition preconditioner
                //Pre.setThreshold(1e1*tol);
                //Pre.compute(A2);

                // Jacobi Preconditioner
                //VectorXr ztilde = CGJ(A2, VectorXr::Zero(N), b2, MM, iter, tol);

                // Decomposition preconditioner
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, Pre, iter, tol);

                // No preconditioner

                // Newton Setp
                //VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, iter, tol);

                // Full soln
                //VectorXr ztilde = CG(A2, x, b2, iter, tol);
                //std::cout << "out cg" << std::endl;
                cg.compute(A2);
                VectorXr ztilde;
                ztilde = cg.solveWithGuess(b2, x);
                iter = cg.iterations();
                //tol = cg.error();

                ++iloop;
                itertot += iter;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                //VectorXr  h = ztilde; // - x;

                // update x
                h = ztilde - x;

                // determing steplength
                //cgScalar d = std::min(1., 0.925*(x.array()/h.array().abs()).minCoeff() );
                cgScalar d(1.);
                for (int ix=0; ix<x.size(); ++ix) {
                    if (h[ix] < 0.) {
                        d = std::min(d, (cgScalar).925*(x[ix]/std::abs(h[ix])));
                    }
                }

//                 if (d < 1e-5) {
//                     std::cout << "not going anywhere d=" << d << " |h| = " << h.norm() << "\n";
//                     //break;
//                     mux  = (phid + lambda*phim) / phib;
//                     muy  = mux;
//                     x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);
//                     //goto restart;  // Gasp!
//                     continue;
//                 }

                // Newton
                //cgScalar d = std::min(1., 0.9*((x.array()/ztilde.array()).abs()).minCoeff());

                // Make step
                x += d*h;        // whole soln
                //x += d*ztilde; // Newton
                // Fix any overstepping
                for (int ib=0; ib<nblocks; ++ib) {
                    while (x.segment(ib*block, block).sum() >= maxVal) {
                        x.segment(ib*block, block).array() *= .99;
                    }
                }

                for (int i=0; i<x.size(); ++i) {
                    if (x(i) < minVal) {
                        x(i) = minVal + delta;
                    }
                }

                b_pre = A*x;

                phib = PhiB2(mux, muy, minVal, maxVal, x, block, nblocks);
                phid = std::sqrt((WdTWd*(b-b_pre)).norm());
                phim = (WmTWm*(x-xr)).norm();

                // Determine mu steps to take
                VectorXr s1 = mux * (X2.asDiagonal()*(ztilde) - 2.*X1);
                mux = SIGMA/((cgScalar)(N)) * std::abs( s1.dot(x) ) ;
                VectorXr s2 = muy * (Y2.asDiagonal()*(ztilde) - 2.*Y1);
                muy = SIGMA/((cgScalar)(N)) * std::abs( s2.dot(x) ) ;

                if ( (std::abs(phib / (phid+lambda*phim)) < epsilon) && phid < 1000. ) {
                //if ( (std::abs(phib / (phid+lambda*phim)) < epsilon) && h.norm() < 1e-5) {
                     cont = false;
                }

            } while ( cont );

            // report
            //std::cout << std::ios::left;
            //std::cout.precision(8);
            std::cout << std::setw(6) << ii << std::scientific << std::setw(18) << phim <<  std::setw(18) <<  phid
                      << std::setw(18) << lambda << std::setw(18) << mux << std::setw(18) << muy
                      << std::setw(12) << itertot << std::setw(12) << iloop << std::setw(18) << h.norm() <<  std::endl;

            phireport.precision(12);
            phireport << ii << "\t" << phim << "\t" <<  phid
                      << "\t" << lambda << "\t" << mux << "\t" << muy << "\t"
                      << itertot << "\t" << iloop  << "\t" << h.norm() <<  std::endl;

            std::fstream modfile;
            std::string fname = "iteration" + to_string(ii) + ".dat";
            modfile.open( fname.c_str(), std::ios::out);
            modfile << ii << "\t" << phim  << "\t" << phid
                  << "\t" << lambda << "\t" << mux << "\t" << muy << "\t" << iter <<  "\n";
            modfile << x << "\n";
            modfile.close();

            // write predicted data file
            std::fstream predata;
            fname = "iteration" + to_string(ii) + "pre.dat";
            predata.open(fname.c_str(), std::ios::out);
            predata << b_pre << std::endl;
            predata.close();

            // update lambda
            // @todo smarter lambda change
            lambda *= .9;

        }
        phireport.close();
        // TODO, determine optimal solution
        return x;
    }


     /** Impliments a logarithmic barrier CG solution of a Real linear system of
     *  the form \f[ \mathbf{A} \mathbf{x} = \mathbf{b} \f] s.t. \f$ x \in
     *  (minVal, maxVal) \f$. Note that this method optimized the complete
     *  solution, using the large matrix ATA. If you have a system with a huge
     *  number of columns, see the implicit version of this routine. Solution of
     *  the dual problem (interior-point) follows "Tikhonov Regularization with
     *  Nonnegativity constraint, (Calavetti et. al. 2004)". This routine only imposes non-negativity. No upper bound
     *  @param[in] A is a Real Matrix.
     *  @param[in] xref is a reference model
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] block is the number of parameters to sum together for the
     *  upper barrier term. So block block number parameters are kept under maxVal.
     *  as such A.rows() / block must be evenly divisible.
     *  @param[in] WdTWd is the data objective function
     *  @param[in] WmTWm is the model objective function
     */
    template <typename cgScalar>
    VectorXr LogBarrierCG_NN(const MatrixXr &A, const VectorXr &xr,
        const VectorXr &b, const cgScalar &minVal,
        const Eigen::SparseMatrix<cgScalar>& WdTWd,
        const Eigen::SparseMatrix<cgScalar>& WmTWm, Real lambda0=1e1, int MAXITER=175) {

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            std::cerr << "A.rows() " << A.rows() << "\n";
            std::cerr << "A.cols() " << A.cols() << "\n";
            std::cerr << "b.size() " << b.size() << "\n";
            exit(1);
        }

        // write predicted data file
        std::fstream obsdata;
        std::string fname = "obsdata.dat";
        obsdata.open(fname.c_str(), std::ios::out);
        obsdata << b << std::endl;
        obsdata.close();

        // TODO make ATA implicit, or at least only compute half
        MatrixXr ATWdTWdA = A.transpose()*WdTWd*A;
        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        //int MAXITER = 175;             // M*N;
        cgScalar SIGMA  = .25;//5.85; //1e-2; // .25; //1e-2;   // 1e-1;
        cgScalar delta  = 1e-4;

//         // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
//         cgScalar limsup = 1e10;
//         for (int i=0; i<N; ++i) {
//             VectorXr Spike = VectorXr::Zero(N);
//             Spike(i) = (minVal + maxVal) / 2.;
//             limsup = std::min(limsup, (ATWdTWdA*Spike).array().abs().maxCoeff());
//         }

        cgScalar lambda  = lambda0;        //*limsup;//e-1;//limsup;
        cgScalar epsilon = 1e-16;          // Ratio between phib and phim+phid

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        //Eigen::ConjugateGradient< MatrixXr > cg;
        // Use ILUT preconditioner
        Eigen::ConjugateGradient< MatrixXr, Eigen::Upper, Eigen::DiagonalPreconditioner<Real> > cg;
        //Eigen::ConjugateGradient< MatrixXr, Eigen::Upper, Eigen::IncompleteLUT<Real> > cg;

        cgScalar phid = (WdTWd*(b - b_pre)).norm();
        cgScalar phim = (WmTWm*(x - xr)).norm();
        cgScalar phib = PhiB2_NN(1., minVal, x);
        cgScalar mux  = (phid + lambda*phim) / phib;
        //cgScalar tol; //  = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);
        std::cout << "Starting CG iterations 4" << std::endl;

        /// @todo add stopping criteria.
        for (int ii=0; ii<MAXITER; ++ii) {

            int iter = N*M;
            mux = (phid + lambda*phim) / phib;
            int iloop(0);
            int itertot(0);
            VectorXr h;
            bool cont(true);
            do {

                //restart:

                VectorXr X1(x.size());
                VectorXr X2(x.size());
                VectorXr b2;
                MatrixXr A2;

                ///////////////////////////////////
                // setup

                ///////////////////////////////////////////////////////////
                // Log barrier terms

                X1 = VectorXr::Ones(N).array() /  (x.array()-minVal) ;
                X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));

                // Full
                b2 =  (A.transpose()*WdTWd*(b)).array()
                     //- lambda*(WmTWm*(x-xr)).array()
                     + (2.*mux)*X1.array(); // + (2.*muy)*Y1.array();

                A2 = ATWdTWdA;
                A2 += lambda*WmTWm;
                A2.diagonal().array() +=  mux*X2.array();

                /////////////////////////////////////////////////////////////
                // Solve system,
                // CG solution of complete system
                // TODO add reference model
                //tol = 1e-5*phid+mux;// 1e-14;
                iter = N*M;

                // Full soln
                //VectorXr ztilde = CG(A2, x, b2, iter, tol);
                cg.compute(A2);
                VectorXr ztilde;
                ztilde = cg.solveWithGuess(b2, x);
                //std::cout << "out cg" << std::endl;
                iter = cg.iterations();
                //tol = cg.error();

                ++iloop;
                itertot += iter;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                h = ztilde - x;

                // determing steplength
                cgScalar d(1.);
                for (int ix=0; ix<x.size(); ++ix) {
                    if (h[ix] < 0.) {
                        d = std::min(d, (cgScalar).925*(x[ix]/std::abs(h[ix])));
                    }
                }

//                 if (d < 1e-5) {
//                     std::cout << "not going anywhere d=" << d << " |h| = " << h.norm() << "\n";
//                     //break;
//                     mux  = (phid + lambda*phim) / phib;
//                     muy  = mux;
//                     x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);
//                     //goto restart;  // Gasp!
//                     continue;
//                 }

                // Newton
                //cgScalar d = std::min(1., 0.9*((x.array()/ztilde.array()).abs()).minCoeff());

                // Make step
                x += d*h;        // whole soln
                // Fix any overstepping
                for (int i=0; i<x.size(); ++i) {
                    if (x(i) < minVal) {
                        x(i) = minVal + delta;
                    }
                }

                b_pre = A*x;

                phib = PhiB2_NN(mux, minVal, x);
                phid = (WdTWd*(b-b_pre)).norm();
                phim = (WmTWm*(x-xr)).norm();

                // Determine mu steps to take
                VectorXr s1 = mux * (X2.asDiagonal()*(ztilde) - 2.*X1);
                mux = SIGMA/((cgScalar)(N)) * std::abs( s1.dot(x) ) ;

                if ( (std::abs(phib / (phid+lambda*phim)) < epsilon)) {
                //if ( (std::abs(phib / (phid+lambda*phim)) < epsilon) && h.norm() < 1e-5) {
                     cont = false;
                }

            } while ( cont );

            // report
            //std::cout << std::ios::left;
            //std::cout.precision(8);
            std::cout << std::setw(6) << ii << std::scientific << std::setw(18) << phim <<  std::setw(18) <<  phid
                      << std::setw(18) << lambda << std::setw(18) << mux
                      << std::setw(12) << itertot << std::setw(12) << iloop << std::setw(18) << h.norm() <<  std::endl;

            phireport.precision(12);
            phireport << ii << "\t" << phim << "\t" <<  phid
                      << "\t" << lambda << "\t" << mux << "\t"
                      << itertot << "\t" << iloop  << "\t" << h.norm() <<  std::endl;

            std::fstream modfile;
            std::string fname = "iteration" + to_string(ii) + ".dat";
            modfile.open( fname.c_str(), std::ios::out);
            modfile << ii << "\t" << phim  << "\t" << phid
                  << "\t" << lambda << "\t" << mux << "\t" << iter <<  "\n";
            modfile << x << "\n";
            modfile.close();

            // write predicted data file
            std::fstream predata;
            fname = "iteration" + to_string(ii) + "pre.dat";
            predata.open(fname.c_str(), std::ios::out);
            predata << b_pre << std::endl;
            predata.close();

            // update lambda
            // @todo smarter lambda change
            lambda *= .9;

        }
        phireport.close();
        // TODO, determine optimal solution
        return x;
    }






}

#endif   // ----- #ifndef LOGBARRIERCG_INC  -----
