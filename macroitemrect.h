#ifndef MACROITEMRECT_H
#define MACROITEMRECT_H

#include <QGraphicsRectItem>

class MacroItemRect : public QObject, public QGraphicsRectItem  {
    Q_OBJECT

    QString iconName;

public:
    explicit MacroItemRect(const QString& iconName, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    void unselectIcon();
    void selectIcon();

signals:
    void iconClicked(MacroItemRect*);
public slots:

};

#endif // MACROITEMRECT_H
