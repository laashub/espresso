/*
 * Copyright (C) 2010-2019 The ESPResSo project
 * Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010
 *   Max-Planck-Institute for Polymer Research, Theory Group
 *
 * This file is part of ESPResSo.
 *
 * ESPResSo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ESPResSo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SCRIPT_INTERFACE_OBSERVABLES_CYLINDRICALLBPROFILEOBSERVABLE_HPP
#define SCRIPT_INTERFACE_OBSERVABLES_CYLINDRICALLBPROFILEOBSERVABLE_HPP

#include "script_interface/auto_parameters/AutoParameters.hpp"

#include <memory>

#include "Observable.hpp"
#include "core/observables/CylindricalLBProfileObservable.hpp"
#include "script_interface/get_value.hpp"

namespace ScriptInterface {
namespace Observables {

template <typename CoreCylLBObs>
class CylindricalLBProfileObservable
    : public AutoParameters<CylindricalLBProfileObservable<CoreCylLBObs>,
                            Observable> {
public:
  static_assert(std::is_base_of<::Observables::CylindricalLBProfileObservable,
                                CoreCylLBObs>::value,
                "");
  CylindricalLBProfileObservable() {
    this->add_parameters({
        {"center",
         [this](const Variant &v) {
           cylindrical_profile_observable()->center =
               get_value<::Utils::Vector3d>(v);
         },
         [this]() { return cylindrical_profile_observable()->center; }},
        {"axis",
         [this](const Variant &v) {
           cylindrical_profile_observable()->axis =
               get_value<Utils::Vector3d>(v);
         },
         [this]() { return cylindrical_profile_observable()->axis; }},
        {"n_r_bins",
         [this](const Variant &v) {
           cylindrical_profile_observable()->n_r_bins =
               static_cast<size_t>(get_value<int>(v));
         },
         [this]() {
           return static_cast<int>(cylindrical_profile_observable()->n_r_bins);
         }},
        {"n_phi_bins",
         [this](const Variant &v) {
           cylindrical_profile_observable()->n_phi_bins =
               static_cast<size_t>(get_value<int>(v));
         },
         [this]() {
           return static_cast<int>(
               cylindrical_profile_observable()->n_phi_bins);
         }},
        {"n_z_bins",
         [this](const Variant &v) {
           cylindrical_profile_observable()->n_z_bins =
               static_cast<size_t>(get_value<int>(v));
         },
         [this]() {
           return static_cast<int>(cylindrical_profile_observable()->n_z_bins);
         }},
        {"min_r",
         [this](const Variant &v) {
           cylindrical_profile_observable()->min_r = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->min_r; }},
        {"min_phi",
         [this](const Variant &v) {
           cylindrical_profile_observable()->min_phi = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->min_phi; }},
        {"min_z",
         [this](const Variant &v) {
           cylindrical_profile_observable()->min_z = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->min_z; }},
        {"max_r",
         [this](const Variant &v) {
           cylindrical_profile_observable()->max_r = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->max_r; }},
        {"max_phi",
         [this](const Variant &v) {
           cylindrical_profile_observable()->max_phi = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->max_phi; }},
        {"max_z",
         [this](const Variant &v) {
           cylindrical_profile_observable()->max_z = get_value<double>(v);
         },
         [this]() { return cylindrical_profile_observable()->max_z; }},
        {"sampling_density",
         [this](const Variant &v) {
           cylindrical_profile_observable()->sampling_density =
               get_value<double>(v);
         },
         [this]() {
           return cylindrical_profile_observable()->sampling_density;
         }},
    });
  }

  void construct(VariantMap const &params) override {
    m_observable =
        make_shared_from_args<CoreCylLBObs, Utils::Vector3d, Utils::Vector3d,
                              int, int, int, double, double, double, double,
                              double, double, double>(
            params, "center", "axis", "n_r_bins", "n_phi_bins", "n_z_bins",
            "min_r", "min_phi", "min_z", "max_r", "max_phi", "max_z",
            "sampling_density");
  }

  Variant call_method(std::string const &method,
                      VariantMap const &parameters) override {
    if (method == "calculate") {
      return cylindrical_profile_observable()->operator()();
    }
    if (method == "shape") {
      auto const shape = cylindrical_profile_observable()->shape();
      return std::vector<int>{shape.begin(), shape.end()};
    }
    return {};
  }

  std::shared_ptr<::Observables::Observable> observable() const override {
    return m_observable;
  }

  virtual std::shared_ptr<::Observables::CylindricalLBProfileObservable>
  cylindrical_profile_observable() const {
    return m_observable;
  }

private:
  std::shared_ptr<CoreCylLBObs> m_observable;
};

} /* namespace Observables */
} /* namespace ScriptInterface */

#endif
