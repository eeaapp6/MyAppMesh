/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelImport.h"
#include "FITK_Kernel/FITKAppFramework/FITKMessage.h"
#include "FITKOCCDXFReader.h"
#include "FITKOCCExtendTool.h"
#include "FITKOCCModelPart.h"
#include <QFile>
#include <STEPControl_Reader.hxx>
#include <TopoDS_Shape.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Reader.hxx>
#include <StlAPI.hxx>
#include <QDateTime>
#include <QDebug>

namespace OCC
{
    FITKOCCModelImport::FITKOCCModelImport() :OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelImport::update()
    {
        FITKOCCModelPart* part = _occShapeAgent->getPart();

        //文件路径判断
        if (_importFileName.isEmpty() || !QFile::exists(_importFileName)) return false;
        AppFrame::FITKMessageNormal(QString("Import file from %1 ...").arg(_importFileName));
        qDebug() << "Import File: " << QDateTime::currentDateTime().toString("hh:mm:ss:zzz");
        const QString lowerStr = _importFileName.toLower();
        QByteArray ba = _importFileName.toLocal8Bit();
        const char* cfileName = ba.data();

        bool ok = false;

        TopoDS_Shape result;
        if (lowerStr.endsWith(".brep"))
        {
            BRep_Builder builder;
            ok = BRepTools::Read(result, (const Standard_CString)cfileName, builder);//读取成功返回true
        }
        else if (lowerStr.endsWith(".stl"))
        {
            TopoDS_Shape ashape; //读取的结果
            ok = StlAPI::Read(result, (const Standard_CString)cfileName);
        }
        else if (lowerStr.endsWith(".stp") || lowerStr.endsWith(".step"))
        {
            STEPControl_Reader aReader;
            Standard_Integer status = aReader.ReadFile(cfileName);
            ok = status == IFSelect_RetDone;
            //读取成功
            if (ok)
            {
                aReader.TransferRoots();
                result = aReader.OneShape();
            }

        }
        else if (lowerStr.endsWith(".igs") || lowerStr.endsWith(".iges"))
        {
            //igs reader 初始化
            IGESControl_Controller::Init();
            IGESControl_Reader Reader;
            Standard_Integer status = Reader.ReadFile(cfileName);
            ok = status == IFSelect_RetDone;

            if (ok)
            {
                Reader.SetReadVisible(Standard_True);
                Reader.PrintCheckLoad(Standard_True, IFSelect_GeneralInfo);
                Reader.ClearShapes();
                Reader.TransferRoots();

                result = Reader.OneShape();
            }
        }
        //读取dxf
        else if (lowerStr.endsWith(".dxf"))
        {
            FITKOCCDXFReader reader(_importFileName);
            //读取成功
            ok = reader.read();
            if (ok)
            {
                result = reader.getShape();
            }
        }
        else
        {
            AppFrame::FITKMessageError(QString("Unknown file format %1").arg(_importFileName));
            return false;
        }

        if (ok)
        {
            if (part)
            {
                TopoDS_Shape* partShape = part->getPartShape();
                TopoDS_Builder builder;
                TopoDS_Compound compound;
                builder.MakeCompound(compound);

                if (!FITKOCCExtendTool::IsEmptyShape(*partShape))
                {
                    builder.Add(compound, *partShape);
                }

                builder.Add(compound, result);
                part->updatePartShape(compound, false);
                _occShapeAgent->updateShape(part->getCopiedPartShape(), false);
            }
            else
            {
                _occShapeAgent->updateShape(result);
            }
        }

        //读取失败报错
        if (!ok)
        {
            AppFrame::FITKMessageError(QString("Read file failed %1").arg(_importFileName));
            return false;
        }
        AppFrame::FITKMessageNormal(QString("Success import file from %1").arg(_importFileName));
        qDebug() << "Import finish: " << QDateTime::currentDateTime().toString("hh:mm:ss:zzz");

        return true;
    }

}



