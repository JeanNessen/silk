#include <chrono>
#include <print>

#include <slk/core/types.h>
#include <slk/io/file.h>

#include "handler.h"
#include "parse_file.h"


int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::println("Need to provide the path to the file");
        return 1;
    }

    slk::Result<slk::MappedFile> file = slk::Open(argv[1]);

    if (!file.has_value())
    {
        std::println("Failed to load file, errno {}", file.error().no);
        return 1;
    }

    std::println("Loaded File Successfully!");

    StdOutHandler handler;

    auto start = std::chrono::high_resolution_clock::now();
    slk::ParseFile<StdOutHandler>(*file, handler);
    auto end = std::chrono::high_resolution_clock::now();

    auto processingTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    handler.PrintResults();
    std::println("Took {}ms", processingTime);
    return 0;
}
