/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKInterfaceHDF5AdaptorSurface.h"

#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"

#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Reader.h"
#include "FITK_Interface/FITKInterfaceIO/FITKAbstractHDF5Writer.h"

#include "FITK_Interface/FITKInterfaceModel/FITKModelSet.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurface.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceNode.h"
#include "FITK_Interface/FITKInterfaceModel/FITKMeshSurfaceElement.h"
#include "FITK_Interface/FITKInterfaceModel/FITKGeometrySurface.h"

#include "FITK_Interface/FITKInterfaceModel/FITKAssembly.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace IO
{
    QString FITKInterfaceHDF5AdaptorSurface::getAdaptorClass()
    {
        return "FITKInterfaceHDF5AdaptorSurface";
    }

    bool FITKInterfaceHDF5AdaptorSurface::adaptR()
    {
        if (!_reader || !_h5Group) return false;
        bool isR = true;
        if (dynamic_cast<Interface::FITKMeshSurface*>(_dataObj) != nullptr)
        {
            Interface::FITKMeshSurface* surfaceData = dynamic_cast<Interface::FITKMeshSurface*>(_dataObj);
            isR &= surfR(surfaceData, *_h5Group);
            return isR;
        }
        else if (dynamic_cast<Interface::FITKGeometrySurface*>(_dataObj) != nullptr)
        {
            Interface::FITKGeometrySurface* surfaceData = dynamic_cast<Interface::FITKGeometrySurface*>(_dataObj);
            isR &= geoSurfR(surfaceData, *_h5Group);
            return isR;
        }
        else return false;
    }

    bool FITKInterfaceHDF5AdaptorSurface::adaptW()
    {
        if (!_writer || !_h5Group) return false;
        bool isW = true;
        if (dynamic_cast<Interface::FITKMeshSurface*>(_dataObj) != nullptr)
        {
            Interface::FITKMeshSurface* surfaceData = dynamic_cast<Interface::FITKMeshSurface*>(_dataObj);
            isW &= surfW(surfaceData, *_h5Group);
            return isW;
        }
        else if (dynamic_cast<Interface::FITKGeometrySurface*>(_dataObj) != nullptr)
        {
            Interface::FITKGeometrySurface* surfaceData = dynamic_cast<Interface::FITKGeometrySurface*>(_dataObj);
            isW &= geoSurfW(surfaceData, *_h5Group);
            return isW;
        }
        else return false;
    }

    bool FITKInterfaceHDF5AdaptorSurface::surfR(Interface::FITKMeshSurface * surfaceData, H5::Group & h5Group)
    {
        if (surfaceData == nullptr) return false;
        bool isR = readNDataObject(surfaceData, h5Group);

        bool isInternal = readBoolAttribute(h5Group, "isInternal");
        surfaceData->isInternal(isInternal);
        int modelID = readIntAttribute(h5Group, "ModelID");
        surfaceData->setModel(modelID);
        if (!h5Group.nameExists("SetIDList")) return false;
        if (!h5Group.nameExists("SetModelID")) return false;

        //写出类型
        auto surfType = surfaceData->getMeshSurfaceType();

        if (surfType == Interface::FITKMeshSurface::MeshSurfaceType::SurNode)
        {
            isR &= nodeTypeR(surfaceData, h5Group);
        }
        else if (surfType == Interface::FITKMeshSurface::MeshSurfaceType::SurEle)
        {
            isR &= elementTypeR(surfaceData, h5Group);
        }
        else
            return false;

        return isR;
    }

    bool FITKInterfaceHDF5AdaptorSurface::geoSurfR(Interface::FITKGeometrySurface* surfaceData, H5::Group & h5Group)
    {
        if (surfaceData == nullptr) return false;
        bool isR = readNDataObject(surfaceData, h5Group);

        bool isInternal = readBoolAttribute(h5Group, "isInternal");
        surfaceData->isInternal(isInternal);
        int modelID = readIntAttribute(h5Group, "ModelID");
        surfaceData->setModel(modelID);

        if (!h5Group.nameExists("SetIDList")) return false;
        if (!h5Group.nameExists("SetModelID")) return false;
        if (!h5Group.nameExists("SurfaceIndex")) return false;

        QList<int> setIdList;
        QList<int> SetModelIDList;
        QList<int> surfaceIndex;
        readIntDataSet(h5Group, "SetIDList", setIdList);
        readIntDataSet(h5Group, "SetModelID", SetModelIDList);
        readIntDataSet(h5Group, "SurfaceIndex", surfaceIndex);
        int count = surfaceIndex.size();
        for (int i = 0; i < count; i++)
        {
            surfaceData->addGeometrySet(SetModelIDList[i], setIdList[i], surfaceIndex[i]);
        }
        return true;
    }

    bool FITKInterfaceHDF5AdaptorSurface::elementTypeR(Interface::FITKMeshSurface* surfaceData, H5::Group& h5Group)
    {
        Interface::FITKMeshSurfaceElement* abaSurfaceData = dynamic_cast<Interface::FITKMeshSurfaceElement*>(surfaceData);
        if (!abaSurfaceData) return false;
        if (!h5Group.nameExists("SurfaceIndex")) return false;

        QList<int> setIdList;
        QList<int> SetModelIDList;
        QList<int> surfaceIndex;
        readIntDataSet(h5Group, "SetIDList", setIdList);
        readIntDataSet(h5Group, "SetModelID", SetModelIDList);
        readIntDataSet(h5Group, "SurfaceIndex", surfaceIndex);

        for (int i = 0; i < surfaceIndex.size(); i++)
        {
            abaSurfaceData->addMeshSet(SetModelIDList[i], setIdList[i], surfaceIndex[i]);
        }
        return true;
    }

    bool FITKInterfaceHDF5AdaptorSurface::nodeTypeR(Interface::FITKMeshSurface* surfaceData, H5::Group& h5Group)
    {
        Interface::FITKMeshSurfaceNode* abaSurfaceData = dynamic_cast<Interface::FITKMeshSurfaceNode*>(surfaceData);
        if (!abaSurfaceData) return false;
        if (!h5Group.nameExists("Weights"))return false;

        QList<int> setIdList;
        QList<int> SetModelIDList;
        QList<double> Weights;
        readIntDataSet(h5Group, "SetIDList", setIdList);
        readIntDataSet(h5Group, "SetModelID", SetModelIDList);
        readDoubleDataSet(h5Group, "Weights", Weights);

        for (int i = 0; i < setIdList.size(); i++)
        {
            abaSurfaceData->addMeshSet(SetModelIDList[i], setIdList[i], Weights[i]);
        }
        return true;
    }

    bool FITKInterfaceHDF5AdaptorSurface::surfW(Interface::FITKMeshSurface* surfaceData, H5::Group& h5Group)
    {
        if (surfaceData == nullptr) return false;
        bool isW = true;

        std::string groupName = createParentAttribute(surfaceData, h5Group);
        if (groupName.empty())return false;

        //创建表面的数据块
        H5::Group surfGroup = h5Group.createGroup(groupName);
        isW &= writeNDataObject(surfaceData, surfGroup);

        int modelID = surfaceData->getAbsModelID();
        writeIntAttribute(surfGroup, "ModelID", &modelID);
        bool isInternal = surfaceData->isInternal();
        writeBoolAttribute(surfGroup, "isInternal", &isInternal);

        //写出类型
        auto surfType = surfaceData->getMeshSurfaceType();

        //枚举转换字符
        Core::FITKEnumTransfer<Interface::FITKMeshSurface::MeshSurfaceType> fitkSurfTypeTrafer;
        bool isValid = false;
        //写出类型
        QString TypeStr = fitkSurfTypeTrafer.toString(surfType, isValid);
        if (!isValid) return false;
        writeStrAttribute(surfGroup, "Type", TypeStr.toStdString());

        if (surfType == Interface::FITKMeshSurface::MeshSurfaceType::SurNode)
        {
            isW &= nodeTypeW(surfaceData, surfGroup);
        }
        else if (surfType == Interface::FITKMeshSurface::MeshSurfaceType::SurEle)
        {
            isW &= elementTypeW(surfaceData, surfGroup);
        }
        else
            return false;

        //写出引用的setID
        QList<int> setIDList;
        for (int i = 0; i < surfaceData->getMeshSetCount(); ++i)
        {
            auto set = surfaceData->getMeshSetAt(i);
            if (!set) return false;
            setIDList.append(set->getDataObjectID());
        }
        writeIntDataSet(surfGroup, "SetIDList", setIDList);

        //写出引用的SetParentID
        QList<int> setParentIDList;
        for (int i = 0; i < surfaceData->getMeshSetCount(); ++i)
        {
            auto model = surfaceData->getModelAt(i);
            setParentIDList.append(model->getDataObjectID());
        }
        writeIntDataSet(surfGroup, "SetModelID", setParentIDList);

        //写出唯一类型
        writeStrAttribute(surfGroup, "UniqueType", "MeshSurface");
        return true;
    }
    bool FITKInterfaceHDF5AdaptorSurface::geoSurfW(Interface::FITKGeometrySurface* surfaceData, H5::Group & h5Group)
    {
        if (surfaceData == nullptr) return false;
        bool isW = true;

        std::string groupName = createParentAttribute(surfaceData, h5Group);
        if (groupName.empty())return false;

        //创建表面的数据块
        H5::Group surfGroup = h5Group.createGroup(groupName);
        isW &= writeNDataObject(surfaceData, surfGroup);

        int modelID = surfaceData->getAbsModelID();
        writeIntAttribute(surfGroup, "ModelID", &modelID);

        bool isInternal = surfaceData->isInternal();
        writeBoolAttribute(surfGroup, "isInternal", &isInternal);

        //写出引用的setID
        int count = surfaceData->getSetCount();
        QList<int> setIDList;
        for (int i = 0; i < count; ++i)
        {
            int setID = surfaceData->getSetIDAt(i);
            setIDList.append(setID);
        }
        writeIntDataSet(surfGroup, "SetIDList", setIDList);

        //写出引用的SetParentID
        QList<int> setParentIDList;
        for (int i = 0; i < count; ++i)
        {
            auto model = surfaceData->getModelAt(i);
            setParentIDList.append(model->getDataObjectID());
        }
        writeIntDataSet(surfGroup, "SetModelID", setParentIDList);
        QList<int> polygonIndexList;

        for (int i = 0; i < count; i++)
        {
            //获取面的索引
            int polygonIndex = surfaceData->getSurfaceIndexAt(i);
            polygonIndexList.append(polygonIndex);
        }
        //写出SurfaceIndex
        writeIntDataSet(surfGroup, "SurfaceIndex", polygonIndexList);
        //写出唯一类型
        writeStrAttribute(surfGroup, "UniqueType", "GeoSurface");
        return true;
    }

    bool FITKInterfaceHDF5AdaptorSurface::nodeTypeW(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group)
    {
        Interface::FITKMeshSurfaceNode* abaSurfaceData = dynamic_cast<Interface::FITKMeshSurfaceNode*>(surfaceData);
        if (!abaSurfaceData) return false;
        int count = abaSurfaceData->getMeshSetCount();
        QList<double> weights;

        for (int i = 0; i < count; ++i)
        {
            //获取权重
            double value = abaSurfaceData->getValueAt(i);
            weights.append(value);
        }
        //写出权重
        writeDoubleDataSet(h5Group, "Weights", weights);
        return true;
    }

    bool FITKInterfaceHDF5AdaptorSurface::elementTypeW(Interface::FITKMeshSurface* surfaceData, H5::Group & h5Group)
    {
        Interface::FITKMeshSurfaceElement* abaSurfaceData = dynamic_cast<Interface::FITKMeshSurfaceElement*>(surfaceData);
        if (!abaSurfaceData) return false;
        int count = surfaceData->getMeshSetCount();
        QList<int> polygonIndexList;

        for (int i = 0; i < count; i++)
        {
            //获取面的索引
            int polygonIndex = abaSurfaceData->getSurfaceIndexAt(i);
            polygonIndexList.append(polygonIndex);
        }
        //写出SurfaceIndex
        writeIntDataSet(h5Group, "SurfaceIndex", polygonIndexList);
        return true;
    }

}


