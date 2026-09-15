/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "MeshGenerateWapper.h"

#include "FITK_Component/FITKHttpPythonDriver/FITKHttpCommons.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITK_Kernel/FITKCore/FITKOperatorRepo.h"
#include "FITK_Kernel/FITKCore/FITKDirFileTools.h"
#include "FITK_Kernel/FITKCore/FITKActionOperator.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"

#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoCommandList.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKGeoInterfaceFactory.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExportTopos.h"
#include "FITK_Interface/FITKInterfaceGeometry/FITKAbsGeoModelExport.h"

#include "FITK_Interface/FITKInterfaceMeshGen/FITKMeshGenInterface.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshSizeInfo.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMesherDriver.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKAbstractMeshProcessor.h"
#include "FITK_Interface/FITKInterfaceMeshGen/FITKGlobalMeshGenerateAlgorithmInfo.h"

#include "OperatorsInterface/GraphEventOperator.h"
#include "OperatorsInterface/TreeEventOperator.h"

namespace
{
    QString sendMessageToServer(const QString& message, bool isToServer = true)
    {
        if (isToServer)
        {
            HttpPython::FITKHttpCommons::sendMessageToServer(message);
        }
        return message;
    }
}

#include <QDebug>

MeshGenerate* MeshGenerateWapper::new_MeshGenerate()
{
    //创建MeshGenerate实例
    return new MeshGenerate;
}

void MeshGenerateWapper::delete_MeshGenerate(MeshGenerate* obj)
{
    //释放MeshGenerate实例
    if (obj) delete obj;
}

QString MeshGenerateWapper::static_MeshGenerate_GenerateMesh(const QString& g_name, int dim, double size_mesh)
{
    //获取几何命令列表
    Interface::FITKGeoCommandList* cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
    if (!cmdList) {
        AppFrame::FITKMessageError("No geometry data for mesh generation.");
        return sendMessageToServer(tr("Failed, no geometry data available for mesh generation"));
    }
    //通过名称获取对应的几何命令数据
    Interface::FITKAbsGeoCommand* cmd = cmdList->getDataByName(g_name);
    if (!cmd) {
        AppFrame::FITKMessageError("No geometry command found for mesh generation.");
        return sendMessageToServer(tr("Failed, geometry %1 was not found").arg(g_name));
    }
    int cmdID = cmd->getDataObjectID();
    QString meshPartName = this->meshGenerate(dim, size_mesh, size_mesh, QList<Interface::VirtualShape>(), cmdID);
    if (meshPartName.isEmpty())
    {
        return sendMessageToServer(tr("Failed, mesh generation for geometry %1 did not produce a mesh").arg(g_name));
    }
    return sendMessageToServer(tr("Success, generated %1D mesh named %2 from geometry %3").arg(dim).arg(meshPartName).arg(g_name));
}

QString MeshGenerateWapper::addFace(MeshGenerate* obj, const QString& g_name, const QVariantList& index_faces)
{
    if (!obj) return sendMessageToServer(tr("Failed, MeshGenerate object is null"));
    //添加面
    bool res = obj->addFace(g_name, index_faces);
    if (!res) {
        AppFrame::FITKMessageError("Failed to add face.");
        return sendMessageToServer(tr("Failed, unable to add faces from geometry %1").arg(g_name));
    }
    return sendMessageToServer(tr("Success, added %1 face item(s) from geometry %2").arg(obj->_vShape_faces.size()).arg(g_name));
}

QString MeshGenerateWapper::addVolume(MeshGenerate* obj, const QString& g_name, const QVariantList& index_volumes)
{
    if (!obj) return sendMessageToServer(tr("Failed, MeshGenerate object is null"));
    //添加体
    bool res = obj->addVolume(g_name, index_volumes);
    if (!res) {
        AppFrame::FITKMessageError("Failed to add volume.");
        return sendMessageToServer(tr("Failed, unable to add volumes from geometry %1").arg(g_name));
    }
    return sendMessageToServer(tr("Success, added %1 volume item(s) from geometry %2").arg(obj->_vShape_volumes.size()).arg(g_name));
}

QString MeshGenerateWapper::setMeshSize(MeshGenerate* obj, double size_min, double size_max)
{
    //设置网格尺寸
    if (!obj) return sendMessageToServer(tr("Failed, MeshGenerate object is null"));
    if (size_min <= 0.0 || size_max <= 0.0 || size_min > size_max)
    {
        return sendMessageToServer(tr("Failed, mesh size range is invalid: min=%1 max=%2").arg(size_min).arg(size_max));
    }
    obj->_size[0] = size_min;
    obj->_size[1] = size_max;
    return sendMessageToServer(tr("Success, mesh size set to min=%1 max=%2").arg(size_min).arg(size_max));
}

QString MeshGenerateWapper::generate(MeshGenerate* obj, int dim)
{
    //执行网格生成
    if (!obj) return sendMessageToServer(tr("Failed, MeshGenerate object is null"));
    //根据维度获取对应的虚拓扑数据
    QList<Interface::VirtualShape>* vShapes = nullptr;
    switch (dim)
    {
    case 2:
        vShapes = &obj->_vShape_faces;
        break;
    case 3:
        vShapes = &obj->_vShape_volumes;
        break;
    default:
        break;
    }
    if (!vShapes) {
        if (dim == 2)AppFrame::FITKMessageError("No face data for 2D mesh generation.");
        else if (dim == 3)AppFrame::FITKMessageError("No volume data for 3D mesh generation.");
        else AppFrame::FITKMessageError("Invalid dimension for mesh generation.");
        return sendMessageToServer(tr("Failed, mesh dimension %1 is invalid; only 2D or 3D is supported").arg(dim));
    }
    if (vShapes->isEmpty())
    {
        return sendMessageToServer(tr("Failed, no topology data has been added for %1D mesh generation").arg(dim));
    }
    //检查网格尺寸
    if (obj->_size[0] < 1e-6 || obj->_size[1] <= 1e-6)
    {
        AppFrame::FITKMessageError("Invalid mesh size.");
        return sendMessageToServer(tr("Failed, mesh size is invalid; call setMeshSize with positive values first"));
    }
    //获取网格划分接口
    Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
    if (!mf) return sendMessageToServer(tr("Failed, mesh generation interface is unavailable"));
    Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
    if (!algorithmInfo) return sendMessageToServer(tr("Failed, Gmsh mesh generation settings are unavailable"));
    //设置网格划分算法信息
    QString meshPartName = this->meshGenerate(dim, obj->_size[0], obj->_size[1], *vShapes);
    if (meshPartName.isEmpty()) {
        AppFrame::FITKMessageError("Mesh generation failed.");
        return sendMessageToServer(tr("Failed, mesh generation did not produce a mesh part"));
    }
    return sendMessageToServer(tr("Success, generated %1D mesh named %2").arg(dim).arg(meshPartName));
}

QString MeshGenerateWapper::meshGenerate(int dim, double size_min, double size_max, const QList<Interface::VirtualShape> vShape, int cmdID)
{
    //内部网格生成方法
    if (vShape.isEmpty() && cmdID == -1) {
        AppFrame::FITKMessageError("No geometry data for mesh generation.");
        return QString();
    }
    //获取工作目录
    QString meshPath = FITKAPP->getTempDir(false, "Gmsh");
    if (meshPath.isEmpty()) return QString();
    Core::ClearDir(meshPath);
    //指定网格划分路径
    QString shapeFile = QString("%1/%2").arg(meshPath).arg("geometryFile.brep");//指定软件生成的几何文件
    QString scriptFile = QString("%1/%2").arg(meshPath).arg("script.geo");      //指定网格划分驱动程序所需的脚本文件路径
    QString meshFile = QString("%1/%2").arg(meshPath).arg("mesh.msh");          //指定网格文件路径
    //获取网格划分接口
    Interface::FITKMeshGenInterface* mf = Interface::FITKMeshGenInterface::getInstance();
    if (!mf) return QString();
    Interface::FITKGlobalMeshGenerateAlgorithmInfo* algorithmInfo = mf->getGlobalMeshGenerateAlgorithmInfo("GmshExec");
    Interface::FITKAbstractMesherDriver* mesher = mf->getMesherDriver("GmshExec");
    Interface::FITKGlobalMeshSizeInfo* sizeInfo = mf->getGlobalMeshSizeInfo("GmshExec");
    if (!mesher || !algorithmInfo || !sizeInfo) return QString();
    //导出几何文件
    bool isExportSuccess = false;
    if (vShape.isEmpty()) {
        //导出完整几何模型
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return QString();
        Interface::FITKAbsGeoModelExport* geoExport = dynamic_cast<Interface::FITKAbsGeoModelExport*>
            (geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExport));
        if (!geoExport) return QString();
        geoExport->addExportCommandID(cmdID);
        geoExport->setFileName(shapeFile);
        isExportSuccess = geoExport->update();
        delete geoExport;
    }
    else {
        //导出具拓扑数据
        Interface::FITKInterfaceGeometryFactory* geoFactory = Interface::FITKInterfaceGeometryFactory::getInstance();
        if (!geoFactory) return QString();
        Interface::FITKAbsGeoModelExportTopos* geoExport = dynamic_cast<Interface::FITKAbsGeoModelExportTopos*>
            (geoFactory->createCommand(Interface::FITKGeoEnum::FITKGeometryComType::FGTExportTopos));
        if (!geoExport) return QString();
        if (dim == 2)
            geoExport->setEnableStitch(true);
        geoExport->setExportTopos(vShape);
        geoExport->setFileName(shapeFile);
        isExportSuccess = geoExport->update();
        delete geoExport;
    }
    if (!isExportSuccess) {
        AppFrame::FITKMessageError("Failed to export geometry file for meshing.");
        return QString();
    }
    //设置网格划分信息
    sizeInfo->setMinSize(size_min);
    sizeInfo->setMaxSize(size_max);
    algorithmInfo->setMeshGenerateDimension(dim);
    mesher->setValue("Method", 0);
    mesher->setValue("ShapeFile", shapeFile);
    mesher->setValue("ScriptFile", scriptFile);
    mesher->setValue("MeshFile", meshFile);
    //创建事件循环实现主进程阻塞等待网格划分完成
    QEventLoop eventLoop;
    //连接网格划分完成信号用于取消阻塞
    connect(mesher, &Interface::FITKAbstractMesherDriver::mesherFinished, &eventLoop, [&eventLoop]() {
        eventLoop.quit();
        });
    //开始驱动Gmsh进行网格划分
    mesher->startMesher();
    //执行事件循环进行阻塞等待mesherFinished信号触发quit()后取消阻塞
    eventLoop.exec();
    //获取网格生成操作器
    auto oper = FITKOPERREPO->getOperatorT<Core::FITKActionOperator>("actionGmshSettings");
    if (oper == nullptr) return QString();
    oper->setArgs("MeshFile", meshFile);
    oper->execProfession();
    QString meshPartName;
    oper->argValue<QString>("MeshPartName", meshPartName);
    return meshPartName;
}

void MeshGenerateWapper::updateGraphAndTreeEvent(int dataObjId)
{
    //更新图形和树形事件
    bool isGrapFitView = true;
    EventOper::GraphEventOperator* operGraph = FITKOPERREPO->getOperatorT<EventOper::GraphEventOperator>("GraphPreprocess");
    if (operGraph)
    {
        operGraph->updateGraph(dataObjId, true, isGrapFitView);
    }
    //通过树形菜单事件处理器刷新树
    QStringList listOpertor = { "TreeAssemblyEvent", "TreeGroupEvent" ,"TreePropertyEvent","TreeSolutionEvent" };
    for (int i = 0; i < listOpertor.size(); i++)
    {
        EventOper::TreeEventOperator* oper = dynamic_cast<EventOper::TreeEventOperator*>(Core::FITKOperatorRepo::getInstance()->getOperator(listOpertor[i]));
        if (oper != nullptr) oper->updateTree();
    }
}

MeshGenerate::MeshGenerate()
{
    //初始化成员变量
    _cmdList = FITKAPP->getGlobalData()->getGeometryData<Interface::FITKGeoCommandList>();
}

bool MeshGenerate::addFace(const QString& g_name, const QVariantList& index_faces)
{
    //添加几何面用于网格面生成
    if (!_cmdList) return false;
    Interface::FITKAbsGeoCommand* geo_cmd = _cmdList->getDataByName(g_name);
    if (!geo_cmd) return false;
    int cmdID = geo_cmd->getDataObjectID();
    Interface::FITKShapeVirtualTopoManager* faceTopoMgr = geo_cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSFace);
    if (!faceTopoMgr) return false;
    if (index_faces.isEmpty())
    {
        //添加所有面
        for (int i = 0; i < faceTopoMgr->getDataCount(); ++i)
        {
            Interface::FITKAbsVirtualTopo* virTopo = faceTopoMgr->getDataByIndex(i);
            if (!virTopo) continue;
            Interface::VirtualShape vShape(cmdID, virTopo->getDataObjectID(), i);
            vShape.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
            _vShape_faces.append(vShape);
        }
    }
    else
    {
        //添加指定面
        for (int i = 0; i < index_faces.size(); ++i)
        {
            int index = index_faces[i].toInt();
            if (index < 0) continue;
            Interface::FITKAbsVirtualTopo* virTopo = faceTopoMgr->getDataByIndex(index);
            if (!virTopo) continue;
            Interface::VirtualShape vShape(cmdID, virTopo->getDataObjectID(), index);
            vShape.Type = Interface::FITKGeoEnum::VTopoShapeType::VSFace;
            _vShape_faces.append(vShape);
        }
    }
    return true;
}

bool MeshGenerate::addVolume(const QString& g_name, const QVariantList& index_volumes)
{
    //添加几何体用于网格体生成
    if (!_cmdList) return false;
    Interface::FITKAbsGeoCommand* geo_cmd = _cmdList->getDataByName(g_name);
    if (!geo_cmd) return false;
    int cmdID = geo_cmd->getDataObjectID();
    Interface::FITKShapeVirtualTopoManager* solidTopoMgr = geo_cmd->getShapeVirtualTopoManager(Interface::FITKGeoEnum::VTopoShapeType::VSSolid);
    if (!solidTopoMgr) return false;
    if (index_volumes.isEmpty())
    {
        //添加所有体
        for (int i = 0; i < solidTopoMgr->getDataCount(); ++i)
        {
            Interface::FITKAbsVirtualTopo* virTopo = solidTopoMgr->getDataByIndex(i);
            if (!virTopo) continue;
            Interface::VirtualShape vShape(cmdID, virTopo->getDataObjectID(), i);
            vShape.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
            _vShape_volumes.append(vShape);
        }
    }
    else
    {
        //添加指定体
        for (int i = 0; i < index_volumes.size(); ++i)
        {
            int index = index_volumes[i].toInt();
            if (index < 0) continue;
            Interface::FITKAbsVirtualTopo* virTopo = solidTopoMgr->getDataByIndex(index);
            if (!virTopo) continue;
            Interface::VirtualShape vShape(cmdID, virTopo->getDataObjectID(), index);
            vShape.Type = Interface::FITKGeoEnum::VTopoShapeType::VSSolid;
            _vShape_volumes.append(vShape);
        }
    }
    return true;
}
