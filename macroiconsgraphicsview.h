#ifndef MACROICONSGRAPHICSVIEW_H
#define MACROICONSGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QGraphicsRectItem>

class MacroItemRect;

class MacroIconsGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    explicit MacroIconsGraphicsView(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *event);
    void parseIconMetadata();
    void setCurrentIcon(const QString& name);

signals:

public slots:
    void iconClicked(MacroItemRect *name);

protected:
    MacroItemRect* currentSelectedIcon;
    QMap<QString, MacroItemRect*> icons;
    QMap<QString, QGraphicsRectItem*> selectionRectangles;

};

#endif // MACROICONSGRAPHICSVIEW_H
