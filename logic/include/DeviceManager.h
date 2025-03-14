#include "utils/Logger.h"
#include <QHash>
#include <QMutex>

#include <memory>

#include "MqttService.h"
#include "dtos/DeviceState.h"
#include "interfaces/IDeviceController.h"

class DeviceManager : public IDeviceController {
  Q_OBJECT
public:
  explicit DeviceManager(std::shared_ptr<MqttService> mqttService,
                         QObject *parent = nullptr);
  ~DeviceManager();

  // IDeviceController Implementation
  void toggleLight(int deviceId, bool light) override;

  void setLightBrightness(int deviceId, double brightness) override;
  bool isLightOn(int deviceId) const override;
  int getLightBrightness(int deviceId) const override;

  // Device discovery/managment
  void registerDevice(int deviceId, const QString &type);
  void unregisterDevice(int deviceId);

signals:
  void deviceAdded(int deviceId);
  void deviceRemoved(int deviceId);
  void deviceStateUpdated(const DeviceState &state);

public slots:
  void handleMqttMessage(const QString &topic, const QByteArray &payload);

private:
  void updateDeviceState(int deviceId, const DeviceState &state);
  void publishStateChange(int deviceId);

  std::shared_ptr<MqttService> m_mqttService;
  QHash<int, DeviceState> m_devices;
  mutable QMutex m_mutex;
  static const QString m_commadTopicBase;
  static const QString m_stateTopicBase;
};
