/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDictValue.h"
#include "FITKDictObject.h"
#include "FITKDictArray.h"
#include "FITKDictGroup.h"
#include "FITKDictVector.h"
#include <QTextStream>

//命名空间
namespace DICT
{
    FITKDictValue::FITKDictValue(FITKAbstractDict::DictType type)
    {
        m_type = type;
    }

    FITKDictValue::FITKDictValue(bool b)
    {
        m_value_Bool = b;
        m_type = FITKAbstractDict::DictType::DictBool;
    }

    FITKDictValue::FITKDictValue(double n)
    {
        m_value_Double = n;
        m_type = FITKAbstractDict::DictType::DictDouble;
    }

    FITKDictValue::FITKDictValue(int n)
    {
        m_value_Int = n;
        m_type = FITKAbstractDict::DictType::DictInt;
    }

    FITKDictValue::FITKDictValue(const QString &s, bool isInclude)
    {
        m_value_String = new QString(s);
        if (isInclude)
            m_type = FITKAbstractDict::DictType::DictInclude;
        else
            m_type = FITKAbstractDict::DictType::DictString;
    }

    FITKDictValue::FITKDictValue(const char *s)
    {
        m_value_String = new QString(s);
        m_type = FITKAbstractDict::DictType::DictString;
    }

    FITKDictValue::FITKDictValue(const QStringList &s)
    {
        m_value_StrList = new QStringList(s);
        m_type = FITKAbstractDict::DictType::DictStrList;
    }

    FITKDictValue::FITKDictValue(FITKAbstractDict* s)
    {
        m_value_Data = s;
        if (m_value_Data)
            m_type = m_value_Data->getDictType();
    }

    FITKDictValue::~FITKDictValue()
    {
        if (m_type == FITKAbstractDict::DictType::DictString || m_type == FITKAbstractDict::DictType::DictInclude)
            delete m_value_String;
        else if (m_type == FITKAbstractDict::DictType::DictStrList)
            delete m_value_StrList;
        else if (!(m_type == FITKAbstractDict::DictType::DictBool || m_type == FITKAbstractDict::DictType::DictDouble || m_type == FITKAbstractDict::DictType::DictInt || m_type == FITKAbstractDict::DictType::DictNone))
            delete m_value_Data;
    }

    FITKDictValue::FITKDictValue(const FITKDictValue &other)
    {
        m_key = other.m_key;
        m_type = other.m_type;
        if (m_type == FITKAbstractDict::DictType::DictBool)
            m_value_Bool = other.m_value_Bool;
        else if (m_type == FITKAbstractDict::DictType::DictDouble)
            m_value_Double = other.m_value_Double;
        else if (m_type == FITKAbstractDict::DictType::DictInt)
            m_value_Int = other.m_value_Int;
        else if (m_type == FITKAbstractDict::DictType::DictString || m_type == FITKAbstractDict::DictType::DictInclude)
            m_value_String = new QString(*other.m_value_String);
        else if (m_type == FITKAbstractDict::DictType::DictStrList)
            m_value_StrList = new QStringList(*other.m_value_StrList);
        else if (m_type == FITKAbstractDict::DictType::DictArray)
            m_value_Data = new FITKDictArray(*dynamic_cast<FITKDictArray*>(other.m_value_Data));
        else if (m_type == FITKAbstractDict::DictType::DictGroup)
            m_value_Data = new FITKDictGroup(*dynamic_cast<FITKDictGroup*>(other.m_value_Data));
        else if (m_type == FITKAbstractDict::DictType::DictVector)
            m_value_Data = new FITKDictVector(*dynamic_cast<FITKDictVector*>(other.m_value_Data));
    }

    FITKDictValue &FITKDictValue::operator=(const FITKDictValue &other)
    {
        m_key = other.m_key;
        m_type = other.m_type;
        if (m_type == FITKAbstractDict::DictType::DictBool)
            m_value_Bool = other.m_value_Bool;
        else if (m_type == FITKAbstractDict::DictType::DictDouble)
            m_value_Double = other.m_value_Double;
        else if (m_type == FITKAbstractDict::DictType::DictInt)
            m_value_Int = other.m_value_Int;
        else if (m_type == FITKAbstractDict::DictType::DictString)
            m_value_String = new QString(*other.m_value_String);
        else if (m_type == FITKAbstractDict::DictType::DictInclude)
            m_value_String = new QString(*other.m_value_String);
        else if (m_type == FITKAbstractDict::DictType::DictStrList)
            m_value_StrList = new QStringList(*other.m_value_StrList);
        else if (m_type == FITKAbstractDict::DictType::DictArray)
            m_value_Data = new FITKDictArray(*dynamic_cast<FITKDictArray*>(other.m_value_Data));
        else if (m_type == FITKAbstractDict::DictType::DictGroup)
            m_value_Data = new FITKDictGroup(*dynamic_cast<FITKDictGroup*>(other.m_value_Data));
        else if (m_type == FITKAbstractDict::DictType::DictVector)
            m_value_Data = new FITKDictVector(*dynamic_cast<FITKDictVector*>(other.m_value_Data));
        return *this;
    }

    FITKAbstractDict::DictType FITKDictValue::getDictType()
    {
        return m_type;
    }

    void FITKDictValue::setKey(const QString key)
    {
        m_key = key;
    }

    QString FITKDictValue::getKey() const
    {
        return m_key;
    }

    bool FITKDictValue::toBool(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictBool)
            return m_value_Bool;
        ok = false;
        return false;
    }

    double FITKDictValue::toDouble(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictDouble)
            return m_value_Double;
        ok = false;
        return 0.0;
    }

    int FITKDictValue::toInt(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictInt)
            return m_value_Int;
        ok = false;
        return -1;
    }

    QString FITKDictValue::toString(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictString)
            return *m_value_String;
        ok = false;
        return QString();
    }

    QStringList FITKDictValue::toStringList(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictStrList)
            return *m_value_StrList;
        ok = false;
        return QStringList();
    }

    QString FITKDictValue::toInclude(bool & ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictInclude)
            return *m_value_String;
        ok = false;
        return QString();
    }

    FITKDictArray* FITKDictValue::toArray(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictArray)
            return dynamic_cast<FITKDictArray*>(m_value_Data);
        ok = false;
        return nullptr;
    }

    FITKDictGroup* FITKDictValue::toGroup(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictGroup)
            return dynamic_cast<FITKDictGroup*>(m_value_Data);
        ok = false;
        return nullptr;
    }

    FITKDictVector* FITKDictValue::toVector(bool& ok) const
    {
        ok = true;
        if (m_type == FITKAbstractDict::DictType::DictVector)
            return dynamic_cast<FITKDictVector*>(m_value_Data);
        ok = false;
        return nullptr;
    }

    bool FITKDictValue::isNull() const
    {
        if (m_type == FITKAbstractDict::DictType::DictNone)return true;
        return false;
    }

    bool FITKDictValue::isSimpleType() const
    {
        if (m_type == FITKAbstractDict::DictType::DictNone || m_type == FITKAbstractDict::DictType::DictBool || m_type == FITKAbstractDict::DictType::DictDouble || m_type == FITKAbstractDict::DictType::DictInt || m_type == FITKAbstractDict::DictType::DictString
            || m_type == FITKAbstractDict::DictType::DictInclude)
            return true;
        return false;
    }

    void FITKDictValue::writeDict(QTextStream* stream, QString lineStart)
    {
        if (m_type == FITKAbstractDict::DictType::DictNone)
            *stream << endl;
        else if (m_type == FITKAbstractDict::DictType::DictBool)
        {
            if (m_value_Bool)
                *stream << QString("%1%2\t%3;").arg(lineStart).arg(m_key).arg("true") << endl;
            else
                *stream << QString("%1%2\t%3;").arg(lineStart).arg(m_key).arg("false") << endl;
        }
        else if (m_type == FITKAbstractDict::DictType::DictDouble)
        {
            QString value = doubleAdaptationTool(m_value_Double);
            *stream << QString("%1%2\t%3;").arg(lineStart).arg(m_key).arg(value) << endl;
        }
        else if (m_type == FITKAbstractDict::DictType::DictInt)
            *stream << QString("%1%2\t%3;").arg(lineStart).arg(m_key).arg(QString::number(m_value_Int)) << endl;
        else if (m_type == FITKAbstractDict::DictType::DictString)
            *stream << QString("%1%2\t%3;").arg(lineStart).arg(m_key).arg(*m_value_String) << endl;
        else if (m_type == FITKAbstractDict::DictType::DictInclude)
            *stream << QString("%1%2 \"%3\"").arg(lineStart).arg("#inludeEtc").arg(*m_value_String) << endl;
        else if (m_type == FITKAbstractDict::DictType::DictStrList)
        {
            QString value = m_value_StrList->join(' ');
            *stream << QString("%1%2\t%3").arg(lineStart).arg(m_key).arg(value) << endl;
        }
        else
        {
            if (!m_value_Data)return;
            if (!m_key.isEmpty())
                *stream << QString("%1%2").arg(lineStart).arg(m_key);
            m_value_Data->writeDict(stream, lineStart);
        }
    }

    QString FITKDictValue::getDataKeyEmpty()
    {
        if (!m_key.isEmpty())return QString();
        if (m_type == FITKAbstractDict::DictType::DictBool)
        {
            if (m_value_Bool)
                return "true";
            else
                return "false";
        }
        else if (m_type == FITKAbstractDict::DictType::DictDouble)
        {
            QString value = doubleAdaptationTool(m_value_Double);
            return value;
        }
        else if (m_type == FITKAbstractDict::DictType::DictInt)
            return QString::number(m_value_Int);
        else if (m_type == FITKAbstractDict::DictType::DictString)
            return *m_value_String;
        return QString();
    }

    QString FITKDictValue::doubleAdaptationTool(double v)
    {
        QString value = QString::number(v).toUpper();
        if (!value.contains('.'))
        {
            if (!value.contains('E'))
                value += ".0";
            else
                value.insert(value.indexOf('E'), ".0");
        }
        return value;
    }
}
