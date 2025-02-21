﻿#include "HttpServerConnection.h"
#include "BoostServer.h"
#include "../Utils/Log.h"

HttpServerConnection::HttpServerConnection(BoostServer* server, tcp::socket& socket)
    : mSession(server->generateSession()),
    mServer(server),
    mSocket(std::move(socket))  // 转成右值引用
{
    LOGI("");

}
HttpServerConnection::~HttpServerConnection() {
    LOGI("");
    if (fp) {
        fclose(fp);
        fp = nullptr;
    }
    this->mSocket.close();
    this->mTimer.cancel();
}
std::string HttpServerConnection::getSession() {
    return mSession;
}

void HttpServerConnection::run()
{
    http::async_read(
        mSocket,
        mTempBuffer,
        mHttpRequest,
        // ReadHandler, 回调, 数据读完就会调用这个
        [this](beast::error_code ec,
            std::size_t bytes_transferred)
        {

            boost::ignore_unused(bytes_transferred);
            if (ec) {
                //接收可读数据失败
                LOGE("run error,msg=%s", ec.message().data());
                m_disconnectionCallback(m_arg, mSession);
            }
            else {
                this->handle(); // 处理请求
            }
        });

    /*
    mTimer.async_wait(
        [this](beast::error_code ec)
        {
            if (ec)
            {
            }
        }
    );*/
}
void HttpServerConnection::handle()
{
    mHttpResponse.version(mHttpRequest.version());
    mHttpResponse.set(http::field::server, "Boost");

    switch (mHttpRequest.method())
    {
    case http::verb::get:
    {

        if (mHttpRequest.target() == "/test.flv")
        {
            // 设置响应头
            mHttpResponse.result(http::status::ok);
            mHttpResponse.keep_alive(true);
            mHttpResponse.set(http::field::access_control_allow_origin, "*");
            mHttpResponse.set(http::field::content_type, "video/x-flv");
            mHttpResponse.set(http::field::connection, "close");
            mHttpResponse.set(http::field::expires, "-1");
            mHttpResponse.set(http::field::pragma, "no-cache");
            mHttpResponse.content_length(-1);

            // 发送响应头
            http::async_write(
                mSocket,
                mHttpResponse,
                // 写回调, 写完就会调用这个
                [this](beast::error_code ec, std::size_t)
                {
                    if (ec)
                    {
                        //发送失败
                        LOGE("play flv error,msg=%s", ec.message().data());
                        //this->mSocket.shutdown(tcp::socket::shutdown_send, ec);
                        m_disconnectionCallback(m_arg, mSession);
                    }
                    else {
                        //发送成功
                        LOGI("play flv success");
                        //const char* filename = "../data/test.flv";
                        const char* filename = "../data/笑傲江湖天地作合.flv";

                        fp = fopen(filename, "rb");
                        // 发送完响应头后, 就不断发送文件内容(响应体)
                        this->keepWrite();

                    }
                });
        }
        else        // 不处理其他文件请求
        {
            mHttpResponse.result(http::status::ok);
            mHttpResponse.keep_alive(false);
            mHttpResponse.result(http::status::not_found);
            mHttpResponse.set(http::field::content_type, "text/plain");
            beast::ostream(mHttpResponse.body()) << "File not found\r\n";

            http::async_write(
                mSocket,
                mHttpResponse,
                [this](beast::error_code ec, std::size_t)
                {
                    if (ec)
                    {
                        //发送失败
                        LOGE("http::async_write error,msg=%s", ec.message().data());
                        //this->mSocket.shutdown(tcp::socket::shutdown_send, ec
                    }
                    else {
                        //发送成功
                        LOGI("http::async_write success,msg=%s", ec.message().data());
                    }
                    m_disconnectionCallback(m_arg, mSession);
                });
        }
        break;
    }
    default:    // 不处理get以外的方法
    {
        mHttpResponse.result(http::status::bad_request);
        mHttpResponse.set(http::field::content_type, "text/plain");
        beast::ostream(mHttpResponse.body())
            << "Invalid request-method '"
            << std::string(mHttpRequest.method_string())
            << "'";
        mHttpResponse.content_length(mHttpResponse.body().size());

        http::async_write(
            mSocket,
            mHttpResponse,
            [this](beast::error_code ec, std::size_t)
            {
                if (ec)
                {
                    //发送失败
                    LOGE("http::async_write error,msg=%s", ec.message().data());
                    //this->mSocket.shutdown(tcp::socket::shutdown_send, ec);
                }
                else {
                    //发送成功
                    LOGI("http::async_write success,msg=%s", ec.message().data());
                }
                m_disconnectionCallback(m_arg, mSession);
            });
        break;
    }

    }

}
// 只要不发送失败, 就一直发送文件内容
void HttpServerConnection::keepWrite() {
    char data[5000];
    int  size;

    size = fread(data, 1, sizeof(data), fp);
    if (size > 0) {
        boost::asio::async_write(
            mSocket,
            boost::asio::buffer(data, size),
            [this](beast::error_code ec, std::size_t)
            {
                if (ec)
                {
                    //发送失败
                    LOGE("keepWrite error,msg=%s", ec.message().data());
                    m_disconnectionCallback(m_arg, mSession);
                }
                else {
                    //发送成功
                    //LOGI("keepWrite successs");
                    this->keepWrite();
                }

            });
    }
    else {
        LOGE("keepWrite error,msg= flv buffer finish");
        m_disconnectionCallback(m_arg, mSession);
    }

}


