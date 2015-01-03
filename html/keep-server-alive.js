/*
 * Copyright 2013 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Include this script in each UI page to ensure that the server keeps running.
// You can display an indicator of the server status in your page by including
// elements with the IDs 'serverStatusAlive' and 'serverStatusDead', with
// display:none. When the server status is determined, the appropriate element
// will be given the style display:block.

(function() {
var keepServerAliveRequestsSent = 0;
var keepServerAliveRequest = null;
var serverStatusAlive = document.getElementById('serverStatusAlive');
var serverStatusDead = document.getElementById('serverStatusDead');
var running = false;
var pageNavigated = false;
window.onbeforeunload = function() {
  pageNavigated = true;
}
function updateDisplay(newValue) {
  if (!serverStatusAlive || !serverStatusDead) return;
  running = newValue;
  // Some browsers (Chrome) kill XMLHttpRequests before calling onbeforeunload.
  // To avoid falsely reporting that the server has died, wait one frame before
  // actually updating the UI to give the browser a chance to call
  // onbeforeunload first.
  setTimeout(function() {
    if (pageNavigated) return;
    serverStatusAlive.style.display = running ? 'block' : 'none';
    serverStatusDead.style.display = running ? 'none' : 'block';
  }, 17);
}
function sendKeepServerAliveRequest() {
  keepServerAliveRequest = new XMLHttpRequest();
  keepServerAliveRequest.open('GET', '/keepServerAlive?randomNumber=' + Math.random(), true);
  keepServerAliveRequest.onreadystatechange = function() {
    if (!running && keepServerAliveRequest.readyState < 4 && keepServerAliveRequest.readyState > 2) {
      updateDisplay(true);
    }
    if (keepServerAliveRequest.readyState == 4) {
      updateDisplay(false);
      window.setTimeout(sendKeepServerAliveRequest, 250);
    }
  };
  keepServerAliveRequest.send();
}
sendKeepServerAliveRequest();
})();
