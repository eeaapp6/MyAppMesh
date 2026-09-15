/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIBoundaryFarfieldType.h
 * @brief  边界条件远场相关参数类
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-09-21
 *********************************************************************/
#ifndef __FITK_ABSTRACT_PHengLEI_BoundaryFarfieldType_H___
#define __FITK_ABSTRACT_PHengLEI_BoundaryFarfieldType_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITKPHengLEIAbsSolver.h"
#include "FITKPHengLEIBoundaryType.h"


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
	/**
	 * @brief  边界条件远场1相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FarfieldOne : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldOne();
		virtual ~FarfieldOne() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场1相关参数类
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件远场2相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FarfieldTwo : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldTwo();
		virtual ~FarfieldTwo() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场2相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件远场3相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI FarfieldThree : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldThree();
		virtual ~FarfieldThree() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场3相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件远场4相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI FarfieldFour : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldFour();
		virtual ~FarfieldFour() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场4相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件远场5相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI FarfieldFive : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldFive();
		virtual ~FarfieldFive() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场5相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件远场6相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI FarfieldSix : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldSix();
		virtual ~FarfieldSix() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场6相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件远场None相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI FarfieldNone : public FITKPHengLEIBoundaryType
	{
	public:
		explicit FarfieldNone();
		virtual ~FarfieldNone() = default;
	protected:
		/**
		 * @brief    初始化边界条件远场6相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};
}


#endif
