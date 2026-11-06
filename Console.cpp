#include "Console.h"
#include <QFileInfo>
#include <QDebug>

Console::Console(QObject *parent)
    : QObject(parent), m_statusMessage("Ready") {}

void Console::updateStatusMessage(const QString &msg)
{
    m_statusMessage = msg;
    emit statusChanged();
}

QStringList Console::registers()
{
    QStringList list;
    auto &regs = machine.getRegisters();
    for (int i = 0; i < 16; ++i)
    {
        list << QString("R%1: 0x%2")
                    .arg(QString::number(i, 16).toUpper())
                    .arg(QString::number(regs.get(i), 16).rightJustified(2, '0').toUpper());
    }
    return list;
}

QStringList Console::memory()
{
    QStringList list;
    auto &mem = machine.getMemory();
    for (int i = 0; i < 256; ++i)
    {
        list << QString::number(mem.read(i), 16).rightJustified(2, '0').toUpper();
    }
    return list;
}

QString Console::pc()
{
    return QString("0x%1").arg(QString::number(machine.getCPU().getPC(), 16).rightJustified(2, '0').toUpper());
}

QString Console::ir()
{
    return QString("0x%1").arg(QString::number(machine.getCPU().getIR(), 16).rightJustified(4, '0').toUpper());
}

QString Console::screenText()
{
    return QString::fromStdString(machine.getScreen().getText());
}

bool Console::isHalted()
{
    return machine.getCPU().isHalted();
}

void Console::loadProgram(const QUrl &fileUrl, int startAddress)
{
    try
    {
        QString localPath = fileUrl.toLocalFile();
        machine.loadProgram(localPath.toStdString(), startAddress);
        updateStatusMessage("Loaded program successfully from: " + QFileInfo(localPath).fileName());
        emit machineUpdated();
    }
    catch (const VoleException &e)
    {
        updateStatusMessage(QString("Loading Error: %1").arg(e.what()));
    }
    catch (const std::exception &e)
    {
        updateStatusMessage(QString("System Error: %1").arg(e.what()));
    }
}

void Console::step()
{
    try
    {
        if (!machine.step())
        {
            updateStatusMessage("Execution halted.");
        }
        else
        {
            updateStatusMessage("Executed 1 step. PC: " + pc());
        }
        emit machineUpdated();
    }
    catch (const VoleException &e)
    {
        updateStatusMessage(QString("Execution Error: %1").arg(e.what()));
    }
}

void Console::runAll()
{
    try
    {
        machine.runAll();
        updateStatusMessage("Program execution completed.");
        emit machineUpdated();
    }
    catch (const VoleException &e)
    {
        updateStatusMessage(QString("Runtime Error: %1").arg(e.what()));
    }
}

void Console::reset()
{
    machine.reset();
    updateStatusMessage("Simulator reset to successfully.");
    emit machineUpdated();
}

void Console::setRegister(int n, const QString &hexValue)
{
    bool ok;
    int v = hexValue.toInt(&ok, 16);
    if (!ok)
    {
        updateStatusMessage(QString("Invalid hex value: %1").arg(hexValue));
        return;
    }
    machine.setRegister(n, v);
    updateStatusMessage(QString("Register R%1 updated to 0x%2").arg(QString::number(n, 16).toUpper()).arg(v, 2, 16, QChar('0')).toUpper());
    emit machineUpdated();
}

void Console::setMemory(int addr, const QString &hexValue)
{
    bool ok;
    int v = hexValue.toInt(&ok, 16);
    if (!ok)
    {
        updateStatusMessage(QString("Invalid hex value: %1").arg(hexValue));
        return;
    }
    machine.setMemory(addr, v);
    updateStatusMessage(QString("Memory [0x%1] updated to 0x%2").arg(QString::number(addr, 16).toUpper()).arg(v, 2, 16, QChar('0')).toUpper());
    emit machineUpdated();
}