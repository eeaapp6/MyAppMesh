/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef _FITKCFDPostColorLibaryData_H
#define _FITKCFDPostColorLibaryData_H

#include "FITKInterfaceCFDPostAPI.h"
#include "FITKCFDPostEnum.h"
#include "FITK_Kernel/FITKCore/FITKAbstractNDataObject.h"
#include <QObject>
#include <QColor>

namespace Interface
{
    /**
     * @brief  后处理色库数据数据类
     * @author BaGuijun (baguijun@163.com)
     * @date   2024-10-25
     */
    class FITKInterfaceCFDPostAPI FITKCFDPostColorLibaryData : public Core::FITKAbstractNDataObject
    {
        FITKCLASS(Interface, FITKCFDPostColorLibaryData);
    public:
        /**
         * @brief    构造函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        FITKCFDPostColorLibaryData();
        /**
         * @brief    析构函数
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        ~FITKCFDPostColorLibaryData();
        /**
         * @brief    清空
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void clear();
        /**
         * @brief    获取所有的颜色数据
         * @return   QHash<double,double*>     颜色数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        QMap<double, FITKPostColorStr> getColorPoints();
        /**
         * @brief    设置所有颜色数据
         * @param[i] colorPoints               颜色数据
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        void setColorPoints(QMap<double, FITKPostColorStr> colorPoints);
        //插入颜色数据
        void insertColorPoints(double pos, FITKPostColorStr color);
        /**
         * @brief    获取颜色
         * @param[i] pos             位置[0-1]对应（0-255）
         * @return   QColor          颜色
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-28
         */
        QColor getColor(double pos);
        /**
         * @brief    读取json
         * @param[i] file        文件
         * @return   bool        是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        bool readColorJson(QString filePath);
        /**
         * @brief    写出json
         * @param[i] file        文件
         * @return   bool        是否成功
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-25
         */
        bool writeColorJson(QString filePath);
        /**
         * @brief    拷贝
         * @param[i] colorLibData 
         * @return   bool
         * @author   BaGuijun (baguijun@163.com)
         * @date     2024-10-29
         */
        bool copy(FITKCFDPostColorLibaryData* colorLibData);
    private:
        /**
         * @brief  颜色点<位置（0-1），颜色值[3]>
         * @author BaGuijun (baguijun@163.com)
         * @date   2024-10-25
         */
        QMap<double, FITKPostColorStr> _colorPoints = {};
    };
}

#endif


