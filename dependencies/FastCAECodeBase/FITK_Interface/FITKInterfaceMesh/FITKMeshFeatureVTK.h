/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKMeshFeatureVTK.h
 * @brief       存储模型所有特征数据。
 *  
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-03-25
 *********************************************************************/

#ifndef _FITKMESHFEATURE_H___
#define _FITKMESHFEATURE_H___

#include "FITKInterfaceMeshAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"

// 前置声明
class vtkDataSet;
class vtkPolyData;
class vtkDataArray;
class vtkAlgorithmOutput;
class vtkIdFilter;
class vtkPassThrough;
class vtkAlgorithm;

class FITKPolyDataNormals;
class FITKSurfaceFilter; 
class FITKShellFeatureEdges;

// 过滤器传递原始id数据名称
#define POINTIDNAME "vtkOriginalPointIds"
#define CELLIDNAME "vtkOriginalCellIds"

namespace Interface
{
    /**
     * @brief       模型特征数据类vtk实现。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-03-25
     */
    class FITKInterfaceMeshAPI FITKMeshFeatureVTK : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief       获取ID过滤器节点编号（原始网格节点编号）数组名称
         * @return      ID过滤器数组名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-03
         */
        static const char* GetPointIdArrayName();

        /**
         * @brief       获取ID过滤器单元编号（原始网格单元编号）数组名称
         * @return      ID过滤器数组名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-03
         */
        static const char* GetCellIdArrayName();

        /**
         * @brief       获取特征边过滤器原始单元编号（表面过滤器输出数据单元编号）数组名称
         * @return      特征边过滤器单元编号数组名称
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-04
         */
        static const char* GetFeatEdgeOriginalIdArrayName();

        /**
         * @brief       构造函数。
         * @param[in]   dataSet: 模型数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        explicit FITKMeshFeatureVTK(vtkDataSet* dataSet);

        /**
         * @brief       构造函数。
         * @param[in]   connection: 模型管线输出
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-17
         */
        explicit FITKMeshFeatureVTK(vtkAlgorithmOutput* connection);

        /**
         * @brief       析构函数。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        virtual ~FITKMeshFeatureVTK();

        /**
         * @brief       更新特征。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        virtual void update();

        /**
         * @brief       设置启用网格特征。
         * @param[in]   flag：是否开启
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        void setEnabledFeature(bool flag);

        /**
         * @brief       获取是否开启网格特征。
         * @return      是否开启
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        bool getEnabledFeature();

        /**
         * @brief       设置模型数据。[虚函数]
         * @param[in]   dataSet: 模型数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        virtual void reset(vtkDataSet* dataSet);

        /**
         * @brief       设置模型管线数据。[虚函数]
         * @param[in]   connection: 模型数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        virtual void reset(vtkAlgorithmOutput* connection);

        /**
         * @brief       开启或关闭节点与单元ID数据生成。
         * @param[in]   pointIdsOn：开启节点ID生成
         * @param[in]   cellIdsOn：开启单元ID生成
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-04
         */
        void setCellAndPointIdsEnable(bool pointIdsOn, bool cellIdsOn);

        /**
         * @brief       开启或关闭表面边线提取。
         * @param[in]   isOn：是否开启
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-04
         */
        void setSurfEdgesEnable(bool isOn);

        /**
         * @brief       获取原始网格管线。
         * @return      网格数据传递过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-29
         */
        vtkPassThrough* getMeshPassThrough();

        /**
         * @brief       获取渲染使用的过滤器。
         * @return      过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        vtkAlgorithm* getRenderingFilter();

        /**
         * @brief       获取ID过滤器
         * @return      ID过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-04
         */
        vtkIdFilter* getIdFilter();

        /**
         * @brief       获取表面提取过滤器
         * @return      表面提取过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-22
         */
        FITKSurfaceFilter* getSurfaceFilter();

        /**
         * @brief       获取提取特征边过滤器
         * @return      提取特征边过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-22
         */
        FITKShellFeatureEdges* getFeatureEdgeFilter();

        /**
         * @brief       获取网格模型壳单元边过滤器
         * @return      网格边过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-06
         */
        FITKShellFeatureEdges* getShellEdgeFilter();

        /**
         * @brief       获取法向提取过滤器
         * @return      法向提取过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-22
         */
        FITKPolyDataNormals* getNormalsFilter();

        /**
         * @brief       获取特征网格
         * @param[in]   type: 特征类型  1-表面 2-法向 3-特征边 4-表面网格边
         * @return      网格数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-22
         */
        vtkPolyData* getFeatureMesh(int type);

        /**
         * @brief       获取单元法向数据
         * @return      单元法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-22
         */
        vtkDataArray* getCellNormals();
        
    private:
        /**
         * @brief       初始化数据管线
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-11
         */
        void initialize();

        /**
         * @brief       析构VTK对象并置空。[模板]
         * @param[in]   obj：VTK对象[引用]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-11
         */
        template <class T>
        void deleteVtkObj(T* & obj)
        {
            if (obj)
            {
                obj->Delete();
            }

            obj = nullptr;
        }

    private:
        /**
         * @brief       模型数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        vtkDataSet* _dataSet{};

        /**
         * @brief       模型管线输出
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-12-17
         */
        vtkAlgorithmOutput* _connection{};

        /**
         * @brief       传递输入过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-11
         */
        vtkPassThrough* _passThrough{ nullptr };

        /**
         * @brief       ID过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-03
         */
        vtkIdFilter* _idFilter{ nullptr };

        /**
         * @brief       提取表面数据过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        FITKSurfaceFilter* _surfaceFilter{};

        /**
         * @brief       法向提取过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        FITKPolyDataNormals* _normalFilter{};

        /**
         * @brief       特征边提取过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-03-25
         */
        FITKShellFeatureEdges* _featureEdgeFilter{};

        /**
         * @brief       网格模型边提取过滤器
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-03-06
         */
        FITKShellFeatureEdges* _edgeFilter{};

        /**
         * @brief       是否使用网格特征。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-11-21
         */
        bool m_useMeshFeat = true;

    };
}

#endif
