/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIGmshSettings.h
 * @brief  Gmsh尺寸信息设置
 * @author guqingtao (15598887859@163.com)
 * @date   2025-06-19
 *********************************************************************/
#ifndef GUIGMSHSETTINGS_H
#define GUIGMSHSETTINGS_H

#include "FITK_Kernel/FITKCore/FITKAbstractGUIDialog.h"
#include "FITKGmshExeDriverAPI.h"

namespace Ui { class GUIGmshSettings; }
namespace Interface {
    class FITKGeoCommandList;
    class VirtualShape;
}
namespace Core
{
    class FITKActionOperator;
}
namespace Gmsh
{
    class FITKMesherDriverGmshExec;
}

namespace GUI {
    /**
     * @brief  Gmsh尺寸信息设置
     * @author guqingtao (15598887859@163.com)
     * @date   2025-06-05
     */
    class  FITKGmshExeDriverAPI GUIGmshSettings : public Core::FITKDialog
    {
        Q_OBJECT

    public:
        /**
         * @brief  构造
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        GUIGmshSettings(QWidget* parent = nullptr);
        /**
         * @brief    构造
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-17
         */
        GUIGmshSettings(Gmsh::FITKMesherDriverGmshExec* driver, QWidget* parent = nullptr);
        /**
         * @brief    析构
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        ~GUIGmshSettings();
        /**
         * @brief    是否启用拾取模型
         * @param[i] isEnable 
         * @return   void
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        void isEnablePickModel(bool isEnable);
        /**
         * @brief    获取拾取模型的虚拓扑
         * @return   QList<Interface::VirtualShape>
         * @author   liuzhonghua (liuzhonghuaszch@163.com)
         * @date     2025-10-21
         */
        QList<Interface::VirtualShape> getPickModelVirtualShape();

    private:
        /**
         * @brief    初始化
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        void init();
        /**
         * @brief    处理拾取到的数据
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-09-18
         */
        void pickFinishedOper(int pickType, int objectId, QHash<QString, void*> objInfo);
    private slots:
        /**
         * @brief  OK按钮
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief    Cancel按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-06-05
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    拾取按钮
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-02
         */
        void on_pushButton_Pick_clicked();
        /**
         * @brief    comboBox页切换
         * @param[i] index 
         * @return   void
         * @author   guqingtao (15598887859@163.com)
         * @date     2025-07-02
         */
        void on_comboBox_Type_currentIndexChanged(int index);
    signals:
        /**
         * @brief  
         * @param[i]  dim   划分维度，二维/三维
         * @param[i]  ids   对象ID列表
         * @author guqingtao (15598887859@163.com)
         * @date   2025-09-18
         */
        void gmshGeneratorObjectSig(int dim, QList<int> ids);
    private:
        /**
         * @brief  ui
         * @author guqingtao (15598887859@163.com)
         * @date   2025-06-05
         */
        Ui::GUIGmshSettings *_ui{};
        /**
         * @brief  是否启用拾取模型功能
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-21
         */
        bool _isEnablePickModel{ true };
        /**
         * @brief  Gmsh驱动器：用于存储界面设置的数据信息
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-17
         */
        Gmsh::FITKMesherDriverGmshExec* _driver{};
        /**
         * @brief  所选中的几何虚拓扑
         * @author liuzhonghua (liuzhonghuaszch@163.com)
         * @date   2025-10-20
         */
        QList<Interface::VirtualShape> _virtualTopos{};

    };
}
#endif 
