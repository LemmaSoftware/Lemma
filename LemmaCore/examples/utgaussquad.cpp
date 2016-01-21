// ===========================================================================
// 
//       Filename:  utqaussquad.cpp
// 
//    Description:  Test for gaussian quadrature algorithm
// 
//        Version:  0.0
//        Created:  10/05/2010 08:54:26 AM
//       Revision:  none
//       Compiler:  Tested with g++
// 
//         Author:  M. Andy Kass (MAK)
//
//   Organisation:  Colorado School of Mines (CSM)
//                  Broken Spoke Development, LLC
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
using namespace std;

#ifdef LEMMAUSEVTK
#include "matplot.h"
using namespace matplot;
#endif

int main() {

	int N = 600;
	int upb = 5;
	int lowb = -1;
	VectorXr func(N);
	VectorXr wx(N);
	Real integrationres,actres;

	gaussianquadrature *lgqw = gaussianquadrature::New();
	lgqw->SetFreqs(N,upb,lowb);
	lgqw->CalcAW();

	wx=lgqw->GetAbscissae();
	func=cos(wx.array());
//	for (int ii=0;ii<N;++ii) {
//		cout << wx(ii) << "  " << func(ii) << endl;
//	}

	lgqw->SetFunc(func);
	lgqw->Integrate();
	integrationres=lgqw->GetResult();

	actres = -sin((Real)upb)+sin((Real)lowb);
	
	cout << "Gaussian Quadrature Result: " << integrationres << endl;
	cout << "Actual Result: " << actres << endl;
	
		
	//Plot things
	#ifdef LEMMAUSEVTK
	double colour1[3] = {0.0,0.0,1.0};

	Plot2D_VTK p1("X", "Y", 800,600);
	p1.plot(wx,func,colour1,".-");
	p1.show();

	#endif

	return EXIT_SUCCESS;
}

