/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef ___RADWRITEINFORMATIONMAPPER_H_
#define ___RADWRITEINFORMATIONMAPPER_H_

/*********************************************************************
 * @file   RadWriteInformationMapper.h
 * @brief  rad写入信息映射表-用于写入前的信息整合
 * @author libaojun 
 * @date   2025-08-11
 *********************************************************************/
#include <QHash>

namespace Radioss
{
    class FITKRadiossCase;
    /**
     * @brief  rad写入信息映射表
     * @author libaojun 
     * @date   2025-08-11
     */
    class RadWriteInformationMapper
    {
    public:
        explicit  RadWriteInformationMapper() = default;
        virtual ~RadWriteInformationMapper() = default;

        //材料
        void insertMaterialIDMap( const int materialID, const int radID);
        int getRadIDByMaterialID(const int materialID);
        int getMaxRadMaterialID();

        //属性
        void insertPropertyIDMap(const int propertyID, const int radID);
        int getRadIDByPropertyID(const int propertyID);
        int getMaxRadPropertyID();

        //曲线
        void insertCurveIDMap(const int curveID, const int radID);
        int getRadIDByCurveID(const int curveID);
        int getMaxRadCurveID();

        //刚性墙
        void insertRWallIDMap(const int rWallID, const int radID);
        int getRadIDByRWallID(const int rWallID);
        int getMaxRadRWallID();

        //重力
        void insertGravityIDMap(const int gravityID, const int radID);
        int getRadIDByGravityID(const int gravityID);
        int getMaxRadGravityID();

        //节点组
        void insertNodeSetIDMap(const int setID, const int radID);
        int getRadIDByNodeSetID(const int setID);
        int getMaxRadNodeSetID();

        //元素组
        void insertEleSetIDMap(const int setID, const int radID);
        int getRadIDByEleSetID(const int setID);
        int getMaxRadEleSetID();

        //初始场
        void insertInitialFieldIDMap(const int initialFieldID, const int radID);
        int getRadIDByInitialFieldID(const int initialFieldID);
        int getMaxRadInitialFieldID();

        //边界
        void insertBcIDMap(const int bcID, const int radID);
        int getRadIDByBcID(const int bcID);
        int getMaxRadBcID();

        //Part
        void insertPartIDMap(const int partID, const int radID);
        int getRadIDByPartID(const int partID);
        int getMaxRadPartID();

        //subset
        void insertSubsetIDMap(const int subsetID, const int radID);
        int getRadIDBySubsetID(const int subsetID);
        int getMaxRadSubsetID();

        //surface
        void insertSurfaceIDMap(const int surfaceID, const int radID);
        int getRadIDBySurfaceID(const int surfaceID);
        int getMaxRadSurfaceID();
        int getNextSegSurfaceID();

        //相互作用
        void insertInteractionIDMap(const int interactionID, const int radID);
        int getRadIDByInteractionID(const int interactionID);
        int getMaxRadInteractionID();

        //探针（加速度计）
        void insertAccelIDMap(const int accelID, const int radID);
        int getRadIDByAccelID(const int accelID);
        int getMaxRadAccelID();

        //探针（截面）
        void insertSectionIDMap(const int sectionID, const int radID);
        int getRadIDBySectionID(const int sectionID);
        int getMaxRadSectionID();

        //连接
        void insertConnectionIDMap(const int connectionID, const int radID);
        int getRadIDByConnectionID(const int connectionID);
        int getMaxRadConnectionID();

        //时间历程
        void insertTimeHIDMap(const int thID, const int radID);
        int getRadIDByTimeHID(const int thID);
        int getMaxRadTimeHID();

        //失效
        void insertFailureIDMap(const int failID, const int radID);
        int getRadIDByFailureID(const int failID);
        int getMaxRadFailureID();

        //BEM
        void insertBEMIDMap(const int bemID, const int radID);
        int getRadIDByBEMID(const int bemID);
        int getMaxRadBEMID();

    private:
        /**
         * @brief  材料ID映射表 materialID -> rad文件中的ID
         * @author libaojun 
         * @date   2025-08-11
         */
        QHash<int, int> _materialIDMap;

        /**
        * @brief  属性ID映射表 propertyID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _propertyIDMap;

        /**
        * @brief  曲线ID映射表 curveID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _curveIDMap;
       
        /**
        * @brief  刚性墙ID映射表 rWallID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _rWallIDMap;

        /**
        * @brief  重力ID映射表ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _gravityIDMap;

        /**
        * @brief  节点组ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _nodeSetIDMap;

        /**
       * @brief  元素组ID映射表 ID -> rad文件中的ID
       * @author wangning
       * @date   2025-08-28
       */
        QHash<int, int> _eleSetIDMap;

        /**
        * @brief  初始场ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _initialFieldIDMap;

        /**
        * @brief   边界ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _bcIDMap;

        /**
        * @brief   Part ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _partIDMap;

        /**
        * @brief   subset ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _subsetIDMap;

        /**
        * @brief   surface ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date   2025-08-28
        */
        QHash<int, int> _surfaceIDMap;

        /**
         * @brief   相互作用ID映射表 ID -> rad文件中的ID
         * @author wangning
         * @date   2025-08-28
         */
        QHash<int, int> _interactionIDMap;

        /**
         * @brief segment Surface id
         * @author libaojun
         * @date 2025-09-19
         */
        int _segSurfaceID{ 0 };

        /**
        * @brief  加速度计ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date 2025-09-23
        */
        QHash<int, int> _accelIDMap;

        /**
        * @brief  截面ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date 2025-09-23
        */
        QHash<int, int> _sectionIDMap;

        /**
        * @brief  连接ID映射表 ID -> rad文件中的ID
        * @author wangning
        * @date 2025-09-23
        */
        QHash<int, int> _connectionIDMap;

        /**
        * @brief  时间历程 ID -> rad文件中的ID
        * @author wangning
        * @date 2025-09-23
        */
        QHash<int, int> _timeHID;

        /**
         * @brief 失效ID -> rad文件中的ID
         * @author wangning
         * @date 2025-09-23
         */
        QHash<int, int> _failureIDMap;

        /**
         * @brief BEM ID映射表 BEM ID -> rad文件中的ID
         * @author liuzhonghua
         * @date 2025-04-27
         */
        QHash<int, int> _bemIDMap;

    };
}
#endif // ___RADWRITEINFORMATIONMAPPER_H_