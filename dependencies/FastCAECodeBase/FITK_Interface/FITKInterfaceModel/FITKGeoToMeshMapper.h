/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKGeoToMeshMapper.h
 * @brief  几何到网格的映射
 * @author BaGuijun (baguijun@163.com)
 * @date   2025-08-28
 *********************************************************************/
#ifndef _FITKGeoToMeshMapper_
#define _FITKGeoToMeshMapper_

#include "FITKInterfaceModelAPI.h"
#include <QObject>
#include <QHash>
#include <QHashFunctions>
#include <QMutex>

namespace Interface
{
    /**
     * @brief  几何到网格的映射结构体-体和面
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-11-21
     */
    struct FITKGeoToMeshMapStrSolidFace
    {
    public:
        FITKGeoToMeshMapStrSolidFace() {};
        FITKGeoToMeshMapStrSolidFace(int solidID, int faceID) {
            _solidID = solidID;
            _faceID = faceID;
        }
        //==函数重载
        bool operator==(const FITKGeoToMeshMapStrSolidFace& other) const {
            return _solidID == other._solidID && _faceID == other._faceID;
        }
    public:
        //体ID
        int _solidID = -1;
        //面ID
        int _faceID = -1;
    };

    inline uint qHash(const FITKGeoToMeshMapStrSolidFace& key, uint seed = 0) {
        QtPrivate::QHashCombine hash;
        seed = hash(seed, key._solidID);
        seed = hash(seed, key._faceID);
        return seed;
    }

    /**
     * @brief  几何到网格的映射
     * @author BaGuijun (baguijun@163.com)
     * @date   2025-08-28
     */
    class FITKInterfaceModelAPI FITKGeoToMeshMapper : public QObject
    {
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-28
         */
        FITKGeoToMeshMapper();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-28
         */
        virtual ~FITKGeoToMeshMapper();
        /**
         * @brief    清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-23
         */
        void clear();
        /**
         * @brief    拷贝函数
         * @param[i] newObj                     被拷贝对象
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-12-08
         */
        virtual void copy(FITKGeoToMeshMapper* copyObj);
        /**
         * @brief    插入几何点ID与网格点ID映射信息
         * @param[i] geoNodeID                  几何点ID
         * @param[i] meshVertexID               网格点ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-01
         */
        void insertMeshVertexOfGeoNode(int geoNodeID, int meshVertexID);
        /**
         * @brief    移除几何点ID与网格点ID映射信息
         * @param[i] geoNodeID                  几何点ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-23
         */
        void removeMeshVertexOfGeoNode(int geoNodeID);
        /**
         * @brief    几何点ID与网格点ID映射信息
         * @return   QHash<int,int>             几何点ID与网格点ID映射信息
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-01
         */
        const QHash<int, int>& getMeshVertexOfGeoNode();
        /**
         * @brief    通过几何点ID获取对应的网格点ID
         * @param[i] geoNodeID                  几何点ID
         * @return   int                        网格点ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-10-15
         */
        int getMeshVertexOfGeoNode(int geoNodeID);
        /**
         * @brief    追加几何面所对应的网格单元信息
         * @param[i] faceID                     面ID
         * @param[i] eleID                      单元ID
         * @param[i] faceIndex                  面索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-29
         */
        void insertMeshEleOfGeoFace(int faceID, int eleID, int faceIndex);
        /**
         * @brief    移除几何面所对应的网格单元信息
         * @param[i] faceID                     面ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-23
         */
        void removeMeshEleOfGeoFace(int faceID);
        /**
         * @brief    几何面所对应网格单元ID
         * @return   QHash<int,QHash<int,int>>  映射表<几何面ID，<单元ID，单元的第几个面>>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-29
         */
        const QHash<int, QHash<int, QList<int>>>& getMeshEleOfGeoFace();
        /**
         * @brief    追加几何共面所对应的网格单元信息
         * @param[i] solidAndFace               体与面的ID
         * @param[i] eleID                      单元ID
         * @param[i] faceIndex                  面索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-11-21
         */
        void insertMeshEleOfGeoCoplane(FITKGeoToMeshMapStrSolidFace solidAndFace, int eleID, int faceIndex);
        /**
         * @brief    几何共面所对应的网格单元信息
         * @return   QHash<FITKGeoToMeshMapStrSolidFace,QHash<int,int>> 映射表<体ID与几何面ID，<单元ID，单元的第几个面>>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-11-21
         */
        const QHash<FITKGeoToMeshMapStrSolidFace, QHash<int, QList<int>>>& getMeshEleOfGeoCoplane();
        /**
         * @brief    追加几何边所对应的网格单元信息
         * @param[i] edgeID                     边ID
         * @param[i] eleID                      单元ID
         * @param[i] edgeIndex                  边索引
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-29
         */
        void insertMeshEleOfGeoEdge(int edgeID, int eleID, int edgeIndex);
        /**
         * @brief    移除几何边所对应的网格单元信息
         * @param[i] edgeID                     边ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-23
         */
        void removeMeshEleOfGeoEdge(int edgeID);
        /**
         * @brief    几何边所对应网格单元ID
         * @return   QHash<int,QHash<int,int>>  映射表<几何边ID，<单元ID，单元的第几条边>>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-29
         */
        const QHash<int, QHash<int, QList<int>>>& getMeshEleOfGeoEdge();
        /**
         * @brief    追加几何线所对应的网格单元信息
         * @param[i] edgeID                     几何线ID
         * @param[i] eleID                      单元ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-11-24
         */
        void insertMeshEleOfGeoWireEdge(int wireEdgeID, int eleID);
        /**
         * @brief    几何线所对应的网格单元信息
         * @return   QHash<int,QList<int>>      映射表<几何边，单元ID列表>
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-11-24
         */
        const QHash<int, QList<int>>& getMeshEleOfGeoWireEdge();
        /**
         * @brief    添加几何体所对应网格单元ID列表
         * @param[i] solidID                    几何体ID
         * @param[i] meshEleID                  网格单元ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-28
         */
        void insertMapOfGeoSolid(int solidID, int meshEleID);
        /**
         * @brief    移除几何体所对应网格单元ID列表
         * @param[i] solidID                    几何体ID
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-09-23
         */
        void removeMapOfGeoSolid(int solidID);
        /**
         * @brief    几何体所对应的网格单元ID
         * @return   QHash<int,QList<int>>      映射表
         * @author   BaGuijun (baguijun@163.com)
         * @date     2025-08-29
         */
        const QHash<int, QList<int>>& getMeshEleOfGeoSolid();
    protected:
        /**
         * @brief  几何点所对应的网格点
         *         <几何点ID，网格点ID>
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-09-01
         */
        QHash<int, int> _meshVerOfGeoNode = {};
        /**
         * @brief  几何边所对应网格单元ID
         *         <几何边ID，<单元ID，单元的第几条边>>
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-28
         */
        QHash<int, QHash<int, QList<int>>> _meshEleOfGeoEdge = {};
        /**
         * @brief  几何线所对应的网格
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-11-24
         */
        QHash<int, QList<int>> _meshEleOfGeoWireEdge = {};
        /**
         * @brief  几何面所对应网格单元ID
         *         <几何面ID，<单元ID，单元的第几个面>>
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-28
         */
        QHash<int, QHash<int, QList<int>>> _meshEleOfGeoFace = {};
        /**
         * @brief  几何面（共面）所对应网格单元ID
         *         <几何体+几何面ID，<单元ID，单元的第几个面>>
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-11-21
         */
        QHash<FITKGeoToMeshMapStrSolidFace, QHash<int, QList<int>>> _meshEleOfGeoCoplane = {};
        /**
         * @brief  几何体所对应的网格单元ID
         * @author BaGuijun (baguijun@163.com)
         * @date   2025-08-28
         */
        QHash<int, QList<int>> _meshEleOfGeoSolid = {};
    private:
        /**
            * @brief 互斥锁对象
            * @author BaGuijun (baguijun@163.com)
            * @date 2024-06-06
            */
        static QMutex _mutex;
    };
}

#endif