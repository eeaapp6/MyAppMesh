/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKFlowOFHDF5AdaptorRegionMesh.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoCommand.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeBox.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeGeom.h"
#include "FITK_Interface/FITKInterfaceIO/FITKHDF5FileTool.h"

#include "FITKFlowOFHDF5Reader.h"
#include "FITKFlowOFHDF5Writer.h"


#include <QDebug>
namespace IO
{
    QString FITKFlowOFHDF5AdaptorRegionMesh::getAdaptorClass()
    {
        return "FITKFlowOFHDF5AdaptorRegionMesh";
    }

    bool FITKFlowOFHDF5AdaptorRegionMesh::adaptR()
    {
        _regionMeshData = dynamic_cast<Interface::FITKAbstractRegionMeshSize*>(_dataObj);
        if (!_reader || !_regionMeshData) return false;
        bool isR = false;
        //读取
        readNDataObject(_regionMeshData, *_h5Group);
        this->regionMeshSizeAbstractR();
        //读取其他数据
        int type = _regionMeshData->getRegionType();
        if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RigonGeom)
            isR = this->regionGeometryR();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionBox)
            isR = this->regionBoxR();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionCylinder)
            isR = this->regionCylinderR();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere)
            isR = this->regionSphereR();
        return isR;
    }

    bool FITKFlowOFHDF5AdaptorRegionMesh::adaptW()
    {
        _regionMeshData = dynamic_cast<Interface::FITKAbstractRegionMeshSize*>(_dataObj);
        if (!_writer || !_regionMeshData) return false;
        bool isW = false;
        //写出
        writeNDataObject(_regionMeshData, *_h5Group);
        this->regionMeshSizeAbstractW();
        //读写出其他数据
        int type = _regionMeshData->getRegionType();
        if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RigonGeom)
            isW = this->regionGeometryW();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionBox)
            isW = this->regionBoxW();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionCylinder)
            isW = this->regionCylinderW();
        else if (type == Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere)
            isW = this->regionSphereW();

        return isW;
    }

    bool FITKFlowOFHDF5AdaptorRegionMesh::regionMeshSizeAbstractW()
    {
        if (!_writer || !_regionMeshData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //获取数据
        int division1 = _regionMeshData->getDivision(0);
        int division2 = _regionMeshData->getDivision(1);
        int division3 = _regionMeshData->getDivision(2);
        double grading1 = _regionMeshData->getGrading(0);
        double grading2 = _regionMeshData->getGrading(1);
        double grading3 = _regionMeshData->getGrading(2);
        double globalSize = _regionMeshData->getGlobalSize();
        double minSize = _regionMeshData->getMinSize();
        double maxSize = _regionMeshData->getMaxSize();
        double sizeFactor = _regionMeshData->getSizeFactor();
        double internalSize = _regionMeshData->getInternalSize();
        double externalSize = _regionMeshData->getExternalSize();
        double thickness = _regionMeshData->getThickness();
        //写出数据
        tool->writeGroupAttrInt(*_h5Group, "division1", division1);
        tool->writeGroupAttrInt(*_h5Group, "division2", division2);
        tool->writeGroupAttrInt(*_h5Group, "division3", division3);
        tool->writeGroupAttrDouble(*_h5Group, "grading1", grading1);
        tool->writeGroupAttrDouble(*_h5Group, "grading2", grading2);
        tool->writeGroupAttrDouble(*_h5Group, "grading3", grading3);
        tool->writeGroupAttrDouble(*_h5Group, "globalSize", globalSize);
        tool->writeGroupAttrDouble(*_h5Group, "minSize", minSize);
        tool->writeGroupAttrDouble(*_h5Group, "maxSize", maxSize);
        tool->writeGroupAttrDouble(*_h5Group, "sizeFactor", sizeFactor);
        tool->writeGroupAttrDouble(*_h5Group, "internalSize", internalSize);
        tool->writeGroupAttrDouble(*_h5Group, "externalSize", externalSize);
        tool->writeGroupAttrDouble(*_h5Group, "thickness", thickness);

        return false;
    }

    bool FITKFlowOFHDF5AdaptorRegionMesh::regionMeshSizeAbstractR()
    {
        if (!_reader || !_regionMeshData) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        int division1 = 0;
        int division2 = 0;
        int division3 = 0;
        double grading1 = 0.0;
        double grading2 = 0.0;
        double grading3 = 0.0;
        double globalSize = 0.0;
        double minSize = 0.0;
        double maxSize = 0.0;
        double sizeFactor = 0.0;
        double internalSize = 0.0;
        double externalSize = 0.0;
        double thickness = 0.0;
        tool->readGroupint(*_h5Group, "division1", &division1);
        tool->readGroupint(*_h5Group, "division2", &division2);
        tool->readGroupint(*_h5Group, "division3", &division3);
        tool->readGroupDouble(*_h5Group, "grading1", &grading1);
        tool->readGroupDouble(*_h5Group, "grading2", &grading2);
        tool->readGroupDouble(*_h5Group, "grading3", &grading3);
        tool->readGroupDouble(*_h5Group, "globalSize", &globalSize);
        tool->readGroupDouble(*_h5Group, "minSize", &minSize);
        tool->readGroupDouble(*_h5Group, "maxSize", &maxSize);
        tool->readGroupDouble(*_h5Group, "sizeFactor", &sizeFactor);
        tool->readGroupDouble(*_h5Group, "internalSize", &internalSize);
        tool->readGroupDouble(*_h5Group, "externalSize", &externalSize);
        tool->readGroupDouble(*_h5Group, "thickness", &thickness);
        //设置数据
        _regionMeshData->setDivision(0, division1);
        _regionMeshData->setDivision(1, division2);
        _regionMeshData->setDivision(2, division3);
        _regionMeshData->setGrading(0, grading1);
        _regionMeshData->setGrading(1, grading2);
        _regionMeshData->setGrading(2, grading3);
        _regionMeshData->setGlobalSize(globalSize);
        _regionMeshData->setMinSize(minSize);
        _regionMeshData->setMaxSize(maxSize);
        _regionMeshData->setSizeFactor(sizeFactor);
        _regionMeshData->setInternalSize(internalSize);
        _regionMeshData->setExternalSize(externalSize);
        _regionMeshData->setThickness(thickness);
        return true;
    }

    bool FITKFlowOFHDF5AdaptorRegionMesh::regionGeometryW()
    {
        Interface::FITKRegionMeshSizeGeom* geoRegion = dynamic_cast<Interface::FITKRegionMeshSizeGeom*>(_regionMeshData);
        if (!_writer || !geoRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        int geomId = geoRegion->getGeomID();
        int min = int(geoRegion->getMinSize());
        int max = int(geoRegion->getMaxSize());

        tool->writeGroupAttrInt(*_h5Group, "geomId", geomId);
        tool->writeGroupAttrInt(*_h5Group, "maxValue", max);
        tool->writeGroupAttrInt(*_h5Group, "minValue", min);
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionGeometryR()
    {
        Interface::FITKRegionMeshSizeGeom* geoRegion = dynamic_cast<Interface::FITKRegionMeshSizeGeom*>(_regionMeshData);
        if (!_reader || !geoRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        int geomId = 0;
        int max = 0;
        int min = 0;

        tool->readGroupint(*_h5Group, "geomId", &geomId);
        tool->readGroupint(*_h5Group, "maxValue", &max);
        tool->readGroupint(*_h5Group, "minValue", &min);
        geoRegion->setGeomID(geomId);
        geoRegion->setMinSize(min);
        geoRegion->setMaxSize(max);
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionBoxW()
    {
        Interface::FITKRegionMeshSizeBox* boxRegion = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(_regionMeshData);
        if (!_writer || !boxRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //写出数据
        double point1[3]{ 0 };
        boxRegion->getPoint1(point1);
        double length[3]{ 0 };
        boxRegion->getLength(length);
        tool->writeGroupAttrDoubleArray(*_h5Group, "point1", point1, 1, 3);
        tool->writeGroupAttrDoubleArray(*_h5Group, "length", length, 1, 3);
        //boundary
        for (int i = 0; i < 6; ++i)
        {
            int boundType = boxRegion->getBoundary(i);
            std::string faceName = boxRegion->getFaceName(i).toStdString();
            std::string boundTypeKey = (QString("boundType%1").arg(i)).toStdString();
            std::string faceNameKey = (QString("faceName%1").arg(i)).toStdString();
            tool->writeGroupAttrInt(*_h5Group, boundTypeKey.c_str(), boundType);
            tool->writeGroupAttrStr(*_h5Group, faceNameKey.c_str(), faceName);
        }
        
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionBoxR()
    {
        Interface::FITKRegionMeshSizeBox* boxRegion = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(_regionMeshData);
        if (!_reader || !boxRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //读取数据
        double point1[3]{ 0 };
        double length[3]{ 0 };
        tool->readGroupAttrDoubleArray(*_h5Group, "point1", point1, 1, 3);
        tool->readGroupAttrDoubleArray(*_h5Group, "length", length, 1, 3);

        boxRegion->setPoint1(point1);
        boxRegion->setLength(length);

        //boundary
        for (int i = 0; i < 6; ++i)
        {
            std::string boundTypeKey = (QString("boundType%1").arg(i)).toStdString();
            std::string faceNameKey = (QString("faceName%1").arg(i)).toStdString();
            int type = 0;
            std::string name;
            tool->readGroupint(*_h5Group, boundTypeKey.c_str(), &type);
            tool->readGroupStr(*_h5Group, faceNameKey.c_str(), name);
            boxRegion->insertBoundary(i, Interface::FITKAbstractRegionMeshSize::BoundaryType(type));
            boxRegion->insertFaceName(i, QString::fromStdString(name));
        }

        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionCylinderW()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinderRegion = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(_regionMeshData);
        if (!_writer || !cylinderRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _writer->getHDF5FileTool();
        if (!tool) return false;
        //圆心坐标
        double location[3] = { 0,0,0 };
        cylinderRegion->getLocation(location);
        tool->writeGroupAttrDoubleArray(*_h5Group, "location", location, 1, 3);
        //轴线方向
        double direction[3] = { 0,0,0 };
        cylinderRegion->getDirection(direction);
        tool->writeGroupAttrDoubleArray(*_h5Group, "direction", direction, 1, 3);
        //半径
        double radius = cylinderRegion->getRadius();
        tool->writeGroupAttrDouble(*_h5Group, "radius", radius);
        //长度
        double length = cylinderRegion->getLength();
        tool->writeGroupAttrDouble(*_h5Group, "length", length);
        //半径因子
        double radiusFaction = cylinderRegion->getRadialFraction();
        tool->writeGroupAttrDouble(*_h5Group, "radiusFaction", radiusFaction);
        //boundary
        for (int i = 0; i < 3; ++i)
        {
            int boundType = cylinderRegion->getBoundary(i);
            std::string faceName = cylinderRegion->getFaceName(i).toStdString();
            std::string boundTypeKey = (QString("boundType%1").arg(i)).toStdString();
            std::string faceNameKey = (QString("faceName%1").arg(i)).toStdString();
            tool->writeGroupAttrInt(*_h5Group, boundTypeKey.c_str(), boundType);
            tool->writeGroupAttrStr(*_h5Group, faceNameKey.c_str(), faceName);
        }

        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionCylinderR()
    {
        Interface::FITKRegionMeshSizeCylinder* cylinderRegion = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(_regionMeshData);
        if (!_reader || !cylinderRegion) return false;
        //获取工具
        IO::FITKHDF5FileTool* tool = _reader->getHDF5FileTool();
        if (!tool) return false;
        //圆心坐标
        double location[3] = { 0 };
        tool->readGroupAttrDoubleArray(*_h5Group, "location", location, 1, 3);
        cylinderRegion->setLocation(location);
        //轴线方向
        double direction[3] = { 0 };
        tool->readGroupAttrDoubleArray(*_h5Group, "direction", direction, 1, 3);
        cylinderRegion->setDirection(direction);
        //半径
        double radius = 0.0;
        tool->readGroupDouble(*_h5Group, "radius", &radius);
        cylinderRegion->setRadius(radius);
        //长度
        double length = 0.0;
        tool->readGroupDouble(*_h5Group, "length", &length);
        cylinderRegion->setLength(length);
        //半径因子
        double radiusFaction = 0.0;
        tool->readGroupDouble(*_h5Group, "radiusFaction", &radiusFaction);
        cylinderRegion->setRadialFraction(radiusFaction);
        //boundary
        for (int i = 0; i < 3; ++i)
        {
            std::string boundTypeKey = (QString("boundType%1").arg(i)).toStdString();
            std::string faceNameKey = (QString("faceName%1").arg(i)).toStdString();
            int type = 0;
            std::string name;
            tool->readGroupint(*_h5Group, boundTypeKey.c_str(), &type);
            tool->readGroupStr(*_h5Group, faceNameKey.c_str(), name);
            cylinderRegion->insertBoundary(i, Interface::FITKAbstractRegionMeshSize::BoundaryType(type));
            cylinderRegion->insertFaceName(i, QString::fromStdString(name));
        }
        return true;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionSphereW()
    {
        if (!_writer || !_regionMeshData) return false;
        return false;
    }
    bool FITKFlowOFHDF5AdaptorRegionMesh::regionSphereR()
    {
        if (!_reader || !_regionMeshData) return false;
        return false;
    }
}
