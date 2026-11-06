#ifndef CONSOLE_H
#define CONSOLE_H
#include <QObject>
#include <QString>
#include <QStringList>
#include <QUrl>
#include "Machine.h"

class Console : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList registers READ registers NOTIFY machineUpdated)
    Q_PROPERTY(QStringList memory READ memory NOTIFY machineUpdated)
    Q_PROPERTY(QString pc READ pc NOTIFY machineUpdated)
    Q_PROPERTY(QString ir READ ir NOTIFY machineUpdated)
    Q_PROPERTY(QString screenText READ screenText NOTIFY machineUpdated)
    Q_PROPERTY(bool isHalted READ isHalted NOTIFY machineUpdated)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusChanged)
private:
    VoleMachine machine;
    QString m_statusMessage;
    void updateStatusMessage(const QString &message);

public:
    explicit Console(QObject *parent = nullptr);
    QStringList registers();
    QStringList memory();
    QString pc();
    QString ir();
    QString screenText();
    bool isHalted();
    QString statusMessage() const { return m_statusMessage; }

    Q_INVOKABLE void loadProgram(const QUrl &fileUrl, int startAddress);
    Q_INVOKABLE void step();
    Q_INVOKABLE void runAll();
    Q_INVOKABLE void reset();

    Q_INVOKABLE void setRegister(int n, int v);
    Q_INVOKABLE void setMemory(int addr, int v);

signals:
    void machineUpdated();
    void statusChanged();
};

#endif // CONSOLE_H