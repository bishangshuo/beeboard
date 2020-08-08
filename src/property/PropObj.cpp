#include "PropObj.h"
#include <QColor>

PropObj *PropObj::g_pProp = nullptr;
PropObj::PropObj(QObject *parent) : QObject(parent)
{

}

PropObj *PropObj::GetInstance(){
    if(nullptr == PropObj::g_pProp)
        PropObj::g_pProp = new PropObj();
    return PropObj::g_pProp;
}

int PropObj::PenWidth() const{
    return 4;
}

QColor PropObj::PenColor() const{
    return  Qt::red;
}

Qt::PenStyle PropObj::PenStyle() const{
    return  Qt::SolidLine;
}

Qt::PenCapStyle PropObj::PenCapStyle() const{
    return  Qt::RoundCap;
}

Qt::PenJoinStyle PropObj::PenJoinStyle() const{
    return  Qt::RoundJoin;
}

QColor PropObj::BrushColor() const{
    return Qt::blue;
}
