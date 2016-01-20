/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      09/25/2013 08:20:01 AM
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@xri-geo.com
 * @copyright Copyright (c) 2013, XRI Geophysics, LLC
 * @copyright Copyright (c) 2013, Trevor Irons
 */


#ifndef  RECTILINEARGRIDVTKEXPORTER_INC
#define  RECTILINEARGRIDVTKEXPORTER_INC

#ifdef  LEMMAUSEVTK

#include "LemmaObject.h"
#include "rectilineargrid.h"

#include "vtkXMLRectilinearGridWriter.h"
#include "vtkRectilinearGrid.h"
#include "vtkDoubleArray.h"
#include "vtkFieldData.h"
#include "vtkCellData.h"

namespace Lemma {

/**
  \brief   Exports a LemmaRectilinearGrid into a VTKRectilinearGrid.
  \details Results can then be dumped to a VTK file or used directly.
 */
class RectilinearGridVTKExporter : public LemmaObject {

    friend std::ostream &operator<<(std::ostream &stream,
            const RectilinearGridVTKExporter &ob);

    public:

    // ====================  LIFECYCLE     =======================

    /**
     * @copybrief LemmaObject::New()
     * @copydetails LemmaObject::New()
     */
    static RectilinearGridVTKExporter* New();

    /**
     *  @copybrief   LemmaObject::Delete()
     *  @copydetails LemmaObject::Delete()
     */
    void Delete();

    // ====================  OPERATORS     =======================

    // ====================  OPERATIONS    =======================

    // ====================  ACCESS        =======================
    /**
     * Sets the Grid to be exported
     */
    void SetGrid(RectilinearGrid* Grid);

    // ====================  INQUIRY       =======================

    /**
     *  @return Returns a pointer to a vtkRectilinearGrid containing same information as the Lemma Grid.
     */
    vtkRectilinearGrid*  GetVTKGrid();

    /**
     *  Writes the exported VTKGrid out to a .vtr XML file
     *  @param[in] name is the filename to write to. The .vtr extension will be added
     */
    void WriteVTKGrid( const std::string& name );

    protected:

    // ====================  LIFECYCLE     =======================

    /** Default protected constructor, use New */
    RectilinearGridVTKExporter (const std::string& name);

    /** Default protected destructor, use Delete */
    ~RectilinearGridVTKExporter ();

    /**
     *  @copybrief   LemmaObject::Release()
     *  @copydetails LemmaObject::Release()
     */
    void Release();

    private:

    void BuildVTKRectilinearGrid();

    // ====================  DATA MEMBERS  =========================

    /** The Grid to convert from */
    RectilinearGrid*    Grid;

    /** The Grid to convert to */
    vtkRectilinearGrid*    VTKGrid;

}; // -----  end of class  RectilinearGridVTKExporter  -----

}		// -----  end of Lemma  name  -----

#endif     // -----  not LEMMAUSEVTK  -----

#endif   // ----- #ifndef RECTILINEARGRIDVTKEXPORTER_INC  -----

