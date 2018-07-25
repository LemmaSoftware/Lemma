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

    std::ostream &operator << (std::ostream &stream, const RectilinearGrid &ob) {
        stream << ob.Serialize()  << "\n"; 
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    RectilinearGrid::RectilinearGrid( const ctor_key& key ) : Grid( key ), nx(0), ny(0), nz(0) {

    }

    RectilinearGrid::RectilinearGrid( const YAML::Node& node, const ctor_key& key ) : Grid(node, key) {

        nx = node["nx"].as<int>( );
        ny = node["ny"].as<int>( );
        nz = node["nz"].as<int>( );

        ox = node["ox"].as<Real>( );
        oy = node["oy"].as<Real>( );
        oz = node["oz"].as<Real>( );

        dx = node["dx"].as< VectorXr >();
        dy = node["dy"].as< VectorXr >();
        dz = node["dz"].as< VectorXr >();

    }

    RectilinearGrid::~RectilinearGrid() {

    }

    std::shared_ptr< RectilinearGrid > RectilinearGrid::NewSP() {
        return std::make_shared< RectilinearGrid > ( ctor_key() );
    }

    YAML::Node RectilinearGrid::Serialize() const {
        YAML::Node node = Grid::Serialize();

        node["nx"] = nx;
        node["ny"] = ny;
        node["nz"] = nz;

        node["ox"] = ox;
        node["oy"] = oy;
        node["oz"] = oz;

        node["dx"] = dx;
        node["dy"] = dy;
        node["dz"] = dz;

        node.SetTag( this->GetName() );
        return node;
    }


    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<RectilinearGrid> RectilinearGrid::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() != "RectilinearGrid") {
            throw  DeSerializeTypeMismatch( "RectilinearGrid", node.Tag());
        }
        return std::make_shared< RectilinearGrid > ( node, ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  RectilinearGrid
    //      Method:  GetName
    // Description:  Class identifier
    //--------------------------------------------------------------------------------------
    inline std::string RectilinearGrid::GetName (  ) const {
        return CName;
    }		// -----  end of method RectilinearGrid::get_GetName  -----

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
        // MSVC doens't allow static_cast here
        //nx = std::static_cast<int>( idx.size() );
        //ny = std::static_cast<int>( idy.size() );
        //nz = std::static_cast<int>( idz.size() );
        nx = (int)( idx.size() );
        ny = (int)( idy.size() );
        nz = (int)( idz.size() );
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

/* vim: set tabstop=4 expandtab: */
/* vim: set filetype=cpp: syntax=cpp.doxygen*/
