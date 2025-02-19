#pragma once

#include <QObject>
#include <QString>

namespace common::interfaces {
class IDeviceController : public QObject {
  Q_OBJECT
public:
  explicit IDeviceController(QObject *parent = nullptr) : QObject(parent) {}
  virtual ~IDeviceController() = default;

  virtual bool isLightOn(int deviceId) const = 0;
  virtual int getLightBrightness(int deviceId) const = 0;
  virtual int getTemp(int deviceId) const = 0;
  virtual int getHumidity(int deviceId) const = 0;

signals:
};

} // namespace common::interfaces

Q_DECLARE_INTERFACE(common::interfaces::IDeviceController,
                    "com.homeautomation.IDeviceController")
