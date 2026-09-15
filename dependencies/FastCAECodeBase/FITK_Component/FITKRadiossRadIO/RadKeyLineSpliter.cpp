/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   RadKeyLineSpliter.cpp
 * @brief  RAD关键字分割，处理所有支持的关键字
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-02
 *********************************************************************/

#include "RadKeyLineSpliter.h"

namespace Radioss
{
 
    QString RadKeyLineSpliter::getKeys()
    {
        return _keys;
    }

    void RadKeyLineSpliter::setText(const QString & aline)
    {
        //清空重置
        _line.clear();
        _keys.clear();
        _ids.clear();
        _keyList.clear();
        
        _line = aline;
        //分割行
        splitLine(aline);
    }

    QString RadKeyLineSpliter::getCurrentLine() const
    {
        return _line;
    }

    void RadKeyLineSpliter::splitLine(QString line)
    {
        //以/分隔字符
        if (!line.startsWith("/")) return;
        line = line.simplified();
        QStringList strList = line.split('/', Qt::SkipEmptyParts);
        //分割后的字符串列表,不能往后面放，防止出现算例名称是数字的情况
        for (size_t i = 0; i < strList.size(); i++)
        {
            _keyList.append("/" + strList.at(i));
        }
        
        //解析关键字
        if (strList.empty()) return;
        //从后往前查找数字ID
        while (!strList.isEmpty() )
        {
            QString str = strList.last();
            bool ok = false;
            //尝试将字符串转换为整数
            int id = str.toInt(&ok);
            if (ok && id > 0)
            {
                _ids.insert(0, id);
                strList.removeLast();
            }
            else break;
        }

        if (!strList.isEmpty())
        {
            _keys = "/" + strList.join('/');
        }
    }

    const QString &RadKeyLineSpliter::getKey(int index) const
    {
        if (index < 0 || index >= _keyList.size()) return _keys;
        return _keyList.at(index);
    }

    int RadKeyLineSpliter::getId(int index) const
    {
        if (index < 0 || index >= _ids.size()) return -1;
        return _ids.at(index);
    }

    int RadKeyLineSpliter::getKeyWordCount() const 
    { 
        return _keyList.size(); 
    }

    int RadKeyLineSpliter::getIdCount() const 
    { 
        return _ids.size(); 
    }
}
