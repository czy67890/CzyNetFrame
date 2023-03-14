//
// Created by czy on 2023/3/14.
//

#ifndef CZYSERVER_WEAKCALLBACK_H
#define CZYSERVER_WEAKCALLBACK_H

#include <functional>
#include <memory>

namespace CzyNetFrame {
    template<typename CLASS, typename ...Args>
    class WeakCallBack {
    public:
        WeakCallBack(const std::weak_ptr<CLASS> &obj, std::function<void(CLASS *, Args...)> func)
                : function_(func), obj_(obj) {
        }

        void operator()(Args &&... args) const {
            std::shared_ptr<CLASS> ptr = obj_.lock();
            if (ptr) {
                function_(ptr.get(), std::forward<Args>(args)...);
            }
        }

    private:
        std::function<void(CLASS *, Args...)> function_;
        std::weak_ptr<CLASS> obj_;
    };

    template<typename CLASS, typename ...Args>

    WeakCallBack<CLASS, Args...> makeWkCallBack(const std::shared_ptr<CLASS> &sptr, void(CLASS::*function)(Args...)) {
        return WeakCallBack<CLASS, Args...>(sptr, function);
    }


    template<typename CLASS, typename ...Args>

    WeakCallBack<CLASS, Args...>
    makeWkCallBack(const std::shared_ptr<CLASS> &sptr, void(CLASS::*function)(Args...) const) {
        return WeakCallBack<CLASS, Args...>(sptr, function);
    }


}


#endif //CZYSERVER_WEAKCALLBACK_H
