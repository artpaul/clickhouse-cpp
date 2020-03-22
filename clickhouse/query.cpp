#include "query.h"

namespace clickhouse {

Query::Query()
{ }

Query::Query(const char* query)
    : query_(query)
{
}

Query::Query(const std::string& query)
    : query_(query)
{
}

Query::~Query()
{ }

void Query::OnData(const Block& block) {
    if (select_cb_) {
        select_cb_(block);
    }
}

bool Query::OnDataCancelable(const Block& block) {
    if (select_cancelable_cb_) {
        return select_cancelable_cb_(block);
    } else {
        return true;
    }
}

void Query::OnExtremes(const Block& block) {
    if (extremes_cb_) {
        extremes_cb_(block);
    }
}

void Query::OnServerException(const Exception& e) {
    if (exception_cb_) {
        exception_cb_(e);
    }
}

void Query::OnProfile(const Profile& profile) {
    (void)profile;
}

void Query::OnProgress(const Progress& progress) {
    if (progress_cb_) {
        progress_cb_(progress);
    }
}

void Query::OnFinish() {
}

void Query::OnTotals(const Block& block) {
    if (totals_cb_) {
        totals_cb_(block);
    }
}

} // namespace clickhouse
