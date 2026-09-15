/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIInFlowTypeNondimensional.h
 * @brief  粘性模型数据
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef _FITKOF_FITKPHengLEIViscousType_H___
#define _FITKOF_FITKPHengLEIViscousType_H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIAbsSolver.h"

namespace Core
{
	class FITKParameter;
}

namespace Interface
{
	/**
	 * @brief  粘性模型基类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIViscousType : public Core::FITKAbstractNDataObject
	{
	public:
		explicit FITKPHengLEIViscousType();
		virtual ~FITKPHengLEIViscousType();
		/**
		 * @brief    获取粘性模型类型数据参数
		 * @return   Core::FITKParameter *
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		Core::FITKParameter* getViscousTypeParams();
	protected:
 		/**
		 * @brief    初始化函数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		virtual void init();
		/**
		 * @brief    初始化粘性模型类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		virtual void initViscousTypeParam() = 0;
	protected:
		/**
		 * @brief  粘性类型参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date   2024-08-21
		 */
		Core::FITKParameter* _viscousTypeParams{};
	};
}

namespace Interface
{
    /**
     * @brief  粘性模型Turbulent1eq类
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-08-21
     */
    class FITKFlowPHengLEIAPI Turbulent1eq : public FITKPHengLEIViscousType
    {
    public:
        explicit Turbulent1eq();
        virtual ~Turbulent1eq() = default;
    protected:
        /**
         * @brief    初始化粘性类型Turbulent1eq参数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-08-21
         */
        virtual void initViscousTypeParam() override;
    private:
    };
}

namespace Interface
{
	/**
	 * @brief  粘性类型Turbulent2eq类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI Turbulent2eq : public FITKPHengLEIViscousType
	{
	public:
		explicit Turbulent2eq();
		virtual ~Turbulent2eq() = default;
	protected:
		/**
		 * @brief    初始化粘性类型Turbulent2eq参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		virtual void initViscousTypeParam() override;
	private:
	};

	/**
	* @brief  粘性模型k-epsilon类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-08-21
	*/
	class FITKFlowPHengLEIAPI KEpsilon : public FITKPHengLEIViscousType
	{
	public:
		explicit KEpsilon();
		virtual ~KEpsilon() = default;
	protected:
		/**
		 * @brief    初始化粘性类型k-epsilon参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		virtual void initViscousTypeParam() override;
	private:
	};

	/**
	* @brief  粘性模型Spalart-Allmaras类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-08-21
	*/
	class FITKFlowPHengLEIAPI SpalartAllmaras : public FITKPHengLEIViscousType
	{
	public:
		explicit SpalartAllmaras();
		virtual ~SpalartAllmaras() = default;
	protected:
		/**
		 * @brief    初始化粘性类型Spalart-Allmaras参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-08-21
		 */
		virtual void initViscousTypeParam() override;
	private:
	};
}


#endif
