/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKRadiossCase.h
 * @brief  Radioss算例类声明
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-05-09
 * 
 */
#ifndef __FITK_RAD__CASE_H__
#define __FITK_RAD__CASE_H__

#include "FITKRadiossDataAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Interface
{
    class FITKAbstractSectionManager;
    class FITKMaterialManager;
}

namespace Radioss
{
    class FITKRadiossGeomPartManager;
    class FITKRadiossMeshModel;
    class FITKRadiossBCSManager;
    class FITKNonLinerCurveManager;
    class FITKRadiossPropValueManager;
    class FITKRadiossSubSetManager;
    class FITKInitialFieldManager;
    class FITKRadiossRWallManager;
    class FITKGravityManager;
    class FITKInteractionManager;
    class FITKRadiossSolverSettings;
    class FITKRadiossResultRequest;
    class FITKRadiossSolutionManager;
    class FITKRadiossSolution;
    class FITKRadiossInteractionDefaultManager;
    class FITKEquationOfStateManager;
    class FITKConnectionManager;
    class FITKRadiossFailureModelManager;
    class FITKProbeManager;
    class FITKRadiossBEMManager;
    /**
     * @brief Radioss算例类
     * @details 此类用于描述一个完整的Radioss计算算例，包含截面、材料等管理器
     * @author libaojun
     * @date 2025-04-25
     */
    class FITKRadiossDataAPI FITKRadiossCase : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Radioss, FITKRadiossCase);
        FITKSafeDownCastA(FITKRadiossCase, Core::FITKAbstractDataObject);
        
    public:
        /**
         * @brief 构造函数
         * @author libaojun
         * @date 2025-04-25
         */
        explicit FITKRadiossCase();
        
        /**
         * @brief 析构函数
         * @author libaojun
         * @date 2025-04-25
         */
        virtual ~FITKRadiossCase();
        /**
         * @brief 获取部件管理器
         * @return 部件管理器指针
         * @author libaojun
         * @date 2025-04-25
         */
        FITKRadiossGeomPartManager* getGeomPartManager() const;

        /**
         * @brief 获取截面管理器
         * @return 截面管理器指针
         * @author libaojun
         * @date 2025-04-25
         */
        Interface::FITKAbstractSectionManager* getSectionManager() const;
        
  
        /**
         * @brief 获取材料管理器
         * @return 材料管理器指针
         * @author libaojun
         * @date 2025-04-25
         */
        Interface::FITKMaterialManager* getMaterialManager() const;
        /**
         * @brief    获取状态方程管理器
         * @return   FITKEquationOfStateManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-28
         */
        FITKEquationOfStateManager* getEquationOfState() const;

        /**
         * @brief 获取网格模型
         * @return FITKRadiossMeshModel* 网格模型指针
         * @author libaojun
         * @date 2025-05-30
         */
        FITKRadiossMeshModel* getMeshModel() const;
        /**
         * @brief    获取失效模型管理器
         * @return   FITKRadiossFailureModelManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-10
         */
        FITKRadiossFailureModelManager* getFailureModelManager() const;
        /**
         * @brief 获取非线性曲线管理器
         * @return FITKNonLinerCurveManager* 非线性曲线管理器指针
         * @author libaojun
         * @date 2025-06-05
         */
        FITKNonLinerCurveManager* getNonLinerCurveManager() const;
        /**
         * @brief 获取默认属性值管理器
         * @return FITKRadiossPropValueManager* 默认属性值管理器指针
         * @author libaojun
         * @date 2025-05-30
         */
        FITKRadiossPropValueManager* getDefaultPropValueManager() const;
        /**
         * @brief  获取默认相互作用管理器
         * @return FITKRadiossInteractionDefaultManager* 默认管理器指针
         * @author wangning (2185896382@qq.com)
         * @date   2025-09-19
         */
        FITKRadiossInteractionDefaultManager* getDefaultInteractionManager();
          /**
         * @brief  获取边界条件管理器
         * @return FITKRadiossBCSManager* 边界条件管理器指针
         * @author libaojun
         * @date 2025-06-05
         */
        FITKRadiossBCSManager* getBCSManager();
        
        /**
         * @brief  获取初始场管理器
         * @return FITKInitialFieldManager* 初始场管理器指针
         * @author libaojun
         * @date 2025-09-02
         */
        FITKInitialFieldManager* getInitialFieldManager();

        /**
         * @brief  获取刚性墙管理器
         * @return FITKRadiossRWallManager* 刚性墙管理器指针
         * @author libaojun
         * @date 2025-08-25
         */
        FITKRadiossRWallManager* getRWallManager();
        /**
         * @brief  获取重力场管理器
         * @return FITKGravityManager* 重力场管理器指针
         * @author libaojun
         * @date 2025-08-25
         */
        FITKGravityManager* getGravityManager();
        /**
         * @brief    获取连接管理器
         * @return   FITKConnectionManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-06
         */
        FITKConnectionManager* getConnectionManager();
        /**
         * @brief 获取相互作用管理器
         * @return FITKInteractionManager* 相互作用管理器指针
         * @author libaojun
         * @date 2025-09-02
         */
        FITKInteractionManager* getInteractionManager();
        /**
         * @brief    获取探针管理器
         * @return   FITKProbeManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-12-04
         */
        FITKProbeManager* getProbeManager();
        /**
         * @brief    获取BEM管理器
         * @return   FITKRadiossBEMManager *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-04-27
         */
        FITKRadiossBEMManager* getBEMManager();
        /**
         * @brief    获取求解设置数据
         * @return   FITKRadiossSolverSettings *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-08
         */
        FITKRadiossSolverSettings* getSolverSettings();
        /**
         * @brief    获取结果请求数据
         * @return   FITKRadiossResultRequest *
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-09-09
         */
        FITKRadiossResultRequest* getResultRequest();

         /**
         * @brief 获取节点边界框
         * @param[out]  bndBox      边界框数组，大小为6，依次为xmin, xmax, ymin, ymax, zmin, zmax
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2025-08-26
         */
        void getNodeBndBox(double *bndBox) const;

        /**
         * @brief 获取求解步骤管理器
         * @return FITKRadiossSolutionManager* 求解步骤管理器指针
         * @author libaojun
         * @date 2025-09-10
         */
        FITKRadiossSolutionManager* getSolutionManager() const;
        /**
         * @brief 获取当前求解步骤
         * @return FITKRadiossSolution* 当前求解步骤指针
         * @author libaojun
         * @date 2025-09-10
         */
        FITKRadiossSolution* getCurrentSolution();
        /**
         * @brief    清楚数据
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-04
         */
        void clearData();

    private:
        FITKRadiossGeomPartManager* _geomPartManager{nullptr}; //!< 几何部件管理器
        FITKRadiossPropValueManager* _defaultPropValueManager{nullptr}; //!< 默认属性值管理器
        Interface::FITKAbstractSectionManager* _sectionManager{nullptr}; //!< 截面管理器
        Interface::FITKMaterialManager* _materialManager{nullptr}; //!< 材料管理器 
        FITKEquationOfStateManager* _equOfStateManager{ nullptr }; //!< 状态方程管理器
        FITKRadiossMeshModel* _meshModel{nullptr}; //!< 网格模型
        FITKNonLinerCurveManager* _nonLinerCurveManager{nullptr}; //!< 非线性曲线管理器
        FITKRadiossFailureModelManager* _failureModelManager{ nullptr }; //!< 失效管理器
        FITKRadiossSolutionManager* _solutionManager{nullptr}; //!< 求解步骤管理器
        FITKRadiossBEMManager* _bemManager{nullptr}; //!< BEM管理器

        
    };

   

} // namespace Radioss

#endif // __FITK_RAD__ANALYSIS_H__