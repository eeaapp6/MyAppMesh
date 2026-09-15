/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEIData.h
 * @brief  PHengLEI物理场数据类
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-11
 * 
 */
#ifndef _FITK_PHHENGLEIDATA___H___
#define _FITK_PHHENGLEIDATA___H___
 

#include "FITK_Kernel/FITKCore/FITKAbstractDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"

namespace Interface
{
	class FITKPHengLEICouplingControl;
	class FITKPHengLEIMeshProcess;
    class FITKPHengLEIAbsSolver;
    class FITKPHengLEIBoundaryManager;
	class FITKPHengLEIGeneral;
	class FITKPHengLEIGeneralControl;
	class FITKPHengLEIInFlow;
	class FITKPHengLEIMeshPara;
	class FITKPHengLEIViscousModel;
	class FITKPHengLEIMultiGrid;
	class FITKPHengLEIAbstractInFlowType;
	class FITKPHengLEIDiscretisation;
	class FITKPHengLEIBoundaryManager;
	class FITKPHengLEIOutputSetting;
	class FITKPHengLEIerDriver;
	class FITKPHengLEIFlow;
	class FITKPHengLEICalculation;
	class FITKPHengLEIResidaulProcess;
	class FITKPHengLEIModels;
	class FITKPHengLEISolution;
	class FITKPHengLEIInitialCondition;
    /**
     * @brief PHengLEI物理场数据类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-11
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIData : public Core::FITKAbstractDataObject
    {
    public:
        /**
         * @brief Construct a new FITKPHengLEIData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        explicit FITKPHengLEIData();
        /**
         * @brief Destroy the FITKPHengLEIData object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual ~FITKPHengLEIData();
        /**
         * @brief 从应用程序框架中获取数据
         * @return FITKPHengLEIData* 
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        static FITKPHengLEIData* GetPHengLEIDataFromFrame();
		/**
		 * @brief 获取网格转化参数与网格分区参数
		 * @return  FITKPHengLEIMeshProcess
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-11
		 */
		FITKPHengLEIMeshProcess* getMeshProcess();
		/**
		 * @brief 获取求解器
		 * @return  FITKPHengLEIAbsSolver
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-09-11
		 */
        FITKPHengLEIAbsSolver* getSolver();
        /**
         * @brief 获取边界管理器类
         * @return  FITKPHengLEIBoundaryManager
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        FITKPHengLEIBoundaryManager* getBoundaryManager();
		FITKPHengLEIGeneral* getGeneral();
		/**
		 * @brief 获取通用控制参数类
		 * @return  FITKPHengLEIGeneralControl
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-13
		 */
		FITKPHengLEIGeneralControl* getGeneralControl();
		/**
		 * @brief 获取来流参数类
		 * @return FITKPHengLEIInFlow
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-14
		 */
		FITKPHengLEIInFlow* getInFlow();
		/**
		 * @brief 获取网格相关参数类
		 * @return FITKPHengLEIMeshPara
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-16
		 */
		FITKPHengLEIMeshPara* getMeshPara();
		/**
		 * @brief 获取粘性模型参数类
		 * @return  FITKPHengLEIViscousModel
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-16
		 */
		FITKPHengLEIViscousModel* getViscousModel();
		/**
		 * @brief 获取多重网格参数类
		 * @return  FITKPHengLEIMultiGrid
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-16
		 */
		FITKPHengLEIMultiGrid* getMultiGrid();
		/**
		 * @brief    设置求解器
		 * @param[i] solver
		 * @return   void
		 * @author   liuzhonghua (liuzhonghuaszch@163.com)
		 * @date     2024-08-21
		 */
		FITKPHengLEIDiscretisation* getDiscretisation();
		/**
		 * @brief 获取计算输出参数类
		 * @return  FITKPHengLEIOutputSetting
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIOutputSetting* getOutputSetting();
		/**
		 * @brief 获取风雷驱动器类
		 * @return  FITKPHengLEIerDriver
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIerDriver* getPHengLEIDirver();
		/**
		 * @brief 获取来流参数
		 * @params solver
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIFlow * getFlow();
		/**
		 * @brief   获取求解计算参数
		 * @return  
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEICalculation* getCalculation();
		/**
		 * @brief   获取残差参数
		 * @return  
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIResidaulProcess* getResidaulProcess();
		/**
		 * @brief   获取模型参数
		 * @return  
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIModels* getModels();
		/**
		 * @brief   获取Solution参数
		 * @return  
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISolution* getSolution();
		/**
		 * @brief   获取InitialCondition参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-01
		 */
		FITKPHengLEIInitialCondition* getInitialCondition();

		FITKPHengLEICouplingControl* getCouplingData();
		void setCouplingData(FITKPHengLEICouplingControl* couplingData);

		void setSolver(FITKPHengLEIAbsSolver* solver);
		void setGeneral(FITKPHengLEIGeneral* general);
		/**
		 * @brief 设置通用控制参数
		 * @params generalControl
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-09-14
		 */
		void setGeneralControl(FITKPHengLEIGeneralControl* generalControl);
		/**
		 * @brief 设置来流参数
		 * @params InFlow
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setInFlow(FITKPHengLEIInFlow* InFlow);
		/**
		 * @brief 设置网格相关参数
		 * @params MeshPara
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setMeshPara(FITKPHengLEIMeshPara* MeshPara);
		/**
		 * @brief 设置粘性模型
		 * @params viscousModel
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setViscousModel(FITKPHengLEIViscousModel* viscousModel);
		/**
		 * @brief 设置多重网格参数
		 * @params multiGrid
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setMultiGrid(FITKPHengLEIMultiGrid* multiGrid);
		/**
		 * @brief 设置离散参数
		 * @params discretisation
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setDiscretisation(FITKPHengLEIDiscretisation* discretisation);
		/**
		 * @brief 设置边界条件
		 * @params boundaryManager
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setBoundaryManager(FITKPHengLEIBoundaryManager* boundaryManager);
		/**
		 * @brief 设置计算输出参数
		 * @params outputSetting
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setOutputSetting(FITKPHengLEIOutputSetting* outputSetting);
		/**
		 * @brief   设置网格相关参数
		 * @param   meshProcess
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setMeshProcess(FITKPHengLEIMeshProcess* meshProcess);
		/**
		 * @brief   设置来流参数
		 * @param   flow
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setFlow(FITKPHengLEIFlow* flow);
		/**
		 * @brief   设置求解计算参数
		 * @param   calculation
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setCalculation(FITKPHengLEICalculation* calculation);
		/**
		 * @brief   设置残差相关参数
		 * @param   residaulProcess
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setResidaulProcess(FITKPHengLEIResidaulProcess* residaulProcess);
		/**
		 * @brief   设置模型参数
		 * @param   models
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setModels(FITKPHengLEIModels* models);
		/**
		 * @brief   设置Solution参数
		 * @param   solution
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setSolution(FITKPHengLEISolution* solution);
		/**
		 * @brief   设置初始条件参数
		 * @param   initialCondition
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-02
		 */
		void setInitialCondition(FITKPHengLEIInitialCondition* initialCondition);
    private:
		/**
		 * @brief 耦合控制参数
		 */
		FITKPHengLEICouplingControl* _couplingData{};
        /**
         * @brief 网格转换和网格分区
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
		FITKPHengLEIMeshProcess* _meshProcess{};

        /**
         * @brief 求解参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        FITKPHengLEIAbsSolver* _solver{};
        /**
         * @brief 边界管理器
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        FITKPHengLEIBoundaryManager* _boundaryManager{};
		FITKPHengLEIGeneral* _general{};
		/**
		 * @brief 通用控制参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @data 2024-09-13
		 */
		FITKPHengLEIGeneralControl* _generalControl{};
		/**
		 * @brief 来流参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @data 2024-10-05
		 */
		FITKPHengLEIInFlow* _inFlow{};
		/**
		 * @brief 网格相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIMeshPara* _meshPara{};
		/**
		 * @brief 粘性模型参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIViscousModel* _viscousModel{};
		/**
		 * @brief 多重网格参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIMultiGrid* _multiGrid{};
		/**
		 * @brief 离散参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIDiscretisation* _discretisation{};
		/**
		 * @brief 计算输出参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIOutputSetting* _outputSetting{};
		/**
		 * @brief 驱动器
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIerDriver* _driver;
		/**
		 * @brief   来流参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIFlow* _flow{};
		/**
		 * @brief   求解计算参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEICalculation* _calculation{};
		/**
		 * @brief   残差相关参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIResidaulProcess* _residaulProcess{};
		/**
		 * @brief   求解模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIModels* _models{};
		/**
		 * @brief   Solution参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISolution* _solution{};
		/**
		 * @brief   InitialCondition参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-01
		 */
		FITKPHengLEIInitialCondition* _initialCondition{};
    };
}


#endif
 