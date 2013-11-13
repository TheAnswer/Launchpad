#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <QString>
#include <QMap>
#include <QTextStream>
#include <QPair>
#include <QVector>

class ConfigToken {
public:
    enum {INVALID = -1, EMPTY, COMMENT, EQUAL, BRACKET_LEFT, BRACKET_RIGHT, NEW_LINE, TAB, CHAR_LITERAL, DOT, SPACE};

    ConfigToken() {
        value = ' ';
        type = INVALID;
    }

    ConfigToken(const ConfigToken& token) {
        value = token.value;
        type = token.type;
    }

    ConfigToken& operator=(const ConfigToken& token) {
        if (this == &token)
            return *this;

        value = token.value;
        type = token.type;

        return *this;
    }

    char value;
    int type;
};

class ConfigValue {
public:
    QString variableName;
    QString variableValue;
    QString fileName;

    ConfigValue() {

    }

    ConfigValue(const QString& varName, const QString& value, const QString& fileName = "") {
        variableName = varName;
        variableValue = value;
        this->fileName = fileName;
    }

    ConfigValue(const ConfigValue& val) {
        initialize(val);
    }

    ConfigValue& operator=(const ConfigValue& val) {
        if (this == &val)
            return *this;

        initialize(val);

        return *this;
    }

    void initialize(const ConfigValue& val) {
        variableName = val.variableName;
        variableValue = val.variableValue;
        fileName = val.fileName;
    }
};

class ConfigParser : public QObject {
    Q_OBJECT

    QMap<char, int> tokens;

    QMap<QString, QVector<ConfigValue> > configValues;

    QString currentClassName;
    QString currentFolder;

    QString currentFile;

    QVector<QString> parsedFiles;

private:
    void registerTokens();
    void setVariable(const QString& variableName, const QString& variableValue);

signals:
    void errorSignal(const QString&);

public:
    ConfigParser();

    int loadFile(const QString& folder, const QString& configFileName);

    int processLine(QTextStream* stream, int currentLineNumber);
    ConfigToken readToken(QTextStream* stream);

    int parseDotExpression(QTextStream* stream, int lineNumber);
    void errorMessage(const QString& errorMsg, int lineNumber);

    QVector<ConfigValue> getValues(const QString& className) {
        return configValues.value(className);
    }

    QMap<QString, QVector<ConfigValue> >* getAllValues() {
        return &configValues;
    }

    bool hasConfigValue(const QString& className, const QString& variable) {
        return getConfigValues(className, variable).size() > 0;
    }

    int getConfigValue(const QString& className, const QString& variable, ConfigValue& returnValue);

    QVector<ConfigValue> getConfigValues(const QString& className, const QString& variable);

    bool getBooleanConfigValue(const QString& className, const QString& variable, bool defaultValue = false);
    int getIntConfigValue(const QString& className, const QString& variable, int defaultValue = 0);
    QString getStringConfigValue(const QString& className, const QString& variable, const QString& defaultValue = "");

};

#endif // CONFIGPARSER_H
