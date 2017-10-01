#ifndef QUARKGL_SHADER_LOADER_H_
#define QUARKGL_SHADER_LOADER_H_

#include <deque>
#include <string>

#include <qrk/shared.h>

namespace qrk {

class ShaderLoaderException : public QuarkException {
  using QuarkException::QuarkException;
};

enum class ShaderType {
  VERTEX,
  FRAGMENT,
};

class ShaderLoader {
 private:
  const char* shaderPath_;
  const ShaderType shaderType_;
  std::deque<std::string> includeChain_;

  void checkShaderType(std::string const& shaderPath);
  std::string load(std::string const& shaderPath);
  std::string preprocessShader(std::string const& shaderPath,
                               std::string const& shaderCode);
  std::string getIncludesTraceback();
  bool checkCircularInclude(std::string const& resolvedPath);

 public:
  ShaderLoader(const char* shaderPath, const ShaderType type);
  std::string load();
};
}  // namespace qrk

#endif
