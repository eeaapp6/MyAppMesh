/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef __FITK_ABSTRACT_PHengLEI_BoundaryType_H___
#define __FITK_ABSTRACT_PHengLEI_BoundaryType_H___

#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"

namespace Core
{
    class FITKParameter;
}

namespace Interface
{
    /**
     * @brief  边界条件类型基类
     * @author WangKai (wang_starry@outlook.com)
     * @date   2024-09-21
     */
    class FITKFlowPHengLEIAPI FITKPHengLEIBoundaryType: public Core::FITKAbstractNDataObject
    {
    public:
        explicit FITKPHengLEIBoundaryType();
        virtual ~FITKPHengLEIBoundaryType();

        /**
         * @brief    获取边界条件类型数据参数
         * @return   Core::FITKParameter *
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-09-21
         */
        Core::FITKParameter* getBoundaryTypeParams();
    protected:
        /**
         * @brief    初始化函数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-09-21
         */
        virtual void init();
        /**
         * @brief    初始化边界条件类型参数
         * @return   void
         * @author   WangKai (wang_starry@outlook.com)
         * @date     2024-09-21
         */
        virtual void initBoundaryTypeParam() = 0;
    protected:
		/**
		 * @brief    边界条件类型相关参数
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		Core::FITKParameter* _boundaryTypeParams{};
    };

	/**
	 * @brief  边界条件PressureInlet类型类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI PressureInlet : public FITKPHengLEIBoundaryType
	{
	public:
		explicit PressureInlet();
		virtual ~PressureInlet() = default;
	protected:
		/**
		 * @brief    初始化边界条件PressureInlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
 * @brief  边界条件PressureOutlet类
 * @author WangKai (wang_starry@outlook.com)
 * @date   2024-09-21
 */
	class FITKFlowPHengLEIAPI PressureOutlet : public FITKPHengLEIBoundaryType
	{
	public:
		explicit PressureOutlet();
		virtual ~PressureOutlet() = default;
	protected:
		/**
		 * @brief    初始化边界条件PressureOutlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
* @brief  边界条件MassFlowInlet类
* @author WangKai (wang_starry@outlook.com)
* @date   2024-09-21
*/
	class FITKFlowPHengLEIAPI MassFlowInlet : public FITKPHengLEIBoundaryType
	{
	public:
		explicit MassFlowInlet();
		virtual ~MassFlowInlet() = default;
	protected:
		/**
		 * @brief    初始化边界条件MassFlowInlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	* @brief  边界条件MassFlowOutlet类
	* @author WangKai (wang_starry@outlook.com)
	* @date   2024-09-21
	*/
	class FITKFlowPHengLEIAPI MassFlowOutlet : public FITKPHengLEIBoundaryType
	{
	public:
		explicit MassFlowOutlet();
		virtual ~MassFlowOutlet() = default;
	protected:
		/**
		 * @brief    初始化边界条件MassFlowOutlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件SYMMETRY类型类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI SYMMETRY : public FITKPHengLEIBoundaryType
	{
	public:
		explicit SYMMETRY();
		virtual ~SYMMETRY() = default;
	protected:
		/**
		 * @brief    初始化边界条件PressureInlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};

	/**
	 * @brief  边界条件OutFLow类型类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI OutFLow : public FITKPHengLEIBoundaryType
	{
	public:
		explicit OutFLow();
		virtual ~OutFLow() = default;
	protected:
		/**
		 * @brief    初始化边界条件PressureInlet类型参数
		 * @return   void
		 * @author   WangKai (wang_starry@outlook.com)
		 * @date     2024-09-21
		 */
		virtual void initBoundaryTypeParam() override;
	private:
	};
}


#endif
