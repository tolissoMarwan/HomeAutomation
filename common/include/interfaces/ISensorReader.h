#pragma once

#include <QObject>
#include <QString>

namespace common::interfaces {
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

} // namespace common::interfaces

Q_DECLARE_INTERFACE(common::interfaces::ISensorReader,
                    "com.homeautomation.ISensorReader")
