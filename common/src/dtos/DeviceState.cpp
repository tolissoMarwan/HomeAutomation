#include "dtos/DeviceState.h"

namespace common::dtos {

DeviceState::DeviceState(int deviceId, bool light, int brightness,
                         const QDateTime &timestamp)
    : m_deviceId(deviceId), m_brightness(qBound(0, brightness, 100)),
      m_timestamp(timestamp) {}

void DeviceState::setBrightness(int brightness) {
  m_brightness = qBound(0, brightness, 100);
}

void DeviceState::setDeviceId(int deviceId) { m_deviceId = deviceId; }

void DeviceState::setLight(bool light) { m_light = light; }

void DeviceState::setTimestamp(const QDateTime &timestamp) {
  if (timestamp.isValid()) {
    m_timestamp = timestamp;
  } else {
    qWarning() << "Invalid timestamp provided for deviceId " << deviceId();
  }
}

int DeviceState::brightness() const { return m_brightness; }

int DeviceState::deviceId() const { return m_deviceId; }

bool DeviceState::light() const { return m_light; }

QDateTime DeviceState::timestamp() const { return m_timestamp; }

QJsonObject DeviceState::toJson() const {
  auto json = QJsonObject();

  json["deviceId"] = deviceId();
  json["light"] = light();
  json["brightness"] = brightness();
  json["timestamp"] = timestamp().toString(Qt::ISODate);

  return json;
}

DeviceState DeviceState::fromJson(const QJsonObject &json) {
  auto data = DeviceState();

  data.setDeviceId(json["deviceId"].toInt());
  data.setLight(json["light"].toBool());
  data.setBrightness(json["brightness"].toInt());
  data.setTimestamp(
      QDateTime::fromString(json["timestamp"].toString(), Qt::ISODate));

  return data;
}

bool DeviceState::isValid() const {
  return (deviceId() >= 0) && (brightness() >= 0 && brightness() <= 100) &&
         timestamp().isValid();
}

bool DeviceState::operator==(const DeviceState &other) const {
  return (deviceId() == other.deviceId()) &&
         (brightness() == other.brightness()) &&
         (timestamp() == other.timestamp());
}

} // namespace common::dtos

QDebug operator<<(QDebug debug, const common::dtos::DeviceState &deviceState) {
  QDebugStateSaver saver(debug);
  debug.nospace() << "timestamp="
                  << deviceState.timestamp().toString(Qt::ISODate)
                  << ", light=" << deviceState.light()
                  << ", brightness=" << deviceState.brightness();

  return debug;
}
