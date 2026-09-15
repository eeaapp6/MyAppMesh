/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistoryINTER.h
 * @brief  输出请求时间历程接口
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-09-24
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_INTER_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_INTER_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程接口
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistoryINTER : public FITKAbstractTimeHistory
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKTimeHistoryINTER)
        RadiossKeyWord(FITKTimeHistoryINTER, /TH/INTER)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THINTERVariableGroup
        {
            VGINTER_Customize = -1,
            VGINTER_DEF = 0,        //< FNX, FNY, FNZ, FTX, FTY, FTZ
            VGINTER_FN,             //< FNX, FNY, FNZ
            VGINTER_FT ,            //< FTX, FTY, FTZ
            VGINTER_AbsoluteFN ,    //< |FNX|, |FNY|, |FNZ|,||FN||
            VGINTER_AbsoluteF ,     //< |FX|, |FY|, |FZ|,||F||
        }; Q_ENUM(THINTERVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistoryINTERVariable
        {
            FNX,         //< X方向的法向力
            FNY,         //< Y方向的法向力
            FNZ,         //< Z方向的法向力
            FTX,         //< X方向的切向力
            FTY,         //< Y方向的切向力
            FTZ,         //< Z方向的切向力
            SFW,         //仅适用于TYPE14和TYPE15
            MX,          //< 全局坐标系的X方向的力矩
            MY,          //< 全局坐标系的Y方向的力矩
            MZ,          //< 全局坐标系的Z方向的力矩

            _FNX_, //< 接触力X方向的法向力的绝对值
            _FNY_, //< 接触力Y方向的法向力的绝对值
            _FNZ_, //< 接触力Z方向的法向力的绝对值
            __FN__,
            _FX_,  //< 接触力X方向的分量绝对值
            _FY_,  //< 接触力Y方向的分量绝对值
            _FZ_,  //< 接触力Z方向的分量绝对值
            __F__,
        }; Q_ENUM(TimeHistoryINTERVariable)

    public:
        explicit FITKTimeHistoryINTER() = default;
        virtual ~FITKTimeHistoryINTER();
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
        void setVarlableGroupType(THINTERVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THINTERVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THINTERVariableGroup getVarlableGroupType();

        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistoryINTERVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistoryINTERVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistoryINTERVariable> getCustomizeVariable();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_INTER_H_RADIOSS