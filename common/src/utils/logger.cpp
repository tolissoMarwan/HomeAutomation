#include "utils/Logger.h"
#include <QDir>
#include <iostream>

namespace common::utils {

std::unique_ptr<Logger> Logger::s_instance = nullptr;

Logger::Logger(const QString &filename, QObject *parent)
    : QObject(parent), m_currentLevel(LogLevel::Info) {
  if (!filename.isEmpty()) {
    m_logFile.setFileName(filename);
  }
}

Logger::~Logger() {
  if (m_logFile.isOpen()) {
    m_logFile.close();
  }
}

void Logger::init(const QString &filename) {
  if (!s_instance) {
    auto timestamp = QDateTime::currentDateTime().toString();
    auto logFileName = QString("%1_log.txt").arg(timestamp);
    auto logDirectory = QString("logs");
    auto dir = QDir(logDirectory);
    auto fullLogPath = dir.filePath(logFileName);
    s_instance = std::make_unique<Logger>(fullLogPath, nullptr);

    if (!dir.exists()) {
      if (!dir.mkpath(logDirectory)) { // Create recursively
        qWarning() << "Failed to create log directory!";
        return;
      }
    }
  }

  if (!filename.isEmpty()) {
    s_instance->m_logFile.setFileName(filename);

    if (!s_instance->m_logFile.open(QIODevice::Append | QIODevice::Text)) {
      qWarning() << "Failed to open log file: " << filename;
    }
  }

  qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context,
                            const QString &msg) {
    auto level = LogLevel();
    switch (type) {
    case QtDebugMsg:
      level = LogLevel::Debug;
      break;
    case QtInfoMsg:
      level = LogLevel::Info;
      break;
    case QtWarningMsg:
      level = LogLevel::Warning;
      break;
    case QtCriticalMsg:
      level = LogLevel::Critical;
      break;
    case QtFatalMsg:
      level = LogLevel::Fatal;
      break;
    }
    Logger::log(level, msg, context);
  });
}

void Logger::cleanup() { s_instance.reset(); }

void Logger::log(LogLevel level, const QString &msg,
                 const QMessageLogContext &context) {
  if (level < instance()->m_currentLevel)
    return;

  auto timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
  auto levelStr = QVariant::fromValue(level).toString();
  auto file = context.file ? context.file : "unknown";
  auto logMsg = QString("[%1] [%2] %3:%4 - %5")
                    .arg(timestamp, levelStr, file)
                    .arg(context.line)
                    .arg(msg);

  QMutexLocker locker(&instance()->m_mutex);

  std::cerr << logMsg.toStdString() << "\n";

  if (instance()->m_logFile.isOpen()) {
    auto stream = QTextStream(&instance()->m_logFile);
    stream << logMsg << Qt::endl;
  }
}

void Logger::setLogLevel(LogLevel level) {
  auto locker = QMutexLocker(&instance()->m_mutex);
  instance()->m_currentLevel = level;
}

Logger::LogLevel Logger::loglevel() {
  auto locker = QMutexLocker(&instance()->m_mutex);
  return instance()->m_currentLevel;
}

Logger *Logger::instance() {
  if (!s_instance) {
    qFatal("Logger is not initiatlized");
  }

  return s_instance.get();
}

} // namespace common::utils
