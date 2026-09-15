/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

/**
 * 
 * @file   FITKAbaConnectorAssignment.h
 * @brief  连接器截面赋予类
 * @author LiJin (1003680421@qq.com)
 * @date   2024-06-20
 * 
 */
#ifndef FITKABACONNECTORASSIGNMENT_H
#define FITKABACONNECTORASSIGNMENT_H
#include "FITK_Interface/FITKInterfacePhysics/FITKSectionAssign.h"
#include "FITKInterfaceStructuralAPI.h"


namespace Interface {

class FITKInterfaceStructuralAPI FITKAbaConnectorAssignment: public FITKSectionAssign
{
    Q_OBJECT
    FITKCLASS(Interface, FITKAbaConnectorAssignment)
public:
    FITKAbaConnectorAssignment() = default;
    ~FITKAbaConnectorAssignment() = default;
    virtual QString getSectionType() override;
    bool copy(FITKAbstractDataObject* obj) override;
    int getCsysId1() const;
    void setCsysId1(int newCsysId1);
    int getCsysId2() const;
    void setCsysId2(int newCsysId2);
    /**
     * @brief    获取激活状态
     * @return   bool                       是否激活
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-03-25
     */
    bool getActiveState();
    /**
     * @brief    设置激活状态
     * @param[i] isActive                   是否激活
     * @author   BaGuijun (baguijun@163.com)
     * @date     2026-03-25
     */
    void setActiveState(int isActive);

protected:
    virtual FITKAbstractDataObject* getSectionObj() override;

private:
    int m_csysId1{0};
    int m_csysId2{0};
    /**
     * @brief  激活状态标签
     * @author BaGuijun (baguijun@163.com)
     * @date   2026-03-25
     */
    bool m_isActive = true;
};
}
#endif // FITKABACONNECTORASSIGNMENT_H
