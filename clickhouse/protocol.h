#pragma once

namespace clickhouse {

/// Packet types that server transmits.
namespace ServerCodes {
    enum {
        /// Name, version, revision.
        Hello = 0,
        /// A block of data (compressed or not).
        Data = 1,
        /// An exception during query execution.
        Exception = 2,
        /// Query execution progress: rows read, bytes read.
        Progress = 3,
        /// Ping response.
        Pong = 4,
        /// All packets were transmitted.
        EndOfStream = 5,
        /// A packet with profiling info.
        ProfileInfo = 6,
        /// A block of data with totals (compressed or not).
        Totals = 7,
        /// A block of data with minimums and maximums (compressed or not).
        Extremes = 8,
    };
}

/// Packet types that client transmits.
namespace ClientCodes {
    enum {
        /// Name, version, revision, default DB.
        Hello = 0,
        /// Query id, query settings, stage up to which the query must be executed,
        /// whether the compression must be used,
        /// query text (without data for INSERTs).
        Query = 1,
        /// A block of data (compressed or not).
        Data = 2,
        /// Cancel the query execution.
        Cancel = 3,
         /// Check that the connection to the server is alive.
        Ping = 4,
    };
}

/// Whether the compression must be used.
namespace CompressionState {
    enum {
        Disable = 0,
        Enable = 1,
    };
}

namespace Stages {
    enum {
        Complete = 2,
    };
}

} // namespace clickhouse
