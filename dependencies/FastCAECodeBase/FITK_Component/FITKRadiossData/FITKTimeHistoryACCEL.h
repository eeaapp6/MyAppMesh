/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistoryACCEL.h
 * @brief  输出请求时间历程加速度计
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_ACCEL_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_ACCEL_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程加速度计
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistoryACCEL : public FITKAbstractTimeHistory
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKTimeHistoryACCEL)
        RadiossKeyWord(FITKTimeHistoryACCEL, /TH/ACCEL)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THACCELVariableGroup
        {
            VGACCEL_Customize=-1,
            VGACCEL_DEF = 0,       //< AX,AY,AZ
            VGACCEL_W,             //< WX,WY,WZ 
        }; Q_ENUM(THACCELVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistoryACCELVariable
        {
            AX, //< X方向的加速度
            AY, //< Y方向的加速度
            AZ, //< Z方向的加速度
            WX, //< X方向的加速度积分
            WY, //< Y方向的加速度积分
            WZ, //< Z方向的加速度积分
        }; Q_ENUM(TimeHistoryACCELVariable)

    public:
        explicit FITKTimeHistoryACCEL() = default;
        virtual ~FITKTimeHistoryACCEL();
        /**
         * @brief    获取时间历程类型
         * @return   TimeHistoryType
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        virtual TimeHistoryType getTimeHistoryType() override;
        /**
         * @brief    设置变量组类型
         * @param[i] type 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setVarlableGroupType(THACCELVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THACCELVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THACCELVariableGroup getVarlableGroupType();

        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistoryACCELVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistoryACCELVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistoryACCELVariable> getCustomizeVariable();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_ACCEL_H_RADIOSS