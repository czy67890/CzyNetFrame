//
// Created by czy on 2023/3/15.
//

#ifndef CZYSERVER_CALLBACKDEFINE_H
#define CZYSERVER_CALLBACKDEFINE_H

#include <memory>
#include <functional>

namespace CzyNetFrame {
    class Buffer;

    class TimeStamp;

    class TcpConnection;

    using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
    using ConnectionCallBack = std::function<void(const TcpConnectionPtr &connectionPtr)>;
    using MessageCallBack = std::function<void(const TcpConnectionPtr &connectionPtr, Buffer *, TimeStamp)>;
    using HighWaterCallBack = std::function<void(const TcpConnectionPtr &, size_t)>;

}
#endif //CZYSERVER_CALLBACKDEFINE_H
