#include "ViewerApp.hpp"

#include <string>
#include <algorithm>

#ifdef _WIN32
#include <direct.h>
#define chdir _chdir
#define PATH_SEPARATOR '\\'
#else
#include <unistd.h>
#define PATH_SEPARATOR '/'
#endif

std::string dirname(const std::string& path) {
    size_t pos = path.find_last_of(PATH_SEPARATOR);
    return (std::string::npos == pos) ? "" : path.substr(0, pos);
}

int main(int argc, char* argv[]) {
    std::string dir = dirname(argv[0]);
    if (chdir(dir.c_str()) != 0) {
        Log::Error("Chyba při změně pracovního adresáře.");
        return 1;
    }

    ViewerApp app;
    app.Run((argc > 1) ? argv[1] : std::string());
    return 0;
}
