#pragma once

#include <QDateTime>
#include <QJsonObject>

class DeviceState {
public:
  DeviceState() = default;
  DeviceState(int deviceId, bool light, int brightness,
              const QDateTime &timestamp = QDateTime::currentDateTime());
  ~DeviceState() = default;

  // Setters
  void setDeviceId(int deviceId);
  void setLight(bool light);
  void setBrightness(int brightness);
  void setTimestamp(const QDateTime &timestamp);
  void setType(const QString &type);

  // Getters
  int deviceId() const;
  bool light() const;
  int brightness() const;
  QString type() const;
  QDateTime timestamp() const;

  // Serialization
  QJsonObject toJson() const;
  static DeviceState fromJson(const QJsonObject &json);

  // Validation
  bool isValid() const;

  // Operators
  bool operator==(const DeviceState &other) const;

private:
  int m_deviceId;
  bool m_light; // true is on, false is off
  int m_brightness;
  QDateTime m_timestamp;
  QString m_type;
};
