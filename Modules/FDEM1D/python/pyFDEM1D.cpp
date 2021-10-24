/* This file is part of Lemma, a geophysical modelling and inversion API.
 * More information is available at http://lemmasoftware.org
 */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/**
 * @file
 * @date      22/04/19 14:06:32
 * @version   $Id$
 * @author    Trevor Irons (ti)
 * @email     Trevor.Irons@utah.edu
 * @copyright Copyright (c) 2019, University of Utah
 * @copyright Copyright (c) 2019, Lemma Software, LLC
 */

#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/eigen.h>
#include "FDEM1D"

namespace py = pybind11;

PYBIND11_MODULE(FDEM1D, m) {

    py::add_ostream_redirect(m, "ostream_redirect");
    m.doc() = "Python binding of Lemma::FDEM1D, additional details can be found at https://lemmasoftware.org";


    py::class_<Lemma::WireAntenna, std::shared_ptr<Lemma::WireAntenna> > WireAntenna(m, "WireAntenna");

        // lifecycle
        WireAntenna.def(py::init(&Lemma::WireAntenna::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::WireAntenna::DeSerialize),
            "Construct object from yaml representation")

        // print
        .def("Serialize", &Lemma::WireAntenna::Print, "YAML representation of the class")
        .def("__repr__", &Lemma::WireAntenna::Print)

        // modifiers
        .def("SetNumberOfPoints", &Lemma::WireAntenna::SetNumberOfPoints,
            "Sets the number of points comprising the antenna")
        .def("SetPoint", py::overload_cast<const int&, const Lemma::Real&, const Lemma::Real&, const Lemma::Real&>(&Lemma::WireAntenna::SetPoint),
            "Sets a point in the antenna")
        .def("SetPoint", py::overload_cast<const int&, const Lemma::Vector3r&>(&Lemma::WireAntenna::SetPoint),
            "Sets a point in the antenna")
        .def("SetNumberOfTurns", &Lemma::WireAntenna::SetNumberOfTurns, "Sets the number of turns of the antenna")
        .def("SetNumberOfFrequencies", &Lemma::WireAntenna::SetNumberOfFrequencies,
            "Sets the number of frequencies of the transmitter")
        .def("SetFrequency", &Lemma::WireAntenna::SetFrequency, "Sets a single frequency of the transmitter")
        .def("SetCurrent", &Lemma::WireAntenna::SetCurrent, "Sets the current of the transmitter in amps")

        // accessors
        .def("GetCurrent", &Lemma::WireAntenna::GetCurrent, "Returns the current of the transmitter in amps")
        .def("GetPoints", &Lemma::WireAntenna::GetPoints, "Returns the points defining the transmitter")
        .def("GetNumberOfDipoles", &Lemma::WireAntenna::GetNumberOfDipoles,
            "Returns the number of dipoles defining the transmitter")
        .def("GetNumberOfFrequencies", &Lemma::WireAntenna::GetNumberOfFrequencies,
            "Returns the number of frequencies for the transmitter")
        .def("IsHorizontallyPlanar", &Lemma::WireAntenna::IsHorizontallyPlanar, "Returns true if the transmitter is flat")
        .def("GetName", &Lemma::WireAntenna::GetName, "Returns the class name of the object")

        // operations
        .def("ApproximateWithElectricDipoles", &Lemma::WireAntenna::ApproximateWithElectricDipoles,
            "Approximates loop with electric dipoles")

    ;

    py::class_<Lemma::PolygonalWireAntenna, std::shared_ptr<Lemma::PolygonalWireAntenna> > PolygonalWireAntenna(m,
            "PolygonalWireAntenna", WireAntenna);

        // lifecycle
        PolygonalWireAntenna.def(py::init(&Lemma::PolygonalWireAntenna::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::PolygonalWireAntenna::DeSerialize),
            "Construct object from yaml representation")

        // print
        .def("__repr__", &Lemma::PolygonalWireAntenna::Print)
        .def("Serialize", &Lemma::PolygonalWireAntenna::Print, "YAML representation of the class")

        // accessors
        .def("GetName", &Lemma::PolygonalWireAntenna::GetName, "Returns the name of the class")

        // operations
        .def("ApproximateWithElectricDipoles", &Lemma::PolygonalWireAntenna::ApproximateWithElectricDipoles,
            "Approximates loop with series of electric dipoles around loop")

        // modifiers
        .def("SetMinDipoleRatio", &Lemma::PolygonalWireAntenna::SetMinDipoleRatio,
            "Sets the minimum dipole ratio use, smaller values increase precision")
        .def("SetMinDipoleMoment", &Lemma::PolygonalWireAntenna::SetMinDipoleMoment,
            "Sets the minimum dipole moment which will be used, smaller values increase precision and computational time")
        .def("SetMaxDipoleMoment", &Lemma::PolygonalWireAntenna::SetMaxDipoleMoment,
            "Sets the maximum dipole moment which will be used, smaller values increase precision and computational time")
    ;

    py::class_<Lemma::DipoleSource, std::shared_ptr<Lemma::DipoleSource> > DipoleSource(m, "DipoleSource");

        // lifecycle
        DipoleSource.def(py::init(&Lemma::DipoleSource::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>(&Lemma::DipoleSource::DeSerialize),
            "Construct object from yaml representation")

        // print
        .def("Serialize", &Lemma::DipoleSource::Print, "YAML representation of the class")
        .def("__repr__", &Lemma::DipoleSource::Print)

        // accessors
        .def("GetName", &Lemma::DipoleSource::GetName, "Returns the name of the class")
        .def("GetNumberOfFrequencies", &Lemma::DipoleSource::GetNumberOfFrequencies,
                "Returns the number of frequencies")
        .def("GetFrequencies", &Lemma::DipoleSource::GetFrequencies, "Returns an array of frequencies")
        .def("GetFrequency", &Lemma::DipoleSource::GetFrequency, "Returns the frequency of the argument index")
        .def("GetAngularFrequency", &Lemma::DipoleSource::GetAngularFrequency,
            "Returns the angular frequency of the argument index")
        .def("GetPhase", &Lemma::DipoleSource::GetPhase, "Returns the phase of the dipole")
        .def("GetMoment", &Lemma::DipoleSource::GetMoment, "Returns the moment of the dipole")
        .def("GetLocation", py::overload_cast< >(&Lemma::DipoleSource::GetLocation), "Returns the location of the dipole")
        .def("GetPolarisation", &Lemma::DipoleSource::GetPolarisation, "Returns the polarisation of the dipole")

        // modifiers
        .def("SetLocation", py::overload_cast<const Lemma::Vector3r&> (&Lemma::DipoleSource::SetLocation),
            "Sets the location of the dipole")
        .def("SetPolarisation", py::overload_cast<const Lemma::Vector3r&> (&Lemma::DipoleSource::SetPolarisation),
            "Sets the polarisation of the dipole")
        .def("SetType", &Lemma::DipoleSource::SetType, "Sets the type")
        .def("SetMoment", &Lemma::DipoleSource::SetMoment, "Sets the moment of the dipole")
        .def("SetPhase", &Lemma::DipoleSource::SetPhase, "Sets the phase of the dipole")
        .def("SetNumberOfFrequencies", &Lemma::DipoleSource::SetNumberOfFrequencies,
            "Sets the number of frequencies to calculate for the dipole")
        .def("SetFrequency", &Lemma::DipoleSource::SetFrequency,
            "Sets a single frequency, first argument is index, second argument is frequency")
        .def("SetFrequencies", &Lemma::DipoleSource::SetFrequencies,
            "Sets all frequencies, argument is numpy array of frequencies")
        ;

    py::class_<Lemma::LayeredEarthEM, std::shared_ptr<Lemma::LayeredEarthEM>, Lemma::EarthModel > LayeredEarthEM(m, "LayeredEarthEM");

        // lifecycle
        LayeredEarthEM.def(py::init(&Lemma::LayeredEarthEM::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>
            (&Lemma::LayeredEarthEM::DeSerialize),"Construct object from yaml representation")

        // print
        .def("Serialize", &Lemma::LayeredEarthEM::Print, "YAML representation of the class")
        .def("__repr__", &Lemma::LayeredEarthEM::Print)

        // accessors
        .def("GetName", &Lemma::LayeredEarthEM::GetName, "Returns the name of the class")

        .def("GetLayerConductivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerConductivity),
            "Returns the conductivity of all layers")
        .def("GetLayerConductivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerConductivity),
            "Returns the conductivity of the specified layer")

        .def("GetLayerSusceptibility", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerSusceptibility),
            "Returns the susceptibility of all layers")
        .def("GetLayerSusceptibility1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerSusceptibility),
            "Returns the susceptibilty of the specified layer")
        .def("GetLayerLowFreqSusceptibility", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerLowFreqSusceptibility),
            "Returns the low frequqncy permitivity of all layers")
        .def("GetLayerLowFreqSusceptibility1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerLowFreqSusceptibility),
            "Returns the low frequency permitivity of the specified layer")
        .def("GetLayerHighFreqSusceptibility", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerHighFreqSusceptibility),
            "Returns the low frequency permitivity of all layers")
        .def("GetLayerHighFreqSusceptibility1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerHighFreqSusceptibility),
            "Returns the low frequency permitivity of the specified layer")
        .def("GetLayerTauSusceptibility", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerTauSusceptibility),
            "Returns the tau permitivity of all layers")
        .def("GetLayerTauSusceptibility1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerTauSusceptibility),
            "Returns the tau permitivity of the specified layer")
        .def("GetLayerBreathSusceptibility", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerBreathSusceptibility),
            "Returns the breth permitivity of all layers")
        .def("GetLayerBreathSusceptibility1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerBreathSusceptibility),
            "Returns the breath permitivity of the specified layer")

        .def("GetLayerPermitivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerPermitivity),
            "Returns the permitivity of all layers")
        .def("GetLayerPermitivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerPermitivity),
            "Returns the permitivity of the specified layer")
        .def("GetLayerLowFreqPermitivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerLowFreqPermitivity),
            "Returns the low frequqncy permitivity of all layers")
        .def("GetLayerLowFreqPermitivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerLowFreqPermitivity),
            "Returns the low frequency permitivity of the specified layer")
        .def("GetLayerHighFreqPermitivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerHighFreqPermitivity),
            "Returns the low frequency permitivity of all layers")
        .def("GetLayerHighFreqPermitivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerHighFreqPermitivity),
            "Returns the low frequency permitivity of the specified layer")
        .def("GetLayerTauPermitivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerTauPermitivity),
            "Returns the tau permitivity of all layers")
        .def("GetLayerTauPermitivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerTauPermitivity),
            "Returns the tau permitivity of the specified layer")
        .def("GetLayerBreathPermitivity", py::overload_cast<>(&Lemma::LayeredEarthEM::GetLayerBreathPermitivity),
            "Returns the breth permitivity of all layers")
        .def("GetLayerBreathPermitivity1", py::overload_cast<const int&>(&Lemma::LayeredEarthEM::GetLayerBreathPermitivity),
            "Returns the breath permitivity of the specified layer")


        // modifiers
        .def("SetNumberOfLayers", &Lemma::LayeredEarthEM::SetNumberOfLayers,
            "Sets the number of layers in the model")
        .def("SetLayerConductivity", py::overload_cast< const Lemma::VectorXcr& >(&Lemma::LayeredEarthEM::SetLayerConductivity),
            "Sets the conductivity of the layers, the input is a complex array of conductivity")
        .def("SetLayerConductivity1", py::overload_cast< const int&, const Lemma::Complex& >(&Lemma::LayeredEarthEM::SetLayerConductivity),
            "Sets the conductivity of a single layer, the first input is the layer index, and the secondinput is a complex conductivity")
        .def("SetLayerThickness", &Lemma::LayeredEarthEM::SetLayerThickness,
            "Sets the thickness of layers, excluding the air and bottom which are infinite")

        .def("SetLayerHighFreqSusceptibility", &Lemma::LayeredEarthEM::SetLayerHighFreqSusceptibility,
            "Sets the high frequency susceptibility for Cole-COle model")
        .def("SetLayerLowFreqSusceptibility", &Lemma::LayeredEarthEM::SetLayerLowFreqSusceptibility,
            "Sets the low frequency susceptibility for Cole-COle model")
        .def("SetLayerBreathSusceptibility", &Lemma::LayeredEarthEM::SetLayerBreathSusceptibility,
            "Sets thesusceptibility breath for Cole-COle model")

        .def("SetLayerHighFreqPermitivity", &Lemma::LayeredEarthEM::SetLayerHighFreqPermitivity,
            "Sets the high frequency permitivity for Cole-COle model")
        .def("SetLayerLowFreqPermitivity", &Lemma::LayeredEarthEM::SetLayerLowFreqPermitivity,
            "Sets the low frequency permitivity for Cole-COle model")
        .def("SetLayerBreathPermitivity", &Lemma::LayeredEarthEM::SetLayerBreathPermitivity,
            "Sets the permitivity breath for Cole-COle model")

        // methods
        .def("EvaluateColeColeModel", &Lemma::LayeredEarthEM::EvaluateColeColeModel,
            "Calculates complex resistivity based on cole-cole parameters")
        ;


    py::class_<Lemma::EMEarth1D, std::shared_ptr<Lemma::EMEarth1D> >
        EMEarth1D(m, "EMEarth1D");

        // lifecycle
        EMEarth1D.def(py::init(&Lemma::EMEarth1D::NewSP))
/*
        //.def_static("DeSerialize", py::overload_cast<const std::string&>
        //    (&Lemma::EMEarth1D::DeSerialize),"Construct object from yaml representation")

        // print
        .def("Serialize", &Lemma::EMEarth1D::Print, "YAML representation of the class")
        .def("__repr__", &Lemma::EMEarth1D::Print)
	
        // accessors
        .def("GetName", &Lemma::EMEarth1D::GetName, "Returns the name of the class")
        .def("GetFieldPoints", &Lemma::EMEarth1D::GetFieldPoints, "Returns the FieldPoint class")

        // modifiers
        .def("AttachWireAntenna", &Lemma::EMEarth1D::AttachWireAntenna,
            "Sets the wire antenna to use for calculations")
        .def("AttachDipoleSource", &Lemma::EMEarth1D::AttachDipoleSource,
            "Sets a DipoleSource to use for calculations")
        .def("AttachFieldPoints", &Lemma::EMEarth1D::AttachFieldPoints,
            "Sets the FieldPoints to use for calculations")
        .def("AttachLayeredEarthEM", &Lemma::EMEarth1D::AttachLayeredEarthEM,
            "Sets the LayeredEarthEM to use for calculations")

        .def("SetFieldsToCalculate", &Lemma::EMEarth1D::SetFieldsToCalculate,
            "Sets which fields to calculate")
        .def("SetHankelTransformMethod", &Lemma::EMEarth1D::SetHankelTransformMethod,
            "Sets which Hankel transform to use")
        .def("SetTxRxMode", &Lemma::EMEarth1D::SetTxRxMode,
            "Sets the TxRx mode flag")

        //methods
	#ifdef KIHALEE_EM1D
        .def("MakeCalc", &Lemma::EMEarth1D::MakeCalc, "Calls KiHa Lee's EM1D FORTRAN77 code")
	#endif

        .def("MakeCalc3", &Lemma::EMEarth1D::MakeCalc3, "Native Lemma EM calculation")
        .def("CalculateWireAntennaFields", &Lemma::EMEarth1D::CalculateWireAntennaFields,
            "Native Lemma calculation of a wire antenna")
*/
	
        ;


    py::class_<Lemma::FieldPoints, std::shared_ptr<Lemma::FieldPoints> >
        FieldPoints(m, "FieldPoints");

        // lifecycle
        FieldPoints.def(py::init(&Lemma::FieldPoints::NewSP))
        .def_static("DeSerialize", py::overload_cast<const std::string&>
            (&Lemma::FieldPoints::DeSerialize),"Construct object from yaml representation")

        // print
        .def("Serialize", &Lemma::FieldPoints::Print, "YAML representation of the class")
        .def("__repr__", &Lemma::FieldPoints::Print)

        // modifiers
        .def("SetNumberOfPoints", &Lemma::FieldPoints::SetNumberOfPoints,
            "Sets the number of locations to make calculations on.")
        .def("SetLocation", py::overload_cast< const int&, const Lemma::Vector3r& >
            (&Lemma::FieldPoints::SetLocation), "Sets the location of the index-specified point." )
        .def("SetLocation", py::overload_cast< const int&,
                    const Lemma::Real&, const Lemma::Real&, const Lemma::Real& >
            (&Lemma::FieldPoints::SetLocation),
            "Sets the location of the index-specified point with the three coordinates")

        // accessors
        .def("GetNumberOfPoints", &Lemma::FieldPoints::GetNumberOfPoints,
            "Returns the number of locations to make calculations on.")
        .def("GetLocations", &Lemma::FieldPoints::GetLocations,
            "Returns the locations which calculations are made on.")
        .def("GetLocationsMat", &Lemma::FieldPoints::GetLocationsMat,
            "Returns a matrix of the locations which calculations are made on.")
        .def("GetLocation", &Lemma::FieldPoints::GetLocation,
            "Returns the location of the specified index.")
        .def("GetLocationX", &Lemma::FieldPoints::GetLocationX,
            "Returns the northing (x) location of the specified index.")
        .def("GetLocationY", &Lemma::FieldPoints::GetLocationY,
            "Returns the easting (y) location of the specified index.")
        .def("GetLocationZ", &Lemma::FieldPoints::GetLocationZ,
            "Returns the altitude/depth (z) location of the specified index.")
        .def("GetEfield", py::overload_cast<  > (&Lemma::FieldPoints::GetEfield),
            "Returns the electric field for all frequencies.")
        .def("GetEfield", py::overload_cast< const int& > (&Lemma::FieldPoints::GetEfield),
            "Returns the electric field for the specified frequency index.")
        .def("GetEfield", py::overload_cast< const int&, const int& > (&Lemma::FieldPoints::GetEfield),
            "Returns the electric field for the specified frequency and location index.")
        .def("GetEfieldMat", &Lemma::FieldPoints::GetEfieldMat,
            "Returns the electric field for the specified frequency.")
        .def("GetHfield", py::overload_cast<  > (&Lemma::FieldPoints::GetHfield),
            "Returns the H field for all frequencies.")
        .def("GetHfield", py::overload_cast< const int& > (&Lemma::FieldPoints::GetHfield),
            "Returns the H field for the specified frequency index.")
        .def("GetHfield", py::overload_cast< const int&, const int& > (&Lemma::FieldPoints::GetHfield),
            "Returns the H field for the specified frequency and location index.")
        //.def("GetBfield", py::overload_cast< const int&, const int& > (&Lemma::FieldPoints::GetBfield),
        //    "Returns the magnetic (B) field for the specified frequency and location index.")
        .def("GetHfieldMat", &Lemma::FieldPoints::GetHfieldMat,
            "Returns the H field for the specified frequency.")
        .def("GetMask", &Lemma::FieldPoints::MaskPoint, "Return the mask boolean value for the specified index")

        // methods
        .def("ClearFields", &Lemma::FieldPoints::ClearFields, "Clears calculated fields")
        .def("MaskPoint", &Lemma::FieldPoints::MaskPoint, "Masks the index resulting in no calculation")
        .def("UnMaskPoint", &Lemma::FieldPoints::UnMaskPoint, "Unmasks the index resulting in a calculation")
	
        ;
}
