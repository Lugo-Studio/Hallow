//
// Created by galex on 5/18/2021.
//

#ifndef HALLOW_RENDEREROPTIONS_HPP
#define HALLOW_RENDEREROPTIONS_HPP

namespace Hallow {
  struct RendererOptions {
    bool using_srgb_color_space{false};
    bool using_vsync{false};
  };
}

#endif //HALLOW_RENDEREROPTIONS_HPP