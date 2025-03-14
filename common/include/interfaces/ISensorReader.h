#pragma once

#include <QObject>
#include <QString>

class ISensorReader : public QObject {
  Q_OBJECT
public:
  explicit ISensorReader(QObject *parent = nullptr) : QObject(parent) {}
  virtual ~ISensorReader() = default;

  virtual void startReading() = 0;
  virtual void stopReading() = 0;
  virtual bool isReading() = 0;

signals:

public slots:
};

Q_DECLARE_INTERFACE(ISensorReader, "com.homeautomation.ISensorReader")
