#pragma once

#include <QHash>
#include <QMqttClient>
#include <QMqttSubscription>
#include <QMutex>
#include <QObject>
#include <memory>

#include "dtos/SensorData.h"

namespace data::Mqtt {
class MqttService : public QObject {

  Q_OBJECT
public:
  explicit MqttService(QObject *parent = nullptr);
  ~MqttService();

  // Connection Managment
  void connectToBroker(const QString &host, quint16 port,
                       const QString &clientId = QString());
  void disconnectFromBroker();

  // Subscribtion Managment
  void subscribe(const QString &topic, quint8 qos = 0);
  void unsubscribe(const QString &topic);

  // Message Publishing
  void publish(const QString &topic, const QByteArray &payload, quint8 qos = 0,
               bool retrain = false);

  // Configuration
  void setCredentials(const QString &username, const QString &password);
  bool isConnected() const;

public slots:
  void retryConnection();

private slots:
  void handleClientStateChange(QMqttClient::ClientState state);
  void handleMessageReceived(const QByteArray &message,
                             const QMqttTopicName &topic);

private:
  std::unique_ptr<QMqttClient> m_client;
  QHash<QString, QMqttSubscription *> m_subscribtion;
  mutable QMutex m_mutex;

  QString m_lastWillTopic;
  QByteArray m_lastWillMessage;
  quint8 m_lastWillQos;
  bool m_lastWillRetain;

  void setupConnections();
  void cleanupSubscriptions();

signals:
  void connected();
  void disconnected();
  void messageReceived(const QString &topic, const QByteArray &payload);
  void errorOccurred(const QString &error);
};

} // namespace data::Mqtt
