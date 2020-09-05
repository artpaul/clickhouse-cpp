#include "date.h"

namespace clickhouse {

ColumnDate::ColumnDate()
    : Column(Type::CreateDate())
    , data_(std::make_shared<ColumnUInt16>())
{
}

void ColumnDate::Append(const std::time_t& value) {
    data_->Append(static_cast<uint16_t>(value / 86400));
}

void ColumnDate::Clear() {
    data_->Clear();
}

std::time_t ColumnDate::At(size_t n) const {
    return static_cast<std::time_t>(data_->At(n)) * 86400;
}

void ColumnDate::Append(ColumnRef column) {
    if (auto col = column->As<ColumnDate>()) {
        data_->Append(col->data_);
    }
}

bool ColumnDate::Load(CodedInputStream* input, size_t rows) {
    return data_->Load(input, rows);
}

void ColumnDate::Save(CodedOutputStream* output) {
    data_->Save(output);
}

size_t ColumnDate::Size() const {
    return data_->Size();
}

ColumnRef ColumnDate::Slice(size_t begin, size_t len) {
    auto col = data_->Slice(begin, len)->As<ColumnUInt16>();
    auto result = std::make_shared<ColumnDate>();

    result->data_->Append(col);

    return result;
}


ColumnDateTime::ColumnDateTime()
    : Column(Type::CreateDateTime())
    , data_(std::make_shared<ColumnUInt32>())
{
}

void ColumnDateTime::Append(const std::time_t& value) {
    data_->Append(static_cast<uint32_t>(value));
}

std::time_t ColumnDateTime::At(size_t n) const {
    return data_->At(n);
}

void ColumnDateTime::Append(ColumnRef column) {
    if (auto col = column->As<ColumnDateTime>()) {
        data_->Append(col->data_);
    }
}

bool ColumnDateTime::Load(CodedInputStream* input, size_t rows) {
    return data_->Load(input, rows);
}

void ColumnDateTime::Save(CodedOutputStream* output) {
    data_->Save(output);
}

size_t ColumnDateTime::Size() const {
    return data_->Size();
}

void ColumnDateTime::Clear() {
    data_->Clear();
}

ColumnRef ColumnDateTime::Slice(size_t begin, size_t len) {
    auto col = data_->Slice(begin, len)->As<ColumnUInt32>();
    auto result = std::make_shared<ColumnDateTime>();

    result->data_->Append(col);

    return result;
}


ColumnDateTime64::ColumnDateTime64(size_t precision)
        : Column(Type::CreateDateTime64(precision))
        , data_(std::make_shared<ColumnUInt64>())
{
}

ColumnDateTime64::ColumnDateTime64(TypeRef type, std::shared_ptr<ColumnUInt64> data)
        : Column(type)
        , data_(std::move(data))
{
}

void ColumnDateTime64::Append(const uint64_t& value) {
    data_->Append(value);
}

uint64_t ColumnDateTime64::At(size_t n) const {
    return data_->At(n);
}

void ColumnDateTime64::Append(ColumnRef column) {
    if (auto col = column->As<ColumnDateTime64>()) {
        data_->Append(col->data_);
    }
}

bool ColumnDateTime64::Load(CodedInputStream* input, size_t rows) {
    return data_->Load(input, rows);
}

void ColumnDateTime64::Save(CodedOutputStream* output) {
    data_->Save(output);
}

size_t ColumnDateTime64::Size() const {
    return data_->Size();
}

void ColumnDateTime64::Clear() {
    data_->Clear();
}

ColumnRef ColumnDateTime64::Slice(size_t begin, size_t len) {
    auto col = data_->Slice(begin, len)->As<ColumnUInt64>();
    std::shared_ptr<ColumnDateTime64> result(new ColumnDateTime64(type_, std::move(col)));
    return result;
}

}
