/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKGmshGeoScriptManuallyWrite.h"

namespace GmshExe
{
    FITKGmshGeoScriptManuallyWrite::FITKGmshGeoScriptManuallyWrite()
    {

    }

    FITKGmshGeoScriptManuallyWrite::~FITKGmshGeoScriptManuallyWrite()
    {

    }

    void FITKGmshGeoScriptManuallyWrite::setFileName(const QString& fileName)
    {
        _fileName = fileName;
    }

    void FITKGmshGeoScriptManuallyWrite::addPiont(int pID, double* coor, double piontSize)
    {
        if (!coor) return;
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Point(%1) = {%2, %3, %4, %5};").arg(pID).arg(coor[0]).arg(coor[1]).arg(coor[2]).arg(piontSize));
    }

    void FITKGmshGeoScriptManuallyWrite::addLine(int lID, int p1ID, int p2ID)
    {
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Line(%1) = {%2, %3};").arg(lID).arg(p1ID).arg(p2ID));
    }

    void FITKGmshGeoScriptManuallyWrite::addPlaneSurface(int fID, QList<int> lListIDs)
    {
        if (lListIDs.isEmpty()) return;
        QStringList idListStr;
        for (int id : lListIDs)
            idListStr.append(QString::number(id));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Curve Loop(%1) = {%2};").arg(fID).arg(idListStr.join(", ")));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Plane Surface(%1) = {%1};").arg(fID));
    }

    void FITKGmshGeoScriptManuallyWrite::addVolume(int vID, QList<int> fListIDs)
    {
        if (fListIDs.isEmpty()) return;
        QStringList idListStr;
        for (int id : fListIDs)
            idListStr.append(QString::number(id));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Surface Loop(%1) = {%2};").arg(vID).arg(idListStr.join(", ")));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Volume(%1) = {%1};").arg(vID));
    }

    void FITKGmshGeoScriptManuallyWrite::deleteGeo(int type, QList<int> idLists)
    {
        //需要删除的类型
        QString strType;
        if (type == 1)
            strType = "Point";
        else if (type == 2)
            strType = "Curve";
        else if (type == 3)
            strType = "Surface";
        if (strType.isEmpty() || idLists.isEmpty()) return;
        //写出需要删除的对象数据ID
        QStringList deleteData{};
        for (int id : idLists)
            deleteData.append(QString("%1{%2};").arg(strType).arg(id));
        //写出命令
        m_scriptCommand.append("//+");
        m_scriptCommand.append("Recursive Delete {");
        m_scriptCommand.append(QString("  %1").arg(deleteData.join(' ')));
        m_scriptCommand.append("}");
    }

    void FITKGmshGeoScriptManuallyWrite::addCompoundSurface(QList<int> fListIDs)
    {
        if (fListIDs.isEmpty()) return;
        QStringList idListStr;
        for (int id : fListIDs)
            idListStr.append(QString::number(id));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Compound Surface {%1};").arg(idListStr.join(", ")));
    }

    void FITKGmshGeoScriptManuallyWrite::addCompoundCurve(QList<int> lListIDs)
    {
        if (lListIDs.isEmpty()) return;
        QStringList idListStr;
        for (int id : lListIDs)
            idListStr.append(QString::number(id));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Compound Curve {%1};").arg(idListStr.join(", ")));
    }

    void FITKGmshGeoScriptManuallyWrite::setMeshSize(double size, double min, double max)
    {
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh.CharacteristicLengthFactor = %1;").arg(size));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh.CharacteristicLengthMin = %1;").arg(min));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh.CharacteristicLengthMax = %1;").arg(max));
    }

    void FITKGmshGeoScriptManuallyWrite::setSubdivisionAlgorithm(int type)
    {
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh.SubdivisionAlgorithm = %1;").arg(type));
    }

    void FITKGmshGeoScriptManuallyWrite::setRecombineAll(int type)
    {
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh.RecombineAll = %1;").arg(type));
    }

    void FITKGmshGeoScriptManuallyWrite::setMeshGenerateDim(int dim)
    {
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("SetFactory(\"OpenCASCADE\");"));
        m_scriptCommand.append("//+");
        m_scriptCommand.append(QString("Mesh %1;").arg(dim));
    }

    void FITKGmshGeoScriptManuallyWrite::run()
    {
        //打开文件流
        if (!this->openFileStream()) return;
        QTextStream* stream = this->getStream();
        if (!stream) return;
        //写出脚本命令
        for (QStringList::iterator iter = m_scriptCommand.begin(); iter != m_scriptCommand.end(); ++iter)
            *stream << *iter << endl;
        //关闭文件流
        this->closeFileStream();
    }

    void FITKGmshGeoScriptManuallyWrite::consoleMessage(int level, const QString& str)
    {

    }
}
