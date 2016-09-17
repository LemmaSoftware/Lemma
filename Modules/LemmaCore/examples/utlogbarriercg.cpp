// ===========================================================================
//
//       Filename:  utlogbarriercg.cpp
//
//        Created:  12/29/2010 09:40:23 AM
//       Compiler:  Tested with g++, icpc, and MSVC 2010
//
//         Author:  Trevor Irons (ti)
//
//   Organisation:  Colorado School of Mines (CSM)
//                  United States Geological Survey (USGS)
//
//          Email:  tirons@mines.edu, tirons@usgs.gov
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ===========================================================================

/**
  @file
  @author   Trevor Irons
  @date     12/29/2010
  @version   0.0
 **/

#include "LemmaObject.h"
//#include "logbarriercg.h"
//#include "datasnmr.h"
#include <ctime>

#ifdef HAVEBOOSTRANDOM
#include <boost/random.hpp>
#include <boost/random/normal_distribution.hpp>
#endif

using namespace std;
using namespace Lemma;

MatrixXr BuildG(const Real& T2, const Real& Larmor, const Real& T,
        const Real& dt, const VectorXr& nT2Bins, const Real& scale);

MatrixXr BuildGw(const Real& T2, const Real& Larmor, const Real& T,
        const Real& dt, const VectorXr& nT2Bins, const Real& scale,
        const VectorXr& freqs);

/// test driver program for log-barrier optimization algorithm
int main() {
    return 0;
}

#ifdef AA
    // Parameters
    Real T2(.200);
    Real Larmor(2000); // Hz
    Real wL = 2.*PI * Larmor;
    Real T(.6);
    Real dt(1e-4);
    int  nT2(20);
    Real T2Low(.020);
    Real T2Hi(.220);

    // T2 bins
    VectorXr T2Bins = VectorXr::LinSpaced(nT2, T2Low, T2Hi);

    // Build true model
    VectorXr m = VectorXr::Zero(T2Bins.size());
    //m[4]  = .15;
    //m[5]  = .15;
    //m[6]  = .15;
    m[7]  = .15;
    m[8]  = .15;
    m[11] = .15;
    m[12] = .15;
    m[13] = .15;
    m[14] = .15;

    // Build G
    srand ( std::time(NULL) );
    Real scale = 1e-4*std::abs(VectorXr::Random(1)[0]);
    MatrixXr G = BuildG(T2, wL, T, dt, T2Bins, scale);

    #ifdef HAVEBOOSTRANDOM
    Real mean = 0;
    Real variance = scale*.05*(.5+.15);
    boost::mt19937 randgen(static_cast<unsigned int> (std::time(0)));
    boost::normal_distribution<Real> noise(mean, variance);
    boost::variate_generator<boost::mt19937, boost::normal_distribution<Real> > nD(randgen, noise);
    VectorXr noisy = G*m;
//     for (int i=0; i < noisy.size(); ++i) {
//        noisy(i) += nD();
//     }
    #else
    VectorXr d = G * m;
    VectorXr noisy = d.array();// + .05*( .5 + .15 ) *Eigen::ArrayXd::Random(d.size());
    #endif

    // Clean Data, and 10% random noise data
    VectorXr RefMod = VectorXr::Zero(m.size());

    // todo let alpha's be set.
    Real alpha_s  =  .0;
    Real alpha_t2 = 1.0;
    Eigen::SparseMatrix<Real> Wd (G.rows(), G.rows());
    Eigen::SparseMatrix<Real> Wm (G.cols(), G.cols());

    /////////////////
    // Model objective function
    int ii = 0;
    for (int iT2=0; iT2<nT2; ++iT2) {
        Wm.coeffRef(ii, ii) += alpha_s - alpha_t2;
        if (iT2 < nT2-1) {
            Wm.coeffRef(ii, ii+1) += alpha_t2;
        }
        ++ii;
    }
    Wm.finalize();
    std::fstream  WmOut( "wmMatrix.dat" , std::ios::out);
    WmOut << Wm << std::endl;
    WmOut.close();

    // Data objective function
    /// @todo add measure of suspected std dev to this.
    for (int id=0; id<G.rows(); ++id) {
        Wd.coeffRef(id, id) = 1.; // 1/sigma(i);
    }
    Wd.finalize();

    // invert time-domain sinusoid exponent
    //LogBarrierCG(G, RefMod, noisy, (Real)0., (Real)1e5, nT2, Wd, Wm, scale*1e1);
    return EXIT_SUCCESS;

//     #if 0
//     // invert FD
//     Eigen::FFT<Real> fft;
//     fft.SetFlag(fft.HalfSpectrum);
//     VectorXcr dw;
//     fft.fwd(dw, noisy);
//
//     // Compute freqs Hz
//     VectorXr freqs = DataSNMR::GetNMRSampledFrequencies(T/dt, dt);
//     Real df = freqs[1];
//     Real window = 300; // Hz
//     int iL = (int)(Larmor/df);
//     MatrixXr Gw = BuildGw(T2, wL, T, dt, T2Bins, scale, freqs.segment(iL-window/2, window));
//
//     VectorXr dwu = VectorXr::Zero(2.*window);
//     dwu.head(window) = dw.segment(iL-window/2, window).real();
//     dwu.tail(window) = dw.segment(iL-window/2, window).imag();
//
//     Eigen::DynamicSparseMatrix<Real> Wdw (Gw.rows(), Gw.rows());
//     for (int id=0; id<Gw.rows(); ++id) {
//         Wdw.coeffRef(id, id) = 1.; // 1/sigma(i);
//     }
//     Wdw.finalize();
//     std::cout << "psize " << (Gw*RefMod).size() << std::endl;
//     VectorXr dw2 = Gw*m;
//     //std::cout << dw2 << std::endl;
//     std::cout << "Gw " << Gw.rows() << "\t" << Gw.cols() << std::endl;
//     std::cout << "dwu " << dwu.size() << std::endl;
//     //RefMod.array() += .1;
//     VectorXr mod = LogBarrierCG(Gw, RefMod, dwu, 0., 3., nT2, Wdw, Wm,scale*1e5);
//     //VectorXr mod = LogBarrierCG(Gw, dwu, 0., 1e5);
//
//     std::fstream Gws("gw.out", std::ios::out);
//     Gws << Gw << std::endl;
//     Gws.close();
//     #endif
}

#endif

MatrixXr BuildG(const Real& T2, const Real& wL, const Real& T,
            const Real& dt, const VectorXr& T2Bins, const Real& scale) {
    std::cout << "scale " << scale << std::endl;
    int nt = T/dt;
    int nT2 = T2Bins.size();
    MatrixXr G = MatrixXr::Zero(nt,nT2);
    for (int iT2=0; iT2<nT2; ++iT2) {
        Real t=0;
        for (int it=0; it<nt; ++it) {
            G(it, iT2) = scale * exp(-t/T2Bins(iT2)) * sin(wL * t);
            t += dt;
        }
    }
    return G;
}


MatrixXr BuildGw(const Real& T2, const Real& wL, const Real& T,
            const Real& dt, const VectorXr& T2Bins, const Real& scale,
            const VectorXr &freqs) {
    VectorXr omegas = freqs.array() * 2.*PI;
    std::cout << "scale " << scale << std::endl;
    int nT2 = T2Bins.size();
    MatrixXr Gw = MatrixXr::Zero(2*freqs.size(), nT2);
    int nW = freqs.size();
    Real sc = 1./dt;
    for (int iT2=0; iT2<nT2; ++iT2) {
        for (int iw=0; iw<freqs.size(); ++iw) {
            Real a = 1./T2Bins(iT2);
            Complex T = wL / ( wL*wL + (a+Complex(0, omegas[iw]))*(a+Complex(0,omegas[iw])) ) ;
            Gw(iw, iT2)     = sc * scale * std::real(T);
            Gw(iw+nW, iT2) = sc * scale * std::imag(T);
        }
    }
    return Gw;
}
