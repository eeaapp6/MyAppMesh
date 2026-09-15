/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKComponents.h"
#include "FITKComponentInterface.h"

namespace AppFrame
{
    FITKCmponents::~FITKCmponents()
    {
        //遍历回收内存
        for (auto c : _components)
            delete c;
        _components.clear();
    }

    int FITKCmponents::getComponentCount() const
    {
        return _components.size();
    }

    FITKComponentInterface* FITKCmponents::getComponentAt(const int index)
    {
        //索引越界判断
        if (index < 0 || index >= _components.size()) return nullptr;
        return _components.at(index);
    }

    FITKComponentInterface* FITKCmponents::getComponentByName(const QString& compName)
    {
        //遍历判断
        for (auto interf : _components )
        {
            if (!interf)continue;
            const QString name = interf->getComponentName();
            //名称相同
            if (name == compName)
                return interf;
        }
        //没找到同名对象
        return nullptr;
    }

    void FITKCmponents::addComponent(FITKComponentInterface* comp)
    {
        if (comp == nullptr) return;
        //已经存在不在追加
        if (_components.contains(comp)) return;
        const QString name = comp->getComponentName();
        if (this->getComponentByName(name) != nullptr) return;
        
        _components.append(comp);
    
    }

    void FITKCmponents::removeComponent(FITKComponentInterface* comp)
    {
        if (comp == nullptr) return;
        //不包含
        if (!_components.contains(comp)) return;
        _components.removeAll(comp);
        delete comp;
    }

    QList<int> FITKCmponents::getUnResetDataObj()
    {
        QList<int> dataObjs;
        for (FITKComponentInterface* comps : _components)
        {
            if(comps == nullptr) continue;
            dataObjs.append(comps->getUnResetDataObj());
        }
        return dataObjs;
    }

}