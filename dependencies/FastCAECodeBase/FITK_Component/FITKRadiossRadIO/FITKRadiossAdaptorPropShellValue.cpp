/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossAdaptorPropShellValue.h"

#include "FITK_Component/FITKRadiossData/FITKRadiossCase.h"
#include "FITK_Component/FITKRadiossData/FITKPropShell.h"
#include "FITK_Component/FITKRadiossData/FITKPropAbstract.hpp"

#include "FITKRadiossRadReader.h"
#include "FITKRadiossRadWriter.h"
#include "RadKeyLineSpliter.h"

namespace Radioss
{
    QString FITKRadiossAdaptorPropShellValue::getAdaptorClass()
    {
        return "Radioss::FITKRadiossAdaptorPropShellValue";
    }

    bool FITKRadiossAdaptorPropShellValue::adaptR()
    {
        if (_caseData == nullptr || _reader == nullptr) return false;
        
        // 读取关键字
        RadKeyLineSpliter *keyLineSpliter = _reader->getKeyLineSpliter();
        if (!keyLineSpliter) return false;
        int pid = keyLineSpliter->getId();
        QString keyLine = keyLineSpliter->getCurrentLine();
        //获取solidvalue
        FITKPropShellValue* shellVal = new FITKPropShellValue();
        if (shellVal == nullptr) return false;
        
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
            ok = this->readDefShell(shellVal);
        }
        if (!ok)
        {
            _reader->consoleMessage(3, QString("Read Shell Value Error : %1").arg(keyLine));
            return false;
        }
        _reader->consoleMessage(1, QString("Read Shell Value : %1").arg(keyLine));
        _caseData->getDefaultPropValueManager()->appendDataObj(shellVal);
        return true;
    }

    bool FITKRadiossAdaptorPropShellValue::adaptW()
    {
        FITKPropShellValue* shell = dynamic_cast<FITKPropShellValue*>(_dataObj);
        if (shell == nullptr || _writer == nullptr) return false;

        QTextStream* stream = _writer->getStream();
        if (stream == nullptr) return false;

        *stream << shell->GetFITKPropShellValueRadiossKeyWord() << endl;

        auto gapStr = [&](int a,int b=10) {
            return QString("%1").arg(a, b);
        };
        // 写出默认壳属性数据
        *stream << QString("#%1%2%3%4%5%6").arg("Ishell", 9).arg("Ismstr", 10).arg("Ithick", 10).
            arg("Iplas", 10).arg("Ish3n", 40).arg("Idril", 10) << endl;
        *stream << gapStr(shell->getIshell()) << gapStr(shell->getIsmstr()) << gapStr(shell->getIthick())
            << gapStr(shell->getIplas()) << gapStr(shell->getIsh3n(),40) << gapStr(shell->getIdril()) << endl;

        return true;
    }

    bool FITKRadiossAdaptorPropShellValue::readDefShell(FITKPropShellValue * shellval)
    {
        if (shellval == nullptr || _reader == nullptr) return false;
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
            _reader->consoleMessage(3, "Invalid Def_Shell : /DEF_SHELL");
            return false;
        }
        //读取默认固体属性参数、赋值
        this->readParam(lines, shellval);
        return true;
    }

    bool FITKRadiossAdaptorPropShellValue::readParam(const QStringList& lines, FITKPropShellValue * shellval)
    {
        if (lines.size() != 1 || shellval == nullptr) return false;

        QString line = lines[0];
        QHash<int, int> shellValPar;
        for (int i = 0; i < 9; ++i) 
        {
            bool ok = false;
            int par = line.mid(10 * i, 10).trimmed().toInt(&ok);
            if (ok)
                shellValPar.insert(i, par);
        }
        //获取对应参数值，如果不存在则为默认值
        shellval->setIshell(shellValPar.value(0, shellval->getIshell()));
        shellval->setIsmstr(shellValPar.value(1, shellval->getIsmstr()));
        shellval->setIthick(shellValPar.value(2, shellval->getIthick()));
        shellval->setIplas(shellValPar.value(3, shellval->getIplas()));
        shellval->setIsh3n(shellValPar.value(7, shellval->getIsh3n()));
        shellval->setIdril(shellValPar.value(8, shellval->getIdril()));
        
        return true;
    }

}


