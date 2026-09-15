/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshGeoScriptWrite.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeBox.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeCylinder.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKRegionMeshSizeSphere.h"

#include <QFileInfo>
#include <QDir>
#include <QTextStream>

#include <math.h>

namespace GmshExe
{
    FITKGmshGeoScriptWrite::FITKGmshGeoScriptWrite(bool* _success) : m_isWriteOK(_success)
    {

    }

    FITKGmshGeoScriptWrite::~FITKGmshGeoScriptWrite()
    {

    }

    void FITKGmshGeoScriptWrite::setShapeFromFile(const QString shapeFilePath)
    {
        m_shapeFilePath = shapeFilePath;
    }

    void FITKGmshGeoScriptWrite::setMeshFile(const QString meshFilePath)
    {
        m_meshFilePath = meshFilePath;
    }

    void FITKGmshGeoScriptWrite::run()
    {
        if (m_isWriteOK) *m_isWriteOK = false;
        //判断形状文件是否写出
        QFileInfo shapeFileInfo(m_shapeFilePath);
        if (!shapeFileInfo.isFile())
        {
            this->consoleMessage(3, "Write Geo Script Err! The shapeFile does not exist !!!");
            return;
        }
        QDir dir;
        //获取网格路径
        QString meshPath = m_meshFilePath.mid(0, m_meshFilePath.lastIndexOf('/'));
        //确保网格路径存在
        if (!dir.exists(meshPath))
        {
            bool res = dir.mkpath(meshPath);
            if (!res)return;
        }
        //获取文件名
        QString fileName = this->getFileName();
        //获取路径
        QString path = fileName.mid(0, fileName.lastIndexOf('/'));
        //确保路径存在
        if (!dir.exists(path))
        {
            bool res = dir.mkpath(path);
            if (!res)return;
        }
        //打开文件
        QFile file(fileName);
        if (file.exists())file.remove();
        //设置写出
        if (!file.open(QIODevice::WriteOnly | QIODevice::NewOnly))
        {
            file.close();
            return;
        }
        //打开文件流
        QTextStream stream(&file);
        //写出几何数据文件
        bool ok = this->writeGeoScript(&stream);
        //关闭文件
        file.close();
        if (m_isWriteOK) *m_isWriteOK = ok;
    }

    void FITKGmshGeoScriptWrite::consoleMessage(int level, const QString& message)
    {
        switch (level)
        {
        case 1: AppFrame::FITKMessageNormal(message);
            break;
        case 2: AppFrame::FITKMessageWarning(message);
            break;
        case 3: AppFrame::FITKMessageError(message);
            break;
        default:
            break;
        }
    }

    bool FITKGmshGeoScriptWrite::writeGeoScript(QTextStream* stream)
    {
        if (!stream) return false;
        *stream << QString("SetFactory(\"OpenCASCADE\");") << endl;
        *stream << QString("Merge \"%1\";").arg(m_shapeFilePath) << endl;
        //step文件默认单位是米，而不是毫米，这里指出单位
        *stream << QString("Geometry.OCCTargetUnit = \"M\";") << endl;
        
        this->writeGeometryEdgeSeedPoint(stream);
        this->writeGeometryProcessConfig(stream);
        this->writeRefinedMeshConfig(stream);
        this->writeMeshConfig(stream);
        
        //获取网格算法信息
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = meshGen->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!algorithmInfo) return false;
        //生成网格
        *stream << QString("Mesh %1;").arg(algorithmInfo->getMeshGenerateDimension()) << endl;
        ////写出网格文件
        //*stream << QString("Save \"%1\";").arg(m_meshFilePath) << endl;
        return true;
    }

    bool FITKGmshGeoScriptWrite::writeGeometryEdgeSeedPoint(QTextStream * stream)
    {
        if (!stream) return false;
        //获取网格算法信息
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        /*meshGen->getMeshParameterManager(1)*/
        return true;
    }

    bool FITKGmshGeoScriptWrite::writeGeometryProcessConfig(QTextStream* stream)
    {
        if (!stream) return false;
        //获取网格算法信息
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = meshGen->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!algorithmInfo) return false;
        //移除几何重复的实体(包含同一位置的实体)
        if (algorithmInfo->getGeometryCoherence())
        {
            *stream << QString("Geometry.OCCFixDegenerated = 1;") << endl;
            *stream << QString("Geometry.OCCFixSmallEdges = 1;") << endl;
            *stream << QString("Geometry.OCCFixSmallFaces = 1;") << endl;
            *stream << QString("Geometry.OCCSewFaces = 1;") << endl;
        }
        //几何合并
        if (algorithmInfo->getGeometryStitch() && algorithmInfo->getMeshGenerateDimension() == 2)
        {
            *stream << QString("Geometry.OCCSewFaces = 1;") << endl;
        }
        *stream << QString("Coherence;") << endl;
        return true;
    }

    bool FITKGmshGeoScriptWrite::writeRefinedMeshConfig(QTextStream* stream)
    {
        if (!stream) return false;
        //获取局部加密区域管理器-可以包含多个加密区域
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKRegionMeshSizeManager* regionMeshMgr = meshGen->getRegionMeshSizeMgr("GmshExec");
        if (!regionMeshMgr)return false;
        int count = regionMeshMgr->getDataCount();
        QStringList fieldsList;
        for (int i = 0; i < count; ++i)
        {
            Interface::FITKAbstractRegionMeshSize* regionMesh = regionMeshMgr->getDataByIndex(i);
            if (!regionMesh) continue;
            int tag = i + 1;
            bool ok = false;
            Interface::FITKAbstractRegionMeshSize::RegionType typeRegion = regionMesh->getRegionType();
            if (typeRegion == Interface::FITKAbstractRegionMeshSize::RegionType::RegionBox)
                ok = this->addGmshMeshFieldBox(stream, tag, regionMesh);
            else if (typeRegion == Interface::FITKAbstractRegionMeshSize::RegionType::RegionCylinder)
                ok = this->addGmshMeshFieldCylinder(stream, tag, regionMesh);
            else if (typeRegion == Interface::FITKAbstractRegionMeshSize::RegionType::RegionSphere)
                ok = this->addGmshMeshFieldSphere(stream, tag, regionMesh);
            if (!ok) continue;
            fieldsList.append(QString::number(tag));
        }
        if (fieldsList.size() > 1)
        {
            Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo =
                meshGen->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
            bool useMin = true;
            if (algorithmInfo)
            {
                const QVariant mergeMode = algorithmInfo->getValue("FieldMergeMin");
                if (mergeMode.isValid())
                    useMin = mergeMode.toBool();
            }
            const QString fieldType = useMin ? QStringLiteral("Min") : QStringLiteral("Max");
            *stream << QString("Field[%1] = %2;").arg(count + 1).arg(fieldType) << endl;
            *stream << QString("Field[%1].FieldsList = {%2};").arg(count + 1).arg(fieldsList.join(',')) << endl;
            *stream << QString("Background Field = %1;").arg(count + 1) << endl;
        }
        else if (fieldsList.size() == 1)
            *stream << QString("Background Field = %1;").arg(fieldsList.at(0)) << endl;
        return true;
    }

    bool FITKGmshGeoScriptWrite::addGmshMeshFieldBox(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh)
    {
        if (!stream) return false;
        //gmsh添加boxField
        Interface::FITKRegionMeshSizeBox* regionBox = dynamic_cast<Interface::FITKRegionMeshSizeBox*>(regionMesh);
        if (!regionBox) return false;
        //获取第一个点,长度
        double point1[3]{ 0.0 }, length[3]{ 0.0 };
        regionBox->getPoint1(point1);
        regionBox->getLength(length);
        //转化point1为最低点，point2为最高点
        double point2[3]{ point1[0], point1[1], point1[2] };
        length[0] < 0 ? point1[0] = point1[0] + length[0] : point2[0] = point1[0] + length[0];
        length[1] < 0 ? point1[1] = point1[1] + length[1] : point2[1] = point1[1] + length[1];
        length[2] < 0 ? point1[2] = point1[2] + length[2] : point2[2] = point1[2] + length[2];
        //获取内部尺寸，外部尺寸，厚度
        double vIn = regionBox->getValueT<double>("InternalSize"), vOut = regionBox->getValueT<double>("ExternalSize"),
            thickness = regionBox->getValueT<double>("Thickness");
        bool background = regionBox->getValueT<bool>("BackgroundRegion");

        //设置网格场数据
        *stream << QString("Field[%1] = Box;").arg(tag) << endl;
        *stream << QString("Field[%1].Thickness = %2;").arg(tag).arg(QString::number(thickness)) << endl;
        *stream << QString("Field[%1].VIn = %2;").arg(tag).arg(QString::number(vIn)) << endl;
        *stream << QString("Field[%1].VOut = %2;").arg(tag).arg(QString::number(vOut)) << endl;
        *stream << QString("Field[%1].XMin = %2;").arg(tag).arg(QString::number(point1[0])) << endl;
        *stream << QString("Field[%1].YMin = %2;").arg(tag).arg(QString::number(point1[1])) << endl;
        *stream << QString("Field[%1].ZMin = %2;").arg(tag).arg(QString::number(point1[2])) << endl;
        *stream << QString("Field[%1].XMax = %2;").arg(tag).arg(QString::number(point2[0])) << endl;
        *stream << QString("Field[%1].YMax = %2;").arg(tag).arg(QString::number(point2[1])) << endl;
        *stream << QString("Field[%1].ZMax = %2;").arg(tag).arg(QString::number(point2[2])) << endl;
        return true;
    }

    bool FITKGmshGeoScriptWrite::addGmshMeshFieldCylinder(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh)
    {
        if (!stream) return false;
        //gmsh添加CylinderField
        Interface::FITKRegionMeshSizeCylinder* regionCylinder = dynamic_cast<Interface::FITKRegionMeshSizeCylinder*>(regionMesh);
        if (!regionCylinder) return false;
        //获取点坐标和方向向量
        double point[3]{ 0.0 }, axis[3]{ 0.0 };
        regionCylinder->getLocation(point);
        regionCylinder->getDirection(axis);
        //获取长度
        double length = regionCylinder->getLength();
        //根据方向向量和移动距离，获取中心点的方向分量
        double multiplicative = length / 2 / sqrt((axis[0] * axis[0]) + (axis[1] * axis[1]) + (axis[2] * axis[2]));
        double heftAxis[3]{ axis[0] * multiplicative, axis[1] * multiplicative, axis[2] * multiplicative };
        //获取中心点的坐标
        double center[3]{ point[0] + heftAxis[0], point[1] + heftAxis[1], point[2] + heftAxis[2] };
        //获取半径
        double radius = regionCylinder->getRadius();
        //获取内部尺寸，外部尺寸，厚度
        double vIn = regionCylinder->getValueT<double>("InternalSize"), vOut = regionCylinder->getValueT<double>("ExternalSize"),
            thickness = regionCylinder->getValueT<double>("Thickness");
        bool background = regionCylinder->getValueT<bool>("BackgroundRegion");

        //设置网格场数据
        *stream << QString("Field[%1] = Cylinder;").arg(tag) << endl;
        *stream << QString("Field[%1].Radius = %2;").arg(tag).arg(QString::number(radius)) << endl;
        *stream << QString("Field[%1].VIn = %2;").arg(tag).arg(QString::number(vIn)) << endl;
        *stream << QString("Field[%1].VOut = %2;").arg(tag).arg(QString::number(vOut)) << endl;
        *stream << QString("Field[%1].XAxis = %2;").arg(tag).arg(QString::number(heftAxis[0])) << endl;
        *stream << QString("Field[%1].XCenter = %2;").arg(tag).arg(QString::number(center[0])) << endl;
        *stream << QString("Field[%1].YAxis = %2;").arg(tag).arg(QString::number(heftAxis[1])) << endl;
        *stream << QString("Field[%1].YCenter = %2;").arg(tag).arg(QString::number(center[1])) << endl;
        *stream << QString("Field[%1].ZAxis = %2;").arg(tag).arg(QString::number(heftAxis[2])) << endl;
        *stream << QString("Field[%1].ZCenter = %2;").arg(tag).arg(QString::number(center[2])) << endl;
        return tag;
    }

    bool FITKGmshGeoScriptWrite::addGmshMeshFieldSphere(QTextStream* stream, int tag, Interface::FITKAbstractRegionMeshSize* regionMesh)
    {
        if (!stream) return false;
        //gmsh添加Sphere
        Interface::FITKRegionMeshSizeSphere* regionSphere = dynamic_cast<Interface::FITKRegionMeshSizeSphere*>(regionMesh);
        if (!regionSphere) return false;
        //获取球心的坐标
        double center[3]{ 0.0 };
        regionSphere->getLocation(center);
        //获取半径
        double radius = regionSphere->getRadius();
        //获取内部尺寸，外部尺寸，厚度
        double vIn = regionSphere->getValueT<double>("InternalSize"), vOut = regionSphere->getValueT<double>("ExternalSize"),
            thickness = regionSphere->getValueT<double>("Thickness");
        bool background = regionSphere->getValueT<bool>("BackgroundRegion");

        //设置网格场数据
        *stream << QString("Field[%1] = Ball;").arg(tag) << endl;
        *stream << QString("Field[%1].Thickness = %2;").arg(tag).arg(QString::number(thickness)) << endl;
        *stream << QString("Field[%1].Radius = %2;").arg(tag).arg(QString::number(radius)) << endl;
        *stream << QString("Field[%1].VIn = %2;").arg(tag).arg(QString::number(vIn)) << endl;
        *stream << QString("Field[%1].VOut = %2;").arg(tag).arg(QString::number(vOut)) << endl;
        *stream << QString("Field[%1].XCenter = %2;").arg(tag).arg(QString::number(center[0])) << endl;
        *stream << QString("Field[%1].YCenter = %2;").arg(tag).arg(QString::number(center[1])) << endl;
        *stream << QString("Field[%1].ZCenter = %2;").arg(tag).arg(QString::number(center[2])) << endl;
        return tag;
    }

    bool FITKGmshGeoScriptWrite::writeMeshConfig(QTextStream* stream)
    {
        if (!stream) return false;
        //获取全局尺寸大小数据对象和网格算法信息
        Interface::FITKMeshGenInterface* meshGen = Interface::FITKMeshGenInterface::getInstance();
        if (!meshGen)return false;
        Interface::FITKGlobalMeshSizeInfo* sizeInfo = meshGen->getGlobalMeshSizeInfo("GmshExec");
        if (!sizeInfo)return false;
        Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = meshGen->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
        if (!algorithmInfo) return false;
        //获取网格生成配置数据
        int algorithm2D = algorithmInfo->getValueT<int>("2DAlgorithm");
        int algorithm3D = algorithmInfo->getValueT<int>("3DAlgorithm");
        int reAlgorithm2D = algorithmInfo->getValueT<int>("2DReAlgorithm");
        bool reTriMesh = algorithmInfo->getValueT<bool>("ReTriangularMesh");
        int subdivisionAlgorithm = algorithmInfo->getValueT<int>("SubdivisionAlgorithm");
        double smoothStep = algorithmInfo->getValueT<double>("SmoothStep");
        double sizeFactor = sizeInfo->getSizeFactor();
        double sizeMin = sizeInfo->getMinSize();
        double sizeMax = sizeInfo->getMaxSize();
        int eleOrader = algorithmInfo->getValueT<int>("ElementOrder");
        bool useIncElem = algorithmInfo->getValueT<bool>("UseIncElem");
        double smoothRatio = algorithmInfo->getValueT<double>("MeshSmoothRatio");
        bool meshOptimize = algorithmInfo->getValueT<bool>("MeshOptimize");
        bool meshOptimizeNetgen = algorithmInfo->getValueT<bool>("MeshOptimizeNetgen");
        double optimizeThreshold = algorithmInfo->getValueT<double>("MeshOptimizeThreshold");
        int qualityType = algorithmInfo->getValueT<int>("MeshQualityType");
        bool checkSurfaceNormal = algorithmInfo->getValueT<bool>("MeshCheckSurfaceNormal");
        //错误参数判断
        if (fabsl(sizeMax) < 1e-20) return false;
        //写出配置到geo脚本
        *stream << QString("Mesh.Algorithm = %1;").arg(this->algorithm2dDisposition(algorithm2D)) << endl;
        *stream << QString("Mesh.Algorithm3D = %1;").arg(this->algorithm3dDisposition(algorithm3D)) << endl;
        *stream << QString("Mesh.RecombinationAlgorithm = %1;").arg(this->reAlgorithm2dDisposition(reAlgorithm2D)) << endl;
        *stream << QString("Mesh.SubdivisionAlgorithm = %1;").arg(this->subdivisionAlgorithmDisposition(subdivisionAlgorithm)) << endl;
        *stream << QString("Mesh.RecombineAll = %1;").arg(int(reTriMesh)) << endl;
        *stream << QString("Mesh.Smoothing = %1;").arg(smoothStep) << endl;
        *stream << QString("Mesh.CharacteristicLengthFactor = %1;").arg(sizeFactor) << endl;
        *stream << QString("Mesh.CharacteristicLengthMin = %1;").arg(sizeMin) << endl;
        *stream << QString("Mesh.CharacteristicLengthMax = %1;").arg(sizeMax) << endl;
        *stream << QString("Mesh.ElementOrder = %1;").arg(eleOrader) << endl;
        *stream << QString("Mesh.SecondOrderIncomplete = %1;").arg(int(useIncElem)) << endl;
        if (smoothRatio > 1e-12)
            *stream << QString("Mesh.SmoothRatio = %1;").arg(smoothRatio) << endl;
        *stream << QString("Mesh.Optimize = %1;").arg(int(meshOptimize)) << endl;
        *stream << QString("Mesh.OptimizeNetgen = %1;").arg(int(meshOptimizeNetgen)) << endl;
        if (optimizeThreshold >= 0.0 && optimizeThreshold <= 1.0)
            *stream << QString("Mesh.OptimizeThreshold = %1;").arg(optimizeThreshold) << endl;
        *stream << QString("Mesh.QualityType = %1;").arg(qualityType) << endl;
        *stream << QString("Mesh.CheckSurfaceNormalValidity = %1;").arg(int(checkSurfaceNormal)) << endl;
        return true;
    }

    int FITKGmshGeoScriptWrite::algorithm2dDisposition(int algorithm2D)
    {
        //2D算法
        Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D dispositionType = Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D(algorithm2D);
        if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::Automatic)
            return 2;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::MeshAdapt)
            return 1;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::Delaunay2D)
            return 5;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::FrontalDelaunay)
            return 6;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::BAMG)
            return 7;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::FrontalDelaunayForQuads)
            return 8;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm2D::PackingOfParallelograms)
            return 9;
        else
            //默认值
            return 6;
    }

    int FITKGmshGeoScriptWrite::algorithm3dDisposition(int algorithm3D)
    {
        //3D算法
        Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D dispositionType = Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D(algorithm3D);
        if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::Delaunay3D)
            return 1;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::Frontal)
            return 4;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::HXT)
            return 10;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::Algorithm3D::MMG3D)
            return 7;
        else
            //默认值
            return 1;
    }

    int FITKGmshGeoScriptWrite::reAlgorithm2dDisposition(int reAlgorithm2D)
    {
        //2D重组算法
        Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D dispositionType = Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D(reAlgorithm2D);
        if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::Simple)
            return 0;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::Blossom)
            return 1;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::SimpleFullQuad)
            return 2;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::ReAlgorithm2D::BlossomFullQuad)
            return 3;
        else
            //默认值
            return 1;
    }

    int FITKGmshGeoScriptWrite::subdivisionAlgorithmDisposition(int subAlgorithm)
    {
        //细分算法
        Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm dispositionType = Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm(subAlgorithm);
        if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::None)
            return 0;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllQuads)
            return 1;
        else if (dispositionType == Interface::FITKGlobalMeshGenerateAlgorithmInfo::SubdivisionAlgorithm::AllHexas)
            return 2;
        else
            //默认值
            return 0;
    }

}
