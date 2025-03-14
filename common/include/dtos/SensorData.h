#pragma once

#include "interfaces/IDeviceController.h"
#include <QDateTime>
#include <QJsonObject>
#include <QtDebug>

class SensorData {
public:
  SensorData() = default;
  SensorData(double temp, double humidity,
             const QDateTime &timestamp = QDateTime::currentDateTime());

  ~SensorData() = default;

  // Getters
  double getTemp() const;
  double getHumidity() const;
  QDateTime timestamp() const;

  // Setters
  void setTemp(double temp);
  void setHumidity(double humidity);
  void setTimestamp(const QDateTime &timestamp);

  // Serialization
  QJsonObject toJson() const;
  static SensorData fromJson(const QJsonObject &json);

  // Validation
  bool isValid() const;

  // Operators
  bool operator==(const SensorData &other) const;

private:
  double m_temp;
  double m_humidity;
  QDateTime m_timestamp;
};
