/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     01/23/2013
  @version  $Id$
 **/

#include "modelreaderfem1d.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const ModelReaderFem1D &ob) {
		stream << *(ModelReaderFem1D*)(&ob);
		return stream;
	}

	ModelReaderFem1D::ModelReaderFem1D(const std::string &name) :
		ModelReader(name) {
	}

	ModelReaderFem1D::~ModelReaderFem1D() {
		if (NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	ModelReaderFem1D* ModelReaderFem1D::New() {
		ModelReaderFem1D* Obj = new ModelReaderFem1D("ModelReaderFem1D");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void ModelReaderFem1D::Delete() {
		this->DetachFrom(this);
	}

	void ModelReaderFem1D::Release() {
		delete this;
	}

} // end of namespace Lemma
