#pragma once

#include "numeric.h"

#include <ctime>

namespace clickhouse {

/** */
class ColumnDate : public Column {
public:
    ColumnDate();

    /// Appends one element to the end of column.
    void Append(const std::time_t& value);

    /// Returns element at given row number.
    std::time_t At(size_t n) const;

public:
    /// Appends content of given column to the end of current one.
    void Append(ColumnRef column) override;

    /// Loads column data from input stream.
    bool Load(CodedInputStream* input, size_t rows) override;

    /// Saves column data to output stream.
    void Save(CodedOutputStream* output) override;

    /// Clear column data .
    void Clear() override;

    /// Returns count of rows in the column.
    size_t Size() const override;

    /// Makes slice of the current column.
    ColumnRef Slice(size_t begin, size_t len) override;

private:
    std::shared_ptr<ColumnUInt16> data_;
};

/** */
class ColumnDateTime : public Column {
public:
    ColumnDateTime();
    explicit ColumnDateTime(std::string timezone);

    /// Appends one element to the end of column.
    void Append(const std::time_t& value);

    /// Returns element at given row number.
    std::time_t At(size_t n) const;

    /// Timezone associated with a data column.
    std::string Timezone() const;

public:
    /// Appends content of given column to the end of current one.
    void Append(ColumnRef column) override;

    /// Loads column data from input stream.
    bool Load(CodedInputStream* input, size_t rows) override;

    /// Clear column data .
    void Clear() override;

    /// Saves column data to output stream.
    void Save(CodedOutputStream* output) override;

    /// Returns count of rows in the column.
    size_t Size() const override;

    /// Makes slice of the current column.
    ColumnRef Slice(size_t begin, size_t len) override;

private:
    std::shared_ptr<ColumnUInt32> data_;
};

/** */
class ColumnDateTime64 : public Column {
public:
    explicit ColumnDateTime64(size_t precision);
    ColumnDateTime64(size_t precision, std::string timezone);

    /// Appends one element to the end of column.
    void Append(const uint64_t& value);

    /// Returns element at given row number.
    uint64_t At(size_t n) const;

    /// Timezone associated with a data column.
    std::string Timezone() const;

public:
    /// Appends content of given column to the end of current one.
    void Append(ColumnRef column) override;

    /// Loads column data from input stream.
    bool Load(CodedInputStream* input, size_t rows) override;

    /// Clear column data .
    void Clear() override;

    /// Saves column data to output stream.
    void Save(CodedOutputStream* output) override;

    /// Returns count of rows in the column.
    size_t Size() const override;

    /// Makes slice of the current column.
    ColumnRef Slice(size_t begin, size_t len) override;

private:
    std::shared_ptr<ColumnUInt64> data_;

    explicit ColumnDateTime64(TypeRef type, std::shared_ptr<ColumnUInt64> data); // for `Slice(…)`
};

}
