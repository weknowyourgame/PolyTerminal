#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include "constants.h"
#include <QString>
#include "globalShortcuts.h"

#include "cef_app/app.h"

#if defined(CEF_X11)
#include <X11/Xlib.h>
#endif

#include "include/base/cef_logging.h"
#include "include/cef_command_line.h"
#include "include/cef_app.h"

#if defined(CEF_X11)
namespace {
  int XErrorHandlerImpl(Display* display, XErrorEvent* event) {
    LOG(WARNING) << "X error received: " << "type " << event->type << ", "
                << "serial " << event->serial << ", " << "error_code "
                << static_cast<int>(event->error_code) << ", " << "request_code "
                << static_cast<int>(event->request_code) << ", " << "minor_code "
                << static_cast<int>(event->minor_code);
    return 0;
  }

  int XIOErrorHandlerImpl(Display* display) {
    return 0;
  }
}
#endif

int main(int argc, char **argv){
    
    // ===== CEF INITIALIZATION =====
    CefMainArgs main_args(argc, argv);

    // Check if this is a CEF sub-process
    int exit_code = CefExecuteProcess(main_args, nullptr, nullptr);
    if (exit_code >= 0) {
      return exit_code;  // CEF sub-process, exit early
    }

#if defined(CEF_X11)
    XSetErrorHandler(XErrorHandlerImpl);
    XSetIOErrorHandler(XIOErrorHandlerImpl);
#endif

    // Configure CEF settings
    CefSettings settings;
#if !defined(CEF_USE_SANDBOX)
    settings.no_sandbox = true;
#endif
    CefString(&settings.resources_dir_path).FromASCII(".");
    CefString(&settings.locales_dir_path).FromASCII("locales");

    // Create CEF app WITHOUT creating browser window yet
    // Pass empty strings - don't create browser in OnContextInitialized
    CefRefPtr<App> cefApp(new App("", ""));  // ← Empty, no auto-browser

    // Initialize CEF
    if (!CefInitialize(main_args, settings, cefApp.get(), nullptr)) {
      return CefGetExitCode();
    }

    // ===== QT APPLICATION =====
    QApplication qtApp(argc, argv);  // ← Different name!
    QMainWindow window;
    window.resize(1200, 800);
    window.setWindowFlags(Qt::FramelessWindowHint);

    // MenuBar
    QMenuBar *menuBar = window.menuBar();
    QAction *leftButton = menuBar->addAction(QString::fromStdString(Constants::LEFT_BUTTON));
    QAction *rightButton = menuBar->addAction(QString::fromStdString(Constants::RIGHT_BUTTON));
    QAction *profileButton = menuBar->addAction(QString::fromStdString(Constants::PROFILE_BUTTON));
    QAction *settingsButton = menuBar->addAction(QString::fromStdString(Constants::SETTINGS_BUTTON));

    // Shortcut Configs
    GlobalKeyFilter *filter = new GlobalKeyFilter(&window);
    qtApp.installEventFilter(filter);

    window.show();
    qInfo() << "Application started";

    // Run Qt event loop
    int result = qtApp.exec();

    // Cleanup CEF on exit
    CefShutdown();
    
    return result;
}