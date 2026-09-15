/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file FITKPHengLEIAbsSolver.h
 * @brief 求解器抽象类 
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-09-11
 * 
 */
#ifndef _FITKABS_PHENGLEI_SOLVER___H___
#define _FITKABS_PHENGLEI_SOLVER___H___

#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITKPHengLEIBoundary.h"
#include <QHash>

namespace Interface
{

	class FITKPHengLEIGeneralControl;
	class FITKPHengLEIInFlow;
	class FITKPHengLEIMeshPara;
	class FITKPHengLEIViscousModel;
	class FITKPHengLEIMultiGrid;
	class FITKPHengLEIInFlow;
	class FITKPHengLEIDiscretisation;
	class FITKPHengLEIOutputSetting;
	class FITKPHengLEIFlow;
	class FITKPHengLEICalculation;
	class FITKPHengLEIInitialCondition;
	class FITKPHengLEISolution;
	class FITKPHengLEIModels;
    /**
     * @brief 求解器抽象类，亚音速 跨音速等求解器的父类
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-09-11
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIAbsSolver 
        : public Core::FITKAbstractNDataObject, public Core::FITKParameter
    {
        Q_OBJECT
    public:
		/**
		 * @brief 求解器过滤类型
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-06-12
		 */
		enum FITKPHengLEISolverFiltersType
		{
			NoneFiltersType,      /// 无过滤类型
			SteadyState,          /// 稳态问题，流体特性不随时间变化
			Transient,            /// 瞬态问题，流体特性随时间变化
			Incompressible,       /// 不可压缩流体
			Compressible,         /// 可压缩流体
			UserDefined,          /// 用户自定义过滤类型
		};
		Q_ENUM(FITKPHengLEISolverFiltersType);
        /**
        * @brief 求解器类型
        * @author LiBaojun (libaojunqd@foxmail.com)
        * @date 2024-09-12
        */
        enum PHengLEISolverType
        {
            PLSolverNone,    ///< 空
            PLSubsonic,     ///< 亚音速
            PLTransonic,    ///< 跨音速
            PLSupersonic,   ///< 超音速
            PLHypersonic,   ///< 高超音速
			PLIncompressible, /// < 不可压缩
			PLCompressibleSIMPLE, /// < 可压缩SIMPLE
			PLPISO,         /// < PISO
			PLSIMPLE,       /// < SIMPLE
        };
        Q_ENUM(PHengLEISolverType);

		enum PHengLEIViscousType
		{
			None,          /// 无过滤类型
			Lamilar,       /// Lamilar
			Eular,         /// Eular
			RANS,          /// RANS
			DES,           /// DES
			DDES,          /// DDES
			IDDES,         /// IDDES
		};
		Q_ENUM(PHengLEIViscousType);
    public:
        /**
         * @brief Construct a new FITKPHengLEIAbsSolver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        explicit FITKPHengLEIAbsSolver();
        /**
         * @brief Destroy the FITKPHengLEIAbsSolver object
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual ~FITKPHengLEIAbsSolver();
        /**
         * @brief 获取求解器类型
         * @return PHengLEISolverType
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-12
         */
        virtual PHengLEISolverType getSolverType() = 0;
		/**
		 * @brief 获取通用控制参数类
		 * @return  FITKPHengLEIGeneralControl
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIGeneralControl* getGeneralControl();
		/**
		 * @brief 获取来流参数类
		 * @return  FITKPHengLEIInFlow
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIInFlow* getInFlow();
		/**
		 * @brief 获取网格相关参数类
		 * @return  FITKPHengLEIMeshPara
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIMeshPara* getMeshPara();

		/**
		 * @brief 获取粘性模型参数类
		 * @return  FITKPHengLEIViscousModel
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIViscousModel* getViscousModel();
		/**
		 * @brief 获取多重网格参数类
		 * @return  FITKPHengLEIMultiGrid
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIMultiGrid* getMultiGrid();
		/**
		 * @brief 获取离散参数类
		 * @return  FITKPHengLEIDiscretisation
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIDiscretisation* getDiscretisation();
		/**
		 * @brief 获取边界条件
		 * @params bType
		 * @return  FITKPHengLEIBoundary
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIBoundary* getBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType bType);
		/**
		 * @brief 获取计算输出参数
		 * @return  FITKPHengLEIOutputSetting
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIOutputSetting* getOutputSetting();
		/**
		 * @brief 获取flow参数
		 * @return  FITKPHengLEIOutputSetting
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIFlow* getFlow();
		/**
		 * @brief   获取求解计算参数
		 * @return  
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEICalculation* getCalculation();
		/**
		 * @brief   获取Solution参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISolution* getSolution();
		/**
		 * @brief   获取Model参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIModels* getModels();
		/**
		 * @brief   获取InitialCondition参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-08-01
		 */
		FITKPHengLEIInitialCondition* getInitialCondition();
    protected:
        /**
         * @brief 初始化参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        void init();
        /**
         * @brief 初始化边界条件
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initBoundaryTypes() = 0;
        /**
         * @brief 初始化通用控制信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initBasicInfo() = 0;
        /**
         * @brief 初始化来流信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initInflow() = 0;
        /**
         * @brief 初始化网格信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initGridParam()=0;
        /**
         * @brief 初始化粘度信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initViscous()=0;
        /**
         * @brief 初始化离散信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initDiscrete()=0;
        /**
         * @brief 初始化多重网格信息
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-09-11
         */
        virtual void initMultiGridParam() =0;
		/**
		 * @brief 初始化风雷输出设置信息
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		virtual void initOutputSetting() = 0;

		/**
		 * @brief 初始化flow参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		virtual void initFlow() = 0;
		/**
		 * @brief   初始化求解计算参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		virtual void initCalculation() = 0;
		/**
		 * @brief   初始化初始条件参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		virtual void initInitialCondition() = 0;
		/**
		 * @brief   初始化Solution参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		virtual void initSolution() = 0;
		/**
		 * @brief   初始化Model参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		virtual void initModels() = 0;
    protected:
        /**
       * @brief 边界管理器
       * @author LiBaojun (libaojunqd@foxmail.com)
       * @date 2024-09-12 
       */
        FITKPHengLEIBoundaryManager* _boundarys{};
		/**
		 * @brief 通用控制参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIGeneralControl* _generalControl{};
		/**
		 * @brief 来流参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIInFlow* _inFlow{};
		/**
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
		FITKPHengLEIMultiGrid* _multiGRID{};
		/**
		 * @brief 初始化离散参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIDiscretisation* _discretisation{};
		/**
		 * @brief 初始化风雷输出设置参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIOutputSetting* _outputSetting{};
		/**
		 * @brief 初始化flow参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIFlow* _flow{};
		/**
		 * @brief   求解计算参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEICalculation* _calculation{};
		/**
		 * @brief   初始条件参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIInitialCondition* _initialCondition{};
		/**
		 * @brief   Solution参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISolution* _solution{};
		/**
		 * @brief   Model参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIModels* _models{};
    };

}


#endif
