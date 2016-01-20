/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   M. Andy Kass
  @date     02/18/2011
  @version  $Id: modelreader.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "modelreader.h"

namespace Lemma {

	std::ostream &operator<<(std::ostream &stream,
		const ModelReader &ob) {
			stream << *(LemmaObject*)(&ob);
		return stream;
	}

	ModelReader::ModelReader(const std::string &name) :
		LemmaObject(name) {

	}

	ModelReader::~ModelReader() {
		if (this->NumberOfReferences != 0) {
			throw DeleteObjectWithReferences(this);
		}
	}

	ModelReader* ModelReader::New() {
		ModelReader* Obj = new ModelReader("ModelReader");
		Obj->AttachTo(Obj);
		return Obj;
	}

	void ModelReader::Delete() {
		this->DetachFrom(this);
	}

	void ModelReader::Release() {
		delete this;
	}

}
