#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

#include "LSPClient.hpp"
#include "utils/JsonUtil.h"

#include <QDebug>

using RequestID = std::string;

LSPClient::LSPClient(QString path, QStringList args)
{
    clientProcess = new QProcess();
    clientProcess->setProgram(path);
    clientProcess->setArguments(args);

    connect(clientProcess, SIGNAL(errorOccurred(QProcess::ProcessError)), this,
            SLOT(onClientError(QProcess::ProcessError)));
    connect(clientProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(onClientReadyReadStdout()));
    connect(clientProcess, SIGNAL(readyReadStandardError()), this, SLOT(onClientReadyReadStderr()));
    connect(clientProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(onClientFinished(int, QProcess::ExitStatus)));

    clientProcess->start();
}

// slots

void LSPClient::onClientReadyReadStdout()
{
    // Fixme(coder3101): If all buffer does not comes in one go, then we are in trouble
    // We can fix it later by specifying that if incomplete buffer recieved in one time
    // then in the next time. We should append and emit the signal only onces.

    QByteArray buffer = clientProcess->readAllStandardOutput();

//    qDebug() << "buffer: " << buffer;

    int messageStart = buffer.indexOf("\r\n\r\n") + 4;
    int lenStart = buffer.indexOf("Content-Length: ") + 16;
    int lenEnd = buffer.indexOf("\r\n");
    bool ok = false;
    int contentLength = buffer.mid(lenStart, lenEnd - lenStart).toInt(&ok);

    if (!ok)
        return;

    QByteArray payload = buffer.mid(messageStart);

//    qDebug() << "payload: " << payload;

    if (payload.size() != contentLength)
    {
        // Warning: Incomplete message has arrived,
        // At this point we should keep it in buffer but we are discarding this message;
        return;
    }
    QJsonParseError error{};

    auto msg = QJsonDocument::fromJson(payload, &error);

//    qDebug() << "msg: " << QString(msg.toJson(QJsonDocument::Indented));

    if (error.error != QJsonParseError::NoError || !msg.isObject())
    {
        // Some JSON Parse Error
        return;
    }
    auto obj = msg.object();

    if (obj.contains("id"))
    {
        if (obj.contains("method"))
        {
            emit onRequest(obj["method"].toString(), obj["param"].toObject(), obj["id"].toObject());
        }
        else if (obj.contains("result"))
        {
//            qDebug() << "obj[\"id\"]: " << obj["id"].toString();
            if (obj["id"].toString() == "textDocument/documentColor") {
                emit onDocumentColorResponse(obj["id"].toString(), obj["result"].toVariant().toJsonArray());
            } else {
                emit onResponse(obj["id"].toObject(), obj["result"].toObject());
            }
        }
        else if (obj.contains("error"))
        {
            emit onError(obj["id"].toObject(), obj["error"].toObject());
        }
    }
    else if (obj.contains("method"))
    {
        // notification
        if (obj.contains("params"))
        {
            emit onNotify(obj["method"].toString(), obj["params"].toObject());
        }
    }
}

void LSPClient::onClientReadyReadStderr()
{
    QString content = clientProcess->readAllStandardError();
    if (!content.isEmpty())
        emit newStderr(content);
}

void LSPClient::onClientError(QProcess::ProcessError error)
{
    emit onServerError(error);
}

void LSPClient::onClientFinished(int exitCode, QProcess::ExitStatus status)
{
    emit onServerFinished(exitCode, status);
}

// Protocol methods
RequestID LSPClient::initialize(option<DocumentUri> rootUri)
{
    if (hasInitialized)
        return "[Didn't send request because the server is already initialized]";
    hasInitialized = true;
    InitializeParams params;
    params.processId = static_cast<unsigned int>(QCoreApplication::applicationPid());
    params.rootUri = rootUri;
    return SendRequest("initialize", params);
}
RequestID LSPClient::shutdown()
{
    return SendRequest("shutdown", json());
}
RequestID LSPClient::sync()
{
    return SendRequest("sync", json());
}
void LSPClient::exit()
{
    SendNotification("exit", json());
}
void LSPClient::initialized()
{
    SendNotification("initialized", json());
}
RequestID LSPClient::registerCapability()
{
    return SendRequest("client/registerCapability", json());
}
void LSPClient::didOpen(DocumentUri uri, string_ref text, string_ref languageId = "cpp")
{
    DidOpenTextDocumentParams params;
    params.textDocument.uri = uri;
    params.textDocument.text = text;
    params.textDocument.languageId = languageId;
    SendNotification("textDocument/didOpen", params);
}
void LSPClient::didClose(DocumentUri uri)
{
    DidCloseTextDocumentParams params;
    params.textDocument.uri = uri;
    SendNotification("textDocument/didClose", params);
}
void LSPClient::didChange(DocumentUri uri, std::vector<TextDocumentContentChangeEvent> &changes,
                          option<bool> wantDiagnostics)
{
    DidChangeTextDocumentParams params;
    params.textDocument.uri = uri;
    params.contentChanges = std::move(changes);
    params.wantDiagnostics = wantDiagnostics;
    SendNotification("textDocument/didChange", params);
}

RequestID LSPClient::documentColor(DocumentUri uri)
{
    DocumentSymbolParams params;
    params.textDocument.uri = uri;
    return SendRequest("textDocument/documentColor", params);
}
RequestID LSPClient::documentHighlight(DocumentUri uri, Position position)
{
    TextDocumentPositionParams params;
    params.textDocument.uri = uri;
    params.position = position;
    return SendRequest("textDocument/documentHighlight", params);
}

// general send and notify
void LSPClient::sendNotification(string_ref method, QJsonDocument &jsonDoc)
{
    json doc = toNlohmann(jsonDoc);
    notify(method, doc);
}

RequestID LSPClient::sendRequest(string_ref method, QJsonDocument &jsonDoc)
{
    std::string id = method.str();
    json doc = toNlohmann(jsonDoc);
    request(method, doc, id);
    return id;
}

// general send and notify
void LSPClient::SendNotification(string_ref method, json jsonDoc)
{
    notify(method, std::move(jsonDoc));
}

RequestID LSPClient::SendRequest(string_ref method, json jsonDoc)
{
    std::string id = method.str();
    request(method, std::move(jsonDoc), id);
    return id;
}

// private

void LSPClient::writeToServer(std::string &content)
{
    writeToServerBuffer.push_back("Content-Length: " + std::to_string(content.length()) + "\r\n");
    writeToServerBuffer.push_back("\r\n");
    writeToServerBuffer.push_back(content);
    if (clientProcess != nullptr && clientProcess->state() == QProcess::Running)
    {
        qDebug() << "writing to server...";
        for (auto s : writeToServerBuffer)
            clientProcess->write(s.c_str());
        writeToServerBuffer.clear();
    }
}

json LSPClient::toNlohmann(QJsonDocument &doc)
{
    return json{doc.toJson().toStdString()};
}

QJsonDocument LSPClient::toJSONDoc(json &nlohman)
{
    QString rawStr = QString::fromStdString(nlohman.dump());
    return QJsonDocument::fromJson(rawStr.toLocal8Bit());
}

void LSPClient::notify(string_ref method, json value)
{
    json payload = {{"jsonrpc", "2.0"}, {"method", method}, {"params", value}};
    std::string content = payload.dump();
    writeToServer(content);
}

void LSPClient::request(string_ref method, json param, RequestID id)
{
    json rpc = {{"jsonrpc", "2.0"}, {"id", id}, {"method", method}, {"params", param}};
    std::string content = rpc.dump();
    writeToServer(content);
}

LSPClient::~LSPClient()
{
    {
        if (clientProcess != nullptr)
        {
            clientProcess->kill();
            delete clientProcess;
        }
    }
}
