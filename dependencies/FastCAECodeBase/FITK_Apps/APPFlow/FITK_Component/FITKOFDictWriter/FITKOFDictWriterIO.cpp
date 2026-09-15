/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOFDictWriterIO.h"
#include "FITKOFBlockMeshDictWriter.h"
#include "FITKOFSnappyHexMeshDictWriter.h"
#include "FITKOFControlDictWriter.h"
#include "FITKOFCreatePatchDictWriter.h"
#include "FITKOFTurbulencePropertiesWrite.h"
#include "FITKOFTransportPropertiesWrite.h"
#include "FITKOFFVSchemesWriter.h"
#include "FITKOFFVSolutionWriter.h"
#include "FITKOFFVOptionsWriter.h"
#include "FITKOFChangeDictionaryDictW.h"
#include "FITKOFOperatingConditionsW.h"
#include "FITKOFInitialDirWriter.h"
#include "FITKOFSetFieldsDictWriter.h"
#include "FITKOFMeshBoundaryW.h"
#include "FITKOFThermoPhyPropDictWriter.h"
#include "FITKOFRadiationPropDictWriter.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include "FITK_Kernel/FITKAppFramework/FITKGlobalData.h"
#include "FITK_Interface/FITKInterfaceFlowOF/FITKOFPhysicsData.h"
#include <QFileInfo>
//命名空间
namespace IO
{
    FITKOFDictWriterIO::FITKOFDictWriterIO()
    {
    }
    FITKOFDictWriterIO::~FITKOFDictWriterIO()
    {
    }

    QString FITKOFDictWriterIO::getComponentName()
    {
        return "IO::FITKOFDictWriterIO";
    }

    void FITKOFDictWriterIO::setPhysicsDictW()
    {
        this->setPhysicsDictWFile();
    }

    void FITKOFDictWriterIO::setReadLevel(QList<IO::DictWriteType> typeList)
    {
        //设置类型链表
        m_writeTypeList = typeList;
    }

    bool FITKOFDictWriterIO::setFilePath(const QString& filePath)
    {
        QFileInfo fileInfo(filePath);
        //判断是否是路径
        if (!fileInfo.isDir())return false;
        //设置文件路径
        m_filePath = filePath;
        return true;
    }

    bool FITKOFDictWriterIO::exec()
    {
        if (m_writeTypeList.isEmpty())return false;
        bool ok = true;
        for (IO::DictWriteType type : m_writeTypeList)
        {
            //获取写出对象
            FITKOFAbstractDictWriter* w = nullptr;
            if (type == IO::DictWriteType::BlockMesh)
                w = new IO::FITKOFBlockMeshDictWriter;
            else if (type == IO::DictWriteType::SnappyHexMesh)
                w = new IO::FITKOFSnappyHexMeshDictWriter;
            else if (type == IO::DictWriteType::Control)
                w = new IO::FITKOFControlDictWriter;
            else if (type == IO::DictWriteType::CreatePatch)
                w = new IO::FITKOFCreatePatchDictWriter;
            else if (type == IO::DictWriteType::TurbulenceProp)
                w = new IO::FITKOFTurbulencePropertiesWrite;
            else if (type == IO::DictWriteType::TransportProp)
                w = new IO::FITKOFTransportPropertiesWrite;
            else if (type == IO::DictWriteType::FvSchemes)
                w = new IO::FITKOFFVSchemesWriter;
            else if (type == IO::DictWriteType::FvSolution)
                w = new IO::FITKOFFVSolutionWriter;
            else if (type == IO::DictWriteType::FvOptions)
                w = new IO::FITKOFFVOptionsWriter;
            else if (type == IO::DictWriteType::ChangeDictionaryDict)
                w = new IO::FITKOFChangeDictionaryDictW;
            else if (type == IO::DictWriteType::InitialDir)
                w = new IO::FITKOFInitialDirWriter;
            else if (type == IO::DictWriteType::OperatingConditions)
                w = new IO::FITKOFOperatingConditionsW;
            else if (type == IO::DictWriteType::SetFieldsDict)
                w = new IO::FITKOFSetFieldsDictWriter;
            else if (type == IO::DictWriteType::BoundaryMesh)
                w = new IO::FITKOFMeshBoundaryW;
            else if (type == IO::DictWriteType::RadiationProperties)
                w = new IO::FITKOFRadiationPropDictWriter;
            else if (type == IO::DictWriteType::ThermophysicalProperties)
                w = new IO::FITKOFThermoPhyPropDictWriter;
            if (w == nullptr)return false;
            //设置路径
            w->setFilePath(m_filePath);
            ok &= w->run();
            //销毁对象
            delete w;
        }
        m_writeTypeList.clear();
        return ok;
    }

    QString FITKOFDictWriterIO::getFilePath()
    {
        //获取文件路径
        return m_filePath;
    }

    FITKOFDictWriterIO &FITKOFDictWriterIO::operator<<(const DictWriteType type)
    {
        //追加类型
        m_writeTypeList.append(type);
        return *this;
    }

    void FITKOFDictWriterIO::setPhysicsDictWFile()
    {
        m_writeTypeList.clear();
        //获取全局数据管理器
        AppFrame::FITKGlobalData* globalData = FITKAPP->getGlobalData();
        if (!globalData)return;
        //获取物理数据
        Interface::FITKOFPhysicsData* physicsData = globalData->getPhysicsData<Interface::FITKOFPhysicsData>();
        if (!physicsData)return;
        Interface::FITKAbstractOFSolver* solver = physicsData->getSolver();
        if (!solver)return;
        m_writeTypeList << Control << TurbulenceProp << TransportProp << FvSchemes << FvSolution << FvOptions 
            << ChangeDictionaryDict << InitialDir << OperatingConditions << SetFieldsDict << BoundaryMesh
            << RadiationProperties << ThermophysicalProperties;

    }
}



