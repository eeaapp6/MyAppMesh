/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKDictObject.h"
#include "FITKDictValue.h"
#include <QTextStream>

//命名空间
namespace DICT
{
    FITKDictObject::~FITKDictObject()
    {
        for (FITKDictValue* value : m_values)
            if (value)delete value;
    }

    FITKDictObject::FITKDictObject(const FITKDictObject &other)
    {
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
    }

    FITKDictObject &FITKDictObject::operator=(const FITKDictObject &other)
    {
        for (QList<FITKDictValue*>::const_iterator iter = other.m_values.begin(); iter != other.m_values.end(); ++iter)
        {
            if (!*iter)continue;
            FITKDictValue* value = *iter;
            this->m_values.append(new FITKDictValue(*value));
        }
        return *this;
    }

    int FITKDictObject::count() const
    {
        return m_values.size();
    }

    FITKDictValue* FITKDictObject::value(const QString key)
    {
        for (FITKDictValue* value : m_values)
        {
            if (!value)continue;
            if (value->getKey() == key)
                return value;
        }
        return nullptr;
    }

    FITKDictValue* FITKDictObject::at(const int index)
    {
        if (index < 0 || m_values.size() <= index)return nullptr;
        return m_values[index];
    }

    void FITKDictObject::append(QString key, FITKAbstractDict::DictType value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }

    void FITKDictObject::append(QString key, bool value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, double value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, int value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, const QString &value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, const char *value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, const QStringList &value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }
    void FITKDictObject::append(QString key, FITKAbstractDict* value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.append(dataValue);
    }

    void FITKDictObject::insert(int index, QString key, FITKAbstractDict::DictType value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, bool value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, double value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, int value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, const QString &value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, const char *value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, const QStringList& value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }
    void FITKDictObject::insert(int index, QString key, FITKAbstractDict* value)
    {
        FITKDictValue* dataValue = new FITKDictValue(value);
        dataValue->setKey(key);
        m_values.insert(index, dataValue);
    }

    void FITKDictObject::removeAt(int index)
    {
        if (index < 0 || m_values.size() <= index)return;
        FITKDictValue* delValue = this->at(index);
        m_values.removeAt(index);
        if (delValue)delete delValue;
    }
    
    void FITKDictObject::removeKey(QString key)
    {
        int num = m_values.size();
        for (int i = num - 1; i >= 0; --i)
        {
            FITKDictValue* value = m_values.at(i);
            if (!value)continue;
            if (value->getKey() == key)
            {
                if (value)delete value;
                m_values.removeAt(i);
            }
        }
    }

    void FITKDictObject::writeObjectDict(QTextStream* stream)
    {
        if (!stream)return;
        for (QList<FITKDictValue*>::iterator iter = m_values.begin(); iter != m_values.end(); ++iter)
        {
            if (!*iter)continue;
            (*iter)->writeDict(stream, "");
        }
    }
}
