#pragma once

#include <QObject>
#include <QString>

enum class DeviceType { LIGHT, HUMIDITY, BRIGHTNESS };

class IDeviceController : public QObject {
  Q_OBJECT
public:
  explicit IDeviceController(QObject *parent = nullptr) : QObject(parent) {}
  virtual ~IDeviceController() = default;

  virtual void toggleLight(int deviceId, bool light) = 0;
  virtual void setLightBrightness(int deviceId, double brightness) = 0;
  // virtual void setHumidity(int deviceId, double Humidity) = 0;
  // virtual void setTemp(int deviceId, double Temp) = 0;

  virtual bool isLightOn(int deviceId) const = 0;
  virtual int getLightBrightness(int deviceId) const = 0;
  // virtual int getTemp(int deviceId) const = 0;
  // virtual int getHumidity(int deviceId) const = 0;

signals:
};

Q_DECLARE_INTERFACE(IDeviceController, "com.homeautomation.IDeviceController")
