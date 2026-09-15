/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_Models__H___
#define _FITK_PHENGLEI_Models__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{
	class FITKPHengLEIEnergyModel;
	class FITKPHengLEISpeciesModel;
	class FITKPHengLEIViscousModel;
	/**
	 * @brief  模型参数类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIModels : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIModels object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIModels();
		/**
		 * @brief Destroy the FITKPHengLEIModels object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIModels();
		/**
		 * @brief   获取Energy模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIEnergyModel* getEnergyModel();
		/**
		 * @brief   设置Energy模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setEnergyModel(FITKPHengLEIEnergyModel* model);
		/**
		 * @brief   获取Species模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISpeciesModel* getSpeciesModel();
		/**
		 * @brief   设置SPecies模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		void setSpeciesModel(FITKPHengLEISpeciesModel* model);
		/**
		 * @brief   获取湍流模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		FITKPHengLEIViscousModel* getViscousModel();
		/**
		 * @brief   设置湍流模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		void setViscousModel(FITKPHengLEIViscousModel* model);
	private:
		QString _name{};
		/**
		 * @brief   Energy模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEIEnergyModel* _energyModel{};
		/**
		 * @brief   Species模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-28
		 */
		FITKPHengLEISpeciesModel* _speciesModel{};
		/**
		 * @brief   湍流模型参数
		 * @author  WangKai (aurora_starry@outlook.com)
		 * @date    2025-07-31
		 */
		FITKPHengLEIViscousModel* _viscousModel{};
	};
}
#endif
