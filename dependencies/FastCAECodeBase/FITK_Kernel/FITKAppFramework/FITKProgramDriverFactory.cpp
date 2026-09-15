/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKProgramDriverFactory.h"

namespace AppFrame
{
    //静态变量初始化
    FITKProgramDriverFactory* FITKProgramDriverFactory::_instance = nullptr;
    QMutex FITKProgramDriverFactory::m_mutex;

    QList<int> FITKProgramDriverFactory::getProgramTypes()
    {
        return _createFuns.uniqueKeys();
    }

    /**
     * @brief 获取指定驱动类型的程序驱动器列表
     *
     * 本函数根据传入的驱动类型，返回对应类型的程序驱动器名称列表。如果给定的驱动类型不存在，
     * 则返回一个空的字符串列表。
     *
     * @param driverType 驱动类型标识
     * @return QStringList 包含指定驱动类型的所有驱动器名称的列表
     */
    QStringList FITKProgramDriverFactory::getProgramDrivers(const int driverType)
    {
        // 检查驱动类型是否存在于创建函数映射中，如果不存在则返回空列表
        if (!_createFuns.contains(driverType)) return QStringList();

        // 获取指定驱动类型的驱动器创建函数集合
        DRIVERTypeFuns ds = _createFuns.value(driverType);

        // 返回指定驱动类型的驱动器名称列表
        return ds.keys();
    }

    /**
     * 根据类型和驱动名称获取驱动创建函数。
     *
     * @param t 驱动类型。
     * @param driver 驱动名称。
     * @return 返回对应的驱动创建函数，如果找不到则返回nullptr。
     */
    std::function<FITKAbstractProgramerDriver*()> FITKProgramDriverFactory::getDriverCreateFun(const int t, const QString driver)
    {
        // 检查驱动类型是否注册，如果没有注册则直接返回nullptr。
        if (!_createFuns.contains(t)) return nullptr;

        // 获取指定驱动类型下的驱动创建函数集合。
        DRIVERTypeFuns ds = _createFuns.value(t);

        // 检查驱动名称是否在创建函数集合中，如果不在则返回nullptr。
        if (!ds.contains(driver)) return nullptr;

        // 返回指定驱动名称对应的驱动创建函数。
        return ds.value(driver);
    }

    void FITKProgramDriverFactory::initialize()
    {

    }

    void FITKProgramDriverFactory::finalize()
    {
        _createFuns.clear();
    }

    FITKProgramDriverFactoryDeleator::~FITKProgramDriverFactoryDeleator()
    {
        if (FITKProgramDriverFactory::_instance)
            delete FITKProgramDriverFactory::_instance;
        FITKProgramDriverFactory::_instance = nullptr;
    }
    static FITKProgramDriverFactoryDeleator _FPDFDELEATOR__;

}


