#include "app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"
#include "client.h"

App::App(const std::string& url, const std::string& window_title)
    : url_(url), window_title_(window_title) {}

void App::OnContextInitialized() {
  CEF_REQUIRE_UI_THREAD();

  // Client implements browser-level callbacks.
  CefRefPtr<Client> handler(new Client);

  // Specify CEF browser settings here.
  CefBrowserSettings browser_settings;

  // Use the URL and window title passed from main.cpp
  std::string url = url_;
  
  // Information used when creating the native window.
  CefWindowInfo window_info;

#if defined(OS_WIN)
  // On Windows we need to specify certain flags that will be passed to
  // CreateWindowEx().
  window_info.SetAsPopup(nullptr, window_title_.c_str());
#elif defined(CEF_X11)
  // On Linux/X11, create a top-level window
  // CEF will automatically create the window - we just need to provide basic info
  // Leave window_info mostly default - CEF handles window creation on Linux
#endif

  // Use Alloy runtime style - minimal embedded browser without Chrome UI
  window_info.runtime_style = CEF_RUNTIME_STYLE_ALLOY;

  // Create the first browser window using native window mode.
  CefBrowserHost::CreateBrowser(window_info, handler, url, browser_settings,nullptr, nullptr);
}
