/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistoryRBODY.h
 * @brief  输出请求时间历程刚体
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_RBODY_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_RBODY_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程刚体
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistoryRBODY : public FITKAbstractTimeHistory
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKTimeHistoryRBODY)
        RadiossKeyWord(FITKTimeHistoryRBODY, /TH/RBODY)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THRBODYVariableGroup
        {
            VGRBODY_Customize = -1,
            VGRBODY_DEF = 0,       //< FX,FY,FZ,MX,MY,MZ,RX,RY,RZ 
            VGRBODY_F ,            //< FX,FY,FZ
            VGRBODY_M ,            //< MX,MY,MZ
            VGRBODY_R ,            //< RX,RY,RZ
            VGRBODY_FI ,           //< FXI,FYI,FZI
            VGRBODY_MI ,           //< MXI,MYI,MZI
        }; Q_ENUM(THRBODYVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistoryRBODYVariable
        {
            FX,  //< X方向的力
            FY,  //< Y方向的力
            FZ,  //< Z方向的力
            MX,  //< X方向的力矩
            MY,  //< Y方向的力矩
            MZ,  //< Z方向的力矩
            RX,  //< X方向的旋转
            RY,  //< Y方向的旋转
            RZ,  //< Z方向的旋转
            FXI, //< X方向的力
            FYI, //< Y方向的力
            FZI, //< Z方向的力
            MXI, //< X方向的力矩
            MYI, //< Y方向的力矩
            MZI, //< Z方向的力矩
        }; Q_ENUM(TimeHistoryRBODYVariable)

    public:
        explicit FITKTimeHistoryRBODY() = default;
        virtual ~FITKTimeHistoryRBODY();
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
        void setVarlableGroupType(THRBODYVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THRBODYVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THRBODYVariableGroup getVarlableGroupType();
        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistoryRBODYVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistoryRBODYVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistoryRBODYVariable> getCustomizeVariable();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_RBODY_H_RADIOSS