/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKAbstractAssInstance.h"

// Global data
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"

// Data
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"
#include "FITKAbsAlgorithmTools.h"
#include "FITKAbsInterfaceFactory.h"

namespace Interface
{
    FITKAbstractAssInstance::FITKAbstractAssInstance()
    {
        // 初始化变换工具。
        initTransformTool();
    }

    FITKAbstractAssInstance::FITKAbstractAssInstance(int modelId)
    {
        // 初始化变换工具。
        initTransformTool();

        setModel(modelId);
    }

    FITKAbstractAssInstance::FITKAbstractAssInstance(int modelId, double * xyz)
    {
        // 初始化变换工具。
        initTransformTool();

        setModel(modelId);

        if (m_transformTool)
        {
            // 获取初始变换信息。
            m_transformTool->transform(xyz[0], xyz[1], xyz[2]);
            m_transformTool->data(m_matrix, m_vector);
        }
        else
        {
            m_vector[0] = xyz[0];
            m_vector[1] = xyz[1];
            m_vector[2] = xyz[2];
        }
    }

    FITKAbstractAssInstance::FITKAbstractAssInstance(int modelId, double * pt1, double * pt2, double angle)
    {
        // 初始化变换工具。
        initTransformTool();

        setModel(modelId);

        if (m_transformTool)
        {
            // 获取初始变换信息。
            m_transformTool->rotate(pt1, pt2, angle);
            m_transformTool->data(m_matrix, m_vector);
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                m_rotate1[i] = pt1[i];
                m_rotate2[i] = pt2[i];
            }

            m_angle = angle;
        }
    }

    FITKAbstractAssInstance::FITKAbstractAssInstance(FITKAbstractAssInstance * instance, double * xyz)
    {
        // 初始化变换工具。
        initTransformTool();

        setModel(instance->getModelID());

        if (m_transformTool)
        {
            // 获取初始变换信息。
            m_transformTool->setData(instance->m_matrix, instance->m_vector);
            m_transformTool->transform(xyz[0], xyz[1], xyz[2]);
            m_transformTool->data(m_matrix, m_vector);
        }
        else
        {
            m_vector[0] = xyz[0];
            m_vector[1] = xyz[1];
            m_vector[2] = xyz[2];
        }
    }

    FITKAbstractAssInstance::FITKAbstractAssInstance(FITKAbstractAssInstance * instance, double * pt1, double * pt2, double angle)
    {
        // 初始化变换工具。
        initTransformTool();

        setModel(instance->getModelID());

        if (m_transformTool)
        {
            // 获取初始变换信息。
            m_transformTool->setData(instance->m_matrix, instance->m_vector);
            m_transformTool->rotate(pt1, pt2, angle);
            m_transformTool->data(m_matrix, m_vector);
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                m_rotate1[i] = pt1[i];
                m_rotate2[i] = pt2[i];
            }

            m_angle = angle;
        }
    }

    FITKAbstractAssInstance::~FITKAbstractAssInstance()
    {
        // 析构变换工具。
        if (m_transformTool)
        {
            delete m_transformTool;
            m_transformTool = nullptr;
        }
    }

    bool FITKAbstractAssInstance::getMatrix4X4(double oMat[4][4])
    {
        if (m_transformTool == nullptr) return false;
        double m[3][3], v[3];
        m_transformTool->data(m, v);

        oMat[0][0] = m[0][0];
        oMat[0][1] = m[0][1];
        oMat[0][2] = m[0][2];
        oMat[0][3] = v[0];

        oMat[1][0] = m[1][0];
        oMat[1][1] = m[1][1];
        oMat[1][2] = m[1][2];
        oMat[1][3] = v[1];

        oMat[2][0] = m[2][0];
        oMat[2][1] = m[2][1];
        oMat[2][2] = m[2][2];
        oMat[2][3] = v[2];

        oMat[3][0] = 0.;
        oMat[3][1] = 0.;
        oMat[3][2] = 0.;
        oMat[3][3] = 1.;

        return true;
    }

    bool FITKAbstractAssInstance::hasTransformTool()
    {
        return m_transformTool != nullptr;
    }

    void FITKAbstractAssInstance::initTransformTool()
    {
        if (m_transformTool)
        {
            return;
        }

        // 获取工具生成器。
        Interface::FITKAbsAlgorithmToolsCreator* creator = Interface::FITKInterfaceAlgorithmFactory::getInstance()->getAlgToolsCreator();
        if (!creator)
        {
            return;
        }

        // 变换工具可重复使用。
        m_transformTool = creator->createTransformTool();
    }

    Interface::FITKAbstractTransformTool* FITKAbstractAssInstance::getInternalTransformTool()
    {
        return m_transformTool;
    }

    void FITKAbstractAssInstance::transform(double x, double y, double z)
    {
        if (!m_transformTool)
        {
            return;
        }

        m_transformTool->setData(m_matrix, m_vector);
        m_transformTool->transform(x, y, z);
        m_transformTool->data(m_matrix, m_vector);
    }

    void FITKAbstractAssInstance::rotate(double* pt1, double* pt2, double angle)
    {
        if (!m_transformTool)
        {
            return;
        }

        m_transformTool->setData(m_matrix, m_vector);
        m_transformTool->rotate(pt1, pt2, angle);
        m_transformTool->data(m_matrix, m_vector);
    }

    void FITKAbstractAssInstance::getTransfom(double * transform)
    {
        // 获取工具生成器。
        if (m_transformTool)
        {
            double vec[3]{ 0., 0., 0. };
            double mat33[3][3]{ 1.,0.,0.,0.,1.,0.,0.,0.,1. };
            m_transformTool->data(mat33, vec);

            for (int i = 0; i < 3; i++)
            {
                transform[i] = vec[i];
            }
        }
        else
        {
            transform[0] = m_vector[0];
            transform[1] = m_vector[1];
            transform[2] = m_vector[2];
        }
    }

    bool FITKAbstractAssInstance::getRotateAxis(double * p1, double * p2, double * angle)
    {
        if (m_transformTool)
        {
            return m_transformTool->isRotation(p1, p2, *angle);
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                p1[i] = m_rotate1[i];
                p2[i] = m_rotate2[i];
            }

            *angle = m_angle;

            return true;
        }

        return false;
    }

    void FITKAbstractAssInstance::setTransform(double* trans, double* rotatePt1, double* rotatePt2, double angle)
    {
        // 执行变换并保存信息。
        if (m_transformTool)
        {
            m_transformTool->setTransform(trans, rotatePt1, rotatePt2, angle);
            m_transformTool->data(m_matrix, m_vector);
            m_transformTool->isRotation(m_rotate1, m_rotate2, m_angle);
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                m_vector[i] = trans[i];
                m_rotate1[i] = rotatePt1[i];
                m_rotate2[i] = rotatePt2[i];
            }

            m_angle = angle;
        }
    }

    bool FITKAbstractAssInstance::getPointCoor(int pointID, double * coor, int modelIndex)
    {
        FITKAbstractModel* model = this->getModel();
        if (model == nullptr) return false;
        //部件坐标
        double pt[3] = { 0,0,0 };
        bool ok = model->getPointCoor(pointID, pt, modelIndex);
        if (!ok) return false;

        if (m_transformTool)
        {
            m_transformTool->transformPoint(pt, coor);
        }
        else
        {
            //计算轴线
            double axis[3] = { 0,0,0 };
            for (int i = 0; i < 3; ++i) axis[i] = m_rotate2[i] - m_rotate1[i];

            //旋转
            Core::FITKPoint FKp(pt);
            Core::FITKVec3D FKAxis(axis);
            Core::FITKPoint res = Core::RotateAroundAxis(FKp, FKAxis, m_angle * FITK_PI / 180.0);
            res.getCoor(pt);

            //平移
            for (int i = 0; i < 3; ++i)
                coor[i] = pt[i] + m_vector[i];
        }

        return true;
    }

    void FITKAbstractAssInstance::transformPoint(double* iPt, double* oPt)
    {
        if (!m_transformTool)
        {
            return;
        }

        m_transformTool->transformPoint(iPt, oPt);
    }

    void FITKAbstractAssInstance::transformDirection(double* iDir, double* oDir)
    {
        if (!m_transformTool)
        {
            return;
        }

        m_transformTool->transformDirection(iDir, oDir);
    }

    Interface::FITKModelEnum::AbsModelType FITKAbstractAssInstance::getAbsModelType()
    {
        //获取类型
        return FITKModelEnum::AbsModelType::AssInstance;
    }

    void FITKAbstractAssInstance::setModel(int modelid)
    {
        _modelID = modelid;
    }

    void FITKAbstractAssInstance::setModel(FITKAbstractModel* model)
    {
        //转成id存储
        if (model == nullptr) _modelID = -1;
        _modelID = model->getDataObjectID();
    }

    FITKAbstractModel* FITKAbstractAssInstance::getModel() const
    {
        //从全局仓库查找
        return Core::FITKDataRepo::getInstance()->getTDataByID<FITKAbstractModel>(_modelID);
    }

    int FITKAbstractAssInstance::getModelID() const
    {
        return _modelID;
    }

    FITKComponentManager* FITKAbstractAssInstance::getComponentManager()
    {
        auto m = this-> getModel();
        if (m == nullptr) return nullptr;
        return m->getComponentManager();
    }

    FITKModelEnum::FITKModelDim FITKAbstractAssInstance::getModelDim()
    {
        auto model = this->getModel();
        //返回原始的模型维度
        if (model)
            return model->getModelDim();

        return FITKModelEnum::FITKModelDim::FMDNone;
    }

    bool FITKAbstractAssInstance::isComponentValid(Interface::FITKModelEnum::FITKModelSetType compType, const QList<int> & members)
    {
        FITKAbstractModel* model = this->getModel();
        if (!model)
        {
            return false;
        }

        return model->isComponentValid(compType, members);
    }
}


