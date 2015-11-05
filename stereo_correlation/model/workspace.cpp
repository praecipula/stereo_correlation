#include "workspace.h"
#include "json/json.h"

namespace Stereo
{
  Workspace::Workspace()
  {

  }

  std::string serialize()
  {
      Json::Value root;
      root["version"] = 1.0;
      std::ostringstream stream;
      stream << root;
      return stream.str();
  }
}
