// truncate_itch.cpp — copy the first N framed ITCH 5.0 messages to a smaller file.
//
// Frames by the 2-byte big-endian length prefix (identical to the M0 count loop),
// so the cut ALWAYS lands on a message boundary and the output is itself a valid,
// smaller ITCH file you can develop and debug against instantly.
//
// This doubles as a reference for the framing loop you'll build in M0 proper.
//
// Build:  g++ -O2 -std=c++23 truncate_itch.cpp -o truncate_itch
//         (drop to -std=c++20 and swap std::byteswap if needed — not used here)
// Usage:  ./truncate_itch <in.NASDAQ_ITCH50> <out.itch> <N>

#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cerr << "usage: " << argv[0] << " <in> <out> <N>\n";
        return 2;
    }
    const char* in_path  = argv[1];
    const char* out_path = argv[2];
    const std::uint64_t n = std::strtoull(argv[3], nullptr, 10);

    std::ifstream in(in_path, std::ios::binary);
    std::ofstream out(out_path, std::ios::binary);
    if (!in)  { std::cerr << "cannot open input: "  << in_path  << '\n'; return 1; }
    if (!out) { std::cerr << "cannot open output: " << out_path << '\n'; return 1; }

    std::vector<char> buf;
    std::uint64_t count = 0;
    unsigned char hdr[2];

    while (count < n && in.read(reinterpret_cast<char*>(hdr), 2)) {
        const std::uint16_t len =
            (std::uint16_t(hdr[0]) << 8) | std::uint16_t(hdr[1]);   // big-endian length
        buf.resize(len);
        if (!in.read(buf.data(), len)) break;                       // truncated tail — stop

        out.write(reinterpret_cast<char*>(hdr), 2);                 // re-emit length prefix
        out.write(buf.data(), static_cast<std::streamsize>(len));   // + payload verbatim
        ++count;
    }

    std::cerr << "wrote " << count << " messages to " << out_path << '\n';
    return 0;
}
