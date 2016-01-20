/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/19/2011
  @version  $Id: modelreadertem1d.cpp 87 2013-09-05 22:44:05Z tirons $
 **/

#include "modelreadertem1d.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const ModelReaderTem1D &ob) {
		stream << *(ModelReaderTem1D*)(&ob);
		return stream;
	}

	ModelReaderTem1D::ModelReaderTem1D(const std::string &name) :
		ModelReader(name) {
	}

	ModelReaderTem1D::~ModelReaderTem1D() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	ModelReaderTem1D* ModelReaderTem1D::New() {
		ModelReaderTem1D* Obj = new ModelReaderTem1D("ModelReaderTem1D");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void ModelReaderTem1D::Delete() {
		this->DetachFrom(this);
	}

	void ModelReaderTem1D::Release() {
		delete this;
	}

	//void ModelReaderTem1D::ReadParameters(/* Pass file name */) {

	//}

	//void ModelReaderTem1D::ReadModel() {

	//}

	//void ModelReaderTem1D::ReadInstrument() {

	//}

	//void ModelReaderTem1D::ReadWaveform() {

	//}

	//Datamembers:
	// modelfile, instrumentfile, waveformfile

} // namespace Lemma
