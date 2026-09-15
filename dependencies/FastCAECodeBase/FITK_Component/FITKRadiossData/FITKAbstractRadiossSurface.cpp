/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractRadiossSurface.h"
#include "FITKRadiossMeshModel.h"
#include "FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"
#include <QSet>

namespace Radioss
{
    FITKAbstractRadiossSurface::~FITKAbstractRadiossSurface()
    {
    }

    FITKAbstractRadiossSurface::RadiossSurfaceType 
        FITKAbstractRadiossSurface::getRadiossSurfaceType()
    {
        return FITKAbstractRadiossSurface::RadiossSurfaceType::RST_None;
    }

    void FITKAbstractRadiossSurface::addElementSurface(int element, int surfaceIndex)
    {
        _elementSurface.insert(element,surfaceIndex);
    }

    QList<int> FITKAbstractRadiossSurface::getSurfaceNodeIDs()
    {
        QList<int> nodeIDs;
        FITKRadiossMeshModel* model = this->getRadiossMeshModel();
        if (!model) return nodeIDs;
        FITKRadiossPartManager* partsManager = model->getPartsManager();
        if (!partsManager) return nodeIDs;
        //遍历qmultihash
        for (auto it = _elementSurface.constBegin(); it != _elementSurface.constEnd(); ++it)
        {
            const int elementID = it.key();
            const int surfaceIndex = it.value();

            Interface::FITKAbstractElement* element = partsManager->getElementByID(elementID).second;
            if (!element) continue;
            nodeIDs.append(element->getFace(surfaceIndex));
        }
        //qlist去重
        QSet<int> uniqueNodeIDs = QSet<int>::fromList(nodeIDs);
        nodeIDs = uniqueNodeIDs.values();
        //排序
        qSort(nodeIDs);
        return nodeIDs;
    }

    QList<int> FITKAbstractRadiossSurface::getElementSurfaceIDs(const int &elementID) const
    {
        return _elementSurface.values(elementID);
    }

    QHash<int, QList<int>> FITKAbstractRadiossSurface::getRadiossSurfaceInfo() const
    {
        QHash<int, QList<int>> surfaceInfo;
        for (auto it = _elementSurface.constBegin(); it != _elementSurface.constEnd(); ++it)
        {
            const int elementID = it.key();
            const int surfaceIndex = it.value();
            surfaceInfo[surfaceIndex].append(elementID);
        }
        return surfaceInfo;
    }

    FITKRadiossMeshModel *FITKAbstractRadiossSurface::getRadiossMeshModel()
    {
        return dynamic_cast<FITKRadiossMeshModel*>(this->getAbstractModel());
    }

} // namespace Radioss


