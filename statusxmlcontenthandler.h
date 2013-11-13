#ifndef STATUSXMLCONTENTHANDLER_H
#define STATUSXMLCONTENTHANDLER_H

#include <QXmlDefaultHandler>
#include <QMap>

class MainWindow;

class StatusXmlContentHandler : public QXmlDefaultHandler {
public:
  StatusXmlContentHandler(MainWindow* mainWindow);
  ~StatusXmlContentHandler();

  bool startElement(const QString & namespaceURI, const QString & localName,
                                          const QString & qName, const QXmlAttributes & atts );

  bool endElement ( const QString & namespaceURI, const QString & localName, const QString & qName );

  bool characters ( const QString & ch );

  QMap<QString, QString>* getValues() {
    return &values;
  }

protected:
  QString currentElement;
  QString currentValue;
  MainWindow* mainWindow;

  QMap<QString, QString> values;
};

#endif // STATUSXMLCONTENTHANDLER_H
