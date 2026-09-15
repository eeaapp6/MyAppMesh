/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**********************************************************************
 * @file   GUIPickInfo.h
 * @brief  拾取相关参数
 * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
 * @date   2025-08-05
 *********************************************************************/
#ifndef _GUIPickInfo_H_
#define _GUIPickInfo_H_

#include <QObject>
#include "PickDataProviderAPI.h"

namespace Interface
{
    class FITKComponentManager;
}

namespace GUI
{
    struct GUIPickInfoStru;
    /**
     * @brief  拾取相关参数
     * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
     * @date   2025-08-05
     */
    class PickDataProviderAPI GUIPickInfo : public QObject
    {
        Q_OBJECT;
    public:
        /**
         * @brief  拾取对象类型枚举值
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-05
         */
        enum PickObjType {
            POBNone = 0,
            POBGeoVert = 1,                 //几何点
            POBGeoEdge = 2,                 //几何线
            POBGeoFace = 3,                 //几何面
            POBGeoSolid = 4,                //几何体
            POBMeshVert = 5,                //网格点
            POBMeshVertSurf = 6,            //网格表面点
            POBMeshCell = 7,                //网格单元
            POBMeshCellSurf = 8,            //网格表面单元
        };
        Q_ENUM(PickObjType);
        /**
         * @brief  拾取方式枚举值
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-05
         */
        enum PickMethod{
            PMNone = 0,
            PMIndividually,        //多选（追加拾取\框选）
                                   //Shift:追加拾取;
                                   //Ctrl:删除选择 
            PMByAngle,             // 角度算法拾取（点选，支持增量<Shift>/减量<Ctrl>）
            PMSingle               //单选
        };
        Q_ENUM(PickMethod);
        /**
         * @brief    清空拾取参数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        static void ClearPickInfo();
        /**
         * @brief    设置拾取参数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        static void SetPickInfo(GUIPickInfoStru info, int dataObjId = -1);
        /**
         * @brief    设置记录组件管理器
         * @param    mgr 组件管理器
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        static void SetComponentManager(Interface::FITKComponentManager* mgr);
        /**
         * @brief
         * @return   组件管理器
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        static Interface::FITKComponentManager* GetComponentManager();
        /**
         * @brief    获取拾取参数
         * @return   GUIPickInfoStru
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        static GUIPickInfoStru GetPickInfo();
    private:
        /**
         * @brief    构造函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        GUIPickInfo() = default;
        /**
         * @brief    析构函数
         * @author   ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date     2025-08-05
         */
        ~GUIPickInfo() = default;
    };

    /**
     * @brief  拾取信息结构体
         * @author ZhouZhaoYi (zhouzhaoyi@diso.cn)
         * @date   2025-08-05
     */
    struct GUIPickInfoStru
    {
        //拾取对象类型
        GUIPickInfo::PickObjType _pickObjType{ GUIPickInfo::POBNone };
        //拾取方法
        GUIPickInfo::PickMethod _pickMethod{ GUIPickInfo::PMNone };
        //拾取参数
        double _pickParam{ 0 };
        //是否创建集合/表面
        bool _createComponent{ false };
        //集合与表面名称
        QString _componentName{};
    };
}
#endif
