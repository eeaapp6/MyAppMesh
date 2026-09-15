/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   RadReadInformationMapper.h
 * @brief  rad读取信息映射表-用于读完的信息整合
 * @author zhangjingx (hiter25@outlook.com)
 * @date   2025-07-02
 *********************************************************************/
#ifndef _RADREADINFORMATIONMAPPER_H_
#define _RADREADINFORMATIONMAPPER_H_

#include <QMultiHash>
#include <QPair>
#include <QStringList>

namespace Radioss
{
    class FITKRadiossCase;
    class FITKRadiossPart;
    /**
     * @brief  rad读取信息映射表
     * @author zhangjingx (hiter25@outlook.com)
     * @date   2025-07-02
     */
    class RadReadInformationMapper
    {
    public:
        explicit  RadReadInformationMapper() = default;
        virtual ~RadReadInformationMapper() = default;
        
        /**
         * @brief    设置算例数据对象
         * @param[i] caseData 算例数据对象
         * @return   void
         * @author   libaojun
         * @date     2025-07-10
         */
        void setCaseData(FITKRadiossCase* caseData);

        /**
         * @brief    进行信息映射
         * @return   void
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-02
         */
        void map();
        /**
         * @brief    通过rad文件id获取part对象，如果不存在则new
         * @param[i] id rad文件id
         * @return   FITKRadiossPart * part对象
         * @author   zhangjingx (hiter25@outlook.com)
         * @date     2025-07-10
         */
        FITKRadiossPart* getRadiossPartByFilePartID(int id,bool isCreate=true);
        /**
         * @brief 插入材料ID映射
         * @param[i]  radID          radioss中标记的id
         * @param[i]  materialID     材料ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-07-10
         */
        void insertMaterialIDMap(const int radID, const int materialID);
        /**
         * @brief  通过radID获取材料ID
         * @param  radID radioss中标记的id
         * @return 材料ID; 未找到返回 -1
         */
        int getMaterialID(const int radID);
        /**
         * @brief  插入属性ID映射
         * @param  radID       radioss中标记的id
         * @param  propertyID  属性ID
         */
        void insertPropertyIDMap(const int radID, const int propertyID);
        /**
         * @brief  通过radID获取属性ID
         * @param  radID radioss中标记的id
         * @return 属性ID; 未找到返回 -1
         */
        int getPropertyID(const int radID);

        /**
         * @brief  插入节点组ID映射 (实现已移至 cpp)
         * @param  radID       radioss中标记的id
         * @param  nodeGroupID 节点组ID
         */
        void insertNodeGroupIDMap(const int radID, const int nodeGroupID);

        /**
         * @brief  通过radID获取节点组ID (实现已移至 cpp)
         * @param  radID radioss中标记的id
         * @return 节点组ID; 未找到返回 -1
         */
        int getNodeGroupIDByRadID(int radID) const;

        /**
         * @brief  radID->节点ID 
         */
        void insertNodeIDMap(const int radID, const int nodeID);
        int getNodeIDByRadID(int radID) const;

        /**
         * @brief  radID->元素(单元)ID
         */
        void insertEleIDMap(const int radID, const int eleID);
        int getEleIDByRadID(int eleID) const;

        /**
         * @brief  radID->子集(part的父集)ID
         */
        void insertSubsetIDMap(const int radID, const int subsetID);
        int getSubsetIDByRadID(int subsetID) const;

        /**
        * @brief  radID->曲线ID
        */
        void insertCurveIDMap(const int radID, const int curveID);
        int getCurveIDByRadID(int curveID) const;

        /**
        * @brief  radID->Surface ID
        */
        void insertSurfaceSetIDMap(const int radID, const int surfaceID);
        int getSurfaceSetIDByRadID(int surfaceID) const;

        /**
        * @brief  radID->Interaction ID
        */
        void insertInteractionIDMap(const int radID, const int interactionID);
        int getInteractionIDByRadID(int interactionID) const;

       /**
        * @brief  radID-> 连接ID
        */
        void insertConnectionIDMap(const int radID, const int connectionID);
        int getConnectionIDByRadID(int connectionID) const;

        /**
         * @brief  radID-> 元素组ID
         */
        void insertElementGroupIDMap(const int radID, const int elementGroupID);
        int getElementGroupIDByRadID(int elementGroupID) const;

        /**
        * @brief  radID-> 探针（加速度计）ID
        */
        void insertProbeAccelIDMap(const int radID, const int probeID);
        int getProbeAccelIDByRadID(int probeID) const;

        /**
        * @brief  radID-> 探针（截面）ID
        */
        void insertProbeSectionIDMap(const int radID, const int probeID);
        int getProbeSectionIDByRadID(int probeID) const;

        /**
        * @brief  radID-> 刚性壁ID
        */
        void insertRWallIDMap(const int radID, const int rwallID);
        int getRWallIDByRadID(int rwallID) const;

        /**
        * @brief  radID-> 状态方程ID
        */
        //void insertEOSIDMap(const int radID, const int eosID);
        //int getEOSIDByRadID(int eosID) const;

        /**
        * @brief  radID-> 失效ID
        */
        void insertFailureIDMap(const int radID, const int failureID);
        int getFailureIDByRadID(int failureID) const;

        /**
        * @brief  radID-> BEM ID
        */
        void insertBEMIDMap(const int radID, const int bemID);
        int getBEMIDByRadID(int bemID) const;

        /**
        * @brief  radID-> Grav ID
        */
        void insertGravIDMap(const int radID, const int gravID);
        int getGravIDByRadID(int gravID) const;

    private:
        /**
         * @brief  算例数据对象
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-02
         */
        FITKRadiossCase* _caseData{};
        /**
         * @brief  partID映射表 rad文件中的ID -> partID
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-11
         */
        QHash<int, int> _partIDMap;
        /**
         * @brief  材料ID映射表 rad文件中的ID -> materialID
         * @author zhangjingx (hiter25@outlook.com)
         * @date   2025-07-11
         */
        QHash<int, int> _materialIDMap;
        /**
         * @brief  属性ID映射表 rad文件中的ID -> propertyID
         * @author libaojun
         * @date   2025-07-11
         */
        QHash<int, int> _propertyIDMap;
        /**
         * @brief  节点组ID映射表 rad文件中的ID -> 节点组ID
         * @author libaojun
         * @date   2025-08-20
         */
        QHash<int,int> _nodeGroupIDMap; 
        /**
        * @brief  节点ID映射表 rad文件中的ID -> 节点ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _nodeIDMap;
        /**
        * @brief  单元ID映射表 rad文件中的ID -> 单元ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _eleIDMap;
        /**
        * @brief  子集(part的父集)ID映射表 rad文件中的ID -> 子集ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _subsetIDMap;
        /**
        * @brief  曲线ID映射表 rad文件中的ID -> 子集ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _curveIDMap;

        /**
        * @brief  面ID映射表 rad文件中的ID -> 子集ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _surfaceIDMap;

        /**
         * @brief  相互作用ID映射表 rad文件中的ID -> 相互作用ID
         * @author libaojun
         * @date   2025-08-20
         */
        QHash<int, int> _interactionIDMap;

        /**
        * @brief  连接ID映射表 rad文件中的ID -> 连接ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _connectionIDMap;

        /**
        * @brief  元素组ID映射表 rad文件中的ID -> 元素组ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _elementGroupID;

        /**
        * @brief  探针（加速度计）ID映射表 rad文件中的ID -> 探针ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _probeAccelID;

        /**
        * @brief  探针（截面）ID映射表 rad文件中的ID -> 探针ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _probeSectionID;

        /**
        * @brief 刚性壁ID映射表 rad文件中的ID -> 刚性壁ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _rWallID;

        /**
        * @brief 状态方程ID映射表 rad文件中的ID -> eosID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _eosID;

        /**
        * @brief 失效ID映射表 rad文件中的ID -> 失效ID
        * @author libaojun
        * @date   2025-08-20
        */
        QHash<int, int> _failureID;

        /**
        * @brief  BEM ID映射表 rad文件中的ID -> BEM ID
        * @author liuzhonghua
        * @date   2025-04-27
        */
        QHash<int, int> _bemID;
        /**
         * @brief  Grav ID映射表 rad文件中的ID -> Grav ID
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2026-04-27
         */
        QHash<int, int> _gravID;
    };
}
#endif // !_RADREADINFORMATIONMAPPER_H_
    