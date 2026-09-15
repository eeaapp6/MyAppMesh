/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaAbstractAdaptor.h"
#include "FITKRadiossLSDynaReader.h"
#include "FITKRadiossLSDynaWriter.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"

namespace Radioss
{
    FITKRadiossLSDynaAbstractAdaptor::~FITKRadiossLSDynaAbstractAdaptor() = default;

    void FITKRadiossLSDynaAbstractAdaptor::setFileReader(FITKRadiossLSDynaReader* reader)
    {
        _reader = reader;
    }

    void FITKRadiossLSDynaAbstractAdaptor::setFileWriter(FITKRadiossLSDynaWriter* writer)
    {
        _writer = writer;
    }

    void FITKRadiossLSDynaAbstractAdaptor::setCaseData(FITKRadiossCase* casedata)
    {
        _caseData = casedata;
    }

    QHash<QString, std::function<FITKRadiossLSDynaAbstractAdaptor*()>>& FITKRadiossLSDynaAbstractAdaptor::adaptorCreators()
    {
        static QHash<QString, std::function<FITKRadiossLSDynaAbstractAdaptor*()>> creators;
        return creators;
    }

    void FITKRadiossLSDynaAbstractAdaptor::registerAdaptor(const QString& keywords,
        std::function<FITKRadiossLSDynaAbstractAdaptor*()> creator)
    {
        adaptorCreators()[keywords] = creator;
    }

    FITKRadiossLSDynaAbstractAdaptor* FITKRadiossLSDynaAbstractAdaptor::createAdaptor(const QString& keywords)
    {
        auto it = adaptorCreators().find(keywords);
        if (it != adaptorCreators().end())
        {
            return it.value()();
        }
        return nullptr;
    }
}
