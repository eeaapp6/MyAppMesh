/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFSnappyHexMeshDictWriter.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include "FITK_Kernel/FITKCore/FITKEnumTransformer.hpp"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFGeometryData.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGeometryMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionGeometryRefine.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKVirtualTopoManager.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsVirtualTopo.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKZonePoints.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoShapeAgent.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractGeoModel.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelBox.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelCylinder.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelSphere.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoOperBool.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelImport.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSize.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeGeom.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include <QFileInfo>
#include <QtMath>
//命名空间
namespace IO
{
    FITKOFSnappyHexMeshDictWriter::FITKOFSnappyHexMeshDictWriter()
    {
    }
    FITKOFSnappyHexMeshDictWriter::~FITKOFSnappyHexMeshDictWriter()
    {
    }

    void FITKOFSnappyHexMeshDictWriter::setFilePath(const QString& filePath)
    {
        FITKOFAbstractDictWriter::setFilePath(filePath);
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return;
        m_fileName = QString("%1/system/snappyHexMeshDict").arg(filePath);
    }

    bool FITKOFSnappyHexMeshDictWriter::run()
    {
        //判断文件是否存在
        if (m_fileName.isEmpty())return false;
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取几何网格区域大小管理器
        m_geometryMeshSizeManager = meshGen->getGeometryMeshSizeManager();
        if (!m_geometryMeshSizeManager)return false;
        //获取字典工程
        this->openDictObject();
        if (!m_objectDict)return false;
        //写出FoamFile
        this->dictObjectFoamFile();
        //设置写出的网格区域数据
        bool Ok = this->snappyHexMeshGeometryMeshW();
        //写出字典文件
        if (Ok)
            Ok = this->writeDictFile();
        //关闭字典工程
        this->closeDictObject();
        return Ok;
    }

    QString FITKOFSnappyHexMeshDictWriter::getFileName()
    {
        return m_fileName;
    }

    void FITKOFSnappyHexMeshDictWriter::dictObjectFoamFile()
    {
        if (!m_objectDict)return;
        //FoamFile
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        group->append("version", 2.0);
        group->append("format", "ascii");
        group->append("class", "dictionary");
        group->append("location", "\"system\"");
        group->append("object", "snappyHexMeshDict");
        m_objectDict->append("FoamFile", group);
    }

    bool FITKOFSnappyHexMeshDictWriter::snappyHexMeshGeometryMeshW()
    {
        if (!m_objectDict)return false;
        bool ok = true;
        m_objectDict->append("castellatedMesh", true);
        m_objectDict->append("snap", true);
        //是否有层数
        bool addLayers = m_geometryMeshSizeManager->getDataCount() != 0 ? true : false;
        m_objectDict->append("addLayers", addLayers);
        ok &= this->geometrySnappyHexMesh();
        ok &= this->castellatedMeshControls();
        ok &= this->snapControls();
        ok &= this->addLayersControls(addLayers);
        ok &= this->meshQualityControls();
        m_objectDict->append("mergeTolerance", "1.0E-6");
        m_objectDict->append("debug", 0);
        return ok;
    }

    bool FITKOFSnappyHexMeshDictWriter::geometrySnappyHexMesh()
    {
        if (!m_objectDict)return false;
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return false;
        //获取几何数据
        Interface::FITKGeoCommandList* geometryData = globalData->getGeometryData<Interface::FITKGeoCommandList>();
        int count = geometryData->getDataCount();
        //创建字典组件
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        m_objectDict->append("geometry", group);
        //获取几何数据
        for (int i = 0; i < count; ++i)
        {
            //获取几何模型数据
            Interface::FITKAbsGeoCommand* geoCommand = dynamic_cast<Interface::FITKAbsGeoCommand*>(geometryData->getDataByIndex(i));
            if (!geoCommand)continue;
            //获取名称
            QString name = geoCommand->getDataObjectName();
            //创建stl字典组件
            DICT::FITKDictGroup* stlGroup = new DICT::FITKDictGroup;
            group->append(QString("%1.stl").arg(name), stlGroup);
            stlGroup->append("type", "triSurfaceMesh");
            //获取类型
            Interface::FITKGeoEnum::FITKGeometryComType type = geoCommand->getGeometryCommandType();
            if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTBox)
            {
                Interface::FITKAbsGeoModelBox* box = dynamic_cast<Interface::FITKAbsGeoModelBox*>(geoCommand);
                if (!box)return false;
                stlGroup->append("simflowType", "primitive");
                stlGroup->append("primitiveType", "box");
                stlGroup->append("name", name);
                //第一个点
                double minP[3]{ 0.0, 0.0, 0.0 };
                box->getPoint1(minP);
                //长度
                double length[3]{ 0.0, 0.0, 0.0 };
                box->getLength(length);
                //最高点
                double maxP[3]{ minP[0], minP[1], minP[2] };
                length[0] < 0 ? minP[0] = minP[0] + length[0] : maxP[0] = minP[0] + length[0];
                length[1] < 0 ? minP[1] = minP[1] + length[1] : maxP[1] = minP[1] + length[1];
                length[2] < 0 ? minP[2] = minP[2] + length[2] : maxP[2] = minP[2] + length[2];
                stlGroup->append("max", QString("(%1 %2 %3)").arg(maxP[0]).arg(maxP[1]).arg(maxP[2]));
                stlGroup->append("min", QString("(%1 %2 %3)").arg(minP[0]).arg(minP[1]).arg(minP[2]));
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTCylinder)
            {
                Interface::FITKAbsGeoModelCylinder* cylinder = dynamic_cast<Interface::FITKAbsGeoModelCylinder*>(geoCommand);
                if (!cylinder)return false;
                stlGroup->append("simflowType", "primitive");
                stlGroup->append("primitiveType", "cylinder");
                stlGroup->append("name", name);
                //底面圆心
                double p[3]{ 0.0, 0.0, 0.0 };
                cylinder->getLocation(p);
                stlGroup->append("point1", QString("(%1 %2 %3)").arg(p[0]).arg(p[1]).arg(p[2]));
                //轴向与长度
                double axis[3]{ 0.0, 0.0, 0.0 };
                double length = cylinder->getLength();
                cylinder->getDirection(axis);
                double axisLenth = qSqrt(axis[0] * axis[0] + axis[1] * axis[1] + axis[2] * axis[2]);
                //获取上圆心
                double p2[3]{ p[0] + length * axis[0] / axisLenth, p[1] + length * axis[1] / axisLenth, p[2] + length * axis[2] / axisLenth };
                stlGroup->append("point2", QString("(%1 %2 %3)").arg(p2[0]).arg(p2[1]).arg(p2[2]));
                //半径
                double r = cylinder->getRadius();
                stlGroup->append("radius", r);
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTSphere)
            {
                Interface::FITKAbsGeoModelSphere* sphere = dynamic_cast<Interface::FITKAbsGeoModelSphere*>(geoCommand);
                if (!sphere)return false;
                stlGroup->append("simflowType", "primitive");
                stlGroup->append("primitiveType", "sphere");
                stlGroup->append("name", name);
                //球心
                double center[3]{ 0.0 };
                sphere->getLocation(center);
                stlGroup->append("centre", QString("(%1 %2 %3)").arg(center[0]).arg(center[1]).arg(center[2]));
                //半径
                double radius = sphere->getRadius();
                stlGroup->append("radius", radius);
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTBool)
            {
                Interface::FITKAbsGeoOperBool* boolOper = dynamic_cast<Interface::FITKAbsGeoOperBool*>(geoCommand);
                if (!boolOper)return false;
                stlGroup->append("simflowType", "brep");
                stlGroup->append("topoType", "compound");
                stlGroup->append("name", name);
            }
            else if (type == Interface::FITKGeoEnum::FITKGeometryComType::FGTImport)
            {
                Interface::FITKAbsGeoModelImport* importModel = dynamic_cast<Interface::FITKAbsGeoModelImport*>(geoCommand);
                if (!importModel)return false;
                //stlGroup->append("simflowType", "stl");
                stlGroup->append("simflowType", "stl");
                stlGroup->append("topoType", "shell");
                stlGroup->append("name", name);
            }
            if (!this->geoFaceGroupStl(geoCommand->getShapeAgent(), stlGroup, name))
                return false;
        }
        return true;
    }

    bool FITKOFSnappyHexMeshDictWriter::castellatedMeshControls()
    {
        if (!m_objectDict)return false;
        //创建字典组件
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        m_objectDict->append("castellatedMeshControls", group);
        //获取网格生成单例
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        //获取材料点管理器
        Interface::FITKZonePointManager* pointManager = meshGen->getZonePointManager();
        if (!pointManager)return false;
        //获取区域网格尺寸管理器
        Interface::FITKRegionMeshSizeManager* meshRegionManager = meshGen->getRegionMeshSizeMgr();
        if (!meshRegionManager)return false;
        //获取材料点的个数
        int count = pointManager->getDataCount();
        double point[3]{ 0,0,0 };
        if (count == 0)return false;
        //材料点
        if (count == 1)
        {
            //Interface::FITKZonePoint* pointData = pointManager->getDataByIndex(0);
            //if (!pointData)return false;
            //pointData->getCoor(point);
            //group->append("locationInMesh", QString("(%1 %2 %3)").arg(point[0]).arg(point[1]).arg(point[2]));
            Interface::FITKZonePoint* pointData = pointManager->getDataByIndex(0);
            if (!pointData)return false;
            pointData->getCoor(point);
            group->append("insidePoint", QString("(%1 %2 %3)").arg(point[0]).arg(point[1]).arg(point[2]));
        }
        else
        {
            ////多个材料点
            //DICT::FITKDictArray* materialPoint = new DICT::FITKDictArray;
            //group->append("locationsInMesh", materialPoint);
            //for (int i = 0; i < count; ++i)
            //{
            //    Interface::FITKZonePoint* pointData = pointManager->getDataByIndex(i);
            //    if (!pointData)continue;
            //    pointData->getCoor(point);
            //    DICT::FITKDictVector* pointDict = new DICT::FITKDictVector;
            //    pointDict->append(QString("(%1 %2 %3)").arg(point[0]).arg(point[1]).arg(point[2]));
            //    pointDict->append(QString("zone%1").arg(i + 1));
            //    materialPoint->append(pointDict);
            //}
            //多个材料点
            DICT::FITKDictArray* materialPoint = new DICT::FITKDictArray;
            group->append("insidePoints", materialPoint);
            for (int i = 0; i < count; ++i)
            {
                Interface::FITKZonePoint* pointData = pointManager->getDataByIndex(i);
                if (!pointData)continue;
                pointData->getCoor(point);
                DICT::FITKDictVector* pointDict = new DICT::FITKDictVector;
                pointDict->append(QString("%1 %2 %3").arg(point[0]).arg(point[1]).arg(point[2]));
                materialPoint->append(pointDict);
            }
        }
        //面加密 创建字典组件
        DICT::FITKDictGroup* refinement = new DICT::FITKDictGroup;
        group->append("refinementSurfaces", refinement);
        //获取集合大小
        QList<Interface::FITKAbstractRegionMeshSize*> geomList = meshRegionManager->getRigonByType(Interface::FITKAbstractRegionMeshSize::RegionType::RigonGeom);
        for (Interface::FITKAbstractRegionMeshSize* geom : geomList)
        {
            Interface::FITKRegionMeshSizeGeom* geomData = dynamic_cast<Interface::FITKRegionMeshSizeGeom*>(geom);
            if (!geomData)continue;
            Interface::FITKAbsGeoCommand* geoCommand = FITKDATAREPO->getTDataByID<Interface::FITKAbsGeoCommand>(geomData->getGeomID());
            if (!geoCommand)continue;
            //获取最值
            int min = int(geomData->getMinSize());
            int max = int(geomData->getMaxSize());
            //获取名称
            QString name = geoCommand->getDataObjectName();
            //创建stl字典组件
            DICT::FITKDictGroup* modelGroup = new DICT::FITKDictGroup;
            refinement->append(name, modelGroup);
            modelGroup->append("level", QString("( %1 %2 )").arg(min).arg(max));
            DICT::FITKDictGroup* patchInfo = new DICT::FITKDictGroup;
            modelGroup->append("patchInfo", patchInfo);
            patchInfo->append("type", "wall");
        }
        //获取几何优化管理器
        Interface::FITKRegionGeometryRefineManager* geoRefineMgr = meshGen->getRegionGeometryRefineManager();
        if (!geoRefineMgr) return false;
        //体加密-细化区域
        DICT::FITKDictGroup* refinementRegions = new DICT::FITKDictGroup;
        group->append("refinementRegions", refinementRegions);
        count = geoRefineMgr->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            //获取区域优化数据
            Interface::FITKRegionGeometryRefine* regRefine = geoRefineMgr->getDataByIndex(i);
            if (!regRefine)continue;
            //获取几何数据
            Interface::FITKAbsGeoCommand* geoCommand = regRefine->getGeometryModel();
            if (!geoCommand) continue;
            DICT::FITKDictGroup* refRegion = new DICT::FITKDictGroup;
            refinementRegions->append(geoCommand->getDataObjectName(), refRegion);
            //获取距离和限制
            double distance = regRefine->getDistance();
            int level = regRefine->getLevel();
            //获取优化模式
            Interface::FITKRegionGeometryRefine::RefineModelType refinetype = regRefine->getRefineModel();
            if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineDistance)
            {
                refRegion->append("mode", "distance");
                refRegion->append("levels", QString("((%1 %2))").arg(distance).arg(level));
            }
            else if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineInside)
            {
                refRegion->append("mode", "inside");
                refRegion->append("levels", QString("((1.0E30 %1))").arg(level));
            }
            else if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineOutside)
            {
                refRegion->append("mode", "outside");
                refRegion->append("levels", QString("((1.0E30 %1))").arg(level));
            }
            //获取间隙参数
            int cells = regRefine->getCellsInGapNum();
            int maxGap = regRefine->getMaxGapLevel();
            //获取间隙模式
            Interface::FITKRegionGeometryRefine::GapModelType gaptype = regRefine->getGapRefineModel();
            if (gaptype == Interface::FITKRegionGeometryRefine::GapModelType::GapMixed)
            {
                refRegion->append("gapMode", "mixed");
                refRegion->append("gapLevel", QString("((%1 0 %2))").arg(cells).arg(maxGap));
            }
            else if (gaptype == Interface::FITKRegionGeometryRefine::GapModelType::GapInside)
            {
                refRegion->append("gapMode", "inside");
                refRegion->append("gapLevel", QString("((%1 0 %2))").arg(cells).arg(maxGap));
            }
            else if (gaptype == Interface::FITKRegionGeometryRefine::GapModelType::GapOutside)
            {
                refRegion->append("gapMode", "outside");
                refRegion->append("gapLevel", QString("((%1 0 %2))").arg(cells).arg(maxGap));
            }
        }
        //限制区域 
        DICT::FITKDictGroup* limitRegions = new DICT::FITKDictGroup;
        group->append("limitRegions", limitRegions);
        for (int i = 0; i < count; ++i)
        {
            //获取区域优化数据
            Interface::FITKRegionGeometryRefine* regRefine = geoRefineMgr->getDataByIndex(i);
            if (!regRefine)continue;
            //获取几何数据
            Interface::FITKAbsGeoCommand* geoCommand = regRefine->getGeometryModel();
            if (!geoCommand) continue;
            if (!regRefine->getLimitRefineMent())continue;
            DICT::FITKDictGroup* refRegion = new DICT::FITKDictGroup;
            limitRegions->append(geoCommand->getDataObjectName(), refRegion);
            //获取距离和限制
            double distance = regRefine->getDistance();
            int level = regRefine->getLevel();
            //获取优化模式
            Interface::FITKRegionGeometryRefine::RefineModelType refinetype = regRefine->getRefineModel();
            if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineDistance)
            {
                refRegion->append("mode", "distance");
                refRegion->append("levels", QString("(%1 %2)").arg(distance).arg(level));
            }
            else if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineInside)
            {
                refRegion->append("mode", "outside");
                refRegion->append("levels", QString("(1.0E30 %1)").arg(level));
            }
            else if (refinetype == Interface::FITKRegionGeometryRefine::RefineModelType::RefineOutside)
            {
                refRegion->append("mode", "inside");
                refRegion->append("levels", QString("(1.0E30 %1)").arg(level));
            }
        }
        //默认数据
        group->append("maxLocalCells", 1000000);
        group->append("maxGlobalCells", 10000000);
        group->append("nCellsBetweenLevels", 4);
        group->append("maxLoadUnbalance", 0.1);
        group->append("minRefinementCells", 10);
        group->append("resolveFeatureAngle", 30.0);
        group->append("allowFreeStandingZoneFaces", true);
        return true;
    }

    bool FITKOFSnappyHexMeshDictWriter::geoFaceGroupStl(Interface::FITKAbsGeoShapeAgent* geoModel, DICT::FITKDictGroup* stlGroup, QString name)
    {
        if (!geoModel || !stlGroup)return false;
        //获取几何组件管理器
        Interface::FITKGeoComponentManager* geoComponentManager = geoModel->getGeoComponentManager();
        if (!geoComponentManager)return false;
        //创建面数据字典组件
        DICT::FITKDictGroup* faceGroups = new DICT::FITKDictGroup;
        stlGroup->append("faceGroups", faceGroups);

        if (!geoModel->getVirtualTopoManager())return false;
        Interface::FITKShapeVirtualTopoManager* faceMgr = geoModel->getVirtualTopoManager()->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
        if (!faceMgr)return false;

        //获取模型全部表面数量
        int allFaceNum = geoModel->getShapeCount(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
        for (int i = 0; i < allFaceNum; ++i)
        {
            DICT::FITKDictGroup* face = new DICT::FITKDictGroup;
            face->append("name", QString("%1").arg(name));
            faceGroups->append(QString("face_%1").arg(i), face);
        }
        //获取面组链表
        QList<Interface::FITKGeoComponent*> componentList = geoComponentManager->getGeoComponent(Interface::FITKModelEnum::FITKModelSetType::FMSSurface);
        for (Interface::FITKGeoComponent* componentData : componentList)
        {
            if (!componentData)continue;
            QString groupName = componentData->getDataObjectName();
            //面id
            QList<int> id_list = componentData->getMember();
            for (int i = 0; i < id_list.size(); ++i)
            {
                Interface::FITKAbsVirtualTopo* topo = faceMgr->getDataByID(id_list.at(i));
                if (!topo)continue;
                int index = topo->getIndexLabel();
                faceGroups->removeKey(QString("face_%1").arg(index));
                DICT::FITKDictGroup* face = new DICT::FITKDictGroup;
                face->append("name", QString("%1_%2").arg(name).arg(groupName));
                faceGroups->append(QString("face_%1").arg(index), face);
            }
        }

        return true;
    }

    bool FITKOFSnappyHexMeshDictWriter::snapControls()
    {
        if (!m_objectDict)return false;
        //创建添加字典组件
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        m_objectDict->append("snapControls", group);
        group->append("tolerance", 1.0);
        group->append("nSmoothPatch", 3);
        group->append("nSolveIter", 500);
        group->append("nRelaxIter", 5);
        group->append("nFeatureSnapIter", 10);
        group->append("implicitFeatureSnap", false);
        group->append("explicitFeatureSnap", true);
        group->append("multiRegionFeatureSnap", true);
        group->append("nFaceSplitInterval", 5);
        return true;
    }

    bool FITKOFSnappyHexMeshDictWriter::addLayersControls(bool isAddLayers)
    {
        if (!m_objectDict || !m_geometryMeshSizeManager)return false;
        //创建添加图层字典组件
        DICT::FITKDictGroup* addLayer = new DICT::FITKDictGroup;
        m_objectDict->append("addLayersControls", addLayer);
        //创建添加层级字典组件
        DICT::FITKDictGroup* layers = new DICT::FITKDictGroup;
        addLayer->append("layers", layers);
        //添加层数
        int count = m_geometryMeshSizeManager->getDataCount();
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKGeometryMeshSize* geoMeshSize = m_geometryMeshSizeManager->getDataByIndex(i);
            if (!geoMeshSize)continue;
            //获取数据
            int layerNum = geoMeshSize->getLayerNumber();
            double ratio = geoMeshSize->getExpansionRatio();
            double thickness = geoMeshSize->getLayerThickness();
            //获取组件
            Interface::FITKGeoComponent* componentData = geoMeshSize->getGeoComponent();
            //获取模型
            Interface::FITKAbsGeoCommand* geoCommand = geoMeshSize->getGeoModel();
            if (!componentData || !geoCommand)continue;
            Interface::FITKAbsGeoShapeAgent* geoModel = geoCommand->getShapeAgent();
            if (!geoModel || !geoModel->getVirtualTopoManager())return false;
            Interface::FITKShapeVirtualTopoManager* faceMgr = geoModel->getVirtualTopoManager()->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
            if (!faceMgr)return false;
            QString name = geoCommand->getDataObjectName();
            //编号
            QList<int> id_list = componentData->getMember();
            for (int i = 0; i < id_list.size(); ++i)
            {
                Interface::FITKAbsVirtualTopo* topo = faceMgr->getDataByID(id_list.at(i));
                if (!topo)continue;
                DICT::FITKDictGroup* face = new DICT::FITKDictGroup;
                face->append("nSurfaceLayers", layerNum);
                face->append("expansionRatio", ratio);
                face->append("firstLayerThickness", thickness);
                int index = topo->getIndexLabel();
                layers->append(QString("%1_face_%2").arg(name).arg(index), face);
            }
        }
        //添加默认数据
        addLayer->append("relativeSizes", true);
        addLayer->append("minThickness", 0.1);
        addLayer->append("firstLayerThickness", 0.2);
        addLayer->append("expansionRatio", 1.25);
        addLayer->append("nGrow", 0);
        addLayer->append("featureAngle", 180.0);
        addLayer->append("maxFaceThicknessRatio", 0.5);
        addLayer->append("nSmoothSurfaceNormals", 5);
        addLayer->append("nSmoothThickness", 10);
        addLayer->append("minMedialAxisAngle", 90.0);
        addLayer->append("maxThicknessToMedialRatio", 0.5);
        addLayer->append("nMedialAxisIter", 100);
        addLayer->append("nSmoothNormals", 3);
        addLayer->append("slipFeatureAngle", 30.0);
        addLayer->append("nRelaxIter", 5);
        addLayer->append("nBufferCellsNoExtrude", 0);
        addLayer->append("nLayerIter", 50);
        addLayer->append("nRelaxedIter", 20);
        addLayer->append("detectExtrusionIsland", true);
        
        //是否添加几何层级
        if (!isAddLayers)return true;
        
        return true;
    }

    bool FITKOFSnappyHexMeshDictWriter::meshQualityControls()
    {
        if (!m_objectDict)return false;
        //创建添加字典组件
        DICT::FITKDictGroup* group = new DICT::FITKDictGroup;
        m_objectDict->append("meshQualityControls", group);
        group->append("maxNonOrtho", 65.0);
        group->append("maxBoundarySkewness", 20.0);
        group->append("maxInternalSkewness", 4.0);
        group->append("maxConcave", 80.0);
        group->append("minVol", "1.0E-14");
        group->append("minTetQuality", "1.0E-20");
        group->append("minArea", -1.0);
        group->append("minTwist", 0.02);
        group->append("minTriangleTwist", -1.0);
        group->append("minDeterminant", 0.01);
        group->append("minFaceWeight", 0.05);
        group->append("minVolRatio", 0.01);
        group->append("minVolCollapseRatio", 0.1);
        group->append("nSmoothScale", 4);
        group->append("errorReduction", 0.75);
        DICT::FITKDictGroup* relaxed = new DICT::FITKDictGroup;
        group->append("relaxed", relaxed);
        relaxed->append("maxNonOrtho", 75.0);
        return true;
    }
}



