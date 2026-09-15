/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsExtendTool.h
 * @brief  抽象类工具类
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2025-05-06
 */
#ifndef FITK_ABS_EXTENT_TOOL
#define FITK_ABS_EXTENT_TOOL

#include "qglobal.h"
#include "FITKAbsVirtualTopoMapper.h"
#include "FITKAbsGeoCommand.h"
#include <queue>
#include <iostream>
#include <cmath>

namespace Interface
{
    /**
     * @brief  ACIS的补充工具类.
     * @author YanZhiHui(chanyuantiandao@126.com)
     * @date   2025-02-26
     */
    class FITKAbsExtendTool {

    public:
        FITKAbsExtendTool() = delete;
        ~FITKAbsExtendTool() = delete;

        /**
         * @brief  计算向量叉积
         * @param  first 第一个向量数组
         * @param  second 第二个向量数组
         * @param  result 结果向量数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-06
         */
        static void crossVector(const double* const first, const double* const second, double* result) {
            result[0] = first[1] * second[2] - first[2] * second[1];
            result[1] = first[2] * second[0] - first[0] * second[2];
            result[2] = first[0] * second[1] - first[1] * second[0];
        }

        /**
         * @brief  计算向量点积
         * @param  first 第一个向量数组
         * @param  second 第二个向量数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-06
         */
        static double dotVector(const double* const first, const double* const second) {
            return first[0] * second[0] + first[1] * second[1] + first[2] * second[2];
        }

        /**
         * @brief  判断向量是否为零向量
         * @param  vec 要判断的向量
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-06
         */
        static bool isZeroVector(const double* const vec) {
            return qFuzzyIsNull(vec[0]) && qFuzzyIsNull(vec[1]) && qFuzzyIsNull(vec[2]);
        }

        /**
         * @brief  向量的模
         * @param  vec 向量
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-06
         */
        static double vectorNorm(const double* const vec) {
            return std::sqrt(std::pow(vec[0], 2) + std::pow(vec[1], 2) + std::pow(vec[2], 2));
        }

        /**
         * @brief  两个向量是否反向
         * @param  first 第一个向量数组
         * @param  second 第二个向量数组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-06
         */
        static bool isFlipDirection(const double* const first, const double* const second) {
            if (isZeroVector(first) || isZeroVector(second)) return false;
            auto cosVal = dotVector(first, second) / vectorNorm(first) / vectorNorm(second);
            return qFuzzyCompare(cosVal, -1.0);
        }

        /**
         * @brief  判断边是否在面的拓扑上
         * @param  edge 边
         * @param  Face 面
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-07
         */
        static bool isEdgeOnFaceTopo(const VirtualShape& edge, const VirtualShape& face) {
            FITKVirtualTopoMapper mapper;

            mapper.mapTopo(face.VirtualTopoId, FITKGeoEnum::VTopoShapeType::VSEdge);

            const int nbEdges = mapper.length();

            for (int i = 0; i < nbEdges; ++i) {
                if (mapper.virtualTopo(i)->getDataObjectID() == edge.VirtualTopoId) return true;
            }
            return false;
        }

        /**
         * @brief  判断两个面是否相连（存在共边）
         * @param  face1 第一个面
         * @param  face2 第二个面
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-14
         */
        static bool isConnectedFace(const VirtualShape& face1, const VirtualShape& face2) {
            FITKVirtualTopoMapper mapper1{}, mapper2{};

            mapper1.mapTopo(face1.VirtualTopoId, FITKGeoEnum::VTopoShapeType::VSEdge);
            mapper2.mapTopo(face2.VirtualTopoId, FITKGeoEnum::VTopoShapeType::VSEdge);

            if (mapper1.length() == 0 || mapper2.length() == 0) return false;

            QList<FITKAbsVirtualTopo*> edgeList1{};
            for (int i = 0; i < mapper1.length(); ++i) {
                edgeList1.append(mapper1.virtualTopo(i));
            }

            for (int i = 0; i < mapper2.length(); ++i) {
                if (edgeList1.contains(mapper2.virtualTopo(i))) return true;
            }
            return false;
        }

        /**
         * @brief  判断一组面是否相连（存在共边）
         * @param  faces 面组
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @data   2025-05-14
         */
        static bool isConnectedFaces(QList<VirtualShape>& faces) {
            // 定义点和多边形的数据结构
            using Point = QPair<double, double>;
            using Face = QVector<Point>;

            if (faces.isEmpty()) return false;

            // 构建邻接表
            QVector<QVector<int>> adj(faces.size());
            for (size_t i = 0; i < faces.size(); ++i) {
                for (size_t j = i + 1; j < faces.size(); ++j) {
                    if (isConnectedFace(faces[i], faces[j])) {
                        adj[i].push_back(j);
                        adj[j].push_back(i);
                    }
                }
            }

            // BFS遍历
            QVector<bool> visited(faces.size(), false);
            std::queue<int> q;
            q.push(0);
            visited[0] = true;
            int count = 1;

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                for (int neighbor : adj[current]) {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                        ++count;
                    }
                }
            }

            // 如果所有面都被访问，则连通
            return count == faces.size();
        }
    };
}


#endif // !FITK_ABS_EXTENT_TOOL
