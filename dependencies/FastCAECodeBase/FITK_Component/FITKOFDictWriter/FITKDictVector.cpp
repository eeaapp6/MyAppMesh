/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDictVector.h"
#include "FITKDictValue.h"
#include <QTextStream>
#include <QStringList>

//命名空间
namespace DICT
{
    FITKDictVector::FITKDictVector(const FITKDictVector &other)
    {
        this->m_parentheses = other.m_parentheses;
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
    }

    FITKDictVector::~FITKDictVector()
    {
        for (FITKDictValue* value : m_values)
            if (value)delete value;
    }

    FITKDictVector &FITKDictVector::operator=(const FITKDictVector &other)
    {
        this->m_parentheses = other.m_parentheses;
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
        return *this;
    }

    FITKAbstractDict::DictType FITKDictVector::getDictType()
    {
        return FITKAbstractDict::DictType::DictVector;
    }

    void FITKDictVector::setParenthesesType(FITKDictVector::ParenthesesType type)
    {
        m_parentheses = type;
    }

    FITKDictVector::ParenthesesType FITKDictVector::getParenthesesType()
    {
        return m_parentheses;
    }

    int FITKDictVector::count() const
    {
        return m_values.size();
    }

    FITKDictValue* FITKDictVector::at(const int index)
    {
        if (index < 0 || m_values.size() <= index)return nullptr;
        return m_values[index];
    }

    void FITKDictVector::append(FITKAbstractDict::DictType value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(bool value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(double value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(int value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(const QString &value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(const char *value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }
    void FITKDictVector::append(FITKAbstractDict* value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.append(dataValue);
    }

    void FITKDictVector::insert(int index, FITKAbstractDict::DictType value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, bool value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, double value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, int value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, const QString &value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, const char *value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }
    void FITKDictVector::insert(int index, FITKAbstractDict* value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        m_values.insert(index, dataValue);
    }

    void FITKDictVector::removeAt(int i)
    {
        if (i < 0 || m_values.size() <= i)return;
        FITKDictValue* delValue = this->at(i);
        m_values.removeAt(i);
        if (delValue)delete delValue;
    }

    bool FITKDictVector::isAllSimpleType()
    {
        for (FITKDictValue* value : m_values)
        {
            if (!value)continue;
            if (!value->isSimpleType())return false;
        }
        return true;
    }

    void FITKDictVector::writeDict(QTextStream* stream, QString lineStart)
    {
        if (!stream)return;
        QStringList valueList;
        for (QList<FITKDictValue*>::iterator iter = m_values.begin(); iter != m_values.end(); ++iter)
        {
            if (!*iter)continue;
            QString value = (*iter)->getDataKeyEmpty();
            if (value.isEmpty())continue;
            valueList.append(value);
        }
        if (m_parentheses == FITKDictVector::ParenthesesType::SmallParentheses)
            *stream << QString("%1(%2)").arg(lineStart).arg(valueList.join(' ')) << endl;
        else if (m_parentheses == FITKDictVector::ParenthesesType::MiddleParentheses)
            *stream << QString("%1[%2]").arg(lineStart).arg(valueList.join(' ')) << endl;
        else if (m_parentheses == FITKDictVector::ParenthesesType::LargeParentheses)
            *stream << QString("%1{%2}").arg(lineStart).arg(valueList.join(' ')) << endl;
        else if (m_parentheses == FITKDictVector::ParenthesesType::NoneParentheses)
            *stream << QString("%1%2").arg(lineStart).arg(valueList.join(' ')) << endl;
    }
}
