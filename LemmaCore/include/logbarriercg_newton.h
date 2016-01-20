/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/11/2010
  @version  $Id: logbarriercg.h 88 2013-09-06 17:24:44Z tirons $
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

#ifdef LEMMAUSEVTK
#include "matplot.h"
#endif

namespace Lemma {

    template < typename  Scalar >
    Scalar PhiB (const Scalar& mux, const Scalar& muy, const Scalar& minVal,
                 const Scalar& maxVal, const VectorXr x) {
        Scalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
               phib += std::abs((maxVal - x.array()).log().sum()*muy);
        return phib;
    }

    // PhiB for block log barrier

    template < typename  Scalar >
    Scalar PhiB2 (const Scalar& mux, const Scalar& muy, const Scalar& minVal,
                const Scalar& maxVal, const VectorXr x, const int& block,
                const int &nblocks) {
        Scalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
        //phib      += std::abs((maxVal - x.array()).log().sum()*muy);
        for (int ib=0; ib<nblocks; ++ib) {
            //HiSegments(ib) = x.segment(ib*block, block).sum();
            phib += Scalar(block)*std::log(maxVal - x.segment(ib*block, block).sum())*muy;
        }
        return phib;
    }

    template < typename  Scalar >
    Scalar PhiB2 (const Scalar& minVal, const Scalar& maxVal, const VectorXr x,
                  const int& block, const int &nblocks) {
        Scalar phib  = std::abs((x.array() - minVal).log().sum());
        //phib      += std::abs((maxVal - x.array()).log().sum()*muy);
        for (int ib=0; ib<nblocks; ++ib) {
            //HiSegments(ib) = x.segment(ib*block, block).sum();
            phib += Scalar(block)*std::log(maxVal - x.segment(ib*block, block).sum());
        }
        return phib;
    }

    template < typename  Scalar >
    Scalar PhiB2_NN (const Scalar& mux, const Scalar& minVal, const VectorXr x) {
        Scalar phib  = std::abs((x.array() - minVal).log().sum()*mux);
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
     *  @param[in] x0 is a reference model.
     *  @param[in] Wd is a Sparse Real matrix, that specifies data objective
     *  function.
     *  @param[in] Wm is a Sparse Real matrix, that sepcifies the model
     *  objective function.
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     */
    template < typename  Scalar >
    VectorXr LogBarrierCG(const MatrixXr &A,  const VectorXr &b,
                          const VectorXr &x0,
                          const Eigen::SparseMatrix<Scalar>& WdTWd,
                          const Eigen::SparseMatrix<Scalar>& WmTWm,
                          const Scalar &minVal,
                          const Scalar &maxVal) {

        // Check that everything is aligned OK
        if (A.rows() != b.size() ) {
            std::cerr << "Matrix A is not aligned with Vector b" << "\n";
            exit(1);
        }

        /////////////////////////////////////////////
        // Build all the large static matrices
        // NOTE, ATA can be large. For some problems an implicit algorithm may
        // be better suited.
        //Eigen::SparseMatrix<Scalar> WmTWm = Wm.transpose()*Wm;
        //Eigen::SparseMatrix<Scalar> WdTWd = Wd.transpose()*Wd;
        MatrixXr ATWdTWdA = A.transpose()*WdTWd*A;

        /////////////////////////
        // Jacobi Preconditioner
        Eigen::SparseMatrix<Scalar> MM (ATWdTWdA.rows(), ATWdTWdA.cols());
        for (int i=0; i<ATWdTWdA.rows(); ++i) {
            MM.insert(i,i) = 1. / ATWdTWdA(i,i);
        }
        MM.finalize();

        int N = A.cols();              // number of model
        int M = A.rows();              // number of data
        int MAXITER = 100;             // M*N;
        Scalar SIGMA  = 1e-2;            // 1e-1;

        // Determine starting lambda_0, find lim_sup of the norm of impulse
        // responses and scale.
        /// @todo the starting lambda is not always a very good number.
        Scalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATWdTWdA*Spike).array().abs().maxCoeff());
        }
        Scalar lambda  = limsup;
        Scalar mux0    = 1e-1*lambda;
        Scalar muy0    = 1e-1*lambda;
        Scalar epsilon = 1e-20;          // Ratio between phib and phim+phid

        // initial guess, start with reference model
        VectorXr x=x0;

        // predicted b
        VectorXr b_pre = A*x;

        Scalar phid = (b - (b_pre)).norm();
        Scalar phim = x.norm();
        Scalar phib = PhiB(mux0, muy0, minVal, maxVal, x);
        Scalar mux = mux0;
        Scalar muy = muy0;
        Scalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

        /// @todo add stopping criteria.
        //int iLambda = MAXITER - 1;
        for (int ii=0; ii<MAXITER; ++ii) {

            //lambda = Lambdas[iLambda];
            //iLambda -= 1;
            // Phi_m  is just L2Norm right now. Maybe add s, alpha_T2, and
            // alpha_z terms

            VectorXr Xm1 = x;
            int iter = N*M;
            mux = mux0;
            muy = muy0;
            int iloop(0);
            do {

                ///////////////////////////////////////////////////////////
                // Log barrier terms
                VectorXr X1 = VectorXr::Ones(N).array() / (x.array()-minVal) ;
                VectorXr Y1 = VectorXr::Ones(N).array() / (maxVal-x.array()) ;

                VectorXr X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));
                VectorXr Y2 = VectorXr::Ones(N).array() / ((maxVal-x.array())*(maxVal-x.array()));

                /////////////////////////////////////////////////////////////
                // Solve system

                tol = 1e-5*phid;// 1e-14;
                iter = N*M;

                //////////////////////////
                // CG solution of complete system
                VectorXr b2 = (-A.transpose()*WdTWd*(b_pre-b)).array() -
                              (lambda*WmTWm*(x0-x0)).array() +
                              2.*mux*X1.array() + 2.*muy*Y1.array();

                // Eigen requires these temporaries :(
                MatrixXr A2 = ATWdTWdA;
                A2 += lambda*WmTWm;
                A2.diagonal() += mux*X2 + muy*Y2;

                VectorXr ztilde = CG(A2, VectorXr::Zero(N), b2, MM, iter, tol);

                // update x
                VectorXr  h = ztilde; // - x;
                Scalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
                x += d*h;

                // Determine mu steps to take
                VectorXr s1 = mux * (X2.asDiagonal()*ztilde - 2.*X1);
                VectorXr s2 = muy * (Y2.asDiagonal()*ztilde - 2.*Y1);

                // determine mu for next step
                mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;

                b_pre = A*x;
                phid = (WdTWd*(b-b_pre)).norm() ; //(b - (b_pre)).norm();
                phim = (WmTWm*(x-x0)).norm();
                phib = PhiB(mux, muy, minVal, maxVal, x);
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

            // update lambda
            /// @todo smarter lambda change
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
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  @param[in] A is a Real Matrix.
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     */
    template < typename Scalar >
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &b, const Scalar &minVal,
        const Scalar &maxVal) {

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
        Scalar SIGMA  = 1e-2;            //1e-1;
        Scalar delta  = 1e-4;

        // Cholesky preconditioner
        //Eigen::FullPivLU <MatrixXr> Pre;
        //Eigen::ColPivHouseholderQR <MatrixXr> Pre;
        //Pre.compute(ATA);

        // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
        Scalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATA*Spike).array().abs().maxCoeff());
        }
        Scalar lambda  = 1e3*limsup;//e-1;//limsup;
        Scalar mux0    = 1e-1*lambda;
        Scalar muy0    = 1e-1*lambda;
        Scalar epsilon = 1e-20;          // Ratio between phib and phim+phid

        /////////////////////////////
        // logX spacing
        //Scalar MinLam = 1e-24;
        //Scalar MaxLam = 1e-4;
        //VectorXr Lambdas;
        //Scalar LS = 5000;
        //Scalar dl10 = std::log(LS*MaxLam+1.)/(Scalar)MAXITER;
        //Lambdas = 1./LS*(dl10*VectorXr::LinSpaced(MAXITER+1, 0, MAXITER)).array().exp()
        // + MinLam - 1./LS;

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        Scalar phid = (b - (b_pre)).norm();
        Scalar phim = x.norm();
        Scalar phib = PhiB(mux0, muy0, minVal, maxVal, x);
        Scalar mux = mux0;
        Scalar muy = muy0;
        Scalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

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
//                 Eigen::SparseMatrix<Scalar> MM =
//                     Eigen::SparseMatrix<Scalar>(A2.rows(), A2.cols());
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
                //Scalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
                //x += d*h;
                Scalar d = std::min(1.,0.995*(x.array()/ztilde.array().abs()).minCoeff());
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
                        mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s2 = muy * (Y2.asDiagonal()*(x+ztilde) - 2.*Y1);
                        muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;
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
    template <typename Scalar>
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &b, const Scalar &minVal,
        const Scalar &maxVal, const int& block) {

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
        Scalar SIGMA  = 1e-2;            //1e-1;
        Scalar delta  = 1e-4;

        // Cholesky preconditioner
        //Eigen::FullPivLU <MatrixXr> Pre;
        //Eigen::ColPivHouseholderQR <MatrixXr> Pre;
        //Pre.compute(ATA);

        // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
        Scalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATA*Spike).array().abs().maxCoeff());
        }
        Scalar lambda  = 1e-6;//*limsup;//e-1;//limsup;
        Scalar mux0    = 1e-1*lambda;
        Scalar muy0    = 1e-1*lambda;
        Scalar epsilon = 1e-25;          // Ratio between phib and phim+phid

        /////////////////////////////
        // logX spacing
        //Scalar MinLam = 1e-24;
        //Scalar MaxLam = 1e-4;
        //VectorXr Lambdas;
        //Scalar LS = 5000;
        //Scalar dl10 = std::log(LS*MaxLam+1.)/(Scalar)MAXITER;
        //Lambdas = 1./LS*(dl10*VectorXr::LinSpaced(MAXITER+1, 0, MAXITER)).array().exp()
        // + MinLam - 1./LS;

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        int nblocks = x.size()/block;
        Scalar phid = (b - (b_pre)).norm();
        Scalar phim = x.norm();
        Scalar phib = PhiB2(mux0, muy0, minVal, maxVal, x, block, nblocks);
        Scalar mux = mux0;
        Scalar muy = muy0;
        Scalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

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
//                 Eigen::SparseMatrix<Scalar> MM =
//                     Eigen::SparseMatrix<Scalar>(A2.rows(), A2.cols());
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
                //Scalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
                //x += d*h;
                Scalar d = std::min(1.,0.995*(x.array()/ztilde.array().abs()).minCoeff());
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
                        mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                    }
                    #ifdef LEMMAUSEOMP
                    #pragma omp section
                    #endif
                    {
                        s2 = muy * (Y2.asDiagonal()*(x+ztilde) - 2.*Y1);
                        muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;
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
    template <typename Scalar>
    VectorXr LogBarrierCG(const MatrixXr &A, const VectorXr &xr,
        const VectorXr &b, const Scalar &minVal,
        const Scalar &maxVal, const int& block,
        const Eigen::SparseMatrix<Scalar>& WdTWd,
        const Eigen::SparseMatrix<Scalar>& WmTWm, Real lambda0=1e1) {

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
        Scalar SIGMA  = .25;//5.85; //1e-2; // .25; //1e-2;   // 1e-1;
        Scalar delta  = 1e-4;

//         // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
//         Scalar limsup = 1e10;
//         for (int i=0; i<N; ++i) {
//             VectorXr Spike = VectorXr::Zero(N);
//             Spike(i) = (minVal + maxVal) / 2.;
//             limsup = std::min(limsup, (ATWdTWdA*Spike).array().abs().maxCoeff());
//         }

        Scalar lambda  = lambda0;        //*limsup;//e-1;//limsup;
        Scalar epsilon = 1e-15;          // Ratio between phib and phim+phid

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        int nblocks = x.size()/block;
        Scalar phid = (WdTWd*(b - b_pre)).norm();
        Scalar phim = (WmTWm*(x - xr)).norm();
        Scalar phib = PhiB2(minVal, maxVal, x, block, nblocks);
        Scalar mux  = (phid + lambda*phim) / phib;
        Scalar muy  = mux;
        //Scalar tol;//  = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

        Eigen::ConjugateGradient< MatrixXr > cg;

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
//                 Eigen::SparseMatrix<Scalar> MM =
//                     Eigen::SparseMatrix<Scalar>(A2.rows(), A2.cols());
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
                VectorXr ztilde = cg.solveWithGuess(b2, x);
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
                //Scalar d = std::min(1., 0.925*(x.array()/h.array().abs()).minCoeff() );
                Scalar d(1.);
                for (int ix=0; ix<x.size(); ++ix) {
                    if (h[ix] < 0.) {
                        d = std::min(d, (Scalar).925*(x[ix]/std::abs(h[ix])));
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
                //Scalar d = std::min(1., 0.9*((x.array()/ztilde.array()).abs()).minCoeff());

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
                phid = (WdTWd*(b-b_pre)).norm();
                phim = (WmTWm*(x-xr)).norm();

                // Determine mu steps to take
                VectorXr s1 = mux * (X2.asDiagonal()*(ztilde) - 2.*X1);
                mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                VectorXr s2 = muy * (Y2.asDiagonal()*(ztilde) - 2.*Y1);
                muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;

                if ( (std::abs(phib / (phid+lambda*phim)) < epsilon)) {
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
    template <typename Scalar>
    VectorXr LogBarrierCG_NN(const MatrixXr &A, const VectorXr &xr,
        const VectorXr &b, const Scalar &minVal,
        const Eigen::SparseMatrix<Scalar>& WdTWd,
        const Eigen::SparseMatrix<Scalar>& WmTWm, Real lambda0=1e1) {

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
        int MAXITER = 175;             // M*N;
        Scalar SIGMA  = .25;//5.85; //1e-2; // .25; //1e-2;   // 1e-1;
        Scalar delta  = 1e-12;

//         // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
//         Scalar limsup = 1e10;
//         for (int i=0; i<N; ++i) {
//             VectorXr Spike = VectorXr::Zero(N);
//             Spike(i) = (minVal + maxVal) / 2.;
//             limsup = std::min(limsup, (ATWdTWdA*Spike).array().abs().maxCoeff());
//         }

        Scalar lambda  = lambda0;        //*limsup;//e-1;//limsup;
        Scalar epsilon = 1e-16;          // Ratio between phib and phim+phid

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        //Eigen::ConjugateGradient< MatrixXr > cg;
        // Use ILUT preconditioner
        Eigen::ConjugateGradient< MatrixXr, Eigen::Upper, Eigen::DiagonalPreconditioner<Real> > cg;
        //Eigen::ConjugateGradient< MatrixXr, Eigen::Upper, Eigen::IncompleteLUT<Real> > cg;

        Scalar phid = (WdTWd*(b - b_pre)).norm();
        Scalar phim = (WmTWm*(x - xr)).norm();
        Scalar phib = PhiB2_NN(1., minVal, x);
        Scalar mux  = (phid + lambda*phim) / phib;
        //Scalar tol; //  = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);
        mux = (phid + lambda*phim) / phib;

        /// @todo add stopping criteria.
        for (int ii=0; ii<MAXITER; ++ii) {

            int iter = N*M;
            int iloop(0);
            int itertot(0);
            //VectorXr h;
            VectorXr ztilde;
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

                X1 = VectorXr::Ones(N).array() / (x.array()-minVal) ;
                X2 = VectorXr::Ones(N).array() / ((x.array()-minVal)*(x.array()-minVal));

                // Full
                b2 =  (A.transpose()*WdTWd*(b-b_pre)).array()
                     - lambda*(WmTWm*(x-xr)).array()
                     + (2.*mux)*X1.array(); // + (2.*muy)*Y1.array();

                // The first two terms could be moved out of the loop, don't know if its worth it
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
                //ztilde = cg.solveWithGuess(b2, x);
                ztilde = cg.solve(b2); // Newton step, don't guess!
                //std::cout << "out cg" << std::endl;
                iter = cg.iterations();
                //tol = cg.error();

                ++iloop;
                itertot += iter;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                //h = ztilde - x;

                // determing steplength
                Scalar d(1.);
                for (int ix=0; ix<x.size(); ++ix) {
                    if (ztilde[ix] < 0.) {
                        d = std::min(d, (Scalar).925*(x[ix]/std::abs(ztilde[ix])));
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
                //Scalar d = std::min(1., 0.9*((x.array()/ztilde.array()).abs()).minCoeff());

                // Make step
                x += d*ztilde;        // whole soln
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
                mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;

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
                      << std::setw(12) << itertot << std::setw(12) << iloop << std::setw(18) << ztilde.norm() <<  std::endl;

            phireport.precision(12);
            phireport << ii << "\t" << phim << "\t" <<  phid
                      << "\t" << lambda << "\t" << mux << "\t"
                      << itertot << "\t" << iloop  << "\t" << ztilde.norm() <<  std::endl;

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



    // Specialized Function that incorporates a buried modulus function.
    // solves the usual problem Ax = b, where A \in C but x and b are real.
    // b = mod(Ax).

    template <typename Scalar>
    VectorXr Tikhonov_CG(const MatrixXr &A, const VectorXr &xr,
        const VectorXr &b,
        const Eigen::SparseMatrix<Scalar>& WdTWd,
        const Eigen::SparseMatrix<Scalar>& WmTWm, Real lambda0=1e1) {


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
        obsdata << b.array() << std::endl;
        obsdata.close();


        // TODO make ATA implicit
        MatrixXr ATWdTWdA = A.transpose()*WdTWd*A;
        int N = A.cols();              // number of model
        int M = A.rows();              // number of dat
        int MAXITER = 175;             // M*N;

        //Scalar SIGMA  = .25;//5.85; //1e-2; // .25; //1e-2;   // 1e-1;
        Scalar delta  = 1e-4;
        Scalar lambda  = lambda0;        // * limsup;//e-1;//limsup;
        Scalar epsilon = 1e-15;          //  Ratio between phib and phim+phid

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;
        Scalar phid = (WdTWd*(b - b_pre)).norm();
        Scalar phim = (WmTWm*(x - xr)).norm();
        Scalar tol  = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

        //ConjugateGradient<SparseMatrix<double> > cg;
        // can't use Eigen, b/c we have mixed types of Re and complex
        Eigen::ConjugateGradient< MatrixXr > cg;
        //Eigen::BiCGSTAB< MatrixXcr > solver;
        /// @todo add stopping criteria.
        for (int ii=0; ii<MAXITER; ++ii) {

            int iter = N*M;
            int iloop(0);
            int itertot(0);
            VectorXr h;
            bool cont(true);

            do {

                //restart:

                //VectorXr  b2;
                //MatrixXr  A2;

                ///////////////////////////////////
                // setup


                // Newton step
                //b2 = - (A.transpose()*WdTWd*(b_pre-b)).array()
                //     - lambda*(WmTWm*(x-xr)).array();
                //     + (2.*mux)*X1.array() + (2.*muy)*Y1.array();

                // Full
                VectorXr b2 =  A.transpose()*WdTWd*b;

                MatrixXr A2 = ATWdTWdA;
                A2 += lambda*WmTWm;


//                 // Jacobi Preconditioner
//                 Eigen::SparseMatrix<Scalar> MM =
//                     Eigen::SparseMatrix<Scalar>(A2.rows(), A2.cols());
//                 for (int i=0; i<ATWdTWdA.rows(); ++i) {
//                     MM.insert(i,i) = 1./ATWdTWdA(i,i);
//                 }
//                 MM.finalize();


                /////////////////////////////////////////////////////////////
                // Solve system,
                // CG solution of complete system
                // TODO add reference model
                tol = 1e-5*phid;// 1e-14;
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
                //cg.compute(A2);
                //cg.setTolerance(tol);
                //VectorXr ztilde = (cg.solveWithGuess(b2,x.cast<Complex>())).real();
                //iter = cg.iterations();

                //VectorXr ztilde = CG(A2, x, b2, iter, tol);
                //cg.setTolerance();
                cg.compute(A2);
                x = cg.solveWithGuess(b2, x);
                b_pre = A*x;
                phid = (WdTWd*(b - b_pre)).norm();
                phim = (WmTWm*(x - xr)).norm();
                //std::cout << "out cg" << std::endl;

                ++iloop;
                itertot += iter;

                /////////////////////////////////////////////////////////////
                // update x, mux, muy
                //VectorXr  h = ztilde; // - x;

                // update x
                //h = ztilde - x;

                // determing steplength
                //Scalar d = std::min(1., 0.925*(x.array()/h.array().abs()).minCoeff() );
//                 Scalar d(1.);
//                 for (int ix=0; ix<x.size(); ++ix) {
//                     if (h[ix] < 0.) {
//                         d = std::min(d, (Scalar).925*(x[ix]/std::abs(h[ix])));
//                     }
//                 }

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
                //Scalar d = std::min(1., 0.9*((x.array()/ztilde.array()).abs()).minCoeff());

                // Make step
                //x += d*h;        // whole soln
                //x += d*ztilde; // Newton

//                 // TODO readd
//                 // Fix any overstepping
//                 for (int ib=0; ib<nblocks; ++ib) {
//                     while (x.segment(ib*block, block).sum() >= maxVal) {
//                         x.segment(ib*block, block).array() *= .99;
//                     }
//                 }
                /*
                for (int i=0; i<x.size(); ++i) {
                    if (x(i) < minVal) {
                        x(i) = minVal + delta;
                    }
                }

                b_pre = (A*x).array().abs();

                phib = PhiB2(mux, muy, minVal, maxVal, x, block, nblocks);
                phid = (WdTWd*(b-b_pre)).norm();
                phim = (WmTWm*(x-xr)).norm();

                // Determine mu steps to take
                VectorXr s1 = mux * (X2.asDiagonal()*(ztilde) - 2.*X1);
                mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                VectorXr s2 = muy * (Y2.asDiagonal()*(ztilde) - 2.*Y1);
                muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;

                if ( (std::abs(phib / (phid+lambda*phim)) < epsilon)) {
                //if ( (std::abs(phib / (phid+lambda*phim)) < epsilon) && h.norm() < 1e-5) {
                     cont = false;
                }
                */
                cont = false;
            } while ( cont );

            // report
            std::cout << std::ios::left;
            std::cout.precision(8);
            std::cout << std::setw(6) << ii << std::scientific << std::setw(18) << phim <<  std::setw(18) <<  phid
                      << std::setw(18) << lambda << std::setw(18) << cg.error()
                      << std::setw(12) << cg.iterations()  <<  std::endl;

            phireport.precision(12);
            phireport << ii << "\t" << phim << "\t" <<  phid
                      << "\t" << lambda << "\t" << 0.0 << "\t" << 0.0 << "\t"
                      << cg.iterations() << "\t" << 0.0 << "\t" << 0.0 <<  std::endl;

            std::fstream modfile;
            std::string fname = "iteration" + to_string(ii) + ".dat";
            modfile.open( fname.c_str(), std::ios::out);
            modfile << ii << "\t" << phim  << "\t" << phid
                    << "\t" << lambda << "\t" << 0 << "\t" << 0 << "\t" << cg.iterations() <<  "\n";
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
     *  Nonnegativity constraint, (Calavetti et. al. 2004)"
     *  Seeks the overdetermined least squares solution
     *  @param[in] A is a Real Matrix.
     *  @param[in] b is a Real vector
     *  @param[in] minVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] maxVal is the minimum allowed value \f$ x \in (minVal, maxVal) \f$
     *  @param[in] block is the number of parameters to sum together for the
     *  upper barrier term. So block block number parameters are kept under maxVal.
     *  as such A.rows() / block must be evenly divisible.
     */
    template <typename Scalar>
    VectorXr LogBarrierCGLS(const MatrixXr &A, const VectorXr &b, const Scalar &minVal,
        const Scalar &maxVal, const int& block) {

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
        Scalar SIGMA  = .25; // 1e-2;          //1e-1;
        Scalar delta  = 1e-8;

        // Cholesky preconditioner
        //Eigen::FullPivLU <MatrixXr> Pre;
        //Eigen::ColPivHouseholderQR <MatrixXr> Pre;
        //Pre.compute(ATA);

        // Determine starting lambda_0, find lim_sup of the norm of impulse responses and scale
        Scalar limsup = 1e10;
        for (int i=0; i<N; ++i) {
            VectorXr Spike = VectorXr::Zero(N);
            Spike(i) = (minVal + maxVal) / 2.;
            limsup = std::min(limsup, (ATA*Spike).array().abs().maxCoeff());
        }
        Scalar lambda  = 1e-6;//*limsup;//e-1;//limsup;
        Scalar mux0    = 1e-1*lambda;
        Scalar muy0    = 1e-1*lambda;
        Scalar epsilon = 1e-5;          // Ratio between phib and phim+phid

        /////////////////////////////
        // logX spacing
        //Scalar MinLam = 1e-24;
        //Scalar MaxLam = 1e-4;
        //VectorXr Lambdas;
        //Scalar LS = 5000;
        //Scalar dl10 = std::log(LS*MaxLam+1.)/(Scalar)MAXITER;
        //Lambdas = 1./LS*(dl10*VectorXr::LinSpaced(MAXITER+1, 0, MAXITER)).array().exp()
        // + MinLam - 1./LS;

        // initial guess, just near zero for now
        VectorXr x = VectorXr::Zero(N).array() + minVal+delta;// ((minVal + maxVal) / 2.);

        // predicted b
        VectorXr b_pre = A*x;

        int nblocks = x.size()/block;
        Scalar phid = (b - (b_pre)).norm();
        Scalar phim = x.norm();
        Scalar phib = PhiB2(mux0, muy0, minVal, maxVal, x, block, nblocks);
        Scalar mux = mux0;
        Scalar muy = muy0;
        Scalar tol = 1e-5*phid; // 1e-14;
        std::fstream phireport("phimphid.dat", std::ios::out);

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
//                 Eigen::SparseMatrix<Scalar> MM =
//                     Eigen::SparseMatrix<Scalar>(A2.rows(), A2.cols());
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
            //Scalar d = std::min(1., 0.995*(x.array()/h.array().abs()).minCoeff() );
            //x += d*h;
            Scalar d = std::min(1.,0.995*(x.array()/ztilde.array().abs()).minCoeff());
            x += d*ztilde;

//             // Fix any overstepping
//             for (int ib=0; ib<nblocks; ++ib) {
//                 while (x.segment(ib*block, block).sum() > maxVal) {
//                     x.segment(ib*block, block).array() *= (.9);
//                 }
//             }
//             for (int i=0; i<x.size(); ++i) {
//                 if (x(i) < minVal) {
//                     x(i) = minVal + delta;
//                 }
//             }
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
                    mux = SIGMA/((Scalar)(N)) * std::abs( s1.dot(x) ) ;
                }
                #ifdef LEMMAUSEOMP
                #pragma omp section
                #endif
                {
                    s2 = muy * (Y2.asDiagonal()*(x+ztilde) - 2.*Y1);
                    muy = SIGMA/((Scalar)(N)) * std::abs( s2.dot(x) ) ;
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


}

#endif   // ----- #ifndef LOGBARRIERCG_INC  -----
