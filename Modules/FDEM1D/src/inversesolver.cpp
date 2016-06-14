/* This file is part of Lemma, a geophysical modelling and inversion API */

/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/**
  @file
  @author   Trevor Irons
  @date     07/14/2010
  @version  $Id: inversesolver.cpp 193 2014-11-10 23:51:41Z tirons $
 **/

#include "inversesolver.h"

namespace Lemma {

    std::ostream &operator<<(std::ostream &stream,
                const InverseSolver &ob) {
        stream << *(LemmaObject*)(&ob);
        return stream;
    }

    // ====================  LIFECYCLE     =======================

    InverseSolver::InverseSolver(const std::string &name) :
        LemmaObject(name), ObservedData(NULL),
        PredictedData(NULL), ModelInstrument(NULL) {
    }

    InverseSolver::~InverseSolver() {

        if (NumberOfReferences != 0) throw DeleteObjectWithReferences(this);

        if (ObservedData != NULL)    ObservedData->DetachFrom(this);

        if (PredictedData != NULL) {
            PredictedData->Delete();
        }

        if (ModelInstrument != NULL)       ModelInstrument->DetachFrom(this);
    }

    // ====================  ACCESS        =======================

    void InverseSolver::SetObservedData(Data* ObsData) {

        if (ObservedData != NULL) {
            ObservedData->DetachFrom(this);
            ObservedData = NULL;
        }

        if (PredictedData != NULL) {
            PredictedData->Delete();
            PredictedData = NULL;
        }

        ObsData->AttachTo(this);
        this->ObservedData = ObsData;

        // Obs and predicted data need to be aligned.
        PredictedData = ObservedData->Clone();
        //PredictedData->AttachTo(this);
    }

    void InverseSolver::SetInstrument(Instrument* inst) {
        if (ModelInstrument != NULL) {
            ModelInstrument->DetachFrom(this);
        }
        inst->AttachTo(this);
        this->ModelInstrument = inst;
    }

    Data* InverseSolver::GetPredictedData() {
        return PredictedData; //->ThrowPointer();
    }

}		// -----  end of Lemma  name  -----
