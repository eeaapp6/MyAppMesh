/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file GUIPickInfo.h
 * @brief 拾取相关参数管理
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2024-04-18
 *
 */
#ifndef __GUIPICK_INFO_H__
#define __GUIPICK_INFO_H__

#include <QObject>

#include "PickDataProviderAPI.h"

namespace GUI
{
    struct GUIPickInfoStru;

    /**
     * @brief 拾取相关参数管理
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-18
     */
    class PICKDATAPROVIDERAPI GUIPickInfo : public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief 拾取对象类型枚举值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        enum PickObjType
        {
            POBJNone = 0,
            POBJCustom, // 默认拾取模式。（整个形状）
            POBJVert,
            POBJEdge,
            POBJWire,
            POBJFace,
            POBJShell,
            POBJSolid,
            POBJAssembly,
            POBJShape, // 整个模型。
            POBJDirecEdge, // 边方向。
            POBJDirecFace, // 面方向
            POBJEdgeProjPos, // 边上投影位置
            POBJFaceProjPos // 面上投影位置
        };
        Q_ENUM(PickObjType);

        /**
         * @brief 拾取方式枚举值
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        enum PickMethod
        {
            PMNone = 0,
            PMIndividually,
            PMSingle
        };
        Q_ENUM(PickMethod);
        /**
         * @brief 清空拾取参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        static void ClearPickInfo();
        /**
         * @brief 设置拾取参数
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        static void SetPickInfo(GUIPickInfoStru info, int dataObjId = -1);
        /**
        /**
         * @brief 获取拾取参数
         * @return GUIPickInfoStru
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
        static GUIPickInfoStru GetPickInfo();
        /**
         * @brief 获取拾取方式
         * @param uiText Ui中的字符串
         * @return PickMethod
         * @author LiBaojun (libaojunqd@foxmail.com)
         * @date 2024-04-18
         */
         //       static PickMethod GetPickMethod(QString uiText);

    protected:
    private:
        GUIPickInfo() = default;
        ~GUIPickInfo() = default;

    };

    /**
     * @brief 拾取信息结构
     * @author LiBaojun (libaojunqd@foxmail.com)
     * @date 2024-04-18
     */
    struct GUIPickInfoStru
    {
        //拾取对象类型
        GUIPickInfo::PickObjType _pickObjType{ GUIPickInfo::POBJNone };
        //拾取方法
        GUIPickInfo::PickMethod _pickMethod{ GUIPickInfo::PMNone };
        //是否启用模型拾取
        bool _enableModelPick{ true };
        //是否启用基准元素拾取
        bool _enableDatumPick{ false };
        //是否启用草图拾取
        bool _enableSketchPick{ false };
        //是否拾取中心
        bool _enablePickCenter{ true };
    };

}

#endif
