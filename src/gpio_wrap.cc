#include "node.h"
#include "gpio.h"
#include "gpio_wrap.h"

using namespace v8;

GPIOWrap::GPIOWrap() {}
GPIOWrap::~GPIOWrap() {}

Handle<Value>
GPIO::New(const Arguments &args) {
    HandleScope scope;

    if (!args[0]->IsUint32())
        return THROW_TYPE_ERROR("GPIO id must be an integer.");

    GPIO * gpio = new GPIO(args[0]->Int32Value());

    gpio->Wrap(args.This());

    return scope.Close(args.This());
}

Handle<Value>
GPIO::Value(const Arguments &args) {
    HandleScope scope;

    GPIO * gpio = ObjectWrap::Unwrap<GPIO>(args.This());

    switch (args->Length()) {
        case 0:
            int value = gpio->Value();
        
            return scope.Close(Integer::New(value));
        case 1:
            int value = args[0]->Int32Value();

            if (value != GPIO_LOW && value != GPIO_HIGH)
                return THROW_TYPE_ERROR("Value must be either LOW or HIGH.");

            try {
                gpio->Value(value);
            } catch(char * e) {
                return THROW_ERROR(e);
            }

            return scope.Close(args.This());
    }
    
    return THROW_TYPE_ERROR("Invalid arguments length.");
}

Handle<Value>
GPIO::Direction(const Arguments &args) {
    HandleScope scope;

    GPIO * gpio = ObjectWrap::Unwrap<GPIO>(args.This());

    switch (args->Length()) {
        case 0:
            int value = gpio->Direction();
        
            return scope.Close(Integer::New(value));
        case 1:
            int value = args[0]->Int32Value();

            if (value != GPIO_IN && value != GPIO_OUT)
                return THROW_TYPE_ERROR("Value must be either IN or OUT.");

            try {
                gpio->Direction(value);
            } catch(char * e) {
                return THROW_ERROR(e);
            }

            return scope.Close(args.This());
    }
    
    return THROW_TYPE_ERROR("Invalid arguments length.");
}

void
Initialize(Handle<Object> exports, Handle<Object> module) {
    Local<FunctionTemplate> tpl = FunctionTemplate::New(GPIOWrap::New);

    tpl->SetClassName(String::NewSymbol("GPIO"));

    tpl->InstanceTemplate()
        ->SetInternalFieldCount(1);

    tpl->Set(String::NewSymbol("IN"),
            Integer::New(GPIO_IN));
    tpl->Set(String::NewSymbol("OUT"),
            Integer::New(GPIO_OUT));
    tpl->Set(String::NewSymbol("LOW"),
            Integer::New(GPIO_LOW));
    tpl->Set(String::NewSymbol("HIGH"),
            Integer::New(GPIO_HIGH));

    tpl->PrototypeTemplate()
        ->Set(String::NewSymbol("value"), 
            FunctionTemplate::New(GPIO::Value)->GetFunction());
    tpl->PrototypeTemplate()
        ->Set(String::NewSymbol("direction"), 
            FunctionTemplate::New(GPIO::Direction)->GetFunction());

    target->Set(String::NewSymbol("exports"),
            Persistent<Function>::New(tpl->GetFunction()));
}

NODE_MODULE(gpio, Initialize);
