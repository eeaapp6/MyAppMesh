#include "FITKModelWire.h"
#include "FITK_Kernel/FITKCore/FITKDataRepo.h"
#include "FITK_Interface/FITKInterfaceModel/FITKAbstractModel.h"

namespace Interface
{

    bool FITKModelWire::copy(FITKAbstractDataObject* obj)
    {
        //错误判断
        FITKModelWire* loc = dynamic_cast<FITKModelWire*>(obj);
        if (!loc) return false;
        //清空成员，复制对象类型
        _locType = loc->getLocationType();
        _memberLoc.clear();
        //对象复制
        const int n = loc->getMemberCount();
        for (int i=0; i<n; ++i)
        {
            const int model = loc->getModelID(i);
            const int mem = loc->getMemberID(i);
            _memberLoc.append(QPair<int,int>(model, mem));
        }

        return Core::FITKAbstractNDataObject::copy(obj);
    }

    void FITKModelWire::setLocationType(FITKModelEnum::FITKModelSetType type)
    {
        _locType = type;
    }

    FITKModelEnum::FITKModelSetType FITKModelWire::getLocationType()
    {
        return _locType;
    }

    int FITKModelWire::getMemberCount()
    {
        return _memberLoc.size();
    }


    int FITKModelWire::getModelID(const int index)
    {
        //错误判断
        if (index < 0 || index >= _memberLoc.size()) return -1;
        return _memberLoc.at(index).first;
    }

    int FITKModelWire::getMemberID(const int index)
    {
        //错误判断
        if (index < 0 || index >= _memberLoc.size()) return -1;
        return _memberLoc.at(index).second;
    }

    void FITKModelWire::appendMember(const int modelID, const int memberID)
    {
        //在最后追加成员
        _memberLoc.append(QPair<int, int>(modelID, memberID));
    }

    FITKAbstractModel* FITKModelWire::getModel(const int index)
    {
        //强制类型转换获取对象指针，null为错误值
        int id = this->getModelID(index);
        return FITKDATAREPO->getTDataByID<FITKAbstractModel>(id);
    }

    void FITKModelWire::setModelMember(const int index, const int model, const int member)
    {
        //错误判断
        if (index < 0 || index >= _memberLoc.size()) return;
        _memberLoc[index] = QPair<int, int>(model, member);
    }

    QString FITKModelWire::getModelName(const int index)
    {
        //获取模型名称
        FITKAbstractModel* m = getModel(index);
        if (m) return m->getDataObjectName();
        return QString();
    }

    void FITKModelWire::removeMember(const int index)
    {
        //错误判断
        if (index < 0 || index >= _memberLoc.size()) return;
        _memberLoc.removeAt(index);
    }

    void FITKModelWire::removeMember(const int modelID, const int member)
    {
        //    删除指定模型成员
        _memberLoc.removeOne(QPair<int, int>(modelID, member));
    }

    void FITKModelWire::swap()
    {
        if (_memberLoc.size() < 2) return;
        //记录第一个
        QPair<int, int> tloc = _memberLoc[0];
        _memberLoc[0] = _memberLoc[1];
        _memberLoc[1] = tloc;
    }

    QString FITKModelWire::getTextLabel(const int index)
    {
        if (index < 0 || index >= _memberLoc.size()) return QString();
        QString name;
        //获取模型名称
        FITKAbstractModel* model  = this->getModel(index);
        if (!model) return name;
        name = model->getDataObjectName() + " ";
        //获取位置类型
        switch (_locType)
        {
        case FITKModelEnum::FMSNone: name += "Node"; break;
        case FITKModelEnum::FMSElem: name += "Element"; break;
        default:
            break;
        }
        //获取成员编号
        int m = this->getMemberID(index);
        name += QString("[%1]").arg(m);
        return name;
    }

    void FITKModelWire::getPointCoor(const int index, double* coor)
    {
        //只有点类型才能获取坐标
        if (_locType != FITKModelEnum::FMSNode && _locType != FITKModelEnum::FMSPoint) return;
        //获取模型
        if (index < 0 || index >= _memberLoc.size() || coor == nullptr) return ;
        FITKAbstractModel* model = this->getModel(index);
        if (!model) return;
        int m = this->getMemberID(index);
       //模型获取节点坐标
        model->getPointCoor(m,coor);
    }

}




