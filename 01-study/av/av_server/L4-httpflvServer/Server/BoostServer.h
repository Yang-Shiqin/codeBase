#ifndef BOOSTSERVER_H
#define BOOSTSERVER_H

#include "boost.h"
#include <map>

class Config;
class HttpServerConnection;

class BoostServer
{
public:
	BoostServer(Config* config);
	~BoostServer();
public:
	void start();

	bool addConn(HttpServerConnection* conn);	// 添加连接
	bool removeConn(std::string session);		// 移除连接
	HttpServerConnection* getConn(std::string session);	// 获取连接
	bool sendData(char* data, int size);	// 可以实现视频源来自摄像头等更多扩展功能, 而不是来自本地文件, 但这里没实现

	static void cbDisconnection(void* arg, std::string session);	// 断开连接的回调函数(静态, 但暂时不知道为啥设静态)
	std::string generateSession();	// 生成session(8位随机数string)(其实不应该叫session, session对应资源, 但这里其实对应的是连接)
private:
	void handleDisconnection(std::string session);	// 断开连接的回调函数实现

	void setOnAccept();		// 设置acceptor的回调函数为onAccept
	void onAccept(beast::error_code ec, tcp::socket socket);	// 接收连接的回调函数

private:
	Config* mConfig;
	net::io_context* mIoc;
	tcp::acceptor*   mAcceptor;
	std::map<std::string, HttpServerConnection*> m_connMap;// <session,conn> 维护所有被创建的连接
	std::mutex								     m_connMap_mtx;

};


#endif //BOOSTSERVER_H