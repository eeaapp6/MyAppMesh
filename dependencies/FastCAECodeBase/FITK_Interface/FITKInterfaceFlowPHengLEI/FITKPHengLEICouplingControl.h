/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITK_PHENGLEI_COUPLINGCONTROL__H___
#define _FITK_PHENGLEI_COUPLINGCONTROL__H___
 
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
	 * @brief  通用控制参数类
	 * @author WangKai (aurora_starry@outlook.com)
	 * @date   2024-09-21
	 */
	class FITKFlowPHengLEIAPI FITKPHengLEICouplingControl : public Core::FITKAbstractNDataObject
	{
	public:
		/**
		 * @brief Construct a new FITKPHengLEIGeneralControl object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		FITKPHengLEICouplingControl();
		/**
		 * @brief Destroy the FITKPHengLEIGeneralControl object
		 * @author LiBaojun (libaojunqd@foxmail.com)
		 * @date 2024-07-18
		 */
		virtual ~FITKPHengLEICouplingControl();

		/**
		 * @brief 设置参与者名称
		 * @params data
		 */
		void setParticipantName(const QString& name);
		QString getParticipantName() const;
		/**
		 * @brief 设置耦合面名称
		 * @params data
		 */
		void setCouplingInterfaceName(const QString& name);
		QString getCouplingInterfaceName() const;
		/**
		 * @brief 设置耦合面网格1名称
		 * @params data
		 */
		void setCouplingMeshName_1(const QString& name);
		QString getCouplingMeshName_1() const;
		/**
		 * @brief 设置耦合面网格2名称
		 * @params data
		 */
		void setCouplingMeshName_2(const QString& name);
		QString getCouplingMeshName_2() const;
		/**
		 * @brief 设置耦合面网格1类型
		 * @params data
		 */
		void setCouplingMeshType_1(const QString& type);
		QString getCouplingMeshType_1() const;
		/**
		 * @brief 设置耦合面网格2类型
		 * @params data
		 */
		void setCouplingMeshType_2(const QString& type);
		QString getCouplingMeshType_2() const;

		/**
		 * @brief 设置写出数据名称
		 * @params data
		 */
		void setWriteDataName(const QString& name);
		QString getWriteDataName() const;
		/**
		 * @brief 设置读取数据名称
		 * @params data
		 */
		void setReadDataName(const QString& name);
		QString getReadDataName() const;

		/**
		 * @brief 设置流固耦合
		 * @params data
		 */
		bool isCouplingEnabled() const;
		void setCouplingEnabled(bool enabled);

	private:
		//QString _name{};
		/**
		 * @brief 通用控制参数
		 * @author WangKai (aurora_starry@outlook.com)
		 * @date 2024-10-05
		 */
		//Core::FITKParameter* _additionalControl{};
		
		/* @brief 参与者名称
         * @note 对应 getParticipantName() 和 setParticipantName()
         */
        QString _participantName{};
		/**
		 * @brief 耦合面名称
		 * @note 对应 getCouplingInterfaceName() 和 setCouplingInterfaceName()
		 */
		QString _couplingInterfaceName{};
		/**
		 * @brief 耦合面网格1名称
		 * @note 对应 getCouplingMeshName_1() 和 setCouplingMeshName_1()
		 */
		QString _couplingMeshName_1{};
		/**
		 * @brief 耦合面网格2名称
		 * @note 对应 getCouplingMeshName_2() 和 setCouplingMeshName_2()
		 */
		QString _couplingMeshName_2{};
		/**
		 * @brief 耦合面网格1类型
		 * @note 对应 getCouplingMeshType_1() 和 setCouplingMeshType_1()
		 */
		QString _couplingMeshType_1{};
		/**
		 * @brief 耦合面网格2类型
		 * @note 对应 getCouplingMeshType_2() 和 setCouplingMeshType_2()
		 */
		QString _couplingMeshType_2{};
		/**
		 * @brief 写出数据名称
		 * @note 对应 getWriteDataName() 和 setWriteDataName()
		 */
		QString _writeDataName{};
		/**
		 * @brief 读取数据名称
		 * @note 对应 getReadDataName() 和 setReadDataName()
		 */
		QString _readDataName{};
		/**
		 * @brief 设置是否进行耦合
		 * @note 对应 getIsCoupling() 和 setIsCoupling()
		 */
		bool _couplingEnabled{};

	};
}
#endif
