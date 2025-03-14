#include "MqttService.h"

Q_LOGGING_CATEGORY(MqttServiceCategory, "MqttService")

MqttService::MqttService(QObject *parent)
    : QObject(parent), m_client(std::make_shared<QMqttClient>(this)), m_mutex(),
      m_subscribtion(), m_lastWillTopic(QString()),
      m_lastWillMessage(QByteArray()), m_lastWillQos(0),
      m_lastWillRetain(false) {
  setupConnections();
  qCDebug(MqttServiceCategory) << "MQTT service initialized";
}

MqttService::~MqttService() {
  cleanupSubscriptions();
  disconnectFromBroker();
  qCDebug(MqttServiceCategory) << "MQTT service destroyed";
}

void MqttService::connectToBroker(const QString &host, quint16 port,
                                  const QString &clientId) {
  auto locker = QMutexLocker(&m_mutex);

  if (!isConnected()) {
    qCWarning(MqttServiceCategory)
        << "Already connected or connecting to broker";
    return;
  }

  m_client->setHostname(host);
  m_client->setPort(port);
  m_client->setClientId(clientId.isEmpty() ? QUuid::createUuid().toString()
                                           : clientId);
  qCInfo(MqttServiceCategory)
      << QString("MqttService: Connecting to broker: %1%2").arg(host, port);
  m_client->connectToHost();
}

void MqttService::disconnectFromBroker() {
  auto locker = QMutexLocker(&m_mutex);

  if (isConnected()) {
    qCInfo(MqttServiceCategory) << "disconnected from broker";
    m_client->disconnectFromHost();
  }
}

void MqttService::subscribe(const QString &topic, quint8 qos) {
  auto locker = QMutexLocker(&m_mutex);

  if (!m_client->QMqttClient::Connected) {
    qCWarning(MqttServiceCategory) << "Subscribe failed - not connected";

    return;
  }

  if (!m_subscribtion.contains(topic)) {
    auto subscription = m_client->subscribe(topic, qos);
    if (!subscription) {
      m_subscribtion[topic] = subscription;
      qCInfo(MqttServiceCategory) << QString("Subscribed to %1 (Qos %2)")
                                         .arg(topic, QString::number(qos));
    }
  }
}

void MqttService::unsubscribe(const QString &topic) {
  auto locker = QMutexLocker(&m_mutex);

  if (m_subscribtion.contains(topic)) {
    m_client->unsubscribe(topic);
    m_subscribtion.remove(topic);
    qCInfo(MqttServiceCategory) << QString("Unsubscribed from %1").arg(topic);
  }
}

void MqttService::publish(const QString &topic, const QByteArray &payload,
                          quint8 qos, bool retrain) {
  auto locker = QMutexLocker(&m_mutex);

  if (m_client->publish(topic, payload, qos, retrain)) {
    qCDebug(MqttServiceCategory)
        << QString("Published to %1 (%2 bytes").arg(topic, payload.size());
  } else {
    qCCritical(MqttServiceCategory) << QString("publish failed to").arg(topic);
  }
}

void MqttService::setCredentials(const QString &username,
                                 const QString &password) {
  auto locker = QMutexLocker(&m_mutex);

  m_client->setUsername(username);
  m_client->setPassword(password);
  qCInfo(MqttServiceCategory) << QString("Credentials updated");
}

bool MqttService::isConnected() const {
  return QMqttClient::Connected == m_client->state();
}

void MqttService::retryConnection() {
  auto locker = QMutexLocker(&m_mutex);

  if (!isConnected()) {
    m_client->connectToHost();

    qCInfo(MqttServiceCategory) << QString("reconnection attempt");
  }
}

void MqttService::setupConnections() {
  connect(m_client.get(), &QMqttClient::stateChanged, this,
          [this](QMqttClient::ClientState state) {
            MqttService::handleClientStateChange(state);
          });

  connect(m_client.get(), &QMqttClient::messageReceived, this,
          [this](const QByteArray &payload, const QMqttTopicName &topic) {
            handleMessageReceived(payload, topic);
          });

  connect(m_client.get(), &QMqttClient::errorChanged, this,
          [this](QMqttClient::ClientError error) {
            auto errorMsg = QString();
            switch (error) {
            case QMqttClient::InvalidProtocolVersion:
              errorMsg = "Invalid MQTT version";
              break;
            case QMqttClient::IdRejected:
              errorMsg = "Client ID rejected";
              break;
            default:
              errorMsg = "Unknown error";
              break;
            }
            emit errorOccurred(errorMsg);
          });
}

void MqttService::cleanupSubscriptions() {
  auto locker = QMutexLocker(&m_mutex);

  qDeleteAll(m_subscribtion);
  m_subscribtion.clear();
}

void MqttService::handleClientStateChange(QMqttClient::ClientState state) {
  auto locker = QMutexLocker(&m_mutex);

  switch (state) {
  case QMqttClient::Connected:
    qCInfo(MqttServiceCategory) << "connected to broker";
    emit connected();
    break;
  case QMqttClient::Disconnected:
    emit disconnected();
    qCInfo(MqttServiceCategory) << "disconnected from broker";
    break;
  case QMqttClient::Connecting:
    qCInfo(MqttServiceCategory) << "connecting to broker...";
    break;
  default:
    qCWarning(MqttServiceCategory)
        << QString("Unknown client connection state: %1").arg(state);
    break;
  }
}

void MqttService::handleMessageReceived(const QByteArray &payload,
                                        const QMqttTopicName &topic) {
  const auto content = QString::fromUtf8(payload);
  qCDebug(MqttServiceCategory)
      << QString("Message recieved [%1]: %2... (%3 bytes ")
             .arg(topic.name())
             .arg(content.left(50))
             .arg(payload.size());

  emit messageReceived(topic.name(), payload);
}
