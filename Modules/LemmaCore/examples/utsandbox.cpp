// ===========================================================================
//
//       Filename:  utsandbox.cpp
//
//        Created:  07/29/2010 10:34:23 AM
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
  @file test sandbox helm1rameters
  @author   Trevor Irons
  @date     07/29/2010
  @version   0.0
 **/

#include "Lemma"

using namespace Lemma;

int main() {

    PolygonalWireAntenna *helm1 = PolygonalWireAntenna::New();
	helm1->SetNumberOfFrequencies(1);
	helm1->SetFrequency(0, 1000);
	helm1->SetNumberOfPoints(5);
	helm1->SetPoint(0, Vector3r(   0,   0, 0));
	helm1->SetPoint(1, Vector3r(   0,   0, 0));
	helm1->SetPoint(2, Vector3r(   0,   0, 0));
	helm1->SetPoint(3, Vector3r(   0,   0, 0));
	helm1->SetPoint(4, Vector3r(   0,   0, 0));
	helm1->SetCurrent(1.);
    helm1->Delete();
}
