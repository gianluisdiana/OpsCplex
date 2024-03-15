#ifndef __JSON_INTERFACE_HPP__
#define __JSON_INTERFACE_HPP__

#include <nlohmann/json.hpp>

/** @brief Represents the basic interface to work with json files. */
class JsonInterface {
  /**
   * @brief Formats the instance to a json file
   *
   * @return The json file with the ops information
   */
  virtual const nlohmann::json toJson() const = 0;

  /**
   * @brief Set the instance from a json file
   *
   * @param json_object The json file with the information
   */
  virtual void setFromJson(const nlohmann::json &json_object) = 0;
};

#endif  // __JSON_INTERFACE_HPP__