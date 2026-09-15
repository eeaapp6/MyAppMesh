/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   FITKPHengLEIAbstractInFlowType.h
 * @brief  来流类型基类
* @author WangKai (wang_starry@outlook.com)
* @date 2024-10-05
 *********************************************************************/
#ifndef _FITKPHENGLEI_BOUNDARY___H___
#define _FITKPHENGLEI_BOUNDARY___H___
 

#include "FITK_Kernel/FITKEasyParam/FITKParameter.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include "FITK_Kernel/FITKCore/FITKAbstractDataManager.hpp"
#include "FITKInterfaceFlowPHengLEIAPI.h"

namespace Interface
{
	/**
	 * @brief  来流类型基类
	 * @author WangKai (wang_starry@outlook.com)
	 * @date 2024-10-05
	 */
	class FITKPHengLEIBoundaryType;
    class  FITKFlowPHengLEIAPI  FITKPHengLEIBoundary : public Core::FITKAbstractNDataObject,
        public Core::FITKParameter
    {
        Q_OBJECT
    public:
		/**
		 * @brief 边界条件类型
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        enum PHengLEIBoundaryType
        {
            PHBNone,
            PHBFarField,
            PHBWall,
			PHBInFlow,
			PhBOutFlow,
			PHBSYMMETRY,
			PHBPressureInlet,
			PHBPressureOutlet,
			PHBMassFlowInlet,
			PHBMassFlowOutlet
        };
        Q_ENUM(PHengLEIBoundaryType);
    public:
		/**
		 * @brief Construct a new FITKPHengLEIBoundary object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        explicit FITKPHengLEIBoundary(PHengLEIBoundaryType type);
		/**
		 * @brief Destroy the FITKPHengLEIBoundary object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        virtual ~FITKPHengLEIBoundary() = default;
        /**
         * @brief 获取网格模型面ID
         * @return  int
         * @author WangKai (wang_starry@outlook.com)
         * @date 2024-10-05
         */
        int getMeshBoundaryID();
		/**
		 * @brief 设置网格模型面ID
		 * @params id
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setMeshBoundaryID(const int & id);
		/**
		 * @brief 获取边界条件枚举值类型
		 * @return  PHengLEIBoundaryType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		PHengLEIBoundaryType getBoundaryEnumType();
		/**
		 * @brief 设置边界条件类型
		 * @params boundaryType
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		void setBoundaryType(FITKPHengLEIBoundaryType* boundaryType);
		/**
		 * @brief 获取边界条件类型类
		 * @return  
		 * @author 
		 * @date 2024-10-05
		 */
		FITKPHengLEIBoundaryType* getBoundaryType();
    private:
		/**
		 * @brief 网格模型面ID
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        int  _meshBoundaryID{ -1 };
		/**
		 * @brief 边界条件枚举值
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        PHengLEIBoundaryType _boundaryType{ PHBNone };
		/**
		 * @brief 边界条件
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
		FITKPHengLEIBoundaryType* _absboundType{};
    };

	/**
	* @brief  边界条件管理器
	* @author WangKai (wang_starry@outlook.com)
	* @date 2024-10-05
	*/
    class FITKFlowPHengLEIAPI FITKPHengLEIBoundaryManager :
        public Core::FITKAbstractDataManager<FITKPHengLEIBoundary>
    {
    public:
		/**
		 * @brief Construct a new FITKPHengLEIBoundaryManager object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        explicit FITKPHengLEIBoundaryManager() = default;
		/**
		 * @brief Destroy the FITKPHengLEIBoundaryManager object
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        virtual ~FITKPHengLEIBoundaryManager() = default;
		/**
		 * @brief 通过边界条件枚举值类型获取指定的边界条件
		 * @params t
		 * @return  FITKPHengLEIBoundary
		 * @author WangKai (wang_starry@outlook.com)
		 * @date 2024-10-05
		 */
        FITKPHengLEIBoundary* getBoundary(FITKPHengLEIBoundary::PHengLEIBoundaryType t);
		/**
		 * @brief    根据网格边界ID获取对应的boundary
		 * @param[i] boundaryID    网格边界ID
		 * @return   FITKOFBoundary*  边界对象
		 * @author   BaGuijun (baguijun@163.com)
		 * @date     2024-08-26
		 */
		FITKPHengLEIBoundary* getBoundary(int MeshBoundaryID);
    };




}


#endif
