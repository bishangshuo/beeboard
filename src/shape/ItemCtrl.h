#ifndef ITEMCTRL_H
#define ITEMCTRL_H

#include <QObject>
using namespace std;

typedef std::function<void(int)> CBRemove;
class ItemCtrl
{
public:
    ItemCtrl();

    void SetRemoveCallback(const CBRemove &callback) {
        m_pCBRemove = callback;
    }

    CBRemove m_pCBRemove;
};

#endif // ITEMCTRL_H
