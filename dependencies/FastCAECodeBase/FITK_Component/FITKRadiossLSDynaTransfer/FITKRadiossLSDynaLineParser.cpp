/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKRadiossLSDynaLineParser.h"
#include <QString>
#include <QStringList>
#include <QLocale>

namespace Radioss
{
    QString FITKRadiossLSDynaLineParser::parseKeyword(const QString& line)
    {
        // 解析关键字行
        QString trimmed = line.trimmed();
        if (trimmed.isEmpty())
        {
            return QString();
        }

        if (trimmed.startsWith('*'))
        {
            return trimmed.toUpper();
        }

        return QString();
    }

    QStringList FITKRadiossLSDynaLineParser::parseDataLine(const QString& line, int numFields)
    {
        // 解析数据行
        QString trimmedLine = line.trimmed();

        if (trimmedLine.isEmpty())
        {
            return QStringList();
        }

        if (trimmedLine.startsWith('$') || trimmedLine.startsWith('*'))
        {
            return QStringList();
        }

        QStringList fields = trimmedLine.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        return fields;
    }

    bool FITKRadiossLSDynaLineParser::isCommentLine(const QString& line)
    {
        //跳过注释行（$开头）
        QString trimmed = line.trimmed();
        return trimmed.startsWith('$');
    }

    bool FITKRadiossLSDynaLineParser::isEmptyLine(const QString& line)
    {
        // 是否为空行
        return line.trimmed().isEmpty();
    }

    bool FITKRadiossLSDynaLineParser::isBlockStart(const QString& keyword)
    {
        // 关键字块检测（*KEYWORD 开始）
        if (keyword.startsWith("*KEYWORD"))
        {
            return true;
        }

        return false;
    }

    bool FITKRadiossLSDynaLineParser::isBlockEnd(const QString& line)
    {
        // 关键字块结束检测（*END）
        QString trimmed = line.trimmed().toUpper();
        if (trimmed == "*END")
        {
            return true;
        }

        return false;
    }

    QString FITKRadiossLSDynaLineParser::normalizeKeyword(const QString& keyword)
    {
        // 标准化关键字（转为大写）
        return keyword.trimmed().toUpper();
    }

    double FITKRadiossLSDynaLineParser::parseFieldValue(const QString& fieldStr)
    {
        // 解析字段值（double）
        QString trimmed = fieldStr.trimmed();
        if (trimmed.isEmpty())
        {
            return 0.0;
        }

        bool ok = false;
        double value = trimmed.toDouble(&ok);
        if (!ok)
        {
            value = QLocale::c().toDouble(trimmed, &ok);
        }

        return ok ? value : 0.0;
    }

    int FITKRadiossLSDynaLineParser::parseFieldIntValue(const QString& fieldStr)
    {
        // 解析字段值（int）
        QString trimmed = fieldStr.trimmed();
        if (trimmed.isEmpty())
        {
            return 0;
        }

        bool ok = false;
        int value = trimmed.toInt(&ok);
        if (!ok)
        {
            value = trimmed.toLongLong(&ok);
        }

        return ok ? value : 0;
    }
}
