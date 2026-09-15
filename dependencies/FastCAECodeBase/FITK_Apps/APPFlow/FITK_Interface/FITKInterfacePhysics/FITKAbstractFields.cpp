#include "FITKAbstractFields.h"

namespace Interface
{
    FITKAbstractFiled::~FITKAbstractFiled()
    {
        //todo
    }

    FITKAbstractFiled::FITKFieldType FITKAbstractFiled::getFieldType()
    {
        //错误值
        return FITKAbstractFiled::FITKFieldType::FFTNone;
    }


    void FITKAbstractFiled::setDescribe(const QString & des)
    {
        _describe = des;
    }

    QString FITKAbstractFiled::getDescribe() const
    {
        //描述
        return _describe;
    }

    bool FITKAbstractFiled::copy(FITKAbstractDataObject* obj)
    {
        FITKAbstractFiled* f = dynamic_cast<FITKAbstractFiled*>(obj);
        if (f == nullptr) return false;

        _describe = f->getDescribe();
        return Core::FITKAbstractNDataObject::copy(obj);
    }

    QList<FITKAbstractFiled*> FITKFieldsManager::getFields(FITKAbstractFiled::FITKFieldType t)
    {
        QList<FITKAbstractFiled*> fs;
        const int n = this->getDataCount();
        //遍历全部成员
        for (int i =0 ; i<n; ++i)
        {
            FITKAbstractFiled* f = this->getDataByIndex(i);
            if(f == nullptr) continue;
            //类型判断
            if (t == f->getFieldType())
                fs.append(f);
        }
        //返回相同类型的列表
        return fs;
    }

}
