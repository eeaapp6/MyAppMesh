/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKAbstractAssInstance.h
 * @brief 抽象装配体实例定义
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-03-29
 *
 */
#ifndef __FITK_ABSTRACT_INSTANCE_H___
#define __FITK_ABSTRACT_INSTANCE_H___

#include "FITKInterfaceModelAPI.h"
#include "FITKAbstractModel.h"

namespace Interface
{
    // 前置声明。
    class FITKAbstractTransformTool;

     /**
      * @brief 抽象装配体实例
      * @author LiBaojun (libaojunqd@foxmail.com)
      * @date 2024-03-29
      */
    class FITKInterfaceModelAPI FITKAbstractAssInstance : public  FITKAbstractModel
    {
        FITKCLASS(Interface, FITKAbstractAssInstance);
    public:
        /**
         * @brief Construct a new FITKAbstractAssInstance object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        explicit FITKAbstractAssInstance();

        /**
         * @brief  构建一个部件实例.
         * @param  modelId 部件ID
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        FITKAbstractAssInstance(int modelId);

        /**
         * @brief  构建一个平移的部件实例.
         * @param  modelId 部件ID
         * @param  xyz xyz方向上的平移量数组
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        FITKAbstractAssInstance(int modelId, double* xyz);

        /**
         * @brief  构建一个旋转的部件实例.
         * @param  modelId 部件ID
         * @param  pt1 旋转轴第一点坐标
         * @param  pt2 旋转轴第二点坐标
         * @param  angle 旋转角度
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        FITKAbstractAssInstance(int modelId, double* pt1, double* pt2, double angle);

        /**
         * @brief  构建一个平移的部件实例.
         * @param  instance 原始实例
         * @param  xyz xyz方向上的平移量数组
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        FITKAbstractAssInstance(FITKAbstractAssInstance* instance, double* xyz);

        /**
         * @brief  构建一个旋转的部件实例.
         * @param  instance 原始实例
         * @param  pt1 旋转轴第一点方向
         * @param  pt2 旋转轴第二点方向
         * @param  angle 旋转角度
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        FITKAbstractAssInstance(FITKAbstractAssInstance* instance, double* pt1, double* pt2, double angle);

        /**
         * @brief Destroy the FITKAbstractAssInstance object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        virtual ~FITKAbstractAssInstance();

        /**
         * @brief       获取几何变换工具。
         * @return      变换工具
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-12-16
         */
        Interface::FITKAbstractTransformTool* getInternalTransformTool();

        /**
         * @brief  平移实例.
         * @param  x x方向上的平移量
         * @param  y y方向上的平移量
         * @param  z z方向上的平移量
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        void transform(double x, double y, double z);

        /**
         * @brief  旋转实例.
         * @param  pt1 旋转轴第一点坐标
         * @param  pt2 旋转轴第二点坐标
         * @param  angle 旋转角度
         * @author Yanzhihui (chanyuantiandao@126.com)
         * @date   2025-04-09
         */
        void rotate(double* pt1, double* pt2, double angle);

        /**
         * @brief       获取平移量。
         * @param[out]  transform：xyz方向平移量
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        void getTransfom(double* transform);

        /**
         * @brief       获取旋转轴。
         * @param[out]  p1：旋转轴第一个点坐标
         * @param[out]  p2：旋转轴第二个点坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        bool getRotateAxis(double* p1, double* p2, double* angle);

        /**
         * @brief       保存变换信息。
         * @param[in]   trans：平移
         * @param[in]   rotatePt1：旋转轴第一点坐标
         * @param[in]   rotatePt2：旋转轴第二点坐标
         * @param[in]   angle：旋转角
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        void setTransform(double* trans, double* rotatePt1, double* rotatePt2, double angle);

        /**
        * @brief        工具方法 - 获取变换后的节点坐标。[重写]
        * @param[in]    pointID：节点ID
        * @param[out]   coor：返回坐标
        * @param[in]    modelIndex：模型索引，用于复合模型（一个模型包含多个子模型）
        * @return       是否获取成功
        * @author       ChengHaotian (yeguangbaozi@foxmail.com)
        * @date         2025-04-09
        */
        bool getPointCoor(int pointID, double* coor, int modelIndex = 0) override;

        /**
         * @brief       坐标变换。[重写]
         * @param[in]   iPt：输入坐标
         * @param[out]  oPt：输出坐标
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        void transformPoint(double* iPt, double* oPt) override;

        /**
         * @brief       方向变换。[重写]
         * @param[in]   iDir：输入方向
         * @param[out]  oDir：输出方向
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        void transformDirection(double* iDir, double* oDir) override;

        /**
         * @brief       获取变换矩阵。
         * @param[out]  oMat：矩阵
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        bool getMatrix4X4(double oMat[4][4]);

        /**
         * @brief       是否存在变换计算工具。
         * @return      是否存在
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        bool hasTransformTool();

        /**
         * @brief 获取模型类型
         * @return AbsModelType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-02-27
         */
        virtual FITKModelEnum::AbsModelType getAbsModelType();
        /**
         * @brief 设置modelID
         * @param[i]  modelid        模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setModel(int modelid);
        /**
         * @brief 设置模型
         * @param[i]  model          设置模型
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        void setModel(FITKAbstractModel* model);
        /**
         * @brief 获取模型对象
         * @return FITKAbstractModel*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        FITKAbstractModel* getModel() const;

        /**
         * @brief 获取模型对象id
         * @return int
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-30
         */
        int getModelID() const;

        template<class T>
        /**
         * @brief 获取到数据对象并转换类型
         * @return T*
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        T* getModelT()
        {
            return dynamic_cast<T*>(this->getModel());
        }

        /**
         * @brief 获取集合/表面管理器
         * @return Interface::FITKModelSetManager*
         * @author libaojun (libaojunqd@foxmail.com)
         * @date 2024-03-17
         */
        virtual FITKComponentManager* getComponentManager() override;

        /**
         * @brief 获取模型维度
         * @return FITKModelDim
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-11
         */
        virtual FITKModelEnum::FITKModelDim getModelDim() override;

    protected:
        /**
         * @brief       初始化变换计算工具。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        void initTransformTool();

    protected:
        /**
         * @brief 模型ID
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-03-22
         */
        int _modelID{ -1 };

        /**
         * @brief       变换计算工具。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        Interface::FITKAbstractTransformTool* m_transformTool{ nullptr };

        /**
         * @brief       旋转矩阵。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        double m_matrix[3][3]{ 1.,0.,0.,0.,1.,0.,0.,0.,1. };

        /**
         * @brief       平移向量。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        double m_vector[3]{ 0., 0., 0. };

        /**
         * @brief       旋转轴第一点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        double m_rotate1[3]{ 0., 0., 0. };

        /**
         * @brief       旋转轴第二点坐标。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        double m_rotate2[3]{ 0., 0., 0. };

        /**
         * @brief       旋转角。
         * @author      ChengHaotian (yeguangbaozi@foxmail.com)
         * @date        2025-04-09
         */
        double m_angle = 0.;

    };
}

#endif
