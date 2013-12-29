#ifndef GAMEMACRO_H
#define GAMEMACRO_H

#include <QString>
#include <QListWidgetItem>

class GameMacro : public QListWidgetItem {
    QString contents;
    QString name;
    QString icon;
    QString color;
    int id;
public:
    GameMacro();
    GameMacro(const GameMacro& m) {
        initializeFrom(m);
    }

    GameMacro& operator=(const GameMacro& m) {
        if (this == &m)
            return *this;

        initializeFrom(m);

        return *this;
    }

    void initializeFrom(const GameMacro& m) {
        contents = m.contents;
        name = m.name;
        icon = m.icon;
        color = m.color;
        id = m.id;
    }

    bool parseMacro(const QString& macroString);
    QString toString();

    void setContents(const QString& cont) {
        contents = cont;
    }

    void setName(const QString& nm) {
        name = nm;
    }

    void setIcon(const QString& ic) {
        icon = ic;
    }

    QString getColor() const {
        return color;
    }

    QString getContents() const {
        return contents;
    }

    QString getIcon() const {
        return icon;
    }

    QString getName() const {
        return name;
    }

    int getId() const {
        return id;
    }
};

#endif // GAMEMACRO_H
