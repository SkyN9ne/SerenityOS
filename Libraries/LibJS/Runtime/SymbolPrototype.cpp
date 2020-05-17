/*
 * Copyright (c) 2020, Matthew Olsson <matthewcolsson@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <AK/Function.h>
#include <AK/StringBuilder.h>
#include <LibJS/Heap/Heap.h>
#include <LibJS/Interpreter.h>
#include <LibJS/Runtime/Error.h>
#include <LibJS/Runtime/GlobalObject.h>
#include <LibJS/Runtime/Object.h>
#include <LibJS/Runtime/PrimitiveString.h>
#include <LibJS/Runtime/SymbolObject.h>
#include <LibJS/Runtime/SymbolPrototype.h>
#include <LibJS/Runtime/Value.h>
#include <string.h>

namespace JS {

SymbolPrototype::SymbolPrototype()
    : Object(interpreter().global_object().object_prototype())
{
    put_native_property("description", description_getter, nullptr, Attribute::Configurable);

    put_native_function("toString", to_string, 0, Attribute::Writable | Attribute::Configurable);
    put_native_function("valueOf", value_of, 0, Attribute::Writable | Attribute::Configurable);
}

SymbolPrototype::~SymbolPrototype()
{
}

static SymbolObject* this_symbol_from_interpreter(Interpreter& interpreter)
{
    auto* this_object = interpreter.this_value().to_object(interpreter);
    if (!this_object)
        return nullptr;
    if (!this_object->is_symbol_object()) {
        interpreter.throw_exception<TypeError>("object must be of type Symbol");
        return nullptr;
    }
    return static_cast<SymbolObject*>(this_object);
}

Value SymbolPrototype::description_getter(Interpreter& interpreter)
{
    auto* this_object = this_symbol_from_interpreter(interpreter);
    if (!this_object)
        return {};
    return js_string(interpreter, this_object->description());
}

Value SymbolPrototype::to_string(Interpreter& interpreter)
{
    auto* this_object = this_symbol_from_interpreter(interpreter);
    if (!this_object)
        return {};
    auto string = this_object->primitive_symbol().to_string();
    return js_string(interpreter, move(string));
}

Value SymbolPrototype::value_of(Interpreter& interpreter)
{
    auto* this_object = this_symbol_from_interpreter(interpreter);
    if (!this_object)
        return {};
    return this_object->value_of();
}

}
