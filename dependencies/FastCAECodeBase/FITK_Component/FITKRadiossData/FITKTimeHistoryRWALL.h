/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistoryRWALL.h
 * @brief  输出请求时间历程刚性墙
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_RWALL_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_RWALL_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程刚性墙
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistoryRWALL : public FITKAbstractTimeHistory
    {
        Q_OBJECT
            FITKCLASS(Radioss, FITKTimeHistoryRWALL)
            RadiossKeyWord(FITKTimeHistoryRWALL, /TH/RWALL)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THRWALLVariableGroup
        {
            VGRWALL_Customize = -1,
            VGRWALL_DEF = 0,       //< FNX, FNY, FNZ, FTX, FTY, FTZ
            VGRWALL_FN ,           //< FNX, FNY, FNZ
            VGRWALL_FT ,           //< FTX, FTY, FTZ
        }; Q_ENUM(THRWALLVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistoryRWALLVariable
        {
            FNX, //< 沿方向X上的法向力
            FNY, //< 沿方向Y上的法向力
            FNZ, //< 沿方向Z上的法向力
            FTX, //< 沿方向X上的切向力
            FTY, //< 沿方向Y上的切向力
            FTZ, //< 沿方向Z上的切向力
        }; Q_ENUM(TimeHistoryRWALLVariable)

    public:
        explicit FITKTimeHistoryRWALL() = default;
        virtual ~FITKTimeHistoryRWALL();
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
        void setVarlableGroupType(THRWALLVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THRWALLVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THRWALLVariableGroup getVarlableGroupType();
        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistoryRWALLVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistoryRWALLVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistoryRWALLVariable> getCustomizeVariable();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_RWALL_H_RADIOSS