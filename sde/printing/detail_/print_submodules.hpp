#pragma once
#include "sde/printing/detail_/rest_printer.hpp"
#include "sde/submodule_request.hpp"
#include "sde/types.hpp"

namespace sde::printing::detail_ {

/** @brief Adds boiler plate description for the submodule section printer
 *
 *  This function contains the boiler plate introduction for introducing the
 *  the section describing the submodules that a module uses.
 *
 * @param[in,out] p The reSTPrinter to add the prose to. Assumed to be in the
 *                  correct section already.
 *
 * @return @p with its internal state modified to contain the intro to the
 *            input section.
 *
 * @throw ??? If the call to ostream::operator<< wrapped in @p throws given the
 *            description. Same guarantee.
 */
reSTPrinter& submod_desc(reSTPrinter& p);

/** @brief Makes a table of the submodules a module uses
 *
 *  This function will loop over the submodules and produce a table suitable for
 *  display in documentation. This table will include the key the submodule is
 *  assigned to, the property type it must satisfy, and a description of what
 *  the submodule is used for.
 *
 * @param[in] submods The submodule_map of submodule requests to table-ize.
 *
 * @return A string containing the formatted table.
 *
 * @throw std::bad_alloc if there is insufficient memory to parse the
 *                       submodules. Strong throw guarantee.
 * @throw ??? If utilities::Table throws during the process of turning the
 *            data into a table. Same throw guarantee.
 */
std::string submod_table(const type::submodule_map& submods);

/** @brief Driver for printing the submodules section of the documentation
 *
 *  This function will assemble and add to the provided printer the section
 *  detailing a module's submodules.
 *
 * @param[in,out] p The printer to add the prose to.
 * @param[in] submods The submodule_map containing the submodule requests that
 *                    the module specifies.
 *
 * @return @p with its internal state modified to store the submodules section.
 *
 * @throw std::bad_alloc if there is insufficient memory to parse the
 *                       submodules. Weak throw guarantee.
 *
 * @throw ??? if adding any of the strings to the printer or the table throws.
 *            Weak throw guarantee.
 */
reSTPrinter& print_submods(reSTPrinter& p, const type::submodule_map& submods);

} // namespace sde::printing::detail_
