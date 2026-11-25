#include "api.h"
#include "console.h"

#ifdef _WIN32
#include <windows.h>
using LibraryHandle = HMODULE;
#else
#include <dlfcn.h>
using LibraryHandle = void*;
#endif

struct Library {
    LibraryHandle handle = nullptr;
    SquareFunction square = nullptr;
    TranslationFunction translate = nullptr;
};

bool close_library(Library& library) {
    if (!library.handle) return true;

#ifdef _WIN32
    if (!FreeLibrary(library.handle)) {
        std::cerr << "FreeLibrary error: " << GetLastError() << '\n';
        return false;
    }
#else
    if (dlclose(library.handle) != 0) {
        std::cerr << "dlclose error: " << dlerror() << '\n';
        return false;
    }
#endif

    library = Library{};
    return true;
}

bool open_library(const char* path, Library& library) {
#ifdef _WIN32
    library.handle = LoadLibraryA(path);
    if (!library.handle) {
        std::cerr << "LoadLibrary error: " << GetLastError() << '\n';
        return false;
    }
    library.square = reinterpret_cast<SquareFunction>(GetProcAddress(library.handle, "Square"));
    if (!library.square) {
        std::cerr << "GetProcAddress Square error: " << GetLastError() << '\n';
        close_library(library);
        return false;
    }
    library.translate = reinterpret_cast<TranslationFunction>(GetProcAddress(library.handle, "translation"));
    if (!library.translate) {
        std::cerr << "GetProcAddress translation error: " << GetLastError() << '\n';
        close_library(library);
        return false;
    }
#else
    library.handle = dlopen(path, RTLD_NOW | RTLD_LOCAL);
    if (!library.handle) {
        std::cerr << "dlopen error: " << dlerror() << '\n';
        return false;
    }
    dlerror();
    library.square = reinterpret_cast<SquareFunction>(dlsym(library.handle, "Square"));
    const char* error = dlerror();
    if (error) {
        std::cerr << "dlsym Square error: " << error << '\n';
        close_library(library);
        return false;
    }
    dlerror();
    library.translate = reinterpret_cast<TranslationFunction>(dlsym(library.handle, "translation"));
    error = dlerror();
    if (error) {
        std::cerr << "dlsym translation error: " << error << '\n';
        close_library(library);
        return false;
    }
#endif
    return true;
}

int main() {
    const char* paths[] = {"./" LAB5_LIBRARY1, "./" LAB5_LIBRARY2};
    Library current;
    int active = 0;
    if (!open_library(paths[active], current)) {
        return 1;
    }

    std::cout << "Program 2: runtime loading\n" << "0: switch; 1 A B: area; 2 x: translation; EOF: exit\n"
              << "Library 1\n";
    int command;
    int result = 0;
    while (std::cin >> command) {
        if (command == 0) {
            int next = 1 - active;
            Library replacement;
            if (!open_library(paths[next], replacement)) {
                std::cerr << "Switch failed; current library remains active\n";
                continue;
            }

            Library previous = current;
            current = replacement;
            active = next;
            if (!close_library(previous)) {
                result = 1;
                break;
            }
            std::cout << "Library " << active + 1 << '\n';
        } 
        else if (!execute_command(command, current.square, current.translate)) {
            result = 1;
            break;
        }
    }
    if (!std::cin.eof() && std::cin.fail()) {
        std::cerr << "Input error: expected a command or its arguments\n";
        result = 1;
    }
    if (!close_library(current)) result = 1;
    return result;
}
