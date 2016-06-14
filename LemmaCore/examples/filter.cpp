// ===========================================================================
//
//       Filename:  utfilter.cpp
//
//        Created:  07/20/2010 10:12:07 AM
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
  @author   Trevor Irons
  @date     07/20/2010
  @version   0.0
 **/

#include "WindowFilter.h"
using namespace Lemma;

int main() {

    auto Hamming = WindowFilter::NewSP();

        Hamming->SetWindowType( HANNING );
        Hamming->SetSamplingInterval(1e-4);
        Hamming->SetBandwidth(300);
        Hamming->SetNumberOfSamples(1./1e-4);

    std::cout << Hamming->GetFilterCoefficients() //.transpose()
              << std::endl;


}
