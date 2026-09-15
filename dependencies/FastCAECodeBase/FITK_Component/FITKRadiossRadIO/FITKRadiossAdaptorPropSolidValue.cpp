/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropSolidValue.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropSolid.h"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorPropSolidValue::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropSolidValue";
    }

    bool FITKRadiossAdaptorPropSolidValue::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取solidvalue
        FITKPropSolidValue* solidVal = new FITKPropSolidValue();
        if (solidVal == nullptr) return false;
        
        bool ok = false;
        while (_reader && !_reader->atEnd())
        {
            QString line = _reader->readLine();
            //其他关键字
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            //读取默认实体属性数据
            ok = this->readDefSolid(solidVal);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Solid Value Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Solid Value : %1").arg(keyLine));
        _caseData->getDefaultPropValueManager()->appendDataObj(solidVal);
        return true;
    }

    bool FITKRadiossAdaptorPropSolidValue::adaptW()
    {
        FITKPropSolidValue* solid = dynamic_cast<FITKPropSolidValue*>(_dataObj);
        if (solid == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (stream == nullptr ) return false;

        *stream << solid->GetFITKPropSolidValueRadiossKeyWord() << endl;

        auto gapStr = [&](int a ,int b=10) {
            return QString("%1").arg(a,b);
        };
        // 写出默认实体属性数据
        *stream << QString("#%1%2%3%4%5%6%7").arg("Isolid",9).arg("Ismstr", 10).arg("Icpre", 10).
            arg("Itetra4", 20).arg("Itetra10", 10).arg("Imas", 10).arg("Iframe", 10) << endl;
        *stream << gapStr(solid->getIsolid()) << gapStr(solid->getIsmstr()) << gapStr(solid->getIcpre())
            << gapStr(solid->getItetra4(),20) << gapStr(solid->getItetra10()) << gapStr(solid->getImas()) 
            << gapStr(solid->getIFrame()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorPropSolidValue::readDefSolid(FITKPropSolidValue * solidval)
    {
        if (solidval == nullptr || _reader == nullptr) return false;
        //没有名称，回退一行
        _reader->backLine();
        //参数存储
        QStringList lines;
        while (!_reader->atEnd())
        {
            QString line = _reader->readLine();
            if (line.startsWith("#")) continue;
            if (line.startsWith("/"))
            {
                _reader->backLine();
                break;
            }
            lines.append(line);
        }
        //与手册说明不符
        if (lines.size() != 1)
        {
            _reader->consoleMessage(3, "Invalid Def_Solid : /DEF_SOLID");
            return false;
        }
        //读取默认固体属性参数、赋值
        this->readParam(lines, solidval);
        return true;
    }

    bool FITKRadiossAdaptorPropSolidValue::readParam(const QStringList& lines, FITKPropSolidValue * solidval)
    {
        if (lines.size() != 1 || solidval == nullptr) return false;

        QString line = lines[0];
        QHash<int, int> solidValPar;
        for (int i = 0; i < 8; ++i) 
        {
            bool ok = false;
            int par = line.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok)
                solidValPar.insert(i, par);
        }
        //获取对应参数值，如果不存在则为默认值
        solidval->setIsolid(solidValPar.value(0, solidval->getIsolid()));
        solidval->setIsmstr(solidValPar.value(1, solidval->getIsmstr()));
        solidval->setIcpre(solidValPar.value(2, solidval->getIcpre()));
        solidval->setItetra4(solidValPar.value(4, solidval->getItetra4()));
        solidval->setItetra10(solidValPar.value(5, solidval->getItetra10()));
        solidval->setImas(solidValPar.value(6, solidval->getImas()));
        solidval->setIFrame(solidValPar.value(7, solidval->getIFrame()));
        return true;
    }

}


