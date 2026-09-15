/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPartPickDialog.h
 * @brief  Part拾取对话框
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-05
 *********************************************************************/
#ifndef _GUIPARTPICKDIALOG_H__
#define _GUIPARTPICKDIALOG_H__

#include "GUIMeshDialogAPI.h"
#include "GUIWidget/GUIDialogBase.h"
#include "PickDataProvider/GUIPickInfo.h"
#include "PickDataProvider/PickedData.h"
#include "FITK_Interface/FITKInterfaceModel/FITKModelEnum.h"

namespace Core {
    class FITKActionOperator;
}
namespace Ui {
    class GUIPartPickDialog;
}
namespace Radioss
{
    class FITKRadiossPart;
}
namespace Interface
{
    class FITKModelSet;
    class FITKMeshSurface;
    class FITKComponentManager;
    class FITKMeshSurfaceElement;
    class FITKAbstractModelComponent;
}
namespace GUI
{
    //单元类型 - 面索引
    typedef QPair<Interface::FITKModelEnum::FITKEleType, int> EleTypeIndex;
    // 单元类型， 单元ID
    typedef QHash < EleTypeIndex, QList<int> >  EleSurfaceInfo;

    class GUIMeshDialogAPI GUIPartPickDialog:public GUIDialogBase
    {
        Q_OBJECT
    public:
        /**
         * @brief    构造函数
         * @param[i] oper       操作器指针
         * @param[i] parent     主窗口
         * @param[i] pickType   拾取对象类型
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-06
         */
        GUIPartPickDialog(Core::FITKActionOperator* oper, QWidget* parent,GUIPickInfo::PickObjType pickType);
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-06
         */
        ~GUIPartPickDialog();
        /**
         * @brief    初始化
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-06
         */
        void init();
        /**
         * @brief    设置UI界面
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void setupUIForPickType();
    private slots:
        /**
         * @brief  更新拾取状态
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void updatePickStatus();
        /**
         * @brief    获取拾取信息
         * @return   GUIPickInfoStru
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        GUIPickInfoStru getCurrentPickInfo() const;
        /**
         * @brief    更新拾取数量
         * @param[i] count 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void updatePickCount(int count);
        /**
         * @brief  确认按钮点击槽
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void on_pushButton_OK_clicked();
        /**
         * @brief  取消按钮点击槽
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-06
         */
        void on_pushButton_Cancel_clicked();
        /**
         * @brief    更新元素按角度拾取值
         * @param[i] value 
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-07
         */
        void on_spinBox_EleByAngle_valueChanged(double value);
        void on_spinBox_PointByAngle_valueChanged(double value);
        /**
         * @brief    拾取完成操作器
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-06
         */
        void pickFinishedOper();
    private:
        /**
         * @brief    创建集合
         * @param[i] pickList 拾取链表
         * @param[i] comMgr   组件管理器
         * @param[i] name     集合名称
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        bool createSet(const QList<GraphData::PickedData*>& pickList, Interface::FITKComponentManager* comMgr,const QString& name);
        /**
         * @brief    创建表面
         * @param[i] pickList  拾取链表
         * @param[i] comMgr    组件管理器
         * @param[i] name      集合名称
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        bool createSurf(const QList<GraphData::PickedData*>& pickList, Interface::FITKComponentManager* comMgr, const QString& name);
        /**
         * @brief    获取表面信息
         * @param[i] pickData   拾取数据
         * @param[i] info       获取到的信息 key:单元ID， value:单元的表面索引
         * @param[i] part       网格对象
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        bool GetSurfInfo(GraphData::PickedData* pickData, QHash<int, QList<int>>& info, Radioss::FITKRadiossPart* part);
        /**
         * @brief    获取表面索引
         * @param[i] surfaceLabel  表面索引
         * @param[i] type          单元类型
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        int getSurfaceInxde(int surfaceLabel, Interface::FITKModelEnum::FITKEleType type);
        /**
         * @brief    获取组件管理器
         * @return   Interface::FITKComponentManager *
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        Interface::FITKComponentManager* getComponentMgr();
        /**
         * @brief    保存PYTHON脚本
         * @return   void
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-09-28
         */
        void writePythonScript(Interface::FITKAbstractModelComponent* component);
        QString getMemberListString(const QList<GraphData::PickedData*>& pickList);
    private:
        /**
         * @brief  拾取对象类型
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-07
         */
        GUIPickInfo::PickObjType _pickType;
        /**
         * @brief  ui指针
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-07
         */
        Ui::GUIPartPickDialog* _ui = nullptr;
        /**
         * @brief  操作器指针
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-07
         */
        Core::FITKActionOperator* _oper = nullptr;
    };
}
#endif // !_GUIPARTPICKDIALOG_H__
