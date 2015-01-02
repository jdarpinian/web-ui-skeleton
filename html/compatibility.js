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

parseQueryString = function(encodedString, useArrays) {
  // strip a leading '?' from the encoded string
  var qstr = (encodedString[0] == "?") ? encodedString.substring(1) :
                                         encodedString;
  var pairs = qstr.replace(/\+/g, "%20").split(/(\&amp\;|\&\#38\;|\&#x26;|\&)/);
  var o = {};
  var decode;
  if (typeof(decodeURIComponent) != "undefined") {
    decode = decodeURIComponent;
  } else {
    decode = unescape;
  }
  if (useArrays) {
    for (var i = 0; i < pairs.length; i++) {
      var pair = pairs[i].split("=");
      if (pair.length !== 2) {
        continue;
      }
      var name = decode(pair[0]);
      var arr = o[name];
      if (!(arr instanceof Array)) {
        arr = [];
        o[name] = arr;
      }
      arr.push(decode(pair[1]));
    }
  } else {
    for (i = 0; i < pairs.length; i++) {
      pair = pairs[i].split("=");
      if (pair.length !== 2) {
        continue;
      }
      o[decode(pair[0])] = decode(pair[1]);
    }
  }
  return o;
};

var capitalize = function(s) {
  var firstLetter = s.slice(0, 1);
  var rest = s.slice(1);
  return firstLetter.toUpperCase() + rest;
};

var uppercasePrefixes = ['', 'Moz', 'ms', 'o', 'WebKit'];
var lowercasePrefixes = uppercasePrefixes.slice(0);
for (var i = 0; i < lowercasePrefixes.length; i++) {
  lowercasePrefixes[i] = lowercasePrefixes[i].toLowerCase();
}
var cssPrefixes = [''];
for (var i = 1; i < lowercasePrefixes.length; i++) {
  cssPrefixes.push('-' + lowercasePrefixes[i] + '-');
}
var prefixes = lowercasePrefixes.concat(uppercasePrefixes.slice(1));

var getPrefixed = function(toCheck, on) {
  if (!on)
    return undefined;
  for (var i = 0; i < prefixes.length; i++) {
    var prefix = prefixes[i];
    var checked = on[prefix + toCheck];
    if (checked !== undefined) return checked;
    checked = on[prefix + capitalize(toCheck)];
    if (checked !== undefined) return checked;
  }
  return undefined;
};

var hasProperty = function(toCheck, on) {
  return on.hasOwnProperty(toCheck) || (Object.getPrototypeOf(on) && hasProperty(toCheck, Object.getPrototypeOf(on)));
};

var setPrefixed = function(toSet, value, on) {
  for (var i = 0; i < prefixes.length; i++) {
    var prefix = prefixes[i];
    if (hasProperty(prefix + toSet, on)) {
      on[prefix + toSet] = value;
      return true;
    }
    if (hasProperty(prefix + capitalize(toSet), on)) {
      on[prefix + capitalize(toSet)] = value;
      return true;
    }
  }
  return false;
};

if (!window.devicePixelRatio) {
  window.devicePixelRatio = 1;
}

var performanceNow = getPrefixed('now', window.performance);
if (performanceNow) {
  window.performance.now = performanceNow;
}

var getMs = function() {
  if(performanceNow)
    return window.performance.now();
  else
    return new Date().getTime();
};
