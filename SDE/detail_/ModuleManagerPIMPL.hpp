#pragma once
#include "SDE/ModuleBase.hpp"
#include "SDE/detail_/ModulePIMPL.hpp"
#include <memory>
#include <typeindex>

namespace SDE {
namespace detail_ {

/** @brief The class that implements the ModuleManager.
 *
 *  Users are expected to go through the API provided by the ModuleManager
 *  class. Inside the ModuleManager class calls are redirected to this class.
 *  When using this class calls should go through the member functions as much
 *  as possible so that error checking occurs. That said members are public so
 *  that it is easier to test the implementation.
 */
struct ModuleManagerPIMPL {
    ///@{
    /// Type of a pointer to a module's implemenation
    using module_base_ptr = std::shared_ptr<const ModuleBase>;

    /// Type of a map from the module implementation's type to the
    /// implementation
    using base_map = std::map<std::type_index, module_base_ptr>;

    /// Type of a usable module
    using shared_module = std::shared_ptr<Module>;

    /// Type of a map holding usable modules
    using module_map = Utilities::CaseInsensitiveMap<std::shared_ptr<Module>>;

    /// Type of a cache
    using cache_type = typename ModulePIMPL::cache_type;

    /// Type of a shared cache
    using shared_cache = std::shared_ptr<cache_type>;

    /// Type of a map holding caches
    using cache_map = std::map<std::type_index, shared_cache>;

    /// Type of a map holding the default module key for a given property type
    using default_map = std::map<std::type_index, type::key>;
    ///@}

    /// Makes a deep copy of this instance on the heap
    auto clone() { return std::make_unique<ModuleManagerPIMPL>(*this); }

    /// Ensures we determine if we have a module consistently
    type::size count(const type::key& key) const noexcept {
        return m_modules.count(key);
    }

    /// Ensures we count the number of modules consistently
    type::size size() const noexcept { return m_modules.size(); }

    /** @brief Sets the default module to use for a given property type
     *
     * When a user requests a module that module initially has no submodules
     * set unless the user has bound some to that module. Instead, the
     * ModuleManager is now responsible to provide the module with submodules
     * using the defaults. This function allows one to specify what the
     * defaults are.
     *
     * @param type The type of the property type this default is for
     * @param key The module key for the module to use as the default
     */
    void set_default(const std::type_info& type, type::input_map inputs,
                     type::key key) {
        if(!count(key)) m_modules.at(key); // Throws a consistent error
        m_defaults[std::type_index(type)] = key;
        m_inputs[std::type_index(type)]   = std::move(inputs);
    }

    /** @brief This function actually adds a module to the list of available
     *         modules.
     *
     * @param key The key under which the module will be registered.
     * @param base The instance containing the algorithm
     */
    void add_module(type::key key, module_base_ptr base) {
        assert_unique_key_(key);

        std::type_index type(base->type());
        if(!m_bases.count(type)) m_bases[type] = base;
        if(!m_caches.count(type))
            m_caches[type] = std::make_shared<cache_type>();
        auto pimpl =
          std::make_unique<ModulePIMPL>(m_bases[type], m_caches[type]);
        auto ptr = std::make_shared<Module>(std::move(pimpl));
        m_modules.emplace(std::move(key), ptr);
    }

    /** @brief Makes a deep copy of a module
     *
     * This function makes a deep copy of a module. The new module is unlocked
     * regardless of whether the old module was locked or not. The user can call
     * lock on the resulting module to make an exact copy
     * @param old_key The key for the module to copy
     * @param new_key The key under which the new module will live
     */
    void copy_module(const type::key& old_key, type::key new_key) {
        assert_unique_key_(new_key);
        Module mod = std::move(m_modules.at(old_key)->unlocked_copy());
        auto ptr   = std::make_shared<Module>(std::move(mod));
        m_modules.emplace(std::move(new_key), ptr);
    }

    /** @brief Returns a module, filling in all non-set submodules with defaults
     *         if a ready default exists.
     *
     * @param key The module you want
     * @return A shared_ptr to the requested module
     */
    shared_module at(const type::key& key) {
        auto mod = m_modules.at(key);
        // Loop over submodules filling them in from the defaults
        for(auto & [k, v] : mod->submods()) {
            const auto& type = v.type();
            // Only change non-ready submodules
            if(!v.ready() && m_defaults.count(type)) {
                // Recursive to make sure that that module gets filled in
                auto default_mod = at(m_defaults.at(type));
                // Only change if the module is also ready
                if(default_mod->ready(m_inputs.at(type)))
                    mod->change_submod(k, default_mod);
            }
        }
        return mod;
    }

    ///@{
    /** @name Comparison operators
     *
     * @param rhs
     * @return
     */
    bool operator==(const ModuleManagerPIMPL& rhs) const {
        // Try to get out early
        if(m_bases.size() != rhs.m_bases.size()) return false;
        if(m_modules.size() != rhs.m_modules.size()) return false;
        if(m_defaults.size() != rhs.m_defaults.size()) return false;

        // Skip checking the values b/c implementations are compared by type
        for(const auto & [k, v] : rhs.m_bases) {
            if(!m_bases.count(k)) return false;
        }

        // Need to check the values b/c user may have switched options
        for(const auto & [k, v] : rhs.m_modules) {
            if(!m_modules.count(k)) return false;
            if(*m_modules.at(k) != *v) return false;
        }

        // Easy since not pointers
        if(m_defaults != rhs.m_defaults) return false;

        return true;
    }

    bool operator!=(const ModuleManagerPIMPL& rhs) const {
        return !((*this) == rhs);
    }
    ///@}

    ///@{
    /** @name ModuleManager state
     *
     *  The members in this section are the state of the ModuleManager class.
     */
    // These are the Modules in the states provided to us by the developer
    base_map m_bases;

    // These are the Modules in the state set by the user
    module_map m_modules;

    // These are the results of the modules running in the user's states
    cache_map m_caches;

    // A map of property types
    default_map m_defaults;

    // A map of inputs for property types
    std::map<std::type_index, type::input_map> m_inputs;
    ///@}
private:
    /// Wraps the check for making sure @p key is not in use.
    void assert_unique_key_(const type::key& key) const {
        if(count(key)) throw std::invalid_argument("Key is in use");
    }
};

} // namespace detail_
} // namespace SDE