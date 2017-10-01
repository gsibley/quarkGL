#include <linux/limits.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

#include <qrk/shader_loader.h>
#include <qrk/utils.h>

namespace qrk {

std::string readFile(std::string const& path) {
  std::ifstream file;
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  file.open(path.c_str());
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

std::string resolvePath(std::string const& path) {
  char resolvedPath[PATH_MAX];
  realpath(path.c_str(), resolvedPath);
  return std::string(resolvedPath);
}

void ShaderLoader::checkShaderType(std::string const& shaderPath) {
  switch (shaderType_) {
    case ShaderType::VERTEX:
      if (!string_has_suffix(shaderPath, ".vert")) {
        throw ShaderLoaderException(
            "ERROR::SHADER_LOADER::INVALID_EXTENSION\n"
            "Vertex shader '" +
            shaderPath + "' must end in '.vert'.");
      }
      break;
    case ShaderType::FRAGMENT:
      if (!string_has_suffix(shaderPath, ".frag")) {
        throw ShaderLoaderException(
            "ERROR::SHADER_LOADER::INVALID_EXTENSION\n"
            "Fragment shader '" +
            shaderPath + "' must end in '.frag'.");
      }
      break;
  }
}

std::string ShaderLoader::getIncludesTraceback() {
  std::stringstream buffer;
  for (std::string path : includeChain_) {
    buffer << "  > " << path << std::endl;
  }
  return buffer.str();
}

bool ShaderLoader::checkCircularInclude(std::string const& resolvedPath) {
  for (std::string path : includeChain_) {
    if (path == resolvedPath) {
      return true;
    }
  }
  return false;
}

ShaderLoader::ShaderLoader(const char* shaderPath, const ShaderType shaderType)
    : shaderPath_(shaderPath), shaderType_(shaderType) {
  checkShaderType(shaderPath);
}

std::string ShaderLoader::load(std::string const& shaderPath) {
  checkShaderType(shaderPath);

  std::string shaderCode;
  try {
    shaderCode = readFile(shaderPath);
  } catch (std::ifstream::failure e) {
    std::string traceback = getIncludesTraceback();
    throw ShaderLoaderException(
        "ERROR::SHADER_LOADER::FILE_NOT_SUCCESSFULLY_READ\n"
        "Unable to read shader '" +
        std::string(shaderPath) + "', traceback below (most recent last):\n" +
        traceback);
  }

  return preprocessShader(shaderPath, shaderCode);
}

std::string ShaderLoader::preprocessShader(std::string const& shaderPath,
                                           std::string const& shaderCode) {
  std::string resolvedPath = resolvePath(shaderPath);
  includeChain_.push_back(resolvedPath);

  std::regex includePattern(
      R"((^|\r?\n)\s*#pragma\s+qrk_include\s+(".*"|<.*>)(?=\r?\n|$))");
  std::string processedCode = regex_replace(
      shaderCode, includePattern, [this, shaderPath](const std::smatch& m) {
        // Extract the path.
        std::string incl = m[2];
        char inclType = incl[0];
        std::string path = trim(incl.substr(1, incl.size() - 2));

        if (inclType == '<') {
          // qrk include.
          return load("quarkgl/shaders/" + path);
        } else {
          // Standard include.
          size_t i = shaderPath.find_last_of("/");
          // This will either be the current shader's directory, or empty
          // string if the current shader is at project root.
          std::string prefix =
              i != std::string::npos ? shaderPath.substr(0, i + 1) : "";
          return load(prefix + path);
        }
      });

  includeChain_.pop_back();
  return processedCode;
}

std::string ShaderLoader::load() { return load(shaderPath_); }
}  // namespace qrk
