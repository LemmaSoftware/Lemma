// ===========================================================================
//
//       Filename:  utdigitalfiltercostrans.cpp
//
//        Created:  02/08/2011 11:52:39 AM
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
  @date     02/08/2011
  @version   0.0
 **/

#include "digitalfiltercostrans.h"
#include "costransintegrationkernel.h"
#include "integrationkernel.h"

//class TestDriverKernel : public IntegrationKernel<Real> {
//
//};

using namespace Lemma;

int main() {

	//Real A = 2.;
	//Real B = 12.5;

    Real A = 1.;
    Real B = 2.;
    DigitalFilterCosTrans* Cos = DigitalFilterCosTrans::New();
    std::cout << *Cos << std::endl;
    CosTransIntegrationKernel* Kernel = CosTransIntegrationKernel::New();
    Cos->AttachKernel(Kernel);

    Cos->Compute(B, 1, 1e-16);
    std::cout << "numeric  " << Cos->GetAnswer() << std::endl;
    Real SQPI = std::sqrt(PI);
    Real analytic =  SQPI*std::exp(-B*B/(4.*A*A))/(2.*A);
    std::cout << "analytic " << analytic <<std::endl;
    std::cout << "absolute error " <<  Cos->GetAnswer()(0,0) - analytic <<std::endl;
    std::cout << "relative error " << (Cos->GetAnswer()(0,0) - analytic) / analytic <<std::endl;

    Cos->Delete();
    Kernel->Delete();
}
