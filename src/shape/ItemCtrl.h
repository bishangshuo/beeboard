#ifndef ITEMCTRL_H
#define ITEMCTRL_H

#include <QObject>
using namespace std;

typedef std::function<void(int)> CBRemove;
typedef std::function<void(qreal, qreal, qreal)> CBRotate;
class ItemCtrl
{
public:
    ItemCtrl();

    void SetRemoveCallback(const CBRemove &callback) {
        m_pCBRemove = callback;
    }

    void SetRotateCallback(const CBRotate &callback){
        m_pCBRotate = callback;
    }

    CBRemove m_pCBRemove;
    CBRotate m_pCBRotate;
};

#endif // ITEMCTRL_H
