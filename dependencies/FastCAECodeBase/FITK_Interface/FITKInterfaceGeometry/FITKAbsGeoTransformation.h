/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * @file   FITKAbsGeoTransformation.h
 * @brief  模型变换抽象类.
 * @author YanZhiHui (chanyuantiandao@126.com)
 * @date   2024-09-10
 */
#ifndef FITKABSGEOTRANSFORMATION_H
#define FITKABSGEOTRANSFORMATION_H

#include "FITKAbsGeoCommand.h"
#include "FITKInterfaceGeometryAPI.h"
#include <array>

namespace Interface {
    /**
     * @brief  变换基类.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoTransformation :
        public FITKAbsGeoCommand
    {
        FITKCLASS(Interface, FITKAbsGeoTransformation);
    public:
        FITKAbsGeoTransformation() = default;
        virtual ~FITKAbsGeoTransformation() = default;

        VirtualShape sourceShape() const { return m_SourceShape; }
        void setSourceShape(VirtualShape shape) { m_SourceShape = shape; }
        bool isCopy() const { return m_IsCopy; }
        void setCopy(bool copy) { m_IsCopy = copy; }

    protected:
        /**
         * @brief  原始形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        VirtualShape m_SourceShape{};
        /**
         * @brief  是否拷贝原形状.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        bool m_IsCopy{ false };
    };
    /**
     * @brief  平移变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTransform :
        public FITKAbsGeoTransformation
    {
        FITKCLASS(Interface, FITKAbsGeoModelTransform);
    public:
        FITKAbsGeoModelTransform() = default;
        ~FITKAbsGeoModelTransform() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> vector() const { return m_Vector; }
        void setVector(std::array<double, 3> vec) { m_Vector = vec; }
        void setVector(double x, double y, double z) { m_Vector = { x,y,z }; }
    protected:
        /**
         * @brief  平移向量.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_Vector{};
    };
    /**
     * @brief  通过两个点平移变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTransformByTwoPoints :
        public FITKAbsGeoModelTransform
    {
        FITKCLASS(Interface, FITKAbsGeoModelTransformByTwoPoints);
    public:
        FITKAbsGeoModelTransformByTwoPoints() = default;
        ~FITKAbsGeoModelTransformByTwoPoints()override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> startPoint() const { return m_StartPoint; }
        void setStartPoint(std::array<double, 3> vec) { m_StartPoint = vec; }
        void setStartPoint(double x, double y, double z) { m_StartPoint = { x,y,z }; }
        std::array<double, 3> endPoint() const { return m_EndPoint; }
        void setEndPoint(std::array<double, 3> vec) { m_EndPoint = vec; }
        void setEndPoint(double x, double y, double z) { m_EndPoint = { x,y,z }; }
    protected:
        /**
         * @brief  起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_StartPoint{};
        /**
         * @brief  终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_EndPoint{};
    };
    /**
     * @brief  通过方向和距离平移变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelTransformByDirAndDis :
        public FITKAbsGeoModelTransform
    {
        FITKCLASS(Interface, FITKAbsGeoModelTransformByDirAndDis);
    public:
        FITKAbsGeoModelTransformByDirAndDis() = default;
        ~FITKAbsGeoModelTransformByDirAndDis()override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> direction() const { return m_Direction; }
        void setDirection(std::array<double, 3> dir) { m_Direction = dir; }
        void setDirection(double x, double y, double z) { m_Direction = { x,y,z }; }
        double distance() const { return m_Distance; }
        void setDistance(double dis) { m_Distance = dis; }
    protected:
        /**
         * @brief  平移方向.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_Direction{};
        /**
         * @brief  平移距离.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        double m_Distance{};
    };
    /**
     * @brief  旋转变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRotate :
        public FITKAbsGeoTransformation
    {
        FITKCLASS(Interface, FITKAbsGeoModelRotate);
    public:
        FITKAbsGeoModelRotate() = default;
        ~FITKAbsGeoModelRotate() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> axisStartPoint() const { return m_AxisStartPoint; }
        void setAxisStartPoint(std::array<double, 3> xyz) { m_AxisStartPoint = xyz; }
        void setAxisStartPoint(double x, double y, double z) { m_AxisStartPoint = { x,y,z }; }
        std::array<double, 3> axisEndPoint() const { return m_AxisEndPoint; }
        void setAxisEndPoint(std::array<double, 3> xyz) { m_AxisEndPoint = xyz; }
        void setAxisEndPoint(double x, double y, double z) { m_AxisEndPoint = { x,y,z }; }
        double degree() const { return m_Angle / 3.14159265358979323846 * 180; }
        void setDegree(double deg) { m_Angle = deg / 180 * 3.14159265358979323846; }
        double radian() const { return m_Angle; }
        void setRadian(double rad) { m_Angle = rad; }
    protected:
        /**
         * @brief  旋转轴起始点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_AxisStartPoint{};
        /**
         * @brief  旋转轴终止点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_AxisEndPoint{};
        /**
         * @brief  旋转角度(弧度值).
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        double m_Angle{};
    };

    /**
     * @brief  缩放变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelScale :
        public FITKAbsGeoTransformation
    {
        FITKCLASS(Interface, FITKAbsGeoModelScale);
    public:
        FITKAbsGeoModelScale() = default;
        ~FITKAbsGeoModelScale()override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> factors() const { return m_Factors; }
        void setFactors(std::array<double, 3> factors) { m_Factors = factors; }
        void setFactors(double x, double y, double z) { m_Factors = { x, y, z }; }
        std::array<double, 3> basePoint() const { return m_BasePoint; }
        void setBasePoint(std::array<double, 3> point) { m_BasePoint = point; }
        void setBasePoint(double x, double y, double z) { m_BasePoint = { x, y, z }; }
    protected:
        /**
         * @brief  缩放因子.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_Factors{};
        /**
         * @brief  缩放基准点.
         * @author YanZhiHui (chanyuantiandao@126.com)
         * @date   2024-09-10
         */
        std::array<double, 3> m_BasePoint{ { 0, 0, 0 } };
    };

    /**
     * @brief  镜像变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelMirror :
        public FITKAbsGeoTransformation
    {
        FITKCLASS(Interface, FITKAbsGeoModelMirror);
    public:
        FITKAbsGeoModelMirror() = default;
        ~FITKAbsGeoModelMirror() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        Interface::VirtualShape reference() const { return m_Reference; }
        void setReference(Interface::VirtualShape ref) { m_Reference = ref; }
    protected:
        /**
         * @brief  镜像轴（可以是点，线或面）.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        Interface::VirtualShape m_Reference{};

    };

    /**
     * @brief  阵列变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelPattern :
        public FITKAbsGeoTransformation
    {
        Q_OBJECT
        FITKCLASS(Interface, FITKAbsGeoModelPattern);
    public:
        FITKAbsGeoModelPattern() = default;
        ~FITKAbsGeoModelPattern() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;
        // 序号对应界面的index，修改请注意
        enum Definition {
            InstanceAndSpacing = 0,
            InstanceAndTotal = 1,
            SpacingAndTotal = 2
        };
        Q_ENUM(Definition);

        Definition firstDefinition() const { return m_FirstDefinition; }
        void setFirstDefinition(Definition def) { m_FirstDefinition = def; }
        Definition secondDefinition() const { return m_SecondDefinition; }
        void setSecondDefinition(Definition def) { m_SecondDefinition = def; }

    protected:
        /**
         * @brief  第一方向的参数定义方式.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        Definition m_FirstDefinition{};
        /**
         * @brief  第二方向的参数定义方式.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        Definition m_SecondDefinition{};
    };


    /**
     * @brief  矩形阵列变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelRectangularPattern :
        public FITKAbsGeoModelPattern
    {
        FITKCLASS(Interface, FITKAbsGeoModelRectangularPattern);
    public:
        FITKAbsGeoModelRectangularPattern() = default;
        ~FITKAbsGeoModelRectangularPattern() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        std::array<double, 3> firstDirection() { return m_FirstDirection; }
        void setFirstDirection(std::array<double, 3> dir) { m_FirstDirection = dir; }
        void setFirstDirection(double x, double y, double z) { m_FirstDirection = { x,y,z }; }
        int firstCount() const { return m_FirstCount; }
        void setFirstCount(int count) { m_FirstCount = count; }
        double firstSpacing() const { return m_FirstSpacing; }
        void setFirstSpacing(double spacing) { m_FirstSpacing = spacing; }
        double firstTotal() const { return m_FirstTotal; }
        void setFirstTotal(double total) { m_FirstTotal = total; }

        std::array<double, 3> secondDirection() { return m_SecondDirection; }
        void setSecondDirection(std::array<double, 3> dir) { m_SecondDirection = dir; }
        void setSecondDirection(double x, double y, double z) { m_SecondDirection = { x,y,z }; }
        int secondCount() const { return m_SecondCount; }
        void setSecondCount(int count) { m_SecondCount = count; }
        double secondSpacing() const { return m_SecondSpacing; }
        void setSecondSpacing(double spacing) { m_SecondSpacing = spacing; }
        double secondTotal() const { return m_SecondTotal; }
        void setSecondTotal(double total) { m_SecondTotal = total; }

    protected:
        /**
         * @brief  第一方向.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        std::array<double, 3> m_FirstDirection{ {1,0,0} };
        /**
         * @brief  第一方向上的数量.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        int m_FirstCount{ 2 };
        /**
         * @brief  第一方向上的实例间隔距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_FirstSpacing{ 20 };
        /**
         * @brief  第一方向上的总距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_FirstTotal{ 20 };
        /**
         * @brief  第二方向.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        std::array<double, 3> m_SecondDirection{ {1,0,0} };
        /**
         * @brief  第二方向上的数量.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        int m_SecondCount{ 1 };
        /**
         * @brief  第二方向上的实例间隔距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_SecondSpacing{ 20 };
        /**
         * @brief  第二方向上的总距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_SecondTotal{ 20 };
    };

    /**
     * @brief  环形阵列变换.
     * @author YanZhiHui (chanyuantiandao@126.com)
     * @date   2024-09-10
     */
    class FITKInterfaceGeometryAPI FITKAbsGeoModelCircularPattern :
        public FITKAbsGeoModelPattern
    {
        FITKCLASS(Interface, FITKAbsGeoModelCircularPattern);
    public:
        FITKAbsGeoModelCircularPattern() = default;
        ~FITKAbsGeoModelCircularPattern() override = default;
        FITKGeoEnum::FITKGeometryComType getGeometryCommandType() override;

        Interface::VirtualShape axis() { return m_AxisShape; }
        void setAxis(Interface::VirtualShape axis) { m_AxisShape = axis; }
        bool isReverse() const { return m_IsReverse; }
        void reverse(bool rev) { m_IsReverse = rev; }
        int axisCount() const { return m_AxisCount; }
        void setAxisCount(int count) { m_AxisCount = count; }
        double axisSpacing() const { return m_AxisSpacingDegree; }
        void setAxisSpacing(double spacing) { m_AxisSpacingDegree = spacing; }
        double axisTotal() const { return m_AxisTotalDegree; }
        void setAxisTotal(double total) { m_AxisTotalDegree = total; }

        int crownCount() const { return m_CrownCount; }
        void setCrownCount(int count) { m_CrownCount = count; }
        double crownSpacing() const { return m_CrownSpacing; }
        void setCrownSpacing(double spacing) { m_CrownSpacing = spacing; }
        double crownTotal() const { return m_CrownTotal; }
        void setCrownTotal(double total) { m_CrownTotal = total; }
    protected:
        /**
         * @brief  旋转轴.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        Interface::VirtualShape m_AxisShape{};
        /**
         * @brief  是否翻转.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-16
         */
        bool m_IsReverse{ false };
        /**
         * @brief  环向上的数量.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        int m_AxisCount{ 2 };
        /**
         * @brief  环向上的实例间隔距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_AxisSpacingDegree{ 45 };
        /**
         * @brief  环向上的总距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_AxisTotalDegree{ 45 };
        /**
         * @brief  径向上的数量.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        int m_CrownCount{ 1 };
        /**
         * @brief  径向上的实例间隔距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_CrownSpacing{ 20 };
        /**
         * @brief  径向上的实例间隔距离.
         * @author YanZhiHui(chanyuantiandao@126.com)
         * @date   2024-10-15
         */
        double m_CrownTotal{ 20 };
    };
}



#endif // FITKABSGEOTRANSFORMATION_H
