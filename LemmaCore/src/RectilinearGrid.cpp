/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     10/28/2010
  @version  $Id: rectilineargrid.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "RectilinearGrid.h"

namespace Lemma {


	std::ostream &operator<<(std::ostream &stream, const
	    RectilinearGrid &ob) {

		stream << *(LemmaObject*)(&ob);

        stream << "\tnx=" << ob.nx << "\tny=" << ob.ny << "\tnz=" << ob.nz << std::endl;
        stream << "\tox=" << ob.ox << "\toy=" << ob.oy << "\toz=" << ob.oz << std::endl;
        stream << "\tdx=" << ob.dx.transpose() << std::endl;
        stream << "\tdy=" << ob.dy.transpose() << std::endl;
        stream << "\tdz=" << ob.dz.transpose() << std::endl;
		return stream;
	}

    // ====================  LIFECYCLE     =======================

    RectilinearGrid::RectilinearGrid(const std::string& name) :
        Grid(name), nx(0), ny(0), nz(0) {

    }

    RectilinearGrid::~RectilinearGrid() {

    }

    std::shared_ptr< RectilinearGrid > RectilinearGrid::NewSP() {
        std::shared_ptr<RectilinearGrid> sp(new  RectilinearGrid("RectilinearGrid"), LemmaObjectDeleter() );
        return sp;
    }

    // ====================  OPERATIONS    =======================

    void RectilinearGrid::SetDimensions (const int &inx, const int &iny,
            const int &inz) {
        dx.resize(inx);
        dy.resize(iny);
        dz.resize(inz);
        nx = inx;
        ny = iny;
        nz = inz;
    }


    void RectilinearGrid::SetOffset (const Real& iox, const Real& ioy, const Real& ioz) {
        ox = iox;
        oy = ioy;
        oz = ioz;
    }

    void RectilinearGrid::SetSpacing (const VectorXr &idx, const VectorXr &idy,
                const VectorXr &idz) {
        nx = idx.size();
        ny = idy.size();
        nz = idz.size();
        dx = idx;
        dy = idy;
        dz = idz;
    }

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetNx
    //--------------------------------------------------------------------------------------
    int RectilinearGrid::GetNx (  ) {
        return nx;
    }		// -----  end of method RectilinearGrid::GetNx  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetNy
    //--------------------------------------------------------------------------------------
    int RectilinearGrid::GetNy (  ) {
        return ny;
    }		// -----  end of method RectilinearGrid::GetNy  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetNz
    //--------------------------------------------------------------------------------------
    int RectilinearGrid::GetNz (  ) {
        return nz;
    }		// -----  end of method RectilinearGrid::GetNz  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetOx
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetOx (  ) {
        return ox;
    }		// -----  end of method RectilinearGrid::GetOx  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetOy
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetOy (  ) {
        return oy;
    }		// -----  end of method RectilinearGrid::GetOy  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetOz
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetOz (  ) {
        return oz;
    }		// -----  end of method RectilinearGrid::GetOz  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDx
    //--------------------------------------------------------------------------------------
    VectorXr RectilinearGrid::GetDx (  ) {
        return dx;
    }		// -----  end of method RectilinearGrid::GetDx  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDx
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetDx ( const int& ix  ) {
        return dx[ix];
    }		// -----  end of method RectilinearGrid::GetDx  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDy
    //--------------------------------------------------------------------------------------
    VectorXr RectilinearGrid::GetDy (  ) {
        return dy;
    }		// -----  end of method RectilinearGrid::GetDy  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDy
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetDy ( const int& iy  ) {
        return dy[iy];
    }		// -----  end of method RectilinearGrid::GetDy  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDz
    //--------------------------------------------------------------------------------------
    VectorXr RectilinearGrid::GetDz (  ) {
        return dz;
    }		// -----  end of method RectilinearGrid::GetDz  -----

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetDz
    //--------------------------------------------------------------------------------------
    Real RectilinearGrid::GetDz ( const int& iz  ) {
        return dz[iz];
    }		// -----  end of method RectilinearGrid::GetDz  -----

}		// -----  end of Lemma  name  -----
