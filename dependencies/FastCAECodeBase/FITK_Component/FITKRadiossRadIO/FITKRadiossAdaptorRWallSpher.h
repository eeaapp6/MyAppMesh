/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKRadiossAdaptorRWallSpher.h
 * @brief  球面刚性壁读写适配器
 * @author liuzhonghuaAI
 * @date   2025-04-27
 *********************************************************************/
#ifndef __FITKRadiossAdaptorRWallSpher_H__
#define __FITKRadiossAdaptorRWallSpher_H__
#include "FITKRadiossRadIOAPI.h"
#include "FITKRadiossAbstractAdaptor.h"
#include "FITK_Kernel/FITKAdaptor/FITKIOAdaptorFactory.h"

namespace Radioss
{
    class FITKRWallSpherical;
    /**
     * @brief  球面刚性壁读写适配器
     * @author liuzhonghuaAI
     * @date   2025-04-27
     */
    class FITKRadiossRadIOAPI FITKRadiossAdaptorRWallSpher : public FITKRadiossAbstractAdaptor
    {
    public:
        explicit FITKRadiossAdaptorRWallSpher() = default;
        ~FITKRadiossAdaptorRWallSpher() = default;
        /**
         * @brief 获取适配器类名
         * @return 适配器类名字符串
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        QString getAdaptorClass() override;
        /**
         * @brief 适配器读取
         * @return 是否读取成功
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        bool adaptR() override;
        /**
         * @brief 适配器写出
         * @return 是否写出成功
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        bool adaptW() override;

    private:
        /**
         * @brief 读取球面刚性壁数据
         * @param spher 球面墙对象指针
         * @return 是否读取成功
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        bool readRWallSpher(FITKRWallSpherical* spher);
        /**
         * @brief 读取参数
         * @param lines 参数行列表
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        void readParam(const QStringList& lines);
        /**
         * @brief 对象赋值
         * @param spher 球面墙对象指针
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        void setData(FITKRWallSpherical* spher);

    private:
        /**
         * @brief 属性参数
         * @author liuzhonghuaAI
         * @date   2025-04-27
         */
        QHash<int, double> _param{};
    };
    /**
     * @brief  注册适配器
     * @author liuzhonghuaAI
     * @date   2025-04-27
     */
    Register2FITKIOAdaptorFactory(rad, Radioss::FITKRWallSpherical, FITKRadiossAdaptorRWallSpher);
    RegRadiossRadAdaptor(/RWALL/SPHER, FITKRadiossAdaptorRWallSpher);
}
#endif //!__FITKRadiossAdaptorRWallSpher_H__