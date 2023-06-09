/*
 * Copyright 2022 NWChemEx-Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../rocksdb.hpp"
#include <stdexcept>

namespace pluginplay::cache::database::detail_ {

/** @brief This class is a stub API for the RocksDBPIMPLStub
 *
 *  If a user chooses not to enable RocksDB they get this definition of the
 *  RocksDBPIMPLStub. This class has the same API as the real RocksDBPIMPLStub
 * so classes can use it without preprocessor protection. Attempting to actually
 *  instantiate an instance of this class will raise a runtime error. For good
 *  measure all functions are defined to throw as well; however, it shouldn't
 *  be possible to call any of them since you can't make an instance of this
 *  class.
 */
class RocksDBPIMPLStub {
public:
    /// The type of the database which relies on this PIMPL
    using parent_type = RocksDB<std::string, std::string>;

    /// Type of an immutable reference to the path where the database should go
    using const_path_reference = typename parent_type::const_path_reference;

    /// Type used for keys in the database
    using key_type = typename parent_type::key_type;

    /// Type of an immutable reference to a key
    using const_key_reference = typename parent_type::const_key_reference;

    /// Type of the values in the database
    using mapped_type = typename parent_type::mapped_type;

    /// Type of a mutable value
    using mapped_reference = mapped_type&;

    /// Type of
    using const_mapped_reference = typename parent_type::const_mapped_reference;

    /// Raises runtime_error if called
    RocksDBPIMPLStub(const_path_reference) { raise_error_(); }

    /// Raises runtime_error if called
    bool count(const_key_reference) const;

    /// Rasies runtime_error if called
    void insert(key_type, mapped_type) { raise_error_(); }

    /// Raises runtime_error if called
    void free(const_key_reference) { raise_error_(); }

    /// Raises runtime_error if called
    const_mapped_reference at(const_key_reference) const;

private:
    /// Code factorization for raising the runtime_error
    void raise_error_() const;
};

// -----------------------------------------------------------------------------
// -- Inline Implementations
// -----------------------------------------------------------------------------
inline bool RocksDBPIMPLStub::count(const_key_reference) const {
    raise_error_();
    return false;
}

inline typename RocksDBPIMPLStub::const_mapped_reference RocksDBPIMPLStub::at(
  const_key_reference) const {
    raise_error_();
    return const_mapped_reference{mapped_type{}};
}

inline void RocksDBPIMPLStub::raise_error_() const {
    throw std::runtime_error("PluginPlay was not compiled with RocksDB "
                             "support. To use RocksDB as a database rebuild "
                             "PluginPlay with the CMake option BUILD_ROCKSDB "
                             "set to ON.");
}

} // namespace pluginplay::cache::database::detail_
