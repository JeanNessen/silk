#pragma once

#include <print>

#include <slk/core/types.h>

class StdOutHandler final
{
public:
    void OnAdd(slk::OrderAdd) { ++m_numAdd; }
    void OnAddMPID(slk::OrderAddMPID) { ++m_numAddMPID; }
    void OnExecuted(slk::OrderExecuted) { ++m_numExecuted; }
    void OnExecutedWithPrice(slk::OrderExecutedWithPrice)
    {
        ++m_numExecutedWithPrice;
    }
    void OnCanceled(slk::OrderCanceled) { ++m_numCanceled; }
    void OnDeleted(slk::OrderDeleted) { ++m_numDeleted; }
    void OnReplaced(slk::OrderReplaced) { ++m_numReplaced; }
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
