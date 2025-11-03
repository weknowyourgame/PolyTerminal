#ifndef CEF_POLYTERMINAL_APP_H_
#define CEF_POLYTERMINAL_APP_H_

#include "include/cef_app.h"

// Application-level callbacks for the browser process.
class App : public CefApp, public CefBrowserProcessHandler {
 public:
  App(const std::string& url, const std::string& window_title);

  // CefApp methods:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
    return this;
  }

  // CefBrowserProcessHandler methods:
  void OnContextInitialized() override;

 private:
  std::string url_;
  std::string window_title_;

  // Include the default reference counting implementation.
  IMPLEMENT_REFCOUNTING(App);
};

#endif  // CEF_POLYTERMINAL_APP_H_
