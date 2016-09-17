// ===========================================================================
// 
//       Filename:  utreadfemubc.cpp
// 
//    Description:  
// 
//        Version:  0.0
//        Created:  01/03/2013 02:47:24 PM
//       Revision:  none
//       Compiler:  Tested with g++ 
// 
//         Author:  M. Andy Kass (MAK)
//
//   Organisation:  Broken Spoke Development, LLC
//                  
//
//          Email:  mkass@numericalgeo.com
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

#ifdef LEMMAUSEVTK
#include "matplot.h"
using namespace matplot;
#endif

int main() {

	std::string datfile;

	datfile = "126.obs";
	
	DataReaderFemUBC* Reader = DataReaderFemUBC::New();
	DataFEM* thedata = DataFEM::New();

	Reader->SetDataFEM(thedata);

	try {
	Reader->ReadData(datfile,1);
	} catch(std::exception& e) {
		exit(EXIT_FAILURE);
	}

	std::cout << *Reader << std::endl;
	std::cout << *thedata << std::endl;


	thedata->Delete();
	Reader->Delete();

	return EXIT_SUCCESS;
}
