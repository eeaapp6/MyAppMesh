/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistorySECTIO.h
 * @brief  输出请求时间历程截面力
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_SECTIO_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_SECTIO_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程截面力
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistorySECTIO : public FITKAbstractTimeHistory
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKTimeHistorySECTIO)
        RadiossKeyWord(FITKTimeHistorySECTIO, /TH/SECTIO)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THSECTIOVariableGroup
        {
            VGSECTIO_Customize = -1,
            VGSECTIO_DEF = 0,       //< FNX, FNY, FNZ, FTX, FTY, FTZ, M1, M2, M3
            VGSECTIO_FN ,           //< FNX, FNY, FNZ
            VGSECTIO_FT ,           //< FTX, FTY, FTZ
            VGSECTIO_M ,            //< M1, M2, M3
            VGSECTIO_CENTER ,       //< CX, CY, CZ
            VGSECTIO_GLOBAL ,       //< FNX, FNY, FNZ, FTX, FTY, FTZ, MX, MY, MZ
            VGSECTIO_LOCAL ,        //< F1, F2, F3, M1, M2, M3
        }; Q_ENUM(THSECTIOVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistorySECTIOVariable
        {
            FNX, //< 全局X方向的法向力
            FNY, //< 全局Y方向的法向力
            FNZ, //< 全局Z方向的法向力
            FTX, //< 全局X方向的切向力
            FTY, //< 全局Y方向的切向力
            FTZ, //< 全局Z方向的切向力
            MX,  //< 全局X方向的力矩
            MY,  //< 全局Y方向的力矩
            MZ,  //< 全局Z方向的力矩
            F1,  //< 局部方向1的力分量
            F2,  //< 局部方向2的力分量
            F3,  //< 局部方向3的力分量
            M1,  //< 局部方向1的力矩分量
            M2,  //< 局部方向2的力矩分量
            M3,  //< 局部方向3的力矩分量
            CX,  //< 全局方向X上的截面中心坐标
            CY,  //< 全局方向Y上的截面中心坐标
            CZ,  //< 全局方向Z上的截面中心坐标
            WORK,
            WORKR,
            DFX,
            DFY,
            DFZ,
            DMX,
            DMY,
            DMZ,
        }; Q_ENUM(TimeHistorySECTIOVariable)

    public:
        explicit FITKTimeHistorySECTIO() = default;
        virtual ~FITKTimeHistorySECTIO();
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
        void setVarlableGroupType(THSECTIOVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THSECTIOVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THSECTIOVariableGroup getVarlableGroupType();
        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistorySECTIOVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistorySECTIOVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistorySECTIOVariable> getCustomizeVariable();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_SECTIO_H_RADIOSS