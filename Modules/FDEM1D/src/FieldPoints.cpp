/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     12/02/2009
  @version  $Id: receiverpoints.cpp 203 2015-01-09 21:19:04Z tirons $
 **/

#include "FieldPoints.h"

namespace Lemma {

    // ====================    FRIENDS     ======================

    std::ostream &operator << (std::ostream &stream, const FieldPoints &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc ---
        return stream;
    }

    // ====================  LIFECYCLE     ===================================

    FieldPoints::FieldPoints( const ctor_key& ) : LemmaObject( ),
        NumberOfPoints(0), NumberOfBinsE(0), NumberOfBinsH(0) {
    }

    //--------------------------------------------------------------------------------------
    //       Class:  FieldPoints
    //      Method:  FieldPoints
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    FieldPoints::FieldPoints (const YAML::Node& node, const ctor_key&) : LemmaObject(node) {
        //DeSerialize
        NumberOfPoints = node["NumberOfPoints"].as<int>();
        NumberOfBinsE = node["NumberOfBinsE"].as<int>();
        NumberOfBinsH = node["NumberOfBinsH"].as<int>();
        Mask = node["Mask"].as<VectorXi>();
        Locations = node["Locations"].as<Vector3Xr>();
    }  // -----  end of method FieldPoints::FieldPoints  (constructor)  -----

    FieldPoints::~FieldPoints() {
    }

    std::shared_ptr<FieldPoints> FieldPoints::NewSP() {
        return std::make_shared<FieldPoints> ( ctor_key() );
    }

    //--------------------------------------------------------------------------------------
    //       Class:  FieldPoints
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  FieldPoints::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize();
        node.SetTag( GetName() );

        // update here
        node["NumberOfPoints"] = NumberOfPoints;
        node["NumberOfBinsE"] = NumberOfBinsE;
        node["NumberOfBinsH"] = NumberOfBinsH;
        node["Mask"] = Mask;
        node["Locations"] = Locations;// Can be huge
        //std::cout << "Locations.data" << Locations.data()[0] << std::endl;
        return node;
    }		// -----  end of method FieldPoints::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  FieldPoints
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    std::shared_ptr<FieldPoints> FieldPoints::DeSerialize ( const YAML::Node& node  ) {
        if (node.Tag() != "FieldPoints") {
            throw  DeSerializeTypeMismatch( "FieldPoints", node.Tag());
        }
        return std::make_shared<FieldPoints> ( node, ctor_key() );
    }		// -----  end of method FieldPoints::DeSerialize  -----

    // ====================  ACCESS        ===================================

    void FieldPoints::SetNumberOfPoints(const int &nrec) {

        if (nrec > 0)
            this->NumberOfPoints = nrec;
        else
            throw std::runtime_error("NUMBER RECEIVERS LESS THAN 1");

        this->Locations.resize(Eigen::NoChange, nrec);
        Locations.setZero();

        this->Mask.resize(nrec);
        Mask.setZero();

        ResizeEField();
        ResizeHField();
    }

    void FieldPoints::ResizeEField() {
        Efield.clear();
        for (int i=0; i<NumberOfBinsE; ++i) {
            Eigen::Matrix<Complex, 3, Eigen::Dynamic> tempe;
            this->Efield.push_back(tempe);
            this->Efield[i].resize(Eigen::NoChange, NumberOfPoints);
            this->Efield[i].setZero();
        }
    }

    void FieldPoints::ResizeHField() {
        Hfield.clear();
        for (int i=0; i<NumberOfBinsH; ++i) {
            Eigen::Matrix<Complex, 3, Eigen::Dynamic> temph;
            this->Hfield.push_back(temph);
            this->Hfield[i].resize(Eigen::NoChange, NumberOfPoints);
            this->Hfield[i].setZero();
        }
    }

    void FieldPoints::SetNumberOfBinsE(const int& nbins) {
        NumberOfBinsE = nbins;
        ResizeEField();
    }

    void FieldPoints::SetNumberOfBinsH(const int& nbins) {
        NumberOfBinsH = nbins;
        ResizeHField();
    }

    void FieldPoints::SetLocation(const int&nrec,const  Vector3r& loc) {
        this->Locations.col(nrec) = loc;
    }

    void FieldPoints::SetLocation(const int&nrec,const  Real& xp,
                    const Real& yp, const Real& zp) {
        this->Locations.col(nrec) << xp, yp, zp;
    }

    void FieldPoints::SetEfield(const int& nbin,
                    const int& loc, const Complex &ex,
                    const Complex &ey, const Complex &ez) {
        this->Efield[nbin].col(loc) << ex, ey, ez;
    }

    void FieldPoints::AppendEfield(const int&nbin, const int& loc,
                    const Complex &ex,
                    const Complex &ey, const Complex &ez) {
        #ifdef LEMMAUSEOMP
        #pragma omp critical
        #endif
        this->Efield[nbin].col(loc) += Vector3cr(ex, ey, ez); //temp;
    }

    void FieldPoints::SetHfield(const int &nbin, const int& loc,
                    const Complex &hx, const Complex &hy,
                    const Complex &hz) {
        this->Hfield[nbin].col(loc) << hx, hy, hz;
    }

    void FieldPoints::AppendHfield(const int &nbin, const int& loc,
                    const Complex &hx, const Complex &hy,
                    const Complex &hz) {
//      #ifdef LEMMAUSEOMP
//      #pragma omp atomic
//      std::real(this->Hfield[nbin].col(loc)[0]) += std::real(hx);
//      #pragma omp atomic
//      std::imag(this->Hfield[nbin].col(loc)[0]) += std::imag(hx);
//      #pragma omp atomic
//      std::real(this->Hfield[nbin].col(loc)[1]) += std::real(hy);
//      #pragma omp atomic
//      std::imag(this->Hfield[nbin].col(loc)[1]) += std::imag(hy);
//      #pragma omp atomic
//      std::real(this->Hfield[nbin].col(loc)[2]) += std::real(hz);
//      #pragma omp atomic
//      std::imag(this->Hfield[nbin].col(loc)[2]) += std::imag(hz);
//          #else
        //(critical sections are slow)
        #ifdef LEMMAUSEOMP
        #pragma omp critical
        #endif
        this->Hfield[nbin].col(loc) += Vector3cr(hx,hy,hz);
        //#endif
    }


    // ====================  INQUIRY       ===================================
    Vector3Xr FieldPoints::GetLocations() {
        return this->Locations;
    }

    MatrixXr FieldPoints::GetLocationsMat() {
        return MatrixXr(this->Locations);
    }

    Vector3r FieldPoints::GetLocation(const int &nrec) {
        return this->Locations.col(nrec);
    }

    Real FieldPoints::GetLocationX(const int &nrec) {
        return this->Locations.col(nrec)[0];
    }

    Real FieldPoints::GetLocationY(const int &nrec) {
        return this->Locations.col(nrec)[1];
    }

    Real FieldPoints::GetLocationZ(const int &nrec) {
        return this->Locations.col(nrec)[2];
    }

    Vector3cr FieldPoints::GetEfield(const int &nfreq, const int&nrec) {
        return this->Efield[nfreq].col(nrec);
    }

    Vector3cr FieldPoints::GetHfield(const int &nfreq, const int&nrec) {
        return this->Hfield[nfreq].col(nrec);
    }

    std::vector<Vector3Xcr> FieldPoints::GetHfield( ) {
        return this->Hfield;
    }

    std::vector<Vector3Xcr> FieldPoints::GetEfield( ) {
        return this->Efield;
    }

    Vector3Xcr FieldPoints::GetEfield (const int &nfreq) {
        return this->Efield[nfreq];
    }

    MatrixXcr FieldPoints::GetEfieldMat (const int &nfreq) {
        return MatrixXcr(this->Efield[nfreq]);
    }

    MatrixXcr FieldPoints::GetHfieldMat (const int &nfreq) {
        return MatrixXcr(this->Hfield[nfreq]);
    }

    Vector3Xcr FieldPoints::GetHfield (const int &nfreq) {
        return this->Hfield[nfreq];
    }

    void FieldPoints::MaskPoint(const int& imask) {
        Mask(imask) = true;
    }

    void FieldPoints::UnMaskPoint(const int& imask) {
        Mask(imask) = false;
    }

    void FieldPoints::UnMaskAllPoints() {
        Mask.setZero();
    }

    int FieldPoints::GetMask(const int& i) {
        return Mask(i);
    }

    int FieldPoints::GetNumberOfPoints() {
        return this->NumberOfPoints;
    }

    void FieldPoints::ClearFields() {
        for (int i=0; i<NumberOfBinsE; ++i) {
            this->Efield[i].setZero();
        }
        for (int i=0; i<NumberOfBinsH; ++i) {
            this->Hfield[i].setZero();
        }
    }

    #ifdef LEMMAUSEVTK
    vtkActor* FieldPoints::GetVtkGlyphActor(const FIELDTYPE &ftype,
                    const Real &clip, const Real &scale,
                    const int &nfreq) {

        vtkArrowSource        *vArrow;
        vtkGlyph3D            *vGlyph;
        vtkActor              *vActor;
        vtkPolyDataMapper     *vPolyMapper;
        vtkPoints             *vPoints;
        vtkDoubleArray        *vVects;
        vtkPolyData           *vPointSet;

        vActor = vtkActor::New();
        vGlyph = vtkGlyph3D::New();
        vArrow = vtkArrowSource::New();
        vPolyMapper = vtkPolyDataMapper::New();
        vPoints = vtkPoints::New();
        vPointSet = vtkPolyData::New();
        vVects = vtkDoubleArray::New();

        vVects->SetNumberOfComponents(3);

        // Make PointData
        for (int ic=0; ic<NumberOfPoints; ++ic) {

            Vector3r loc = this->GetLocation(ic);

            vPoints->InsertPoint(ic, loc[0], loc[1],
                            loc[2]);
            Vector3r temp;
            temp.setZero();

            switch (ftype) {
                case HFIELDREAL:
                    temp = scale*(GetHfield(nfreq,ic).real());
                    break;
                case HFIELDIMAG:
                    temp = scale*(GetHfield(nfreq,ic).imag());
                    break;
                case EFIELDREAL:
                    temp = scale*(GetEfield(nfreq, ic).real());
                    break;
                case EFIELDIMAG:
                    temp = scale*(GetEfield(nfreq, ic).imag());
                    break;
            }
            if (temp.norm() > clip) {
                temp /= temp.norm(); // norm
                temp *= clip;        // clip dimension
            }

            vVects->InsertTuple3(ic, temp(0), temp(1),
                            temp(2));
        }

        vPointSet->SetPoints(vPoints);
        vPointSet->GetPointData()->SetVectors(vVects);
        vGlyph->ScalingOn();
        vGlyph->SetScaleModeToScaleByVector();
        vGlyph->SetSourceConnection(vArrow->GetOutputPort());
        vGlyph->SetVectorMode(true);
        vGlyph->SetVectorModeToUseVector();
        vGlyph->OrientOn();
        vGlyph->SetInputData(vPointSet);
        vPolyMapper->SetInputConnection(vGlyph->GetOutputPort());
        vActor->SetMapper(vPolyMapper);

        if (vArrow != NULL) {
            vArrow->Delete();
            vArrow = NULL;
        }
        //if (vActor != NULL) {
        //  vActor->Delete();
        //  vActor = NULL;
        //}
        if (vPolyMapper != NULL) {
            vPolyMapper->Delete();
            vPolyMapper = NULL;
        }
        if (vVects != NULL) {
            vVects->Delete();
            vVects = NULL;
        }
        if (vPointSet != NULL) {
            vPointSet->Delete();
            vPointSet = NULL;
        }
        if (vPoints != NULL) {
            vPoints->Delete();
            vPoints = NULL;
        }
        if (vGlyph != NULL) {
            vGlyph->Delete();
            vGlyph = NULL;
        }

        return vActor;
    }

    vtkDataObject* FieldPoints::GetVtkDataObject(const FIELDTYPE &ftype,
                    const int& nbin,
                    const int& start, const int& end,
                    const FIELDCOMPONENT& fcomp,
                    const SPATIALCOORDINANT &scord) {

        if (start < 0) throw 77;
        if (end > NumberOfPoints) throw 78;
        if (start > end) throw 79;

        int ifc(-1);
        switch (fcomp) {
            case XCOMPONENT:
                ifc = 0;
                break;
            case YCOMPONENT:
                ifc = 1;
                break;
            case ZCOMPONENT:
                ifc = 2;
                break;
        }

        int isc(-1);
        switch (scord) {
            case XCOORD:
                isc = 0;
                break;

            case YCOORD:
                isc = 1;
                break;

            case ZCOORD:
                isc = 2;
                break;
        }

        vtkDoubleArray *_data = vtkDoubleArray::New();
            _data->SetNumberOfComponents(1);

        vtkDoubleArray *_pos = vtkDoubleArray::New();
            _pos->SetNumberOfComponents(1);

        int id=0;
        for (int irec=start; irec<end; ++irec) {

            switch (ftype) {

                case HFIELDREAL:
                    _data->InsertTuple1(id, std::real(Hfield[nbin](ifc, irec)));
                    break;

                case HFIELDIMAG:
                    _data->InsertTuple1(id, std::imag(Hfield[nbin](ifc, irec)));
                    break;

                case EFIELDREAL:
                    _data->InsertTuple1(id, std::real(Efield[nbin](ifc, irec)));
                    break;

                case EFIELDIMAG:
                    _data->InsertTuple1(id, std::imag(Efield[nbin](ifc, irec)));
                    break;
            }

            _pos->InsertTuple1(id, Locations(isc, irec));
            ++id;

        }

        vtkFieldData *_fieldData = vtkFieldData::New();
            _fieldData->AllocateArrays(2);
            _fieldData->AddArray(_data);
            _fieldData->AddArray(_pos);

        vtkDataObject *_dataObject = vtkDataObject::New();
            _dataObject->SetFieldData(_fieldData);

        _data->Delete();
        _pos->Delete();
        _fieldData->Delete();

        return _dataObject;
    }

    vtkDataObject* FieldPoints::GetVtkDataObjectFreq(const FIELDTYPE &ftype,
                    const int& nrec,
                    const int& fstart, const int& fend,
                    const FIELDCOMPONENT& fcomp,
                    const VectorXr& Freqs) {

        if (fstart < 0) throw 77;
        //if (fend > NumberOfFrequencies) throw 78;
        if (fstart > fend) throw 79;

        int ifc(-1);
        switch (fcomp) {
            case XCOMPONENT:
                ifc = 0;
                break;
            case YCOMPONENT:
                ifc = 1;
                break;
            case ZCOMPONENT:
                ifc = 2;
                break;
        }

        vtkDoubleArray *_data = vtkDoubleArray::New();
            _data->SetNumberOfComponents(1);

        vtkDoubleArray *_pos = vtkDoubleArray::New();
            _pos->SetNumberOfComponents(1);

        int id=0;
        //std::cout.precision(12);
        for (int ifreq=fstart; ifreq<fend; ++ifreq) {

            switch (ftype) {

                case HFIELDREAL:
                    _data->InsertTuple1(id, std::real(Hfield[ifreq](ifc, nrec)));
                    //std::cout <<  Hfield[ifreq](ifc, nrec) << std::endl;
                    break;

                case HFIELDIMAG:
                    _data->InsertTuple1(id, std::imag(Hfield[ifreq](ifc, nrec)));
                    break;

                case EFIELDREAL:
                    _data->InsertTuple1(id, std::real(Efield[ifreq](ifc, nrec)));
                    break;

                case EFIELDIMAG:
                    _data->InsertTuple1(id, std::imag(Efield[ifreq](ifc, nrec)));
                    break;
            }

            _pos->InsertTuple1(id, Freqs[ifreq]);
            ++id;

        }

        vtkFieldData *_fieldData = vtkFieldData::New();
            _fieldData->AllocateArrays(2);
            _fieldData->AddArray(_data);
            _fieldData->AddArray(_pos);

        vtkDataObject *_dataObject = vtkDataObject::New();
            _dataObject->SetFieldData(_fieldData);

        _data->Delete();
        _pos->Delete();
        _fieldData->Delete();

        return _dataObject;
    }
    #endif
}
