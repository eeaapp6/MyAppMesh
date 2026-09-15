/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKAbstractConnection.h
 * @brief  连接的基类
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef __FITKABSTRACT_CONNECTION_H__
#define __FITKABSTRACT_CONNECTION_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
     /**
      * @brief  连接数据基类
      * @author liuzhonghua (liuzhonghuaszch@163.com)
      * @date   2025-09-24
      */
    class FITKRadiossDataAPI FITKAbstractConnection : public Core::FITKAbstractNDataObject,
        public FITKAbstractRadiossData
    {
        Q_OBJECT
    public:
        enum ConnectionType
        {
            Connection_Unknown = 0,
            Connection_RBODY = 1,
        };
        Q_ENUM(ConnectionType);

    public:
        explicit FITKAbstractConnection() =default;
        virtual ~FITKAbstractConnection() =0;
        /**
         * @brief    获取类型
         * @return   ConnectionType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        virtual ConnectionType getType() const =0;

    private:
    };
    /**
     * @brief  连接数据管理器
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKConnectionManager :
        public Core::FITKAbstractDataManager<FITKAbstractConnection>
    {
    public:
        explicit FITKConnectionManager() = default;
        virtual ~FITKConnectionManager() = default;
    };


}


#endif