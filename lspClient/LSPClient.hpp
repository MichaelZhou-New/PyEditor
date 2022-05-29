#ifndef LSPCLIENT_HPP
#define LSPCLIENT_HPP

#include "../lsp-cpp/include/LSP.hpp"
#include "../lsp-cpp/include/LSPUri.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QObject>
#include <QProcess>

class LSPClient : public QObject
{
    Q_OBJECT

    using RequestID = std::string;

  public:
    explicit LSPClient(QString processPath, QStringList args);

    LSPClient(LSPClient &&) = delete;
    LSPClient(LSPClient &) = delete;

    LSPClient &operator=(LSPClient &&) = delete;
    LSPClient &operator=(LSPClient &) = delete;

    ~LSPClient() final;

    // LSP methods Requests to send to server
    RequestID initialize(option<DocumentUri> rootUri = {});
    RequestID shutdown();
    RequestID sync();
    RequestID registerCapability();

    RequestID documentColor(DocumentUri uri);
    RequestID documentHighlight(DocumentUri uri, Position position);

    // LSP methods notifications to Send to server
    void exit();
    void initialized();
    void didOpen(DocumentUri uri, string_ref code, string_ref lang);
    void didClose(DocumentUri uri);
    void didChange(DocumentUri uri, std::vector<TextDocumentContentChangeEvent> &changes,
                   option<bool> wantDiagnostics = {});

    // General sender and requester for sever
    void sendNotification(string_ref method, QJsonDocument &jsonDoc);
    RequestID sendRequest(string_ref method, QJsonDocument &jsonDoc);

  signals:
    void onNotify(QString method, QJsonObject param);
    void onResponse(QJsonObject id, QJsonObject response);
    void onDocumentColorResponse(QString id, QJsonArray response);
    void onRequest(QString method, QJsonObject param, QJsonObject id);
    void onError(QJsonObject id, QJsonObject error);
    void onServerError(QProcess::ProcessError error);
    void onServerFinished(int exitCode, QProcess::ExitStatus status);
    void newStderr(const QString &content);

  private slots:
    void onClientReadyReadStdout();
    void onClientReadyReadStderr();
    void onClientError(QProcess::ProcessError error);
    void onClientFinished(int exitCode, QProcess::ExitStatus status);

  private:
    QProcess *clientProcess = nullptr;
    std::vector<std::string> writeToServerBuffer;
    bool hasInitialized = false;

    void writeToServer(std::string &in);

    QJsonDocument toJSONDoc(json &nlohman);
    json toNlohmann(QJsonDocument &doc);

    void notify(string_ref method, json value);
    void request(string_ref mthod, json param, RequestID id);

    void SendNotification(string_ref method, json jsonDoc);
    RequestID SendRequest(string_ref method, json jsonDoc);
};

#endif
