// offline_genome.cpp — resolve_genome_path for the headless module (make headless), in
// place of wasm_online.cpp: that streams a URL reference through gw's page's
// online-files.mjs and needs Asyncify, neither of which the headless module has. A host
// puts remote files in the file system itself (gwplot-js: fs_bridge's mountUrl) and passes
// their paths.

#include <iostream>
#include <string>

#include "utils.h"

std::string resolve_genome_path(const std::string& path) {
    if (path.rfind("http://", 0) != 0 && path.rfind("https://", 0) != 0) return path;
    std::cerr << "Error: the headless module can't open a reference by URL (" << path
              << "); put it in the file system first and pass its path\n";
    return "";
}
