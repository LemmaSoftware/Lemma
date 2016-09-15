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

#include "receiverpoints.h"

namespace Lemma {

    // ====================    FRIENDS     ======================

#ifdef HAVE_YAMLCPP
    std::ostream &operator << (std::ostream &stream, const ReceiverPoints &ob) {
        stream << ob.Serialize()  << "\n---\n"; // End of doc --- as a direct stream should encapulste thingy
        return stream;
    }
#else
    std::ostream &operator<<(std::ostream &stream,
                const ReceiverPoints &ob) {

        stream << *(LemmaObject*)(&ob);
        stream << "Number of Receivers "<< ob.NumberOfReceivers  << "\n";
        return stream;
    }
#endif

    // ====================  LIFECYCLE     ===================================

    ReceiverPoints::ReceiverPoints(const std::string& name) :
        LemmaObject(name),
        NumberOfReceivers(0), NumberOfBinsE(0), NumberOfBinsH(0) {
    }

#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  ReceiverPoints
    //      Method:  ReceiverPoints
    // Description:  constructor (protected)
    //--------------------------------------------------------------------------------------
    ReceiverPoints::ReceiverPoints (const YAML::Node& node) : LemmaObject(node) {

        //DeSerialize
        NumberOfReceivers = node["NumberOfReceivers"].as<int>();
        NumberOfBinsE = node["NumberOfBinsE"].as<int>();
        NumberOfBinsH = node["NumberOfBinsH"].as<int>();
        Mask = node["Mask"].as<VectorXi>();
        Locations = node["Locations"].as<Vector3Xr>();

    }  // -----  end of method ReceiverPoints::ReceiverPoints  (constructor)  -----
#endif

    ReceiverPoints::~ReceiverPoints() {
        if (this->NumberOfReferences != 0)
            throw DeleteObjectWithReferences( this );
    }

    ReceiverPoints* ReceiverPoints::New() {
        ReceiverPoints* Obj = new ReceiverPoints("ReceiverPoints");
        Obj->AttachTo(Obj);
        return Obj;
    }

    void ReceiverPoints::Delete() {
        this->DetachFrom(this);
    }

    void ReceiverPoints::Release() {
        delete this;
    }

    // ====================  ACCESS        ===================================

    void ReceiverPoints::SetNumberOfReceivers(const int &nrec) {

        if (nrec > 0)
            this->NumberOfReceivers = nrec;
        else
            throw std::runtime_error("NUMBER RECEIVERS LESS THAN 1");

        this->Locations.resize(Eigen::NoChange, nrec);
        Locations.setZero();

        this->Mask.resize(nrec);
        Mask.setZero();

        ResizeEField();
        ResizeHField();
    }

    void ReceiverPoints::ResizeEField() {
        Efield.clear();
        for (int i=0; i<NumberOfBinsE; ++i) {
            Eigen::Matrix<Complex, 3, Eigen::Dynamic> tempe;
            this->Efield.push_back(tempe);
            this->Efield[i].resize(Eigen::NoChange, NumberOfReceivers);
            this->Efield[i].setZero();
        }
    }

    void ReceiverPoints::ResizeHField() {
        Hfield.clear();
        for (int i=0; i<NumberOfBinsH; ++i) {
            Eigen::Matrix<Complex, 3, Eigen::Dynamic> temph;
            this->Hfield.push_back(temph);
            this->Hfield[i].resize(Eigen::NoChange, NumberOfReceivers);
            this->Hfield[i].setZero();
        }
    }

    void ReceiverPoints::SetNumberOfBinsE(const int& nbins) {
        NumberOfBinsE = nbins;
        ResizeEField();
    }

    void ReceiverPoints::SetNumberOfBinsH(const int& nbins) {
        NumberOfBinsH = nbins;
        ResizeHField();
    }

    void ReceiverPoints::SetLocation(const int&nrec,const  Vector3r& loc) {
        this->Locations.col(nrec) = loc;
    }

    void ReceiverPoints::SetLocation(const int&nrec,const  Real& xp,
                    const Real& yp, const Real& zp) {
        this->Locations.col(nrec) << xp, yp, zp;
    }

    void ReceiverPoints::SetEfield(const int& nbin,
                    const int& loc, const Complex &ex,
                    const Complex &ey, const Complex &ez) {
        this->Efield[nbin].col(loc) << ex, ey, ez;
    }

    void ReceiverPoints::AppendEfield(const int&nbin, const int& loc,
                    const Complex &ex,
                    const Complex &ey, const Complex &ez) {
        #ifdef LEMMAUSEOMP
        #pragma omp critical
        #endif
        this->Efield[nbin].col(loc) += Vector3cr(ex, ey, ez); //temp;
    }

    void ReceiverPoints::SetHfield(const int &nbin, const int& loc,
                    const Complex &hx, const Complex &hy,
                    const Complex &hz) {
        this->Hfield[nbin].col(loc) << hx, hy, hz;
    }

    void ReceiverPoints::AppendHfield(const int &nbin, const int& loc,
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
    Vector3Xr ReceiverPoints::GetLocations() {
        return this->Locations;
    }

    MatrixXr ReceiverPoints::GetLocationsMat() {
        return MatrixXr(this->Locations);
    }

    Vector3r ReceiverPoints::GetLocation(const int &nrec) {
        return this->Locations.col(nrec);
    }

    Real ReceiverPoints::GetLocationX(const int &nrec) {
        return this->Locations.col(nrec)[0];
    }

    Real ReceiverPoints::GetLocationY(const int &nrec) {
        return this->Locations.col(nrec)[1];
    }

    Real ReceiverPoints::GetLocationZ(const int &nrec) {
        return this->Locations.col(nrec)[2];
    }

    Vector3cr ReceiverPoints::GetEfield(const int &nfreq, const int&nrec) {
        return this->Efield[nfreq].col(nrec);
    }

    Vector3cr ReceiverPoints::GetHfield(const int &nfreq, const int&nrec) {
        return this->Hfield[nfreq].col(nrec);
    }

    std::vector<Vector3Xcr> ReceiverPoints::GetHfield( ) {
        return this->Hfield;
    }

    std::vector<Vector3Xcr> ReceiverPoints::GetEfield( ) {
        return this->Efield;
    }

    Vector3Xcr ReceiverPoints::GetEfield (const int &nfreq) {
        return this->Efield[nfreq];
    }

    MatrixXcr ReceiverPoints::GetEfieldMat (const int &nfreq) {
        return MatrixXcr(this->Efield[nfreq]);
    }

    MatrixXcr ReceiverPoints::GetHfieldMat (const int &nfreq) {
        return MatrixXcr(this->Hfield[nfreq]);
    }

    Vector3Xcr ReceiverPoints::GetHfield (const int &nfreq) {
        return this->Hfield[nfreq];
    }

    void ReceiverPoints::MaskPoint(const int& imask) {
        Mask(imask) = true;
    }

    void ReceiverPoints::UnMaskPoint(const int& imask) {
        Mask(imask) = false;
    }

    void ReceiverPoints::UnMaskAllPoints() {
        Mask.setZero();
    }

    int ReceiverPoints::GetMask(const int& i) {
        return Mask(i);
    }

    int ReceiverPoints::GetNumberOfReceivers() {
        return this->NumberOfReceivers;
    }

    void ReceiverPoints::ClearFields() {
        for (int i=0; i<NumberOfBinsE; ++i) {
            this->Efield[i].setZero();
        }
        for (int i=0; i<NumberOfBinsH; ++i) {
            this->Hfield[i].setZero();
        }
    }


#ifdef HAVE_YAMLCPP
    //--------------------------------------------------------------------------------------
    //       Class:  ReceiverPoints
    //      Method:  Serialize
    //--------------------------------------------------------------------------------------
    YAML::Node  ReceiverPoints::Serialize (  ) const {
        YAML::Node node = LemmaObject::Serialize(); //static_cast<const LemmaObject*>(this)->Serialize();
        node.SetTag( this->Name ); // Set Tag after doing parents

        // update here
        node["NumberOfReceivers"] = NumberOfReceivers;
        node["NumberOfBinsE"] = NumberOfBinsE;
        node["NumberOfBinsH"] = NumberOfBinsH;
        //node["Mask"] = Mask;
        //std::cout << "Locations.data" << Locations.data()[0] << std::endl;
        // node["Locations"] = Locations.data(); // HUGE
        return node;
    }		// -----  end of method ReceiverPoints::Serialize  -----


    //--------------------------------------------------------------------------------------
    //       Class:  ReceiverPoints
    //      Method:  DeSerialize
    //--------------------------------------------------------------------------------------
    ReceiverPoints* ReceiverPoints::DeSerialize ( const YAML::Node& node  ) {
        ReceiverPoints* Object = new ReceiverPoints(node);
        Object->AttachTo(Object);
        DESERIALIZECHECK( node, Object )
        return Object ;
    }		// -----  end of method ReceiverPoints::DeSerialize  -----
#endif


    #ifdef LEMMAUSEVTK
    vtkActor* ReceiverPoints::GetVtkGlyphActor(const FIELDTYPE &ftype,
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
        for (int ic=0; ic<NumberOfReceivers; ++ic) {

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

    vtkDataObject* ReceiverPoints::GetVtkDataObject(const FIELDTYPE &ftype,
                    const int& nbin,
                    const int& start, const int& end,
                    const FIELDCOMPONENT& fcomp,
                    const SPATIALCOORDINANT &scord) {

        if (start < 0) throw 77;
        if (end > NumberOfReceivers) throw 78;
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

    vtkDataObject* ReceiverPoints::GetVtkDataObjectFreq(const FIELDTYPE &ftype,
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
