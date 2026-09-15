/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKOCCAlgTools.h"

// OCC
#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>
#include <gp_Quaternion.hxx>
#include <NCollection_Mat4.hxx>

#include <QDebug>

// Data
#include "FITK_Kernel/FITKCore/FITKVec3DAlg.h"

namespace OCC
{
#ifndef M_PI
#define M_PI 3.1415926
#endif
    // 定义精度常量
    const double EPS = 1e-8;

    // 三维向量结构
    struct Vector3 {
        double x, y, z;

        Vector3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

        // 向量加法
        Vector3 operator+(const Vector3 &other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }

        // 向量减法
        Vector3 operator-(const Vector3 &other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }

        // 向量数乘
        Vector3 operator*(double s) const { return Vector3(x * s, y * s, z * s); }

        // 点积
        double dot(const Vector3 &other) const {
            return x * other.x + y * other.y + z * other.z;
        }

        // 叉积
        Vector3 cross(const Vector3 &other) const {
            return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                x * other.y - y * other.x);
        }

        // 向量归一化
        Vector3 normalize() const {
            double len = std::sqrt(x * x + y * y + z * z);
            if (len > EPS) {
                return Vector3(x / len, y / len, z / len);
            }
            return *this;
        }

        // 计算向量范数平方
        double normSquared() const { return x * x + y * y + z * z; }
    };

    // 3x3矩阵结构
    struct Matrix3x3 {
        double m[3][3];

        Matrix3x3() {
            // 初始化为单位矩阵
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    m[i][j] = (i == j) ? 1.0 : 0.0;
                }
            }
        }

        // 矩阵与向量乘法
        Vector3 multiply(const Vector3 &v) const {
            return Vector3(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
                m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
                m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z);
        }

        // 矩阵减法
        Matrix3x3 operator-(const Matrix3x3 &other) const {
            Matrix3x3 result;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    result.m[i][j] = m[i][j] - other.m[i][j];
                }
            }
            return result;
        }

        // 计算矩阵Frobenius范数平方
        double normSquared() const {
            double sum = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    sum += m[i][j] * m[i][j];
                }
            }
            return sum;
        }
    };

    // 4x4矩阵结构
    struct Matrix4x4 {
        double m[4][4];

        Matrix4x4() {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    m[i][j] = (i == j) ? 1.0 : 0.0;
                }
            }
        }

        Matrix3x3 getRotationMatrix() const {
            Matrix3x3 result;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    result.m[i][j] = m[i][j];
                }
            }
            return result;
        }

        Vector3 getTranslationVector() const {
            return Vector3(m[0][3], m[1][3], m[2][3]);
        }

        // 矩阵乘法：成员函数形式
        Matrix4x4 multiply(const Matrix4x4& other) const {
            Matrix4x4 result;
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    result.m[i][j] = 0.0;
                    // 计算第i行第j列的结果（当前行 × 其他矩阵的列）
                    for (int k = 0; k < 4; k++) {
                        result.m[i][j] += m[i][k] * other.m[k][j];
                    }
                }
            }
            return result;
        }

        // 矩阵乘法：运算符重载形式，使代码更直观（如a * b）
        Matrix4x4 operator*(const Matrix4x4& other) const {
            return multiply(other);
        }

    };

    // 使用叉积法直接计算旋转轴方向
    Vector3 computeRotationAxisDirection(const Matrix3x3 &R) {
        // 计算R-I
        Matrix3x3 R_minus_I = R - Matrix3x3();

        // 获取列向量
        Vector3 col0(R_minus_I.m[0][0], R_minus_I.m[1][0], R_minus_I.m[2][0]);
        Vector3 col1(R_minus_I.m[0][1], R_minus_I.m[1][1], R_minus_I.m[2][1]);
        Vector3 col2(R_minus_I.m[0][2], R_minus_I.m[1][2], R_minus_I.m[2][2]);

        // 计算列向量的范数平方
        double n0 = col0.normSquared();
        double n1 = col1.normSquared();
        double n2 = col2.normSquared();

        Vector3 axis_dir;
        double max_norm = 0;
        bool found = false;

        // 尝试所有列组合的叉积
        if (n0 > EPS && n1 > EPS) {
            axis_dir = col0.cross(col1);
            double norm_sq = axis_dir.normSquared();
            if (norm_sq > max_norm) {
                max_norm = norm_sq;
                found = true;
            }
        }

        if (n0 > EPS && n2 > EPS) {
            Vector3 candidate = col0.cross(col2);
            double norm_sq = candidate.normSquared();
            if (norm_sq > max_norm) {
                axis_dir = candidate;
                max_norm = norm_sq;
                found = true;
            }
        }

        if (n1 > EPS && n2 > EPS) {
            Vector3 candidate = col1.cross(col2);
            double norm_sq = candidate.normSquared();
            if (norm_sq > max_norm) {
                axis_dir = candidate;
                max_norm = norm_sq;
                found = true;
            }
        }

        // 如果没有找到有效的叉积，使用默认方向
        if (!found || max_norm < EPS) {
            // 可能无旋转或旋转角很小，返回默认方向
            return Vector3(1, 0, 0);
        }

        return axis_dir.normalize();
    }

    // 从3x3变换矩阵中提取旋转轴方向向量
    Vector3 extractRotationAxisDirection(const Matrix3x3 &rotate) {

        // 检查是否无旋转
        if ((rotate - Matrix3x3()).normSquared() < EPS) {
            return Vector3(1, 0, 0); // 默认方向
        }

        return computeRotationAxisDirection(rotate);
    }

    // 从3x3变换矩阵中提取不过原点的旋转轴（返回轴上一点和方向向量）
    bool extractRotationAxis(const Matrix3x3 &rotate, const Vector3& transform, Vector3 &axis_point, Vector3 &axis_dir) {

        // 检查是否无旋转
        Matrix3x3 R_minus_I = rotate - Matrix3x3();
        if (R_minus_I.normSquared() < EPS) {
            axis_dir = Vector3(1, 0, 0);
            axis_point = Vector3(0, 0, 0);
            // 检查平移是否为零
            if (transform.normSquared() > EPS) {
                // 纯平移变换，没有固定旋转轴
                return false;
            }
            return true;
        }

        // 计算旋转轴方向向量
        axis_dir = computeRotationAxisDirection(rotate);

        // 构造与旋转轴方向正交的基
        Vector3 u = axis_dir.normalize();
        Vector3 u1;

        // 选择一个与u不平行的基础向量
        if (std::abs(u.x) < 0.9) {
            u1 = Vector3(1, 0, 0);
        }
        else {
            u1 = Vector3(0, 1, 0);
        }

        // Gram-Schmidt正交化
        u1 = u1 - u * u.dot(u1);
        u1 = u1.normalize();
        Vector3 u2 = u.cross(u1).normalize();

        // 计算(R-I)在正交基上的投影
        Vector3 A_u1 = R_minus_I.multiply(u1);
        Vector3 A_u2 = R_minus_I.multiply(u2);

        // 构造2x2方程组系数
        double a11 = u1.dot(A_u1);
        double a12 = u1.dot(A_u2);
        double a21 = u2.dot(A_u1);
        double a22 = u2.dot(A_u2);

        // 构造右端项: -t 在正交基上的投影
        double b1 = -u1.dot(transform);
        double b2 = -u2.dot(transform);

        // 解2x2方程组: [a11 a12; a21 a22] * [x; y] = [b1; b2]
        double det = a11 * a22 - a12 * a21;
        if (std::abs(det) < EPS) {
            // 方程组奇异，使用原点作为轴上一点
            axis_point = Vector3(0, 0, 0);
        }
        else {
            double x = (a22 * b1 - a12 * b2) / det;
            double y = (a11 * b2 - a21 * b1) / det;
            axis_point = u1 * x + u2 * y;
        }

        return true;
    }

    Interface::FITKAbstractTransformTool* FITKOCCAlgToolCreator::createTransformTool()
    {
        // Create the transform tool.
        return new FITKOCCTransformTool;
    }

    // Transformation.
    //@{
    FITKOCCTransformTool::FITKOCCTransformTool()
    {
        // Create the transform.
        m_Transformation = new gp_Trsf;
        m_Transformation->SetForm(gp_TrsfForm::gp_Identity);
    }

    FITKOCCTransformTool::~FITKOCCTransformTool()
    {
        if (m_Transformation)
        {
            delete m_Transformation;
            m_Transformation = nullptr;
        }
    }

    gp_Trsf* FITKOCCTransformTool::getInternalTransform()
    {
        return m_Transformation;
    }

    double FITKOCCTransformTool::degToRad(double degree)
    {
        return degree * FITK_PI / 180.;
    }

    double FITKOCCTransformTool::radToDeg(double rad)
    {
        return rad * 180. / FITK_PI;
    }

    void FITKOCCTransformTool::setTransform(double* iTrans, double* iAxisPt1, double* iAxisPt2, double iAngleDeg)
    {
        transform(iTrans[0], iTrans[1], iTrans[2]);
        rotate(iAxisPt1, iAxisPt2, iAngleDeg);
    }

    void FITKOCCTransformTool::transform(double iX, double iY, double iZ)
    {
        // Perform a translate.
        gp_Trsf newTrans;

        try
        {
            newTrans.SetTranslation(gp_Vec(iX, iY, iZ));
        }
        catch (...)
        {
            return;
        }

        *m_Transformation *= newTrans;
    }

    void FITKOCCTransformTool::rotate(double* iPt1, double* iPt2, double iAngleDeg)
    {
        // Perform a rotate.
        gp_Trsf newTrans;

        try
        {
            gp_Pnt pt1(iPt1[0], iPt1[1], iPt1[2]);
            gp_Pnt pt2(iPt2[0], iPt2[1], iPt2[2]);
            gp_Vec vec(pt1, pt2);
            newTrans.SetRotation(gp_Quaternion(vec, degToRad(iAngleDeg)));
        }
        catch (...)
        {
            return;
        }

        *m_Transformation *= newTrans;
    }

    bool FITKOCCTransformTool::isRotation(double* oPt1, double* oPt2, double & oAngleDeg)
    {
        gp_Quaternion qu = m_Transformation->GetRotation();

        gp_Vec theAxis;
        Standard_Real theAngle;
        qu.GetVectorAndAngle(theAxis, theAngle);
        auto flag = !qFuzzyIsNull(theAngle);

        if (flag)
        {

            NCollection_Mat4<double> mat{};
            m_Transformation->GetMat4(mat);

            Matrix4x4 oriTransMat{}, invTransMat{}, acisMat{};
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    oriTransMat.m[i][j] = mat.GetValue(i, j);
                }
            }
            for (int i = 0; i < 3; ++i) {
                invTransMat.m[i][3] = -mat.GetValue(i, 3);
            }

            acisMat = oriTransMat * invTransMat;

            Vector3 p{}, d{};
            if (!extractRotationAxis(acisMat.getRotationMatrix(), acisMat.getTranslationVector(), p, d)) { return false; }

            oPt1[0] = p.x;
            oPt1[1] = p.y;
            oPt1[2] = p.z;

            oPt2[0] = p.x + theAxis.X();
            oPt2[1] = p.y + theAxis.Y();
            oPt2[2] = p.z + theAxis.Z();

            // 计算旋转角度
            double trace = mat.GetValue(0, 0) + mat.GetValue(1, 1) + mat.GetValue(2, 2);
            oAngleDeg = radToDeg(std::acos((trace - 1) / 2));

            if (theAngle < 0)
            {
                oAngleDeg = 360. - oAngleDeg;
            }
        }
        else
        {
            oPt1[0] = 0.;
            oPt1[1] = 0.;
            oPt1[2] = 0.;

            oPt2[0] = 0.;
            oPt2[1] = 0.;
            oPt2[2] = 1.;

            oAngleDeg = 0.;
        }

        return flag;
    }

    void FITKOCCTransformTool::transformPoint(double* iPt, double* oPt)
    {
        gp_Pnt pnt(iPt[0], iPt[1], iPt[2]);
        pnt.Transform(*m_Transformation);
        oPt[0] = pnt.X();
        oPt[1] = pnt.Y();
        oPt[2] = pnt.Z();
    }

    void FITKOCCTransformTool::transformDirection(double* iDir, double* oDir)
    {
        gp_Vec dir(iDir[0], iDir[1], iDir[2]);
        dir.Normalize();
        dir.Transform(*m_Transformation);
        oDir[0] = dir.X();
        oDir[1] = dir.Y();
        oDir[2] = dir.Z();
    }

    void FITKOCCTransformTool::data(double oMat[3][3], double* oVec)
    {
        NCollection_Mat4<Standard_Real> mat4;
        m_Transformation->GetMat4(mat4);
        for (int row = 0; row < 3; row++)
        {
            for (int col = 0; col < 3; col++)
            {
                oMat[row][col] = mat4.GetValue(row, col);
            }
        }

        oVec[0] = mat4.GetValue(0, 3);
        oVec[1] = mat4.GetValue(1, 3);
        oVec[2] = mat4.GetValue(2, 3);
    }

    void FITKOCCTransformTool::setData(double iMat[3][3], double* iVec)
    {
        m_Transformation->SetValues(
            iMat[0][0], iMat[0][1], iMat[0][2], iVec[0],
            iMat[1][0], iMat[1][1], iMat[1][2], iVec[1],
            iMat[2][0], iMat[2][1], iMat[2][2], iVec[2]);
    }
    //@}
}
