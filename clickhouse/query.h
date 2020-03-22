#pragma once

#include "block.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace clickhouse {

/**
 * Settings of individual query.
 */
struct QuerySettings {
    /// A maximum number of threads for query execution. Determined automatically by default.
    int max_threads = 0;
    /// Calculate minimum and maximum values of each column.
    bool extremes = false;
    /// Silently skip unavailable shards.
    bool skip_unavailable_shards = false;
    /// Write statistics about read rows, bytes, time elapsed, etc.
    bool output_format_write_statistics = true;
    /// Use client timezone for interpreting DateTime string values, instead of adopting server timezone.
    bool use_client_time_zone = false;

    // connect_timeout
    // max_block_size
    // distributed_group_by_no_merge = false
    // strict_insert_defaults = 0
    // network_compression_method = LZ4
    // priority = 0
};

struct Exception {
    int code = 0;
    std::string name;
    std::string display_text;
    std::string stack_trace;
    /// Pointer to nested exception.
    std::unique_ptr<Exception> nested;
};

struct Profile {
    uint64_t rows = 0;
    uint64_t blocks = 0;
    uint64_t bytes = 0;
    uint64_t rows_before_limit = 0;
    bool applied_limit = false;
    bool calculated_rows_before_limit = false;
};

struct Progress {
    uint64_t rows = 0;
    uint64_t bytes = 0;
    uint64_t total_rows = 0;
};

class QueryEvents {
public:
    virtual ~QueryEvents() = default;

    /// Some data has been received.
    virtual void OnData(const Block& block) = 0;
    virtual bool OnDataCancelable(const Block& block) = 0;

    /// A block with extremes values has been received.
    virtual void OnExtremes(const Block& block) = 0;

    virtual void OnServerException(const Exception& e) = 0;

    virtual void OnProfile(const Profile& profile) = 0;

    virtual void OnProgress(const Progress& progress) = 0;

    virtual void OnFinish() = 0;

    /// A block with totals values has been received.
    virtual void OnTotals(const Block& block) = 0;
};

using DataCallback = std::function<void(const Block& block)>;
using ExceptionCallback = std::function<void(const Exception& e)>;
using ProgressCallback = std::function<void(const Progress& progress)>;
using SelectCallback = DataCallback;
using SelectCancelableCallback = std::function<bool(const Block& block)>;

class Query : public QueryEvents {
public:
     Query();
     Query(const char* query);
     Query(const std::string& query);
    ~Query() override;

    ///
    inline std::string GetText() const {
        return query_;
    }

    /// Set handler for receiving result data.
    inline Query& OnData(DataCallback cb) {
        select_cb_ = cb;
        return *this;
    }

    inline Query& OnDataCancelable(SelectCancelableCallback cb) {
        select_cancelable_cb_ = cb;
        return *this;
    }

    /// Set handler for receiving server's exception.
    inline Query& OnException(ExceptionCallback cb) {
        exception_cb_ = cb;
        return *this;
    }

    /// Set handler for receiving extremes values.
    inline Query& OnExtremes(DataCallback cb) {
        extremes_cb_ = cb;
        return *this;
    }

    /// Set handler for receiving a progress of query exceution.
    inline Query& OnProgress(ProgressCallback cb) {
        progress_cb_ = cb;
        return *this;
    }

    /// Set handler for receiving totals values.
    inline Query& OnTotals(DataCallback cb) {
        totals_cb_ = cb;
        return *this;
    }

private:
    void OnData(const Block& block) override;

    bool OnDataCancelable(const Block& block) override;

    void OnExtremes(const Block& block) override;

    void OnServerException(const Exception& e) override;

    void OnProfile(const Profile& profile) override;

    void OnProgress(const Progress& progress) override;

    void OnFinish() override;

    void OnTotals(const Block& block) override;

private:
    std::string query_;
    ExceptionCallback exception_cb_;
    ProgressCallback progress_cb_;
    DataCallback select_cb_;
    SelectCancelableCallback select_cancelable_cb_;
    DataCallback totals_cb_;
    DataCallback extremes_cb_;
};

} // namespace clickhouse
