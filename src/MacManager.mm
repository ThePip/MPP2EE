#include <Cocoa/Cocoa.h>
#include "MacManager.h"

namespace Internal {
	inline NSWindow * getNSWindow(long winId = -1) {
		if(winId == -1)
		{
			QWindowList windows = QGuiApplication::allWindows();
			QWindow* win = windows.first();
			winId = win->winId();
		}

		NSView *nativeView = reinterpret_cast<NSView *>(winId);
		NSWindow* nativeWindow = [nativeView window];
		return nativeWindow;
	}

	inline NSButton * getZoomButton(NSWindow * nsWindow){
		return [nsWindow standardWindowButton:NSWindowZoomButton];
	}

	inline NSButton * getMiniaturizeButton(NSWindow * nsWindow){
		return [nsWindow standardWindowButton:NSWindowMiniaturizeButton];
	}

	inline NSButton * getCloseButton(NSWindow * nsWindow){
		return [nsWindow standardWindowButton:NSWindowCloseButton];
	}

	inline void toggleAlwaysOnTop(NSWindow * nsWindow, bool alwaysOnTop){
		if(alwaysOnTop)
			[nsWindow setLevel: NSStatusWindowLevel];
		else
			[nsWindow setLevel: NSNormalWindowLevel];
	}

	inline void hideButton(NSButton *button) {
		[button setHidden:YES];
		//button.alphaValue = 0.0;
		//[button setEnabled:NO];
		//button.image = nil;
		//button.alternateImage = nil;
	}

	inline void showButton(NSButton *button) {
		[button setHidden:NO];
	}

	inline void disableButton(NSButton *button) {
		[button setEnabled:NO];
	}

	inline void enableButton(NSButton *button) {
		[button setEnabled:YES];
	}
}
void MacManager::removeTitlebarFromWindow(long winId) {
	NSWindow * nativeWindow = Internal::getNSWindow(winId);
	[nativeWindow setStyleMask:[nativeWindow styleMask] | NSWindowStyleMaskFullSizeContentView];
	[nativeWindow setTitlebarAppearsTransparent:YES];
	[nativeWindow setMovableByWindowBackground:NO];
	[nativeWindow setTitleVisibility:NSWindowTitleVisibility::NSWindowTitleHidden];
}

void MacManager::hideZoomButton(long winId){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button = Internal::getZoomButton(nativeWindow);
	Internal::hideButton(button);
}

void MacManager::disableZoomButton(long winId){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button = Internal::getZoomButton(nativeWindow);
	Internal::disableButton(button);
}

void MacManager::hideButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button;
	if(closeButton) {
		button = Internal::getCloseButton(nativeWindow);
		Internal::hideButton(button);
	}
	if(miniaturizeButton) {
		button = Internal::getMiniaturizeButton(nativeWindow);
		Internal::hideButton(button);
	}
	if(zoomButton) {
		button = Internal::getZoomButton(nativeWindow);
		Internal::hideButton(button);
	}
}
void MacManager::showButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button;
	if(closeButton) {
		button = Internal::getCloseButton(nativeWindow);
		Internal::showButton(button);
	}
	if(miniaturizeButton) {
		button = Internal::getMiniaturizeButton(nativeWindow);
		Internal::showButton(button);
	}
	if(zoomButton) {
		button = Internal::getZoomButton(nativeWindow);
		Internal::showButton(button);
	}
}

void MacManager::disableButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button;
	if(closeButton) {
		button = Internal::getCloseButton(nativeWindow);
		Internal::disableButton(button);
	}
	if(miniaturizeButton) {
		button = Internal::getMiniaturizeButton(nativeWindow);
		Internal::disableButton(button);
	}
	if(zoomButton) {
		button = Internal::getZoomButton(nativeWindow);
		Internal::disableButton(button);
	}
}

void MacManager::enableButton(long winId, bool closeButton, bool miniaturizeButton, bool zoomButton){
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	NSButton *button;
	if(closeButton) {
		button = Internal::getCloseButton(nativeWindow);
		Internal::enableButton(button);
	}
	if(miniaturizeButton) {
		button = Internal::getMiniaturizeButton(nativeWindow);
		Internal::enableButton(button);
	}
	if(zoomButton) {
		button = Internal::getZoomButton(nativeWindow);
		Internal::enableButton(button);
	}
}


void MacManager::nonFunctionalZoomButton(long winId)
{
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	[nativeWindow setCollectionBehavior:NSWindowCollectionBehaviorFullScreenNone];
}

void MacManager::toggleAlwaysOnTop(long winId, bool alwaysOnTop) {
	NSWindow* nativeWindow = Internal::getNSWindow(winId);
	Internal::toggleAlwaysOnTop(nativeWindow, alwaysOnTop);
}
