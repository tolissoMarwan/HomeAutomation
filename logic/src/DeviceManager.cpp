#include "DeviceManager.h"

#include <QJsonParseError>

const QString DeviceManager::m_commadTopicBase = "home/devices/commands/";
const QString DeviceManager::m_stateTopicBase = "home/devices/states/";

DeviceManager::DeviceManager(std::shared_ptr<MqttService> mqttService,
                             QObject *parent)
    : m_mqttService(mqttService), m_devices(), m_mutex() {
  QObject::connect(m_mqttService.get(), &MqttService::messageReceived, this,
                   [this](const QString &topic, const QByteArray &payload) {
                     DeviceManager::handleMqttMessage(topic, payload);
                   });
}

DeviceManager::~DeviceManager() {
  auto locker = QMutexLocker(&m_mutex);
  m_devices.clear();
}

void DeviceManager::toggleLight(int deviceId, bool light) {
  auto locker = QMutexLocker(&m_mutex);

  if (!m_devices.contains(deviceId)) {
    // Logger::log(Logger::Error, "DeviceManager",QString("Fan speed set failed
    // - device %1 not found").arg(deviceId));
    return;
  }

  auto state = m_devices[deviceId];
  state.setLight(light);
  updateDeviceState(deviceId, state);
}

void DeviceManager::setLightBrightness(int deviceId, double brightness) {
  auto locker = QMutexLocker(&m_mutex);

  if (!m_devices.contains(deviceId)) {
    // Logger::log(Logger::Error, "DeviceManager",QString("Fan speed set failed
    // - device %1 not found").arg(deviceId));
    return;
  }

  auto state = m_devices[deviceId];
  state.setBrightness(brightness);
  updateDeviceState(deviceId, state);
}

bool DeviceManager::isLightOn(int deviceId) const {
  auto locker = QMutexLocker(&m_mutex);
  return m_devices.value(deviceId, DeviceState()).light();
}

int DeviceManager::getLightBrightness(int deviceId) const {

  auto locker = QMutexLocker(&m_mutex);
  return m_devices.value(deviceId, DeviceState()).brightness();
}

void DeviceManager::registerDevice(int deviceId, const QString &type) {
  auto locker = QMutexLocker(&m_mutex);

  if (!m_devices.contains(deviceId)) {
    auto state = DeviceState();
    state.setDeviceId(deviceId);
    state.setType(type);

    m_devices.insert(deviceId, state);

    auto topic = m_stateTopicBase + type + "/" + QString::number(deviceId);
    m_mqttService->subscribe(topic, 1);

    // Logger::log(Logger::Info, "DeviceManager",
    //             QString("Registered device %1
    //             (%2)").arg(deviceId).arg(type));

    emit deviceAdded(deviceId);
  }
}

void DeviceManager::unregisterDevice(int deviceId) {
  auto locker = QMutexLocker(&m_mutex);

  if (m_devices.remove(deviceId)) {
    // Log
    emit deviceRemoved(deviceId);
  }
}

void DeviceManager::handleMqttMessage(const QString &topic,
                                      const QByteArray &payload) {
  auto locker = QMutexLocker(&m_mutex);

  if (topic.startsWith(m_stateTopicBase)) {
    auto parts = topic.split('/');
    if (parts.size() < 4)
      return;

    auto deviceId = parts.last().toInt();
    auto type = parts[parts.size() - 2];

    if (m_devices.contains(deviceId)) {
      auto error = QJsonParseError();
      auto doc = QJsonDocument::fromJson(payload, &error);

      if (error.error == QJsonParseError::NoError && doc.isObject()) {
        auto state = DeviceState();
        if (state.fromJson(doc.object()).isValid()) {
          updateDeviceState(deviceId, state);
        }
      }
    }
  }
}

void DeviceManager::updateDeviceState(int deviceId, const DeviceState &state) {
  if (!(m_devices[deviceId] == state)) {
    m_devices[deviceId] = state;
    publishStateChange(deviceId);
    emit deviceStateUpdated(state);
  }
}

void DeviceManager::publishStateChange(int deviceId) {
  const auto &state = m_devices[deviceId];
  const auto topic =
      m_commadTopicBase + state.type() + '/' + QString::number(deviceId);

  auto json = state.toJson();

  m_mqttService->publish(topic, QJsonDocument(json).toJson());

  // Log
}
