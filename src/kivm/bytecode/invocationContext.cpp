//
// Created by kiva on 2018/4/13.
//
#include <kivm/bytecode/invocationContext.h>
#include <kivm/bytecode/execution.h>
#include <kivm/runtime/thread.h>
#include <kivm/oop/primitiveOop.h>
#include <kivm/oop/mirrorOop.h>

namespace kivm {
    InvocationContext::InvocationContext(JavaThread *thread, Method *method, Stack *stack)
        : _thread(thread), _method(method), _stack(stack), _instanceKlass(_method->getClass()),
          _obtainArgsFromStack(true) {
    }

    InvocationContext::InvocationContext(JavaThread *thread, Method *method, const std::list<oop> &args)
        : _thread(thread), _method(method), _stack(nullptr), _instanceKlass(_method->getClass()),
          _obtainArgsFromStack(false), _args(args) {
    }

    void InvocationContext::prepareEnvironment() {
        Execution::initializeClass(_thread, _instanceKlass);
    }

    void InvocationContext::prepareSynchronized(oop thisObject) {
        if (_method->isSynchronized()) {
            D("invocationContext: method is synchronized");
            if (_method->isStatic()) {
                _method->getClass()->getJavaMirror()->getMarkOop()->monitorEnter();
            } else {
                thisObject->getMarkOop()->monitorEnter();
            }
        }
    }

    void InvocationContext::finishSynchronized(oop thisObject) {
        if (_method->isSynchronized()) {
            if (_method->isStatic()) {
                _method->getClass()->getJavaMirror()->getMarkOop()->monitorExit();
            } else {
                thisObject->getMarkOop()->monitorExit();
            }
        }
    }
}
