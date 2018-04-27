//
// Created by kiva on 2018/4/26.
//

#include <kivm/bytecode/invocationContext.h>

namespace kivm {
    Method *InvocationContext::resolveVirtualMethod(oop thisObject, Method *tagMethod) {
        // if method is not an interface method
        // there's no need to resolve twice
        // just return itself
        if (tagMethod->getMaxLocals() != 0) {
            return tagMethod;
        }

        Method *resolved = nullptr;
        if (thisObject->getClass()->getClassType() == ClassType::INSTANCE_CLASS) {
            auto instanceClass = (InstanceKlass*) thisObject->getClass();
            resolved = instanceClass->getVirtualMethod(tagMethod->getName(), tagMethod->getDescriptor());
        }
        return resolved;
    }
}