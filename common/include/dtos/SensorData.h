#pragma once

#include <QDateTime>
#include <QJsonObject>
#include <QtDebug>

namespace common::dtos {

class SensorData {
public:
  SensorData() = default;
  SensorData(double temp, double humidity,
             const QDateTime &timestamp = QDateTime::currentDateTime());

  ~SensorData() = default;

  // Getters
  double temp() const;
  double humidity() const;
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
} // namespace common::dtos
