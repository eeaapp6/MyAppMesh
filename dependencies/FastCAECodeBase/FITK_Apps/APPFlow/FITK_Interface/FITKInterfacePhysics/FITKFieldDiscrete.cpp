#include "FITKFieldDiscrete.h"

namespace Interface
{
    FITKAbstractFiled::FITKFieldType FITKFieldDiscrete::getFieldType()
    {
        //离散场
        return  FITKAbstractFiled::FITKFieldType::FFTDiscrete;
    }

    FITKFieldDiscrete::FITKDisFieldLoc FITKFieldDiscrete::getFieldLoc() const
    {
        return _fieldLoc;
    }


    void FITKFieldDiscrete::setDisFieldLoc(FITKDisFieldLoc loc)
    {
        _fieldLoc = loc;
    }

    bool FITKFieldDiscrete::hasDefaultValue() const
    {
        //默认值是否被设置
        return _hasDefaultValue;
    }

    void FITKFieldDiscrete::hasDefaultValue(const bool v)
    {
        _hasDefaultValue = v;
    }

    void FITKFieldDiscrete::setDefaultValue(const double v)
    {
        //默认值被设置
        _hasDefaultValue = true;
        _defaultValue = v;
    }


    double FITKFieldDiscrete::getDefaultValue() const
    {
        return _defaultValue;
    }

    int FITKFieldDiscrete::getFileDataCount()
    {
        return _fieldData.count();
    }

    FieldData FITKFieldDiscrete::getFieldData(const int index)
    {
        //索引判断
        if (index < 0 || index >= _fieldData.size())
            return FieldData();
        return _fieldData.at(index);
    }

    void FITKFieldDiscrete::appendFieldData(int id, double v)
    {
        FieldData f;
        f._id = id;
        f._value = v;
        //追加数据
        _fieldData.append(f);
    }

    void FITKFieldDiscrete::insertFieldData(int index, int id, int v)
    {
        FieldData f;
        f._id = id;
        f._value = v;
        //插入
        _fieldData.insert(index,f);
    }

    void FITKFieldDiscrete::removeFieldData(int index)
    {
        //移除
        _fieldData.removeAt(index);
    }

    void FITKFieldDiscrete::clearFieldData()
    {
        //清空
        _fieldData.clear();
    }

    bool FITKFieldDiscrete::copy(FITKAbstractDataObject* obj)
    {
        FITKFieldDiscrete* fe = dynamic_cast<FITKFieldDiscrete*>(obj);
        if (fe == nullptr) return false;

        _fieldLoc = fe->getFieldLoc();
        _hasDefaultValue = fe->hasDefaultValue();
        _defaultValue = fe->getDefaultValue();

        _fieldData.clear();
        const int n = fe->getFileDataCount();
        for (int i = 0; i < n; ++i)
        {
            FieldData fd = fe->getFieldData(i);
            _fieldData.append(fd);
        }

        return FITKAbstractFiled::copy(obj);

    }

}



