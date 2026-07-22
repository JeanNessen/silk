#include <chrono>
#include <print>

#include <slk/core/types.h>
#include <slk/io/file.h>
#include <slk/parser/parse_file.h>

class StdOutHandler final
{
public:
    void OnAdd(silk::OrderAdd) { ++m_numAdd; }
    void OnAddMPID(silk::OrderAddMPID) { ++m_numAddMPID; }
    void OnExecuted(silk::OrderExecuted) { ++m_numExecuted; }
    void OnExecutedWithPrice(silk::OrderExecutedWithPrice)
    {
        ++m_numExecutedWithPrice;
    }
    void OnCanceled(silk::OrderCanceled) { ++m_numCanceled; }
    void OnDeleted(silk::OrderDeleted) { ++m_numDeleted; }
    void OnReplaced(silk::OrderReplaced) { ++m_numReplaced; }
    void OnSkipped() { ++m_numSkipped; }

    void PrintResults() const
    {
        std::println("Num Adds:              {}", m_numAdd);
        std::println("Num AddsMPID:          {}", m_numAddMPID);
        std::println("Num Executed:          {}", m_numExecuted);
        std::println("Num ExecutedWithPrice: {}", m_numExecutedWithPrice);
        std::println("Num Canceled:          {}", m_numCanceled);
        std::println("Num Deleted:           {}", m_numDeleted);
        std::println("Num Replaced:          {}", m_numReplaced);
        std::println("Num Skipped:           {}", m_numSkipped);
    }

private:
private:
    size_t m_numAdd               = 0;
    size_t m_numAddMPID           = 0;
    size_t m_numExecuted          = 0;
    size_t m_numExecutedWithPrice = 0;
    size_t m_numCanceled          = 0;
    size_t m_numDeleted           = 0;
    size_t m_numReplaced          = 0;
    size_t m_numSkipped           = 0;
};

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        std::println("Need to provide the path to the file");
        return 1;
    }

    silk::Result<silk::MappedFile> file = silk::Open(argv[1]);

    if (!file.has_value())
    {
        std::println("Failed to load file, errno {}", file.error().no);
        return 1;
    }

    std::println("Loaded File Successfully!");

    StdOutHandler handler;

    auto start = std::chrono::high_resolution_clock::now();
    silk::ParseFile<StdOutHandler>(*file, handler);
    auto end = std::chrono::high_resolution_clock::now();

    auto processingTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
            .count();

    handler.PrintResults();
    std::println("Took {}ms", processingTime);
    return 0;
}
