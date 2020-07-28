#ifndef TRACKBUTTON_H
#define TRACKBUTTON_H

#include <QObject>
#include <QPushButton>

class TrackButton : public QPushButton
{
    Q_OBJECT
public:
    explicit TrackButton(QWidget *parent = nullptr);
    explicit TrackButton(const QString &text, QWidget *parent = nullptr);
    TrackButton(const QIcon& icon, const QString &text, QWidget *parent = nullptr);
signals:
    void sigMousePressed(const QPoint &pos);
    void sigMouseMove(const QPoint &pos);
    void sigMouseReleased(const QPoint &pos);
protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
private:
    bool m_bTracking;
};

#endif // TRACKBUTTON_H
