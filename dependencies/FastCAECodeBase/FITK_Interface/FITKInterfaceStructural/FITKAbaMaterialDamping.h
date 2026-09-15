/*
Copyright (c) 2020-2026, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#ifndef FITKABAMATERIALDAMPING_H
#define FITKABAMATERIALDAMPING_H
#include "FITKAbaMaterialBehavior.h"

namespace Interface {
    class FITKInterfaceStructuralAPI FITKAbaMaterialDamping: public FITKAbaMaterialBehavior
    {
    public:
        FITKAbaMaterialDamping() = default;
        FITKAbaMaterialBehaviorEnum::FITKAbaMaterialBehaviorType getMaterialBehaviorType() override;
        /**
         * @brief  获取属性行为类名
         * @param[in]  void
         * @return 属性行为类名
         * @author libaojun
         * @date   2025-08-08
         */
        virtual QString getBehaviorClassName() override { 
            return "Interface::FITKAbaMaterialDamping"; }
        double getAlpha() const;
        void setAlpha(double newAlpha);
        double getBeta() const;
        void setBeta(double newBeta);
        double getComposite() const;
        void setComposite(double newComposite);
        double getStructural() const;
        void setStructural(double newStructural);

    private:
        double m_alpha{0.0};
        double m_beta{0.0};
        double m_composite{0.0};
        double m_structural{0.0};
    };
}
#endif // FITKABAMATERIALDAMPING_H
