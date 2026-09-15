/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIBoundaryFarfieldType.h
 * @brief  边界条件来流类型相关参数类
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-09-22
 *********************************************************************/
#ifndef __FITK_ABSTRACT_PHengLEI_BoundaryInFlowType_H___
#define __FITK_ABSTRACT_PHengLEI_BoundaryInFlowType_H___

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
	 * @brief  边界条件来流类型1相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-22
	 */
	class FITKFlowPHengLEIAPI InflowOne : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowOne();
		virtual ~InflowOne() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型1相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件来流类型2相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-22
	 */
	class FITKFlowPHengLEIAPI InflowTwo : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowTwo();
		virtual ~InflowTwo() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型2相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型3相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowThree : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowThree();
		virtual ~InflowThree() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型3相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型4相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowFour : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowFour();
		virtual ~InflowFour() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型4参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型5相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowFive : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowFive();
		virtual ~InflowFive() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型5相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型6相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowSix : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowSix();
		virtual ~InflowSix() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型6相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型6相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowNone : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowNone();
		virtual ~InflowNone() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型6相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件来流类型7相关参数类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-22
	*/
	class FITKFlowPHengLEIAPI InflowSeven : public FITKPHengLEIBoundaryType
	{
	public:
		explicit InflowSeven();
		virtual ~InflowSeven() = default;
	protected:
		/**
		 * @brief    初始化边界条件来流类型7相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-22
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};
}


#endif
