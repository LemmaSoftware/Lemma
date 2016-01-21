// ===========================================================================
//
//       Filename:
//
//    Description:
//
//        Version:  0.0
//        Created:  03/02/2010 02:10:53 PM
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

	DataTEM * tdata = DataTEM::New();
	std::cout << *tdata << std::endl;
	tdata->Zero();
	tdata->Delete();

}
