/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file GlobalDataFactory.h
 * @brief 全局数据工厂
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-04
 */
#ifndef __GLOBALDATAFACTORY__
#define __GLOBALDATAFACTORY__

#include "FITK_Kernel/FITKAppFramework/FITKAbstractGlobalDataFactory.h"
/**
 * @brief 全局数据工厂类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-09-04
 */
class GlobalDataFactory : public AppFrame::FITKAbstractGlobalDataFactory
{
private:
    /**
     * @brief 创建后处理数据
     * @return 后处理场数据
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-19
     */
    virtual Core::FITKAbstractDataObject *createPostData() override;
    /**
     * @brief 创建其他数据
     * @return 其他数据
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date 2024-04-19
     */
    virtual QHash<int, Core::FITKAbstractDataObject *> createOtherData() override;
};

#endif
