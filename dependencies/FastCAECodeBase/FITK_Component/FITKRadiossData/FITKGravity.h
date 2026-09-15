/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITK_GRAVITY_H__
#define FITK_GRAVITY_H__

#include "FITKAbstractRadiossData.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

namespace Radioss
{
    /**
     * @brief 重力场数据对象。
     * @details 表示一个重力场，包括重力加速度和方向。
     * @author libaojun
     * @date 2025-08-25
     */
    class FITKRadiossDataAPI FITKGravity : public Core::FITKAbstractNDataObject,
                      public FITKAbstractRadiossData
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKGravity);
        RadiossKeyWord(FITKGravity, /GRAV);
    public:
        enum GravityDirection
        {
            GRAVITY_X = 1,   ///< X 方向。
            GRAVITY_Y,       ///< Y 方向。
            GRAVITY_Z,       ///< Z 方向。
        }; Q_ENUM(GravityDirection)

    public:
        /**
         * @brief 默认构造函数。
         * @author libaojun
         * @date 2025-08-25
         */
        explicit FITKGravity() = default;

        /**
         * @brief 析构函数。
         * @author libaojun
         * @date 2025-08-25
         */
        virtual ~FITKGravity() = default;
 
        /**
         * @brief 获取时间函数 ID。
         * @return 时间函数 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        int getTimeFunctionID() const;
        
        /**
         * @brief 设置时间函数 ID。
         * @param id 时间函数 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        void setTimeFunctionID(int id);
        
        /**
         * @brief 获取重力方向。
         * @return 重力方向枚举值。
         * @author libaojun
         * @date 2025-08-25
         */
        GravityDirection getGravityDirection() const;
        
        /**
         * @brief 设置重力方向。
         * @param direction 重力方向枚举值。
         * @author libaojun
         * @date 2025-08-25
         */
        void setGravityDirection(GravityDirection direction);
        
        /**
         * @brief 获取坐标系 ID。
         * @return 坐标系 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        int getSystemID() const;
        
        /**
         * @brief 设置坐标系 ID。
         * @param id 坐标系 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        void setSystemID(int id);
        
        /**
         * @brief 获取传感器 ID。
         * @return 传感器 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        int getSensorID() const;
        
        /**
         * @brief 设置传感器 ID。
         * @param id 传感器 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        void setSensorID(int id);
        
        /**
         * @brief 获取节点组 ID。
         * @return 节点组 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        int getNodeGroupID() const;
        
        /**
         * @brief 设置节点组 ID。
         * @param id 节点组 ID。
         * @author libaojun
         * @date 2025-08-25
         */
        void setNodeGroupID(int id);
        
        /**
         * @brief 获取 X 方向缩放因子。
         * @return X 方向缩放因子。
         * @author libaojun
         * @date 2025-08-25
         */
        double getAScaleX() const;
        
        /**
         * @brief 设置 X 方向缩放因子。
         * @param scale X 方向缩放因子。
         * @author libaojun
         * @date 2025-08-25
         */
        void setAScaleX(double scale);
        
        /**
         * @brief 获取 Y 方向缩放因子。
         * @return Y 方向缩放因子。
         * @author libaojun
         * @date 2025-08-25
         */
        double getFScaleY() const;
        
        /**
         * @brief 设置 Y 方向缩放因子。
         * @param scale Y 方向缩放因子。
         * @author libaojun
         * @date 2025-08-25
         */
        void setFScaleY(double scale);

    private:
        int _timeFunctionID{0}; //!< 时间函数
        GravityDirection _gravityDirection{GRAVITY_Z}; //!< 重力方向
        int _coordinateSystemID{0}; //!< 坐标系 ID
        int _sensorID{0}; //!< 传感器 ID
        int _nodeGroupID{0}; //!< 节点组 ID
        double _AScaleX{1.0}; //!< 缩放因子
        double _FScaleY{1.0}; //!< 缩放因子
    };

    class FITKRadiossDataAPI FITKGravityManager :
        public Core::FITKAbstractDataManager<FITKGravity>
    {
    public:
        explicit FITKGravityManager() = default;
        virtual ~FITKGravityManager() = default;
    };

} // namespace Radioss

#endif // FITK_GRAVITY_H__
