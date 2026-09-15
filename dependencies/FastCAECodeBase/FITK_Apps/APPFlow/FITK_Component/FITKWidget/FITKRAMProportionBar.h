/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   RAMProportionBar.h
 * @brief  运行内存占比信息条
 * @author liuzhonghua (liuzhonghuaszch@163.com)
 * @date   2025-11-26
 *********************************************************************/
#ifndef __FITK_RAMPROPORTIONBAR_H__
#define __FITK_RAMPROPORTIONBAR_H__

#include "FITKWidgetAPI.h"
#include "FITK_Kernel/FITKCore/FITKAbstractGUI.h"

class QTimer;

namespace Ui
{
    class FITKRAMProportionBar;
}

namespace GUI
{
    /**
     * @brief  运行内存占比信息条
     * @author liuzhonghua (liuzhonghuaszch@163.com)
     * @date   2025-11-26
     */
    class FITKWidgetAPI FITKRAMProportionBar : public Core::FITKWidget
    {
        Q_OBJECT
    public:
        /**
         * @brief  Construct a new RAM Proportion Bar widget
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-26
         */
        explicit FITKRAMProportionBar(QWidget *parent = nullptr);
        /**
         * @brief  Destroy the RAM Proportion Bar widget
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-26
         */
        virtual ~FITKRAMProportionBar();

        /**
         * @brief    显示或隐藏内存条
         * @param[i] isShow 是否显示
         * @return   void
         * @author   libaojun 
         * @date     2025-12-12
         */
        void showMemBar(bool isShow);
        /**
         * @brief    显示或隐藏内存标签
         * @param[i] isShow 是否显示
         * @return   void
         * @author   libaojun 
         * @date     2025-12-12
         */
        void showRAMLabel(bool isShow);
    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-26
         */
        void init();
        /**
         * @brief    获取系统内存
         * @param[o] max 系统最大内存
         * @param[o] cost 系统当前内存
         * @param[o] currProcess 程序占用内存
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-27
         */
        void getSystemMemoryKBs(double& max, double& cost, double& currProcess);
        /**
         * @brief    内存单位转换
         * @param[i] kbs KBs单位数据
         * @param[o] val 
         * @param[o] unit 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-11-27
         */
        void memUnitTransKBs(double kbs, double& val, QString& unit);
        

    private slots:
        /**
         * @brief  内存定时器超出槽
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-11-27
         */
        void slot_memoryTimerOut();

    private:
        Ui::FITKRAMProportionBar *_ui{ nullptr };

        QTimer* _timerMem{nullptr};
    };

}  // namespace EventOper

#endif