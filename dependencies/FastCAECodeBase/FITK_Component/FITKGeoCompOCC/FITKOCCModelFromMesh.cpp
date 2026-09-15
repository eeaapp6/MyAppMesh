/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCModelFromMesh.h"
#include "FITK_Kernel/FITKAppFramework/FITKAppFramework.h"
#include <TopoDS_Shape.hxx>
#include <StlAPI.hxx>
#include <QDebug>

namespace OCC
{
    FITKOCCModelFromMesh::FITKOCCModelFromMesh()
        : OCCShapeAgent(this)
    {
        _shapeAgent = _occShapeAgent;
    }

    bool FITKOCCModelFromMesh::update()
    {
        // 写出stl文件
        QString filePath = FITKAPP->getTempDir("geo");
        filePath += "/mesh.stl";
        if (!writeSTL(filePath))
        {
            qDebug() << "Write STL file failed!";
            return false;
        }

        QByteArray ba = filePath.toLocal8Bit();
        const char* cfileName = ba.data();

        try
        {
            TopoDS_Shape ashape; //读取的结果
            bool  ok = StlAPI::Read(ashape, (const Standard_CString)cfileName);
            if (ok)
            {
                _occShapeAgent->updateShape(ashape);
            }
        }
        catch(...)
        {
            return false;
        }
        
        return true;

    }
}
 