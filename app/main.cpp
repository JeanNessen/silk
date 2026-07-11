#include <print>

#include "silk/version.hpp"

int main() {
    std::println("silk {}", silk::library_version());
    return 0;
}
