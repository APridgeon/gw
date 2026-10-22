// wasm_online.cpp — resolve_genome_path for gw's browser page: a reference given as an
// http(s) URL is streamed through the page's online-files.mjs (deps/wasm), mounted as a
// file in Emscripten's file system. Needs Asyncify (EM_ASYNC_JS).
//
// The headless module (make headless) leaves this file out and links
// src/headless/offline_genome.cpp instead, so it doesn't name online-files.mjs, which it
// doesn't ship. On non-Emscripten targets this file is empty.

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/em_js.h>
#include <cstdlib>
#include <map>
#include <string>

#include "utils.h"

// CLI/session reference URLs use the same bounded transport as Open → Online.
// This also avoids Asyncify suspension inside individual readv buffers.
EM_ASYNC_JS(char*, gw_mount_remote_js, (const char* url), {
    try {
        var api=await import('./online-files.mjs');
        var resource={id:'reference', name:'Reference', category:'reference', url:UTF8ToString(url)};
        var prepared=await api.prepareOnline(resource, {FS:FS, Module:Module, status:function(text){console.log(text);}});
        var text=prepared.entry.url;
        var bytes=lengthBytesUTF8(text)+1;
        var ptr=_malloc(bytes);stringToUTF8(text,ptr,bytes);return ptr;
    } catch(error) {console.error('[GW] Cannot stream reference:',error);return 0;}
});

std::string resolve_genome_path(const std::string& path) {
    if (path.rfind("http://",0)!=0 && path.rfind("https://",0)!=0) return path;
    static std::map<std::string,std::string> mounted;
    auto it=mounted.find(path);if(it!=mounted.end())return it->second;
    char* local=gw_mount_remote_js(path.c_str());
    if(!local)return "";
    std::string result(local);std::free(local);mounted[path]=result;return result;
}

#endif  // __EMSCRIPTEN__
