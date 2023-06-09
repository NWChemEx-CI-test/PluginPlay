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

#include "database_api.hpp"
#include <parallelzone/serialization.hpp>

namespace pluginplay::cache::database {

/** @brief Extends an existing binary-based database's API to be object-oriented
 *
 *  Binary is a convient storage format; however the code using the cache is
 *  likely object-oriented. Serialization is typically used to go from an objet
 *  to a flattened view of the object (such as binary) and deserialization
 *  undoes the flattening. This class wraps a database which stores things in
 *  binary format so that it has an object-oriented API. In other words, users
 *  of this database only deal with objects. This class handles the
 *  serialization/deserialization of the objects under the hood.
 *
 *  @note This class should be thought of as a wrapper around the managed
 *        databse. In particular, this means this instance does not maintain
 *        separate records of the keys/values; rather, all interactions actually
 *        occur on the wrapped database (e.g., calling dump will actually call
 *        dump on the wrapped database too)
 *
 *  @tparam KeyType The type of the keys. Must be serializable.
 *  @tparam ValueType The type of the objects that the keys map to. Must be
 *                    serializable.
 */
template<typename KeyType, typename ValueType>
class Serialized : public DatabaseAPI<KeyType, ValueType> {
private:
    /// Type the class implements
    using base_type = DatabaseAPI<KeyType, ValueType>;

public:
    /// Type that objects get serialized into
    using binary_type = std::string;

    /// Type of the binary-based database API we wrapping
    using sub_db_type = DatabaseAPI<binary_type, binary_type>;

    /// Type of a managed pointer to an instance of sub_db_type
    using sub_db_pointer = std::unique_ptr<sub_db_type>;

    /// Typedef of KeyType
    using typename base_type::key_type;

    /// Ultimately a typedef of DatabaseAPI::key_set_type
    using typename base_type::key_set_type;

    /// Typedef of const key_type&
    using typename base_type::const_key_reference;

    /// Typedef of ValueType
    using typename base_type::mapped_type;

    /// Typedef of ConstValue<mapped_type>
    using typename base_type::const_mapped_reference;

    /** @brief Creates a new Serialized instance which wraps the provided
     *         binary-based database.
     *
     *  This ctor creates a new Serialized instance that is interfaced to the
     *  provided binary-based database. The resulting Serialized instance more
     *  or less functions as a translational layer from objects to binary, and
     *  binary to objects.
     *
     *  @param[in] p A smart pointer to the database to wrap. The user is
     *               expected to ensure that @p p is non-null. Using a null @p p
     *               will result in undefined behavior.
     *
     *  @throw None No throw guarantee.
     */
    Serialized(sub_db_pointer p) : m_db_(std::move(p)) {}

protected:
    /// returns a container with the deserialized keys
    key_set_type keys_() const override;

    /// Checks if wrapped db has serialized @p key
    bool count_(const_key_reference key) const noexcept override;

    /// Serializes @p key and @p value, adds to wrapped database
    void insert_(key_type key, mapped_type value) override;

    /// Frees (serialized) value associated with serialized @p key,
    void free_(const_key_reference key) override;

    /// Serializes @p key, gets serialized value, deserializes and returns value
    const_mapped_reference at_(const_key_reference key) const override;

    /// Implements backup by calling backup on the wrapped database
    void backup_() override { m_db_->backup(); }

    /// Implements dump by calling dump on the wrapped database
    void dump_() override { m_db_->dump(); }

private:
    /// Wraps the process of serializing an object of type @p T
    template<typename T>
    binary_type serialize_(T&& serialize_me) const;

    /// Wraps the process of deserializing to an object of type @p T
    template<typename T>
    T deserialize_(const binary_type& deserialize_me) const;

    /// The binary database we are serializing in to/out of
    sub_db_pointer m_db_;
};

} // namespace pluginplay::cache::database

#include "serialized.ipp"
