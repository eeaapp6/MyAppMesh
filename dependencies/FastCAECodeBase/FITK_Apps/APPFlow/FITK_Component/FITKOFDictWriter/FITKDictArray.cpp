/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDictArray.h"
#include "FITKDictValue.h"
#include <QTextStream>
#include <QStringList>

//命名空间
namespace DICT
{
    FITKDictArray::FITKDictArray(const FITKDictArray &other)
    {
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
    }

    FITKDictArray::~FITKDictArray()
    {
        for (FITKDictValue* value : m_values)
            if (value)delete value;
    }

    FITKDictArray &FITKDictArray::operator=(const FITKDictArray &other)
    {
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
        return *this;
    }

    FITKAbstractDict::DictType FITKDictArray::getDictType()
    {
        return FITKAbstractDict::DictType::DictArray;
    }

    int FITKDictArray::count() const
    {
        return m_values.size();
    }

    FITKDictValue* FITKDictArray::at(const int index)
    {
        if (index < 0 || m_values.size() <= index)return nullptr;
        return m_values[index];
    }

    void FITKDictArray::append(FITKAbstractDict::DictType value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(bool value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(double value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(int value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(const QString &value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(const char *value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(const QStringList &value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictArray::append(FITKAbstractDict* value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }

    void FITKDictArray::insert(int index, FITKAbstractDict::DictType value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, bool value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, double value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, int value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, const QString &value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, const QStringList& value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, const char *value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictArray::insert(int index, FITKAbstractDict* value, QString key)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }

    void FITKDictArray::removeAt(int i)
    {
        if (i < 0 || m_values.size() <= i)return;
        FITKDictValue* delValue = this->at(i);
        m_values.removeAt(i);
        if (delValue)delete delValue;
    }

    bool FITKDictArray::isAllSimpleType()
    {
        for (FITKDictValue* value : m_values)
        {
            if (!value)continue;
            if (!value->isSimpleType())return false;
        }
        return true;
    }

    void FITKDictArray::writeDict(QTextStream* stream, QString lineStart)
    {
        if (!stream)return;
        if (m_values.isEmpty())
        {
            *stream << ("\t();") << endl;
            return;
        }
        QString line = lineStart + QString("\t");
        QStringList strList;
        *stream << ("\t(");
        for (QList<FITKDictValue*>::iterator iter = m_values.begin(); iter != m_values.end(); ++iter)
        {
            if (!*iter)continue;
            if (iter == m_values.begin())
            {
                if ((*iter)->getDataKeyEmpty().isEmpty())
                    *stream << endl;
            }
            QString value = (*iter)->getDataKeyEmpty();
            if (value.isEmpty())
            {
                if (!strList.isEmpty())
                {
                    *stream << strList.join(' ') << endl;
                    strList.clear();
                }
                (*iter)->writeDict(stream, line);
                continue;
            }
            strList.append(value);
        }
        if (!strList.isEmpty())
        {
            *stream << strList.join(' ');
            *stream << ");" << endl;
            return;
        }
        *stream << QString("%1);").arg(lineStart) << endl;
    }
}
