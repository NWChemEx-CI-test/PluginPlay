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

// This file is meant only for inclusion in key_injector.hpp

namespace pluginplay::cache::database {

#define TPARAMS template<typename KeyType, typename ValueType>
#define KEY_INJECTOR KeyInjector<KeyType, ValueType>

TPARAMS
KEY_INJECTOR::KeyInjector(key_key_type key_to_inject,
                          key_value_type value_to_inject,
                          sub_db_pointer sub_db) :
  m_key_to_inject_(std::move(key_to_inject)),
  m_value_to_inject_(std::move(value_to_inject)),
  m_db_(sub_db) {
    if(sub_db) return;
    throw std::runtime_error("Expected a non-null database to wrap.");
}

TPARAMS
typename KEY_INJECTOR::key_set_type KEY_INJECTOR::keys_() const {
    auto rv = m_db_->keys();
    for(auto& key : rv) key.erase(m_key_to_inject_);
    return rv;
}

TPARAMS
bool KEY_INJECTOR::count_(const_key_reference key) const noexcept {
    // This actually can throw if there's not enough memory
    try {
        return m_db_->count(inject_(key));
    } catch(...) {
        std::cerr << "An uncaught exception was raised in KeyInjector::count_";
        std::abort();
    }
}

TPARAMS
void KEY_INJECTOR::insert_(key_type key, mapped_type value) {
    m_db_->insert(inject_(std::move(key)), std::move(value));
}

TPARAMS
void KEY_INJECTOR::free_(const_key_reference key) { m_db_->free(inject_(key)); }

TPARAMS
typename KEY_INJECTOR::const_mapped_reference KEY_INJECTOR::at_(
  const_key_reference key) const {
    return m_db_->at(inject_(key));
}

TPARAMS
typename KEY_INJECTOR::key_type KEY_INJECTOR::inject_(key_type key) const {
    key.emplace(m_key_to_inject_, m_value_to_inject_);
    return std::move(key);
}

#undef KEY_INJECTOR
#undef TPARAMS

} // namespace pluginplay::cache::database
