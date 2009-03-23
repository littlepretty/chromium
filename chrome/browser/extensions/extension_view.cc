// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/extensions/extension_view.h"

#include "chrome/browser/extensions/extension.h"
#include "chrome/browser/extensions/extension_message_service.h"
#include "chrome/browser/renderer_host/render_view_host.h"

ExtensionView::ExtensionView(
    Extension* extension, const GURL& url, Profile* profile) :
    HWNDHtmlView(url, this, false), extension_(extension), profile_(profile) {
  // TODO(mpcomplete): query this from the renderer somehow?
  set_preferred_size(gfx::Size(100, 100));
}

void ExtensionView::CreatingRenderer() {
  render_view_host()->AllowExtensionBindings();
}

void ExtensionView::RenderViewCreated(RenderViewHost* render_view_host) {
  ExtensionMessageService::GetInstance()->RegisterExtensionView(this);
}

WebPreferences ExtensionView::GetWebkitPrefs() {
  // TODO(mpcomplete): return some reasonable prefs.
  return WebPreferences();
}

void ExtensionView::RunJavaScriptMessage(
    const std::wstring& message,
    const std::wstring& default_prompt,
    const GURL& frame_url,
    const int flags,
    IPC::Message* reply_msg,
    bool* did_suppress_message) {
  // Automatically cancel the javascript alert (otherwise the renderer hangs
  // indefinitely).
  *did_suppress_message = true;
  render_view_host()->JavaScriptMessageBoxClosed(reply_msg, true, L"");
}
