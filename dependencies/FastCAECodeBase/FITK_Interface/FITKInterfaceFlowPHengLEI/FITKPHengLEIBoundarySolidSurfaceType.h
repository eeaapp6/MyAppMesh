/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIBoundarySolidSurfaceType.h
 * @brief  边界条件Wall类型相关参数类
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-08-21
 *********************************************************************/
#ifndef __FITK_ABSTRACT_PHengLEI_BoundarySolidSurfaceType_H___
#define __FITK_ABSTRACT_PHengLEI_BoundarySolidSurfaceType_H___

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
	 * @brief  边界条件Wall类型1相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI WallOne : public FITKPHengLEIBoundaryType
	{
	public:
		explicit WallOne();
		virtual ~WallOne() = default;
	protected:
		/**
		 * @brief    初始化边界条件Wall类型1相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件Wall类型2相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI WallTwo : public FITKPHengLEIBoundaryType
	{
	public:
		explicit WallTwo();
		virtual ~WallTwo() = default;
	protected:
		/**
		 * @brief    初始化边界条件Wall类型2相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件Wall类型3相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI WallThree : public FITKPHengLEIBoundaryType
	{
	public:
		explicit WallThree();
		virtual ~WallThree() = default;
	protected:
		/**
		 * @brief    初始化边界条件Wall类型3相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件WallNone相关参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI WallNone : public FITKPHengLEIBoundaryType
	{
	public:
		explicit WallNone();
		virtual ~WallNone() = default;
	protected:
		/**
		 * @brief    初始化边界条件Wall类型3相关参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};
}


#endif
