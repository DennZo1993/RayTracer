#pragma once

#include <unordered_map>
#include <string>
#include "Material.h"

class MaterialManager {
  MaterialManager() {}
  MaterialManager(const MaterialManager &other) = delete;
  MaterialManager& operator= (const MaterialManager &other) = delete;

  void AddMaterial(const std::string &name, const Material &mat) {
    materials[name] = mat;
  }

  const Material &GetMaterialByName(const std::string &name) const {
    auto it = materials.find(name);
    assert(it != materials.end() && "Material not found!");
    return it->second;
  }

private:
  std::unordered_map<std::string, Material> materials;
};
