/*
Copyright (c) 2020-2025, Qingdao Digital Intelligent Ship & Ocean Technology Co., Ltd.
All rights reserved.

This file is part of FastCAE and is distributed under the terms of the
BSD 3-Clause License. See the LICENSE file in the project root for details.
*/

#include "FITKNumberComplex.h"
#include <cmath>

namespace Core
{
    FITKNumberComplex::FITKNumberComplex(const double& R, const double & I)
        :_real(R),_imaginary(I)
    {
    }

    FITKNumberComplex::FITKNumberComplex(const FITKNumberComplex& number)
    {
        //拷贝对象
        _real = number.getReal();
        _imaginary = number.getImaginary();
    }


    double FITKNumberComplex::getReal() const
    {
        return _real;
    }

    void FITKNumberComplex::setReal(const double& r)
    {
        _real = r;
    }


    double FITKNumberComplex::getImaginary() const
    {
        return _imaginary;
    }

    void FITKNumberComplex::setImaginary(const double& i)
    {
        _imaginary = i;
    }


    double FITKNumberComplex::getNorm() const
    {
        //模大小
        return sqrt(_real*_real + _imaginary * _imaginary);
    }

}



