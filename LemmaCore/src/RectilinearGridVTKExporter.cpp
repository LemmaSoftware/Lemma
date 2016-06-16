/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/25/2013 08:20:14 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, Trevor Irons
 */

#if  LEMMAUSEVTK

#include "RectilinearGridVTKExporter.h"

namespace Lemma {

// ====================  FRIEND METHODS  =====================

std::ostream &operator << (std::ostream &stream, const RectilinearGridVTKExporter &ob) {
    stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
    return stream;
}


// ====================  LIFECYCLE     =======================

//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  RectilinearGridVTKExporter
// Description:  constructor (protected)
//--------------------------------------------------------------------------------------
RectilinearGridVTKExporter::RectilinearGridVTKExporter ( ) : LemmaObject( ), Grid(nullptr), VTKGrid(nullptr) {

}  // -----  end of method RectilinearGridVTKExporter::RectilinearGridVTKExporter  (constructor)  -----


//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  NewSP()
// Description:  public constructor
//--------------------------------------------------------------------------------------
std::shared_ptr< RectilinearGridVTKExporter > RectilinearGridVTKExporter::NewSP() {
    std::shared_ptr<RectilinearGridVTKExporter> sp(new  RectilinearGridVTKExporter( ), LemmaObjectDeleter() );
    return sp;
}



//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  ~RectilinearGridVTKExporter
// Description:  destructor (protected)
//--------------------------------------------------------------------------------------
RectilinearGridVTKExporter::~RectilinearGridVTKExporter () {
}  // -----  end of method RectilinearGridVTKExporter::~RectilinearGridVTKExporter  (destructor)  -----

//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  SetGrid
//--------------------------------------------------------------------------------------
void RectilinearGridVTKExporter::SetGrid ( std::shared_ptr<RectilinearGrid> GridIn ) {
    Grid = GridIn;
    // I see no harm in just doing this now.
    BuildVTKRectilinearGrid();
    return ;
}		// -----  end of method RectilinearGridVTKExporter::SetGrid  -----

//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  GetVTKRectilinearGrid
//--------------------------------------------------------------------------------------
vtkSmartPointer<vtkRectilinearGrid> RectilinearGridVTKExporter::GetVTKGrid (  ) {
    return VTKGrid;
}		// -----  end of method RectilinearGridVTKExporter::GetVTKRectilinearGrid  -----

//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  WriteVTKGrid
//--------------------------------------------------------------------------------------
void RectilinearGridVTKExporter::WriteVTKGrid ( const std::string& fname  ) {
    vtkXMLRectilinearGridWriter *gridWrite = vtkXMLRectilinearGridWriter::New();
        gridWrite->SetInputData(VTKGrid);
        gridWrite->SetFileName( (fname+std::string(".vtr")).c_str() );
        gridWrite->Update();
        gridWrite->Write();
        gridWrite->Delete();
    return ;
}		// -----  end of method RectilinearGridVTKExporter::WriteVTKGrid  -----

//--------------------------------------------------------------------------------------
//       Class:  RectilinearGridVTKExporter
//      Method:  BuildVTKRectilinearGrid
//--------------------------------------------------------------------------------------
void RectilinearGridVTKExporter::BuildVTKRectilinearGrid (  ) {

    // Set Coordinate>s
    vtkDoubleArray *xCoords = vtkDoubleArray::New();
    xCoords->InsertNextValue(Grid->GetOx()-Grid->GetDx(0)/2.);
    double xm1 = Grid->GetOx() - Grid->GetDx(0)/2.;
    for (int ix=0; ix<Grid->GetNx(); ix++) {
        xCoords->InsertNextValue(xm1 + Grid->GetDx(ix));
        xm1 += Grid->GetDx(ix);
    }

    vtkDoubleArray *yCoords = vtkDoubleArray::New();
    yCoords->InsertNextValue(Grid->GetOy()-Grid->GetDy(0)/2.);
    double ym1 = Grid->GetOy()-Grid->GetDy(0)/2.;
    for (int iy=0; iy<Grid->GetNy(); iy++) {
        yCoords->InsertNextValue(ym1 + Grid->GetDy(iy));
        ym1 += Grid->GetDy(iy);
    }

    vtkDoubleArray *zCoords = vtkDoubleArray::New();
    zCoords->InsertNextValue(Grid->GetOz()-Grid->GetDz(0)/2.);
    double zm1 = Grid->GetOz()-Grid->GetDz(0)/2.;
    for (int iz=0; iz<Grid->GetNz(); iz++) {
        zCoords->InsertNextValue(zm1 + Grid->GetDz(iz));
        zm1 += Grid->GetDz(iz);
    }

    VTKGrid = vtkSmartPointer<vtkRectilinearGrid>::New();
        VTKGrid->SetDimensions(Grid->GetNx()+1, Grid->GetNy()+1, Grid->GetNz()+1);
        VTKGrid->SetXCoordinates(xCoords);
        VTKGrid->SetYCoordinates(yCoords);
        VTKGrid->SetZCoordinates(zCoords);

    // Clean up
    xCoords->Delete();
    yCoords->Delete();
    zCoords->Delete();

    return ;
}		// -----  end of method RectilinearGridVTKExporter::BuildVTKRectilinearGrid  -----



}		// -----  end of Lemma  name  -----

#endif     // -----  not LEMMAUSEVTK  -----
