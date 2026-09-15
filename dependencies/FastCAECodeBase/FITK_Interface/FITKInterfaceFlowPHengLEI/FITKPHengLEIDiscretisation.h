/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_Discretisation__H___
#define _FITK_PHENGLEI_Discretisation__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
	class FITKPHengLEIAbsSpatialDiscretisation;
	/**
	 * @brief  离散参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIDiscretisation : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIDiscretisation object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIDiscretisation();
		/**
		 * @brief Destroy the FITKPHengLEIDiscretisation object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		virtual ~FITKPHengLEIDiscretisation();
		/**
		 * @brief 获取时间离散参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getTemporalAdditionalData();

		/**
		 * @brief    获取空间离散类型参数
		 * @return   FITKOFAbsSolutionSolver *
		 * @author   liuzhonghua (liuzhonghuaszch@163.com)
		 * @date     2024-09-03
		 */
		FITKPHengLEIAbsSpatialDiscretisation* getSpatialDiscretisation();
		/**
		 * @brief    设置空间离散类型参数
		 * @param[i] solver
		 * @return   void
		 * @author   liuzhonghua (liuzhonghuaszch@163.com)
		 * @date     2024-09-03
		 */
		void setSpatialDiscretisation(FITKPHengLEIAbsSpatialDiscretisation* spatialDiscretisation);
		/**
		 * @brief   获取流动离散参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* getFlowDiscretisation();
		/**
		 * @brief   设置流动离散参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setFlowDiscretisation(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief 时间离散参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalTemporal{};
		/**
		 * @brief 空间离散类型
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIAbsSpatialDiscretisation* _spatialDiscretisation{};
		/**
		 * @brief   流动离散参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		Core::FITKParameter* _additionalFlowPara{};

	};
}
#endif
