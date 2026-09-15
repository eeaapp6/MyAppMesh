/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 *
 * @file FITKOCCDXFReader.h
 * @brief DXF文件读取
 * @author LiBaojun (libaojunqd@foxmail.com)
 * @date 2025-03-20
 *
 */
#ifndef _FITK_OCC_DXF__READER__H___
#define _FITK_OCC_DXF__READER__H___

#include "FITKGeoCompOCCAPI.h"
#include <QString>

class TopoDS_Shape;
class TopoDS_Compound;
class BRep_Builder;
class dimeEntity;

namespace OCC
{
    /**
     * @class FITKOCCDXFReader
     * @brief 用于读取DXF文件并转换为OpenCASCADE TopoDS_Shape对象的类。
     * @author libaojun
     */
    class FITKGeoCompOCCAPI FITKOCCDXFReader
    {
    public:
        /**
         * @brief 构造函数，初始化DXF文件名。
         * @param fileName DXF文件的路径。
         */
        explicit FITKOCCDXFReader(const QString &fileName);

        /**
         * @brief 析构函数，释放资源。
         */
        virtual ~FITKOCCDXFReader();

        /**
         * @brief 读取DXF文件并生成TopoDS_Shape对象。
         * @return 如果读取成功，返回true；否则返回false。
         */
        bool read();

        /**
         * @brief 获取读取的TopoDS_Shape对象。
         * @return 读取的TopoDS_Shape对象的引用。
         */
        const TopoDS_Shape & getShape();

    private:
        /**
         * @brief 读取DXF文件中的点实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的点实体。
         */
        void readPoint(BRep_Builder* builder, dimeEntity* entity);

        /**
         * @brief 读取DXF文件中的线实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的线实体。
         */
        void readLine(BRep_Builder* builder, dimeEntity* entity);

        /**
         * @brief 读取DXF文件中的圆实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的圆实体。
         */
        void readCircle(BRep_Builder* builder, dimeEntity* entity);

        /**
         * @brief 读取DXF文件中的弧实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的弧实体。
         */
        void readArc(BRep_Builder* builder, dimeEntity* entity);

        /**
         * @brief 读取DXF文件中的椭圆实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的椭圆实体。
         */
        void readEllipse(BRep_Builder* builder, dimeEntity* entity);

        /**
         * @brief 读取DXF文件中的LWPolyLine实体并添加到TopoDS_Shape对象中。
         * @param builder BRep_Builder对象，用于构建TopoDS_Shape。
         * @param entity DXF文件中的LWPolyLine实体。
         */
        void readLWPolyLine(BRep_Builder* builder, dimeEntity* entity);

    private:
        QString _fileName{};       ///< DXF文件的路径。
        TopoDS_Compound* _resShape{}; ///< 读取的TopoDS_Shape对象。
    };
}

#endif