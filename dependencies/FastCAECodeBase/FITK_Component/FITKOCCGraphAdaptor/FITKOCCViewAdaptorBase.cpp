/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCViewAdaptorBase.h"

// Graph and data
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKOCCGraphObject3D.h"

namespace Exchange
{
    void FITKOCCViewAdaptorBase::setDataObject(Core::FITKAbstractDataObject* data)
    {
        if (data && data != _dataObj)
        {
            m_needUpdate = true;
        }

        Adaptor::FITKAbstractViewAdaptor::setDataObject(data);
    }

    void FITKOCCViewAdaptorBase::setDetails(QVariant details)
    {
        // Set the details information for exchanging data.
        m_details = details;
    }

    Exchange::FITKOCCGraphObject3D* FITKOCCViewAdaptorBase::getOutputData()
    {
        // The data exchanged to the output.
        return m_outputData;
    }

    bool FITKOCCViewAdaptorBase::update()
    {
        // Override in subclass.
        return true;
    }
}   // namespace Exchange
