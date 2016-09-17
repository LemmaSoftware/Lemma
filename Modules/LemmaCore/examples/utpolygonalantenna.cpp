// ===========================================================================
//
//       Filename:  utpolygonalantennae.cpp
//
//    Description:  Unit test of PolygonalAntenna
//
//        Version:  0.0
//        Created:  05/26/2010 04:30:56 PM
//       Revision:  none
//       Compiler:  Tested with g++, icpc, and MSVC 2000
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

#include "Lemma"

using namespace Lemma;

int main() {

    PolygonalWireAntenna *pa = PolygonalWireAntenna::New();
        pa->SetNumberOfPoints(5);
        pa->SetPoint(0, Vector3r(  0,  0, -1e-3));
        pa->SetPoint(1, Vector3r( 10,  0, -1e-3));
        pa->SetPoint(2, Vector3r( 10, 10, -1e-3));
        pa->SetPoint(3, Vector3r(  0, 10, -1e-3));
        pa->SetPoint(4, Vector3r(  0,  0, -1e-3));
        pa->SetNumberOfFrequencies(1);
        pa->SetFrequency(0,1000.);
        pa->SetCurrent(10.);
        pa->SetNumberOfTurns(1);

    Vector3r pos;
    pos << 23,23,23;
    pa->ApproximateWithElectricDipoles(pos);
    std::cout << "pos "<< pos.transpose() << "  ndipoles="
              << pa->GetNumberOfDipoles() << std::endl;
    pos << 2,2,2;
    pa->ApproximateWithElectricDipoles(pos);
    std::cout << "pos "<< pos.transpose() << "  ndipoles="
              << pa->GetNumberOfDipoles() << std::endl;
    pos << 1,1,1;
    pa->ApproximateWithElectricDipoles(pos);
    std::cout << "pos "<< pos.transpose() << "  ndipoles="
              << pa->GetNumberOfDipoles() << std::endl;

    pa->Delete();
}
