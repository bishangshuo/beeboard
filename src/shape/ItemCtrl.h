#ifndef ITEMCTRL_H
#define ITEMCTRL_H

#include <QObject>
#include <QStack>
#include <QLineF>
using namespace std;

typedef struct _BASEITEM_GEO{
    _BASEITEM_GEO(qreal _px, qreal _py, int _width, int _height, qreal _angle)
        : px(_px), py(_py), width(_width), height(_height), angle(_angle){}
    bool compare(const _BASEITEM_GEO &_geo){
        return (int(_geo.px) == int(px) && int(_geo.py) == int(py )
                && _geo.width == width && _geo.height == height
                && int(_geo.angle) == int(angle));
    }
    qreal px;
    qreal py;
    int width;
    int height;
    qreal angle;
    QLineF line;
}BASEITEM_GEO;

typedef QStack<BASEITEM_GEO *> StGeo;

typedef std::function<void(int)> CBRemove;
typedef std::function<void(qreal, qreal, qreal)> CBRotate;
typedef std::function<void(int)> CBItemChanged;
typedef std::function<void(int)> CBEraserChanged;
class ItemCtrl
{
public:
    ItemCtrl();
    virtual ~ItemCtrl();

    void SetRemoveCallback(const CBRemove &callback) {
        m_pCBRemove = callback;
    }

    void SetRotateCallback(const CBRotate &callback){
        m_pCBRotate = callback;
    }

    void SetItemChangedCallback(const CBItemChanged &callback){
        m_pCBItemChanged = callback;
    }

    void SetItemEraserChanged(const CBEraserChanged &callback){
        m_pCBEraserChanged = callback;
    }


    CBRemove m_pCBRemove;
    CBRotate m_pCBRotate;
    CBItemChanged m_pCBItemChanged;
    CBEraserChanged m_pCBEraserChanged;

    StGeo m_stUndo;
    StGeo m_stRedo;
    QPointF m_ptClicked;

};

#endif // ITEMCTRL_H
