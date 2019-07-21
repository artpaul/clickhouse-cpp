//
// Created by hj8088 on 19-7-21.
//
#pragma once

#include "column.h"
#include "numeric.h"

namespace clickhouse {

    using UInt128 = std::pair<uint64_t, uint64_t>;

    class ColumnIPv6 : public Column{
    public:
        ColumnIPv6();
        explicit ColumnIPv6(ColumnRef data);

        /// Appends one element to the column.
        void Append(const std::string& str);

        void Append(const char* addr, size_t len);

        /// Returns element at given row number.
        UInt128 At(size_t n) const;

        /// Returns element at given row number.
        UInt128 operator [] (size_t n) const;

        std::string to_string (size_t n) const;

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

        std::shared_ptr<ColumnUInt64> data_;
    };

}
