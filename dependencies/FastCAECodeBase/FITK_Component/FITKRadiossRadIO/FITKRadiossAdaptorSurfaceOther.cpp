/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorSurfaceOther.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossMeshModel.h"
#include "FITK_Interface/FITKInterfaceModel/FITKComponentManager.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossSurfaceSeg.h"
#include "FITK_Component/FITKRadiossData/FITKRadiossNodesParts.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractElement.h"

#include "FITKRadiossRadReader.h"
#include "RadKeyLineSpliter.h"
#include "RadReadInformationMapper.h"

namespace Radioss
{
    QString FITKRadiossAdaptorSurfaceOther::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorSurfaceOther";
    }
    bool FITKRadiossAdaptorSurfaceOther::adaptR()
    {
        //错误判断，获取必要的工具类
        if (_caseData == nullptr || _reader == nullptr) return false;
        RadKeyLineSpliter* spliter = _reader->getKeyLineSpliter();
        if (!spliter)return false;
        RadReadInformationMapper* mapper = _reader->getReadRadInformationMapper();
        if (!mapper)return false;
        Radioss::FITKRadiossMeshModel* radiossMeshModel = _caseData->getMeshModel();
        if (!radiossMeshModel)return false;
        Interface::FITKComponentManager* componentManager = radiossMeshModel->getComponentManager();
        if (!componentManager)return false;

        //读取surface ID
        const int surfaceID = spliter->getId(0);
        if (surfaceID <= 0) return false;

        const QString setName = _reader->readLine();
        if (setName.isEmpty()) return false;

        //判断面组类型
        QString radKeys = spliter->getKeys();
        FITKAbstractRadiossSurface::RadiossSurfaceType type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_None;
        if (radKeys == "/SURF/PART")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PART;
        else if (radKeys == "/SURF/PART/ALL")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PartAll;
        else if (radKeys == "/SURF/PART/EXT")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PartExt;
        else if (radKeys == "/SURF/SURF")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_SURF;
        else if (radKeys == "/SURF/GRSHEL")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSHEL;
        else if (radKeys == "/SURF/GRSH3N")
            type = FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSH3N;


        if (type < 0)return false;

        QList<int> radIDs{};

        //读取Part列表
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            //读取结束标记
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            switch (type)
            {
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PART:
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PartAll: 
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_PartExt:
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_SURF:
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSHEL:
            case FITKAbstractRadiossSurface::RadiossSurfaceType::RST_GRSH3N: {
                QStringList partStrList = line.split(" ", Qt::SkipEmptyParts);
                do {
                    for (int i = 0; i < partStrList.size(); ++i) {
                        //RADId
                        int RADId = partStrList[i].toInt();
                        radIDs.append(RADId);
                    }
                    line = _reader->readLine();
                    partStrList = line.split(" ", Qt::SkipEmptyParts);
                    if (partStrList.isEmpty())
                        break;
                } while (partStrList[0].toInt() > 0);
                _reader->backLine();
                break;
            }
            default:
                break;
            }
        }
        if (radIDs.isEmpty()) {
            return false;
        }

        Radioss::FITKRadiossSurfaceSeg * surfaceSeg = new Radioss::FITKRadiossSurfaceSeg();
        surfaceSeg->setDataObjectName(setName);
        FITKRadiossSurfaceRadIOInfo* info = surfaceSeg->getRadIOInfo();

        info->setRadiossIDs(radIDs);
        info->setSurfaceType(type);
        //设置单元面集到RadiossMeshModel
        componentManager->appendDataObj(surfaceSeg);
        mapper->insertSurfaceSetIDMap(surfaceID, surfaceSeg->getDataObjectID());

        return true;
    }

}