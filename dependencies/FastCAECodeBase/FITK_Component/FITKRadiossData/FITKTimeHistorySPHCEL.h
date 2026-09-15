/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKTimeHistorySPHCEL.h
 * @brief  输出请求时间历程SPH粒子
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-07
 *********************************************************************/
#ifndef FITK_ABSTRACT_TIMEHISTORY_SPHCEL_H_RADIOSS
#define FITK_ABSTRACT_TIMEHISTORY_SPHCEL_H_RADIOSS

#include "FITKAbstractTimeHistory.h"

namespace Radioss
{
    /**
     * @brief  输出请求时间历程SPH粒子
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-09-24
     */
    class FITKRadiossDataAPI FITKTimeHistorySPHCEL : public FITKAbstractTimeHistory
    {
        Q_OBJECT
        FITKCLASS(Radioss, FITKTimeHistorySPHCEL)
        RadiossKeyWord(FITKTimeHistorySPHCEL, /TH/SPHCEL)
    public:
        /**
         * @brief  变量组
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum THSPHCELVariableGroup
        {
            VGSPHCEL_Customize = -1,
            VGSPHCEL_ALL = 0,       //所有变量
            VGSPHCEL_DEF =1,       //< OFF, SX, SY, SZ, SXY, SYZ, SXZ, IE, DENS, PLAS, TEMP
        }; Q_ENUM(THSPHCELVariableGroup)
        /**
         * @brief  变量
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-09-24
         */
        enum TimeHistorySPHCELVariable
        {
           /*所有材料都可以使用的输出变量*/
            OFF,      //< 元素
            SX,       //< 沿方向X上的应力
            SY,       //< 沿方向Y上的应力
            SZ,       //< 沿方向Z上的应力
            SXY,      //< 沿方向XY上的应力
            SYZ,      //< 沿方向YZ上的应力
            SXZ,      //< 沿方向XZ上的应力
            IE,       //< 体积的比内能
            DENS,     //< 密度
            VOL,      //< 体积
            WFVIS,    //< 粘性力所做的功
            DIAMETER, //< 粒子直径，平滑长度

           /*特定材料才能使用的输出变量*/
            TEMP,     //温度
            PLAS,     //等效塑性应变
            PLSR,     //塑性应变率
            VPLA,     //等效体积塑性应变

        }; Q_ENUM(TimeHistorySPHCELVariable)

    public:
        explicit FITKTimeHistorySPHCEL() = default;
        virtual ~FITKTimeHistorySPHCEL();
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
        void setVarlableGroupType(THSPHCELVariableGroup type);
        /**
         * @brief    获取变量组类型
         * @return   THSPHCELVariableGroup
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        THSPHCELVariableGroup getVarlableGroupType();
        /**
         * @brief    设置自定义变量数据
         * @param[1] variables 变量
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        void setCustomizeVariable(QList<TimeHistorySPHCELVariable> variables);
        /**
         * @brief    获取自定义变量数据
         * @return   QList<TimeHistorySECTIOVariable>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-24
         */
        QList<TimeHistorySPHCELVariable> getCustomizeVariable();
        /**
         * @brief    设置SPH粒子节点ID
         * @param[i] id 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setSPHNodeID(int id);
        /**
         * @brief    获取SPH粒子节点ID
         * @return   int
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        int getSPHNodeID();
        /**
         * @brief    设置SPH粒子名称
         * @param[i] name 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        void setSPHNodeName(QString name);
        /**
         * @brief    获取SPH粒子名称
         * @return   QString
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-07
         */
        QString getSPHNodeName();
        /**
         * @brief   获取变量数据字符串
         * @param[i] isRemoveInclude 是否剔除包含关系的字符串
         */
        virtual QStringList getVariableKeyWords(bool isRemoveInclude = true);

    private:
        /**
         * @brief  粒子节点ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        int m_nodeID{ 0 };
        /**
         * @brief  粒子名称
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-07
         */
        QString m_nodeName{};
    };

} // namespace Radioss

#endif // FITK_ABSTRACT_TIMEHISTORY_SECTIO_H_RADIOSS