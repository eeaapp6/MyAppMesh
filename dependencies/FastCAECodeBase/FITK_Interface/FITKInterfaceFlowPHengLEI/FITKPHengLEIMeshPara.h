/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_MESHPARA__H___
#define _FITK_PHENGLEI_MESHPARA__H___
 
#include "FITKInterfaceFlowPHengLEIAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QString>


namespace Core
{
    class FITKParameter;
}

namespace Interface
{ 
	/**
	 * @brief  网格相关参数类
	 * @author liuzhonghua (liuzhonghuaszch@163.com)
	 * @date   2024-08-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEIMeshPara : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIMeshPara object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEIMeshPara();
		/**
		 * @brief Destroy the FITKPHengLEIMeshPara object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEIMeshPara();
		/**
		 * @brief 获取网格相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalMeshData();
		/**
		 * @brief 设置网格相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalMeshData(Core::FITKParameter* data);
		/**
		 * @brief 获取ForceReference相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalForceReference();
		/**
		 * @brief 设置ForceReference相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalForceReference(Core::FITKParameter* data);
		/**
		 * @brief 获取ReferencePoint相关参数
		 * @return  FITKParameter
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* getAdditionalReferencePoint();
		/**
		 * @brief 设置ReferencePoint相关参数
		 * @params data
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setAdditionalReferencePoint(Core::FITKParameter* data);
	private:
		QString _name{};
		/**
		 * @brief 网格相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalMeshPara{};
		/**
		 * @brief ForceReference相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalForceReference{};
		/**
		 * @brief ReferencePoint相关参数
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		Core::FITKParameter* _additionalReferencePoint{};
	};
}
#endif
