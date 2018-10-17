#include "gemfire_worker.hpp"

#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exceptions.hpp"

using namespace v8;

namespace node_gemfire {

void GemfireWorker::Execute() {
  try {
    ExecuteGemfireWork();
  } catch (apache::geode::client::Exception& exception) {
    // TODO : need to figure out logging for debug level info.
    SetError(exception.getName().c_str(), exception.getMessage().c_str());
  }
}
void GemfireWorker::HandleErrorCallback() {
  static const int argc = 1;
  Local<Value> argv[argc] = {errorObject()};
  callback->Call(argc, argv);
}
void GemfireWorker::SetError(const char* name, const char* message) {
  errorName = name;
  SetErrorMessage(message);
}

Local<Value> GemfireWorker::errorObject() {
  Nan::EscapableHandleScope scope;
  Local<Object> err = Nan::Error(ErrorMessage()).As<v8::Object>();
  Nan::Set(err, Nan::New("name").ToLocalChecked(),
           Nan::New(errorName).ToLocalChecked());
  return scope.Escape(err);
}
}  // namespace node_gemfire
