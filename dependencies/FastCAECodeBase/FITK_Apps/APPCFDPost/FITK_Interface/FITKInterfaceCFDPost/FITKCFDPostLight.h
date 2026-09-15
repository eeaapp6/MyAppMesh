/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKCFDPostLight.h
 * @brief  后处理灯光数据类
 * @author BaGuijun (baguijun@163.com)
 * @date   2024-10-16
 *********************************************************************/
#ifndef _FITKCFDPostLight_H
#define _FITKCFDPostLight_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostEnum.h"

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

namespace Interface
{
    /**
     * @brief  灯光数据对象
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-16
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostLight : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        FITKCFDPostLight() = default;
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        virtual ~FITKCFDPostLight() = default;
        /**
         * @brief    设置灯光类型
         * @param[i] type                 灯光类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void setLightType(FITKPostLightType type);
        /**
         * @brief    获取灯光类型
         * @return   FITKPostLightType     灯光类型
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        FITKPostLightType getLightType();
        /**
         * @brief    设置颜色RGB
         * @param[i] color      颜色[3](0-1)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void setColor(double* color);
        /**
         * @brief    获取颜色RGB
         * @param[o] color      颜色[3](0-1)
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void getColor(double* color);
        /**
         * @brief    设置灯光位置
         * @param[i] position    灯光位置[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void setPosition(double* position);
        /**
         * @brief    获取灯光位置
         * @param[o] position    灯光位置[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void getPosition(double* position);
        /**
         * @brief    设置灯管焦点
         * @param[i] focalPoint  灯光焦点[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void setFocalPoint(double* focalPoint);
        /**
         * @brief    获取灯光焦点
         * @param[o] focalPoint  灯光焦点[3]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-16
         */
        void getFocalPoint(double* focalPoint);
        /**
         * @brief    设置灯光角度
         * @param[i] value     角度[0 - 90]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        void setConeAngle(double value);
        /**
         * @brief    获取灯光角度
         * @return   double    角度[0 - 90]
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-17
         */
        double getConeAngle();
    private:
        /**
         * @brief  灯光类型
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        FITKPostLightType _lightType = FITKPostLightType::Post_Direction;
        /**
         * @brief  颜色
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        double _color[3] = { 1,1,1 };
        /**
         * @brief  光照位置
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        double _position[3] = { 0,0,1 };
        /**
         * @brief  灯光焦点
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-16
         */
        double _focalPoint[3] = { 0,0,0 };
        /**
         * @brief  灯光角度
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-17
         */
        double _coneAngle = 30;
    };
}
#endif
