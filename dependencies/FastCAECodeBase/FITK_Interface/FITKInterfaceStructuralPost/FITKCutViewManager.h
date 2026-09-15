/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/*****************************************************************//**
 * @file        FITKCutViewManager.h
 * @brief       裁切/切片数据类数据管理器。
 *
 * @author      ChengHaotian (yeguangbaozi@foxmail.com)
 * @date        2024-06-17
 *********************************************************************/

#ifndef  _FITK_CUT_VIEW_MANAGER_H__
#define  _FITK_CUT_VIEW_MANAGER_H__

#include "FITKInterfaceStructuralPostAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"

// 前置声明
class vtkCutter;
class vtkTableBasedClipDataSet;
class vtkImplicitFunction;
class vtkAlgorithmOutput;
class vtkAlgorithm;
class vtkDataSet;

namespace Interface
{
    class FITKAbstractStructuralPostVTK;
    class FITKMeshFeatureVTK;

    // Base
    //@{
    /**
     * @brief       裁切/切片数据抽象类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-17
     */
    class FITKInterfaceStructuralPostAPI FITKAbstractViewCut : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKAbstractViewCut);

        Q_OBJECT
    public:
        /**
         * @brief       裁切器形状。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        enum CutShapeType
        {
            Plane,
            Cylinder,
            Sphere,
        };
        Q_ENUM(CutShapeType);

        /**
         * @brief       获取裁切器显示数据类型。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        enum DisplayModelType
        {
            Clip,
            Slice,
            RevClip,
        };
        Q_ENUM(DisplayModelType);

    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        explicit FITKAbstractViewCut();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual ~FITKAbstractViewCut();

        /**
         * @brief       获取裁切器形状。[纯虚函数]
         * @return      裁切形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual CutShapeType getShapeType() = 0;

        /**
         * @brief       开启或关闭网格特征生成。
         * @param[in]   flag：标记
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-09
         */
        void setUseMeshFeature(bool flag);

        /**
         * @brief       获取是否使用网格特征。
         * @return      是否开启
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-09
         */
        bool getUseMeshFeature();

        /**
         * @brief       设置裁切对象可见性。
         * @param[in]   visible：是否可见
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void setVisible(bool visible);

        /**
         * @brief       获取裁切对象可见性。
         * @return      是否可见
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        bool isVisible();

        /**
         * @brief       设置裁切算法中心点。
         * @param[in]   x：X坐标
         * @param[in]   y：Y坐标
         * @param[in]   z：Z坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void setOrigin(double x, double y, double z);

        /**
         * @brief       获取裁切算法中心点。
         * @param[out]  coor：坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void getOrigin(double* coor);

        /**
         * @brief       切换裁切显示模式。
         * @param[in]   t：显示模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual void setModelType(DisplayModelType t);

        /**
         * @brief       获取裁切显示模式。
         * @return      显示模式
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        DisplayModelType getModelType();

        /**
         * @brief       设置裁切器输入数据管线。
         * @param[in]   inp：输入数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void setInput(vtkAlgorithm* inp);

        /**
         * @brief       获取输出算法。
         * @return      vtk算法
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-22
         */
        vtkAlgorithm* getOutputAlgorithm();

        /**
         * @brief       根据传入的显示数据类型，获取输出算法
         * @param[i]    传入显示数据类型
         * @return      vtk算法
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-14
         */
        vtkAlgorithm* getOutputAlgorithm(DisplayModelType type);

        /**
         * @brief       获取裁切器输出数据管线。
         * @return      输出数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        vtkAlgorithmOutput* getOutputPort();

        /**
         * @brief       根据传入的显示数据类型,获取裁切器输出数据管线
         * @param[i]    传入显示数据类型
         * @return      输出数据
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-14
         */
        vtkAlgorithmOutput* getOutputPort(DisplayModelType type);

        /**
         * @brief       获取裁切器输出数据。
         * @return      输出数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-22
         */
        vtkDataSet* getOutput();

        /**
         * @brief       根据传入的显示数据类型,获取裁切器输出数据
         * @param[i]    传入显示数据类型
         * @return      输出数据
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-14
         */
        vtkDataSet* getOutput(DisplayModelType type);

        /**
         * @brief       获取网格特征数据。
         * @return      网格特征数据
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-22
         */
        FITKMeshFeatureVTK* getMeshFeature();

        /**
         * @brief       更新。[虚函数]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual void update();

        /**
         * @brief       获取被裁切数据包围盒尺寸。
         * @param[out]  bounds：包围盒尺寸
         * @param[in]   factor：放大系数[缺省]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-06
         */
        void getInputBounds(double* bounds, double factor = 1.0);

        /**
         * @brief       设置后处理模型ID。
         * @param[in]   id：后处理模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        void setModelID(int id);

        /**
         * @brief       获取后处理模型ID。
         * @return      后处理模型数据ID
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        int getModelID();

        // 以下函数是为AppCDEM中复选框不互斥实现 
        /**
         * @brief       获取裁切器显示的数据组
         * @return      裁切器显示的数据组
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-09
         */
        QList<DisplayModelType> getDisplayModelTypes();

        /**
         * @brief       将裁切器显示的数据类型添加到数据组中
         * @param[i]    显示的数据类型
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-09
         */
        void appenDisplayModelType(DisplayModelType type);

        /**
         * @brief       将裁切器不显示的数据类型从数据组中移除
         * @param[i]    不显示的数据类型
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-09
         */
        void removeDisplayModelType(DisplayModelType type);

        /**
         * @brief       update，适用裁切数据类型列表的更新方式
         * @param[i]    仅用于与update区分
         * @author      WuYizhen (wyz_hanhan@163.com)
         * @date        2026-01-12
         */
        void update(int diff);

    protected:
        /**
          * @brief      裁切数据类型列表
          * @author     WuYizhen (wyz_hanhan@163.com)
          * @date       2026-01-09
          */
        QList<DisplayModelType> _modelTypes = {};

        /**
          * @brief       裁切过滤器，_clipper不显示的部分
          * @author      WuYizhen (wyz_hanhan@163.com)
          * @date        2026-01-12
          */
        vtkTableBasedClipDataSet* _revClipper{};
        // 
    protected:
        /**
         * @brief       可见性。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        bool _visible = false;

        /**
         * @brief       网格特征数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-01-22
         */
        FITKMeshFeatureVTK* _feature{ nullptr };

        /**
         * @brief       裁切显示模式。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        DisplayModelType _modelType{ Clip };

        /**
         * @brief       裁切算法中心点。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        double _origin[3] = { 0, 0, 0 };

        /**
         * @brief       裁切算法。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        vtkImplicitFunction* _function{};

        /**
         * @brief       切片过滤器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        vtkCutter* _cutter{};

        /**
         * @brief       裁切过滤器。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        vtkTableBasedClipDataSet* _clipper{};

        /**
         * @brief       裁切输入数据算法。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-06
         */
        vtkAlgorithm* _input{ nullptr };

        /**
         * @brief       后处理模型数据ID。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-11
         */
        int _modelID = -1;
    };
    //@}

    // Plane
    //@{
    /**
     * @brief       平面裁切数据类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-17
     */
    class FITKInterfaceStructuralPostAPI FITKViewCutPlane : public FITKAbstractViewCut
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        explicit FITKViewCutPlane();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual ~FITKViewCutPlane() = default;

        /**
         * @brief       获取裁切器形状。[重写]
         * @return      裁切器形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        CutShapeType getShapeType() override;

        /**
         * @brief       设置平面轴（法向）。
         * @param[in]   x：X方向值
         * @param[in]   y：Y方向值
         * @param[in]   z：Z方向值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void setNormalAxis(double x, double y, double z);

        /**
         * @brief       获取裁切平面轴（法向）。
         * @param[out]  axis：法向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void getNormalAxis(double* axis);

        /**
         * @brief       更新。[重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void update() override;

        /**
         * @brief       拷贝对象。[重写]
         * @param[in]   obj：拷贝结果
         * @return      是否拷贝成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-09
         */
        bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief       裁切屏幕轴（法向）。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        double _axis[3] = { 1, 0, 0 };
    };
    //@}

    // Cylinder
    //@{
    /**
     * @brief       圆柱裁切数据类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-17
     */
    class FITKInterfaceStructuralPostAPI FITKViewCutCylinder : public FITKAbstractViewCut
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        explicit FITKViewCutCylinder();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual ~FITKViewCutCylinder() = default;

        /**
         * @brief       获取裁切器形状。[重写]
         * @return      裁切器形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        CutShapeType getShapeType() override;

        /**
         * @brief       设置圆柱方向。
         * @param[in]   x：X方向值
         * @param[in]   y：Y方向值
         * @param[in]   z：Z方向值
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void setAxis(double x, double y, double z);

        /**
         * @brief       获取圆柱裁切器方向。
         * @param[out]  axis：方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void getAxis(double* axis);

        /**
         * @brief       设置圆柱半径。
         * @param[in]   radius：半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        void setRadius(double radius);

        /**
         * @brief       获取圆柱半径。
         * @return      半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        double getRadius();

        /**
         * @brief       更新。[重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void update() override;

        /**
         * @brief       拷贝对象。[重写]
         * @param[in]   obj：拷贝结果
         * @return      是否拷贝成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-09
         */
        bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief       圆柱方向。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        double _axis[3] = { 1, 0, 0 };

        /**
         * @brief       圆柱半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        double _radius = 1.;

    };
    //@}

    // Sphere
    //@{
    /**
     * @brief       球体裁切数据类。
     * @author      ChengHaotian (yeguangbaozi@foxmail.com)
     * @date        2024-06-17
     */
    class FITKInterfaceStructuralPostAPI FITKViewCutSphere : public FITKAbstractViewCut
    {
    public:
        /**
         * @brief       构造函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        explicit FITKViewCutSphere();

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual ~FITKViewCutSphere() = default;

        /**
         * @brief       获取裁切器形状。[重写]
         * @return      裁切器形状
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        CutShapeType getShapeType() override;

        /**
         * @brief       设置球体半径。
         * @param[in]   radius：半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        void setRadius(double radius);

        /**
         * @brief       获取球体半径。
         * @return      半径
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        double getRadius();

        /**
         * @brief       更新。[重写]
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void update() override;

        /**
         * @brief       拷贝对象。[重写]
         * @param[in]   obj：拷贝结果
         * @return      是否拷贝成功
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-09
         */
        bool copy(FITKAbstractDataObject* obj) override;

    private:
        /**
         * @brief       球体半径。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-07-05
         */
        double _radius = 1.;

    };
    //@}

    // Manager
    //@{
    class FITKInterfaceStructuralPostAPI FITKCutViewManager :
        public Core::FITKAbstractDataManager<FITKAbstractViewCut>
    {
    public:
        /**
         * @brief       构造函数。
         * @param[in]   absPost：模型后处理数据对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        explicit FITKCutViewManager(FITKAbstractStructuralPostVTK* absPost);

        /**
         * @brief       析构函数。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        virtual ~FITKCutViewManager() = default;

        /**
         * @brief       追加对象。[重写]
         * @param[in]   item：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void appendDataObj(Core::FITKAbstractDataObject* item) override;

        /**
         * @brief       插入对象。[重写]
         * @param[in]   index：索引
         * @param[in]   item：对象
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        void insertDataObj(int index, Core::FITKAbstractDataObject* item) override;

    private:
        /**
         * @brief       管理的可视化对象数据。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2024-06-17
         */
        FITKAbstractStructuralPostVTK* _postObj{};

    };
    //@}
}

#endif
