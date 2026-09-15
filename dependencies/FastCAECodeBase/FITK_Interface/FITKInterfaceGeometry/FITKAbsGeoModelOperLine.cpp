/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbsGeoModelOperLine.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{
    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoModelOperLine::getGeometryCommandType()
    {
        return FITKGeoEnum::FGTMergeLine;
    }

    void FITKAbsGeoModelOperLine::add(VirtualShape geoCommandID)
    {
        m_tempVShapes.append(geoCommandID);
    }

    void FITKAbsGeoModelOperLine::add(QList<VirtualShape> geoCommandIDs)
    {
        m_tempVShapes.append(geoCommandIDs);
    }

    void FITKAbsGeoModelOperLine::set(QList<VirtualShape> geoCommandIDs)
    {
        m_tempVShapes.clear();
        m_tempVShapes.append(geoCommandIDs);
    }

    void FITKAbsGeoModelOperLine::remove(int index)
    {
        m_tempVShapes.removeAt(index);
    }

    void FITKAbsGeoModelOperLine::clear()
    {
        m_tempVShapes.clear();
    }

    void FITKAbsGeoModelOperLine::setType(GeoLineOperType t)
    {
        _lineOperType = t;
    }

    FITKAbsGeoModelOperLine::GeoLineOperType FITKAbsGeoModelOperLine::getType()
    {
        return _lineOperType;
    }

    //GeoLineOperType FITKAbsGeoModelOperLine::getType()
    //{
    //    return _lineOperType;
    //}

    QList<VirtualShape> FITKAbsGeoModelOperLine::getVShapes()
    {
        return m_tempVShapes;
    }

    QList<int> FITKAbsGeoModelOperLine::getModelOperLinesID()
    {
        QList<int> _geoCommands;

        for (int i = 0; i < m_tempVShapes.size(); i++)
        {
            _geoCommands.append(m_tempVShapes[i].CmdId);
        }
        return _geoCommands;
    }
    void FITKAbsGeoModelOperLine::setAngle(double angle)
    {
        _angle = angle;
    }
    double FITKAbsGeoModelOperLine::getAngle()
    {
        return _angle;
    }

    FITKGeoEnum::FITKGeometryComType FITKAbsGeoModelMergeOperLine::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTMergeLine;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoModelMiddleOperLine::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSplitByMiddlePositionLine;
    }

    FITKGeoEnum::FITKGeometryComType Interface::FITKAbsGeoModelAngleOperLine::getGeometryCommandType()
    {
        return FITKGeoEnum::FITKGeometryComType::FGTSplitByAngleLine;
    }
}